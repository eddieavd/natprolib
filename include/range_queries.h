//
//  range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cmath>
#include <memory>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include <concepts>

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


/**
 *  prefix array
 *  container holding sum of all previous elements at each index
 *  allows for constant-time sum queries on any interval
 */
template< typename T >
class prefix_array;

template< typename T, bool C >
class prefix_array_iterator
{
	friend class prefix_array< T >;
	friend class prefix_array_iterator< T, !C >;

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
	bool operator== ( prefix_array_iterator< T, R > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( prefix_array_iterator< T, R > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	operator prefix_array_iterator< T, true > () const
	{ return prefix_array_iterator< T, true >{ ptr_ }; }

private:
	pointer ptr_;

	explicit prefix_array_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
};

template< typename T >
class prefix_array
{
public:
	using     value_type = T;
	using       iterator = prefix_array_iterator< T, false >;
	using const_iterator = prefix_array_iterator< T,  true >;

	auto begin ()       { return       iterator{ data_         }; }
	auto   end ()       { return       iterator{ data_ + size_ }; }
	auto begin () const { return const_iterator{ data_         }; }
	auto   end () const { return const_iterator{ data_ + size_ }; }

	prefix_array (                                     ) : capacity_ { DEFAULT_CAPACITY } { alloc(); }
	prefix_array (              std::size_t _capacity_ ) : capacity_ {       _capacity_ } { alloc(); }
	prefix_array ( T ** _head_, std::size_t _capacity_ ) : capacity_ {       _capacity_ }
	{
		alloc();

		for( std::size_t i = 0; i < capacity_; ++i )
		{
			push_back( ( *_head_ )[ i ] );
		}

		*_head_ = nullptr;
	}
	prefix_array ( std::size_t _count_, T _value_ ) : capacity_ { _count_ }
	{
		alloc();

		for( std::size_t i = 0; i < capacity_; ++i )
		{
			push_back( _value_ );
		}
	}
	template< typename Iterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< Iterator >::value_type, void > > >
	prefix_array ( Iterator begin, Iterator end ) : capacity_ { static_cast< std::size_t >( std::distance( begin, end ) ) }
	{
		alloc();

		while( begin != end )
		{
			push_back( *begin++ );
		}
	}
	prefix_array ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() }
	{
		alloc();

		for( T const & val : _list_ )
		{
			push_back( val );
		}
	}

	auto & operator+= ( prefix_array< T > const & _rhs_ ) const
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

	auto operator+ ( prefix_array< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		prefix_array< T > res( size_ );

		for( std::size_t i = 0; i < size_; ++i )
		{
			res.push_back( element_at( i ) + _rhs_.element_at( i ) );
		}

		return res;
	}

	auto & operator-= ( prefix_array< T > const & _rhs_ ) const
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

	auto operator- ( prefix_array< T > const & _rhs_ ) const
	{
		if( size_ != _rhs_.size_ )
		{
			throw std::out_of_range( "size != rhs.size" );
		}

		prefix_array< T > res( size_ );

		for( std::size_t i = 0; i < size_; ++i )
		{
			res.push_back( element_at( i ) - _rhs_.element_at( i ) );
		}

		return res;
	}

	bool operator== ( prefix_array< T > const & _rhs_ ) const noexcept
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
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return operator[]( _index_ );
	}

	//  2D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U const & at ( std::size_t _x_, std::size_t _y_ ) const
	{
		return at( _x_ ).at( _y_ );
	}

	//  3D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U const & at ( std::size_t _x_, std::size_t _y_, std::size_t _z_ ) const
	{
		return at( _x_ ).at( _y_ ).at( _z_ );
	}

	T range ( std::size_t _x_, std::size_t _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _x_ == 0 ?
			operator[]( _y_ ) :
			operator[]( _y_ ) - operator[]( _x_ - 1 );
	}

	//  2D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U range ( std::size_t _x1_, std::size_t _y1_, std::size_t _x2_, std::size_t _y2_ ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2_ );
	}

	//  3D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U range ( std::size_t _x1_, std::size_t _y1_, std::size_t _z1_,
			std::size_t _x2_, std::size_t _y2_, std::size_t _z2_ ) const
	{
		return range( _x1_, _x2_ ).range( _y1_, _y2_ ).range( _z1_, _z2_ );
	}

	T element_at ( std::size_t _index_ ) const
	{
		return range( _index_, _index_ );
	}

	//  2D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< U > > > >
	U element_at ( std::size_t _x_, std::size_t _y_ ) const
	{
		return element_at( _x_ ).element_at( _y_ );
	}

	//  3D overload
	template< typename U, typename = std::enable_if_t< std::is_same_v< T, prefix_array< prefix_array< U > > > > >
	U element_at ( std::size_t _x_, std::size_t _y_, std::size_t _z_ ) const
	{
		return element_at( _x_ ).element_at( _y_ ).element_at( _z_ );
	}

	void reserve ( std::size_t _capacity_ ) { resize( _capacity_ ); }

	void push_back ( T _value_ )
	{
		if( size_ >= capacity_ )
		{
			resize();
		}

		if( size_ == 0 )
		{
			data_[ size_ ] = _value_;
		}
		else
		{
			data_[ size_ ] = _value_ + data_[ size_ - 1 ];
		}

		size_++;
	}

	template< typename... Args >
	void push_back ( T _value_, Args&&... _args_ )
	{
		push_back( _value_ );
		push_back( _args_... );
	}

	template< typename... Args >
	void emplace_back ( Args&&... _args_ )
	{
		if( size_ >= capacity_ )
		{
			resize();
		}

		if( size_ == 0 )
		{
			data_[ size_ ] = std::move( T( _args_... ) );
		}
		else
		{
			data_[ size_ ] = T( _args_... ) + data_[ size_ - 1 ];
		}

		size_++;
	}

	~prefix_array () {}

private:
	T           * data_ { nullptr };
	std::size_t   size_       { 0 };
	std::size_t   capacity_   { 0 };

	void alloc  () { data_ = ( T* ) std::malloc( sizeof( T ) * capacity_ ); if( !data_ ) throw std::bad_alloc(); }
	void resize ()
	{
		auto new_cap = ( capacity_ + 1 ) * 1.618;  //  golden ratio magic
		auto tmp = ( T* ) std::realloc( data_, sizeof( T ) * new_cap );

		if( tmp )
		{
			data_     = tmp;
			capacity_ = new_cap;
		}
	}

	void resize ( std::size_t _capacity_ )
	{
		if( _capacity_ > max_size() )
		{
			throw std::out_of_range( "capacity > max_size" );
		}

		auto tmp = ( T* ) std::realloc( data_, sizeof( T ) * _capacity_ );

		if( tmp )
		{
			data_     = tmp;
			capacity_ = _capacity_;
		}
	}

	bool is_index_in_range ( std::size_t _index_ ) const noexcept { return _index_ < size_; }

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
};


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
