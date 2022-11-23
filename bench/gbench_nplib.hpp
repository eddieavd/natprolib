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
