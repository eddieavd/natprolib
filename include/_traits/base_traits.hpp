//
//
//      natprolib
//      primary.hpp
//

#pragma once

#if !__has_builtin(__is_final)                   || \
    !__has_builtin(__is_empty)                   || \
    !__has_builtin(__is_enum)                    || \
    !__has_builtin(__is_union)                   || \
    !__has_builtin(__underlying_type)            || \
    !__has_builtin(__is_trivially_constructible) || \
    !__has_builtin(__is_trivially_copyable)      || \
    ( !__has_builtin(__is_trivially_destructible)  || !__has_builtin(__has_trivial_destructor) )
#       ifdef NPL_HAS_STL
#               include <type_traits>
#       else
//              oh boi
#       endif
#else
#endif


namespace npl
{


using size_t = unsigned long ;


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
//      is_constant_evaluated
//=====================================================================

#if __has_builtin(__builtin_is_constant_evaluated)

inline constexpr bool is_constant_evaluated () noexcept
{
        return __builtin_is_constant_evaluated();
}

#elif defined( NPL_HAS_STL )

inline constexpr bool is_constant_evaluated () noexcept
{
        return std::is_constant_evaluated();
}

#else

#endif

//=====================================================================
//      remove_all_extents
//=====================================================================

template< typename T >
struct remove_all_extents
{
        using type = T ;
};

template< typename T >
struct remove_all_extents< T[] >
{
        using type = typename remove_all_extents< T >::type ;
};

template< typename T, size_t N >
struct remove_all_extents< T[ N ] >
{
        using type = typename remove_all_extents< T >::type ;
};


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
//      if
//=====================================================================

template< bool >
struct _if_impl ;

template<>
struct _if_impl< true >
{
        template< typename IfRes, typename ElseRes >
        using select_t = IfRes ;
};

template<>
struct _if_impl< false >
{
        template< typename IfRes, typename ElseRes >
        using select_t = ElseRes ;
};

template< bool Cond, typename IfRes, typename ElseRes >
using _if = typename _if_impl< Cond >::template select_t< IfRes, ElseRes > ;

//=====================================================================
//      conditional
//=====================================================================

template< bool Cond, typename If, typename Then >
struct conditional
{
        using type = If ;
};

template< typename If, typename Then >
struct conditional< false, If, Then >
{
        using type = Then ;
};

template< bool Cond, typename IfRes, typename ElseRes >
using conditional_t = typename conditional< Cond, IfRes, ElseRes >::type ;

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
//      or
//=====================================================================

template< bool >
struct _or_impl ;

template<>
struct _or_impl< true >
{
        template< typename Res, typename First, typename... Rest >
        using Result = typename _or_impl< !bool( First::value ) && sizeof...( Rest ) != 0 >::template Result< First, Rest... > ;
};

template<>
struct _or_impl< false >
{
        template< typename Res, typename... >
        using Result = Res ;
};

template< typename... Args >
using _or = typename _or_impl< sizeof...( Args ) != 0 >::template Result< false_type, Args... > ;

//=====================================================================
//      disjunction
//=====================================================================

template< typename... Args >
struct disjunction : _or< Args... > {} ;

template< typename... Args >
using disjunction_t = typename disjunction< Args... >::type ;

template< typename... Args >
inline constexpr bool disjunction_v = _or< Args... >::value ;

//=====================================================================
//      is
//=====================================================================

template< bool >
struct is : false_type {} ;

template<>
struct is< true > : true_type {} ;

template< bool C >
inline constexpr bool is_v = is< C >::value ;

//=====================================================================
//      is_not
//=====================================================================

template< bool >
struct is_not : true_type {} ;

template<>
struct is_not< true > : false_type {} ;

template< bool C >
inline constexpr bool is_not_v = is_not< C >::value ;

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
//      is_valid_expansion
//=====================================================================

template< template< typename... > typename Templ, typename... Args, typename = Templ< Args... > >
true_type _sfinae_test_impl ( int ) ;
template< template< typename... > typename, typename ... >
false_type _sfinae_test_impl ( ... ) ;

template< template< typename... > typename Templ, typename... Args >
using is_valid_expansion = decltype( _sfinae_test_impl< Templ, Args... >( 0 ) ) ;

//=====================================================================
//      is_primary_template
//=====================================================================

template< typename T >
using _test_for_primary_template = enable_if_t< is_same_v< T, typename T::_primary_template > > ;

template< typename T >
using is_primary_template = is_valid_expansion< _test_for_primary_template, T > ;

template< typename T >
inline constexpr bool is_primary_template_v = is_primary_template< T >::value ;

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


using nullptr_t = decltype( nullptr ) ;
using ptrdiff_t = decltype( declval< int * >() - declval< int * >() );


//=====================================================================
//  ┌─┐┬─┐┬┌┬┐┌─┐┬─┐┬ ┬
//  ├─┘├┬┘││││├─┤├┬┘└┬┘
//  ┴  ┴└─┴┴ ┴┴ ┴┴└─ ┴
//=====================================================================

//=====================================================================
//      is_empty
//=====================================================================

#if __has_builtin(__is_empty)

template< typename T >
struct is_empty : bool_constant< __is_empty( T ) > {} ;

#elif NPL_HAS_STL

template< typename T >
struct is_empty : std::is_empty< T > {} ;

#else

#endif

template< typename T >
inline constexpr bool is_empty_v = is_empty< T >::value ;

//=====================================================================
//      is_final
//=====================================================================

#if __has_builtin(__is_final)

template< typename T >
struct is_final : bool_constant< __is_final( T ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T >
struct is_final : std::is_final< T > {} ;

#else
#endif

template< typename T >
inline constexpr bool is_final_v = is_final< T >::value ;

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

template<> struct is_integral_base< unsigned       int > : true_type {} ;
template<> struct is_integral_base< unsigned     short > : true_type {} ;
template<> struct is_integral_base< unsigned      char > : true_type {} ;
template<> struct is_integral_base< unsigned      long > : true_type {} ;
template<> struct is_integral_base< unsigned long long > : true_type {} ;

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

#if __has_builtin(__is_enum)

template< typename T >
struct is_enum : bool_constant< __is_enum( T ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T >
struct is_enum : std::is_enum< T > {} ;

#else

#endif

template< typename T >
inline constexpr bool is_enum_v = is_enum< T >::value ;

//=====================================================================
//      is_union
//=====================================================================

#if __has_builtin(__is_union)

template< typename T >
struct is_union : bool_constant< __is_union( T ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T >
struct is_union : std::is_union< T > {} ;

#else

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
//      is_arithmetic
//=====================================================================

template< typename T >
struct is_arithmetic : bool_constant< is_integral_v< T > || is_floating_point_v< T > > {} ;

template< typename T >
inline constexpr bool is_arithmetic_v = is_arithmetic< T >::value ;

//=====================================================================
//      is_scalar
//=====================================================================

template< typename T >
struct is_scalar : bool_constant< is_arithmetic_v    < T > ||
                                  is_enum_v          < T > ||
                                  is_pointer_v       < T > ||
                                  is_member_pointer_v< T > ||
                                  is_null_pointer_v  < T > > {} ;

template< typename T >
inline constexpr bool is_scalar_v = is_scalar< T >::value ;

//=====================================================================
//      convert_to_integral
//=====================================================================

template< typename T, bool = is_enum_v< T > >
struct _underlying_type_impl ;

template< typename T >
struct _underlying_type_impl< T, false > {} ;

template< typename T >
struct _underlying_type_impl< T, true >
{
        using type = __underlying_type( T ) ;
};

template< typename T >
struct underlying_type : _underlying_type_impl< T, is_enum_v< T > > {} ;

template< typename T >
using underlying_type_t = typename underlying_type< T >::type ;


template< typename T, bool = is_enum_v< T > >
struct _sfinae_underlying_type
{
        using type = underlying_type_t< T > ;
        using _promoted_type = decltype( ( ( type ) 1 ) + 0 ) ;
};

template< typename T >
struct _sfinae_underlying_type< T, false > {} ;

inline constexpr                int convert_to_integral (                int _val_ ) { return _val_; }
inline constexpr unsigned           convert_to_integral ( unsigned           _val_ ) { return _val_; }
inline constexpr               long convert_to_integral (               long _val_ ) { return _val_; }
inline constexpr unsigned      long convert_to_integral ( unsigned      long _val_ ) { return _val_; }
inline constexpr          long long convert_to_integral (          long long _val_ ) { return _val_; }
inline constexpr unsigned long long convert_to_integral ( unsigned long long _val_ ) { return _val_; }

inline constexpr         __int128_t convert_to_integral (         __int128_t _val_ ) { return _val_; }
inline constexpr        __uint128_t convert_to_integral (        __uint128_t _val_ ) { return _val_; }

// template< typename T >
// inline constexpr
// enable_if_t< is_integral_v< T >, T >
// convert_to_integral ( T _val_ ) { return _val_; }

template< typename F >
inline constexpr
enable_if_t< is_floating_point_v< F >, long long >
convert_to_integral ( F _val_ ) { return _val_; }

template< typename T >
inline constexpr
typename _sfinae_underlying_type< T >::_promoted_type
convert_to_integral ( T _val_ ) { return _val_; }

//=====================================================================
//      is_callable
//=====================================================================

template< typename Func, typename... Args,
          typename = decltype( declval< Func >()( declval< Args >()... ) ) >
true_type _is_callable_impl ( int ) ;
template< typename... >
false_type _is_callable_impl ( ... ) ;

template< typename Func, typename... Args >
struct is_callable : decltype( _is_callable_impl< Func, Args... >( 0 ) ) {} ;

template< typename Func, typename... Args >
inline constexpr bool is_callable_v = is_callable< Func, Args... >::value ;

//=====================================================================
//      is_invocable
//=====================================================================


//=====================================================================
//  ┌─┐┌─┐┌─┐┬┌─┐┌┐┌┌─┐┌┐ ┬┬  ┬┌┬┐┬ ┬
//  ├─┤└─┐└─┐││ ┬│││├─┤├┴┐││  │ │ └┬┘
//  ┴ ┴└─┘└─┘┴└─┘┘└┘┴ ┴└─┘┴┴─┘┴ ┴  ┴
//=====================================================================

#if __has_builtin(__is_assignable)

template< typename T, typename U >
struct is_assignable : bool_constant< __is_assignable( T, U ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T, typename U >
struct is_assignable : std::is_assignable< T, U > {} ;

#else

template< typename, typename T >
struct _select_2nd { using type = T ; };

template< typename T, typename Arg >
typename _select_2nd< decltype( ( declval< T >() = declval< Arg >() ) ), true_type >::type _is_assignable_test( int ) ;

template< typename, typename > false_type _is_assignable_test ( ... ) ;

template< typename T, typename Arg, bool = is_void_v< T > || is_void_v< Arg > >
struct _is_assignable_impl
        : public decltype( ( _is_assignable_test< T, Arg >( 0 ) ) ) {} ;

template< typename T, typename Arg >
struct _is_assignable_impl< T, Arg, true >
        : public false_type {} ;

template< typename T, typename Arg >
struct is_assignable
        : public _is_assignable_impl< T, Arg > {} ;

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

//=====================================================================
//      nothrow_assignability
//=====================================================================

#if __has_builtin(__is_nothrow_assignable)

template< typename T, typename... Args >
struct is_nothrow_assignable : bool_constant< __is_nothrow_assignable( T, Args... ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T, typename... Args >
struct is_nothrow_assignable : std::is_nothrow_assignable< T, Args... > {} ;

#else

template< bool, typename T, typename Arg > struct _is_nothrow_assignable_impl ;

template< typename T, typename Arg >
struct _is_nothrow_assignable_impl< false, T, Arg > : public false_type {} ;

template< typename T, typename Arg >
struct _is_nothrow_assignable_impl< true, T, Arg >
        : public bool_constant< noexcept( declval< T >() = declval< Arg >() ) > {} ;

template< typename T, typename Arg >
struct is_nothrow_assignable
        : public _is_nothrow_assignable_impl< is_assignable_v< T, Arg >, T, Arg > {} ;

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

#elif defined( NPL_HAS_STL )

template< typename T, typename... Args >
struct _is_constructible_impl : std::is_constructible< T, Args... > {} ;

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

#elif defined( NPL_HAS_STL )

template< typename T, typename... Args >
struct _is_trivially_constructible_impl : std::is_trivially_constructible< T, Args... > {} ;

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

#elif defined( NPL_HAS_STL )

template< typename T, typename... Args >
struct is_nothrow_constructible : std::is_nothrow_constructible< T, Args... > {} ;

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

template< typename >
struct _is_destructible_apply
{
        using type = int ;
};

template< typename T >
struct _is_destructor_wellformed
{
        template< typename T1 >
        static true_type _test ( typename _is_destructible_apply< decltype( declval< T1 & >().~T1() ) >::type ) ;

        template< typename T1 >
        static false_type _test ( ... ) ;

        static const bool value = decltype( _test< T >( 8372 ) )::value ;
};

template< typename, bool >
struct _destructible_impl ;

template< typename T >
struct _destructible_impl< T, false >
        : public bool_constant< _is_destructor_wellformed< typename remove_all_extents< T >::type >::value > {} ;

template< typename T >
struct _destructible_impl< T, true >
        : public true_type {} ;

template< typename T, bool >
struct _destructible_false ;

template< typename T >
struct _destructible_false< T, false > : public _destructible_impl< T, is_reference_v< T > > {} ;

template< typename T >
struct _destructible_false< T, true > : public false_type {} ;

template< typename T >
struct is_destructible : public _destructible_false< T, is_function< T >::value > {} ;

template< typename T >
struct is_destructible< T[] > : public false_type {} ;

template<>
struct is_destructible< void > : public false_type {} ;

template< typename T >
inline constexpr bool is_destructible_v = is_destructible< T >::value ;

//=====================================================================
//      is_nothrow_destructible
//=====================================================================

template< bool, typename T >
struct _is_nothrow_destructible_impl ;

template< typename T >
struct _is_nothrow_destructible_impl< false, T >
        : public false_type {} ;

template< typename T >
struct _is_nothrow_destructible_impl< true, T >
        : public bool_constant< noexcept( declval< T >().~T() ) > {} ;

template< typename T >
struct is_nothrow_destructible : public _is_nothrow_destructible_impl< is_destructible_v< T >, T > {} ;

template< typename T >
inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible< T >::value ;

//=====================================================================
//      is_trivially_destructible
//=====================================================================

#if __has_builtin(__is_trivially_destructible)

template< typename T >
struct is_trivially_destructible : bool_constant< __is_trivially_destructible( T ) > {} ;

#elif __has_builtin(__has_trivial_destructor)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wdeprecated-builtins"

template< typename T >
struct is_trivially_destructible : bool_constant< __has_trivial_destructor( T ) > {} ;

#pragma GCC diagnostic pop

#elif defined( NPL_HAS_STL )

template< typename T >
struct is_trivially_destructible : std::is_trivially_destructible< T > {} ;

#elif 0  //  works only prior to cpp11

template< typename T >
struct _trivial_destructor_impl : public bool_constant< is_scalar_v< T > || is_reference_v< T > > {} ;

template< typename T >
struct is_trivially_destructible : _trivial_destructor_impl< typename remove_all_extents< T >::type > {} ;

template< typename T >
struct is_trivially_destructible< T[] > : public false_type {} ;

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
//      is_trivially_copyable
//=====================================================================

#if __has_builtin(__is_trivially_copyable)

template< typename T >
struct is_trivially_copyable : bool_constant< __is_trivially_copyable( T ) > {} ;

#elif defined( NPL_HAS_STL )

template< typename T >
struct is_trivially_copyable : std::is_trivially_copyable< T > {} ;

#else

#endif

template< typename T >
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable< T >::value ;

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

template< typename T >
auto test_returnable( int ) -> decltype( void( static_cast< T( * )() >( nullptr ) ), true_type{} );

template< typename >
auto test_returnable( ... ) -> false_type;

template< typename From, typename To >
auto test_implicitly_convertible( int ) -> decltype(
                void( declval< void( & )( To ) >()( declval< From >() ) ), true_type{} ) ;

template< typename, typename >
auto test_implicitly_convertible( ... ) -> false_type;


template< typename From, typename To >
struct is_convertible
        : bool_constant
          <
                ( decltype( test_returnable< To >( 0 ) )::value &&
                  decltype( test_implicitly_convertible< From, To >( 0 ) )::value ) ||
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
