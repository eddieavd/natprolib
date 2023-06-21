//
//
//      natprolib
//      algorithm.hpp
//

#pragma once

#include <functional>

#include <util.hpp>
#include <_traits/base_traits.hpp>
#include <_algo/operations.hpp>
#include <_iter/iter_traits.hpp>
#include <_iter/std_iter.hpp>


namespace npl
{


//=====================================================================
//      advance
//=====================================================================

template< typename Iter, typename Dist,
          typename IntegralDist = decltype( convert_to_integral( declval< Dist >() ) ),
          typename = enable_if_t< is_integral_v< IntegralDist > > >
constexpr void advance ( Iter & _iter_, Dist _dist_ )
{
        using diff_t = typename iterator_traits< Iter >::difference_type ;

        [[ maybe_unused ]]
        diff_t dist = static_cast< diff_t >( convert_to_integral( _dist_ ) );
        NPL_CONSTEXPR_ASSERT( dist >= 0 || is_at_least_bidirectional_iterator_v< Iter >,
                        "attempt to advance( it, n ) with negative n on non-bidirectional iterator" );
        _advance( _iter_, _dist_, typename iterator_traits< Iter >::iterator_category() );
}

//=====================================================================
//      distance
//=====================================================================

template< typename Iter >
inline constexpr
typename iterator_traits< Iter >::difference_type
distance ( Iter _first_, Iter _last_ )
{
        return _distance( _first_, _last_, typename iterator_traits< Iter >::iterator_category() );
}

//=====================================================================
//      invoke
//=====================================================================



//=====================================================================
//      swap
//=====================================================================

template< typename T >
using _swap_result_t = enable_if_t< is_move_constructible_v< T > && is_move_assignable_v< T > > ;

template< typename T >
inline constexpr
_swap_result_t< T > swap ( T & _lhs_, T & _rhs_ ) noexcept( is_nothrow_move_constructible_v< T > && is_nothrow_move_assignable_v< T > )
{
        T tmp(  NPL_MOVE( _lhs_ ) );
        _lhs_ = NPL_MOVE( _rhs_ );
        _rhs_ = NPL_MOVE(  tmp  );
}

template< typename T >
inline constexpr
_swap_result_t< T > _swap_integral ( T & _lhs_, T & _rhs_ ) noexcept
{
        _lhs_ += _rhs_;
        _rhs_  = _lhs_ - _rhs_;
        _lhs_ -= _rhs_;
}

/*
template< typename T >
        requires is_integral_v< T >
inline constexpr
_swap_result_t< T > swap ( T & _lhs_, T & _rhs_ ) noexcept
{
        _lhs_ += _rhs_;
        _rhs_  = _lhs_ - _rhs_;
        _lhs_ -= _rhs_;
}
*/

/*
template< typename T, size_t N >
inline constexpr
enable_if_t< is_swappable_v< T > >
swap ( T ( &_lhs_ )[ N ], T ( &_rhs_ )[ N ] ) noexcept( is_nothrow_swappable_v< T > )           // TODO needs swappable traits
{
        for( size_t i = 0; i != N; ++i )
        {
                swap( _lhs_[ i ], _rhs_[ i ] );
        }
}
*/


//=====================================================================
//      min
//=====================================================================

template< typename Comp >
struct _comp_ref_type
{
        using type = add_lvalue_reference_t< Comp > ;
};

template< typename Comp, typename Iter, typename Sent, typename Proj >
inline constexpr
Iter _min_element ( Iter _first_, Sent _last_, Comp _comp_, Proj & _proj_ )
{
        if( _first_ == _last_ )
        {
                return _first_;
        }
        Iter iter = _first_;
        while( ++iter != _last_ )
        {
                if( std::invoke( _comp_, std::invoke( _proj_, *iter ), std::invoke( _proj_, *_first_ ) ) )
                {
                        _first_ = iter;
                }
        }
        return _first_;
}

template< typename Comp, typename Iter, typename Sent >
inline constexpr
Iter _min_element ( Iter _first_, Sent _last_, Comp _comp_ )
{
        auto proj = std::identity();
        return _min_element< Comp >( NPL_MOVE( _first_ ), NPL_MOVE( _last_ ), _comp_, proj );
}

template< typename ForwardIter, typename Comp >
NPL_NODISCARD inline constexpr
ForwardIter min_element ( ForwardIter _first_, ForwardIter _last_, Comp _comp_ )
{
        static_assert( is_at_least_forward_iterator_v< ForwardIter >, "npl::min_element: requires a forward iterator" );
        static_assert( is_callable_v< Comp, decltype( *_first_ ), decltype( *_first_ ) >, "npl::min_element: comparator must be callable" );

        using comp_ref_t = typename _comp_ref_type< Comp >::type ;
        return _min_element< comp_ref_t >( NPL_MOVE( _first_ ), NPL_MOVE( _last_ ), _comp_ );
}

template< typename ForwardIter >
NPL_NODISCARD inline constexpr
ForwardIter min_element ( ForwardIter _first_, ForwardIter _last_ )
{
        return min_element( _first_, _last_, less< typename iterator_traits< ForwardIter >::value_type >() );
}

template< typename T, typename Comp >
NPL_NODISCARD inline constexpr
T const & min ( T const & _lhs_, T const & _rhs_, Comp _comp_ )
{
        return _comp_( _rhs_, _lhs_ ) ? _rhs_ : _lhs_ ;
}

template< typename T >
NPL_NODISCARD inline constexpr
T const & min ( T const & _lhs_, T const & _rhs_ )
{
        return min( _lhs_, _rhs_, less< T >() );
}

#ifdef NPL_HAS_STL

template< typename T, typename Comp >
NPL_NODISCARD inline constexpr
T min ( std::initializer_list< T > _list_, Comp _comp_ )
{
        using comp_ref_t = typename _comp_ref_type< Comp >::type ;
        return *_min_element< comp_ref_t >( _list_.begin(), _list_.end(), _comp_ );
}

template< typename T >
NPL_NODISCARD inline constexpr
T min ( std::initializer_list< T > _list_ )
{
        return *min_element( _list_.begin(), _list_.end(), less< T >() );
}

#endif

//=====================================================================
//      max
//=====================================================================

template< typename Comp, typename ForwardIter >
inline constexpr
ForwardIter _max_element ( ForwardIter _first_, ForwardIter _last_, Comp _comp_ )
{
        static_assert( is_at_least_forward_iterator_v< ForwardIter >,
                        "npl::_max_element: requires at least forward iterator" );
        if( _first_ != _last_ )
        {
                ForwardIter iter = _first_;
                while( ++iter != _last_ )
                {
                        if( _comp_( *_first_, *iter ) )
                        {
                                _first_ = iter;
                        }
                }
        }
        return _first_;
}

template< typename ForwardIter, typename Comp >
NPL_NODISCARD inline constexpr
ForwardIter max_element ( ForwardIter _first_, ForwardIter _last_, Comp _comp_ )
{
        return npl::_max_element< _comp_ref_type< Comp > >( _first_, _last_, _comp_ );
}

template< typename ForwardIter >
NPL_NODISCARD inline constexpr
ForwardIter max_element ( ForwardIter _first_, ForwardIter _last_ )
{
        return npl::max_element( _first_, _last_, less< typename iterator_traits< ForwardIter >::value_type >() );
}

template< typename T, typename Comp >
NPL_NODISCARD inline constexpr
T const & max ( T const & _lhs_, T const & _rhs_, Comp _comp_ )
{
        return _comp_( _lhs_, _rhs_ ) ? _rhs_ : _lhs_ ;
}

template< typename T >
NPL_NODISCARD inline constexpr
T const & max ( T const & _lhs_, T const & _rhs_ )
{
        return npl::max( _lhs_, _rhs_, less< T >() );
}

#ifdef NPL_HAS_STL

template< typename T, typename Comp >
NPL_NODISCARD inline constexpr
T max ( std::initializer_list< T > _list_, Comp _comp_ )
{
        using comp_ref_t = typename _comp_ref_type< Comp >::type ;
        return *npl::_max_element< comp_ref_t >( _list_.begin(), _list_.end(), _comp_ );
}

template< typename T >
NPL_NODISCARD inline constexpr
T max ( std::initializer_list< T > _list_ )
{
        return *npl::max_element( _list_.begin(), _list_.end(), less< T >() );
}

#endif

//=====================================================================
//      floor
//=====================================================================

#ifndef __sun__

NPL_NODISCARD inline       float floor (       float _val_ ) noexcept { return __builtin_floorf( _val_ ); }

template< typename = int >
NPL_NODISCARD             double floor (      double _val_ ) noexcept { return __builtin_floor ( _val_ ); }

NPL_NODISCARD inline long double floor ( long double _val_ ) noexcept { return __builtin_floorl( _val_ ); }

#endif

template< typename T >
NPL_NODISCARD inline
enable_if_t
<
        is_integral_v< T >,
        double
>
floor ( T _val_ ) noexcept { return __builtin_floor( ( double ) _val_ ); }

//=====================================================================
//      ceil
//=====================================================================

#ifndef __sun__

NPL_NODISCARD inline       float ceil (       float _val_ ) noexcept { return __builtin_ceilf( _val_ ); }

template< typename = int >
NPL_NODISCARD             double ceil (      double _val_ ) noexcept { return __builtin_ceil ( _val_ ); }

NPL_NODISCARD inline long double ceil ( long double _val_ ) noexcept { return __builtin_ceill( _val_ ); }

#endif

template< typename T >
NPL_NODISCARD inline
enable_if_t
<
        is_integral_v< T >,
        double
>
ceil ( T _val_ ) noexcept { return __builtin_ceil( ( double ) _val_ ); }

//=====================================================================
//      copy ( in mem )
//=====================================================================

//=====================================================================
//      fill_n
//=====================================================================



} // namespace npl
