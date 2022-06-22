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

#include "../include/range_queries/range_queries.h"


namespace npl_benchmark
{

namespace rq
{


/**
 *
 *	benchmarking speed of push_backs/inserts
 *	creates container of type Container and performs n insertions
 *	where n is the range passed to the benchmark
 *
 **/
template< typename T, typename Container >
static void bm_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		int num = rand();
		Container c;

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			num++;

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
 *	where y - x is around container.size / 4
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

	std::size_t x = ( rand() % ( c.size() / 2 ) ) + ( rand() % 64 - 32 );;
	std::size_t y = x + ( c.size() / 4 ) + ( rand() % 64 - 32 );

	std::size_t range = y - x;

	for( auto _ : state )
	{
		T res = 0;

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

		benchmark::DoNotOptimize( res );
	}

	state.counters[ "range" ] = range;
	state.counters[  "size" ] = state.range( 0 );
}


} // namespace range_queries

} // namespace npl_benchmark
