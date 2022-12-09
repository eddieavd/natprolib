//
//
//      natprolib
//      gtest_static_vector.cpp
//

#include "gtest_static_vector.hpp"


TEST( StaticVectorTest, DefaultConstruct )
{
        npl::static_vector<      int, CUSTOM_CAPACITY >    int_vec;
        npl::static_vector< unsigned, CUSTOM_CAPACITY >   uint_vec;
        npl::static_vector<    float, CUSTOM_CAPACITY >  float_vec;
        npl::static_vector<   double, CUSTOM_CAPACITY > double_vec;

        static_assert(    int_vec.capacity() == CUSTOM_CAPACITY );
        static_assert(   uint_vec.capacity() == CUSTOM_CAPACITY );
        static_assert(  float_vec.capacity() == CUSTOM_CAPACITY );
        static_assert( double_vec.capacity() == CUSTOM_CAPACITY );
}

TEST( StaticVectorTest, ReserveConstruct )
{
        constexpr npl::static_vector< int, CUSTOM_CAPACITY > vec( CUSTOM_CAPACITY );

        static_assert( vec.size()     == CUSTOM_CAPACITY );
        static_assert( vec.capacity() == CUSTOM_CAPACITY );
}

TEST( StaticVectorTest, FillConstruct )
{
        constexpr npl::static_vector< int, CUSTOM_CAPACITY > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( vec.size() == CUSTOM_CAPACITY );
}

TEST( StaticVectorTest, CopyConstruct )
{
        constexpr npl::static_vector< int, CUSTOM_CAPACITY > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( source.size() == CUSTOM_CAPACITY );

        constexpr npl::static_vector< int, CUSTOM_CAPACITY > vec( source );

        static_assert( vec.size() == CUSTOM_CAPACITY );
        static_assert( vec == source );
}

TEST( StaticVectorTest, MoveConstruct )
{
        constexpr npl::static_vector< int, CUSTOM_CAPACITY > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( source.size() == CUSTOM_CAPACITY );

        constexpr npl::static_vector< int, CUSTOM_CAPACITY > vec( NPL_MOVE( source ) );

        static_assert( vec.size() == CUSTOM_CAPACITY );
}

TEST( StaticVectorTest, InitListConstruct )
{
        constexpr npl::static_vector< int, CUSTOM_CAPACITY > vec( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        static_assert( vec.size() == 8 );
}

consteval bool static_vector_test_subscript_operator () noexcept
{
        npl::static_vector< int, CUSTOM_CAPACITY > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< int, CUSTOM_CAPACITY > vec_copy ( vec_fill );
        npl::static_vector< int, CUSTOM_CAPACITY > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec_fill[ i ] != 1 || vec_copy[ i ] != 1 || vec_init[ i ] != 1 )
                {
                        return false;
                }
        }

        return true;
}

TEST( StaticVectorTest, SubscriptOperator )
{
        static_assert( static_vector_test_subscript_operator() );
}

consteval bool static_vector_test_at () noexcept
{
        npl::static_vector< int, CUSTOM_CAPACITY > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< int, CUSTOM_CAPACITY > vec_copy ( vec_fill );
        npl::static_vector< int, CUSTOM_CAPACITY > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec_fill.at( i ) != 1 || vec_copy.at( i ) != 1 || vec_init.at( i ) != 1 )
                {
                        return false;
                }
        }

        return true;
}

TEST( StaticVectorTest, At )
{
        static_assert( static_vector_test_at() );
}

consteval bool static_vector_test_accessors () noexcept
{
        npl::static_vector< int, CUSTOM_CAPACITY > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        if( *vec.  begin() != 1 ) return false;
        if( *vec. cbegin() != 1 ) return false;
//        if( *vec. rbegin() != 1 ) return false;
//        if( *vec.crbegin() != 1 ) return false;

        if( *( --vec.  end() ) != 1 ) return false;
        if( *( --vec. cend() ) != 1 ) return false;
//        if( *( --vec. rend() ) != 1 ) return false;
//        if( *( --vec.crend() ) != 1 ) return false;

        if( vec. front() != 1 ) return false;
        if( vec.cfront() != 1 ) return false;
        if( vec.  back() != 1 ) return false;
        if( vec. cback() != 1 ) return false;

        if( *( vec.data() ) != 1 ) return false;

        return true;
}

TEST( StaticVectorTest, Accessors )
{
        static_assert( static_vector_test_accessors() );
}

consteval bool static_vector_test_push_back () noexcept
{
        npl::static_vector< int, 4096 > vec_def;
        npl::static_vector< int, 4096 > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< int, 4096 > vec_copy ( vec_fill );
        npl::static_vector< int, 4096 > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .push_back( 1 );
                vec_fill .push_back( 1 );
                vec_copy .push_back( 1 );
                vec_init .push_back( 1 );

                if( vec_def.size() != i + 1 ) return false;
                if( vec_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }
        return true;
}

TEST( StaticVectorTest, PushBack )
{
        static_assert( static_vector_test_push_back() );
}

consteval bool static_vector_test_emplace_back_basic () noexcept
{
        npl::static_vector< int, 4096 > vec_def;
        npl::static_vector< int, 4096 > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< int, 4096 > vec_copy ( vec_fill );
        npl::static_vector< int, 4096 > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1 );
                vec_fill .emplace_back( 1 );
                vec_copy .emplace_back( 1 );
                vec_init .emplace_back( 1 );

                if( vec_def.size() != i + 1 ) return false;
                if( vec_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }
        return true;
}

/*
consteval bool static_vector_test_emplace_back () noexcept
{
        npl::static_vector< nplib_test::some_addable_data, 4096 > vec_def;
        npl::static_vector< nplib_test::some_addable_data, 4096 > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< nplib_test::some_addable_data, 4096 > vec_copy ( vec_fill );
        npl::static_vector< nplib_test::some_addable_data, 4096 > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1.0f );
                vec_fill .emplace_back( 1.0f );
                vec_copy .emplace_back( 1.0f );
                vec_init .emplace_back( 1.0f );

                if( vec_def.size() != i + 1 ) return false;
                if( vec_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( vec_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }

}
*/

TEST( StaticVectorTest, EmplaceBack )
{
        static_assert( static_vector_test_emplace_back_basic() );
//        static_assert( static_vector_test_emplace_back() );
}

consteval bool static_vector_test_clear () noexcept
{
        npl::static_vector< int, CUSTOM_CAPACITY > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::static_vector< int, CUSTOM_CAPACITY > empty;

        vec.clear();

        if( vec != empty ) return false;
        return true;
}

TEST( StaticVectorTest, Clear )
{
        static_assert( static_vector_test_clear() );
}

consteval bool static_vector_test_two_dimensional () noexcept
{
        npl::static_vector< int, CUSTOM_CAPACITY > vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::static_vector< npl::static_vector< int, CUSTOM_CAPACITY >, CUSTOM_CAPACITY > vec2d( CUSTOM_CAPACITY, vec1d );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec2d.at( i ) != vec1d ) return false;

                for( size_t j = 0; j < vec2d.at( i ).size(); ++j )
                {
                        if( vec2d.at( i ).at( j ) != vec1d.at( j ) ) return false;
                }
        }
        return true;
}

TEST( StaticVectorTest, TwoDimensional )
{
        static_assert( static_vector_test_two_dimensional() );
}

consteval bool static_vector_test_three_dimensional () noexcept
{
        using vec1d_t = npl::static_vector< int, CUSTOM_CAPACITY >;
        using vec2d_t = npl::static_vector< vec1d_t, CUSTOM_CAPACITY >;
        using vec3d_t = npl::static_vector< vec2d_t, CUSTOM_CAPACITY >;

        vec1d_t vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );
        vec2d_t vec2d( CUSTOM_CAPACITY, vec1d );
        vec3d_t vec3d( CUSTOM_CAPACITY, vec2d );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec3d.at( i ) != vec2d ) return false;

                for( size_t j = 0; j < CUSTOM_CAPACITY; ++j )
                {
                        if( vec3d.at( i ).at( j ) != vec2d.at( j ) ) return false;

                        for( size_t k = 0; k < CUSTOM_CAPACITY; ++k )
                        {
                                if( vec3d.at( i ).at( j ).at( k ) != vec2d.at( j ).at( k ) ) return false;
                        }
                }
        }
        return true;
}

TEST( StaticVectorTest, ThreeDimensional )
{
        static_assert( static_vector_test_three_dimensional() );
}
