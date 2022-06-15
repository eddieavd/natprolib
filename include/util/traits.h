//
//	traits.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <memory>


namespace npl
{


template< typename T >
struct is_default_allocator : std::false_type {};
template< typename T >
struct is_default_allocator< std::allocator< T > > : std::true_type {};

template< typename Alloc, typename... Args,
	  typename = decltype( std::declval< Alloc >().construct( std::declval< Args >()... )) >
static std::true_type test_has_construct( int );
template< typename Alloc, typename... >
static std::false_type test_has_construct( ... );
template< typename Alloc, typename... Args >
struct has_construct : decltype( test_has_construct< Alloc, Args... >( 0 ) ) {};

template< typename Alloc,
	bool = has_construct< Alloc, typename Alloc::value_type*,  typename Alloc::value_type&& >::value && !is_default_allocator< Alloc >::value >
struct is_cpp17_move_insertable;
template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, true > : std::true_type {};
template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, false > : std::is_move_constructible< typename Alloc::value_type > {};


} // namespace npl
