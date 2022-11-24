//
//
//      natprolib
//      gtest_segtree.hpp
//

#pragma once

#include "gtest_nplib.hpp"


template< typename T >
auto pb_max
{
        []( T const & lhs, T const & rhs )
        {
                return lhs > rhs ? lhs : rhs;
        }
};

template< typename T >
auto pb_min
{
        []( T const & lhs, T const & rhs )
        {
                return lhs < rhs ? lhs : rhs;
        }
};

template< typename T >
auto pb_sum
{
        []( T const & lhs, T const & rhs )
        {
                return lhs + rhs;
        }
};
