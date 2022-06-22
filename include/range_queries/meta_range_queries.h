//
//  meta_range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include "../util/traits.h"

#define DEFAULT_SIZE 16


namespace npl
{

namespace meta_rq
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

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t< C, T const *, T * >;
	using reference         = std::conditional_t< C, T const &, T & >;
	using iterator_category = std::random_access_iterator_tag;

	constexpr reference   operator*  (     ) const { return *ptr_; }
	constexpr auto      & operator++ (     )       { ptr_++; return *this; }
	constexpr auto      & operator-- (     )       { ptr_--; return *this; }
	constexpr auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
	constexpr auto        operator-- ( int )       { auto it = *this; --*this; return it; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator== ( prefix_array_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R, std::size_t Size_ >
	constexpr bool operator!= ( prefix_array_iterator< T, R, Size_ > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	constexpr operator prefix_array_iterator< T, true, Size > () const
	{ return prefix_array_iterator< T, true, Size >{ ptr_ }; }

private:
	pointer ptr_;

	explicit constexpr prefix_array_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {}
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
	}
	constexpr prefix_array ( T _value_ )
	{
		for( std::size_t i = 0; i < Size; ++i )
		{
			data_[ size_ ] = _value_;

			if( size_ > 0 )
			{
				data_[ size_ ] += data_[ size_ - 1 ];
			}
			size_++;
		}
	}

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

template< typename T >
constexpr auto make_prefix ( T value )
{
	return prefix_array< T >( value );
}

template< typename T, std::size_t Size >
constexpr auto make_prefix ( T value )
{
	return prefix_array< T, Size >( value );
}


template< typename T, std::size_t Size_x = DEFAULT_SIZE, std::size_t Size_y = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_2d;

template< typename T, bool C, std::size_t Size_x = DEFAULT_SIZE, std::size_t Size_y = DEFAULT_SIZE,
	  typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_2d_iterator
{
	friend class prefix_2d< T, Size_x, Size_y >;
	friend class prefix_2d_iterator< T, !C, Size_x, Size_y >;

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t< C, T const *, T * >;
	using reference         = std::conditional_t< C, T const &, T & >;
	using iterator_category = std::random_access_iterator_tag;

	constexpr reference   operator* (     ) const { return *ptr_; }
	constexpr auto      & operator++(     )       { ptr_++; return *this; }
	constexpr auto      & operator--(     )       { ptr_--; return *this; }
	constexpr auto        operator++( int )       { auto it = *this; ++*this; return it; }
	constexpr auto        operator--( int )       { auto it = *this; --*this; return it; }

	template< bool R, std::size_t Size_x_, std::size_t Size_y_ >
	constexpr bool operator== ( prefix_2d_iterator< T, R, Size_x_, Size_y_ > const & rhs ) const
	{ return ptr_ == rhs.ptr_; }

	template< bool R, std::size_t Size_x_, std::size_t Size_y_ >
	constexpr bool operator!= ( prefix_2d_iterator< T, R, Size_x_, Size_y_ > const & rhs ) const
	{ return ptr_ != rhs.ptr_; }

	constexpr operator prefix_2d_iterator< T, true, Size_x, Size_y > () const
	{ return prefix_2d_iterator< T, true, Size_x, Size_y >{ ptr_ }; }

private:
	pointer ptr_;

	explicit constexpr prefix_2d_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
};

template< typename T, std::size_t Size_x, std::size_t Size_y, typename U >
class prefix_2d
{
	using     value_type = T;
	using       iterator = prefix_2d_iterator< T, false, Size_x, Size_y >;
	using const_iterator = prefix_2d_iterator< T,  true, Size_x, Size_y >;

private:
	T           data_[ Size_x ][ Size_y ] {   };
	std::size_t size_x_                   { 0 };
	std::size_t size_y_                   { 0 };
	std::size_t capacity_x_          { Size_x };
	std::size_t capacity_y_          { Size_y };

	constexpr bool is_index_in_range ( std::size_t _x_, std::size_t _y_ ) const noexcept { return _x_ * capacity_y_ + _y_ < size_x_ * capacity_y_ + size_y_; }

public:
	          auto begin ()       { return       iterator{ &data_[          0 ][          0 ] }; }
		  auto   end ()       { return       iterator{ &data_[ Size_x - 1 ][ Size_y - 2 ] }; }
	constexpr auto begin () const { return const_iterator{ &data_[          0 ][          0 ] }; }
	constexpr auto   end () const { return const_iterator{ &data_[ Size_x - 1 ][ Size_y - 2 ] }; }

	constexpr auto     size_x () const noexcept { return     size_x_; }
	constexpr auto     size_y () const noexcept { return     size_y_; }
	constexpr auto capacity_x () const noexcept { return capacity_x_; }
	constexpr auto capacity_y () const noexcept { return capacity_y_; }

	constexpr T const * operator[] ( std::size_t _index_ ) const
	{ return data_[ _index_ ]; }

	constexpr prefix_2d ( std::initializer_list< T > const & _list_ )
	{
		for( auto & t : _list_ )
		{
			if( size_x_ >= Size_x )
			{
				break;
			}

			if( size_x_ != 0 && size_y_ != 0 )
			{
				data_[ size_x_ ][ size_y_ ] = t
					+ data_[ size_x_ - 1 ][ size_y_     ]
					+ data_[ size_x_     ][ size_y_ - 1 ]
					- data_[ size_x_ - 1 ][ size_y_ - 1 ];
			}
			else if( size_x_ != 0 )
			{
				data_[ size_x_ ][ size_y_ ] = t + data_[ size_x_ - 1 ][ size_y_ ];
			}
			else if( size_y_ != 0 )
			{
				data_[ size_x_ ][ size_y_ ] = t + data_[ size_x_ ][ size_y_ - 1 ];
			}
			else
			{
				data_[ size_x_ ][ size_y_ ] = t;
			}

			size_y_++;

			if( size_y_ >= Size_y )
			{
				size_x_++;
				size_y_ = 0;
			}
		}
	}

	constexpr prefix_2d ( T _value_ )
	{
		for( size_x_ = 0; size_x_ < capacity_x_; ++size_x_ )
		{
			for( size_y_ = 0; size_y_ < capacity_y_; ++size_y_ )
			{
				if( size_x_ != 0 && size_y_ != 0 )
				{
					data_[ size_x_ ][ size_y_ ] = _value_
						+ data_[ size_x_ - 1 ][ size_y_     ]
						+ data_[ size_x_     ][ size_y_ - 1 ]
						- data_[ size_x_ - 1 ][ size_y_ - 1 ];
				}
				else if( size_x_ != 0 )
				{
					data_[ size_x_ ][ size_y_ ] = _value_ + data_[ size_x_ - 1 ][ size_y_ ];
				}
				else if( size_y_ != 0 )
				{
					data_[ size_x_ ][ size_y_ ] = _value_ + data_[ size_x_ ][ size_y_ - 1 ];
				}
				else
				{
					data_[ size_x_ ][ size_y_ ] = _value_;
				}
			}
		}

		size_y_ = 0;
	}

	constexpr T const & at ( std::size_t _x_, std::size_t _y_ ) const
	{
		if( !is_index_in_range( _x_, _y_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		return data_[ _x_ ][ _y_ ];
	}

	constexpr T range ( std::size_t _x1_, std::size_t _y1_, std::size_t _x2_, std::size_t _y2_ ) const
	{
		if( !is_index_in_range( _x1_, _y1_ ) || !is_index_in_range( _x2_, _y2_ ) )
		{
			throw std::out_of_range( "index out of bounds" );
		}

		if( _x1_ == 0 && _y1_ == 0 )
		{
			return data_[ _x2_ ][ _y2_ ];
		}
		else if( _x1_ == 0 )
		{
			return data_[ _x2_ ][ _y2_ ] - data_[ _x2_ ][ _y1_ - 1 ];
		}
		else if( _y1_ == 0 )
		{
			return data_[ _x2_ ][ _y2_ ] - data_[ _x1_ - 1 ][ _y2_ ];
		}
		else
		{
			return data_[ _x2_ ][ _y2_ ]
				- data_[ _x2_     ][ _y1_ - 1 ]
				- data_[ _x1_ - 1 ][ _y2_     ]
				+ data_[ _x1_ - 1 ][ _y1_ - 1 ];
		}
	}

	constexpr T range () const
	{
		return range( 0, 0, ( size_x_ - 1 ) % capacity_x_, ( size_y_ - 1 ) % capacity_y_ );
	}

	constexpr T element_at ( std::size_t _x_, std::size_t _y_ ) const
	{
		return range( _x_, _y_, _x_, _y_ );
	}
};

template< typename T, std::size_t Size_x, std::size_t Size_y >
constexpr auto make_prefix_2d ( std::initializer_list< T > const & list )
{
	return prefix_2d< T, Size_x, Size_y >( list );
}

template< typename T >
constexpr auto make_prefix_2d ( T value )
{
	return prefix_2d< T >( value );
}

template< typename T, std::size_t Size_x, std::size_t Size_y >
constexpr auto make_prefix_2d ( T value )
{
	return prefix_2d< T, Size_x, Size_y >( value );
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

public:
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t< C, T const *, T * >;
	using reference         = std::conditional_t< C, T const &, T & >;
	using iterator_category = std::random_access_iterator_tag;

	pointer ptr_;

	explicit constexpr fenwick_tree_iterator ( pointer _ptr_ ) : ptr_{ _ptr_ } {}

public:
	constexpr reference   operator* (     ) const { return *ptr_; }
	constexpr auto      & operator++(     )       { ptr_++; return *this; }
	constexpr auto      & operator--(     )       { ptr_--; return *this; }
	constexpr auto        operator++( int )       { auto it = *this; ++*this; return it; }
	constexpr auto        operator--( int )       { auto it = *this; --*this; return it; }

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

	constexpr fenwick_tree ( T _value_ )
	{
		while( size_ < Size )
		{
			update( _value_, size_++ );
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

template< typename T >
constexpr auto make_ftree ( T value )
{
	return fenwick_tree< T >( value );
}

template< typename T, std::size_t Size >
constexpr auto make_ftree ( T value )
{
	return fenwick_tree< T, Size >( value );
}


} // namespace meta_rq

} // namespace npl
