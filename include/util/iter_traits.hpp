//
//
//      natprolib
//      iter_traits.hpp
//

#pragma once

#include <memory>

#include "base_traits.hpp"


namespace npl
{


//=====================================================================
//      iterator_category
//=====================================================================

template< typename Iterator >
using _iter_value_type = typename std::iterator_traits< Iterator >::value_type ;

template< typename T >
struct has_iterator_category
{
private:
        struct _two { char _lx; char _lxx; };
        template< typename U > static _two _test( ... ) ;
        template< typename U > static char _test( typename U::iterator_category* = 0 ) ;
public:
        static bool const value = sizeof( _test< T >( 0 ) ) == 1 ;
};

template< typename T, typename U, bool = has_iterator_category< std::iterator_traits< T > >::value >
struct has_iterator_category_convertible_to
        : bool_constant< is_convertible_v< typename std::iterator_traits< T >::iterator_category, U > > {} ;

template< typename T, typename U >
struct has_iterator_category_convertible_to< T, U, false > : false_type {} ;

template< typename T >
struct is_cpp17_input_iterator : has_iterator_category_convertible_to< T, std::input_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_cpp17_input_iterator_v = is_cpp17_input_iterator< T >::value ;

template< typename T >
struct is_cpp17_forward_iterator : has_iterator_category_convertible_to< T, std::forward_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_cpp17_forward_iterator_v = is_cpp17_forward_iterator< T >::value ;


} // namespace npl
