//
//
//      natprolib
//      gtest_vector.cpp
//

#include "gtest_vector.hpp"


TEST( VectorTest, DefaultConstruct )
{
        npl::vector<      int >    int_vec;
        npl::vector< unsigned >   uint_vec;
        npl::vector<    float >  float_vec;
        npl::vector<   double > double_vec;

        EXPECT_EQ(    int_vec._invariants(), true );
        EXPECT_EQ(   uint_vec._invariants(), true );
        EXPECT_EQ(  float_vec._invariants(), true );
        EXPECT_EQ( double_vec._invariants(), true );
}

TEST( VectorTest, ReserveConstruct )
{
        npl::vector< int > vec( CUSTOM_CAPACITY );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       ,               0 );
        EXPECT_EQ( vec.capacity()   , CUSTOM_CAPACITY );
}

TEST( VectorTest, FillConstruct )
{
        npl::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );

        for( auto const & val : vec )
        {
                EXPECT_EQ( val, CUSTOM_VALUE );
        }
}

TEST( VectorTest, InputIterConstruct )
{
        std::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        using input_iter = nplib_test::input_iterator< int, true, decltype( source.get_allocator() ) >;

        input_iter begin( &source[ 0 ] );
        input_iter end  ( &source[ source.size() - 1 ] + 1 );

        npl::vector< int > vec( begin, end );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );
}

TEST( VectorTest, ForwardIterConstruct )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::vector< int > vec( source.begin(), source.end() );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );
}

TEST( VectorTest, CopyConstruct )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::vector< int > vec( source );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( vec              ,          source );
}

TEST( VectorTest, MoveConstruct )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        npl::vector< int > vec( NPL_MOVE( source ) );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );

        for( auto const & val : vec )
        {
                EXPECT_EQ( val, CUSTOM_VALUE );
        }
}

TEST( VectorTest, InitListConstruct )
{
        npl::vector< int > vec( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        EXPECT_EQ( vec._invariants(), true );
        EXPECT_EQ( vec.size()       ,    8 );

        for( auto const & val : vec )
        {
                EXPECT_EQ( val, 1 );
        }
}

TEST( VectorTest, CopyAssign )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        vec = source;

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );
        EXPECT_EQ( vec              ,          source );
}

TEST( VectorTest, MoveAssign )
{
        npl::vector< int > source( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec;

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.size()       , CUSTOM_CAPACITY );

        vec = NPL_MOVE( source );

        EXPECT_EQ( source._invariants(),            true );
        EXPECT_EQ( source.empty()      ,            true );

        EXPECT_EQ( vec._invariants(),            true );
        EXPECT_EQ( vec.size()       , CUSTOM_CAPACITY );

        for( auto const & val : vec )
        {
                EXPECT_EQ( val, CUSTOM_VALUE );
        }
}

TEST( VectorTest, InitListAssign )
{
        npl::vector< int > vec;

        EXPECT_EQ( vec._invariants(), true );
        EXPECT_EQ( vec.size()       ,    0 );

        vec = { 1, 1, 1, 1, 1, 1, 1, 1 };

        EXPECT_EQ( vec._invariants(), true );
        EXPECT_EQ( vec.size()       ,    8 );

        for( auto const & val : vec )
        {
                EXPECT_EQ( val, 1 );
        }
}

TEST( VectorTest, SubscriptOperator )
{
        npl::vector< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::vector< int > vec_copy ( vec_fill );
        npl::vector< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::vector< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec_fill [ i ], 1 );
                EXPECT_EQ( vec_piter[ i ], 1 );
                EXPECT_EQ( vec_copy [ i ], 1 );
                EXPECT_EQ( vec_init [ i ], 1 );
                EXPECT_EQ( vec_cpass[ i ], 1 );
        }
}

TEST( VectorTest, At )
{
        npl::vector< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::vector< int > vec_copy ( vec_fill );
        npl::vector< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::vector< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec_fill .at( i ), 1 );
                EXPECT_EQ( vec_piter.at( i ), 1 );
                EXPECT_EQ( vec_copy .at( i ), 1 );
                EXPECT_EQ( vec_init .at( i ), 1 );
                EXPECT_EQ( vec_cpass.at( i ), 1 );
        }
}

TEST( VectorTest, Accessors )
{
        npl::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        EXPECT_EQ( *vec.begin()  , 1 );
        EXPECT_EQ( *vec.cbegin() , 1 );
        EXPECT_EQ( *vec.rbegin() , 1 );
        EXPECT_EQ( *vec.crbegin(), 1 );

        EXPECT_EQ( *( --vec.end()   ), 1 );
        EXPECT_EQ( *( --vec.cend()  ), 1 );
        EXPECT_EQ( *( --vec.rend()  ), 1 );
        EXPECT_EQ( *( --vec.crend() ), 1 );

        EXPECT_EQ( vec.front() , 1 );
        EXPECT_EQ( vec.cfront(), 1 );
        EXPECT_EQ( vec.back()  , 1 );
        EXPECT_EQ( vec.cback() , 1 );

        EXPECT_EQ( *( vec.data() )                 , 1 );
        EXPECT_EQ( *( vec.data() + vec.size() - 1 ), 1 );
}

TEST( VectorTest, PushBack )
{
        npl::vector< int > vec_def;
        npl::vector< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec_copy ( vec_fill );
        npl::vector< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::vector< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .push_back( 1 );
                vec_fill .push_back( 1 );
                vec_copy .push_back( 1 );
                vec_init .push_back( 1 );
                vec_cpass.push_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_cpass.size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( VectorTest, EmplaceBackBasic )
{
        npl::vector< int > vec_def;
        npl::vector< int > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::vector< int > vec_copy ( vec_fill );
        npl::vector< int > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::vector< int > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1 );
                vec_fill .emplace_back( 1 );
                vec_piter.emplace_back( 1 );
                vec_copy .emplace_back( 1 );
                vec_init .emplace_back( 1 );
                vec_cpass.emplace_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_cpass.size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( VectorTest, EmplaceBack )
{
        npl::vector< nplib_test::some_addable_data > vec_def;
        npl::vector< nplib_test::some_addable_data > vec_fill ( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< nplib_test::some_addable_data > vec_piter( vec_fill.begin(), vec_fill.end() );
        npl::vector< nplib_test::some_addable_data > vec_copy ( vec_fill );
        npl::vector< nplib_test::some_addable_data > vec_init ( { 1, 1, 1, 1, 1, 1, 1, 1 } );

        npl::vector< nplib_test::some_addable_data > vec_cpass;
        vec_cpass = vec_fill;

        for( std::size_t i = 0; i < 1024; ++i )
        {
                vec_def  .emplace_back( 1 );
                vec_fill .emplace_back( 1 );
                vec_piter.emplace_back( 1 );
                vec_copy .emplace_back( 1 );
                vec_init .emplace_back( 1 );

                EXPECT_EQ( vec_def  .size(), i + 1 );
                EXPECT_EQ( vec_fill .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_piter.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_copy .size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( vec_init .size(), CUSTOM_CAPACITY + i + 1 );
        }
}

TEST( VectorTest, Swap )
{
        npl::vector< int > vec1( CUSTOM_CAPACITY, CUSTOM_VALUE     );
        npl::vector< int > vec2( CUSTOM_CAPACITY, CUSTOM_VALUE + 1 );

        npl::vector< int > copy1( vec1 );
        npl::vector< int > copy2( vec2 );

        vec1.swap( vec2 );

        EXPECT_EQ( vec1, copy2 );
        EXPECT_EQ( vec2, copy1 );
}

TEST( VectorTest, Clear )
{
        npl::vector< int > vec( CUSTOM_CAPACITY, CUSTOM_VALUE );
        npl::vector< int > empty;

        vec.clear();

        EXPECT_EQ( vec, empty );
}

TEST( VectorTest, TwoDimensional )
{
        npl::vector< int > vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::vector< npl::vector< int > > vec2d( CUSTOM_CAPACITY, vec1d );

        EXPECT_EQ( vec2d._invariants(),            true );
        EXPECT_EQ( vec2d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec2d.at( i ), vec1d );

                for( std::size_t j = 0; j < vec2d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( vec2d.at( i, j ), vec1d.at( j ) );
                }
        }
}

TEST( VectorTest, ThreeDimensional )
{
        npl::vector< int > vec1d( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::vector vec2d( CUSTOM_CAPACITY, vec1d );
        npl::vector vec3d( CUSTOM_CAPACITY, vec2d );

        EXPECT_EQ( vec3d._invariants(),            true );
        EXPECT_EQ( vec3d.size()       , CUSTOM_CAPACITY );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec3d.at( i ), vec2d );

                for( std::size_t j = 0; j < vec3d.at( i ).size(); ++j )
                {
                        EXPECT_EQ( vec3d.at( i, j ), vec2d.at( j ) );

                        for( std::size_t k = 0; k < vec3d.at( i, j ).size(); ++k )
                        {
                                EXPECT_EQ( vec3d.at( i, j, k ), vec2d.at( j, k ) );
                        }
                }
        }
}

#ifdef NPL_HAS_STL

TEST( VectorTest, StdVectorTests )
{
        std::vector< int > stdvec( CUSTOM_CAPACITY, CUSTOM_VALUE );

        npl::vector< int > vec( stdvec.begin(), stdvec.end() );

        for( std::size_t i = 0; i < CUSTOM_CAPACITY; ++i )
        {
                EXPECT_EQ( vec   [ i ], CUSTOM_VALUE );
                EXPECT_EQ( vec.at( i ), CUSTOM_VALUE );
        }

        npl::vector< int > pbvec( stdvec.begin(), stdvec.end() );
        npl::vector< int > ebvec( stdvec.begin(), stdvec.end() );

        for( std::size_t i = 0; i < 1024; ++i )
        {
                pbvec.push_back( CUSTOM_VALUE );
                ebvec.push_back( CUSTOM_VALUE );

                EXPECT_EQ( pbvec.size(), CUSTOM_CAPACITY + i + 1 );
                EXPECT_EQ( ebvec.size(), CUSTOM_CAPACITY + i + 1 );
        }
}

#endif
