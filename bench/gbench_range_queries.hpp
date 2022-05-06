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


auto pb
{
	[]( int const & lhs, int const & rhs )
	{
		return lhs + rhs;
	}
};

static void vector_push_back  ( benchmark::State & state );
static void stdset_insert     ( benchmark::State & state );
static void prefix_push_back  ( benchmark::State & state );
static void fenwick_push_back ( benchmark::State & state );
static void segment_push_back ( benchmark::State & state );

static void vector_range_sum  ( benchmark::State & state );
static void prefix_range_sum  ( benchmark::State & state );
static void fenwick_range_sum ( benchmark::State & state );
static void segment_range_sum ( benchmark::State & state );


} // namespace range_queries

} // namespace npl_benchmark
