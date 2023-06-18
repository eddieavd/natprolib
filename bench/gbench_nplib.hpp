//
//
//      natprolib
//      gbench_nplib.hpp
//

#pragma once


#include <benchmark/benchmark.h>

#include <natprolib>
#include <vector>


namespace npl_bench
{


template< typename T >
static void bm_swap_integral ( benchmark::State & state )
{
        T a = 5 ;
        T b = 3 ;

        for( auto _ : state )
        {
                for( size_t i = 0; i < 1000; ++i )
                {
                        npl::_swap_integral( a, b ) ;
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( a );
                benchmark::DoNotOptimize( b );
        }
}

template< typename T >
static void bm_swap_integral_with_temp ( benchmark::State & state )
{
        T a = 5 ;
        T b = 3 ;

        for( auto _ : state )
        {
                for( size_t i = 0; i < 1000; ++i )
                {
                        npl::swap( a, b );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( a );
                benchmark::DoNotOptimize( b );
        }
}

template< typename T >
static void bm_swap_array ( benchmark::State & state )
{
        T * a = ::new T[ state.range( 0 ) ];
        T * b = ::new T[ state.range( 0 ) ];

        for( int i = 0; i < state.range( 0 ); ++i )
        {
                a[ i ] = 1;
                b[ i ] = 2;
        }
        for( auto _ : state )
        {
                for( int i = 0; i < 100; ++i )
                {
                        for( int i = 0; i < state.range( 0 ); ++i )
                        {
                                npl::_swap_integral( a[ i ], b[ i ] );
                        }
                }
                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( a );
                benchmark::DoNotOptimize( b );
        }
        ::delete[] a;
        ::delete[] b;
}

template< typename T >
static void bm_swap_array_with_temp ( benchmark::State & state )
{
        T * a = ::new T[ state.range( 0 ) ];
        T * b = ::new T[ state.range( 0 ) ];

        for( int i = 0; i < state.range( 0 ); ++i )
        {
                a[ i ] = 1;
                b[ i ] = 2;
        }
        for( auto _ : state )
        {
                for( int i = 0; i < 100; ++i )
                {
                        for( int i = 0; i < state.range( 0 ); ++i )
                        {
                                npl::swap( a[ i ], b[ i ] );
                        }
                }
                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( a );
                benchmark::DoNotOptimize( b );
        }
        ::delete[] a;
        ::delete[] b;
}

template< typename T >
static void bm_sbuff_pfront ( benchmark::State & state )
{
        for( auto _ : state )
        {
                npl::split_buffer< T > buffer;

                for( long long i = 0; i < state.range( 0 ); ++i )
                {
                        buffer.push_front( 1 );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( buffer );
        }
}

template< typename T >
static void bm_sbuff_pback ( benchmark::State & state )
{
        for( auto _ : state )
        {
                npl::split_buffer< T > buffer;

                for( long long i = 0; i < state.range( 0 ); ++i )
                {
                        buffer.push_back( 1 );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( buffer );
        }
}

template< typename Container >
static void bm_copy_container ( benchmark::State & state )
{
        Container c;

        for( long i = 0; i < state.range( 0 ); ++i )
        {
                c.push_back( 1024 );
        }

        for( auto _ : state )
        {
                Container copy( c );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( copy );
        }
}

template< typename Container >
static void bm_push_back ( benchmark::State & state )
{
        for( auto _ : state )
        {
                Container c;

                for( long i = 0; i < state.range( 0 ); ++i )
                {
                        c.push_back( 1024 );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( c );
        }
}

template< typename Container >
static void bm_push_back_reserve ( benchmark::State & state )
{
        for( auto _ : state )
        {
                Container c( state.range( 0 ) );

                for( long i = 0; i < state.range( 0 ); ++i )
                {
                        c.push_back( 1024 );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( c );
        }
}

struct addable
{
        float x_;
        float y_;

        addable ( float x, float y ) : x_( x ), y_( y ) {};

        auto operator+ ( addable const & other ) const noexcept
        { return addable( x_ + other.x_, y_ + other.y_ ); }

        auto & operator+= ( addable const & other ) noexcept
        { x_ += other.x_; y_ += other.y_; return *this; }

        auto operator- ( addable const & other ) const noexcept
        { return addable( x_ - other.x_, y_ - other.y_ ); }

        auto & operator-= ( addable const & other ) noexcept
        { x_ -= other.x_; y_ -= other.y_; return *this; }
};

template< typename Container >
static void bm_emplace_back ( [[ maybe_unused ]] benchmark::State & state )
{
        for( auto _ : state )
        {
                Container c;

                for( long i = 0; i < state.range( 0 ); ++i )
                {
                        c.emplace_back( 128.0, 2.5 );
                }

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( c );
        }
}

template< typename Container >
static void bm_range ( [[ maybe_unused ]] benchmark::State & state )
{

}


} // namespace npl_bench
