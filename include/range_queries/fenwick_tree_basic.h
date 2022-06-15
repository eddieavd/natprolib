//
//	fenwick_tree_basic.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <type_traits>
#include <iterator>
#include <stdexcept>

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


template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree;

template< typename T, bool C,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree_iterator
{
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

	void alloc  (                        ) { head_ = ( T* ) std::calloc( NPL_RQ_DEFAULT_CAPACITY, sizeof( T ) ); if( !head_ ) throw std::bad_alloc(); }
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

	fenwick_tree (                                     ) : capacity_ { NPL_RQ_DEFAULT_CAPACITY } { alloc(            ); }
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


} // namespace rq

} // namespace npl
