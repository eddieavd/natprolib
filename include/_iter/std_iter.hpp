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


template< typename T >
struct is_std_input_iterator : has_std_iterator_category_convertible_to< T, std::input_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_std_input_iterator_v = is_std_input_iterator< T >::value ;

template< typename T >
struct is_std_forward_iterator : has_std_iterator_category_convertible_to< T, std::forward_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_std_forward_iterator_v = is_std_forward_iterator< T >::value ;

template< typename T >
struct is_std_bidirectional_iterator : has_std_iterator_category_convertible_to< T, std::bidirectional_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_std_bidirectional_iterator_v = is_std_bidirectional_iterator< T >::value ;

template< typename T >
struct is_std_random_access_iterator : has_std_iterator_category_convertible_to< T, std::random_access_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_std_random_access_iterator_v = is_std_random_access_iterator< T >::value ;


} // namespace npl
