//
//
//      natprolib
//      gbench_nplib.cpp
//

#include "gbench_nplib.hpp"


namespace npl_bench
{


#ifdef SPLIT_BUFF_BENCH
BENCHMARK( bm_sbuff_pback< char > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pback<  int > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pback< long > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );

BENCHMARK( bm_sbuff_pfront< char > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pfront<  int > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pfront< long > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
#endif

/*
BENCHMARK( bm_copy_container< std::vector      < int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_copy_container< npl::prefix_array< int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_copy_container< npl::vector      < int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
*/

BENCHMARK( bm_push_back< std::vector      < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::vector      < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::prefix_array< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::fenwick_tree< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );

/*
BENCHMARK( bm_push_back_reserve< std::vector      < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::vector      < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::prefix_array< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::fenwick_tree< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
*/

/*
BENCHMARK( bm_emplace_back< std::vector      < addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_emplace_back< npl::prefix_array< addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_emplace_back< npl::vector      < addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
*/


} // namespace npl_bench
