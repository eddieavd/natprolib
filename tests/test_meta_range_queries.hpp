//
//  test_meta_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 05/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include "../include/meta_range_queries.h"

#define DEFAULT_CAPACITY 16


namespace test_meta_range_queries
{

void prefix_make_prefix ();
void prefix_range ();
void prefix_at ();
void prefix_element_at ();
void prefix_iterator ();

void prefix_2d_make_prefix_2d ();
void prefix_2d_range ();
void prefix_2d_at ();
void prefix_2d_element_at ();
void prefix_2d_iterator ();

void ftree_make_ftree ();
void ftree_range ();
void ftree_at ();

} // namespace test_meta_range_queries
