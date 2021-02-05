//
//  test_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <vector>

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
void prefix_push_array ();
void prefix_reserve ();
void test_prefix ();

} // namespace test_range_queries
