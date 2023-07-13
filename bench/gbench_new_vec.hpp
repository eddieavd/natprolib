//
//
//      natprolib
//      gbench_new_vec.hpp
//

#pragma once


#include <benchmark/benchmark.h>

#include <vector>

#include <_alloc/_stack_alloc.hpp>
#include <container/vector>


template< typename T >
static void bm_vector_push_back ( benchmark::State & state )
{
        std::vector< T > vec;

        for( auto _ : state )
        {
                for( int i = 0; i < 1000; ++i )
                {
                        vec.push_back( 1 );
                }

                benchmark::DoNotOptimize( vec );
                benchmark::ClobberMemory();

                state.PauseTiming();
                vec.clear();
                state.ResumeTiming();
        }
}

template< typename T >
static void bm_new_vector_push_back ( benchmark::State & state )
{
        npl::vector< T > vec;

        for( auto _ : state )
        {
                for( int i = 0; i < 1000; ++i )
                {
                        vec.push_back( 1 );
                }
                benchmark::DoNotOptimize( vec );
                benchmark::ClobberMemory();

                state.PauseTiming();
                vec.clear();
                state.ResumeTiming();
        }
}
