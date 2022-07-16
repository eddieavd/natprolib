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


template< typename >
struct _void_t { using type = void; };

template< typename T >
struct _identity { using type = T; };

template< typename T, bool >
struct _dependent_type : public T {};

template< typename T >
struct npl_is_final : public std::integral_constant< bool, std::is_final_v< T > > {};

template< typename T >
inline constexpr bool npl_is_final_v = npl_is_final< T >::value;

template< typename Alloc, typename = void, typename = void >
struct _is_allocator : std::false_type {};

template< typename Alloc >
struct _is_allocator< Alloc,
	typename _void_t< typename Alloc::value_type >::type,
	typename _void_t< decltype( std::declval< Alloc& >().allocate( std::size_t( 0 ) ) ) >::type
	>
	: std::true_type {};

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

template< typename InputIterator >
using _iter_value_type = typename std::iterator_traits< InputIterator >::value_type;

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

template< typename T, typename = void, typename = void >
struct _is_container : std::false_type {};

template< typename T >
struct _is_container< T,
                         typename _void_t< typename T::value_type >::type,
                         typename _void_t< decltype( std::declval< T& >().at( std::size_t( 0 ) ) ) >::type
                        >
                        : std::true_type {};

template< typename Container >
struct is_2d_container : _is_container< typename Container::value_type > {};

template< typename Container >
struct is_3d_container : std::conjunction< is_2d_container< Container >, is_2d_container< typename Container::value_type > > {};

template< typename Container >
inline constexpr bool is_2d_container_v = is_2d_container< Container >::value;

template< typename Container >
inline constexpr bool is_3d_container_v = is_3d_container< Container >::value;

template< typename Container >
using _2d_container_base_t = typename Container::value_type::value_type;

template< typename Container >
using _3d_container_base_t = typename Container::value_type::value_type::value_type;

template< typename Container, typename = void, typename = void >
struct _is_npl_range_container : std::false_type {};

template< typename Container >
struct _is_npl_range_container< Container,
                                typename _void_t< typename Container::value_type >::type,
                                typename _void_t< decltype( std::declval< Container& >().range() ) >::type
                                >
                                : std::true_type {};

template< typename Container >
struct is_npl_range_container : _is_npl_range_container< Container > {};

template< typename Container >
struct is_2d_npl_range_container : std::conjunction< _is_npl_range_container< Container >, _is_npl_range_container< typename Container::value_type > > {};

template< typename Container >
using is_npl_range_container_v = typename is_npl_range_container< Container >::value;

template< typename Container >
using is_2d_npl_range_container_v = typename is_2d_npl_range_container< Container >::value;

template< typename T >
struct enable_2d_container_base : std::enable_if< is_2d_container_v< T >, _2d_container_base_t< T > > {};

template< typename T >
using enable_2d_container_base_t = typename enable_2d_container_base< T >::type;

template< typename T >
struct enable_2d_range_container_base : std::enable_if< std::conjunction_v< is_2d_container< T >, is_npl_range_container< typename T::value_type > >, _2d_container_base_t< T > > {};

template< typename T >
using enable_2d_range_container_base_t = typename enable_2d_range_container_base< T >::type;

template< typename T >
struct enable_3d_container_base : std::enable_if< is_3d_container_v< T >, _3d_container_base_t< T > > {};

template< typename T >
using enable_3d_container_base_t = typename enable_3d_container_base< T >::type;

template< typename T >
struct enable_3d_range_container_base : std::enable_if< std::conjunction_v< is_3d_container< T >, is_2d_npl_range_container< typename T::value_type > >, _3d_container_base_t< T > > {};

template< typename T >
using enable_3d_range_container_base_t = typename enable_3d_range_container_base< T >::type;


} // namespace npl
