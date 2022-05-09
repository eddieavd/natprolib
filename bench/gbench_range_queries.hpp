//
//  gbench_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 02/05/2022
//  Distributed under the terms of the GNU General Public License
//

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <set>
#include <benchmark/benchmark.h>

#include "../include/range_queries.h"


namespace npl_benchmark
{

namespace range_queries
{


/**
 *
 *	benchmarking speed of push_backs/inserts
 *	creates container of type Container and performs n insertions of random numbers
 *	where n is the range passed to the benchmark
 *	time includes call to rand() cause reasons...
 *
 **/
template< typename T, typename Container >
static void bm_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		Container c;
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			num = rand();

			if constexpr( std::is_same_v< Container, std::set< T > > )
			{
				c.insert( num );
			}
			else
			{
				c.push_back( num );
			}

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( c );
	}
}

/**
 *
 *	benchmarking speed of calculating sum on range [ x, y ]
 *	repeats calculation 1000 times cause the timing wasn't precise enough otherwise
 *	there's probably a smarter way to fix this...
 *
 **/
template< typename T, typename Container >
static void bm_range_sum ( benchmark::State & state )
{
	srand( time( nullptr ) );

	Container c;

	for( auto i = 0; i < state.range( 0 ); i++ )
	{
		c.push_back( rand() );
	}

	double      avg_range  = 0;
	std::size_t iterations = 0;

	for( auto _ : state )
	{
		state.PauseTiming();

		T res = 0;
		std::size_t x;
		std::size_t y;

		x = rand() % c.size();
		y = rand() % c.size();

		if( x > y )
		{
			std::swap( x, y );
		}

		iterations++;
		avg_range += y - x;

		state.ResumeTiming();

		for( auto i = 0; i < 1000; i++ )
		{
			if constexpr( std::is_same_v< Container, std::vector< T > > )
			{
				for( auto i = x; i < y; i++ )
				{
					res += c[ i ];
				}
			}
			else
			{
				res += c.range( x, y );
			}
		}

		benchmark::DoNotOptimize( res );
	}

	state.counters[      "size" ] = state.range( 0 );
	state.counters[ "avg_range" ] = avg_range / iterations;
}


} // namespace range_queries

} // namespace npl_benchmark
