//
//  gbench_range_queries.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 02/05/2022
//  Distributed under the terms of the GNU General Public License
//

#include "gbench_range_queries.hpp"


namespace npl_benchmark
{

namespace range_queries
{


static void vector_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		std::vector< int > vec;
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			state.PauseTiming();
			num = rand();
			state.ResumeTiming();

			vec.push_back( num );

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( vec );
	}
}

static void set_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		std::set< int > set;
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			state.PauseTiming();
			num = rand();
			state.ResumeTiming();

			set.insert( num );

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( set );
	}
}

static void prefix_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		natprolib::range_queries::prefix_array< int > prefix;
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			state.PauseTiming();
			num = rand();
			state.ResumeTiming();

			prefix.push_back( num );

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( prefix );
	}
}

static void fenwick_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		natprolib::range_queries::fenwick_tree< int > ftree;
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			state.PauseTiming();
			num = rand();
			state.ResumeTiming();

			ftree.push_back( num );

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( ftree );
	}
}

static void segment_push_back ( benchmark::State & state )
{
	srand( time( nullptr ) );

	for( auto _ : state )
	{
		state.PauseTiming();
		int num;
		natprolib::range_queries::segment_tree< int, decltype( pb ) > segtree( pb );
		state.ResumeTiming();

		for( auto i = 0; i < state.range( 0 ); i++ )
		{
			state.PauseTiming();
			num = rand();
			state.ResumeTiming();

			segtree.push_back( num );

			benchmark::ClobberMemory();
		}

		benchmark::DoNotOptimize( segtree );
	}
}

static void vector_range_sum ( benchmark::State & state )
{
	srand( time( nullptr ) );

	std::vector< int > vec;

	for( auto i = 0; i < state.range( 0 ); i++ )
	{
		vec.push_back( rand() );
	}

	double      avg_range  = 0;
	std::size_t iterations = 0;

	for( auto _ : state )
	{
		state.PauseTiming();

		std::size_t x;
		std::size_t y;

		x = rand() % vec.size();
		y = rand() % vec.size();

		if( x > y )
		{
			std::swap( x, y );
		}

		iterations++;
		avg_range += y - x;

		state.ResumeTiming();

		int res = 0;

		for( std::size_t i = 0; i < y - x; i++ )
		{
			res += vec[ i ];
		}

		benchmark::DoNotOptimize( res );
	}

	state.counters[      "size" ] = state.range( 0 );
	state.counters[ "avg_range" ] = avg_range / iterations;
}

static void prefix_range_sum ( benchmark::State & state )
{
	srand( time( nullptr ) );

	natprolib::range_queries::prefix_array< int > prefix;

	for( auto i = 0; i < state.range( 0 ); i++ )
	{
		prefix.push_back( rand() );
	}

	double      avg_range  = 0;
	std::size_t iterations = 0;

	for( auto _ : state )
	{
		state.PauseTiming();

		std::size_t x;
		std::size_t y;

		x = rand() % prefix.size();
		y = rand() % prefix.size();

		if( x > y )
		{
			std::swap( x, y );
		}

		iterations++;
		avg_range += y - x;

		state.ResumeTiming();

		int res = prefix.range( x, y );

		benchmark::DoNotOptimize( res );
	}

	state.counters[      "size" ] = state.range( 0 );
	state.counters[ "avg_range" ] = avg_range / iterations;
}

static void fenwick_range_sum ( benchmark::State & state )
{
	srand( time( nullptr ) );

	natprolib::range_queries::fenwick_tree< int > ftree;

	for( auto i = 0; i < state.range( 0 ); i++ )
	{
		ftree.push_back( rand() );
	}

	double      avg_range  = 0;
	std::size_t iterations = 0;

	for( auto _ : state )
	{
		state.PauseTiming();

		std::size_t x;
		std::size_t y;

		x = rand() % ftree.size();
		y = rand() % ftree.size();

		if( x > y )
		{
			std::swap( x, y );
		}

		iterations++;
		avg_range += y - x;

		state.ResumeTiming();

		int res = ftree.range( x, y );

		benchmark::DoNotOptimize( res );
	}

	state.counters[      "size" ] = state.range( 0 );
	state.counters[ "avg_range" ] = avg_range / iterations;
}

static void segment_range_sum ( benchmark::State & state )
{
	srand( time( nullptr ) );

	natprolib::range_queries::segment_tree< int, decltype( pb ) > segtree( pb );

	for( auto i = 0; i < state.range( 0 ); i++ )
	{
		segtree.push_back( rand() );
	}

	double      avg_range  = 0;
	std::size_t iterations = 0;

	for( auto _ : state )
	{
		state.PauseTiming();

		std::size_t x;
		std::size_t y;

		x = rand() % segtree.size();
		y = rand() % segtree.size();

		if( x > y )
		{
			std::swap( x, y );
		}

		iterations++;
		avg_range += y - x;

		state.ResumeTiming();

		int res = segtree.range( x, y );

		benchmark::DoNotOptimize( res );
	}

	state.counters[      "size" ] = state.range( 0 );
	state.counters[ "avg_range" ] = avg_range / iterations;
}


//                                                         (  num push_backs )
BENCHMARK( vector_push_back  )->RangeMultiplier( 4 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMillisecond );
BENCHMARK( set_push_back     )->RangeMultiplier( 4 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMillisecond );
BENCHMARK( prefix_push_back  )->RangeMultiplier( 4 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMillisecond );
BENCHMARK( fenwick_push_back )->RangeMultiplier( 4 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMillisecond );
BENCHMARK( segment_push_back )->RangeMultiplier( 4 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMillisecond );

//                                                         ( container size )
BENCHMARK( vector_range_sum  )->RangeMultiplier( 8 )->Range( 128, 128 << 16 );
BENCHMARK( prefix_range_sum  )->RangeMultiplier( 8 )->Range( 128, 128 << 16 );
BENCHMARK( fenwick_range_sum )->RangeMultiplier( 8 )->Range( 128, 128 << 16 );
BENCHMARK( segment_range_sum )->RangeMultiplier( 8 )->Range( 128, 128 << 16 );


} // namespace range_queries

} // namespace npl_benchmark
