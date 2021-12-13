//
//  gtest_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 21/10/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <vector>
#include <gtest/gtest.h>

#include "../include/range_queries.h"

int compare1 ( int const &, int const & );
int compare2 ( int const &, int const & );

std::vector< int >* compare_vec_ptr ( std::vector< int > * const & lhs, std::vector< int > * const & rhs );
