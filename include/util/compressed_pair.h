//
//	compressed_pair.h
//	natprolib
//
//	Created by Edhem Avdagic on 16/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include "tuple.h"
#include "traits.h"


namespace npl
{


struct _default_init_tag {};
struct _value_init_tag   {};

template< typename T, int Idx,
	bool CanBeEmptyBase =
		std::is_empty_v< T > && !npl_is_final_v< T > >
struct _compressed_pair_elem
{
	using          ParamT = T ;
	using       reference = T&;
	using const_reference = T const &;

	constexpr _compressed_pair_elem ( _default_init_tag ) {}
	constexpr _compressed_pair_elem (   _value_init_tag ) {}

	template< typename U, typename = typename std::enable_if_t<
		!std::is_same_v< _compressed_pair_elem, typename std::decay_t< U > > > >
	constexpr explicit
	_compressed_pair_elem ( U && _u_ )
		: value_ ( std::forward< U >( _u_ ) )
	{
	}

	template< typename... Args, std::size_t... Indices >
	constexpr
	_compressed_pair_elem ( std::piecewise_construct_t, std::tuple< Args... > _args_,
				_tuple_indices< Indices... > )
		: value_ ( std::forward< Args >( std::get< Indices >( _args_ ) )... ) {}

	      reference _get ()       noexcept { return value_; }
	const_reference _get () const noexcept { return value_; }

private:
	T value_;
};

template< typename T, int Idx >
struct _compressed_pair_elem< T, Idx, true > : private T
{
	using          ParamT = T;
	using      value_type = T;
	using       reference = T&;
	using const_reference = T const &;

	constexpr _compressed_pair_elem (                   ) = default;
	constexpr _compressed_pair_elem ( _default_init_tag ) {}
	constexpr _compressed_pair_elem (   _value_init_tag ) : value_type() {}

	template< typename U, typename = typename std::enable_if_t<
		!std::is_same_v< _compressed_pair_elem, typename std::decay_t< U > > > >
	constexpr explicit
	_compressed_pair_elem ( U && _u_ )
		: value_type( std::forward< U >( _u_ ) )
	{
	}

	template< typename... Args, std::size_t... Indices >
	constexpr
	_compressed_pair_elem ( std::piecewise_construct_t, std::tuple< Args... > _args_,
				_tuple_indices< Indices... > )
		: value_type( std::forward< Args >( std::get< Indices >( _args_ ) )... ) {}

	      reference _get ()       noexcept { return *this; }
	const_reference _get () const noexcept { return *this; }
};

template< typename T1, typename T2 >
class compressed_pair : private _compressed_pair_elem< T1, 0 >,
			private _compressed_pair_elem< T2, 1 >
{
public:
	using _base1 = _compressed_pair_elem< T1, 0 >;
	using _base2 = _compressed_pair_elem< T2, 1 >;

	template< bool Dummy = true,
		  typename = typename std::enable_if_t<
			  _dependent_type< std::is_default_constructible< T1 >, Dummy >::value &&
			  _dependent_type< std::is_default_constructible< T2 >, Dummy >::value
		>
	>
	constexpr compressed_pair () : _base1( _value_init_tag() ), _base2( _value_init_tag() ) {}

	template< typename U1, typename U2 >
	constexpr
	compressed_pair ( U1 && _u1_, U2 && _u2_ )
		: _base1( std::forward< U1 >( _u1_ ) ), _base2( std::forward< U2 >( _u2_ ) ) {}

	template< typename... Args1, typename... Args2 >
	constexpr
	compressed_pair ( std::piecewise_construct_t _pc_, std::tuple< Args1... > _first_args_, std::tuple< Args2... > _second_args_ )
		: _base1( _pc_, std::move(  _first_args_ ), typename _make_tuple_indices< sizeof...( Args1 ) >::type() ),
		  _base2( _pc_, std::move( _second_args_ ), typename _make_tuple_indices< sizeof...( Args2 ) >::type() ) {}

	typename _base1::reference first () noexcept
	{ return static_cast< _base1& >( *this )._get(); }

	typename _base1::const_reference first () const noexcept
	{ return static_cast< _base1 const & >( *this )._get(); }

	typename _base2::reference second () noexcept
	{ return static_cast< _base2& >( *this )._get(); }

	typename _base2::const_reference second () const noexcept
	{ return static_cast< _base2 const & >( *this )._get(); }

	constexpr static
	_base1 * _get_first_base ( compressed_pair * _pair_ ) noexcept
	{ return static_cast< _base1* >( _pair_ ); }

	constexpr static
	_base2 * _get_second_base ( compressed_pair * _pair_ ) noexcept
	{ return static_Cast< _base2* >( _pair_ ); }

	void swap ( compressed_pair & _x_ )
		noexcept ( std::__is_nothrow_swappable< T1 >::value &&
			   std::__is_nothrow_swappable< T2 >::value )
	{
		using std::swap;

		swap(  first(),  _x_.first() );
		swap( second(), _x_.second() );
	}
};

template< typename T1, typename T2 >
inline void swap ( compressed_pair< T1, T2 > & _x_, compressed_pair< T1, T2 > & _y_ )
	noexcept( std::__is_nothrow_swappable< T1 >::value &&
		  std::__is_nothrow_swappable< T2 >::value )
{
	_x_.swap( _y_ );
}


} // namespace npl
