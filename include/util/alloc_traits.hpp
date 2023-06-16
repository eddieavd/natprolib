//
//
//      natprolib
//      alloc_traits.hpp
//

#pragma once

#include <memory>

#include "base_traits.hpp"


namespace npl
{


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


} // namespace npl
