//
//
//      natprolib
//      traits.hpp
//

#pragma once


#include <memory>

#define NPL_MOVE_IF_NOEXCEPT(...) \
        _move_if_noexcept(__VA_ARGS__)


namespace npl
{


using size_t = std::size_t;


template< typename >
struct _void_t { using type = void; };

template< typename T >
struct _identity { using type = T; };

template< typename T, bool >
struct _dependent_type : public T {};

template< typename T >
constexpr typename std::conditional_t<
                                        !std::is_nothrow_move_constructible_v< T > &&
                                         std::is_copy_constructible_v< T >,
                                         T const &,
                                         T &&
                                     >
_move_if_noexcept ( T & val ) noexcept
{
        return NPL_MOVE( val );
}

template< typename T >
struct is_final : public std::is_final< T > {};

template< typename T >
inline constexpr bool is_final_v = is_final< T >::value;

template< typename Alloc, typename = void, typename = void >
struct _is_allocator : std::false_type {};

template< typename Alloc >
struct _is_allocator<
                        Alloc,
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

template< typename Alloc, typename... Args,
          typename = decltype( std::declval< Alloc >().construct( std::declval< Args >()... ) ) >
static std::true_type test_has_construct( int );
template< typename Alloc, typename... >
static std::false_type test_has_construct( ... );

#pragma GCC diagnostic pop

template< typename Alloc, typename... Args >
struct has_construct : decltype( test_has_construct< Alloc, Args... >( 0 ) ) {};

template< typename Alloc, typename... Args >
inline constexpr bool has_construct_v = has_construct< Alloc, Args... >::value;

template< typename Alloc,
          bool = has_construct< Alloc, typename Alloc::value_type *, typename Alloc::value_type && >::value && !is_default_allocator< Alloc >::value >
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
        struct _two { char _lx; char _lxx; };
        template< typename U > static _two _test( ... );
        template< typename U > static char _test( typename U::iterator_category* = 0 );
public:
        static bool const value = sizeof( _test< T >( 0 ) ) == 1;
};

template< typename T, typename U, bool = has_iterator_category< std::iterator_traits< T > >::value >
struct has_iterator_category_convertible_to
        : public std::integral_constant< bool, std::is_convertible_v< typename std::iterator_traits< T >::iterator_category, U > > {};

template< typename T, typename U >
struct has_iterator_category_convertible_to< T, U, false > : public std::false_type {};

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
struct _is_container<
                        T,
                        typename _void_t< typename T::value_type >::type,
                        typename _void_t< decltype( std::declval< T& >().at( std::size_t( 0 ) ) ) >::type
                    >
                    : std::true_type {};

template< typename T, typename = void, typename = void >
struct _has_range : std::false_type {};

template< typename T >
struct _has_range<
                        T,
                        typename _void_t< typename T::value_type >::type,
                        typename _void_t< decltype( std::declval< T& >().range() ) >::type
                 >
                 : std::true_type {};

template< typename T >
struct is_container : _is_container< T > {};

template< typename T >
inline constexpr bool is_container_v = is_container< T >::value;

template< typename T >
struct is_range_container : std::conjunction<
                                                _is_container< T >,
                                                _has_range< T >
                                            > {};

template< typename T >
inline constexpr bool is_range_container_v = is_range_container< T >::value;

template< typename Container >
struct is_2d_container : std::conjunction<
                                                is_container< Container >,
                                                is_container< typename Container::value_type >
                                         > {};

template< typename Container >
inline constexpr bool is_2d_container_v = is_2d_container< Container >::value;

template< typename Container >
struct is_2d_range_container : std::conjunction<
                                                        is_range_container< Container >,
                                                        is_range_container< typename Container::value_type >
                                               > {};

template< typename Container >
inline constexpr bool is_2d_range_container_v = is_2d_range_container< Container >::value;

template< typename Container >
struct is_3d_container : std::conjunction<
                                                is_2d_container< Container >,
                                                is_2d_container< typename Container::value_type >
                                         > {};

template< typename Container >
inline constexpr bool is_3d_container_v = is_3d_container< Container >::value;

template< typename Container >
struct is_3d_range_container : std::conjunction<
                                                        is_2d_range_container< Container >,
                                                        is_2d_range_container< typename Container::value_type >
                                               > {};

template< typename Container >
inline constexpr bool is_3d_range_container_v = is_3d_range_container< Container >::value;

template< typename Container >
using _2d_container_base_t = typename Container::value_type::value_type;

template< typename Container >
using _3d_container_base_t = typename Container::value_type::value_type::value_type;

template< typename Container >
struct enable_2d_container_base : std::enable_if< is_2d_container_v< Container >, _2d_container_base_t< Container > > {};

template< typename Container >
using enable_2d_container_base_t = typename enable_2d_container_base< Container >::type;

template< typename Container >
struct enable_2d_range_container_base : std::enable_if< is_2d_range_container_v< Container >, _2d_container_base_t< Container > > {};

template< typename Container >
using enable_2d_range_container_base_t = typename enable_2d_range_container_base< Container >::type;

template< typename Container >
struct enable_3d_container_base : std::enable_if< is_3d_container_v< Container >, _3d_container_base_t< Container > > {};

template< typename Container >
using enable_3d_container_base_t = typename enable_3d_container_base< Container >::type;

template< typename Container >
struct enable_3d_range_container_base : std::enable_if< is_3d_range_container_v< Container >, _3d_container_base_t< Container > > {};

template< typename Container >
using enable_3d_range_container_base_t = typename enable_3d_range_container_base< Container >::type;


} // namespace npl
