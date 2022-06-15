//
//	traits.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <memory>
#include <vector>


namespace npl
{


template< typename T >
struct _identity { using type = T; };

template< typename T >
struct is_default_allocator : std::false_type {};
template< typename T >
struct is_default_allocator< std::allocator< T > > : std::true_type {};
template< typename T >
inline constexpr bool is_default_allocator_v = is_default_allocator< T >::value;

template< typename Alloc, typename... Args,
	  typename = decltype( std::declval< Alloc >().construct( std::declval< Args >()... )) >
static std::true_type test_has_construct( int );
template< typename Alloc, typename... >
static std::false_type test_has_construct( ... );

template< typename Alloc, typename... Args >
struct has_construct : decltype( test_has_construct< Alloc, Args... >( 0 ) ) {};

template< typename Alloc, typename... Args >
inline constexpr bool has_construct_v = has_construct< Alloc, Args... >::value;

template< typename Alloc,
	bool = has_construct< Alloc, typename Alloc::value_type*, typename Alloc::value_type&& >::value && !is_default_allocator< Alloc >::value >
struct is_cpp17_move_insertable;

template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, true > : std::true_type {};
template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, false > : std::is_move_constructible< typename Alloc::value_type > {};

template< typename Alloc >
inline constexpr bool is_cpp17_move_insertable_v = is_cpp17_move_insertable< Alloc >::value;

template< typename T >
struct has_iterator_category
{
private:
	struct _two {char _lx; char _lxx;};
	template< typename U > static _two _test( ... );
	template< typename U > static char _test( typename U::iterator_category* = 0 );
public:
	static bool const value = sizeof( _test< T >( 0 ) ) == 1;
};

template< typename T, typename U, bool = has_iterator_category< std::iterator_traits< T > >::value >
struct has_iterator_category_convertible_to
	: public std::integral_constant< bool, std::is_convertible_v< typename std::iterator_traits< T >::iterator_category, U > > {};

template< typename T >
struct is_cpp17_input_iterator : public has_iterator_category_convertible_to< T, std::input_iterator_tag > {};
template< typename T >
inline constexpr bool is_cpp17_input_iterator_v = is_cpp17_input_iterator< T >::value;

template< typename T >
struct is_cpp17_forward_iterator : public has_iterator_category_convertible_to< T, std::forward_iterator_tag > {};
template< typename T >
inline constexpr bool is_cpp17_forward_iterator_v = is_cpp17_forward_iterator< T >::value;

template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
struct noexcept_move_assign_container : public std::integral_constant< bool,
					Traits::propagate_on_container_move_assignment::value ||
					Traits::is_always_equal::value > {};
template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
inline constexpr bool noexcept_move_assign_container_v = noexcept_move_assign_container< Alloc, Traits >::value;

template< typename T, typename U >
struct is_2d_container : std::is_same< typename T::value_type, U > {};
template< typename T, typename U >
inline constexpr bool is_2d_container_v = is_2d_container< T, U >::value;

template< typename T, typename U >
struct is_3d_container : std::is_same< typename T::value_type::value_type, U > {};
template< typename T, typename U >
inline constexpr bool is_3d_container_v = is_3d_container< T, U >::value;


} // namespace npl
