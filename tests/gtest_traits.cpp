//
//
//      natprolib
//      gtest_traits.cpp
//

#include "gtest_traits.hpp"


TEST( TraitsTest, Identity )
{
        static_assert( std::is_same_v< void, npl::void_t<  int >::type > );
        static_assert( std::is_same_v< void, npl::void_t< char >::type > );
        static_assert( std::is_same_v< void, npl::void_t< long >::type > );

        static_assert( std::is_same_v< void, npl::void_t< int, char, long >::type > );

        static_assert( npl::value_identity_v<  5  > ==  5  );
        static_assert( npl::value_identity_v< '5' > == '5' );

        static_assert( std::is_same_v<  int, npl::type_identity_t<  int > > );
        static_assert( std::is_same_v< char, npl::type_identity_t< char > > );
        static_assert( std::is_same_v< long, npl::type_identity_t< long > > );

        static_assert( std::is_same_v< npl::enable_if_t< npl::true_type::value, int >, int > );
}

TEST( TraitsTest, Constants )
{
        static_assert( std::is_same_v< std::   size_t, npl::   size_t > );
        static_assert( std::is_same_v< std::nullptr_t, npl::nullptr_t > );
        static_assert( std::is_same_v< std::ptrdiff_t, npl::ptrdiff_t > );

        static_assert( npl::integral_constant< int, 5 >::value == 5 );
        static_assert( std::is_same_v< npl::integral_constant< int, 5 >::value_type, int > );

        static_assert( npl::integral_constant< int, 5 >()() == 5 );

        static_assert( std::is_same_v< std::integral_constant< int, 5 >, decltype( static_cast< std::integral_constant< int, 5 > >( npl::integral_constant< int, 5 >{} ) ) > );
        static_assert( std::is_same_v< npl::integral_constant< int, 5 >, decltype( static_cast< npl::integral_constant< int, 5 > >( std::integral_constant< int, 5 >{} ) ) > );

        static_assert( std::is_same_v< std::true_type, decltype( static_cast< std::true_type >( npl::true_type{} ) ) > );
        static_assert( std::is_same_v< npl::true_type, decltype( static_cast< npl::true_type >( std::true_type{} ) ) > );
}

TEST( TraitsTest, ConstVolatile )
{
        static_assert(                npl::is_const_v< int const          >   );
        static_assert(                npl::is_const_v< int const volatile >   );
        static_assert( npl::is_not_v< npl::is_const  < int                > > );
        static_assert( npl::is_not_v< npl::is_const  < int       volatile > > );

        static_assert( std::is_same_v< int const         , npl::add_const_t< int          > > );
        static_assert( std::is_same_v< int const volatile, npl::add_const_t< int volatile > > );

        static_assert( std::is_same_v< int         , npl::remove_const_t< int const          > > );
        static_assert( std::is_same_v< int volatile, npl::remove_const_t< int const volatile > > );

        static_assert(                npl::is_volatile_v< int       volatile >   );
        static_assert(                npl::is_volatile_v< int const volatile >   );
        static_assert( npl::is_not_v< npl::is_volatile  < int                > > );
        static_assert( npl::is_not_v< npl::is_volatile  < int const          > > );

        static_assert( std::is_same_v< int       volatile, npl::add_volatile_t< int       > > );
        static_assert( std::is_same_v< int const volatile, npl::add_volatile_t< int const > > );

        static_assert( std::is_same_v< int      , npl::remove_volatile_t< int       volatile > > );
        static_assert( std::is_same_v< int const, npl::remove_volatile_t< int const volatile > > );

        static_assert( std::is_same_v< int, npl::remove_cv_t< int                > > );
        static_assert( std::is_same_v< int, npl::remove_cv_t< int const          > > );
        static_assert( std::is_same_v< int, npl::remove_cv_t< int       volatile > > );
        static_assert( std::is_same_v< int, npl::remove_cv_t< int const volatile > > );
}

TEST( TraitsTest, BinaryOps )
{
        static_assert( std::is_same_v<   int, npl::conditional_t<  true, int, float > > );
        static_assert( std::is_same_v< float, npl::conditional_t< false, int, float > > );

        static_assert( std::is_same_v< npl::true_type, npl::conjunction_t<                                                > > );
        static_assert( std::is_same_v< npl::true_type, npl::conjunction_t< npl::true_type                                 > > );
        static_assert( std::is_same_v< npl::true_type, npl::conjunction_t< npl::true_type, npl::true_type                 > > );
        static_assert( std::is_same_v< npl::true_type, npl::conjunction_t< npl::true_type, npl::true_type, npl::true_type > > );

        static_assert( std::is_same_v< npl::false_type, npl::conjunction_t< npl::false_type                                  > > );
        static_assert( std::is_same_v< npl::false_type, npl::conjunction_t< npl:: true_type, npl::false_type                 > > );
        static_assert( std::is_same_v< npl::false_type, npl::conjunction_t< npl:: true_type, npl::false_type, npl::true_type > > );

        static_assert( std::is_same_v< npl::false_type, npl::disjunction_t<                                                   > > );
        static_assert( std::is_same_v< npl:: true_type, npl::disjunction_t< npl:: true_type                                   > > );
        static_assert( std::is_same_v< npl:: true_type, npl::disjunction_t< npl:: true_type, npl:: true_type                  > > );
        static_assert( std::is_same_v< npl:: true_type, npl::disjunction_t< npl:: true_type, npl::false_type                  > > );
        static_assert( std::is_same_v< npl:: true_type, npl::disjunction_t< npl::false_type, npl:: true_type, npl::false_type > > );

        static_assert(                npl::is_same_v< npl:: true_type, npl:: true_type >   );
        static_assert(                npl::is_same_v< npl::false_type, npl::false_type >   );
        static_assert( npl::is_not_v< npl::is_same  < npl:: true_type, npl:: true_type > > );
        static_assert( npl::is_not_v< npl::is_same  < npl::false_type, npl::false_type > > );

        static_assert(                npl::is_same_v< int, int                >   );
        static_assert( npl::is_not_v< npl::is_same  < int, int const          > > );
        static_assert( npl::is_not_v< npl::is_same  < int, int       volatile > > );
        static_assert( npl::is_not_v< npl::is_same  < int, int const volatile > > );

        static_assert( npl::is_same_raw_v< int, int                > );
        static_assert( npl::is_same_raw_v< int, int const          > );
        static_assert( npl::is_same_raw_v< int, int       volatile > );
        static_assert( npl::is_same_raw_v< int, int const volatile > );

        static_assert( !npl::is_one_of_v< int > );
        static_assert(  npl::is_one_of_v< int, float, int, short, bool > );
        static_assert( !npl::is_one_of_v< int, float, char, short, bool > );
}

TEST( TraitsTest, References )
{
        static_assert( std::is_same_v< int & , npl::add_lvalue_reference_t< int > > );
        static_assert( std::is_same_v< int &&, npl::add_rvalue_reference_t< int > > );
}

TEST( TraitsTest, PrimaryTypeCategories )
{
        static_assert( npl::is_integral_v< int                > );
        static_assert( npl::is_integral_v< int const          > );
        static_assert( npl::is_integral_v< int       volatile > );
        static_assert( npl::is_integral_v< int const volatile > );

        static_assert( npl::is_integral_v< unsigned long > );
}

TEST( TraitsTest, Declval )
{

}

TEST( TraitsTest, Assignability )
{

}

TEST( TraitsTest, Constructability )
{

}

TEST( TraitsTest, Convertibility )
{

}

TEST( TraitsTest, Allocators )
{

}

TEST( TraitsTest, Iterators )
{

}

TEST( TraitsTest, Containers )
{
        static_assert( npl::is_container_v< npl::vector< int > > );
        static_assert( npl::is_container_v< npl::array< int, 4 > > );
        static_assert( npl::is_container_v< npl::static_vector< int, 4 > > );

        static_assert( npl::is_container_v< std::vector< int > > );

        static_assert( npl::is_2d_container_v< npl::vector< npl::vector< int > > > );
        static_assert( npl::is_2d_container_v< npl::vector< npl::prefix_vector< int > > > );

        static_assert( npl::is_2d_container_v< std::vector< std::vector< int > > > );

        static_assert( !npl::is_2d_container_v< npl::vector< int > > );
        static_assert( !npl::is_2d_container_v< std::vector< int > > );

        static_assert( npl::is_3d_container_v< npl::vector< npl::vector< npl::vector< int > > > > );
        static_assert( npl::is_3d_container_v< npl::vector< npl::prefix_array< npl::prefix_vector< int >, 4 > > > );

        static_assert( npl::is_3d_container_v< std::vector< std::vector< std::vector< int > > > > );

        static_assert( std::is_same_v< int, npl::enable_2d_container_base_t< npl::vector< npl::vector< int > > > > );

        static_assert( std::is_same_v< int, npl::enable_3d_container_base_t< npl::vector< npl::vector< npl::array< int, 4 > > > > > );
}



















