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

namespace rq
{


using namespace npl::rq;


BENCHMARK( bm_push_back< long long, std::vector < long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< long long, std::set    < long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< long long, prefix_array< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< long long, fenwick_tree< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< long long, segment_tree< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 8 )->Unit( benchmark::kMicrosecond );

BENCHMARK( bm_range_sum< long long, std::vector < long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 16 )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_range_sum< long long, prefix_array< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 16 )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_range_sum< long long, fenwick_tree< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 16 )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_range_sum< long long, segment_tree< long long > > )->RangeMultiplier( 8 )->Range( 1024, 1024 << 16 )->Unit( benchmark::kNanosecond );


} // namespace range_queries

} // namespace npl_benchmark
