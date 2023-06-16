//
//
//      natprolib
//      traits.hpp
//

#pragma once

#include "util.hpp"
#include "base_traits.hpp"

#define NPL_MOVE_IF_NOEXCEPT(...) \
        _move_if_noexcept(__VA_ARGS__)


namespace npl
{


template< typename T >
constexpr conditional_t
<
        !is_nothrow_move_constructible_v< T > &&
         is_copy_constructible_v< T >,
         T const &,
         T &&
>
_move_if_noexcept ( T & val ) noexcept
{
        return NPL_MOVE( val );
}


//=====================================================================
//      is_container
//=====================================================================

template< typename T, typename = void, typename = void >
struct _is_container_impl : false_type {} ;

template< typename T >
struct _is_container_impl<
                                T,
                                typename void_t< typename T::value_type >::type,
                                typename void_t< decltype( declval< T & >().at( size_t( 0 ) ) ) >::type
                         >
                         : true_type {} ;

template< typename T >
struct is_container : _is_container_impl< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_container_v = is_container< T >::value ;

//=====================================================================
//      has_range
//=====================================================================

template< typename T, typename = void, typename = void >
struct _has_range_impl : false_type {} ;

template< typename T >
struct _has_range_impl<
                        T,
                        typename void_t< typename T::value_type >::type,
                        typename void_t< decltype( declval< T & >().range( size_t(), size_t() ) ) >::type
                      >
                      : true_type {} ;

template< typename T >
struct has_range : _has_range_impl< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool has_range_v = has_range< T >::value ;

//=====================================================================
//      is_range_container
//=====================================================================

template< typename T >
struct is_range_container : conjunction< is_container< T >, has_range< T > > {} ;

template< typename T >
inline constexpr bool is_range_container_v = is_range_container< T >::value ;

//=====================================================================
//      2d containers
//=====================================================================

template< typename Container >
struct is_2d_container : conjunction<
                                        is_container< Container >,
                                        is_container< typename Container::value_type >
                                    > {} ;
template< typename Container >
inline constexpr bool is_2d_container_v = is_2d_container< Container >::value ;

template< typename Container >
struct is_2d_range_container : conjunction<
                                                is_range_container< Container >,
                                                is_range_container< typename Container::value_type >
                                          > {} ;
template< typename Container >
inline constexpr bool is_2d_range_container_v = is_2d_range_container< Container >::value ;

//=====================================================================
//      3d containers
//=====================================================================

template< typename Container >
struct is_3d_container : conjunction<
                                        is_2d_container< Container >,
                                        is_2d_container< typename Container::value_type >
                                    > {} ;
template< typename Container >
inline constexpr bool is_3d_container_v = is_3d_container< Container >::value ;

template< typename Container >
struct is_3d_range_container : conjunction<
                                                is_2d_range_container< Container >,
                                                is_2d_range_container< typename Container::value_type >
                                          > {} ;
template< typename Container >
inline constexpr bool is_3d_range_container_v = is_3d_range_container< Container >::value ;

//=====================================================================
//      multidim container base type enablers
//=====================================================================

template< typename Container >
using _2d_container_base_t = typename Container::value_type::value_type ;

template< typename Container >
using _3d_container_base_t = typename _2d_container_base_t< Container >::value_type ;

template< typename Container >
struct enable_if_2d_container : enable_if< is_2d_container_v< Container > > {} ;

template< typename Container >
using enable_if_2d_container_t = typename enable_if_2d_container< Container >::type ;

template< typename Container >
struct enable_if_3d_container : enable_if< is_3d_container_v< Container > > {} ;

template< typename Container >
using enable_if_3d_container_t = typename enable_if_3d_container< Container >::type ;

template< typename Container >
struct enable_if_2d_range_container : enable_if< is_2d_range_container_v< Container > > {} ;

template< typename Container >
using enable_if_2d_range_container_t = typename enable_if_2d_range_container< Container >::type ;

template< typename Container >
struct enable_if_3d_range_container : enable_if< is_3d_range_container_v< Container > > {} ;

template< typename Container >
using enable_if_3d_range_container_t = typename enable_if_3d_range_container< Container >::type ;

template< typename Container >
struct enable_2d_container_base : enable_if< is_2d_container_v< Container >, _2d_container_base_t< Container > > {} ;

template< typename Container >
using enable_2d_container_base_t = typename enable_2d_container_base< Container >::type ;

template< typename Container >
struct enable_3d_container_base : enable_if< is_3d_container_v< Container >, _3d_container_base_t< Container > > {} ;

template< typename Container >
using enable_3d_container_base_t = typename enable_3d_container_base< Container >::type ;

template< typename Container >
struct enable_2d_range_container_base : enable_if< is_2d_range_container_v< Container >, _2d_container_base_t< Container > > {} ;

template< typename Container >
using enable_2d_range_container_base_t = typename enable_2d_range_container_base< Container >::type ;

template< typename Container >
struct enable_3d_range_container_base : enable_if< is_3d_range_container_v< Container >, _3d_container_base_t< Container > > {} ;

template< typename Container >
using enable_3d_range_container_base_t = typename enable_3d_range_container_base< Container >::type ;


} // namespace npl
