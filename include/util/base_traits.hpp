//
//
//      natprolib
//      primary.hpp
//

#pragma once

#if !__has_builtin(__is_final) || !__has_builtin(__is_enum) || !__has_builtin(__is_union)
#include <type_traits>
#else
#endif

#if __has_builtin(__is_final)
#       define _builtin_is_final(x) __is_final(x)
#endif
#if __has_builtin(__is_enum)
#       define _builtin_is_enum(x) __is_enum(x)
#endif
#if __has_builtin(__is_union)
#       define _builtin_is_union(x) __is_union(x)
#endif


namespace npl
{


using    size_t =       unsigned long ;
using nullptr_t = decltype( nullptr ) ;


//=====================================================================
//  ┬┌┬┐┌─┐┌┐┌┌┬┐┬┌┬┐┬ ┬
//  │ ││├┤ │││ │ │ │ └┬┘
//  ┴─┴┘└─┘┘└┘ ┴ ┴ ┴  ┴
//=====================================================================

template< typename... >
struct void_t
{
        using type = void ;
};

template< auto Val >
struct value_identity
{
        static constexpr auto value = Val ;
};

template< auto Val >
inline constexpr auto value_identity_v = value_identity< Val >::value ;

template< typename T >
struct type_identity
{
        using type = T ;
};

template< typename T >
using type_identity_t = typename type_identity< T >::type ;

template< typename T, bool >
struct dependent_type : public T {} ;


//=====================================================================
//  ┌─┐┌┐┌┌─┐┌┐ ┬  ┌─┐    ┬┌─┐
//  ├┤ │││├─┤├┴┐│  ├┤     │├┤
//  └─┘┘└┘┴ ┴└─┘┴─┘└─┘────┴└
//=====================================================================

template< bool, typename T = void >
struct enable_if {} ;

template< typename T >
struct enable_if< true, T >
{
        using type = T ;
};

template< bool C, typename T = void >
using enable_if_t = typename enable_if< C, T >::type ;


//=====================================================================
//  ┌─┐┌─┐┌┐┌┌─┐┌┬┐┌─┐┌┐┌┌┬┐┌─┐
//  │  │ ││││└─┐ │ ├─┤│││ │ └─┐
//  └─┘└─┘┘└┘└─┘ ┴ ┴ ┴┘└┘ ┴ └─┘
//=====================================================================

template< typename T, T Val >
struct integral_constant
{
        static constexpr T value = Val ;

        using value_type = T ;
        using type       = integral_constant< T, Val > ;

        constexpr integral_constant () noexcept = default ;

        template< template< typename T1, T1 Val1 > typename U >
        constexpr integral_constant ( U< T, Val > ) noexcept {}

        template< template< typename T1, T1 Val1 > typename U >
        constexpr operator U< T, Val > () const {}

        constexpr   operator value_type () const noexcept { return value; }
        constexpr value_type operator() () const noexcept { return value; }
};

template< bool B >
using bool_constant = integral_constant< bool, B > ;

using  true_type = bool_constant<  true > ;
using false_type = bool_constant< false > ;


//=====================================================================
//  ┌─┐┌─┐┌┐┌┌─┐┌┬┐
//  │  │ ││││└─┐ │
//  └─┘└─┘┘└┘└─┘ ┴
//=====================================================================

//=====================================================================
//      is_const
//=====================================================================

template< typename T >
struct is_const : false_type {} ;

template< typename T >
struct is_const< T const > : true_type {} ;

template< typename T >
inline constexpr bool is_const_v = is_const< T >::value ;

//=====================================================================
//      add_const
//=====================================================================

template< typename T >
struct add_const
{
        using type = T const ;
};

template< typename T >
using add_const_t = typename add_const< T >::type ;

//=====================================================================
//      remove_const
//=====================================================================

template< typename T >
struct remove_const : type_identity< T > {} ;

template< typename T >
struct remove_const< T const > : type_identity< T > {} ;

template< typename T >
using remove_const_t = typename remove_const< T >::type ;


//=====================================================================
//  ┬  ┬┌─┐┬  ┌─┐┌┬┐┬┬  ┌─┐
//  └┐┌┘│ ││  ├─┤ │ ││  ├┤
//   └┘ └─┘┴─┘┴ ┴ ┴ ┴┴─┘└─┘
//=====================================================================

//=====================================================================
//      is_volatile
//=====================================================================

template< typename T >
struct is_volatile : false_type {} ;

template< typename T >
struct is_volatile< T volatile > : true_type {} ;

template< typename T >
inline constexpr bool is_volatile_v = is_volatile< T >::value ;

//=====================================================================
//      add_volatile
//=====================================================================

template< typename T >
struct add_volatile
{
        using type = T volatile ;
};

template< typename T >
using add_volatile_t = typename add_volatile< T >::type ;

//=====================================================================
//      remove_volatile
//=====================================================================

template< typename T >
struct remove_volatile : type_identity< T > {} ;

template< typename T >
struct remove_volatile< T volatile > : type_identity< T > {} ;

template< typename T >
using remove_volatile_t = typename remove_volatile< T >::type ;

//=====================================================================
//      remove_cv
//=====================================================================

template< typename T >
struct remove_cv : remove_const< remove_volatile_t< T > > {} ;

template< typename T >
using remove_cv_t = typename remove_cv< T >::type ;

//=====================================================================
//      remove_reference
//=====================================================================

template< typename T >
struct remove_reference : type_identity< T > {} ;

template< typename T >
struct remove_reference< T & > : type_identity< T > {} ;

template< typename T >
struct remove_reference< T && > : type_identity< T > {} ;

template< typename T >
using remove_reference_t = typename remove_reference< T >::type ;

//=====================================================================
//      remove_cvref
//=====================================================================

template< typename T >
struct remove_cvref : remove_reference< remove_const_t< remove_volatile_t< T > > > {} ;

template< typename T >
using remove_cvref_t = typename remove_cvref< T >::type ;


//=====================================================================
//  ┌┐ ┬┌┐┌┌─┐┬─┐┬ ┬
//  ├┴┐││││├─┤├┬┘└┬┘
//  └─┘┴┘└┘┴ ┴┴└─ ┴
//=====================================================================

//=====================================================================
//      conditional
//=====================================================================

template< bool Condition, typename T, typename F >
struct conditional : type_identity< T > {} ;

template< typename T, typename F >
struct conditional< false, T, F > : type_identity< F > {} ;

template< bool Condition, typename T, typename F >
using conditional_t = typename conditional< Condition, T, F >::type ;

//=====================================================================
//      conjunction
//=====================================================================

template< typename... >
struct conjunction : true_type {} ;

template< typename T >
struct conjunction< T > : T {} ;

template< typename T, typename... Ts >
struct conjunction< T, Ts... >
        : conditional_t< bool( T::value ), conjunction< Ts... >, T > {} ;

template< typename... Args >
using conjunction_t = typename conjunction< Args... >::type ;

template< typename... Args >
inline constexpr bool conjunction_v = conjunction< Args... >::value ;

//=====================================================================
//      disjunction
//=====================================================================

template< typename... >
struct disjunction : false_type {} ;

template< typename T >
struct disjunction< T > : T {} ;

template< typename T, typename... Ts >
struct disjunction< T, Ts... >
        : conditional_t< bool( T::value ), T, disjunction< Ts... > > {} ;

template< typename... Args >
using disjunction_t = typename disjunction< Args... >::type ;

template< typename... Args >
inline constexpr bool disjunction_v = disjunction< Args... >::value ;

//=====================================================================
//      is
//=====================================================================

template< typename T >
struct is : false_type {} ;

template<>
struct is< true_type > : true_type {} ;

template< typename T >
inline constexpr bool is_v = is< T >::value ;

//=====================================================================
//      is_not
//=====================================================================

template< typename T >
struct is_not : true_type {} ;

template<>
struct is_not< true_type > : false_type {} ;

template< typename T >
inline constexpr bool is_not_v = is_not< T >::value ;

//=====================================================================
//      is_same
//=====================================================================

template< typename T1, typename T2 >
struct is_same : false_type {} ;

template< typename T >
struct is_same< T, T > : true_type {} ;

template< typename T1, typename T2 >
inline constexpr bool is_same_v = is_same< T1, T2 >::value ;

//=====================================================================
//      is_same_raw
//=====================================================================

template< typename T1, typename T2 >
struct is_same_raw : is_same< remove_cv_t< T1 >, remove_cv_t< T2 > > {} ;

template< typename T1, typename T2 >
inline constexpr bool is_same_raw_v = is_same_raw< T1, T2 >::value ;

//=====================================================================
//      is_not_same
//=====================================================================

template< typename T1, typename T2 >
struct is_not_same : true_type {} ;

template< typename T >
struct is_not_same< T, T > : false_type {} ;

template< typename T1, typename T2 >
inline constexpr bool is_not_same_v = is_not_same< T1, T2 >::value ;

//=====================================================================
//      is_one_of
//=====================================================================

template< typename T, typename... Args >
struct is_one_of ;

template< typename T >
struct is_one_of< T > : false_type {} ;

template< typename T, typename... Args >
struct is_one_of< T, T, Args... > : true_type {} ;

template< typename T, typename Arg1, typename... Args >
struct is_one_of< T, Arg1, Args... > : is_one_of< T, Args... > {} ;

template< typename T, typename... Args >
using is_one_of_t = typename is_one_of< T, Args... >::type ;

template< typename T, typename... Args >
inline constexpr bool is_one_of_v = is_one_of< T, Args... >::value ;

//=====================================================================
//      is_referenceable
//=====================================================================

struct _is_referenceable_impl
{
        template< typename T > static T &        _test( int ) ;
        template< typename T > static false_type _test( ... ) ;
};

template< typename T >
struct is_referenceable : bool_constant< is_not_same_v< decltype( _is_referenceable_impl::_test< T >( 0 ) ), false_type > > {} ;

template< typename T >
inline constexpr bool is_referenceable_v = is_referenceable< T >::value ;

//=====================================================================
//      add_lvalue_reference
//=====================================================================

template< typename T, bool = is_referenceable_v< T > >
struct _add_lvalue_reference_impl
{
        using type = T ;
};

template< typename T >
struct _add_lvalue_reference_impl< T, true >
{
        using type = T & ;
};

template< typename T >
using add_lvalue_reference_t = typename _add_lvalue_reference_impl< T >::type ;

//=====================================================================
//      add_rvalue_reference
//=====================================================================

template< typename T, bool = is_referenceable_v< T > >
struct _add_rvalue_reference_impl
{
        using type = T ;
};

template< typename T >
struct _add_rvalue_reference_impl< T, true >
{
        using type = T && ;
};

template< typename T >
using add_rvalue_reference_t = typename _add_rvalue_reference_impl< T >::type ;


//=====================================================================
//  ┌┬┐┌─┐┌─┐┬ ┬  ┬┌─┐┬
//   ││├┤ │  │ └┐┌┘├─┤│
//  ─┴┘└─┘└─┘┴─┘└┘ ┴ ┴┴─┘
//=====================================================================

template< typename T >
constexpr bool always_false = false ;

template< typename T >
add_rvalue_reference_t< T > declval () noexcept
{
        static_assert( always_false< T >, "declval not allowed in evaluated context" );
}


//=====================================================================
//  ┌─┐┬─┐┬┌┬┐┌─┐┬─┐┬ ┬
//  ├─┘├┬┘││││├─┤├┬┘└┬┘
//  ┴  ┴└─┴┴ ┴┴ ┴┴└─ ┴
//=====================================================================

//=====================================================================
//      is_final
//=====================================================================

#ifdef _builtin_is_final

template< typename T >
struct is_final : bool_constant< _builtin_is_final( T ) > {} ;

template< typename T >
inline constexpr bool is_final_v = _builtin_is_final( T ) ;

#else

template< typename T >
struct is_final : std::is_final< T > {} ;

#endif

//=====================================================================
//      is_void
//=====================================================================

template< typename T >
using is_void = is_one_of< T
                         , void
                         , void const
                         , void       volatile
                         , void const volatile
                         >;

template< typename T >
inline constexpr bool is_void_v = is_void< T >::value ;

//=====================================================================
//      is_null_pointer
//=====================================================================

template< typename T >
using is_null_pointer = is_one_of< T
                                 , nullptr_t
                                 , nullptr_t const
                                 , nullptr_t       volatile
                                 , nullptr_t const volatile
                                 >;

template< typename T >
inline constexpr bool is_null_pointer_v = is_null_pointer< T >::value ;

//=====================================================================
//      is_integral
//=====================================================================

template< typename > struct is_integral_base : false_type {} ;

template<> struct is_integral_base<      bool > : true_type {} ;
template<> struct is_integral_base<       int > : true_type {} ;
template<> struct is_integral_base<     short > : true_type {} ;
template<> struct is_integral_base<      char > : true_type {} ;
template<> struct is_integral_base<   wchar_t > : true_type {} ;
template<> struct is_integral_base<  char16_t > : true_type {} ;
template<> struct is_integral_base<  char32_t > : true_type {} ;
template<> struct is_integral_base<      long > : true_type {} ;
template<> struct is_integral_base< long long > : true_type {} ;

template< typename T >
struct is_integral : is_integral_base< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_integral_v = is_integral< T >::value ;

//=====================================================================
//      is_floating_point
//=====================================================================

template< typename > struct is_floating_base : false_type {} ;

template<> struct is_floating_base<       float > : true_type {} ;
template<> struct is_floating_base<      double > : true_type {} ;
template<> struct is_floating_base< long double > : true_type {} ;

template< typename T >
struct is_floating_point : is_floating_base< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_floating_point_v = is_floating_point< T >::value ;

//=====================================================================
//      is_array
//=====================================================================

template< typename T >
struct is_array : false_type {} ;

template< typename T, size_t N >
struct is_array< T[ N ] > : true_type {} ;

template< typename T >
struct is_array< T[] > : true_type {} ;

template< typename T >
inline constexpr bool is_array_v = is_array< T >::value ;

//=====================================================================
//      is_pointer
//=====================================================================

template< typename T >
struct _is_pointer_impl : false_type {} ;

template< typename T >
struct _is_pointer_impl< T * > : true_type {} ;

template< typename T >
struct is_pointer : _is_pointer_impl< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_pointer_v = is_pointer< T >::value ;

//=====================================================================
//      is_reference
//=====================================================================

template< typename T >
struct is_reference : false_type {} ;

template< typename T >
struct is_reference< T & > : true_type {} ;

template< typename T >
struct is_reference< T && > : true_type {} ;

template< typename T >
inline constexpr bool is_reference_v = is_reference< T >::value ;

//=====================================================================
//      is_lvalue_reference
//=====================================================================

template< typename T >
struct is_lvalue_reference : false_type {} ;

template< typename T >
struct is_lvalue_reference< T & > : true_type {} ;

template< typename T >
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference< T >::value ;

//=====================================================================
//      is_rvalue_reference
//=====================================================================

template< typename T >
struct is_rvalue_reference : false_type {} ;

template< typename T >
struct is_rvalue_reference< T && > : true_type {} ;

template< typename T >
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference< T >::value ;

//=====================================================================
//      is_enum
//=====================================================================

#ifdef _builtin_is_enum

template< typename T >
struct is_enum : bool_constant< _builtin_is_enum( T ) > {} ;

#else

template< typename T >
struct is_enum : std::is_enum< T > {} ;

#endif

template< typename T >
inline constexpr bool is_enum_v = is_enum< T >::value ;

//=====================================================================
//      is_union
//=====================================================================

#ifdef _builtin_is_union

template< typename T >
struct is_union : bool_constant< _builtin_is_union( T ) > {} ;

#else

template< typename T >
struct is_union : std::is_union< T > {} ;

#endif

template< typename T >
inline constexpr bool is_union_v = is_union< T >::value ;

//=====================================================================
//      is_class
//=====================================================================

struct _is_class_impl
{
        template< typename T > static bool_constant< !is_union_v< T > > _test ( int T::* ) ;
        template< typename T > static false_type                        _test (      ... ) ;
};

template< typename T >
struct is_class : bool_constant< is_not_same_v< decltype( _is_class_impl::_test< T >( nullptr ) ), false_type > > {} ;

template< typename T >
inline constexpr bool is_class_v = is_class< T >::value ;

//=====================================================================
//      is_function
//=====================================================================

template< typename T >
struct is_function : bool_constant< !is_const_v< T const > && !is_reference_v< T > > {} ;

template< typename T >
inline constexpr bool is_function_v = is_function< T >::value ;

//=====================================================================
//      is_member_pointer
//=====================================================================

template< typename T >
struct is_member_pointer : false_type {} ;

template< typename T, typename U >
struct is_member_pointer< T U::* > : true_type {} ;

template< typename T >
inline constexpr bool is_member_pointer_v = is_member_pointer< T >::value ;

//=====================================================================
//      is_member_function_pointer
//=====================================================================

template< typename T >
struct _is_member_function_pointer_impl : false_type {} ;

template< typename T, typename U >
struct _is_member_function_pointer_impl< T U::* > : is_function< T > {} ;

template< typename T >
struct is_member_function_pointer : _is_member_function_pointer_impl< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_member_function_pointer_v = is_member_function_pointer< T >::value ;

//=====================================================================
//      is_member_object_pointer
//=====================================================================

template< typename T >
struct is_member_object_pointer : bool_constant< is_member_pointer_v< T > && !is_member_function_pointer_v< T > > {} ;

template< typename T >
inline constexpr bool is_member_object_pointer_v = is_member_object_pointer< T >::value ;


//=====================================================================
//  ┌─┐┌─┐┌─┐┬┌─┐┌┐┌┌─┐┌┐ ┬┬  ┬┌┬┐┬ ┬
//  ├─┤└─┐└─┐││ ┬│││├─┤├┴┐││  │ │ └┬┘
//  ┴ ┴└─┘└─┘┴└─┘┘└┘┴ ┴└─┘┴┴─┘┴ ┴  ┴
//=====================================================================

#if __has_builtin(__is_assignable)

template< typename T, typename U >
struct is_assignable : bool_constant< __is_assignable( T, U ) > {} ;

#else

template< typename T, typename U >
struct is_assignable : std::is_assignable< T, U > {} ;

#endif

template< typename T, typename U >
inline constexpr bool is_assignable_v = is_assignable< T, U >::value ;

template< typename T >
struct is_copy_assignable : is_assignable< add_lvalue_reference_t< T >, add_lvalue_reference_t< add_const_t< T > > > {} ;

template< typename T >
inline constexpr bool is_copy_assignable_v = is_copy_assignable< T >::value ;

template< typename T >
struct is_move_assignable : is_assignable< add_lvalue_reference_t< T >, add_rvalue_reference_t< T > > {} ;

template< typename T >
inline constexpr bool is_move_assignable_v = is_move_assignable< T >::value ;

/*
template< typename T >
using move_assignment_t = decltype( declval< T & >() = declval< T && >() ) ;

template< typename T, typename = void >
struct is_move_assignable : false_type {} ;

template< typename T >
struct is_move_assignable< T, void_t< move_assignment_t< T > > > : is_same< move_assignment_t< T >, T & > {} ;

template< typename T >
inline constexpr bool is_move_assignable_v = is_move_assignable< T >::value ;
*/

//=====================================================================
//      nothrow_assignability
//=====================================================================

#if __has_builtin(__is_nothrow_assignable)

template< typename T, typename... Args >
struct is_nothrow_assignable : bool_constant< __is_nothrow_assignable( T, Args... ) > {} ;

#else

#endif

template< typename T, typename... Args >
inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable< T, Args... >::value ;

template< typename T >
struct is_nothrow_move_assignable : is_nothrow_assignable< T, add_rvalue_reference_t< T > > {} ;

template< typename T >
inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable< T >::value ;

template< typename T >
struct is_nothrow_copy_assignable : is_nothrow_assignable< T, add_lvalue_reference_t< T > > {} ;

template< typename T >
inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable< T >::value ;

//=====================================================================
//      remove_pointer
//=====================================================================

template< typename T >
struct _remove_pointer_impl : type_identity< T > {} ;

template< typename T >
struct _remove_pointer_impl< T * > : type_identity< T > {} ;

template< typename T >
struct remove_pointer : _remove_pointer_impl< remove_cv_t< T > > {} ;

template< typename T >
using remove_pointer_t = typename remove_pointer< T >::type ;

//=====================================================================
//      is_pointer_to_const
//=====================================================================

template< typename T >
struct _is_pointer_to_const_impl : false_type {} ;

template< typename T >
struct _is_pointer_to_const_impl< T * > : is_const< T > {} ;

template< typename T >
struct is_pointer_to_const : _is_pointer_to_const_impl< remove_cv_t< T > > {} ;

template< typename T >
inline constexpr bool is_pointer_to_const_v = is_pointer_to_const< T >::value ;


//=====================================================================
//  ┌─┐┌─┐┌┐┌┌─┐┌┬┐┬─┐┬ ┬┌─┐┌┬┐┌─┐┌┐ ┬┬  ┬┌┬┐┬ ┬
//  │  │ ││││└─┐ │ ├┬┘│ ││   │ ├─┤├┴┐││  │ │ └┬┘
//  └─┘└─┘┘└┘└─┘ ┴ ┴└─└─┘└─┘ ┴ ┴ ┴└─┘┴┴─┘┴ ┴  ┴
//=====================================================================

//=====================================================================
//      is_constructible
//=====================================================================

#if __has_builtin(__is_constructible)

template< typename T, typename... Args >
struct _is_constructible_impl : bool_constant< __is_constructible( T, Args... ) > {} ;

#else

template< typename, typename T, typename... Args >
struct _is_constructible_impl : false_type {} ;

template< typename T, typename... Args >
struct _is_constructible_impl< void_t< decltype( ::new T( declval< Args >()... ) ) >, T, Args... > : true_type {} ;

#endif

template< typename T, typename... Args >
using is_constructible = _is_constructible_impl< /* void_t<>, */ T, Args... > ;

template< typename T, typename... Args >
inline constexpr bool is_constructible_v = is_constructible< T, Args... >::value ;

//=====================================================================
//      is_trivially_constructible
//=====================================================================

#if __has_builtin(__is_trivially_constructible)

template< typename T, typename... Args >
struct _is_trivially_constructible_impl : bool_constant< __is_trivially_constructible( T, Args... ) > {} ;

#else

#endif

template< typename T, typename... Args >
using is_trivially_constructible = _is_trivially_constructible_impl< T, Args... > ;

template< typename T, typename... Args >
inline constexpr bool is_trivially_constructible_v = is_trivially_constructible< T, Args... >::value ;

//=====================================================================
//      is_nothrow_constructible
//=====================================================================

#if __has_builtin(__is_nothrow_constructible)

template< typename T, typename... Args >
struct is_nothrow_constructible : bool_constant< __is_nothrow_constructible( T, Args... ) > {} ;

#else

template< bool, bool, typename T, typename... Args > struct _is_nothrow_constructible_impl ;

template< typename T, typename... Args >
struct _is_nothrow_constructible_impl< /* is_constructible */ true, /* is_reference */ false, T, Args... >
        : bool_constant< noexcept( T( declval< Args >()... ) ) > {} ;

template< typename T >
void _implicit_conversion_to ( T ) noexcept {}

template< typename T, typename Arg >
struct _is_nothrow_constructible_impl< true, true, T, Arg >
        : bool_constant< noexcept( _implicit_conversion_to< T >( declval< Arg >() ) ) > {} ;

template< typename T, bool IsReference, typename... Args >
struct _is_nothrow_constructible_impl< false, IsReference, T, Args... >
        : public false_type {} ;

template< typename T, typename... Args >
struct is_nothrow_constructible : _is_nothrow_constructible_impl< is_constructible_v< T, Args... >, is_reference_v< T >, T, Args... > {} ;

template< typename T, size_t Ns >
struct is_nothrow_constructible< T[ Ns ] >
        : _is_nothrow_constructible_impl< is_constructible_v< T >, is_reference_v< T >, T > {} ;

#endif

template< typename T, typename... Args >
inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible< T, Args... >::value ;

//=====================================================================
//      is_destructible
//=====================================================================

//=====================================================================
//      is_nothrow_destructible
//=====================================================================

//=====================================================================
//      is_trivially_destructible
//=====================================================================

#if __has_builtin(__is_trivially_destructible)

template< typename T >
struct is_trivially_destructible : bool_constant< __is_trivially_destructible( T ) > {} ;

#else

#endif

template< typename T >
inline constexpr bool is_trivially_destructible_v = is_trivially_destructible< T >::value ;

//=====================================================================
//      is_swappable
//=====================================================================

//=====================================================================
//      is_nothrow_swappable
//=====================================================================

//=====================================================================
//      is_trivially_swappable
//=====================================================================


//=====================================================================
//      is_copy_constructible
//=====================================================================

template< typename T >
struct is_copy_constructible
        : is_constructible
          <
                T,
                add_lvalue_reference_t< add_const_t< T > >
          > {} ;

template< typename T >
inline constexpr bool is_copy_constructible_v = is_copy_constructible< T >::value ;

//=====================================================================
//      is_move_constructible
//=====================================================================

template< typename T >
struct is_move_constructible
        : is_constructible
          <
                T,
                add_rvalue_reference_t< T >
          > {} ;

template< typename T >
inline constexpr bool is_move_constructible_v = is_move_constructible< T >::value ;

//=====================================================================
//      is_trivially_copy_constructible
//=====================================================================

template< typename T >
struct is_trivially_copy_constructible : is_trivially_constructible< T, add_lvalue_reference_t< add_const_t< T > > > {} ;

template< typename T >
inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible< T >::value ;

//=====================================================================
//      is_trivially_move_constructible
//=====================================================================

template< typename T >
struct is_trivially_move_constructible : is_trivially_constructible< T, add_rvalue_reference_t< T > > {} ;

template< typename T >
inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible< T >::value ;

//=====================================================================
//      is_nothrow_default_constructible
//=====================================================================

template< typename T >
struct is_nothrow_default_constructible : is_nothrow_constructible< T > {} ;

template< typename T >
inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible< T >::value ;

//=====================================================================
//      is_nothrow_copy_constructible
//=====================================================================

template< typename T >
struct is_nothrow_copy_constructible : is_nothrow_constructible< T, add_lvalue_reference_t< add_const_t< T > > > {} ;

template< typename T >
inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible< T >::value ;

//=====================================================================
//      is_nothrow_move_constructible
//=====================================================================

template< typename T >
struct is_nothrow_move_constructible : is_nothrow_constructible< T, add_rvalue_reference_t< T > > {} ;

template< typename T >
inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible< T >::value ;


//=====================================================================
//  ┌─┐┌─┐┌┐┌┬  ┬┌─┐┬─┐┌┬┐┬┌┐ ┬┬  ┬┌┬┐┬ ┬
//  │  │ ││││└┐┌┘├┤ ├┬┘ │ │├┴┐││  │ │ └┬┘
//  └─┘└─┘┘└┘ └┘ └─┘┴└─ ┴ ┴└─┘┴┴─┘┴ ┴  ┴
//=====================================================================

//=====================================================================
//      is_convertible
//=====================================================================


namespace _detail
{


template< typename T >
auto test_returnable( int ) -> decltype( void( static_cast< T( * )() >( nullptr ) ), true_type{} );

template< typename >
auto test_returnable( ... ) -> false_type;

template< typename From, typename To >
auto test_implicitly_convertible( int ) -> decltype(
                void( declval< void( & )( To ) >()( declval< From >() ) ), true_type{} ) ;

template< typename, typename >
auto test_implicitly_convertible( ... ) -> false_type;


} // namespace _detail


template< typename From, typename To >
struct is_convertible
        : bool_constant
          <
                ( decltype( _detail::test_returnable< To >( 0 ) )::value &&
                  decltype( _detail::test_implicitly_convertible< From, To >( 0 ) )::value ) ||
                ( is_void_v< From > && is_void_v< To > )
          > {} ;

template< typename From, typename To >
inline constexpr bool is_convertible_v = is_convertible< From, To >::value ;

//=====================================================================
//      is_nothrow_convertible
//=====================================================================

template< typename From, typename To >
struct is_nothrow_convertible : conjunction< is_void< From >, is_void< To > > {} ;

template< typename From, typename To >
        requires requires
        {
                static_cast< To( * )() >( nullptr );
                { declval< void( & )( To ) noexcept >()( declval< From >() ) } noexcept;
        }
struct is_nothrow_convertible< From, To > : true_type {} ;

template< typename From, typename To >
inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible< From, To >::value ;


} // namespace npl
