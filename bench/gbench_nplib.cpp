//
//
//      natprolib
//      gbench_nplib.cpp
//

#include "gbench_nplib.hpp"

#define NPL_BENCH_PUSH_BACK
#define NPL_BENCH_PUSH_BACK_RESERVE


namespace npl_bench
{


#ifdef NPL_BENCH_SPLITBUFF
BENCHMARK( bm_sbuff_pback< char > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pback<  int > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pback< long > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );

BENCHMARK( bm_sbuff_pfront< char > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pfront<  int > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_sbuff_pfront< long > )->RangeMultiplier( 2 )->Range( 8, 1024 )->Unit( benchmark::kMicrosecond );
#endif

#ifdef NPL_BENCH_COPY_CONTAINER
BENCHMARK( bm_copy_container< std::vector      < int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_copy_container< npl::prefix_array< int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_copy_container< npl::vector      < int > > )->RangeMultiplier( 2 )->Range( 512, 512 << 8 )->Unit( benchmark::kMicrosecond );
#endif

#ifdef NPL_BENCH_SWAP_INTEGRAL
BENCHMARK( bm_swap_integral_with_temp<       int > )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_swap_integral_with_temp<      long > )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_swap_integral_with_temp< long long > )->Unit( benchmark::kNanosecond );

BENCHMARK( bm_swap_integral<       int > )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_swap_integral<      long > )->Unit( benchmark::kNanosecond );
BENCHMARK( bm_swap_integral< long long > )->Unit( benchmark::kNanosecond );

BENCHMARK( bm_swap_array_with_temp<       int > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
BENCHMARK( bm_swap_array          <       int > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
BENCHMARK( bm_swap_array_with_temp<      long > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
BENCHMARK( bm_swap_array          <      long > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
BENCHMARK( bm_swap_array_with_temp< long long > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
BENCHMARK( bm_swap_array          < long long > )->RangeMultiplier( 2 )->Range( 256, 256 << 2 );
#endif

#ifdef NPL_BENCH_PUSH_BACK
BENCHMARK( bm_push_back< std::vector       < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::vector       < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::prefix_vector< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back< npl::fenwick_tree < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
#endif

#ifdef NPL_BENCH_PUSH_BACK_RESERVE
BENCHMARK( bm_push_back_reserve< std::vector       < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::vector       < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::prefix_vector< int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_push_back_reserve< npl::fenwick_tree < int > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
#endif

#ifdef NPL_BENCH_EMPLACE_BACK
BENCHMARK( bm_emplace_back< std::vector      < addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_emplace_back< npl::prefix_array< addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
BENCHMARK( bm_emplace_back< npl::vector      < addable > > )->RangeMultiplier( 2 )->Range( 256, 256 << 8 )->Unit( benchmark::kMicrosecond );
#endif


} // namespace npl_bench
