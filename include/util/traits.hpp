//
//
//      natprolib
//      traits.hpp
//

#pragma once

#include <memory>
// used for std::allocator, std::iterator_traits, std::allocator_traits

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
//      is_allocator
//=====================================================================

template< typename Alloc, typename = void, typename = void >
struct is_allocator : false_type {} ;

template< typename Alloc >
struct is_allocator
<
        Alloc,
        typename void_t< typename Alloc::value_type >::type,
        typename void_t< decltype( declval< Alloc & >().allocate( size_t( 0 ) ) ) >::type
>
        : true_type {} ;


//=====================================================================
//      is_default_allocator
//=====================================================================

template< typename T >
struct is_default_allocator : false_type {} ;

template< typename T >
struct is_default_allocator< std::allocator< T > > : true_type {} ;

template< typename T >
inline constexpr bool is_default_allocator_v = is_default_allocator< T >::value ;

//=====================================================================
//      has_construct
//=====================================================================

template< typename Alloc, typename... Args,
          typename = decltype( declval< Alloc >().construct( declval< Args >()... ) ) >
static true_type test_has_construct( int ) ;
template< typename Alloc, typename... >
static false_type test_has_construct( ... ) ;

template< typename Alloc, typename... Args >
struct has_construct : decltype( test_has_construct< Alloc, Args... >( 0 ) ) {} ;

template< typename Alloc, typename... Args >
inline constexpr bool has_construct_v = has_construct< Alloc, Args... >::value ;

//=====================================================================
//      is_cpp17_move_insertable
//=====================================================================

template< typename Alloc,
          bool = has_construct_v
                 <
                        Alloc,
                        typename Alloc::value_type *,
                        typename Alloc::value_type &&
                 >
                 &&
                 !is_default_allocator_v< Alloc >
>
struct is_cpp17_move_insertable ;

template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, true > : true_type {} ;
template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, false > : is_move_constructible< typename Alloc::value_type > {} ;

template< typename Alloc >
inline constexpr bool is_cpp17_move_insertable_v = is_cpp17_move_insertable< Alloc >::value ;

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

//=====================================================================
//      noexcept_move_assign_container
//=====================================================================

template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
struct noexcept_move_assign_container : bool_constant
                                        <
                                                Traits::propagate_on_container_move_assignment::value ||
                                                Traits::is_always_equal::value
                                        > {} ;
template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
inline constexpr bool noexcept_move_assign_container_v = noexcept_move_assign_container< Alloc, Traits >::value ;

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
