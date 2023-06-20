//
//
//      natprolib
//      gtest_prefix.cpp
//

#include "gtest_prefix.hpp"


TEST( PrefixVectorTest, DefaultConstruct )
{
        npl::prefix_vector<      int >    int_prefix;
        npl::prefix_vector< unsigned >   uint_prefix;
        npl::prefix_vector<    float >  float_prefix;
        npl::prefix_vector<   double > double_prefix;

        npl::prefix_vector< nplib_test::some_addable_data > some_prefix;

        EXPECT_EQ(    int_prefix._invariants(), true );
        EXPECT_EQ(   uint_prefix._invariants(), true );
        EXPECT_EQ(  float_prefix._invariants(), true );
        EXPECT_EQ( double_prefix._invariants(), true );

        EXPECT_EQ( some_prefix._invariants(), true );
}

TEST( PrefixVectorTest, ReserveConstruct )
{
        npl::prefix_vector< int > prefix( CUSTOM_CAPACITY );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       ,               0 );
        EXPECT_EQ( prefix.capacity()   , CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, FillConstruct )
{
        npl::prefix_vector< int > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, PrefixIterConstruct )
{
        npl::prefix_vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::prefix_vector< int > prefix( source.begin(), source.end() );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix              ,          source );
}

TEST( PrefixVectorTest, InputIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        using input_iter = nplib_test::input_iterator< int, true, decltype( source.get_allocator() ) >;

        input_iter begin( &source[ 0 ] );
        input_iter end  ( &source[ source.size() - 1 ] + 1 );

        npl::prefix_vector< int > prefix( begin, end );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix              ,           begin );
}

TEST( PrefixVectorTest, ForwardIterConstruct )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::prefix_vector< int > prefix( source.begin(), source.end() );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix              ,  source.begin() );
}

TEST( PrefixVectorTest, CopyConstruct )
{
        npl::prefix_vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::prefix_vector< int > prefix( source );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix              ,          source );
}

TEST( PrefixVectorTest, MoveConstruct )
{
        npl::prefix_vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        int source_range = source.range();

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::prefix_vector< int > prefix( NPL_MOVE( source ) );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( source_range, prefix.range() );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, InitListConstruct )
{
        npl::prefix_vector< int > prefix( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        EXPECT_EQ( prefix._invariants(), true );
        EXPECT_EQ( prefix.size()       ,    8 );
}

TEST( PrefixVectorTest, CopyAssign )
{
        npl::prefix_vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        prefix = source;

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix              ,          source );
}

TEST( PrefixVectorTest, MoveAssign )
{
        npl::prefix_vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix;

        auto source_range = source.range();

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        prefix = NPL_MOVE( source );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( prefix._invariants(),            true );
        EXPECT_EQ( prefix.size()       , CUSTOM_CAPACITY );

        EXPECT_EQ( source_range, prefix.range() );
}

TEST( PrefixVectorTest, InitListAssign )
{
        npl::prefix_vector< int > prefix;

        EXPECT_EQ( prefix._invariants(), true );
        EXPECT_EQ( prefix.size()       ,    0 );

        prefix = { 1, 1, 1, 1, 1, 1, 1, 1 };

        EXPECT_EQ( prefix._invariants(), true );
        EXPECT_EQ( prefix.size()       ,    8 );
}

TEST( PrefixVectorTest, SubscriptOperator )
{
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cpass;
        prefix_cpass = prefix_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( prefix_fill [ i ], ( i + 1 ) );
                EXPECT_EQ( prefix_piter[ i ], ( i + 1 ) );
                EXPECT_EQ( prefix_copy [ i ], ( i + 1 ) );
                EXPECT_EQ( prefix_init [ i ], ( i + 1 ) );
                EXPECT_EQ( prefix_cpass[ i ], ( i + 1 ) );
        }
}

TEST( PrefixVectorTest, At )
{
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cpass;
        prefix_cpass = prefix_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( prefix_fill .at( i ), ( i + 1 ) );
                EXPECT_EQ( prefix_piter.at( i ), ( i + 1 ) );
                EXPECT_EQ( prefix_copy .at( i ), ( i + 1 ) );
                EXPECT_EQ( prefix_init .at( i ), ( i + 1 ) );
                EXPECT_EQ( prefix_cpass.at( i ), ( i + 1 ) );
        }
}

TEST( PrefixVectorTest, ElementAt )
{
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cpass;
        prefix_cpass = prefix_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( prefix_fill .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( prefix_piter.element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( prefix_copy .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( prefix_init .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( prefix_cpass.element_at( i ), CUSTOM_VALUE );
        }
}

TEST( PrefixVectorTest, Range )
{
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cpass;
        prefix_cpass = prefix_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                for( std::size_t j = i; j < CUSTOM_CAPACITY; ++j )
                {
                        EXPECT_EQ( prefix_fill .range( i, j ), j - i + 1 );
                        EXPECT_EQ( prefix_piter.range( i, j ), j - i + 1 );
                        EXPECT_EQ( prefix_copy .range( i, j ), j - i + 1 );
                        EXPECT_EQ( prefix_init .range( i, j ), j - i + 1 );
                        EXPECT_EQ( prefix_cpass.range( i, j ), j - i + 1 );
                }
        }

        EXPECT_EQ( prefix_fill .range(), CUSTOM_CAPACITY );
        EXPECT_EQ( prefix_piter.range(), CUSTOM_CAPACITY );
        EXPECT_EQ( prefix_copy .range(), CUSTOM_CAPACITY );
        EXPECT_EQ( prefix_init .range(), CUSTOM_CAPACITY );
        EXPECT_EQ( prefix_cpass.range(), CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, Accessors )
{
        npl::prefix_vector< int > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( *prefix.begin()  ,               1 );
        EXPECT_EQ( *prefix.cbegin() ,               1 );
        EXPECT_EQ( *prefix.rbegin() , CUSTOM_CAPACITY );
        EXPECT_EQ( *prefix.crbegin(), CUSTOM_CAPACITY );

        EXPECT_EQ( *( --prefix.end()   ), CUSTOM_CAPACITY );
        EXPECT_EQ( *( --prefix.cend()  ), CUSTOM_CAPACITY );
        EXPECT_EQ( *( --prefix.rend()  ),               1 );
        EXPECT_EQ( *( --prefix.crend() ),               1 );

        EXPECT_EQ( prefix.front() ,               1 );
        EXPECT_EQ( prefix.cfront(),               1 );
        EXPECT_EQ( prefix.back()  , CUSTOM_CAPACITY );
        EXPECT_EQ( prefix.cback() , CUSTOM_CAPACITY );

        EXPECT_EQ( *( prefix.data() )                    ,               1 );
        EXPECT_EQ( *( prefix.data() + prefix.size() - 1 ), CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, PushBack )
{
        npl::prefix_vector< int > prefix_def;
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cp_assign;
        prefix_cp_assign = prefix_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .push_back( 1 );
                prefix_fill .push_back( 1 );
                prefix_piter.push_back( 1 );
                prefix_copy .push_back( 1 );
                prefix_init .push_back( 1 );

                EXPECT_EQ( prefix_def  .size(), i + 1 );
                EXPECT_EQ( prefix_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( PrefixVectorTest, EmplaceBackBasic )
{
        npl::prefix_vector< int > prefix_def;
        npl::prefix_vector< int > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< int > prefix_copy ( prefix_fill );
        npl::prefix_vector< int > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< int > prefix_cp_assign;
        prefix_cp_assign = prefix_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .emplace_back( 1 );
                prefix_fill .emplace_back( 1 );
                prefix_piter.emplace_back( 1 );
                prefix_copy .emplace_back( 1 );
                prefix_init .emplace_back( 1 );

                EXPECT_EQ( prefix_def  .size(), i + 1 );
                EXPECT_EQ( prefix_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( PrefixVectorTest, EmplaceBack )
{
        npl::prefix_vector< nplib_test::some_addable_data > prefix_def;
        npl::prefix_vector< nplib_test::some_addable_data > prefix_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< nplib_test::some_addable_data > prefix_piter( prefix_fill.begin(), prefix_fill.end() );
        npl::prefix_vector< nplib_test::some_addable_data > prefix_copy ( prefix_fill );
        npl::prefix_vector< nplib_test::some_addable_data > prefix_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::prefix_vector< nplib_test::some_addable_data > prefix_cp_assign;
        prefix_cp_assign = prefix_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                prefix_def  .emplace_back( 1 );
                prefix_fill .emplace_back( 1 );
                prefix_piter.emplace_back( 1 );
                prefix_copy .emplace_back( 1 );
                prefix_init .emplace_back( 1 );

                EXPECT_EQ( prefix_def  .size(), i + 1 );
                EXPECT_EQ( prefix_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( prefix_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( PrefixVectorTest, Swap )
{
        npl::prefix_vector< int > prefix1( CUSTOM_CAPACITY, CUSTOM_VALUE     );
        npl::prefix_vector< int > prefix2( CUSTOM_CAPACITY, CUSTOM_VALUE + 1 );

        npl::prefix_vector< int > copy1( prefix1 );
        npl::prefix_vector< int > copy2( prefix2 );

        prefix1.swap( prefix2 );

        EXPECT_EQ( prefix1, copy2 );
        EXPECT_EQ( prefix2, copy1 );
}

TEST( PrefixVectorTest, Clear )
{
        npl::prefix_vector< int > prefix( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::prefix_vector< int > empty;

        prefix.clear();

        EXPECT_EQ( prefix, empty );
}

TEST( PrefixVectorTest, TwoDimensional )
{
        npl::prefix_vector< int > prefix1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::prefix_vector< npl::prefix_vector< int > > prefix2d( CUSTOM_CAPACITY, prefix1d );

        EXPECT_EQ( prefix2d._invariants(),            true );
        EXPECT_EQ( prefix2d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( prefix2d.element_at( i ), prefix1d );

                for( std::size_t j = 0; j < prefix2d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( prefix2d.element_at( i, j ), prefix1d.element_at( j ) );
                }
        }

        EXPECT_EQ( prefix1d.range(),                   CUSTOM_CAPACITY );
        EXPECT_EQ( prefix2d.range(), CUSTOM_CAPACITY * CUSTOM_CAPACITY );
}

TEST( PrefixVectorTest, ThreeDimensional )
{
        npl::prefix_vector< int > prefix1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::prefix_vector prefix2d( CUSTOM_CAPACITY, prefix1d );
        npl::prefix_vector prefix3d( CUSTOM_CAPACITY, prefix2d );

        EXPECT_EQ( prefix3d._invariants(),            true );
        EXPECT_EQ( prefix3d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( prefix3d.element_at( i ), prefix2d );

                for( std::size_t j = 0; j < prefix3d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( prefix3d.element_at( i, j ), prefix2d.element_at( j ) );

                        for( std::size_t k = 0; k < prefix3d.at( i, j ).size(); ++k )
                        {
                                EXPECT_EQ( prefix3d.element_at( i, j, k ), prefix2d.element_at( j, k ) );
                        }
                }
        }

        EXPECT_EQ( prefix1d.range(),                                     CUSTOM_CAPACITY );
        EXPECT_EQ( prefix2d.range(),                   CUSTOM_CAPACITY * CUSTOM_CAPACITY );
        EXPECT_EQ( prefix3d.range(), CUSTOM_CAPACITY * CUSTOM_CAPACITY * CUSTOM_CAPACITY );
}

#ifdef NPL_HAS_STL

TEST( PrefixVectorTest, StdVectorTests )
{
        std::vector< int > stdvec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::prefix_vector< int > vec( stdvec.begin(), stdvec.end() );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec   [ i ], i + 1 );
                EXPECT_EQ( vec.at( i ), i + 1 );
        }

        npl::prefix_vector< int > pbvec( stdvec.begin(), stdvec.end() );
        npl::prefix_vector< int > ebvec( stdvec.begin(), stdvec.end() );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                pbvec.push_back( CUSTOM_VALUE );
                ebvec.push_back( CUSTOM_VALUE );

                EXPECT_EQ( pbvec.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ebvec.size(), CUSTOM_CAPACITY + i + 1 );
        }

}

#endif
