//
//  test_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <set>
#include <vector>
#include <iostream>

#include "../include/range_queries.h"


namespace test_range_queries
{

void prefix_default_constructor ();
void prefix_reserve_constructor ();
void prefix_pointer_constructor ();
void prefix_iterator_constructor ();
void prefix_init_list_constructor ();
void prefix_range ();
void prefix_at ();
void prefix_push_back ();
void prefix_emplace_back ();
void prefix_push_array ();
void prefix_reserve ();
void test_prefix ();
  
void segment_default_constructor ();
void segment_pointer_constructor ();
void segment_iterator_constructor ();
void segment_range ();
void segment_range2 ();
void segment_at ();
void segment_push_back ();
void segment_emplace_back ();
void segment_update ();
void segment_parent_builder ();
void test_segment ();

} // namespace test_range_queries
