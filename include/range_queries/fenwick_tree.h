//
//	fenwick_tree.h
//	natprolib
//
//	Created by Edhem Avdagic on 16/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <iostream>

#include "../util/util.h"
#include "../util/traits.h"
#include "../util/mem.h"
#include "../util/compressed_pair.h"
#include "../util/split_buffer.h"


namespace npl
{

namespace rq
{


template< bool >
class NPL_TEMPLATE_VIS _fenwick_tree_base_common
{
protected:
	NPL_INLINE_VISIBILITY _fenwick_tree_base_common () {}

	[[ noreturn ]] void _throw_length_error () const;
	[[ noreturn ]] void _throw_out_of_range () const;
};

template< bool B >
void _fenwick_tree_base_common< B >::_throw_length_error () const
{
#ifndef NPL_NO_EXCEPTIONS
	throw std::length_error( "fenwick_tree" );
#else
	std::abort();
#endif
}
template< bool B >
void _fenwick_tree_base_common< B >::_throw_out_of_range () const
{
#ifndef NPL_NO_EXCEPTIONS
	throw std::out_of_range( "fenwick_tree" );
#else
	std::abort();
#endif
}

extern template class NPL_EXTERN_TEMPLATE_TYPE_VIS _fenwick_tree_base_common< true >;


template< typename T, typename Allocator >
class _fenwick_tree_base
	: protected _fenwick_tree_base_common< true >
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
	using	 pointer = typename _alloc_traits::pointer;
	using   const_pointer = typename _alloc_traits::const_pointer;
	using	iterator = pointer;
	using  const_iterator = const_pointer;

	pointer begin_;
	pointer end_;
	compressed_pair< pointer, allocator_type > end_cap_;

	allocator_type       & _alloc ()       noexcept { return end_cap_.second(); }
	allocator_type const & _alloc () const noexcept { return end_cap_.second(); }

	pointer       & _end_cap ()       noexcept { return end_cap_.first(); }
	pointer const & _end_cap () const noexcept { return end_cap_.first(); }

	_fenwick_tree_base (	                     ) noexcept( std::is_nothrow_default_constructible_v< allocator_type > );
	_fenwick_tree_base ( allocator_type const &  _a_ );
	_fenwick_tree_base ( allocator_type       && _a_ ) noexcept;

	~_fenwick_tree_base ();

	void clear () noexcept { _destruct_at_end( begin_ ); }

	size_type capacity () const noexcept
	{ return static_cast< size_type >( _end_cap() - begin_ ); }

	void _destruct_at_end ( pointer _new_last_ ) noexcept;

	void _copy_assign_alloc ( _fenwick_tree_base const & _x_ )
	{
		_copy_assign_alloc( _x_, std::integral_constant< bool,
				_alloc_traits::propagate_on_container_copy_assignment::value >() );
	}

	void _move_assign_alloc ( _fenwick_tree_base & _x_ )
		noexcept(
				!_alloc_traits::propagate_on_container_move_assignment::value ||
				std::is_nothrow_move_assignable_v< allocator_type >
			)
	{
		_move_assign_alloc( _x_, std::integral_constant< bool,
				_alloc_traits::propagate_on_container_move_assignment::value >() );
	}

	[[ noreturn ]] void _throw_length_error () const
	{
#ifndef NPL_NO_EXCEPTIONS
		_fenwick_tree_base_common< true >::_throw_length_error();
#else
		std::abort();
#endif
	}

	[[ noreturn ]] void _throw_out_of_range () const
	{
#ifndef NPL_NO_EXCEPTIONS
		_fenwick_tree_base_common< true >::_throw_out_of_range();
#else
		std::abort();
#endif
	}

private:
	void _copy_assign_alloc ( _fenwick_tree_base const & _x_, std::true_type )
	{
		if( _alloc() != _x_._alloc() )
		{
			clear();
			_alloc_traits::deallocate( _alloc(), begin_, capacity() );
			begin_ = end_ = end_cap_() = nullptr;
		}
		_alloc() = _x_._alloc();
	}

	void _copy_assign_alloc ( _fenwick_tree_base const &, std::false_type ) {}

	void _move_assign_alloc ( _fenwick_tree_base & _x_, std::true_type )
		noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
	{
		_alloc() = std::move( _x_._alloc() );
	}

	void _move_assign_alloc ( _fenwick_tree_base &, std::false_type ) noexcept {}
};

template< typename T, typename Allocator >
inline
void _fenwick_tree_base< T, Allocator >::_destruct_at_end ( pointer _new_last_ ) noexcept
{
	pointer soon_to_be_end = end_;
	while( _new_last_ != soon_to_be_end )
	{
		_alloc_traits::destroy( _alloc(), std::to_address( --soon_to_be_end ) );
	}
	end_ = _new_last_;
}

template< typename T, typename Allocator >
inline
_fenwick_tree_base< T, Allocator >::_fenwick_tree_base ()
	noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, _default_init_tag() ) {}

template< typename T, typename Allocator >
inline
_fenwick_tree_base< T, Allocator >::_fenwick_tree_base ( allocator_type const & _a_ )
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, _a_ ) {}

template< typename T, typename Allocator >
inline
_fenwick_tree_base< T, Allocator >::_fenwick_tree_base ( allocator_type && _a_ ) noexcept
	: begin_  ( nullptr ),
	  end_    ( nullptr ),
	  end_cap_( nullptr, std::move( _a_ ) ) {}

template< typename T, typename Allocator >
_fenwick_tree_base< T, Allocator >::~_fenwick_tree_base ()
{
	if( begin_ != nullptr )
	{
		clear();
		_alloc_traits::deallocate( _alloc(), begin_, capacity() );
	}
}

template< typename T, typename Allocator = std::allocator< T > >
class fenwick_tree;

template< typename T, bool C, typename Allocator = std::allocator< T > >
class fenwick_tree_iterator
{
};

template< typename T, typename Allocator >
class fenwick_tree
	: private _fenwick_tree_base< T, Allocator >
{
private:
	using	           _base = _fenwick_tree_base< T, Allocator >;
	using _default_allocator_type = std::allocator< T >;
public:
	using	           _self = fenwick_tree;
	using	      value_type = T;
	using	  allocator_type = Allocator;
	using	   _alloc_traits = typename _base::_alloc_traits;
	using	       reference = typename _base::reference;
	using	 const_reference = typename _base::const_reference;
	using	       size_type = typename _base::size_type;
	using	 difference_type = typename _base::difference_type;
	using	         pointer = typename _base::pointer;
	using	   const_pointer = typename _base::const_pointer;

	using	        iterator = pointer;
	using	  const_iterator = const_pointer;
//	using	        iterator = fenwick_tree_iterator< value_type, false, allocator_type >;
//	using	  const_iterator = fenwick_tree_iterator< value_type,  true, allocator_type >;
	using	reverse_iterator = std::reverse_iterator<       iterator >;
	using  const_reverse_iterator = std::reverse_iterator< const_iterator >;

	static_assert( ( std::is_same_v< typename allocator_type::value_type, value_type > ),
			"Allocator::value_type != value_type" );

	fenwick_tree () noexcept( std::is_nothrow_default_constructible_v< allocator_type > ) {}

	explicit fenwick_tree ( allocator_type const & _a_ ) noexcept : _base( _a_ ) {}

	explicit fenwick_tree ( size_type _n_                             );
	explicit fenwick_tree ( size_type _n_, allocator_type const & _a_ );

	fenwick_tree ( size_type _n_, value_type const & _x_                             );
	fenwick_tree ( size_type _n_, value_type const & _x_, allocator_type const & _a_ );

	template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
	fenwick_tree ( InputIterator _first_,
			typename std::enable_if_t< is_cpp17_input_iterator_v  < InputIterator > &&
						  !is_cpp17_forward_iterator_v< InputIterator > &&
						   std::is_constructible_v<
						   	value_type,
							typename std::iterator_traits< InputIterator >::reference
						>,
						InputIterator > _last_ );

	template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
	fenwick_tree ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
			typename std::enable_if_t< is_cpp17_input_iterator_v  < InputIterator > &&
						  !is_cpp17_forward_iterator_v< InputIterator > &&
						   std::is_constructible_v<
						   	value_type,
							typename std::iterator_traits< InputIterator >::reference
						 > > * = 0 );

	template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
	fenwick_tree ( ForwardIterator _first_,
			typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
						   std::is_constructible_v<
						   	value_type,
							typename std::iterator_traits< ForwardIterator >::reference
						 >,
						 ForwardIterator > _last_ );

	template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
	fenwick_tree ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
			typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
						   std::is_constructible_v<
						   	value_type,
							typename std::iterator_traits< ForwardIterator >::reference
						 > > * = 0 );

	~fenwick_tree ()
	{
		_annotate_delete();
	}

	fenwick_tree ( fenwick_tree const & _x_                                          );
	fenwick_tree ( fenwick_tree const & _x_, _identity< allocator_type > const & _a_ );

	fenwick_tree & operator= ( fenwick_tree const & _x_ );

	fenwick_tree ( std::initializer_list< value_type > _list_                             );
	fenwick_tree ( std::initializer_list< value_type > _list_, allocator_type const & _a_ );

	fenwick_tree ( fenwick_tree && _x_                                          ) noexcept;
	fenwick_tree ( fenwick_tree && _x_, _identity< allocator_type > const & _a_ );

	fenwick_tree & operator= ( fenwick_tree && _x_ )
		noexcept( ( noexcept_move_assign_container_v< Allocator, _alloc_traits > ) );

	fenwick_tree & operator= ( std::initializer_list< value_type > _list_ )
	{ assign( _list_.begin(), _list_.end() ); return *this; }

	template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
	typename std::enable_if_t
	<
		 is_cpp17_input_iterator_v  < InputIterator > &&
		!is_cpp17_forward_iterator_v< InputIterator > &&
		 std::is_constructible_v<
		 	value_type,
			typename std::iterator_traits<  InputIterator >::reference
		>,
		void
	>
	assign ( InputIterator _first_, InputIterator _last_ );

	template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
	typename std::enable_if_t
	<
		is_cpp17_forward_iterator_v< ForwardIterator > &&
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

	bool operator== ( fenwick_tree< T, Allocator > const & _rhs_ ) const noexcept;

	const_iterator begin () const noexcept;
	const_iterator   end () const noexcept;

	const_reverse_iterator rbegin () const noexcept
	{ return const_reverse_iterator( end() ); }

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

	const_reference operator[] ( size_type _n_ ) const noexcept;

	const_reference at ( size_type _n_ ) const;

	value_type element_at ( size_type _n_ ) const;

	value_type range (                              ) const;
	value_type range ( size_type _x_, size_type _y_ ) const;

	const_reference front () const noexcept
	{
	        NPL_ASSERT( !empty(), "fenwick_tree::front called on empty prefix_array" );

	        return *this->begin_;
	}

	reference back () noexcept
	{
	        NPL_ASSERT( !empty(), "fenwick_tree::back called on empty prefix_array" );

	        return *( this->end_ - 1 );
	}

	const_reference back () const noexcept
	{
	        NPL_ASSERT( !empty(), "fenwick_tree::back called on empty prefix_array" );

	        return *( this->end_ - 1 );
	}

	value_type const * data () const noexcept
	{ return std::to_address( this->begin_ ); }

	template< typename Arg >
	void _emplace_back ( Arg&& _arg_ )
	{
	        emplace_back( std::forward< Arg >( _arg_ ) );
	}

	void add    ( size_type _n_, value_type const &  _x_ );
	void update ( size_type _n_, value_type const &  _x_ );
	void update ( size_type _n_, value_type       && _x_ );

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

	void clear () noexcept
	{
	        size_type old_size = size();
	        _base::clear();
	        _annotate_shrink( old_size );
	        _invalidate_all_iterators();
	}

	bool _invariants () const;

#if NPL_DEBUG_LEVEL == 2
	bool _dereferenceable ( const_iterator const * _i_                     ) const;
	bool _decrementable   ( const_iterator const * _i_                     ) const;
	bool _addable         ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const;
	bool _subscriptable   ( const_iterator const * _i_, std::ptrdiff_t _n_ ) const;
#endif

private:
	size_type _p ( size_type _k_ ) const noexcept { return _k_ & -_k_; }

	value_type _sum_to_index ( size_type _n_                ) const noexcept;
	value_type _element_at   ( size_type _n_                ) const;
	value_type _range        ( size_type _x_, size_type _y_ ) const;

	void _add    ( size_type _n_, size_type _size_, value_type       && _x_ );
	void _add    ( size_type _n_, size_type _size_, value_type const &  _x_ );
	void _update ( size_type _n_, size_type _size_, value_type       && _x_ );
	void _update ( size_type _n_, size_type _size_, value_type const &  _x_ );

	void _verify_tree () noexcept;

	void _invalidate_all_iterators ();
	void _invalidate_iterators_past ( pointer _new_last_ );

	void _vallocate   ( size_type _n_ );
	void _vdeallocate (               ) noexcept;

	size_type _recommend ( size_type _new_size_ ) const;

	void _construct_at_end ( size_type _n_                      );
	void _construct_at_end ( size_type _n_, const_reference _x_ );

	template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
	typename std::enable_if_t
	<
		is_cpp17_forward_iterator_v< ForwardIterator >,
		void
	>
	_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, size_type _n_ );

	void _append ( size_type _n_                      );
	void _append ( size_type _n_, const_reference _x_ );

	      iterator _make_iter ( pointer _p_ )       noexcept;
	const_iterator _make_iter ( pointer _p_ ) const noexcept;

	void    _swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_              );
	pointer _swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_, pointer _p_ );

	void _move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ );

	void _move_assign ( fenwick_tree & _x_, std::true_type  ) noexcept( std::is_nothrow_move_assignable_v< allocator_type > );
	void _move_assign ( fenwick_tree & _x_, std::false_type ) noexcept( _alloc_traits::is_always_equal::value );

	void _destruct_at_end ( pointer _new_last_ ) noexcept
	{
		_invalidate_iterators_past( _new_last_ );
		size_type old_size = size();
		_base::_destruct_at_end( _new_last_ );
		_annotate_shrink( old_size );
	}

	template< typename U >
	inline void _push_back_slow_path ( U && _x_ );

	template< typename... Args >
	inline void _emplace_back_slow_path ( Args&&... _args_ );

#ifdef NPL_HAS_ASAN
	void _annotate_contiguous_container ( void const & _beg_, void const * _end_,
					      void const * _old_mid_,
					      void const * _new_mid_ ) const
	{
		if( _beg_ && std::is_same_v< allocator_type, _default_allocator_type > )
		{
			std::__sanitizer_annotate_contiguous_container( _beg_, _end_, _old_mid_, _new_mid_ );
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
	        explicit _construct_transaction ( fenwick_tree & _p_, size_type _n_ )
	                : p_( _p_ ), pos_( _p_.end_ ), new_end_( _p_.end_ + _n_ )
	        {
#ifdef NPL_HAS_ASAN
	                p_._annotate_increate( _n_ );
#endif
	        }

	        ~_construct_transaction ()
	        {
	                p_.end_ = pos_;
#ifdef NPL_HAS_ASAN
	                if( pos_ != new_end_ )
	                {
	                        p_._annotate_shrink( new_end_ - p_.begin_ );
	                }
#endif
	        }

	        fenwick_tree  &     p_;
	        pointer             pos_;
	        const_pointer const new_end_;

	private:
		_construct_transaction             ( _construct_transaction const & ) = delete;
		_construct_transaction & operator= ( _construct_transaction const & ) = delete;
	};

	void _update_impl ( size_type _n_, size_type _size_, _construct_transaction & tx, value_type const &  _x_ )
	{
		NPL_ASSERT( !empty() || _n_ >= _size_, "fenwick_tree::_update: bad args" );

		value_type current = _element_at( _n_++ );

		while( _n_ <= _size_ )
		{
			tx.pos_[ _n_ - 1 ] += _x_ - current;
			_n_ += _p( _n_ );
		}
	}

	void _update_impl ( size_type _n_, size_type _size_, split_buffer< value_type, allocator_type & > & _b_, value_type const & _x_ )
	{
		NPL_ASSERT( !empty() || _n_ >= _size_, "fenwick_tree::_update: bad args" );

		value_type current = _element_at( _n_++ );

		while( _n_ <= _size_ )
		{
			_b_.begin_[ _n_ - 1 ] += _x_ - current;
			_n_ += _p( _n_ );
		}
	}

	template< typename... Args >
	void _construct_one_at_end ( Args... _args_ )
	{
		// oh boi
		std::cout << "_construct_one_at_end( ... )\n";

		_construct_transaction tx( *this, 1 );

		if( this->begin_ == this->end_ )
		{
			_alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ), std::forward< Args >( _args_ )... );
		}
		else
		{
			_alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ) );

			_update( size() - 1, size(), std::forward< Args >( _args_ )... );
		}
		++tx.pos_;
	}
};

template< typename InputIterator,
	  typename Alloc = std::allocator< _iter_value_type< InputIterator > >,
	  typename = std::enable_if< _is_allocator< Alloc >::value >
>
fenwick_tree ( InputIterator, InputIterator )
	-> fenwick_tree< _iter_value_type< InputIterator >, Alloc >;

template< typename InputIterator,
	  typename Alloc,
	  typename = std::enable_if< _is_allocator< Alloc >::value >
>
fenwick_tree ( InputIterator, InputIterator, Alloc )
	-> fenwick_tree< _iter_value_type< InputIterator >, Alloc >;

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_ )
{
	_annotate_delete();

	mem::_construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, this->end_, _b_.begin_ );

	std::swap( this->begin_    , _b_.begin_     );
	std::swap( this->end_      , _b_.end_       );
	std::swap( this->_end_cap(), _b_._end_cap() );

	_b_.first_ = _b_.begin_;
	_annotate_new( size() );
	_invalidate_all_iterators();
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::pointer
fenwick_tree< T, Allocator >::_swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_, pointer _p_ )
{
	_annotate_delete();

	pointer r = _b_.begin_;

	mem::_construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, _p_, _b_.begin_ );
	mem::_construct_forward_with_exception_guarantees( this->_alloc(), _p_, this->end_, _b_.end_ );

	std::swap( this->begin_    , _b_.begin_     );
	std::swap( this->end_      , _b_.end_       );
	std::swap( this->_end_cap(), _b_._end_cap() );

	_b_.first_ = _b_.begin_;
	_annotate_new( size() );
	_invalidate_all_iterators();

	return r;
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::_sum_to_index ( size_type _n_ ) const noexcept
{
	value_type s = value_type{};
	_n_++;

	while( _n_ >= 1 )
	{
		s += this->begin_[ _n_ - 1 ];
		_n_ -= _p( _n_ );
	}

	return s;
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::_element_at ( size_type _n_ ) const
{
	NPL_ASSERT( !empty() || _n_ >= size(), "fenwick_tree::_element_at: bad args" );

	if( _n_ == 0 )
	{
		return this->begin_[ 0 ];
	}
	else
	{
		return _sum_to_index( _n_ ) - _sum_to_index( _n_ - 1 );
	}
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::_range ( size_type _x_, size_type _y_ ) const
{
	NPL_ASSERT( !empty() || _x_ >= size() || _y_ >= size(),
			"fenwick_tree::_range: bad args" );

	if( _x_ == 0 )
	{
		return _sum_to_index( _y_ );
	}
	else
	{
		return _sum_to_index( _y_ ) - _sum_to_index( _x_ - 1 );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_add ( size_type _n_, size_type _size_, value_type && _x_ )
{
	NPL_ASSERT( !empty() || _n_ >= _size_, "frenwick_tree:_add: bad args" );

	_n_++;

	while( _n_ <= _size_ )
	{
		this->begin_[ _n_ - 1 ] += _x_;
		_n_ += _p( _n_ );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_add ( size_type _n_, size_type _size_, value_type const & _x_ )
{
	NPL_ASSERT( !empty() || _n_ >= _size_, "frenwick_tree:_add: bad args" );

	_n_++;

	while( _n_ <= _size_ )
	{
		this->begin_[ _n_ - 1 ] += _x_;
		_n_ += _p( _n_ );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_update ( size_type _n_, size_type _size_, value_type && _x_ )
{
	NPL_ASSERT( !empty() || _n_ >= _size_, "frenwick_tree:_update: bad args" );

	value_type current = _element_at( _n_++ );

	while( _n_ <= _size_ )
	{
		this->begin_[ _n_ - 1 ] += _x_ - current;
		_n_ += _p( _n_ );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_update ( size_type _n_, size_type _size_, value_type const & _x_ )
{
	NPL_ASSERT( !empty() || _n_ >= _size_, "frenwick_tree:_update: bad args" );

	value_type current = _element_at( _n_++ );

	while( _n_ <= _size_ )
	{
		this->begin_[ _n_ - 1 ] += _x_ - current;
		_n_ += _p( _n_ );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_verify_tree () noexcept
{
	for( size_type i = 0; i < size(); ++i )
	{
		_add( i, size(), 0 );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_vallocate ( size_type _n_ )
{
	if( _n_ > max_size() )
	{
//j		this->_throw_length_error();
		throw std::length_error( "fenwick_tree::_vallocate: n > max size" );
	}

	this->begin_ = this->end_ = _alloc_traits::allocate( this->_alloc(), _n_ );
	this->_end_cap() = this->begin_ + _n_;

	_annotate_new( 0 );
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_vdeallocate () noexcept
{
	if( this->begin_ != nullptr )
	{
		clear();
		_alloc_traits::deallocate( this->_alloc(), this->begin_, capacity() );
		this->begin_ = this->end_ = this->_end_cap() = nullptr;
	}
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::size_type
fenwick_tree< T, Allocator >::max_size () const noexcept
{
	return std::min< size_type >( _alloc_traits::max_size( this->_alloc() ), std::numeric_limits< difference_type >::max() );
}

template< typename T, typename Allocator >
inline
typename fenwick_tree< T, Allocator >::size_type
fenwick_tree< T, Allocator >::_recommend ( size_type _new_size_ ) const
{
	size_type const ms = max_size();

	if( _new_size_ > ms )
	{
//		this->_throw_length_error();
		throw std::length_error( "fenwick_tree::_recommend: new size > max size" );
	}

	size_type const cap = capacity();

	if( cap >= ms / 2 )
	{
		return ms;
	}

	return std::max< size_type >( 1.618 * cap, _new_size_ );
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_construct_at_end ( size_type _n_ )
{
	_construct_transaction tx( *this, _n_ );
	const_pointer new_end = tx.new_end_;

	for( pointer pos = tx.pos_; pos != new_end; ++pos, tx.pos_ = pos )
	{
		_alloc_traits::construct( this->_alloc(), std::to_address( pos ) );
	}
}

template< typename T, typename Allocator >
inline
void
fenwick_tree< T, Allocator >::_construct_at_end ( size_type _n_, const_reference _x_ )
{
	for( size_type i = 0; i < _n_; ++i )
	{
		_construct_one_at_end( _x_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator, typename U >
typename std::enable_if_t
<
	is_cpp17_forward_iterator_v< ForwardIterator >,
	void
>
fenwick_tree< T, Allocator >::_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, [[ maybe_unused ]] size_type _n_ )
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
fenwick_tree< T, Allocator >::_append ( size_type _n_ )
{
	if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
	{
		this->_construct_at_end( _n_ );
	}
	else
	{
		allocator_type & a = this->_alloc();
		split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
		b._construct_at_end( _n_ );
		_swap_out_circular_buffer( b );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_append ( size_type _n_, const_reference _x_ )
{
	if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
	{
		this->_construct_at_end( _n_, _x_ );
	}
	else
	{
		allocator_type & a = this->_alloc();
		split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
		b._construct_at_end( _n_, _x_ );
		_swap_out_circular_buffer( b );
	}
}

template< typename T, typename Allocator >
fenwick_tree< T, Allocator >::fenwick_tree ( size_type _n_ )
{
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
	}
}

template< typename T, typename Allocator >
fenwick_tree< T, Allocator >::fenwick_tree ( size_type _n_, allocator_type const & _a_ )
	: _base( _a_ )
{
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
	}
}

template< typename T, typename Allocator >
fenwick_tree< T, Allocator >::fenwick_tree ( size_type _n_, value_type const & _x_ )
{
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _n_, _x_ );
	}
}

template< typename T, typename Allocator >
fenwick_tree< T, Allocator >::fenwick_tree ( size_type _n_, value_type const & _x_, allocator_type const & _a_ )
	: _base( _a_ )
{
	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _n_, _x_ );
	}
}

template< typename T, typename Allocator >
template< typename InputIterator, typename U >
fenwick_tree< T, Allocator >::fenwick_tree ( InputIterator _first_,
						typename std::enable_if_t< is_cpp17_input_iterator_v  < InputIterator > &&
									  !is_cpp17_forward_iterator_v< InputIterator > &&
									   std::is_constructible_v<
									   	value_type,
										typename std::iterator_traits< InputIterator >::reference
									 >,
									 InputIterator > _last_ )
{
	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename InputIterator, typename U >
fenwick_tree< T, Allocator >::fenwick_tree ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
						typename std::enable_if_t< is_cpp17_input_iterator_v  < InputIterator > &&
									  !is_cpp17_forward_iterator_v< InputIterator > &&
									   std::is_constructible_v<
									   	value_type,
										typename std::iterator_traits< InputIterator >::reference
									 > > * )
	: _base( _a_ )
{
	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator, typename U >
fenwick_tree< T, Allocator >::fenwick_tree ( ForwardIterator _first_,
						typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
									   std::is_constructible_v<
									   	value_type,
										typename std::iterator_traits< ForwardIterator >::reference
									 >,
									 ForwardIterator > _last_ )
{
	size_type _n_ = static_cast< size_type >( std::distance( _first_, _last_ ) );

	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _first_, _last_, _n_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator, typename U >
fenwick_tree< T, Allocator >::fenwick_tree ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
						typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
									   std::is_constructible_v<
									   	value_type,
										typename std::iterator_traits< ForwardIterator >::reference
									 > > * )
	: _base( _a_ )
{
	size_type _n_ = static_cast< size_type >( std::distance( _first_, _last_ ) );

	if( _n_ > 0 )
	{
		_vallocate( _n_ );
		_construct_at_end( _first_, _last_, _n_ );
	}
}

template< typename T, typename Allocator >
fenwick_tree< T, Allocator >::fenwick_tree ( fenwick_tree const & _x_ )
	: _base( _alloc_traits::select_on_container_copy_construction( _x_._alloc() ) )
{
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
fenwick_tree< T, Allocator >::fenwick_tree ( fenwick_tree const & _x_, _identity< allocator_type > const & _a_ )
	: _base( _a_ )
{
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
fenwick_tree< T, Allocator >::fenwick_tree ( fenwick_tree && _x_ ) noexcept
	: _base( std::move( _x_._alloc() ) )
{
	this->begin_     = _x_.begin_;
	this->end_       = _x_.end_;
	this->_end_cap() = _x_._end_cap();
	_x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
}

template< typename T, typename Allocator >
inline
fenwick_tree< T, Allocator >::fenwick_tree ( fenwick_tree && _x_, _identity< allocator_type > const & _a_ )
	: _base( _a_ )
{
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
fenwick_tree< T, Allocator >::fenwick_tree ( std::initializer_list< value_type > _list_ )
{
	if( _list_.size() > 0 )
	{
		_vallocate( _list_.size() );
		_construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
	}
}

template< typename T, typename Allocator >
inline
fenwick_tree< T, Allocator >::fenwick_tree ( std::initializer_list< value_type > _list_, allocator_type const & _a_ )
	: _base( _a_ )
{
	if( _list_.size() )
	{
		_vallocate( _list_.size() );
		_construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
	}
}

template< typename T, typename Allocator >
inline
fenwick_tree< T, Allocator > &
fenwick_tree< T, Allocator >::operator= ( fenwick_tree && _x_ )
	noexcept( ( noexcept_move_assign_container_v< Allocator, _alloc_traits > ) )
{
	_move_assign( _x_, std::integral_constant< bool,
			_alloc_traits::propagate_on_container_move_assignment::value >() );
	return *this;
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_move_assign ( fenwick_tree & _x_, std::false_type )
	noexcept( _alloc_traits::is_always_equal::value )
{
	if( _base::_alloc() != _x_._alloc() )
	{
		using _ip = std::move_iterator< pointer >;
		assign( _ip( _x_.begin_ ), _ip( _x_.end_ ) );
	}
	else
	{
		_move_assign( _x_, std::true_type() );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_move_assign ( fenwick_tree & _x_, std::true_type )
	noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
{
	_vdeallocate();
	_base::_move_assign_alloc( _x_ ); // could throw

	this->begin_     = _x_.begin_;
	this->end_       = _x_.end_;
	this->_end_cap() = _x_._end_cap();

	_x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
}

template< typename T, typename Allocator >
inline
fenwick_tree< T, Allocator > &
fenwick_tree< T, Allocator >::operator= ( fenwick_tree const & _x_ )
{
	if( this != &_x_ )
	{
		_base::_copy_assign_alloc( _x_ );
		assign( _x_.begin_, _x_.end_ );
	}
	return *this;
}

template< typename T, typename Allocator >
template< typename InputIterator, typename U >
typename std::enable_if_t
<
	 is_cpp17_input_iterator_v  < InputIterator > &&
	!is_cpp17_forward_iterator_v< InputIterator > &&
	 std::is_constructible_v<
	 	T,
		typename std::iterator_traits< InputIterator >::reference
	>,
	void
>
fenwick_tree< T, Allocator >::assign ( InputIterator _first_, InputIterator _last_ )
{
	clear();

	for( ; _first_ != _last_; ++_first_ )
	{
		_emplace_back( *_first_ );
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator, typename U >
typename std::enable_if_t
<
	is_cpp17_forward_iterator_v< ForwardIterator > &&
	std::is_constructible_v<
		T,
		typename std::iterator_traits< ForwardIterator >::reference
	>,
	void
>
fenwick_tree< T, Allocator >::assign ( ForwardIterator _first_, ForwardIterator _last_ )
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
fenwick_tree< T, Allocator >::assign ( size_type _n_, const_reference _x_ )
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
			this->destruct_at_end( this->begin_ + _n_ );
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
typename fenwick_tree< T, Allocator >::iterator
fenwick_tree< T, Allocator >::_make_iter ( pointer _p_ ) noexcept
{
#if NPL_DEBUG_LEVEL == 2
	return iterator( this, _p_ );
#else
	return iterator( _p_ );
#endif
}

template< typename T, typename Allocator >
inline
typename fenwick_tree< T, Allocator >::const_iterator
fenwick_tree< T, Allocator >::_make_iter ( pointer _p_ ) const noexcept
{
#if NPL_DEBUG_LEVEL == 2
	return const_iterator( this, _p_ );
#else
	return const_iterator( _p_ );
#endif
}

template< typename T, typename Allocator >
inline
typename fenwick_tree< T, Allocator >::const_iterator
fenwick_tree< T, Allocator >::begin () const noexcept
{
	return _make_iter( this->begin_ );
}

template< typename T, typename Allocator >
inline
typename fenwick_tree< T, Allocator >::const_iterator
fenwick_tree< T, Allocator >::end () const noexcept
{
	return _make_iter( this->end_ );
}

template< typename T, typename Allocator >
inline
typename fenwick_tree< T, Allocator >::const_reference
fenwick_tree< T, Allocator >::operator[] ( size_type _n_ ) const noexcept
{
	NPL_ASSERT( _n_ < size(), "fenwick_tree[] index out of bounds" );
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
bool
fenwick_tree< T, Allocator >::operator== ( fenwick_tree< T, Allocator > const & _rhs_ ) const noexcept
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
typename fenwick_tree< T, Allocator >::const_reference
fenwick_tree< T, Allocator >::at ( size_type _n_ ) const
{
	if( empty() || _n_ >= size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::at: index out of bounds" );
	}
	return this->begin_[ _n_ ];
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::element_at ( size_type _n_ ) const
{
	if( empty() || _n_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::element_at: index out of bounds" );
	}
	return _element_at( _n_ );
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::range () const
{
	return _range( 0, size() - 1 );
}

template< typename T, typename Allocator >
typename fenwick_tree< T, Allocator >::value_type
fenwick_tree< T, Allocator >::range ( size_type _x_, size_type _y_ ) const
{
	if( empty() || _x_ > size() || _y_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::range: index out of bounds" );
	}

	return _range( _x_, _y_ );
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::reserve ( size_type _n_ )
{
	if( _n_ > capacity() )
	{
		allocator_type & a = this->_alloc();
		split_buffer< value_type, allocator_type & > b( _n_, size(), a );
		_swap_out_circular_buffer( b );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::shrink_to_fit () noexcept
{
	if( capacity() > size() )
	{
#ifndef NPL_NO_EXCEPTIONS
		try
		{
#endif
			allocator_type & a = this->_alloc();
			split_buffer< value_type, allocator_type & > b( size(), size(), a );
			_swap_out_circular_buffer( b );
#ifndef NPL_NO_EXCEPTIONS
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
fenwick_tree< T, Allocator >::_push_back_slow_path ( U && _x_ )
{
	// oh boi

	std::cout << "_push_back_slow_path( U&& )\n";

	allocator_type & a = this->_alloc();

	split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), size(), a );

	if( this->begin_ == this->end_ )
	{
		_alloc_traits::construct( a, std::to_address( b.end_ ), std::forward< U >( _x_ ) );
	}
	else
	{
		_alloc_traits::construct( a, std::to_address( b.end_ ) );

		_update( size() - 1, size(), std::forward< U >( _x_ ) );
	}

	b.end_++;
	_swap_out_circular_buffer( b );
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::add ( size_type _n_, value_type const & _x_ )
{
	if( empty() || _n_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::add: index out of bounds" );
	}

	_n_++;

	while( _n_ <= size() )
	{
		this->begin_[ _n_ - 1 ] += _x_;
		_n_ += _p( _n_ );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::update ( size_type _n_, value_type const & _x_ )
{
	// oh boi
	if( empty() || _n_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::update: index out of bounds" );
	}

	value_type current = element_at( _n_ );

	int pos = static_cast< int >( _n_ + 1 );

	while( pos <= static_cast< int >( size() + 1 ) )
	{
		this->begin_[ pos - 1 ] += _x_ - current;
		pos += _p( pos );
	}
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::update ( size_type _n_, value_type && _x_ )
{
	if( empty() || _n_ > size() )
	{
//		this->_throw_out_of_range();
		throw std::out_of_range( "fenwick_tree::update: index out of bounds" );
	}

	value_type current = element_at( _n_ );

	int pos = static_cast< int >( _n_ + 1 );

	while( pos <= static_cast< int >( size() + 1 ) )
	{
		this->begin_[ pos - 1 ] += _x_ - current;
		pos += _p( pos );
	}
}

template< typename T, typename Allocator >
inline
void
fenwick_tree< T, Allocator >::push_back ( const_reference _x_ )
{
	if( this->end_ < this->_end_cap() )
	{
		_construct_one_at_end( _x_ );
	}
	else
	{
		_push_back_slow_path( _x_ );
	}

	_verify_tree();
}

template< typename T, typename Allocator >
inline
void
fenwick_tree< T, Allocator >::push_back ( value_type && _x_ )
{
	if( this->end_ < this->_end_cap() )
	{
		_construct_one_at_end( std::move( _x_ ) );
	}
	else
	{
		_push_back_slow_path( std::move( _x_ ) );
	}

	_verify_tree();
}

template< typename T, typename Allocator >
template< typename... Args >
void
fenwick_tree< T, Allocator >::_emplace_back_slow_path ( Args&&... _args_ )
{
	// oh boi

	std::cout << "_emplace_back_slow_path\n";

	allocator_type & a = this->_alloc();

	split_buffer< value_type, allocator_type & > b( _recommend( size() + 1 ), size(), a );

	if( this->begin_ == this->end_ )
	{
		_alloc_traits::construct( a, std::to_address( b.end_ ), std::forward< Args >( _args_ )... );
	}
	else
	{
		_alloc_traits::construct( a, std::to_address( b.end_ ) );

		_update( size() - 1, size(), std::forward< Args >( _args_ )... );
	}

	b.end_++;
	_swap_out_circular_buffer( b );
}

template< typename T, typename Allocator >
template< typename... Args >
inline
typename fenwick_tree< T, Allocator >::reference
fenwick_tree< T, Allocator >::emplace_back ( Args&&... _args_ )
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
fenwick_tree< T, Allocator >::pop_back ()
{
	NPL_ASSERT( !empty(), "fenwick_tree::pop_back called on empty fenwick_tree" );

	this->_destruct_at_end( this->end_ - 1 );
}

template< typename T, typename Allocator >
void
fenwick_tree< T, Allocator >::_move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ )
{
	pointer old_last = this->end_;
	difference_type n = old_last - _to_;
	{
		pointer i = _from_s_ + n;
		_construct_transaction tx( *this, _from_e_ - i );

		for( pointer pos = tx.pos_; i < _from_e_;
				++i, ++pos, tx.pos_ = pos )
		{
			_alloc_traits::construct( this->_alloc(), std::to_address( pos ), std::move( *i ) );
		}
	}

	std::move_backward( _from_s_, _from_s_ + n, old_last );
}

template< typename T, typename Allocator >
inline
void
fenwick_tree< T, Allocator >::_invalidate_all_iterators ()
{
#if NPL_DEBUG_LEVEL == 2
#endif
}

template< typename T, typename Allocator >
inline
void
fenwick_tree< T, Allocator >::_invalidate_iterators_past ( pointer _new_last_ )
{
#if NPL_DEBUG_LEVEL == 2
#else
	( ( void ) _new_last_ );
#endif
}


} // namespace rq

} // namespace npl
