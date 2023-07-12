//
//
//      natprolib
//      gbench_new_vec.cpp
//

#include "gbench_new_vec.hpp"


namespace npl_bench
{


BENCHMARK( bm_vector_push_back< int > );
BENCHMARK( bm_new_vector_push_back< int > );


} // namespace npl_bench
