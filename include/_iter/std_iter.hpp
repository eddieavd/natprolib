//
//
//      natprolib
//      std_iter.hpp
//

#pragma once

#include <memory>

#include <_iter/iter_traits.hpp>


namespace npl
{


template< typename T, typename U, bool = has_iterator_category< std::iterator_traits< T > >::value >
struct has_std_iterator_category_convertible_to
        : bool_constant< is_convertible_v< typename std::iterator_traits< T >::iterator_category, U > > {} ;

template< typename T, typename U >
struct has_std_iterator_category_convertible_to< T, U, false > : false_type {} ;


template< typename T >
struct is_at_least_std_input_iterator : has_std_iterator_category_convertible_to< T, std::input_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_std_input_iterator_v = is_at_least_std_input_iterator< T >::value ;

template< typename T >
struct is_at_least_std_forward_iterator : has_std_iterator_category_convertible_to< T, std::forward_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_std_forward_iterator_v = is_at_least_std_forward_iterator< T >::value ;

template< typename T >
struct is_at_least_std_bidirectional_iterator : has_std_iterator_category_convertible_to< T, std::bidirectional_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_std_bidirectional_iterator_v = is_at_least_std_bidirectional_iterator< T >::value ;

template< typename T >
struct is_at_least_std_random_access_iterator : has_std_iterator_category_convertible_to< T, std::random_access_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_std_random_access_iterator_v = is_at_least_std_random_access_iterator< T >::value ;


template< typename InputIter >
inline constexpr
typename iterator_traits< InputIter >::difference_type
_distance ( InputIter _first_, InputIter _last_, std::input_iterator_tag )
{
        return _distance( _first_, _last_, input_iterator_tag{} );
}

template< typename RandIter >
inline constexpr
typename iterator_traits< RandIter >::difference_type
_distance ( RandIter _first_, RandIter _last_, std::random_access_iterator_tag )
{
        return _distance( _first_, _last_, random_access_iterator_tag{} );
}


template< typename InputIter >
constexpr void _advance ( InputIter & _iter_, typename iterator_traits< InputIter >::difference_type _dist_, std::input_iterator_tag )
{
        _advance( _iter_, _dist_, input_iterator_tag{} );
}

template< typename BiDirIter >
constexpr void _advance ( BiDirIter & _iter_, typename iterator_traits< BiDirIter >::difference_type _dist_, std::bidirectional_iterator_tag )
{
        _advance( _iter_, _dist_, bidirectional_iterator_tag{} );
}

template< typename RandIter >
constexpr void _advance ( RandIter & _iter_, typename iterator_traits< RandIter >::difference_type _dist_, std::random_access_iterator_tag )
{
        _advance( _iter_, _dist_, random_access_iterator_tag{} );
}


} // namespace npl
