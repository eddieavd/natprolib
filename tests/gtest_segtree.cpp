//
//
//      natprolib
//      gtest_segtree.cpp
//

#include "gtest_segtree.hpp"


TEST( SegtreeTest, DefaultConstruct )
{
        npl::segment_tree<      int >    int_seg;
        npl::segment_tree< unsigned >   uint_seg;
        npl::segment_tree<    float >  float_seg;
        npl::segment_tree<   double > double_seg;

        EXPECT_EQ(    int_seg._invariants(), true );
        EXPECT_EQ(   uint_seg._invariants(), true );
        EXPECT_EQ(  float_seg._invariants(), true );
        EXPECT_EQ( double_seg._invariants(), true );
}

TEST( SegtreeTest, ReserveConstruct )
{
        npl::segment_tree< int > seg( CUSTOM_CAPACITY );

        EXPECT_EQ( seg._invariants(),                true );
        EXPECT_EQ( seg.size()       ,                   8 );
        EXPECT_EQ( seg.capacity()   , 2 * CUSTOM_CAPACITY );
}

TEST( SegtreeTest, FillConstruct )
{
        npl::segment_tree< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
}

TEST( SegtreeTest, SegtreeIterConstruct )
{
        npl::segment_tree< int > seg1( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( seg1._invariants(),            true );
        EXPECT_EQ( seg1.size()       , CUSTOM_CAPACITY );

        npl::segment_tree< int > seg2( seg1.begin(), seg1.end() );

        EXPECT_EQ( seg1._invariants(),            true );
        EXPECT_EQ( seg1.size()       , CUSTOM_CAPACITY );
}

/*
TEST( SegtreeTest, InputIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        using input_iter = nplib_test::input_iterator< int, true, decltype( source.get_allocator() ) >;

        input_iter begin( &source[ 0 ] );
        input_iter end  ( &source[ source.size() - 1 ] + 1 );

        npl::segment_tree< int > seg( begin, end );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
}
*/

TEST( SegtreeTest, ForwardIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > seg( source.begin(), source.end() );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
}

TEST( SegtreeTest, CopyConstruct )
{
        npl::segment_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::segment_tree< int > seg( source );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( seg              ,          source );
}

TEST( SegtreeTest, MoveConstruct )
{
        npl::segment_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::segment_tree< int > seg( NPL_MOVE( source ) );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
}

TEST( SegtreeTest, InitListConstruct )
{
        npl::segment_tree< int > seg( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        EXPECT_EQ( seg._invariants(), true );
        EXPECT_EQ( seg.size()       ,    8 );
}

TEST( SegtreeTest, CopyAssign )
{
        npl::segment_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > seg;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        seg = source;

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( seg              ,          source );
}

TEST( SegtreeTest, MoveAssign )
{
        npl::segment_tree< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > seg;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        seg = NPL_MOVE( source );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( seg._invariants(),            true );
        EXPECT_EQ( seg.size()       , CUSTOM_CAPACITY );
}

TEST( SegtreeTest, InitListAssign )
{
        npl::segment_tree< int > seg;

        EXPECT_EQ( seg._invariants(), true );
        EXPECT_EQ( seg.size()       ,    0 );

        seg = { 1, 1, 1, 1, 1, 1, 1, 1 };

        EXPECT_EQ( seg._invariants(), true );
        EXPECT_EQ( seg.size()       ,    8 );
}

/*
TEST( SegtreeTest, Assign ) // add to other containers as well, tests all assign(...)s
{

}
*/

TEST( SegtreeTest, SubscriptOperator )
{
        std::vector< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::segment_tree< int > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_copy ( vec_fill );
        npl::segment_tree< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec_fill [ i ], 1 );
                EXPECT_EQ( vec_piter[ i ], 1 );
                EXPECT_EQ( vec_fiter[ i ], 1 );
                EXPECT_EQ( vec_copy [ i ], 1 );
                EXPECT_EQ( vec_init [ i ], 1 );
                EXPECT_EQ( vec_cpass[ i ], 1 );
        }
}

TEST( SegtreeTest, At )
{
        std::vector< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::segment_tree< int > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_copy ( vec_fill );
        npl::segment_tree< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec_fill .at( i ), 1 );
                EXPECT_EQ( vec_piter.at( i ), 1 );
                EXPECT_EQ( vec_fiter.at( i ), 1 );
                EXPECT_EQ( vec_copy .at( i ), 1 );
                EXPECT_EQ( vec_init .at( i ), 1 );
                EXPECT_EQ( vec_cpass.at( i ), 1 );
        }
}

TEST( SegtreeTest, ElementAt )
{
        std::vector< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::segment_tree< int > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_copy ( vec_fill );
        npl::segment_tree< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec_fill .element_at( i + CUSTOM_CAPACITY ), 1 );
                EXPECT_EQ( vec_piter.element_at( i + CUSTOM_CAPACITY ), 1 );
                EXPECT_EQ( vec_fiter.element_at( i + CUSTOM_CAPACITY ), 1 );
                EXPECT_EQ( vec_copy .element_at( i + CUSTOM_CAPACITY ), 1 );
                EXPECT_EQ( vec_init .element_at( i + CUSTOM_CAPACITY ), 1 );
                EXPECT_EQ( vec_cpass.element_at( i + CUSTOM_CAPACITY ), 1 );
        }

}

TEST( SegtreeTest, Range )
{
        std::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int, pb_sum< int > > segtree_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int, pb_sum< int > > segtree_piter( segtree_fill.begin(), segtree_fill.end() );
        npl::segment_tree< int, pb_sum< int > > segtree_fiter( vec.begin(), vec.end() );
        npl::segment_tree< int, pb_sum< int > > segtree_copy ( segtree_fill );
        npl::segment_tree< int, pb_sum< int > > segtree_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int, pb_sum< int > > segtree_cpass;
        segtree_cpass = segtree_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                for( std::size_t j = i; j < CUSTOM_CAPACITY; ++j )
                {
                        EXPECT_EQ( segtree_fill .range( i, j ), j - i + 1 );
                        EXPECT_EQ( segtree_piter.range( i, j ), j - i + 1 );
                        EXPECT_EQ( segtree_fiter.range( i, j ), j - i + 1 );
                        EXPECT_EQ( segtree_copy .range( i, j ), j - i + 1 );
                        EXPECT_EQ( segtree_init .range( i, j ), j - i + 1 );
                        EXPECT_EQ( segtree_cpass.range( i, j ), j - i + 1 );
                }
        }
}

/*
TEST( SegtreeTest, ParentBuilders )
{
        EXPECT_EQ( true, false );
}
*/

TEST( SegtreeTest, Accessors )
{
        npl::segment_tree< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( *seg.begin()  , 1 );
        EXPECT_EQ( *seg.cbegin() , 1 );
        EXPECT_EQ( *seg.rbegin() , 1 );
        EXPECT_EQ( *seg.crbegin(), 1 );

        EXPECT_EQ( *( --seg.end()   ), 1 );
        EXPECT_EQ( *( --seg.cend()  ), 1 );
        EXPECT_EQ( *( --seg.rend()  ), 1 );
        EXPECT_EQ( *( --seg.crend() ), 1 );

        EXPECT_EQ( seg.front() , 1 );
        EXPECT_EQ( seg.cfront(), 1 );
        EXPECT_EQ( seg.back()  , 1 );
        EXPECT_EQ( seg.cback() , 1 );

        EXPECT_EQ( *( seg.data() )                     , 0 );
        EXPECT_EQ( *( seg.data() + seg.capacity() - 1 ), 1 );
}

/*
TEST( SegtreeTest, PushBack )
{
        std::vector< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > vec_def;
        npl::segment_tree< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > vec_piter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_copy ( vec_fill );
        npl::segment_tree< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .push_back( 1 );
                vec_fill .push_back( 1 );
                vec_piter.push_back( 1 );
                vec_fiter.push_back( 1 );
                vec_copy .push_back( 1 );
                vec_init .push_back( 1 );
                vec_cpass.push_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_cpass.size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( SegtreeTest, EmplaceBackBasic )
{
        std::vector< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< int > vec_def;
        npl::segment_tree< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::segment_tree< int > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< int > vec_copy ( vec_fill );
        npl::segment_tree< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1 );
                vec_fill .emplace_back( 1 );
                vec_piter.emplace_back( 1 );
                vec_fiter.emplace_back( 1 );
                vec_copy .emplace_back( 1 );
                vec_init .emplace_back( 1 );
                vec_cpass.emplace_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_cpass.size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( SegtreeTest, EmplaceBack )
{
        std::vector< nplib_test::some_addable_data > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< nplib_test::some_addable_data > vec_def;
        npl::segment_tree< nplib_test::some_addable_data > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< nplib_test::some_addable_data > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::segment_tree< nplib_test::some_addable_data > vec_fiter( seg.begin(), seg.end() );
        npl::segment_tree< nplib_test::some_addable_data > vec_copy ( vec_fill );
        npl::segment_tree< nplib_test::some_addable_data > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::segment_tree< nplib_test::some_addable_data > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1 );
                vec_fill .emplace_back( 1 );
                vec_piter.emplace_back( 1 );
                vec_fiter.emplace_back( 1 );
                vec_copy .emplace_back( 1 );
                vec_init .emplace_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_fiter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}
*/

TEST( SegtreeTest, Swap )
{
        npl::segment_tree< int > vec1( CUSTOM_CAPACITY, CUSTOM_VALUE     );
        npl::segment_tree< int > vec2( CUSTOM_CAPACITY, CUSTOM_VALUE + 1 );

        npl::segment_tree< int > copy1( vec1 );
        npl::segment_tree< int > copy2( vec2 );

        vec1.swap( vec2 );

        EXPECT_EQ( vec1, copy2 );
        EXPECT_EQ( vec2, copy1 );
}

TEST( SegtreeTest, Clear )
{
        npl::segment_tree< int > seg( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::segment_tree< int > empty;

        seg.clear();

        EXPECT_EQ( seg, empty );
}

TEST( SegtreeTest, TwoDimensional )
{
        npl::segment_tree< int > seg1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree< npl::segment_tree< int > > seg2d( CUSTOM_CAPACITY, seg1d );

        EXPECT_EQ( seg2d._invariants(),            true );
        EXPECT_EQ( seg2d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( seg2d.at( i ), seg1d );

                for( std::size_t j = 0; j < seg2d.at( i ).size(); ++j )
                {
//                        EXPECT_EQ( seg2d.at( i, j ), seg1d.at( j ) );
                        EXPECT_EQ( seg2d.at( i ).at( j ), seg1d.at( j ) );
                }
        }
}

TEST( SegtreeTest, ThreeDimensional )
{
        npl::segment_tree< int > seg1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::segment_tree seg2d( CUSTOM_CAPACITY, seg1d );
        npl::segment_tree seg3d( CUSTOM_CAPACITY, seg2d );

        EXPECT_EQ( seg3d._invariants(),            true );
        EXPECT_EQ( seg3d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( seg3d.at( i ), seg2d );

                for( std::size_t j = 0; j < seg3d.at( i ).size(); ++j )
                {
//                        EXPECT_EQ( seg3d.at( i, j ), seg2d.at( j ) );
                        EXPECT_EQ( seg3d.at( i ).at( j ), seg2d.at( j ) );

//                        for( std::size_t k = 0; k < seg3d.at( i, j ).size(); ++k )
                        for( std::size_t k = 0; k < seg3d.at( i ).at( j ).size(); ++k )
                        {
//                                EXPECT_EQ( seg3d.at( i, j, k ), seg2d.at( j, k ) );
                                EXPECT_EQ( seg3d.at( i ).at( j ).at( k ), seg2d.at( j ).at( k ) );
                        }
                }
        }
}
