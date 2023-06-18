//
//
//      natprolib
//      gtest_fenwick.cpp
//

#include "gtest_fenwick.hpp"


TEST( FenwickTreeTest, DefaultConstruct )
{
        npl::fenwick_tree<      int >    int_ftree;
        npl::fenwick_tree< unsigned >   uint_ftree;
        npl::fenwick_tree<    float >  float_ftree;
        npl::fenwick_tree<   double > double_ftree;

        npl::fenwick_tree< nplib_test::some_addable_data > some_ftree;

        EXPECT_EQ(    int_ftree._invariants(), true );
        EXPECT_EQ(   uint_ftree._invariants(), true );
        EXPECT_EQ(  float_ftree._invariants(), true );
        EXPECT_EQ( double_ftree._invariants(), true );

        EXPECT_EQ( some_ftree._invariants(), true );
}

TEST( FenwickTreeTest, ReserveConstruct )
{
        npl::fenwick_tree< int > ftree( CUSTOM_CAPACITY );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       ,               0 );
        EXPECT_EQ( ftree.capacity()   , CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, FillConstruct )
{
        npl::fenwick_tree< int > ftree( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, FtreeIterConstruct )
{
        npl::fenwick_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::fenwick_tree< int > ftree( source.begin(), source.end() );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( ftree              ,          source );
}

TEST( FenwickTreeTest, InputIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        using input_iter = nplib_test::input_iterator< int, true, decltype( source.get_allocator() ) >;

        input_iter begin( &source[ 0 ] );
        input_iter end  ( &source[ source.size() - 1 ] + 1 );

        npl::fenwick_tree< int > ftree( begin, end );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, ForwardIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        /*
        npl::fenwick_tree< int > ftree( source.begin(), source.end() );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
        */
}

TEST( FenwickTreeTest, CopyConstruct )
{
        npl::fenwick_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::fenwick_tree< int > ftree( source );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( ftree              ,          source );
}

TEST( FenwickTreeTest, MoveConstruct )
{
        npl::fenwick_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::fenwick_tree< int > ftree( NPL_MOVE( source ) );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, InitListConstruct )
{
        npl::fenwick_tree< int > ftree( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        EXPECT_EQ( ftree._invariants(), true );
        EXPECT_EQ( ftree.size()       ,    8 );
}

TEST( FenwickTreeTest, CopyAssign )
{
        npl::fenwick_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        ftree = source;

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( ftree              ,          source );
}

TEST( FenwickTreeTest, MoveAssign )
{
        npl::fenwick_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        ftree = NPL_MOVE( source );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( ftree._invariants(),            true );
        EXPECT_EQ( ftree.size()       , CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, InitListAssign )
{
        npl::fenwick_tree< int > ftree;

        EXPECT_EQ( ftree._invariants(), true );
        EXPECT_EQ( ftree.size()       ,    0 );

        ftree = { 1, 1, 1, 1, 1, 1, 1, 1 };

        EXPECT_EQ( ftree._invariants(), true );
        EXPECT_EQ( ftree.size()       ,    8 );
}

TEST( FenwickTreeTest, SubscriptOperator )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cpass;
        ftree_cpass = ftree_fill;

        for( std::size_t i = 0; i < 1; ++i )
        {
                EXPECT_EQ( ftree_fill [ i ], ( i + 1 ) );
                EXPECT_EQ( ftree_piter[ i ], ( i + 1 ) );
//                EXPECT_EQ( ftree_fiter[ i ], ( i + 1 ) );
                EXPECT_EQ( ftree_copy [ i ], ( i + 1 ) );
                EXPECT_EQ( ftree_init [ i ], ( i + 1 ) );
                EXPECT_EQ( ftree_cpass[ i ], ( i + 1 ) );
        }
}

TEST( FenwickTreeTest, At )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cpass;
        ftree_cpass = ftree_fill;

        for( std::size_t i = 0; i < 1; ++i )
        {
                EXPECT_EQ( ftree_fill .at( i ), ( i + 1 ) );
                EXPECT_EQ( ftree_piter.at( i ), ( i + 1 ) );
//                EXPECT_EQ( ftree_fiter.at( i ), ( i + 1 ) );
                EXPECT_EQ( ftree_copy .at( i ), ( i + 1 ) );
                EXPECT_EQ( ftree_init .at( i ), ( i + 1 ) );
                EXPECT_EQ( ftree_cpass.at( i ), ( i + 1 ) );
        }
}

TEST( FenwickTreeTest, ElementAt )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cpass;
        ftree_cpass = ftree_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( ftree_fill .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( ftree_piter.element_at( i ), CUSTOM_VALUE );
//                EXPECT_EQ( ftree_fiter.element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( ftree_copy .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( ftree_init .element_at( i ), CUSTOM_VALUE );
                EXPECT_EQ( ftree_cpass.element_at( i ), CUSTOM_VALUE );
        }
}

TEST( FenwickTreeTest, Range )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cpass;
        ftree_cpass = ftree_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                for( std::size_t j = i; j < CUSTOM_CAPACITY; ++j )
                {
                        EXPECT_EQ( ftree_fill .range( i, j ), j - i + 1 );
                        EXPECT_EQ( ftree_piter.range( i, j ), j - i + 1 );
//                        EXPECT_EQ( ftree_fiter.range( i, j ), j - i + 1 );
                        EXPECT_EQ( ftree_copy .range( i, j ), j - i + 1 );
                        EXPECT_EQ( ftree_init .range( i, j ), j - i + 1 );
                        EXPECT_EQ( ftree_cpass.range( i, j ), j - i + 1 );
                }
        }
}

TEST( FenwickTreeTest, Accessors )
{
        npl::fenwick_tree< int > ftree( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( *ftree.begin()  ,               1 );
        EXPECT_EQ( *ftree.cbegin() ,               1 );
        EXPECT_EQ( *ftree.rbegin() , CUSTOM_CAPACITY );
        EXPECT_EQ( *ftree.crbegin(), CUSTOM_CAPACITY );

        EXPECT_EQ( *( --ftree.end()   ), CUSTOM_CAPACITY );
        EXPECT_EQ( *( --ftree.cend()  ), CUSTOM_CAPACITY );
        EXPECT_EQ( *( --ftree.rend()  ),               1 );
        EXPECT_EQ( *( --ftree.crend() ),               1 );

        EXPECT_EQ( ftree.front() ,               1 );
        EXPECT_EQ( ftree.cfront(),               1 );
        EXPECT_EQ( ftree.back()  , CUSTOM_CAPACITY );
        EXPECT_EQ( ftree.cback() , CUSTOM_CAPACITY );

        EXPECT_EQ( *( ftree.data() )                    ,               1 );
        EXPECT_EQ( *( ftree.data() + ftree.size() - 1 ), CUSTOM_CAPACITY );
}

TEST( FenwickTreeTest, PushBack )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_def;
        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cp_assign;
        ftree_cp_assign = ftree_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                ftree_def  .push_back( 1 );
                ftree_fill .push_back( 1 );
                ftree_piter.push_back( 1 );
//                ftree_fiter.push_back( 1 );
                ftree_copy .push_back( 1 );
                ftree_init .push_back( 1 );

                EXPECT_EQ( ftree_def  .size(), i + 1 );
                EXPECT_EQ( ftree_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_piter.size(), CUSTOM_CAPACITY + i + 1 );
//                EXPECT_EQ( ftree_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( FenwickTreeTest, EmplaceBackBasic )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< int > ftree_def;
        npl::fenwick_tree< int > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< int > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< int > ftree_copy ( ftree_fill );
        npl::fenwick_tree< int > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< int > ftree_cp_assign;
        ftree_cp_assign = ftree_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                ftree_def  .emplace_back( 1 );
                ftree_fill .emplace_back( 1 );
                ftree_piter.emplace_back( 1 );
//                ftree_fiter.emplace_back( 1 );
                ftree_copy .emplace_back( 1 );
                ftree_init .emplace_back( 1 );

                EXPECT_EQ( ftree_def  .size(), i + 1 );
                EXPECT_EQ( ftree_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_piter.size(), CUSTOM_CAPACITY + i + 1 );
//                EXPECT_EQ( ftree_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( FenwickTreeTest, EmplaceBack )
{
        std::vector< nplib_test::some_addable_data > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< nplib_test::some_addable_data > ftree_def;
        npl::fenwick_tree< nplib_test::some_addable_data > ftree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< nplib_test::some_addable_data > ftree_piter( ftree_fill.begin(), ftree_fill.end() );
//        npl::fenwick_tree< nplib_test::some_addable_data > ftree_fiter( vec.begin(), vec.end() );
        npl::fenwick_tree< nplib_test::some_addable_data > ftree_copy ( ftree_fill );
        npl::fenwick_tree< nplib_test::some_addable_data > ftree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::fenwick_tree< nplib_test::some_addable_data > ftree_cp_assign;
        ftree_cp_assign = ftree_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                ftree_def  .emplace_back( 1 );
                ftree_fill .emplace_back( 1 );
                ftree_piter.emplace_back( 1 );
//                ftree_fiter.emplace_back( 1 );
                ftree_copy .emplace_back( 1 );
                ftree_init .emplace_back( 1 );

                EXPECT_EQ( ftree_def  .size(), i + 1 );
                EXPECT_EQ( ftree_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_piter.size(), CUSTOM_CAPACITY + i + 1 );
//                EXPECT_EQ( ftree_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ftree_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( FenwickTreeTest, Swap )
{
        npl::fenwick_tree< int > ftree1( CUSTOM_CAPACITY, CUSTOM_VALUE     );
        npl::fenwick_tree< int > ftree2( CUSTOM_CAPACITY, CUSTOM_VALUE + 1 );

        npl::fenwick_tree< int > copy1( ftree1 );
        npl::fenwick_tree< int > copy2( ftree2 );

        ftree1.swap( ftree2 );

        EXPECT_EQ( ftree1, copy2 );
        EXPECT_EQ( ftree2, copy1 );
}

TEST( FenwickTreeTest, Clear )
{
        npl::fenwick_tree< int > ftree( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::fenwick_tree< int > empty;

        ftree.clear();

        EXPECT_EQ( ftree, empty );
}

TEST( FenwickTreeTest, TwoDimensional )
{
        npl::fenwick_tree< int > ftree1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree< npl::fenwick_tree< int > > ftree2d( CUSTOM_CAPACITY, ftree1d );

        EXPECT_EQ( ftree2d._invariants(),            true );
        EXPECT_EQ( ftree2d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( ftree2d.element_at( i ), ftree1d );

                for( std::size_t j = 0; j < ftree2d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( ftree2d.element_at( i, j ), ftree1d.element_at( j ) );
                }
        }
}

/*
TEST( FenwickTreeTest, ThreeDimensional )
{
        npl::fenwick_tree< int > ftree1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::fenwick_tree ftree2d( CUSTOM_CAPACITY, ftree1d );
        npl::fenwick_tree ftree3d( CUSTOM_CAPACITY, ftree2d );

        EXPECT_EQ( ftree3d._invariants(),            true );
        EXPECT_EQ( ftree3d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( ftree3d.element_at( i ), ftree2d );

                for( std::size_t j = 0; j < ftree3d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( ftree3d.element_at( i, j ), ftree2d.element_at( j ) );

                        for( std::size_t k = 0; k < ftree3d.at( i, j ).size(); ++k )
                        {
                                EXPECT_EQ( ftree3d.element_at( i, j, k ), ftree2d.element_at( j, k ) );
                        }
                }
        }
}
*/
