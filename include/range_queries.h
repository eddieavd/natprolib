//
//  range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <memory>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include <concepts>
#include <utility>

#include <__split_buffer>

#include "util.h"

#ifdef _WIN32
#   include <windows.h>
#else
#   include <unistd.h>
#endif

#define DEFAULT_CAPACITY 16


namespace natprolib
{

namespace range_queries
{


template< bool >
class _prefix_array_base_common
{
protected:
	__attribute__((__visibility__("hidden"))) __attribute__((internal_linkage))
	_prefix_array_base_common () {}

	[[ noreturn ]] void _throw_length_error () const;
	[[ noreturn ]] void _throw_out_of_range () const;
};

template< bool B >
void _prefix_array_base_common< B >::_throw_length_error () const
{
	std::__throw_length_error( "prefix_array" );
}
template< bool B >
void _prefix_array_base_common< B >::_throw_out_of_range () const
{
	std::__throw_out_of_range( "prefix_array" );
}

extern template class _prefix_array_base_common< true >;


template< typename T, typename Allocator = std::allocator< T > >
class _prefix_array_base
	: protected _prefix_array_base_common< true >
{
public:
	using  allocator_type = Allocator;
	using   _alloc_traits = std::allocator_traits< allocator_type >;
	using       size_type = typename _alloc_traits::size_type;
protected:
	using      value_type = T;
	using difference_type = typename _alloc_traits::difference_type;
	using       reference = value_type &;
	using const_reference = value_type const &;
	using         pointer = typename _alloc_traits::pointer;
	using   const_pointer = typename _alloc_traits::const_pointer;
	using        iterator = pointer;  //  hmm
	using  const_iterator = const_pointer;  //  hmm

	pointer begin_;
	pointer end_;
	_VSTD::__compressed_pair< pointer, allocator_type > end_cap_;

	allocator_type       & _alloc ()       noexcept { return end_cap_.second(); }
	allocator_type const & _alloc () const noexcept { return end_cap_.second(); }

	pointer       & _end_cap ()       noexcept { return end_cap_.first(); }
	pointer const & _end_cap () const noexcept { return end_cap_.first(); }

	_prefix_array_base (                             ) noexcept( std::is_nothrow_default_constructible_v< allocator_type > );
	_prefix_array_base ( allocator_type const &  _a_ );
	_prefix_array_base ( allocator_type       && _a_ ) noexcept;

	~_prefix_array_base ();

	void clear () noexcept { _destruct_at_end( begin_ ); }

	size_type capacity () const noexcept
	{ return static_cast< size_type >( _end_cap() - begin_ ); }

	void _destruct_at_end ( pointer _new_last_ ) noexcept;

	void _copy_assign_alloc ( _prefix_array_base const & _c_ )
	{
		_copy_assign_alloc( _c_, std::integral_constant< bool,
				_alloc_traits::propagate_on_container_copy_assignment::value >() );
	}

	void _move_assign_alloc ( _prefix_array_base & _c_ )
		noexcept(
				!_alloc_traits::propagate_on_container_move_assignment::value ||
				std::is_nothrow_move_assignable_v< allocator_type >
			)
	{
		_move_assign_alloc( _c_, std::integral_constant< bool,
				_alloc_traits::propagate_on_container_move_assignment::value >() );
	}

	[[ noreturn ]] void _throw_length_error () const
	{
#ifndef _LIBCPP_NO_EXCEPTIONS
//		_prefix_array_base_common< true >::_throw_length_error();
		throw std::length_error( "_prefix_array_base" );
#else
		std::abort();
#endif
	}

	[[ noreturn ]] void _throw_out_of_range () const
	{
#ifndef _LIBCPP_NO_EXCEPTIONS
//		_prefix_array_base_common< true >::_throw_out_of_range();
		throw std::out_of_range( "_prefix_array_base" );
#else
		std::abort();
#endif
	}

private:
	void _copy_assign_alloc ( _prefix_array_base const & _c_, std::true_type )
	{
		if( _alloc() != _c_._alloc() )
		{
			clear();
			_alloc_traits::deallocate( _alloc(), begin_, capacity() );
			begin_ = end_ = end_cap_() = nullptr;
		}
		_alloc() = _c_._alloc();
	}

	void _copy_assign_alloc ( _prefix_array_base const &, std::false_type ) {}

	void _move_assign_alloc ( _prefix_array_base & _c_, std::true_type )
		noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
	{
		_alloc() = std::move( _c_._alloc() );
	}

	void _move_assign_alloc ( _prefix_array_base &, std::false_type ) noexcept {}
};

template< typename T, class Allocator >
inline
void _prefix_array_base< T, Allocator >::_destruct_at_end ( pointer _new_last_ ) noexcept
{
	pointer soon_to_be_end = end_;
	while( _new_last_ != soon_to_be_end )
	{
		_alloc_traits::destroy( _alloc(), _VSTD::__to_address( --soon_to_be_end ) );
	}
	end_ = _new_last_;
}

template< typename T, typename Allocator >
inline
_prefix_array_base< T, Allocator >::_prefix_array_base ()
	noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, std::__default_init_tag() ) {}

template< typename T, typename Allocator >
inline
_prefix_array_base< T, Allocator >::_prefix_array_base ( allocator_type const & _a_ )
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, _a_ ) {}

template< typename T, typename Allocator >
inline
_prefix_array_base< T, Allocator >::_prefix_array_base ( allocator_type && _a_ ) noexcept
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, std::move( _a_ ) ) {}

template< typename T, typename Allocator >
_prefix_array_base< T, Allocator >::~_prefix_array_base ()
{
	if( begin_ != nullptr )
	{
		clear();
		_alloc_traits::deallocate( _alloc(), begin_, capacity() );
	}
}

template< typename T, typename Allocator = std::allocator< T > >
class prefix_array
	: private _prefix_array_base< T, Allocator >
{
private:
	using _base                    = _prefix_array_base< T, Allocator >;
	using _default_allocator_type  = std::allocator< T >;
public:
	using _self                    = prefix_array;
	using value_type               = T;
	using allocator_type           = Allocator;
	using _alloc_traits            = typename _base::_alloc_traits;
	using       reference          = typename _base::reference;
	using const_reference          = typename _base::const_reference;
	using size_type                = typename _base::size_type;
	using difference_type          = typename _base::difference_type;
	using       pointer            = typename _base::pointer;
	using const_pointer            = typename _base::const_pointer;

	using               iterator = std::__wrap_iter<        pointer >;
	using         const_iterator = std::__wrap_iter<  const_pointer >;
	using       reverse_iterator = std::reverse_iterator<       iterator >;
	using const_reverse_iterator = std::reverse_iterator< const_iterator >;

	static_assert( ( std::is_same_v< typename allocator_type::value_type, value_type > ),
			"Allocator::value_type != value_type" );

	prefix_array () noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
	{
#if _NPL_DEBUG_LEVEL == 2
#endif
	}

	explicit prefix_array ( allocator_type const & _a_ ) noexcept : _base( _a_ )
	{
#if _NPL_DEBUG_LEVEL == 2
#endif
	}

	explicit prefix_array ( size_type _n_ );
	explicit prefix_array ( size_type _n_, allocator_type const & _a_ );

	prefix_array ( size_type _n_, value_type const & _x_                             );
	prefix_array ( size_type _n_, value_type const & _x_, allocator_type const & _a_ );

	template< typename InputIterator >
	prefix_array ( InputIterator _first_,
			typename std::enable_if_t< std::__is_cpp17_input_iterator  < InputIterator >::value &&
			                          !std::__is_cpp17_forward_iterator< InputIterator >::value &&
						   std::is_constructible_v<
							value_type,
							typename std::iterator_traits< InputIterator >::reference
						>,
						InputIterator > _last_ );

	template< typename InputIterator >
	prefix_array ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
			typename std::enable_if_t< std::__is_cpp17_input_iterator  < InputIterator >::value &&
			                          !std::__is_cpp17_forward_iterator< InputIterator >::value &&
						   std::is_constructible_v<
						 	value_type,
							typename std::iterator_traits< InputIterator >::reference
						> > * = 0 );
	template< typename ForwardIterator >
	prefix_array ( ForwardIterator _first_,
			typename std::enable_if_t< std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
						   std::is_constructible_v<
						 	value_type,
							typename std::iterator_traits< ForwardIterator >:: reference
						>,
						ForwardIterator > _last_ );

	template< typename ForwardIterator >
	prefix_array ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
			typename std::enable_if_t< std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
						   std::is_constructible_v<
						 	value_type,
							typename std::iterator_traits< ForwardIterator >::reference
						> > * = 0 );

	~prefix_array ()
	{
		_annotate_delete();
#if _NPL_DEBUG_LEVEL == 2
#endif
	}

	prefix_array ( prefix_array const & _x_ );
	prefix_array ( prefix_array const & _x_, std::__identity< allocator_type > const & _a_ );

	prefix_array & operator= ( prefix_array const & _x_ );

	prefix_array ( std::initializer_list< value_type > _list_                             );
	prefix_array ( std::initializer_list< value_type > _list_, allocator_type const & _a_ );

	prefix_array ( prefix_array && _x_                                                ) noexcept;
	prefix_array ( prefix_array && _x_, std::__identity< allocator_type > const & _a_ );

	prefix_array & operator= ( prefix_array && _x_ )
		noexcept(( std::__noexcept_move_assign_container< Allocator, _alloc_traits >::value ));

	prefix_array & operator= ( std::initializer_list< value_type > _list_ )
	{ assign( _list_.begin(), _list_.end() ); return *this; }

	template< typename InputIterator >
		typename std::enable_if_t
		<
			 std::__is_cpp17_input_iterator  < InputIterator >::value &&
			!std::__is_cpp17_forward_iterator< InputIterator >::value &&
			 std::is_constructible_v<
			 	value_type,
				typename std::iterator_traits< InputIterator >::reference
			>,
			void
		>
	assign ( InputIterator _first_, InputIterator _last_ );

	template< typename ForwardIterator >
		typename std::enable_if_t
		<
			std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
			std::is_constructible_v<
				value_type,
				typename std::iterator_traits< ForwardIterator >::reference
			>,
			void
		>
	assign ( ForwardIterator _first_, ForwardIterator _last_ );

	void assign ( size_type _n_, const_reference _x_ );

	void assign ( std::initializer_list< value_type > _list_ )
	{ assign( _list_.begin(), _list_.end() ); }

	allocator_type get_allocator () const noexcept
	{ return this->_alloc(); }

	      iterator begin ()       noexcept;
	const_iterator begin () const noexcept;
	      iterator   end ()       noexcept;
	const_iterator   end () const noexcept;

	reverse_iterator rbegin () noexcept
	{ return reverse_iterator( end() ); }

	const_reverse_iterator rbegin () const noexcept
	{ return const_reverse_iterator( end() ); }

	reverse_iterator rend () noexcept
	{ return reverse_iterator( begin() ); }

	const_reverse_iterator rend () const noexcept
	{ return const_reverse_iterator( begin() ); }

	const_iterator cbegin () const noexcept
	{ return begin(); }

	const_iterator cend () const noexcept
	{ return end(); }

	const_reverse_iterator crbegin () const noexcept
	{ return rbegin(); }

	const_reverse_iterator crend () const noexcept
	{ return rend(); }

	size_type size () const noexcept
	{ return static_cast< size_type >( this->end_ - this->begin_ ); }

	size_type capacity () const noexcept
	{ return _base::capacity(); }

	[[ nodiscard ]]
	bool empty () const noexcept
	{ return this->begin_ == this->end_; }

	size_type max_size () const noexcept;

	void reserve ( size_type _n_ );
	void shrink_to_fit () noexcept;

	      reference operator[] ( size_type _n_ )       noexcept;
	const_reference operator[] ( size_type _n_ ) const noexcept;

	bool   operator== ( prefix_array< T, Allocator > const & _rhs_ ) const noexcept;
	auto   operator+  ( prefix_array< T, Allocator > const & _rhs_ ) const;
	auto & operator+= ( prefix_array< T, Allocator > const & _rhs_ )       noexcept;
	auto   operator-  ( prefix_array< T, Allocator > const & _rhs_ ) const;
	auto & operator-= ( prefix_array< T, Allocator > const & _rhs_ )       noexcept;

	      reference at ( size_type _n_ );
	const_reference at ( size_type _n_ ) const;

	value_type element_at ( size_type _n_ ) const;

	value_type range (                              ) const;
	value_type range ( size_type _x_, size_type _y_ ) const;

	// 2D overloads
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U & at ( size_type _x_, size_type _y_ )
	{
		return at( _x_ ).at( _y_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U const & at ( size_type _x_, size_type _y_ ) const
	{
		return at( _x_ ).at( _y_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U element_at ( size_type _x_, size_type _y_ ) const
	{
		return element_at( _x_ ).element_at( _y_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U range ( size_type _x1_, size_type _y1_, size_type _x2_, size_type _y2 ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2 );
	}

	// 3D overloads
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U & at ( size_type _x_, size_type _y_, size_type _z_ )
	{
		return at( _x_ ).at( _y_ ).at( _z_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U const & at ( size_type _x_, size_type _y_, size_type _z_ ) const
	{
		return at( _x_ ).at( _y_ ).at( _z_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U element_at ( size_type _x_, size_type _y_, size_type _z_ ) const
	{
		return element_at( _x_ ).element_at( _y_ ).element_at( _z_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U range ( size_type _x1_, size_type _y1_, size_type _z1_, size_type _x2_, size_type _y2_, size_type _z2_ ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2_ ).range( _z1_, _z2_ );
	}

	reference front () noexcept
	{
		_NPL_ASSERT( !empty(), "prefix_array::front called on empty prefix_array" );

		return *this->begin_;
	}

	const_reference front () const noexcept
	{
		_NPL_ASSERT( !empty(), "prefix_array::front called on empty prefix_array" );

		return *this->begin_;
	}

	reference back () noexcept
	{
		_NPL_ASSERT( !empty(), "prefix_array::back called on empty prefix_array" );

		return *( this->end_ - 1 );
	}

	const_reference back () const noexcept
	{
		_NPL_ASSERT( !empty(), "prefix_array::back called on empty prefix_array" );

		return *( this->end_ - 1 );
	}

	value_type * data () noexcept
	{ return _VSTD::__to_address( this->begin_ ); }

	value_type const * data () const noexcept
	{ return _VSTD::__to_address( this->begin_ ); }

	template< typename Arg >
	void _emplace_back ( Arg&& _arg_ )
	{
		emplace_back( std::forward< Arg >( _arg_ ) );
	}

	void push_back ( const_reference _x_ );

	template< typename... Args >
	void push_back ( const_reference _x_, Args... _args_ )
	{
		push_back( _x_ );
		push_back( _args_... );
	}

	void push_back ( value_type && _x_ );

	template< typename... Args >
	void push_back ( value_type && _x_, Args... _args_ )
	{
		push_back( _x_ );
		push_back( _args_... );
	}

	template< typename... Args >
	reference emplace_back ( Args&&... _args_ );

	void pop_back ();

private:
	iterator insert ( const_iterator _position_, const_reference _x_ );
	iterator insert ( const_iterator _position_, value_type && _x_ );

	template< typename... Args >
	iterator emplace ( const_iterator _position_, Args&&... _args_ );

	iterator insert ( const_iterator _position_, size_type _n_, const_reference _x_ );
	template< typename InputIterator >
		typename std::enable_if_t
		<
			 std::__is_cpp17_input_iterator  < InputIterator >::value &&
			!std::__is_cpp17_forward_iterator< InputIterator >::value &&
			 std::is_constructible_v<
			 	value_type,
				typename std::iterator_traits< InputIterator >::reference
			>,
			iterator
		>
	insert ( const_iterator _position_, InputIterator _first_, InputIterator _last_ );

	template< typename ForwardIterator >
		typename std::enable_if_t
		<
			std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
			std::is_constructible_v<
				value_type,
				typename std::iterator_traits< ForwardIterator >::reference
			>,
			iterator
		>
	insert ( const_iterator _position_, ForwardIterator _first_, ForwardIterator _last_ );

	iterator insert ( const_iterator _position_, std::initializer_list< value_type > _list_ )
	{ return insert( _position_, _list_.begin(), _list_.end() ); }

	iterator erase ( const_iterator _position_                        );
	iterator erase ( const_iterator    _first_, const_iterator _last_ );

	void resize ( size_type _n_                      );
	void resize ( size_type _n_, const_reference _x_ );

	void swap ( prefix_array & ) noexcept;

public:
	void clear () noexcept
	{
		size_type old_size = size();
		_base::clear();
		_annotate_shrink( old_size );
		_invalidate_all_iterators();
	}

	bool _invariants () const;

#if _NPL_DEBUG_LEVEL == 2
	bool _dereferenceable ( const_iterator const * _i_                     ) const;
	bool _decrementable   ( const_iterator const * _i_                     ) const;
	bool _addable         ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const;
	bool _subscriptable   ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const;
#endif

private:
	void _invalidate_all_iterators ();
	void _invalidate_iterators_past ( pointer _new_last_ );

	void _vallocate   ( size_type _n_ );
	void _vdeallocate (               ) noexcept;

	size_type _recommend ( size_type _new_size_ ) const;

	void _construct_at_end ( size_type _n_                      );
	void _construct_at_end ( size_type _n_, const_reference _x_ );

	template< typename ForwardIterator >
		typename std::enable_if_t
		<
			std::__is_cpp17_forward_iterator< ForwardIterator >::value,
			void
		>
	_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, size_type _n_ );

	void _append ( size_type _n_                      );
	void _append ( size_type _n_, const_reference _x_ );

	      iterator _make_iter ( pointer _p_ )       noexcept;
	const_iterator _make_iter ( pointer _p_ ) const noexcept;

	void    _swap_out_circular_buffer ( std::__split_buffer< value_type, allocator_type & > & _b_              );
	pointer _swap_out_circular_buffer ( std::__split_buffer< value_type, allocator_type & > & _b_, pointer _p_ );

	void _move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ );

	void _move_assign ( prefix_array & _c_, std::true_type  ) noexcept( std::is_nothrow_move_assignable_v< allocator_type > );
	void _move_assign ( prefix_array & _c_, std::false_type ) noexcept( _alloc_traits::is_always_equal::value );

	void _destruct_at_end ( pointer _new_last_ ) noexcept
	{
		_invalidate_iterators_past( _new_last_ );
		size_type old_size = size();
		_base::_destruct_at_end( _new_last_ );
		_annotate_shrink( old_size );
	}

	template< typename U >
	inline void _push_back_slow_path ( U&& _x_ );

	template< typename... Args >
	inline void _emplace_back_slow_path ( Args&&... _args_ );

	//  AddressSanitizer stuff
#ifndef _LIBCPP_HAS_NO_ASAN
	void _annotate_contiguous_container ( void const * _beg_, void const * _end_,
					      void const * _old_mid_,
					      void const * _new_mid_ ) const
	{
		if( _beg_ && std::is_same_v< allocator_type, _default_allocator_type > )
		{
			__sanitizer_annotate_contiguous_container( _beg_, _end_, _old_mid_, _new_mid_ );
		}
	}
#else
	void _annotate_contiguous_container ( void const *, void const *, void const *, void const * ) const noexcept {}
#endif

	void _annotate_new ( size_type _current_size_ ) const noexcept
	{
		_annotate_contiguous_container( data(), data() + capacity(),
						data() + capacity(), data() + _current_size_ );
	}

	void _annotate_delete () const noexcept
	{
		_annotate_contiguous_container( data(), data() + capacity(),
						data() + size(), data() + capacity() );
	}

	void _annotate_increase ( size_type _n_ ) const noexcept
	{
		_annotate_contiguous_container( data(), data() + capacity(),
						data() + size(), data() + size() + _n_ );
	}

	void _annotate_shrink ( size_type _old_size_ ) const noexcept
	{
		_annotate_contiguous_container( data(), data() + capacity(),
						data() + _old_size_, data() + size() );
	}

	struct _construct_transaction
	{
		explicit _construct_transaction ( prefix_array & _p_, size_type _n_ )
			: p_( _p_ ), pos_( _p_.end_ ), new_end_( _p_.end_ + _n_ )
		{
#ifndef _LIBCPP_HAS_NO_ASAN
			p_._annotate_increate( _n_ );
#endif
		}

		~_construct_transaction ()
		{
			p_.end_ = pos_;
#ifndef _LIBCPP_HAS_NO_ASAN
			if( pos_ != new_end_ )
			{
				p_._annotate_shrink( new_end_ - p_.begin_ );
			}
#endif
		}

		prefix_array  &     p_;
		pointer             pos_;
		const_pointer const new_end_;

	private:
		_construct_transaction             ( _construct_transaction const & ) = delete;
		_construct_transaction & operator= ( _construct_transaction const & ) = delete;
	};

	template< typename... Args >
	void _construct_one_at_end ( Args... _args_ )
	{
		_construct_transaction tx( *this, 1 );

		if( this->begin_ == this->end_ )
		{
			_alloc_traits::construct( this->_alloc(), _VSTD::__to_address( tx.pos_ ), std::forward< Args >( _args_ )... );
		}
		else
		{
			value_type val( _args_... );
			val += *( tx.pos_ - 1 );

			_alloc_traits::construct( this->_alloc(), _VSTD::__to_address( tx.pos_ ), val );
		}
		++tx.pos_;
	}
};

template< typename InputIterator,
	  typename Alloc = std::allocator< std::__iter_value_type< InputIterator > >,
	  typename = std::enable_if< std::__is_allocator< Alloc >::value >
	  >
prefix_array ( InputIterator, InputIterator )
	-> prefix_array< std::__iter_value_type< InputIterator >, Alloc >;

template< typename InputIterator,
	  typename Alloc,
	  typename = std::enable_if< std::__is_allocator< Alloc >::value >
	  >
prefix_array ( InputIterator, InputIterator, Alloc )
	-> prefix_array< std::__iter_value_type< InputIterator >, Alloc >;

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_swap_out_circular_buffer ( std::__split_buffer< value_type, allocator_type & > & _b_ )
{
	_annotate_delete();

	_VSTD::__construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, this->end_, _b_.__begin_ );

	std::swap( this->begin_    , _b_.__begin_ );
	std::swap( this->end_      , _b_.__end_   );
	std::swap( this->_end_cap(), _b_.__end_cap() );

	_b_.__first_ = _b_.__begin_;
	_annotate_new( size() );
	_invalidate_all_iterators();
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::pointer
prefix_array< T, Allocator >::_swap_out_circular_buffer( std::__split_buffer< value_type, allocator_type & > & _b_, pointer _p_ )
{
	_annotate_delete();

	pointer r = _b_.__begin_;

	_VSTD::__construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, _p_, _b_.__begin_ );
	_VSTD::__construct_forward_with_exception_guarantees( this->_alloc(), _p_, this->end_, _b_.__end_ );

	std::swap( this->begin_    , _b_.begin_ );
	std::swap( this->end_      , _b_.end_   );
	std::swap( this->_end_cap(), _b_._end_cap() );

	_b_._first_ = _b_.__begin_;
	_annotate_new( size() );
	_invalidate_all_iterators();

	return r;
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_vallocate ( size_type _n_ )
{
	if( _n_ > max_size() )
	{
//		this->_throw_length_error();
		throw std::length_error( "prefix_array::_vallocate: n > max size" );
	}

	this->begin_ = this->end_ = _alloc_traits::allocate( this->_alloc(), _n_ );
	this->_end_cap() = this->begin_ + _n_;

	_annotate_new( 0 );
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_vdeallocate () noexcept
{
	if( this->begin_ != nullptr )
	{
		clear();
		_alloc_traits::deallocate( this->_alloc(), this->begin_, capacity() );
		this->begin_ = this->end_ = this->_end_cap() = nullptr;
	}
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::size_type
prefix_array< T, Allocator >::max_size () const noexcept
{
	return std::min< size_type >( _alloc_traits::max_size( this->_alloc() ), std::numeric_limits< difference_type >::max() );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::size_type
prefix_array< T, Allocator >::_recommend ( size_type _new_size_ ) const
{
	size_type const ms = max_size();

	if( _new_size_ > ms )
	{
//		this->_throw_length_error();
		throw std::length_error( "prefix_array::_recommend: new size > max size" );
	}

	size_type const cap = capacity();

	if( cap >= ms / 2 )
	{
		return ms;
	}

	return std::max< size_type >( 1.618 * cap, _new_size_ );  //  golden ratio magic
}

/**
 *
 * Default constructs _n_ objects starting at end_
 * throws if construction throws
 * precondition: _n_ > 0
 * precondition: size() + _n_ < capacity()
 * postcondition: size() = size() + _n_
 *
 * */
template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_construct_at_end ( size_type _n_ )
{
	_construct_transaction tx( *this, _n_ );
	const_pointer new_end = tx.new_end_;

	for( pointer pos = tx.pos_; pos != new_end; ++pos, tx.pos_ = pos )
	{
		_alloc_traits::construct( this->_alloc(), _VSTD::__to_address( pos ) );
	}
}

/**
 *
 * copy constructs _n_ objects starting at end_ from _x_
 * throws if construction throws
 * precondition: _n_ > 0
 * precondition: size() + _n_ <= capacity()
 * postcondition: size() == old size() + _n_
 * postcondition: [ i ] == _x_ for all i in [ size() - _n_, _n_ ]
 *
 * */
template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::_construct_at_end ( size_type _n_, const_reference _x_ )
{
	for( size_type i = 0; i < _n_; ++i )
	{
		_construct_one_at_end( _x_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
typename std::enable_if_t
<
	std::__is_cpp17_forward_iterator< ForwardIterator >::value,
	void
>
prefix_array< T, Allocator >::_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, [[ maybe_unused ]] size_type _n_ )
{
	for( ; _first_ != _last_; ++_first_ )
	{
		if( this->end_ != this->_end_cap() )
		{
			_construct_one_at_end( *_first_ );
		}
		else
		{
			_push_back_slow_path( *_first_ );
		}
	}
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_append ( size_type _n_ )
{
	if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
	{
		this->_construct_at_end( _n_ );
	}
	else
	{
		allocator_type & a = this->_alloc();
		std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
		b._construct_at_end( _n_ );
		_swap_out_circular_buffer( b );
	}
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_append ( size_type _n_, const_reference _x_ )
{
	if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
	{
		this->_construct_at_end( _n_, _x_ );
	}
	else
	{
		allocator_type & a = this->_alloc();
		std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
		b._construct_at_end( _n_, _x_ );
		_swap_out_circular_buffer( b );
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( size_type _n_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
//		_construct_at_end( _n_ );
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( size_type _n_, allocator_type const & _a_ )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
//		_construct_at_end( _n_ );
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( size_type _n_, value_type const & _x_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _n_, _x_ );
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( size_type _n_, value_type const & _x_, allocator_type const & _a_ )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _n_, _x_ );
	}
}

template< typename T, typename Allocator >
template< typename InputIterator >
prefix_array< T, Allocator >::prefix_array ( InputIterator _first_,
		typename std::enable_if_t< std::__is_cpp17_input_iterator  < InputIterator >::value &&
					  !std::__is_cpp17_forward_iterator< InputIterator >::value &&
					   std::is_constructible_v<
					   	value_type,
						typename std::iterator_traits< InputIterator >::reference
					>,
		InputIterator > _last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename InputIterator >
prefix_array< T, Allocator >::prefix_array ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
		typename std::enable_if_t< std::__is_cpp17_input_iterator  < InputIterator >::value &&
			                  !std::__is_cpp17_forward_iterator< InputIterator >::value &&
					   std::is_constructible_v<
						 	value_type,
							typename std::iterator_traits< InputIterator >::reference
					> > * )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
prefix_array< T, Allocator >::prefix_array ( ForwardIterator _first_,
		typename std::enable_if_t< std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
					   std::is_constructible_v<
					   	value_type,
						typename std::iterator_traits< ForwardIterator >::reference
					>,
					ForwardIterator > _last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	size_type _n_ = static_cast< size_type >( std::distance( _first_, _last_ ) );

	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _first_, _last_, _n_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
prefix_array< T, Allocator >::prefix_array ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
		typename std::enable_if_t< std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
					   std::is_constructible_v<
					 	value_type,
						typename std::iterator_traits< ForwardIterator >::reference
					> > * )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	size_type _n_ = static_cast< size_type >( std::distance( _first_, _last_ ) );

	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _first_, _last_, _n_ );
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( prefix_array const & _x_ )
	: _base( _alloc_traits::select_on_container_copy_construction( _x_._alloc() ) )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	size_type _n_ = _x_.size();

	if( _n_ > 0 )
	{
		_vallocate( _n_ );

		for( size_type i = 0; i < _x_.size(); ++i )
		{
			_construct_one_at_end( _x_.element_at( i ) );
		}
	}
}

template< typename T, typename Allocator >
prefix_array< T, Allocator >::prefix_array ( prefix_array const & _x_, std::__identity< allocator_type > const & _a_ )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	size_type _n_ = _x_.size();

	if( _n_ > 0 )
	{
		_vallocate( _n_ );

		for( size_type i = 0; i < _x_.size(); ++i )
		{
			_construct_one_at_end( _x_.element_at( i ) );
		}
	}
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator >::prefix_array ( prefix_array && _x_ ) noexcept
	: _base( std::move( _x_._alloc() ) )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	this->begin_     = _x_.begin_;
	this->end_       = _x_.end_;
	this->_end_cap() = _x_._end_cap();
	_x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator >::prefix_array ( prefix_array && _x_, std::__identity< allocator_type > const & _a_ )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _a_ == _x_._alloc() )
	{
		this->begin_     = _x_.begin_;
		this->end_       = _x_.end_;
		this->_end_cap() = _x_._end_cap();
		_x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
	}
	else
	{
		using _ip = std::move_iterator< pointer >;
		assign( _ip( _x_.begin_ ), _ip( _x_.end_ ) );
	}
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator >::prefix_array ( std::initializer_list< value_type > _list_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _list_.size() > 0 )
	{
		_vallocate( _list_.size() );
		_construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
	}
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator >::prefix_array ( std::initializer_list< value_type > _list_, allocator_type const & _a_ )
	: _base( _a_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	if( _list_.size() )
	{
		_vallocate( _list_.size() );
		_construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
	}
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator > &
prefix_array< T, Allocator >::operator= ( prefix_array && _x_ )
	noexcept(( std::__noexcept_move_assign_container< Allocator, _alloc_traits >::value ))
{
	_move_assign( _x_, std::integral_constant< bool,
			_alloc_traits::propagate_on_container_move_assignment::value >() );
	return *this;
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_move_assign ( prefix_array & _c_, std::false_type )
	noexcept( _alloc_traits::is_always_equal::value )
{
	if( _base::_alloc() != _c_._alloc() )
	{
		using _ip = std::move_iterator< pointer >;
		assign( _ip( _c_.begin_ ), _ip( _c_.end_ ) );
	}
	else
	{
		_move_assign( _c_, std::true_type() );
	}
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_move_assign ( prefix_array & _c_, std::true_type )
	noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
{
	_vdeallocate();
	_base::_move_assign_alloc( _c_ );  //  this can throw

	this->begin_     = _c_.begin_;
	this->end_       = _c_.end_;
	this->_end_cap() = _c_._end_cap();

	_c_.begin_ = _c_.end_ = _c_._end_cap() = nullptr;
}

template< typename T, typename Allocator >
inline
prefix_array< T, Allocator > &
prefix_array< T, Allocator >::operator= ( prefix_array const & _x_ )
{
	if( this != &_x_ )
	{
		_base::_copy_assign_alloc( _x_ );
		assign( _x_.begin_, _x_.end_ );
	}
	return *this;
}

template< typename T, typename Allocator >
template< typename InputIterator >
typename std::enable_if_t
<
	 std::__is_cpp17_input_iterator  < InputIterator >::value &&
	!std::__is_cpp17_forward_iterator< InputIterator >::value &&
	 std::is_constructible_v<
	 	T,
		typename std::iterator_traits< InputIterator >::reference
	>,
	void
>
prefix_array< T, Allocator >::assign ( InputIterator _first_, InputIterator _last_ )
{
	clear();

	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
typename std::enable_if_t
<
	std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
	std::is_constructible_v<
		T,
		typename std::iterator_traits< ForwardIterator >::reference
	>,
	void
>
prefix_array< T, Allocator >::assign ( ForwardIterator _first_, ForwardIterator _last_ )
{
	size_type new_size = static_cast< size_type >( std::distance( _first_, _last_ ) );

	if( new_size <= capacity() )
	{
		ForwardIterator mid = _last_;
		bool growing = false;

		if( new_size > size() )
		{
			growing = true;
			mid     = _first_;
			std::advance( mid, size() );
		}

		pointer m = std::copy( _first_, mid, this->begin_ );

		if( growing )
		{
			_construct_at_end( mid, _last_, new_size - size() );
		}
		else
		{
			this->_destruct_at_end( m );
		}
	}
	else
	{
		_vdeallocate();
		_vallocate( _recommend( new_size ) );
		_construct_at_end( _first_, _last_, new_size );
	}

	_invalidate_all_iterators();
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::assign ( size_type _n_, const_reference _x_ )
{
	if( _n_ <= capacity() )
	{
		size_type s = size();

		std::fill_n( this->begin_, std::min( _n_, s ), _x_ );

		if( _n_ > s )
		{
			_construct_at_end( _n_ - s, _x_ );
		}
		else
		{
			this->_destruct_at_end( this->begin_ + _n_ );
		}
	}
	else
	{
		_vdeallocate();
		_vallocate( _recommend( static_cast< size_type >( _n_ ) ) );
		_construct_at_end( _n_, _x_ );
	}

	_invalidate_all_iterators();
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::_make_iter ( pointer _p_ ) noexcept
{
#if _NPL_DEBUG_LEVEL == 2
	return iterator( this, _p_ );
#else
	return iterator( _p_ );
#endif
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::const_iterator
prefix_array< T, Allocator >::_make_iter ( pointer _p_ ) const noexcept
{
#if _NPL_DEBUG_LEVEL == 2
	return iterator( this, _p_ );
#else
	return iterator( _p_ );
#endif
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::begin () noexcept
{
	return _make_iter( this->begin_ );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::const_iterator
prefix_array< T, Allocator >::begin () const noexcept
{
	return _make_iter( this->begin_ );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::end () noexcept
{
	return _make_iter( this->end_ );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::const_iterator
prefix_array< T, Allocator >::end () const noexcept
{
	return _make_iter( this->end_ );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::reference
prefix_array< T, Allocator >::operator[] ( size_type _n_ ) noexcept
{
	_NPL_ASSERT( _n_ < size(), "prefix_array[] index out of bounds" );
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::const_reference
prefix_array< T, Allocator >::operator[] ( size_type _n_ ) const noexcept
{
	_NPL_ASSERT( _n_ < size(), "prefix_array[] index out of bounds" );
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::operator== ( prefix_array< T, Allocator > const & _rhs_ ) const noexcept
{
	if( this->_alloc() != _rhs_._alloc() )
	{
		return false;
	}

	if( size() != _rhs_.size() )
	{
		return false;
	}

	for( size_type i = 0; i < size(); ++i )
	{
		if( element_at( i ) != _rhs_.element_at( i ) )
		{
			return false;
		}
	}

	return true;
}

template< typename T, typename Allocator >
auto
prefix_array< T, Allocator >::operator+ ( prefix_array< T, Allocator > const & _rhs_ ) const
{
	_NPL_ASSERT( size() == _rhs_.size(), "prefix_array::operator+: size != rhs size" );

	_self res( size() );

	for( size_type i = 0; i < size(); ++i )
	{
		res.push_back( element_at( i ) + _rhs_.element_at( i ) );
	}

	return res;
}

template< typename T, typename Allocator >
auto &
prefix_array< T, Allocator >::operator+= ( prefix_array< T, Allocator > const & _rhs_ ) noexcept
{
	if( size() >= _rhs_.size() )
	{
		for( size_type i = 0; i < _rhs_.size(); ++i )
		{
			this->begin_[ i ] += _rhs_.begin_[ i ];
		}
		for( size_type i = _rhs_.size(); i < size(); ++i )
		{
			this->begin_[ i ] += this->begin_[ i - 1 ];
		}
	}
	else
	{
		for( size_type i = 0; i < size(); ++i )
		{
			this->begin_[ i ] += _rhs_.begin_[ i ];
		}
		for( size_type i = size(); i < _rhs_.size(); ++i )
		{
			push_back( _rhs_.element_at( i ) );
		}
	}

	return *this;
}

template< typename T, typename Allocator >
auto
prefix_array< T, Allocator >::operator- ( prefix_array< T, Allocator > const & _rhs_ ) const
{
	_NPL_ASSERT( size() == _rhs_.size(), "prefix_array::operator-: size != rhs size" );

	_self res( size() );

	for( size_type i = 0; i < size(); ++i )
	{
		res.push_back( element_at( i ) - _rhs_.element_at( i ) );
	}

	return res;
}

template< typename T, typename Allocator >
auto &
prefix_array< T, Allocator >::operator-= ( prefix_array< T, Allocator > const & _rhs_ ) noexcept
{
	_NPL_ASSERT( size() == _rhs_.size(), "prefix_array::operator-=: size != rhs size" );

	for( size_type i = 0; i < size(); ++i )
	{
		this->begin_[ i ] -= _rhs_.begin_[ i ];
	}

	return *this;
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::reference
prefix_array< T, Allocator >::at ( size_type _n_ )
{
	if( empty() || _n_ >= size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "prefix_array::at: index out of bounds" );
	}
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::const_reference
prefix_array< T, Allocator >::at ( size_type _n_ ) const
{
	if( empty() || _n_ >= size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "prefix_array::at: index out of bounds" );
	}
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::value_type
prefix_array< T, Allocator >::element_at ( size_type _n_ ) const
{
	if( empty() || _n_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "prefix_array::element_at: index out of bounds" );
	}

	return range( _n_, _n_ );
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::value_type
prefix_array< T, Allocator >::range () const
{
	return range( 0, size() - 1 );
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::value_type
prefix_array< T, Allocator >::range ( size_type _x_, size_type _y_ ) const
{
	if( empty() || _x_ > size() || _y_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "prefix_array::range: index out of bounds" );
	}

	return _x_ == 0 ?
		this->begin_[ _y_ ] :
		this->begin_[ _y_ ] - this->begin_[ _x_ - 1 ];
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::reserve ( size_type _n_ )
{
	if( _n_ > capacity() )
	{
		allocator_type & a = this->_alloc();
		std::__split_buffer< value_type, allocator_type & > _b_( _n_, size(), a );
		_swap_out_circular_buffer( _b_ );
	}
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::shrink_to_fit () noexcept
{
	if( capacity() > size() )
	{
#ifndef _NPL_NO_EXCEPTIONS
		try
		{
#endif
			allocator_type & a = this->_alloc();
			std::__split_buffer< value_type, allocator_type & > b( size(), size(), a );
			_swap_out_circular_buffer( b );
#ifndef _NPL_NO_EXCEPTIONS
		}
		catch( ... )
		{
		}
#endif
	}
}

template< typename T, typename Allocator >
template< typename U >
void
prefix_array< T, Allocator >::_push_back_slow_path ( U && _x_ )
{
	allocator_type & a = this->_alloc();

	std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), size(), a );

	if( this->begin_ == this->end_ )
	{
		_alloc_traits::construct( a, _VSTD::__to_address( b.__end_ ), std::forward< U >( _x_ ) );
	}
	else
	{
		auto val = _x_;
		val += *( this->end_ - 1 );

		_alloc_traits::construct( a, _VSTD::__to_address( b.__end_ ), std::forward< U >( val ) );
	}

	b.__end_++;
	_swap_out_circular_buffer( b );
}

template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::push_back ( const_reference _x_ )
{
	if( this->end_ != this->_end_cap() )
	{
		_construct_one_at_end( _x_ );
	}
	else
	{
		_push_back_slow_path( _x_ );
	}
}

template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::push_back ( value_type && _x_ )
{
	if( this->end_ < this->_end_cap() )
	{
		_construct_one_at_end( std::move( _x_ ) );
	}
	else
	{
		_push_back_slow_path( std::move( _x_ ) );
	}
}

template< typename T, typename Allocator >
template< typename... Args >
void
prefix_array< T, Allocator >::_emplace_back_slow_path ( Args&&... _args_ )
{
	allocator_type & a = this->_alloc();

	std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), size(), a );

	if( this->begin_ == this->end_ )
	{
		_alloc_traits::construct( a, _VSTD::__to_address( b.__end_ ), std::forward< Args >( _args_ )... );
	}
	else
	{
		auto val = value_type( _args_... );
		val += *( this->end_ - 1 );

		_alloc_traits::construct( a, _VSTD::__to_address( b.__end_ ), std::forward< Args >( val )... );
	}

	b.__end_++;
	_swap_out_circular_buffer( b );
}

template< typename T, typename Allocator >
template< typename... Args >
inline
typename prefix_array< T, Allocator >::reference
prefix_array< T, Allocator >::emplace_back ( Args&&... _args_ )
{
	if( this->end_ < this->_end_cap() )
	{
		_construct_one_at_end( std::forward< Args >( _args_ )... );
	}
	else
	{
		_emplace_back_slow_path( std::forward< Args >( _args_ )... );
	}
	return this->back();
}

template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::pop_back ()
{
	_NPL_ASSERT( !empty(), "prefix_array::pop_back called on empty prefix_array" );

	this->_destruct_at_end( this->end_ - 1 );
}

template< typename T, typename Allocator >
inline
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::erase ( const_iterator _position_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	_NPL_ASSERT( _position_ != end(), "prefix_array::erase( iterator ) called with a non-dereferenceable iterator" );

	difference_type pos = _position_ - cbegin();
	pointer p = this->begin_ + pos;

	this->_destruct_at_end( std::move( p + 1, this->end_, p ) );
	this->_invalidate_iterators_past( p - 1 );

	iterator r = _make_iter( p );
	return r;
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::erase ( const_iterator _first_, const_iterator _last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	_NPL_ASSERT( _first_ <= _last_, "prefix_array::erase( first, last ) called with invalid range" );

	pointer p = this->begin_ + ( _first_ - begin() );

	if( _first_ != _last_ )
	{
		this->_destruct_at_end( std::move( p + ( _last_ - _first_ ), this->end_, p ) );
		this->_invalidate_iterators_past( p - 1 );
	}

	iterator r = _make_iter( p );
	return r;
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::_move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ )
{
	pointer old_last = this->end_;
	difference_type n = old_last - _to_;
	{
		pointer i = _from_s_ + n;
		_construct_transaction tx( *this, _from_e_ - i );

		for( pointer pos = tx.pos_; i < _from_e_;
				++i, ++pos, tx.pos_ = pos )
		{
			_alloc_traits::construct( this->_alloc(), _VSTD::__to_address( pos ), std::move( *i ) );
		}
	}
	std::move_backward( _from_s_, _from_s_ + n, old_last );
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::insert ( const_iterator _position_, const_reference _x_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	pointer p = this->begin_ + ( _position_ - begin() );

	if( this->end_ < this->_end_cap() )
	{
		if( p == this->end_ )
		{
			_construct_one_at_end( _x_ );
		}
		else
		{
			_move_range( p, this->end_, p + 1 );
			const_pointer xr = std::pointer_traits< const_pointer >::pointer_to( _x_ );

			if( p <= xr && xr < this->end_ )
			{
				++xr;
			}
			*p = *xr;
		}
	}
	else
	{
		allocator_type & a = this->_alloc();
		std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), p - this->begin_, a );

		b.push_back( _x_ );
		p = _swap_out_circular_buffer( b, p );
	}

	return _make_iter( p );
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::insert ( const_iterator _position_, value_type && _x_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	pointer p = this->begin_ + ( _position_ - begin() );

	if( this->end_ < this->_end_cap() )
	{
		if( p == this->end_ )
		{
			_construct_one_at_end( std::move( _x_ ) );
		}
		else
		{
			_move_range( p, this->end_, p + 1 );
			*p = std::move( _x_ );
		}
	}
	else
	{
		allocator_type & a = this->_alloc();

		std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), p - this->begin_, a );
		b.push_back( std::move( _x_ ) );
		p = _swap_out_circular_buffer( b, p );
	}

	return _make_iter( p );
}

template< typename T, typename Allocator >
template< typename... Args >
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::emplace ( const_iterator _position_, Args&&... _args_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	pointer p = this->begin_ + ( _position_ - begin() );

	if( this->end_ < this->_end_cap() )
	{
		if( p == this->end_ )
		{
			_construct_one_at_end( std::forward< Args >( _args_ )... );
		}
		else
		{
			std::__temp_value< value_type, Allocator > tmp( this->_alloc(), std::forward< Args >( _args_ )... );
			_move_range( p, this->end_, p + 1 );
			*p = std::move( tmp.get() );
		}
	}
	else
	{
		allocator_type & a = this->_alloc();

		std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), p - this->begin_, a );
		b.emplace_back( std::forward< Args >( _args_ )... );
		p = _swap_out_circular_buffer( b, p );
	}

	return _make_iter( p );
}

template< typename T, typename Allocator >
typename prefix_array< T, Allocator >::iterator
prefix_array< T, Allocator >::insert ( const_iterator _position_, size_type _n_, const_reference _x_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	pointer p = this->begin_ + ( _position_ - begin() );

	if( _n_ > 0 )
	{
		if( _n_ <= static_cast< size_type >( this->_end_cap() - this->end_ ) )
		{
			size_type old_n = _n_;
			pointer old_last = this->end_;

			if( _n_ > static_cast< size_type >( this->end_ - p ) )
			{
				size_type cx = _n_ - ( this->end_ - p );
				_construct_at_end( cx, _x_ );
				_n_ -= cx;
			}
			if( _n_ > 0 )
			{
				_move_range( p, old_last, p + old_n );

				const_pointer xr = std::pointer_traits< const_pointer >::pointer_to( _x_ );
				if( p <= xr && xr < this->end_ )
				{
					xr += old_n;
				}
				std::fill_n( p, _n_ * xr );
			}
		}
		else
		{
			allocator_type & a = this->_alloc();

			std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), p - this->begin_, a );
			b._construct_at_end( _n_, _x_ );
			p = _swap_out_circular_buffer( b, p );
		}
	}

	return _make_iter( p );
}

template< typename T, typename Allocator >
template< typename InputIterator >
typename std::enable_if_t
<
	 std::__is_cpp17_input_iterator  < InputIterator >::value &&
	!std::__is_cpp17_forward_iterator< InputIterator >::value &&
	 std::is_constructible_v<
	 	T,
		typename std::iterator_traits< InputIterator >::reference
	>,
	typename prefix_array< T, Allocator >::iterator
>
prefix_array< T, Allocator >::insert ( const_iterator _position_, InputIterator _first_, InputIterator _last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	difference_type   off      = _position_ - begin();
	pointer           p        = this->begin_ + off;
	allocator_type  & a        = this->_alloc();
	pointer           old_last = this->end_;

	for( ; this->end_ != this->_end_cap() && _first_ != _last_; ++_first_ )
	{
		_construct_one_at_end( *_first_ );
	}

	std::__split_buffer< value_type, allocator_type & > b( a );

	if( _first_ != _last_ )
	{
#ifndef _NPL_NO_EXCEPTIONS
		try
		{
#endif
			b._construct_at_end( _first_, _last_ );

			difference_type old_size = old_last - this->begin_;
			difference_type old_p    = p - this->begin_;

			reserve( _recommend( size() + b.size() ) );

			p = this->begin_ + old_p;
			old_last = this->begin_ + old_size;
#ifndef _NPL_NO_EXCEPTIONS
		}
		catch( ... )
		{
			erase( _make_iter( old_last ), end() );
			throw;
		}
#endif
	}

	p = std::rotate( p, old_last, this->end_ );

	insert( _make_iter( p ), std::make_move_iterator( b.begin() ),
				 std::make_move_iterator( b.end() ) );

	return begin() + off;
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
typename std::enable_if_t
<
	std::__is_cpp17_forward_iterator< ForwardIterator >::value &&
	std::is_constructible_v<
		T,
		typename std::iterator_traits< ForwardIterator >::reference
	>,
	typename prefix_array< T, Allocator >::iterator
>
prefix_array< T, Allocator >::insert ( const_iterator _position_, ForwardIterator _first_, ForwardIterator _last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#endif
	pointer p = this->begin_ + ( _position_ - begin() );
	difference_type n = std::distance( _first_, _last_ );

	if( n > 0 )
	{
		if( n <= this->_end_cap() - this->end_ )
		{
			size_type old_n = n;
			pointer old_last = this->end_;
			ForwardIterator m = _last_;
			difference_type dx = this->end_ - p;

			if( n > dx )
			{
				m = _first_;
				difference_type diff = this->end_ - p;
				std::advance( m, diff );
				_construct_at_end( m, _last_, n - diff );
				n = dx;
			}
			if( n > 0 )
			{
				_move_range( p, old_last, p + old_n );
				std::copy( _first_, m, p );
			}
		}
		else
		{
			allocator_type & a = this->_alloc();

			std::__split_buffer< value_type, allocator_type & > b( _recommend( size() + n ), p - this->begin_, a );
			b._construct_at_end( _first_, _last_ );
			p = _swap_out_circular_buffer( b, p );
		}
	}

	return _make_iter( p );
}

template< typename T, typename Allocator >
void
prefix_array< T, Allocator >::resize ( size_type _sz_ )
{
	size_type cs = size();

	if( cs < _sz_ )
	{
		this->_append( _sz_ - cs );
	}
	else if( cs > _sz_ )
	{
		this->_destruct_at_end( this->begin_ + _sz_ );
	}
}

template< typename T, typename Allocator >
void prefix_array< T, Allocator >::resize ( size_type _sz_, const_reference _x_ )
{
	size_type cs = size();

	if( cs < _sz_ )
	{
		this->_append( _sz_ - cs, _x_ );
	}
	else if( cs > _sz_ )
	{
		this->_destruct_at_end( this->begin_ + _sz_ );
	}
}

template< typename T, class Allocator >
void
prefix_array< T, Allocator >::swap ( prefix_array & _x_ )
	noexcept
{
	_NPL_ASSERT( _alloc_traits::propagate_on_container_swap::value ||
			this->_alloc() == _x_._alloc(),
			"prefix_array::swap: either propagate_on_container_swap must be true"
			" or the allocators must compare equal" );

	std::swap( this->begin_    , _x_.begin_ );
	std::swap( this->end_      , _x_.end_   );
	std::swap( this->_end_cap(), _x_._end_cap() );

	std::__swap_allocator( this->_alloc(), _x_._alloc(),
			std::integral_constant< bool, _alloc_traits::propagate_on_container_swap::value >() );
#if _NPL_DEBUG_LEVEL == 2
#endif
}

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::_invariants () const
{
	if( this->begin_ == nullptr )
	{
		if( this->end_ != nullptr || this->_end_cap() != nullptr )
		{
			return false;
		}
	}
	else
	{
		if( this->begin_ > this->end_ )
		{
			return false;
		}
		if( this->begin_ == this->_end_cap() )
		{
			return false;
		}
		if( this->end_ > this->_end_cap() )
		{
			return false;
		}
	}
	return true;
}

#if _NPL_DEBUG_LEVEL == 2

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::_dereferenceable ( const_iterator const * _i_ ) const
{
	return this->begin_ <= _i_->base() && _i_->base() < this->end_;
}

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::_decrementable ( const_iterator const * _i_ ) const
{
	return this->begin_ < _i_->base() && _i_->base() <= this->end_;
}

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::_addable ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const
{
	const_pointer p = _i_->base() + _n_;
	return this->begin_ <= p && p <= this->end_;
}

template< typename T, typename Allocator >
bool
prefix_array< T, Allocator >::_subscriptable ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const
{
	const_pointer p = _i_->base() + _n_;
	return this->begin_ <= p && p < this->end_;
}

#endif

template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::_invalidate_all_iterators ()
{
#if _NPL_DEBUG_LEVEL == 2
#endif
}

template< typename T, typename Allocator >
inline
void
prefix_array< T, Allocator >::_invalidate_iterators_past ( pointer _new_last_ )
{
#if _NPL_DEBUG_LEVEL == 2
#else
	( ( void ) _new_last_ );
#endif
}



/*
template< typename T, typename Allocator = std::allocator< T > >
class prefix_array_broken;

template< typename T, bool C, typename Allocator = std::allocator< T > >
class prefix_array_iterator_broken
{
	friend class prefix_array_broken< T, Allocator >;
	friend class prefix_array_iterator_broken< T, !C >;

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using iterator_category = std::random_access_iterator_tag;
	using reference         = std::conditional_t< C, T const &, T & >;
	using pointer           = std::conditional_t< C,
	      typename std::allocator_traits< Allocator >::const_pointer,
	      typename std::allocator_traits< Allocator >::pointer
		      >;

	reference   operator*  (     ) const { return *ptr_; }
	auto      & operator++ (     )       { ptr_++; return *this; }
	auto      & operator-- (     )       { ptr_--; return *this; }
	auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
	auto        operator-- ( int )       { auto it = *this; --*this; return it; }

	template< bool R >
	bool operator== ( prefix_array_iterator_broken< T, R, Allocator > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( prefix_array_iterator_broken< T, R, Allocator > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	operator prefix_array_iterator_broken< T, true, Allocator > () const
	{ return prefix_array_iterator_broken< T, true, Allocator >{ ptr_ }; }

private:
	pointer ptr_;

	explicit prefix_array_iterator_broken ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
};

template< typename T, typename Allocator >
class prefix_array_broken
{
public:
	using     value_type = T;
	using       iterator = prefix_array_iterator_broken< T, false >;
	using const_iterator = prefix_array_iterator_broken< T,  true >;
	using   alloc_traits = std::allocator_traits< Allocator >;
	using        pointer = typename alloc_traits::pointer;

	auto begin ()       { return       iterator{ data_         }; }
	auto   end ()       { return       iterator{ data_ + size_ }; }
	auto begin () const { return const_iterator{ data_         }; }
	auto   end () const { return const_iterator{ data_ + size_ }; }

	prefix_array_broken (                                      Allocator _a_ = {} ) : capacity_ { DEFAULT_CAPACITY }, allocator_ { _a_ } { alloc__(); }
	prefix_array_broken (              std::size_t _capacity_, Allocator _a_ = {} ) : capacity_ {       _capacity_ }, allocator_ { _a_ } { alloc__(); }
	prefix_array_broken ( T ** _data_, std::size_t _capacity_, Allocator _a_ = {} ) : capacity_ {       _capacity_ }, allocator_ { _a_ }
	{
		alloc__();

		for( std::size_t i = 0; i < capacity_; ++i )
		{
			push_back( std::move( ( *_data_ )[ i ] ) );
		}

		*_data_ = nullptr;
	}
	prefix_array_broken ( std::size_t _count_, T _value_, Allocator _a_ = {} ) : capacity_ { _count_ }, allocator_ { _a_ }
	{
		alloc__();

		for( std::size_t i = 0; i < capacity_; ++i )
		{
			push_back( std::move( T( _value_ ) ) );
		}
	}
	// buggy for multidimensional
	prefix_array_broken ( std::initializer_list< T > const & _list_, Allocator _a_ = {} ) : capacity_ { _list_.size() }, allocator_ { _a_ }
	{
		alloc__();

		for( auto val : _list_ )
		{
			push_back( std::move( val ) );
		}
	}
	template< typename Iterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< Iterator >::value_type, void > > >
	prefix_array_broken ( Iterator begin, Iterator end, Allocator _a_ = {} ) : capacity_ { static_cast< std::size_t >( std::distance( begin, end ) ) }, allocator_ { _a_ }
	{
		alloc__();

		while( begin != end )
		{
			push_back( std::move( *begin++ ) );
		}
	}
	prefix_array_broken ( prefix_array_broken< T, Allocator > const & _rhs_ ) : capacity_ { _rhs_.capacity_ }, allocator_ { _rhs_.allocator_ }
	{
		alloc__();

		for( std::size_t i = 0; i < _rhs_.size_; ++i )
		{
			push_back( _rhs_.element_at( i ) );
		}
	}
	prefix_array_broken ( prefix_array_broken< T, Allocator > && _rhs_ ) noexcept
		: data_ { _rhs_.data_ }, size_ { _rhs_.size_ }, capacity_ { _rhs_.capacity_ }, allocator_ { _rhs_.allocator_ } {}

	prefix_array_broken & operator= ( prefix_array_broken< T, Allocator > const & _rhs_ )
	{
		clear();

		size_      = 0;
		capacity_  = _rhs_.capacity_;
		allocator_ = _rhs_.allocator_;

		resize__( _rhs_.capacity_ );

		for( std::size_t i = 0; i < _rhs_.size(); ++i )
		{
			data_[ i ] = _rhs_.data_[ i ];
			size_++;
		}

		return *this;
	}

	prefix_array_broken & operator= ( prefix_array_broken< T, Allocator > && _rhs_ ) noexcept
	{
		clear();

		data_      = _rhs_.data_;
		size_      = _rhs_.size_;
		capacity_  = _rhs_.capacity_;
		allocator_ = _rhs_.allocator_;

		return *this;
	}

	auto & operator+= ( prefix_array_broken< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		for( std::size_t i = 0; i < size_; ++i )
		{
			data_[ i ] += _rhs_.data_[ i ];
		}

		return *this;
	}

	auto operator+ ( prefix_array_broken< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		prefix_array_broken< T > res( size_ );

		for( std::size_t i = 0; i < size_; ++i )
		{
			res.push_back( element_at( i ) + _rhs_.element_at( i ) );
		}

		return res;
	}

	auto & operator-= ( prefix_array_broken< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		for( std::size_t i = 0; i < size_; ++i )
		{
			data_[ i ] -= _rhs_.data_[ i ];
		}

		return *this;
	}

	auto operator- ( prefix_array_broken< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		prefix_array_broken< T > res( size_ );

		for( std::size_t i = 0; i < size_; ++i )
		{
			res.push_back( element_at( i ) - _rhs_.element_at( i ) );
		}

		return res;
	}

	bool operator== ( prefix_array_broken< T > const & _rhs_ ) const noexcept
	{
		if( size_ != _rhs_.size_ )
		{
			return false;
		}

		for( std::size_t i = 0; i < size_; ++i )
		{
			if( element_at( i ) != _rhs_.element_at( i ) )
			{
				return false;
			}
		}

		return true;
	}

	T const & operator[] ( std::size_t _index_ ) const noexcept
	{ return data_[ _index_ ]; }

	std::size_t     size () const noexcept { return     size_; }
	std::size_t capacity () const noexcept { return capacity_; }

	T const & at ( std::size_t _index_ ) const
	{
		if( !is_index_in_range__( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return operator[]( _index_ );
	}

	T range ( std::size_t _x_, std::size_t _y_ ) const
	{
		if( !is_index_in_range__( _x_ ) || !is_index_in_range__( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _x_ == 0 ?
			operator[]( _y_ ) :
			operator[]( _y_ ) - operator[]( _x_ - 1 );
	}

	T element_at ( std::size_t _index_ ) const
	{
		return range( _index_, _index_ );
	}

	//  2D overloads
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< U > > > >
	U const & at ( std::size_t _x_, std::size_t _y_ ) const
	{
		return at( _x_ ).at( _y_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< U > > > >
	U range ( std::size_t _x1_, std::size_t _y1_, std::size_t _x2_, std::size_t _y2_ ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< U > > > >
	U element_at ( std::size_t _x_, std::size_t _y_ ) const
	{
		return element_at( _x_ ).element_at( _y_ );
	}

	//  3D overloads
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< prefix_array_broken< U > > > > >
	U const & at ( std::size_t _x_, std::size_t _y_, std::size_t _z_ ) const
	{
		return at( _x_ ).at( _y_ ).at( _z_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< prefix_array_broken< U > > > > >
	U range ( std::size_t _x1_, std::size_t _y1_, std::size_t _z1_,
			std::size_t _x2_, std::size_t _y2_, std::size_t _z2_ ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2_ ).range( _z1_, _z2_ );
	}

	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array_broken< prefix_array_broken< U > > > > >
	U element_at ( std::size_t _x_, std::size_t _y_, std::size_t _z_ ) const
	{
		return element_at( _x_ ).element_at( _y_ ).element_at( _z_ );
	}

	void reserve ( std::size_t _capacity_ ) { resize__( _capacity_ ); }

	void push_back ( T const & _value_ )
	{
		emplace_back( _value_ );
	}

	void push_back ( T&& _value_ )
	{
		emplace_back( std::move( _value_ ) );
	}

	template< typename... Args >
	void push_back ( T&& _value_, Args&&... _args_ )
	{
		push_back( std::move( _value_ ) );
		push_back( std::forward< Args >( _args_ )... );
	}

	template< typename... Args >
	void emplace_back ( Args&&... _args_ )
	{
		if( size_ >= capacity_ )
		{
			resize__();
		}

		if( size_ == 0 )
		{
			data_[ size_ ] = T( _args_... );
		}
		else
		{
			data_[ size_ ] = T( _args_... );
			data_[ size_ ] += data_[ size_ - 1 ];
		}

		size_++;
	}

	void old_clear () noexcept
	{
		if( data_ )
		{
			T * raw = static_cast< T* >( data_ );

			for( std::size_t i = 0; i < capacity_; ++i )
			{
				alloc_traits::destroy( allocator_, raw + i );
			}

			alloc_traits::deallocate( allocator_, data_, capacity_ );
			data_ = nullptr;
		}
	}

	void clear () noexcept { destruct_at_end__( data_ ); }

	~prefix_array_broken ()
	{
//		clear();
		alloc_traits::deallocate( allocator_, data_, capacity_ );
	}

private:
	pointer     data_ { nullptr };
	std::size_t size_       { 0 };
	std::size_t capacity_   { 0 };

	Allocator allocator_;

	void alloc__ () { data_ = alloc_traits::allocate( allocator_, capacity_ ); }
	void resize__ ()
	{
		auto new_cap = ( capacity_ + 1 ) * 1.618;
		auto tmp = alloc_traits::allocate( allocator_, new_cap );

		for( std::size_t i = 0; i < size_; ++i )
		{
			try
			{
				T * raw = static_cast< T* >( tmp + i );

				alloc_traits::construct( allocator_, raw, std::move( data_[ i ] ) );
			}
			catch( ... )
			{
				alloc_traits::deallocate( allocator_, tmp, new_cap );
				throw;
			}
		}

		clear();

		data_     = tmp;
		capacity_ = new_cap;
	}
	void resize__ ( std::size_t _capacity_ )
	{
		auto tmp = alloc_traits::allocate( allocator_, _capacity_ );

		for( std::size_t i = 0; i < size_; ++i )
		{
			try
			{
				T * raw = static_cast< T* >( tmp + i );

				alloc_traits::construct( allocator_, raw, std::move( data_[ i ] ) );
			}
			catch( ... )
			{
				alloc_traits::deallocate( allocator_, tmp, _capacity_ );
				throw;
			}
		}

		clear();

		data_     = tmp;
		capacity_ = _capacity_;
	}

	void destruct_at_end__ ( pointer new_last ) noexcept
	{
		pointer soon_to_be_end = data_ + size_;
		while( new_last != soon_to_be_end )
		{
			alloc_traits::destroy( allocator_, --soon_to_be_end );
		}
		size_ = new_last - data_;
	}

	bool is_index_in_range__ ( std::size_t _index_ ) const noexcept { return _index_ < size_; }

#ifdef _WIN32
        std::size_t max_size__ () const noexcept
        {
                MEMORYSTATUSEX status;
                status.dwLength = sizeof( status );
                GlobalMemoryStatusEx( &status );

                return status.ullAvailPhys;
        }
#else
        std::size_t max_size__ () const noexcept
        {
                auto pages     = sysconf( _SC_PHYS_PAGES );
                auto page_size = sysconf( _SC_PAGE_SIZE  );

                return pages * page_size;
        }
#endif
};
*/



template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree;

template< typename T, bool C,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree_iterator
{
	//  TODO: implement properly
	friend class fenwick_tree< T >;
	friend class fenwick_tree_iterator< T, !C >;

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t< C, T const *, T * >;
	using reference         = std::conditional_t< C, T const &, T & >;
	using iterator_category = std::random_access_iterator_tag;

	reference   operator*  (     ) const { return *ptr_; }
	auto      & operator++ (     )       { ptr_++; return *this; }
	auto      & operator-- (     )       { ptr_--; return *this; }
	auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
	auto        operator-- ( int )       { auto it = *this; --*this; return it; }

	template< bool R >
	bool operator== ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
	{ return ptr_ != rhs.ptr_; }

	operator fenwick_tree_iterator< T, true > () const
	{ return fenwick_tree_iterator< T, true >{ ptr_ }; }

private:
	pointer ptr_;

	explicit fenwick_tree_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
};

template< typename T, typename U >
class fenwick_tree
{
	using     value_type = T;
	using       iterator = fenwick_tree_iterator< T, false >;
	using const_iterator = fenwick_tree_iterator< T,  true >;

private:
	T           * head_     { nullptr };
	std::size_t   size_           { 0 };
	std::size_t   capacity_       { 0 };

	void alloc  (                        ) { head_ = ( T* ) std::calloc( DEFAULT_CAPACITY, sizeof( T ) ); if( !head_ ) throw std::bad_alloc(); }
	void alloc  ( std::size_t _capacity_ )
	{
		if( _capacity_ > max_size() )
		{
			throw std::out_of_range( "capacity > max_size" );
		}
		head_ = ( T* ) std::calloc(   _capacity_, sizeof( T ) );
		if( !head_ )
		{
			throw std::bad_alloc();
		}
	}
	void resize ()
	{
		auto new_cap = ( capacity_ + 1 ) * 1.618;
		auto tmp = ( T* ) std::realloc( head_, sizeof( T ) * new_cap );

		if( tmp )
		{
			head_     = tmp;
			capacity_ = new_cap;
		}
	}
	void resize ( std::size_t _capacity_ )
	{
		if( _capacity_ > max_size() )
		{
			throw std::out_of_range( "capacity > max_size" );
		}
		auto tmp = ( T* ) std::realloc( head_, sizeof( T ) * _capacity_ );

		if( tmp )
		{
			head_     = tmp;
			capacity_ = _capacity_;
		}
	}

#ifdef _WIN32
	std::size_t max_size () const
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof( status );
		GlobalMemoryStatusEx( &status );

		return status.ullAvailPhys;
	}
#else
	std::size_t max_size () const
	{
		long pages     = sysconf( _SC_PHYS_PAGES );
		long page_size = sysconf( _SC_PAGE_SIZE );

		return pages * page_size;
	}
#endif

	std::size_t p ( std::size_t k ) const noexcept { return k & -k; }

	bool is_index_in_range ( std::size_t _index_ ) const noexcept { return _index_ < capacity_; }

	T sum_to_index ( std::size_t _index_ ) const
	{
		T s = 0;
		_index_++;

		while( _index_ >= 1 )
		{
			s += head_[ _index_ - 1 ];
			_index_ -= p( _index_ );
		}

		return s;
	}

public:
	auto begin ()       { return       iterator{ head_         }; }
	auto   end ()       { return       iterator{ head_ + size_ }; }
	auto begin () const { return const_iterator{ head_         }; }
	auto   end () const { return const_iterator{ head_ + size_ }; }

	T const & operator[] ( std::size_t _index_ ) const { return head_[ _index_ ]; }

	fenwick_tree (                                     ) : capacity_ { DEFAULT_CAPACITY } { alloc(            ); }
	fenwick_tree (              std::size_t _capacity_ ) : capacity_ {       _capacity_ } { alloc( _capacity_ ); }
	fenwick_tree ( T ** _head_, std::size_t _capacity_ ) : capacity_ {       _capacity_ }
	{
		alloc( _capacity_ );

		for( std::size_t i = 0; i < _capacity_; ++i )
		{
			update( ( *_head_ )[ i ], size_++ );
		}
		*_head_ = nullptr;
	}
	fenwick_tree ( std::size_t _count_, T _value_ ) : capacity_ { _count_ }
	{
		alloc( _count_ );

		for( std::size_t i = 0; i < _count_; ++i )
		{
			update( _value_, size_++ );
		}
	}
	fenwick_tree ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() }
	{
		alloc( capacity_ );

		for( T const & t : _list_ )
		{
			update( t, size_++ );
		}
	}
	template< typename Iterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< Iterator >::value_type, void > > >
	fenwick_tree ( Iterator begin, Iterator end ) : capacity_ { static_cast< std::size_t >( std::distance( begin, end ) ) }
	{
		alloc( capacity_ );

		while( begin != end )
		{
			update( *begin++, size_++ );
		}
	}

	std::size_t     size () const noexcept { return     size_; }
	std::size_t capacity () const noexcept { return capacity_; }

	T at ( std::size_t _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _index_ == 0 ?
			sum_to_index( _index_ ) :
			sum_to_index( _index_ ) - sum_to_index( _index_ - 1 );
	}

	T range ( std::size_t _x_, std::size_t _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _x_ == 0 ?
			sum_to_index( _y_ ) :
			sum_to_index( _y_ ) - sum_to_index( _x_ - 1 );
	}

	void update ( T _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		T current = at( _index_++ );

		while( _index_ <= size_ )
		{
			head_[ _index_ - 1 ] += _value_ - current;
			_index_ += p( _index_ );
		}
	}

	void add ( T _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );

		}

		_index_++;

		while( _index_ <= size_ )
		{
			head_[ _index_ - 1 ] += _value_;
			_index_ += p( _index_ );
		}
	}

	void reserve ( std::size_t _capacity_ ) { resize( _capacity_ ); }

	void push_back ( T _value_ )
	{
		if( capacity_ <= size_ ) { resize(); }

		size_++;

		if( size_ == 1 )
		{
			head_[ size_ - 1 ] = _value_;
		}
		else
		{
			update( _value_, size_ - 1 );
		}
	}

	template< typename... Args >
	void push_back ( T _value_, Args... _values_ )
	{
		push_back( _value_ );
		push_back( _values_... );
	}

	template < typename... Args >
	void emplace_back ( Args&&... args )
	{
		if( capacity_ <= size_ ) { resize(); }

		size_++;

		if( size_ == 1 )
		{
			head_[ size_ - 1 ] = T( args... );
		}
		else
		{
			update( T( args... ), size_ - 1 );
		}
	}

	~fenwick_tree () { free( head_ ); }
};


template< typename F, typename T >
concept ParentBuilder = requires( F test, T const & obj )
{
	{ test( obj, obj ) } -> std::same_as< T >;
};

template< typename T >
auto pb_default_
{
	[]( T const & lhs, T const & rhs )
	{
		return lhs + rhs;
	}
};

template< typename T, ParentBuilder< T > PB = decltype( pb_default_< T > ),
	  typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree;

template< typename T, ParentBuilder< T > PB, bool C,
	  typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree_iterator
{
	friend class segment_tree< T, PB >;
	friend class segment_tree_iterator< T, PB, !C >;

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t< C, T const *, T * >;
	using reference         = std::conditional_t< C, T const &, T & >;
	using iterator_category = std::random_access_iterator_tag;

	reference   operator*  (     ) const { return *ptr_; }
	auto      & operator++ (     )       { ptr_++; return *this; }
	auto      & operator-- (     )       { ptr_--; return *this; }
	auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
	auto        operator-- ( int )       { auto it = *this; --*this; return it; }

	template< bool R >
	bool operator== ( segment_tree_iterator< T, PB, R > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( segment_tree_iterator< T, PB, R > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	operator segment_tree_iterator< T, PB, true > () const
	{ return segment_tree_iterator< T, PB, true >{ ptr_ }; }

private:
	pointer ptr_;

	explicit segment_tree_iterator ( pointer _ptr_ ) : ptr_{ _ptr_ } {};
};

template< typename T, ParentBuilder< T > PB, typename U >
class segment_tree
{
	using     value_type = T;
	using       iterator = segment_tree_iterator< T, PB, false >;
	using const_iterator = segment_tree_iterator< T, PB,  true >;

private:
	T           * head_    { nullptr };
	std::size_t   size_    {       0 };
	std::size_t   capacity_{       0 };

	PB parent_builder_;

	void alloc ( std::size_t _capacity_ )
	{
		capacity_ = 2 * round_up_to_pow_2( _capacity_ );
		head_     = ( T* ) std::malloc( sizeof( T ) * capacity_ );
	}

	void resize ()
	{
		auto new_cap = capacity_ * 2;
		auto tmp = ( T* ) std::malloc( sizeof( T ) * new_cap );

		if( tmp )
		{
			for( std::size_t i = 0; i < size_; ++i )
			{
				tmp[ new_cap / 2 + i ] = head_[ capacity_ / 2 + i ];
			}

			free( head_ );

			head_     = tmp;
			capacity_ = new_cap;
		}
	}

	void resize ( std::size_t _capacity_ )
	{
		if( _capacity_ > max_size() )
		{
			throw std::out_of_range( "capacity > max_size" );
		}

		_capacity_ = 2 * round_up_to_pow_2( _capacity_ );
		auto tmp = ( T* ) std::malloc( sizeof( T ) * _capacity_ );

		if( tmp )
		{
			for( std::size_t i = 0; i < size_; ++i )
			{
				tmp[ _capacity_ / 2 + i ] = head_[ capacity_ / 2 + i ];
			}

			free( head_ );

			head_     = tmp;
			capacity_ = _capacity_;
		}
	}

	std::size_t msb64 ( std::size_t val ) const noexcept
	{
		val |= ( val >>  1 );
		val |= ( val >>  2 );
		val |= ( val >>  4 );
		val |= ( val >>  8 );
		val |= ( val >> 16 );
		val |= ( val >> 32 );

		return ( val & ~( val >> 1 ) );
	}

	std::size_t round_up_to_pow_2 ( std::size_t _size_ ) const noexcept
	{
		std::size_t msb = msb64( _size_ );

		if( _size_ == msb )
		{
			return _size_;
		}
		else
		{
			return ( msb << 1 );
		}
	}

	void construct_tree ()
	{
		for( auto i = ( capacity_ / 2 ) - 1; i > 0; i-- )
		{
			head_[ i ] = parent_builder_( head_[ 2 * i ], head_[ 2 * i + 1 ] );
		}
	}

	bool is_index_in_range ( std::size_t _index_ ) const noexcept { return _index_ < capacity_ / 2; }

#ifdef _WIN32
	std::size_t max_size () const noexcept
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof( status );
		GlobalMemoryStatusEx( &status );

		return status.ullAvailPhys;
	}
#else
	std::size_t max_size () const noexcept
	{
		auto pages     = sysconf( _SC_PHYS_PAGES );
		auto page_size = sysconf( _SC_PAGE_SIZE  );

		return pages * page_size;
	}
#endif

public:
	auto begin ()       { return       iterator{ head_ + ( capacity_ / 2 )         }; }
	auto   end ()       { return       iterator{ head_ + ( capacity_ / 2 ) + size_ }; }
	auto begin () const { return const_iterator{ head_ + ( capacity_ / 2 )         }; }
	auto   end () const { return const_iterator{ head_ + ( capacity_ / 2 ) + size_ }; }

	T const & operator[] ( std::size_t _index_ ) const noexcept { return head_[ ( capacity_ / 2 ) + _index_ ]; }

	bool operator== ( segment_tree< T, PB > const & rhs ) const { return head_ == rhs.head_; }
	bool operator!= ( segment_tree< T, PB > const & rhs ) const { return head_ != rhs.head_; }

	segment_tree (                                                                      ) : parent_builder_ { pb_default_< T > } { alloc( DEFAULT_CAPACITY ); }
	segment_tree (                                      ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ } { alloc( DEFAULT_CAPACITY ); }
	segment_tree (              std::size_t _capacity_, ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ } { alloc(       _capacity_ ); }
	segment_tree ( T ** _head_, std::size_t _capacity_, ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( _capacity_ );

		for( std::size_t i = 0; i < _capacity_; ++i )
		{
			head_[ ( capacity_ / 2 ) + i ] = ( *_head_ )[ i ];
			size_++;
		}
		*_head_ = nullptr;
		construct_tree();
	}
	segment_tree ( std::size_t _count_, T _value_, ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( _count_ );

		for( std::size_t i = 0; i < _count_; ++i )
		{
			head_[ ( capacity_ / 2 ) + i ] = _value_;
			size_++;
		}
		construct_tree();
	}
	template< typename Iterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< Iterator >::value_type, void > > >
	segment_tree ( Iterator begin, Iterator end, ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( std::distance( begin, end ) );

		while( begin != end )
		{
			head_[ ( capacity_ / 2 ) + size_ ] = *begin++;
			size_++;
		}
		construct_tree();
	}
	segment_tree ( std::initializer_list< T > const & _list_, ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( _list_.size() );

		for( auto const & el : _list_ )
		{
			head_[ ( capacity_ / 2 ) + size_ ] = el;
			size_++;
		}
		construct_tree();
	}

	std::size_t     size () const noexcept { return         size_; }
	std::size_t capacity () const noexcept { return capacity_ / 2; }

	T const & at ( std::size_t _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );

		}

		return operator[]( _index_ );
	}

	T range ( std::size_t _x_, std::size_t _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );

		}

		_x_ += capacity_ / 2;
		_y_ += capacity_ / 2;

		T res = T();
		bool is_res_default = true;

		while( _x_ <= _y_ )
		{
			if( _x_ % 2 == 1 )
			{
				if( is_res_default )
				{
					res = head_[ _x_++ ];
					is_res_default = false;
				}
				else
				{
					res = parent_builder_( res, head_[ _x_++ ] );
				}
			}
			if( _y_ % 2 == 0 )
			{
				if( is_res_default )
				{
					res = head_[ _y_-- ];
					is_res_default = false;
				}
				else
				{
					res = parent_builder_( res, head_[ _y_-- ] );
				}
			}
			_x_ /= 2;
			_y_ /= 2;
		}
		return res;
	}

	void update ( T _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		_index_ += capacity_ / 2;

		head_[ _index_ ] = std::move( _value_ );

		for( _index_ /= 2; _index_ >= 1; _index_ /= 2 )
		{
			head_[ _index_ ] = parent_builder_( head_[ 2 * _index_ ], head_[ 2 * _index_ + 1 ] );
		}
	}

	void reserve ( std::size_t _capacity_ ) { resize( _capacity_ ); construct_tree(); }

	void push_back ( T _value_ )
	{
		if( size_ >= capacity_ / 2 )
		{
			resize();

			head_[ capacity_ / 2 + size_ ] = std::move( _value_ );
			size_++;

			construct_tree();
		}
		else
		{
			update( _value_, size_++ );
		}
	}

	template< class... Args >
	void emplace_back ( Args&&... args )
	{
		if( size_ >= capacity_ / 2 )
		{
			resize();
		}

		head_[ capacity_ / 2 + size_ ] = std::move( T ( args... ) );
		size_++;
		construct_tree();
	}

	~segment_tree () { free( head_ ); }
};


//  TODO

template< typename T >
class lazy_segtree;

template< typename T >
class dynamic_segtree;

template< typename T >
class persistent_segtree;




} // namespace range_queries

} // namespace natprolib
