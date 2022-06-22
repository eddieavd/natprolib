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

#include "../include/range_queries/range_queries.h"


auto pb1
{
	[]( int const & lhs, int const & rhs )
	{
		return lhs < rhs ? lhs : rhs;
	}
};

auto pb2
{
	[]( int const & lhs, int const & rhs )
	{
		return lhs > rhs ? lhs : rhs;
	}
};

auto pb_vec
{
	[]( std::vector< int > * lhs, std::vector< int > * rhs )
	{
		return ( *lhs )[ 0 ] > ( *rhs )[ 0 ] ? lhs : rhs;
	}
};
