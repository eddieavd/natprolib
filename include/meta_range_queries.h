//
//  meta_range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <initializer_list>
#include <type_traits>
#include <stdexcept>

#define DEFAULT_SIZE 16


namespace natprolib
{

namespace meta_range_queries
{


/**
 *  fixed-size, constexpr prefix array
 *  container limited to arithmetic types
 *  holds sum of all previous elements at each index
 *  allows for constant-time sum queries on any interval
 */
template< typename T, std::size_t Size = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array;

template< typename T, bool C, std::size_t Size = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array_iterator
{
	friend class prefix_array< T, Size >;
	friend class prefix_array_iterator< T, !C, Size >;

	using pointer   = std::conditional_t< C, T const *, T * >;
	using reference = std::conditional_t< C, T const &, T & >;

	pointer ptr_;

	explicit constexpr prefix_array_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};

public:
	constexpr reference   operator*  (     ) const { return *ptr_; }
	constexpr auto      & operator++ (     )       { ptr_++; return *this; }
	constexpr auto        operator++ ( int )       { auto it = *this; ++*this; return it; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator== ( prefix_array_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator!= ( prefix_array_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	constexpr operator prefix_array_iterator< T, true, Size > () const
	{ return prefix_array_iterator< T, true, Size >{ ptr_ }; }
};

template< typename T, std::size_t Size, typename U >
class prefix_array
{
	using     value_type = T;
	using       iterator = prefix_array_iterator< T, false, Size >;
	using const_iterator = prefix_array_iterator< T,  true, Size >;

private:
	T           data_[ Size ]    { 0 };
	std::size_t size_            { 0 };
	std::size_t capacity_     { Size };

	constexpr bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }

public:
	          auto begin ()       { return       iterator{ data_         }; }
	          auto   end ()       { return       iterator{ data_ + size_ }; }
	constexpr auto begin () const { return const_iterator{ data_         }; }
	constexpr auto   end () const { return const_iterator{ data_ + size_ }; }

	constexpr auto     size () const noexcept { return     size_; }
	constexpr auto capacity () const noexcept { return capacity_; }

	constexpr T const & operator[] ( std::size_t const _index_ ) const
	{ return data_[ _index_ ]; }

	constexpr prefix_array ( std::initializer_list< T > const & _list_ )
	{
		for( auto & t : _list_ )
		{
			if( size_ < Size )
			{
				data_[ size_ ] = std::move( t );
				if( size_ > 0 )
				{
					data_[ size_ ] += data_[ size_ - 1 ];
				}
				size_++;
			}
			else
			{
				break;
			}
		}
	};

	constexpr T const & at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}
		else
		{
			return data_[ _index_ ];
		}
	}

	constexpr T range ( std::size_t const _x_, std::size_t const _y_ ) const
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

	constexpr T element_at ( std::size_t const _index_ ) const { return range( _index_, _index_ ); }
};

template< typename T >
constexpr auto make_prefix ( std::initializer_list< T > const & list )
{
	return prefix_array< T >( list );
}

template< typename T, std::size_t Size >
constexpr auto make_prefix ( std::initializer_list< T > const & list )
{
	return prefix_array< T, Size >( list );
}


template< typename T, std::size_t Size = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree;

template< typename T, bool C, std::size_t Size = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree_iterator
{
	friend class fenwick_tree< T, Size >;
	friend class fenwick_tree_iterator< T, !C, Size >;

	using pointer   = std::conditional_t< C, T const *, T * >;
	using reference = std::conditional_t< C, T const &, T & >;

	pointer ptr_;

	explicit constexpr fenwick_tree_iterator ( pointer _ptr_ ) : ptr_{ _ptr_ } {}

public:
	constexpr reference   operator* (     ) const { return *ptr_; }
	constexpr auto      & operator++(     )       { ptr_++; return *this; }
	constexpr auto        operator++( int )       { auto it = *this; ++*this; return it; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator== ( fenwick_tree_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator!= ( fenwick_tree_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	constexpr operator fenwick_tree_iterator< T, true, Size > () const
	{ return fenwick_tree_iterator< T, true, Size >{ ptr_ }; }
};

template< typename T, std::size_t Size, typename U >
class fenwick_tree
{
	using     value_type = T;
	using       iterator = fenwick_tree_iterator< T, false, Size >;
	using const_iterator = fenwick_tree_iterator< T,  true, Size >;

private:
	T           data_[ Size ] { 0 };
	std::size_t size_         { 0 };
	std::size_t capacity_  { Size };

	constexpr std::size_t p ( std::size_t const k ) const noexcept { return k & -k; }

	constexpr bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }

	constexpr T sum_to_index ( std::size_t _index_ ) const
	{
		T s = 0;
		_index_++;

		while( _index_ >= 1 )
		{
			s += data_[ _index_ - 1 ];
			_index_ -= p( _index_ );
		}

		return s;
	}

	constexpr void update ( T const _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index of out bounds" );
		}

		T current = at( _index_++ );

		while( _index_ <= size_ )
		{
			data_[ _index_ - 1 ] += _value_ - current;
			_index_ += p( _index_ );
		}
	}

	constexpr void add ( T const _value_, std::size_t _index_ )
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		_index_++;

		while( _index_ <= size_ )
		{
			data_[ _index_ - 1 ] += _value_;
			_index_ += p( _index_ );
		}
	}

public:
	          auto begin ()       { return       iterator{ data_         }; }
	          auto   end ()       { return       iterator{ data_ + size_ }; }
	constexpr auto begin () const { return const_iterator{ data_         }; }
	constexpr auto   end () const { return const_iterator{ data_ + size_ }; }

	constexpr std::size_t     size () const noexcept { return     size_; }
	constexpr std::size_t capacity () const noexcept { return capacity_; }

	constexpr T const & operator[] ( std::size_t const _index_ ) const
	{ return data_[ _index_ ]; }

	constexpr fenwick_tree ( std::initializer_list< T > const & _list_ )
	{
		for( auto & t : _list_ )
		{
			if( size_ < Size )
			{
				update( t, size_++ );
			}
			else
			{
				break;
			}
		}
	}

	constexpr T at ( std::size_t const _index_ ) const
	{
		if( !is_index_in_range( _index_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _index_ == 0 ?
			sum_to_index( _index_ ) :
			sum_to_index( _index_ ) - sum_to_index( _index_ - 1 );
	}

	constexpr T range ( std::size_t const _x_, std::size_t const _y_ ) const
	{
		if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return _x_ == 0 ?
			sum_to_index( _y_ ) :
			sum_to_index( _y_ ) - sum_to_index( _x_ - 1 );
	}
};

template< typename T >
constexpr auto make_ftree ( std::initializer_list< T > const & list )
{
	return fenwick_tree< T >( list );
}

template< typename T, std::size_t Size >
constexpr auto make_ftree ( std::initializer_list< T > const & list )
{
	return fenwick_tree< T, Size >( list );
}


} // namespace meta_range_queries

} // namespace natprolib
