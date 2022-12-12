//
//
//      natprolib
//      gtest_static_prefix.cpp
//

#include "gtest_static_prefix.hpp"


TEST( StaticPrefixTest, DefaultConstruct )
{
        npl::prefix_array<      int, CUSTOM_CAPACITY >    int_prefix;
        npl::prefix_array< unsigned, CUSTOM_CAPACITY >   uint_prefix;
        npl::prefix_array<    float, CUSTOM_CAPACITY >  float_prefix;
        npl::prefix_array<   double, CUSTOM_CAPACITY > double_prefix;

        static_assert(    int_prefix.capacity() == CUSTOM_CAPACITY );
        static_assert(   uint_prefix.capacity() == CUSTOM_CAPACITY );
        static_assert(  float_prefix.capacity() == CUSTOM_CAPACITY );
        static_assert( double_prefix.capacity() == CUSTOM_CAPACITY );
}

TEST( StaticPrefixTest, ReserveConstruct )
{
        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > prefix( CUSTOM_CAPACITY );

        static_assert( prefix.size()     == CUSTOM_CAPACITY );
        static_assert( prefix.capacity() == CUSTOM_CAPACITY );
}

TEST( StaticPrefixTest, FillConstruct )
{
        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( prefix.size() == CUSTOM_CAPACITY );
}

TEST( StaticPrefixTest, CopyConstruct )
{
        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( source.size() == CUSTOM_CAPACITY );

        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > prefix( source );

        static_assert( prefix.size() == CUSTOM_CAPACITY );
        static_assert( prefix == source );
}

TEST( StaticPrefixTest, MoveConstruct )
{
        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        static_assert( source.size() == CUSTOM_CAPACITY );

        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > prefix( NPL_MOVE( source ) );

        static_assert( prefix.size() == CUSTOM_CAPACITY );
}

TEST( StaticPrefixTest, InitListConstruct )
{
        constexpr npl::prefix_array< int, CUSTOM_CAPACITY > prefix( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        static_assert( prefix.size() == 8 );
}

consteval bool prefix_array_test_subscript_operator () noexcept
{
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_copy ( prefix_fill );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( prefix_fill[ i ] != i + 1 || prefix_copy[ i ] != i + 1 || prefix_init[ i ] != i + 1 )
                {
                        return false;
                }
        }

        return true;
}

TEST( StaticPrefixTest, SubscriptOperator )
{
        static_assert( prefix_array_test_subscript_operator() );
}

consteval bool prefix_array_test_at () noexcept
{
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_copy ( prefix_fill );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( prefix_fill.at( i ) != i + 1 || prefix_copy.at( i ) != i + 1 || prefix_init.at( i ) != i + 1 )
                {
                        return false;
                }
        }

        return true;
}

TEST( StaticPrefixTest, At )
{
        static_assert( prefix_array_test_at() );
}

consteval bool prefix_array_test_element_at () noexcept
{
        npl::prefix_array< int, CUSTOM_CAPACITY > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< int, CUSTOM_CAPACITY > prefix_copy ( prefix_fill );
        npl::prefix_array< int, CUSTOM_CAPACITY > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( prefix_fill.element_at( i ) != 1 || prefix_copy.element_at( i ) != 1 || prefix_init.element_at( i ) != 1 )
                {
                        return false;
                }
        }

        return true;
}

TEST( StaticPrefixTest, ElementAt )
{
        static_assert( prefix_array_test_element_at() );
}

consteval bool prefix_array_test_range () noexcept
{
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_copy ( prefix_fill );
        npl::prefix_array< size_t, CUSTOM_CAPACITY > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                for( size_t j = i; j < CUSTOM_CAPACITY; ++j )
                {
                        if( prefix_fill.range( i, j ) != j - i + 1 ) return false;
                        if( prefix_copy.range( i, j ) != j - i + 1 ) return false;
                        if( prefix_init.range( i, j ) != j - i + 1 ) return false;
                }
        }

        if( prefix_fill.range() != CUSTOM_CAPACITY ) return false;
        if( prefix_copy.range() != CUSTOM_CAPACITY ) return false;
        if( prefix_init.range() != CUSTOM_CAPACITY ) return false;

        return true;
}

TEST( StaticPrefixTest, Range )
{
        static_assert( prefix_array_test_range() );
}

consteval bool prefix_array_test_accessors () noexcept
{
        npl::prefix_array< int, CUSTOM_CAPACITY > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );

        if( *prefix.  begin() != 1 ) return false;
        if( *prefix. cbegin() != 1 ) return false;
//        if( *prefix. rbegin() != CUSTOM_CAPACITY ) return false;
//        if( *prefix.crbegin() != CUSTOM_CAPACITY ) return false;

        if( *( --prefix.  end() ) != CUSTOM_CAPACITY ) return false;
        if( *( --prefix. cend() ) != CUSTOM_CAPACITY ) return false;
//        if( *( --prefix. rend() ) != 1 ) return false;
//        if( *( --prefix.crend() ) != 1 ) return false;

        if( prefix. front() !=               1 ) return false;
        if( prefix.cfront() !=               1 ) return false;
        if( prefix.  back() != CUSTOM_CAPACITY ) return false;
        if( prefix. cback() != CUSTOM_CAPACITY ) return false;

        if( *( prefix.data() ) != 1 ) return false;

        return true;
}

TEST( StaticPrefixTest, Accessors )
{
        static_assert( prefix_array_test_accessors() );
}

consteval bool prefix_array_test_push_back () noexcept
{
        npl::prefix_array< int, 4096 > prefix_def;
        npl::prefix_array< int, 4096 > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< int, 4096 > prefix_copy ( prefix_fill );
        npl::prefix_array< int, 4096 > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .push_back( 1 );
                prefix_fill .push_back( 1 );
                prefix_copy .push_back( 1 );
                prefix_init .push_back( 1 );

                if( prefix_def.size() != i + 1 ) return false;
                if( prefix_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }
        return true;
}

TEST( StaticPrefixTest, PushBack )
{
        static_assert( prefix_array_test_push_back() );
}

consteval bool prefix_array_test_emplace_back_basic () noexcept
{
        npl::prefix_array< int, 4096 > prefix_def;
        npl::prefix_array< int, 4096 > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< int, 4096 > prefix_copy ( prefix_fill );
        npl::prefix_array< int, 4096 > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .emplace_back( 1 );
                prefix_fill .emplace_back( 1 );
                prefix_copy .emplace_back( 1 );
                prefix_init .emplace_back( 1 );

                if( prefix_def.size() != i + 1 ) return false;
                if( prefix_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }
        return true;
}

/*
consteval bool prefix_array_test_emplace_back () noexcept
{
        npl::prefix_array< nplib_test::some_addable_data, 4096 > prefix_def;
        npl::prefix_array< nplib_test::some_addable_data, 4096 > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< nplib_test::some_addable_data, 4096 > prefix_copy ( prefix_fill );
        npl::prefix_array< nplib_test::some_addable_data, 4096 > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .emplace_back( 1.0f );
                prefix_fill .emplace_back( 1.0f );
                prefix_copy .emplace_back( 1.0f );
                prefix_init .emplace_back( 1.0f );

                if( prefix_def.size() != i + 1 ) return false;
                if( prefix_fill.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_copy.size() != CUSTOM_CAPACITY + i + 1 ) return false;
                if( prefix_init.size() != CUSTOM_CAPACITY + i + 1 ) return false;
        }

}
*/

TEST( StaticPrefixTest, EmplaceBack )
{
        static_assert( prefix_array_test_emplace_back_basic() );
//        static_assert( prefix_array_test_emplace_back() );
}

consteval bool prefix_array_test_clear () noexcept
{
        npl::prefix_array< int, CUSTOM_CAPACITY > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_array< int, CUSTOM_CAPACITY > empty;

        prefix.clear();

        if( prefix != empty ) return false;
        return true;
}

TEST( StaticPrefixTest, Clear )
{
        static_assert( prefix_array_test_clear() );
}

consteval bool prefix_array_test_two_dimensional () noexcept
{
        npl::prefix_array< int, CUSTOM_CAPACITY > vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::prefix_array< npl::prefix_array< int, CUSTOM_CAPACITY >, CUSTOM_CAPACITY > vec2d( CUSTOM_CAPACITY, vec1d );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec2d.element_at( i ) != vec1d ) return false;

                for( size_t j = 0; j < vec2d.element_at( i ).size(); ++j )
                {
                        if( vec2d.element_at( i, j ) != vec1d.element_at( j ) ) return false;
                }
        }

        if( vec1d.range() != CUSTOM_CAPACITY                   ) return false;
        if( vec2d.range() != CUSTOM_CAPACITY * CUSTOM_CAPACITY ) return false;

        return true;
}

TEST( StaticPrefixTest, TwoDimensional )
{
        static_assert( prefix_array_test_two_dimensional() );
}

consteval bool prefix_array_test_three_dimensional () noexcept
{
        using vec1d_t = npl::prefix_array< int, CUSTOM_CAPACITY >;
        using vec2d_t = npl::prefix_array< vec1d_t, CUSTOM_CAPACITY >;
        using vec3d_t = npl::prefix_array< vec2d_t, CUSTOM_CAPACITY >;

        vec1d_t vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );
        vec2d_t vec2d( CUSTOM_CAPACITY, vec1d );
        vec3d_t vec3d( CUSTOM_CAPACITY, vec2d );

        for( size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                if( vec3d.element_at( i ) != vec2d ) return false;

                for( size_t j = 0; j < CUSTOM_CAPACITY; ++j )
                {
                        if( vec3d.element_at( i, j ) != vec2d.element_at( j ) ) return false;

                        for( size_t k = 0; k < CUSTOM_CAPACITY; ++k )
                        {
                                if( vec3d.element_at( i, j, k ) != vec2d.element_at( j, k ) ) return false;
                        }
                }
        }

        if( vec1d.range() != CUSTOM_CAPACITY                                     ) return false;
        if( vec2d.range() != CUSTOM_CAPACITY * CUSTOM_CAPACITY                   ) return false;
        if( vec3d.range() != CUSTOM_CAPACITY * CUSTOM_CAPACITY * CUSTOM_CAPACITY ) return false;

        return true;
}

TEST( StaticPrefixTest, ThreeDimensional )
{
        static_assert( prefix_array_test_three_dimensional() );
}
