//
//	split_buffer.h
//	natprolib
//
//	Created by Edhem Avdagic on 16/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <algorithm>

#include "mem.h"
#include "traits.h"
#include "compressed_pair.h"


namespace npl
{


template< bool >
class _split_buffer_common
{
protected:
	[[ noreturn ]] void _throw_length_error () const;
	[[ noreturn ]] void _throw_out_of_range () const;
};

template< typename T, typename Allocator = std::allocator< T > >
struct split_buffer
	: private _split_buffer_common< true >
{
private:
	split_buffer ( split_buffer const & );
	split_buffer & operator= ( split_buffer const & );
public:
	using      value_type = T;
	using  allocator_type = Allocator;
	using       _alloc_rr = typename std::remove_reference_t< allocator_type >;
	using   _alloc_traits = std::allocator_traits< _alloc_rr >;
	using       reference = value_type &;
	using const_reference = value_type const &;
	using       size_type = typename _alloc_traits::size_type;
	using difference_type = typename _alloc_traits::difference_type;
	using         pointer = typename _alloc_traits::pointer;
	using   const_pointer = typename _alloc_traits::const_pointer;
	using        iterator = pointer;
	using  const_iterator = const_pointer;

	pointer first_;
	pointer begin_;
	pointer end_;
	compressed_pair< pointer, allocator_type > end_cap_;

	using _alloc_ref       = std::add_lvalue_reference_t< allocator_type >;
	using _alloc_const_ref = std::add_lvalue_reference_t< allocator_type >;

	_alloc_rr       & _alloc   ()       noexcept { return end_cap_.second(); }
	_alloc_rr const & _alloc   () const noexcept { return end_cap_.second(); }
	pointer         & _end_cap ()       noexcept { return end_cap_.first();  }
	pointer   const & _end_cap () const noexcept { return end_cap_.first();  }

	split_buffer () noexcept( std::is_nothrow_default_constructible_v< allocator_type > );

	explicit split_buffer ( _alloc_rr       & _a_ );
	explicit split_buffer ( _alloc_rr const & _a_ );

	split_buffer ( size_type _cap_, size_type _start_, _alloc_rr & _a_ );

	~split_buffer ();

	split_buffer ( split_buffer && _c_ )
		noexcept( std::is_nothrow_move_constructible_v< allocator_type > );
	split_buffer ( split_buffer && _c_, _alloc_rr const & _a_ );

	split_buffer & operator= ( split_buffer && _c_ )
		noexcept( ( _alloc_traits::propagate_on_container_move_assignment::value &&
					std::is_nothrow_move_assignable_v< allocator_type > ) ||
				!_alloc_traits::propagate_on_container_move_assignment::value );

	      iterator begin ()       noexcept { return begin_; }
	const_iterator begin () const noexcept { return begin_; }
	      iterator   end ()       noexcept { return end_;   }
	const_iterator   end () const noexcept { return end_;   }

	void clear () noexcept { _destruct_at_end( begin_ ); }

	size_type     size () const { return static_cast< size_type >( end_ - begin_ ); }
	bool         empty () const { return end_ == begin_; }
	size_type capacity () const { return static_cast< size_type >( begin_ - _end_cap() ); }

	size_type _front_spare () const { return static_cast< size_type >( begin_ - first_ ); }
	size_type  _back_spare () const { return static_cast< size_type >( _end_cap() - end_ ); }

	      reference front ()       { return *begin_; }
	const_reference front () const { return *begin_; }
	      reference  back ()       { return *( end_ - 1 ); }
	const_reference  back () const { return *( end_ - 1 ); }

	void reserve ( size_type _n_ );
	void shrink_to_fit () noexcept;

	void push_front ( const_reference _x_ );
	void push_back  ( const_reference _x_ );
	void push_front ( value_type && _x_ );
	void push_back  ( value_type && _x_ );
	template< typename... Args >
	void emplace_back ( Args&&... _args_ );

	void pop_front () { _destruct_at_begin( begin_ + 1 ); }
	void pop_back  () { _destruct_at_end  ( end_   - 1 ); }

	void _construct_at_end ( size_type _n_                      );
	void _construct_at_end ( size_type _n_, const_reference _x_ );

	template< typename InputIterator >
		typename std::enable_if_t
		<
			 is_cpp17_input_iterator_v  < InputIterator > &&
			!is_cpp17_forward_iterator_v< InputIterator >,
			 void
		>
	_construct_at_end ( InputIterator _first_, InputIterator _last_ );

	template< typename ForwardIterator >
		typename std::enable_if_t
		<
			is_cpp17_forward_iterator_v< ForwardIterator >,
			void
		>
	_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_ );

	void _destruct_at_begin ( pointer _new_begin_ )
	{ _destruct_at_begin( _new_begin_, std::is_trivially_destructible< value_type >() ); }

	void _destruct_at_begin ( pointer _new_begin_, std::false_type );
	void _destruct_at_begin ( pointer _new_begin_, std::true_type  );

	void _destruct_at_end ( pointer _new_last_ ) noexcept
	{ _destruct_at_end( _new_last_, std::false_type() ); }

	void _destruct_at_end ( pointer _new_last_, std::false_type ) noexcept;
	void _destruct_at_end ( pointer _new_last_, std::true_type  ) noexcept;

	void swap ( split_buffer & _x_ )
		noexcept( !_alloc_traits::propagate_on_container_swap::value ||
				std::is_nothrow_swappable< _alloc_rr >::value );

	bool _invariants () const;

private:
	void _move_assign_alloc ( split_buffer & _x_, std::true_type )
		noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
	{
		_alloc() = std::move( _x_._alloc() );
	}
	void _move_assign_alloc ( split_buffer &, std::false_type ) noexcept {}

	struct _construct_transaction
	{
		explicit _construct_transaction ( pointer * _p_, size_type _n_ ) noexcept
			: pos_( *_p_ ), end_( *_p_ + _n_ ), dest_( _p_ )
		{
		}
		~_construct_transaction ()
		{
			*dest_ = pos_;
		}

		pointer pos_;
		pointer const end_;
	private:
		pointer *dest_;
	};
};

template< typename T, typename Allocator >
bool
split_buffer< T, Allocator >::_invariants () const
{
	if( first_ == nullptr )
	{
		if( begin_ != nullptr )
		{
			return false;
		}
		if( end_ != nullptr )
		{
			return false;
		}
		if( _end_cap() != nullptr )
		{
			return false;
		}
	}
	else
	{
		if( begin_ < first_ )
		{
			return false;
		}
		if( end_ < begin_ )
		{
			return false;
		}
		if( _end_cap() < end_ )
		{
			return false;
		}
	}
	return true;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::_construct_at_end ( size_type _n_ )
{
	_construct_transaction tx( &this->end_, _n_ );

	for( ; tx.pos_ != tx.end_; ++tx.pos_ )
	{
		_alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ) );
	}
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::_construct_at_end ( size_type _n_, const_reference _x_ )
{
	_construct_transaction tx( &this->end_, _n_ );

	for( ; tx.pos_ != tx.end_; ++tx.pos_ )
	{
		_alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ), _x_ );
	}
}

template< typename T, typename Allocator >
template< typename InputIterator >
typename std::enable_if_t
<
	 is_cpp17_input_iterator_v  < InputIterator > &&
	!is_cpp17_forward_iterator_v< InputIterator >,
	 void
>
split_buffer< T, Allocator >::_construct_at_end ( InputIterator _first_, InputIterator _last_ )
{
	_alloc_rr & _a_ = this->_alloc();

	for( ; _first_ != _last_; ++_first_ )
	{
		if( end_ == _end_cap() )
		{
			size_type old_cap = _end_cap() - _first_;
			size_type new_cap = std::max< size_type >( 2 * old_cap, 8 );

			split_buffer buf( new_cap, 0, _a_ );

			for( pointer p = begin_; p != end_; ++p, ++buf.end_ )
			{
				_alloc_traits::construct( buf._alloc(),
						std::to_address( buf.end_ ), std::move( *p ) );
			}
			swap( buf );
		}
		_alloc_traits::construct( _a_, std::to_address( this->end_ ), *_first_ );
		++this->end_;
	}
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
typename std::enable_if_t
<
	is_cpp17_forward_iterator_v< ForwardIterator >,
	void
>
split_buffer< T, Allocator >::_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_ )
{
	_construct_transaction tx( &this->end_, std::distance( _first_, _last_ ) );

	for( ; tx.pos_ != tx.end_; ++tx.pos_, ++_first_ )
	{
		_alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ), *_first_ );
	}
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_begin ( pointer _new_begin_, std::false_type )
{
	while( begin_ != _new_begin_ )
	{
		_alloc_traits::destroy( _alloc(), std::to_address( begin_++ ) );
	}
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_begin ( pointer _new_begin_, std::true_type )
{
	begin_ = _new_begin_;
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_end ( pointer _new_last_, std::false_type ) noexcept
{
	while( _new_last_ != end_ )
	{
		_alloc_traits::destroy( _alloc(), std::to_address( --end_ ) );
	}
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_end ( pointer _new_last_, std::true_type ) noexcept
{
	end_ = _new_last_;
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( size_type _cap_, size_type _start_, _alloc_rr & _a_ )
	: end_cap_( nullptr, _a_ )
{
	first_ = _cap_ != 0 ? _alloc_traits::allocate( _alloc(), _cap_ ) : nullptr;
	begin_ = end_ = first_ + _start_;
	_end_cap() = first_ + _cap_;
}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ()
	noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
	: first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr, _default_init_tag() )
{
}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ( _alloc_rr & _a_ )
	: first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr, _a_ )
{
}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ( _alloc_rr const & _a_ )
	: first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr, _a_ )
{
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::~split_buffer ()
{
	clear();
	if( first_ )
	{
		_alloc_traits::deallocate( _alloc(), first_, capacity() );
	}
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( split_buffer && _x_ )
	noexcept( std::is_nothrow_move_constructible_v< allocator_type > )
	: first_  ( std::move( _x_.first_ ) ),
	  begin_  ( std::move( _x_.begin_ ) ),
	  end_    ( std::move( _x_.end_   ) ),
	  end_cap_( std::move( _x_.end_cap_ ) )
{
	_x_.first_     = nullptr;
	_x_.begin_     = nullptr;
	_x_.end_       = nullptr;
	_x_._end_cap() = nullptr;
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( split_buffer && _x_, _alloc_rr const & _a_ )
	: end_cap_( nullptr, _a_ )
{
	if( _a_ == _x_._alloc() )
	{
		first_     = _x_.first_;
		begin_     = _x_.begin_;
		end_       = _x_.end_;
		_end_cap() = _x_._end_cap();

		_x_.first_     = nullptr;
		_x_.begin_     = nullptr;
		_x_.end_       = nullptr;
		_x_._end_cap() = nullptr;
	}
	else
	{
		size_type cap = _x_.size();

		first_ = _alloc_traits::allocate( _alloc(), cap );
		begin_ = end_ = first_;
		_end_cap() = first_ + cap;

		using _ip = std::move_iterator< iterator >;
		_construct_at_end( _ip( _x_.begin() ), _ip( _x_.end() ) );
	}
}

template< typename T, typename Allocator >
split_buffer< T, Allocator > &
split_buffer< T, Allocator >::operator= ( split_buffer && _x_ )
	noexcept( ( _alloc_traits::propagate_on_container_move_assignment::value &&
		    std::is_nothrow_move_assignable_v< allocator_type > ) ||
		   !_alloc_traits::propagate_on_container_move_assignment::value )
{
	clear();
	shrink_to_fit();

	first_     = _x_.first_;
	begin_     = _x_.begin_;
	end_       = _x_.end_;
	_end_cap() = _x_._end_cap();

	_move_assign_alloc( _x_, std::integral_constant< bool, _alloc_traits::propagate_on_container_move_assignment::value >() );

	_x_.first_ = _x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
	return *this;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::swap ( split_buffer & _x_ )
	noexcept( !_alloc_traits::propagate_on_container_swap::value ||
			std::is_nothrow_swappable< _alloc_rr >::value )
{
	std::swap( first_    , _x_.first_     );
	std::swap( begin_    , _x_.begin_     );
	std::swap( end_      , _x_.end_       );
	std::swap( _end_cap(), _x_._end_cap() );

	mem::_swap_allocator( _alloc(), _x_._alloc() );
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::reserve ( size_type _n_ )
{
	if( _n_ < capacity() )
	{
		split_buffer< value_type, _alloc_rr& > t( _n_, 0, _alloc() );

		t._construct_at_end( std::move_iterator< pointer >( begin_ ),
				     std::move_iterator< pointer >( end_   ) );

		std::swap( first_    , t.first_ );
		std::swap( begin_    , t.begin_ );
		std::swap( end_      , t.end_   );
		std::swap( _end_cap(), t._end_cap() );
	}
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::shrink_to_fit () noexcept
{
	if( capacity() > size() )
	{
#ifndef NPL_NO_EXCEPTIONS
		try
		{
#endif
			split_buffer< value_type, _alloc_rr& > t( size(), 0, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );
			t.end_ = t.begin_ + ( end_ - begin_ );

			std::swap( first_    , t.first_ );
			std::swap( begin_    , t.begin_ );
			std::swap( end_      , t.end_   );
			std::swap( _end_cap(), t._end_cap() );
#ifndef NPL_NO_EXCEPTIONS
		}
		catch ( ... )
		{
		}
#endif
	}
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::push_front ( const_reference _x_ )
{
	if( begin_ == first_ )
	{
		if( end_ < _end_cap() )
		{
			difference_type d = _end_cap() - end_;

			d = ( d + 1 ) / 2;
			begin_ = std::move_backward( begin_, end_, end_ + d );
			end_ += d;
		}
		else
		{
			size_type c = std::max< size_type >( 2 * static_cast< std::size_t >( _end_cap() - first_ ), 1 );

			split_buffer< value_type, _alloc_rr& > t( c, ( c + 3 ) / 4, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );

			std::swap( first_    , t.first_     );
			std::swap( begin_    , t.begin_     );
			std::swap( end_      , t.end_       );
			std::swap( _end_cap(), t._end_cap() );
		}
	}
	_alloc_traits::construct( _alloc(), std::to_address( begin_ - 1 ), _x_ );
	--begin_;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::push_front( value_type && _x_ )
{
	if( begin_ == first_ )
	{
		if( end_ < _end_cap() )
		{
			difference_type d = _end_cap() - end_;

			d = ( d + 1 ) / 2;
			begin_ = std::move_backward( begin_, end_, end_ + d );
			end_ += d;
		}
		else
		{
			size_type c = std::max< size_type >( 2 * static_cast< std::size_t >( _end_cap() - first_ ), 1 );

			split_buffer< value_type, _alloc_rr& > t( c, ( c + 3 ) / 4, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );

			std::swap( first_    , t.first_     );
			std::swap( begin_    , t.begin_     );
			std::swap( end_      , t.end_       );
			std::swap( _end_cap(), t._end_cap() );
		}
	}
	_alloc_traits::construct( _alloc(), std::to_address( begin_ - 1 ), std::move( _x_ ) );
	--begin_;
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::push_back ( const_reference _x_ )
{
	if( end_ == _end_cap() )
	{
		if( begin_ > first_ )
		{
			difference_type d = begin_ - first_;

			d = ( d + 1 ) / 2;
			end_ = std::move( begin_, end_, begin_ - d );
			begin_ -= d;
		}
		else
		{
			size_type c = std::max< size_type >( 2 * static_cast< std::size_t >( _end_cap() - first_ ), 1 );

			split_buffer< value_type, _alloc_rr& > t( c, c / 4, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );

			std::swap( first_    , t.first_     );
			std::swap( begin_    , t.begin_     );
			std::swap( end_      , t.end_       );
			std::swap( _end_cap(), t._end_cap() );
		}
	}
	_alloc_traits::construct( _alloc(), std::to_address( end_ ), _x_ );
	++end_;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::push_back ( value_type && _x_ )
{
	if( end_ == _end_cap() )
	{
		if( begin_ > first_ )
		{
			difference_type d = begin_ - first_;

			d = ( d + 1 ) / 2;
			end_ = std::move( begin_, end_, begin_ - d );
			begin_ -= d;
		}
		else
		{
			size_type c = std::max< size_type >( 2 * static_Cast< std::size_t >( _end_cap() - first_ ), 1 );

			split_buffer< value_type, _alloc_rr& > t( c, c / 4, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );

			std::swap( first_   , t.first_      );
			std::swap( begin_   , t.begin_      );
			std::swap( end_     , t.end_        );
			std::swap( _end_cap(), t._end_cap() );
		}
	}
	_alloc_traits::construct( _alloc(), std::to_address( end_ ), std::move( _x_ ) );
	++end_;
}

template< typename T, typename Allocator >
template< typename... Args >
void
split_buffer< T, Allocator >::emplace_back ( Args&&... _args_ )
{
	if( end_ == _end_cap() )
	{
		if( begin_ > first_ )
		{
			difference_type d = begin_ - first_;

			d = ( d + 1 ) / 2;
			end_ = std::move( begin_, end_, begin_ - d );
			begin_ -= d;
		}
		else
		{
			size_type c = std::max< size_type >( 2 * static_cast< std::size_t >( _end_cap() - first_ ), 1 );

			split_buffer< value_type, _alloc_rr& > t( c, c / 4, _alloc() );

			t._construct_at_end( std::move_iterator< pointer >( begin_ ),
					     std::move_iterator< pointer >( end_   ) );

			std::swap( first_    , t.first_     );
			std::swap( begin_    , t.begin_     );
			std::swap( end_      , t.end_       );
			std::swap( _end_cap(), t._end_cap() );
		}
	}
	_alloc_traits::construct( _alloc(), std::to_address( end_ ), std::forward< Args >( _args_ )... );
	++end_;
}

template< typename T, typename Allocator >
inline
void
swap ( split_buffer< T, Allocator > & _x_, split_buffer< T, Allocator > & _y_ )
	noexcept( noexcept( _x_.swap( _y_ ) ) )
{
	_x_.swap( _y_ );
}


} // namespace npl
