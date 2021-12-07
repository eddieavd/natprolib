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
#include <functional>
#include <type_traits>
#include <stdexcept>

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
 *  vector-like container limited to arithmetic types
 *  holds sum of all previous elements at each index
 *  allows for constant-time sum queries on any interval
 */
template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array;

template< typename T, bool C,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array_iterator
{
	friend class prefix_array< T >;
	friend class prefix_array_iterator< T, !C >;

	using pointer   = std::conditional_t< C, T const *, T * >;
	using reference = std::conditional_t< C, T const &, T & >;

	pointer ptr_;

	explicit prefix_array_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};

public:
	reference   operator*  (     ) const { return *ptr_; }
	auto      & operator++ (     )       { ptr_++; return *this; }
	auto        operator++ ( int )       { auto it = *this; ++*this; return it; }

	template< bool R >
	bool operator== ( prefix_array_iterator< T, R > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( prefix_array_iterator< T, R > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	operator prefix_array_iterator< T, true > () const
	{ return prefix_array_iterator< T, true >{ ptr_ }; }
};
// raw pointers ahead, it's 2021, i deserve to be punished
template< typename T, typename U >
class prefix_array
{
	using     value_type = T;
	using       iterator = prefix_array_iterator< T, false >;
	using const_iterator = prefix_array_iterator< T,  true >;

private:
	T           * head_{ nullptr };
	std::size_t   size_      { 0 };
	std::size_t   capacity_  { 0 };

	void alloc  () { head_ = ( T* ) std::malloc( sizeof( T ) * capacity_ ); if( !head_ ) throw std::bad_alloc(); }
	void resize ()
	{
		auto new_cap = ( capacity_ + 1 ) * 1.618;  // golden ratio magic
		auto tmp = ( T* ) std::realloc( head_, sizeof( T ) * new_cap );

		if( tmp )
		{
			head_     = tmp;
			capacity_ = new_cap;
		}
	}
	void resize ( std::size_t const _capacity_ )
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

	bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }

#ifdef _WIN32
	inline std::size_t max_size () const noexcept
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof( status );
		GlobalMemoryStatusEx( &status );

		return status.ullTotalPhys;
	}
#else
	inline std::size_t max_size () const noexcept
	{
		auto pages     = sysconf( _SC_PHYS_PAGES );
		auto page_size = sysconf( _SC_PAGE_SIZE  );

		return pages * page_size;
	}
#endif

public:
	auto begin ()       { return       iterator{ head_         }; }
	auto   end ()       { return       iterator{ head_ + size_ }; }
	auto begin () const { return const_iterator{ head_         }; }
	auto   end () const { return const_iterator{ head_ + size_ }; }

	/**
	 *  @brief returns sum on range ( 0 ... _index_ )
	 *  @param _index_ - index of last element in range
	 */
	inline T const & operator[] ( std::size_t const _index_ ) const noexcept
	{ return head_[ _index_ ]; }

	inline bool operator== ( prefix_array< T, U > const & rhs ) const noexcept
	{ return head_ == rhs.head_; }

	inline bool operator!= ( prefix_array< T, U > const & rhs ) const noexcept
	{ return head_ != rhs.head_; }

	prefix_array ( prefix_array const &  ) = delete;
	prefix_array ( prefix_array       && ) = delete;

	prefix_array (                                           ) : capacity_ { DEFAULT_CAPACITY } { alloc(); };
	prefix_array (              std::size_t const _capacity_ ) : capacity_ {       _capacity_ } { alloc(); };
	prefix_array ( T ** _head_, std::size_t const _capacity_ ) : capacity_ {       _capacity_ }
	{
		alloc();

		for( std::size_t i = 0; i < _capacity_; i++ )
		{
			push_back( ( *_head_ )[ i ] );
		}
		*_head_ = nullptr;
	}
	prefix_array ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() }
	{
		alloc();

		for( auto it = _list_.begin(); it != _list_.end(); it++ )
		{
			push_back( *it );
		}
	}
	template< class Iterator >
	prefix_array ( Iterator const & begin, Iterator const & end ) : capacity_ { DEFAULT_CAPACITY }
	{
		alloc();

		for( auto it = begin; it != end; it++ )
		{
			push_back( *it );
		}
	}

	inline std::size_t     size () const noexcept { return     size_; }
	inline std::size_t capacity () const noexcept { return capacity_; }

	/**
	 *  @brief returns sum on range ( 0 ... _index_ )
	 *  @param _index_ - index of last element in range
	 */
	inline T const & at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}
		else
		{
			return operator[]( _index_ );
		}
	}

	/**
	 *  @brief returns sum of passed range
	 *  @param _x_ - index of first element in range
	 *  @param _y_ - index of last element in range
	 */
	inline T range ( std::size_t const _x_, std::size_t const _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}
		else
		{
			return _x_ == 0 ?
				operator[]( _y_ ) :
				operator[]( _y_ ) - operator[]( _x_ - 1 );
		}
	}

	/**
	 *  @brief returns original value of element
	 *  @param _index_ - index of the element
	 */
	inline T element_at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}
		else
		{
			return _index_ == 0 ?
				operator[]( _index_ ) :
				operator[]( _index_ ) - operator[]( _index_ - 1 );
		}
	}

	void reserve ( std::size_t const _capacity_ ) { resize( _capacity_ ); }

	/**
	 *  @brief inserts element at the end and adds previous sum
	 *  @param _value_ - value to be pushed back
	 */
	void push_back ( T _value_ )
	{
		if( capacity_ <= size_ ) { resize(); }

		if( size_ == 0 )
		{
			head_[ size_ ] = _value_;
		}
		else
		{
			head_[ size_ ] = head_[ size_ - 1 ] + _value_;
		}
		size_++;
	}

	template< typename... Args >
	void emplace_back ( Args&&... args )
	{
		if( capacity_ <= size_ ) { resize(); }

		if( size_ == 0 )
		{
			head_[ size_ ] = T ( args... );
		}
		else
		{
			head_[ size_ ] = T ( args... );
			head_[ size_ ] += head_[ size_ - 1 ];
		}
		size_++;
	}

	void push_array ( T _value_ )
	{
		push_back( _value_ );
	}
	template< typename... Args >
	void push_array( T _value_, Args... _values_ )
	{
		push_back( _value_ );
		push_array( _values_... );
	}

	~prefix_array () { free( head_ ); }
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

	using pointer   = std::conditional_t< C, T const *, T * >;
	using reference = std::conditional_t< C, T const &, T & >;

	pointer ptr_;

	explicit fenwick_tree_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};

public:
	reference   operator*  (     ) const { return *ptr_; }
	auto      & operator++ (     )       { ptr_++; return *this; }
	auto        operator++ ( int )       { auto it = *this; ++*this; return it; }

	template< bool R >
	bool operator== ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
	{ return ptr_ == rhs.ptr_; }

	template< bool R >
	bool operator!= ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
	{ return ptr_ != rhs.ptr_; }

	operator fenwick_tree_iterator< T, true > () const
	{ return fenwick_tree_iterator< T, true >{ ptr_ }; }
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
	void resize ( std::size_t const _capacity_ )
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

		return status.ullTotalPhys;
	}
#else
	std::size_t max_size () const
	{
		long pages     = sysconf( _SC_PHYS_PAGES );
		long page_size = sysconf( _SC_PAGE_SIZE );

		return pages * page_size;
	}
#endif

	inline std::size_t p ( std::size_t const k ) const noexcept { return k & -k; }

	inline bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < capacity_; }

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

	inline T const & operator[] ( std::size_t const _index_ ) const { return head_[ _index_ ]; }

	fenwick_tree (                                           ) : capacity_ { DEFAULT_CAPACITY } { alloc(            ); }
	fenwick_tree (              std::size_t const _capacity_ ) : capacity_ {       _capacity_ } { alloc( _capacity_ ); }
	fenwick_tree ( T ** _head_, std::size_t const _capacity_ ) : capacity_ {       _capacity_ }
	{
		alloc( _capacity_ );

		for( std::size_t i = 0; i < _capacity_; i++ )
		{
			update( ( *_head_ )[ i ], size_++ );
		}
		*_head_ = nullptr;
	}
	fenwick_tree ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() }
	{
		alloc( capacity_ );

		for( auto t : _list_ )
		{
			update( t, size_++ );
		}
	}
	template< class Iterator >
	fenwick_tree ( Iterator begin, Iterator const & end ) : capacity_ { static_cast< std::size_t >( std::distance( begin, end ) ) }
	{
		alloc( capacity_ );

		for( ; begin != end; begin++ )
		{
			update( *begin, size_++ );
		}
	}

	inline std::size_t     size () const noexcept { return     size_; }
	inline std::size_t capacity () const noexcept { return capacity_; }

	T at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _index_ == 0 ?
			sum_to_index( _index_ ) :
			sum_to_index( _index_ ) - sum_to_index( _index_ - 1 );
	}

	T range ( std::size_t const _x_, std::size_t const _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _x_ == 0 ?
			sum_to_index( _y_ ) :
			sum_to_index( _y_ ) - sum_to_index( _x_ - 1 );
	}

	void update ( T const _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		T current = at( _index_ );

		_index_++;

		while( _index_ <= size_ )
		{
			head_[ _index_ - 1 ] += _value_ - current;
			_index_ += p( _index_ );
		}
	}

	void add ( T const _value_, std::size_t _index_ )
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

	void reserve ( std::size_t const _capacity_ ) { resize( _capacity_ ); }

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

	void push_array ( T _value_ )
	{
		push_back( _value_ );
	}
	template< typename... Args >
	void push_array( T _value_, Args... _values_ )
	{
		push_back( _value_ );
		push_array( _values_... );
	}

	~fenwick_tree () { free( head_ ); }
};


template< typename T, typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree;

template< typename T, bool C,
	  typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree_iterator
{
	//  TODO: implement
};

template< typename T, typename U >
class segment_tree
{
	using     value_type = T;
	using       iterator = segment_tree_iterator< T, false >;
	using const_iterator = segment_tree_iterator< T,  true >;

private:
	T           * head_;
	std::size_t   size_;
	std::size_t   og_size_;
	std::size_t   capacity_;

	std::function< T ( T, T ) > parent_builder_;

	void alloc ( std::size_t const _capacity_ )
	{
		og_size_  = _capacity_;
		size_     = 0;
		capacity_ = 2 * round_up_to_pow_2( _capacity_ );
		head_     = ( T* ) std::malloc( sizeof( T ) * capacity_ );
	}

	void alloc_default ( std::size_t const _capacity_ )
	{
		og_size_  = _capacity_;
		size_     = 0;
		capacity_ = 2 * round_up_to_pow_2( _capacity_ );
		head_     = ( T* ) std::malloc( sizeof( T ) * capacity_ );

		for( std::size_t i = 0; i < capacity_; i++ )
		{
			head_[ i ] = T();
		}
	}

	std::size_t round_up_to_pow_2 ( std::size_t const _size_ ) const noexcept
	{
		auto log2 = std::log2( _size_ );

		if( log2 != std::floor( log2 ) )
		{
			auto pow = std::ceil( log2 );
			return std::pow( 2, pow );
		}
		return _size_;
	}

	void construct_tree ()
	{
		for( auto i = ( capacity_ / 2 ) - 1; i > 0; i-- )
		{
			head_[ i ] = parent_builder_( head_[ 2 * i ], head_[ 2 * i + 1 ] );
		}
	}

	bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }

public:
	auto begin ()       { return       iterator{ head_ + size_            }; }
	auto   end ()       { return       iterator{ head_ + size_ + og_size_ }; }
	auto begin () const { return const_iterator{ head_ + size_            }; }
	auto   end () const { return const_iterator{ head_ + size_ + og_size_ }; }

	inline T const & operator[] ( std::size_t const _index_ ) const noexcept { return head_[ ( capacity_ / 2 ) + _index_ ]; }

	inline bool operator== ( segment_tree< T > const & rhs ) const { return head_ == rhs.head_; }
	inline bool operator!= ( segment_tree< T > const & rhs ) const { return head_ != rhs.head_; }

	segment_tree (                                      std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ } { alloc( DEFAULT_CAPACITY ); }
	segment_tree (              std::size_t _capacity_, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ } { alloc(       _capacity_ ); }
	segment_tree ( T ** _head_, std::size_t _capacity_, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( _capacity_ );

		for( std::size_t i = 0; i < _capacity_; i++ )
		{
			head_[ ( capacity_ / 2 ) + i ] = ( *_head_ )[ i ];
			size_++;
		}
		*_head_ = nullptr;
		construct_tree();
	}
	template< class Iterator >
	segment_tree ( Iterator begin, Iterator const & end, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( std::distance( begin, end ) );

		for( ; begin != end; begin++ )
		{
			head_[ ( capacity_ / 2 ) + size_ ] = *begin;
			size_++;
		}
		construct_tree();
	}
	segment_tree ( std::initializer_list< T > const & _list_, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ }
	{
		alloc( _list_.size() );

		for( auto el : _list_ )
		{
			head_[ ( capacity_ / 2 ) + size_ ] = el;
			size_++;
		}
		construct_tree();
	}

	void push_back ( T _value_ )
	{
		if( size_ >= capacity_ / 2 )
		{
			return;
		}
		if( size_ >= og_size_ )
		{
			og_size_ = size_ + 1;
		}

		head_[ capacity_ / 2 + size_ ] = std::move( _value_ );
		size_++;
		construct_tree();
	}

	template< class... Args >
	void emplace_back ( Args&&... args )
	{
		if( size_ >= capacity_ / 2 )
		{
			return;
		}
		if( size_ >= og_size_ )
		{
			og_size_ = size_ + 1;
		}

		head_[ capacity_ / 2 + size_ ] = T ( args... );
		size_++;
		construct_tree();
	}

	inline std::size_t     size () const noexcept { return    size_; }
	inline std::size_t capacity () const noexcept { return og_size_; }

	inline T const & at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}
		else
		{
			return operator[]( _index_ );
		}
	}

	inline T range ( std::size_t _x_, std::size_t _y_ ) const
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

	void update ( std::size_t _index_, T const _value_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		_index_ += capacity_ / 2;

		head_[ _index_ ] = _value_;

		for( _index_ /= 2; _index_ >= 1; _index_ /= 2 )
		{
			head_[ _index_ ] = parent_builder_( head_[ 2 * _index_ ], head_[ 2 * _index_ + 1 ] );
		}
	}

	void set_parent_builder ( std::function< T ( T, T ) > _pb_ ) { parent_builder_ = _pb_; construct_tree(); }

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
