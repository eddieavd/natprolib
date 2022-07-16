//
//	segment_tree_basic.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <concepts>

#include "../util/traits.h"
#include "../util/util.h"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <unistd.h>
#endif


namespace npl
{

namespace rq
{


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

	segment_tree (                                                                      ) : parent_builder_ { pb_default_< T > } { alloc( NPL_RQ_DEFAULT_CAPACITY ); }
	segment_tree (                                      ParentBuilder< T > auto && _pb_ ) : parent_builder_ { _pb_ } { alloc( NPL_RQ_DEFAULT_CAPACITY ); }
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

		head_[ _index_ ] = NPL_MOVE( _value_ );

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

			head_[ capacity_ / 2 + size_ ] = NPL_MOVE( _value_ );
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

		head_[ capacity_ / 2 + size_ ] = NPL_MOVE( T ( args... ) );
		size_++;
		construct_tree();
	}

	~segment_tree () { free( head_ ); }
};


} // namespace rq

} // namespace npl
