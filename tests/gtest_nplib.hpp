//
//
//      natprolib
//      gtest_nplib.hpp
//

#pragma once


#ifndef NPL_DEBUG
#define NPL_DEBUG
#endif

#include <gtest/gtest.h>

#include <vector>

#include <util.hpp>
#include <mem.hpp>
#include <iterator.hpp>
#include <allocator.hpp>

#include <_traits/base_traits.hpp>
#include <_traits/npl_traits.hpp>
#include <_iter/iter_traits.hpp>
#include <_iter/move_iterator.hpp>
#include <_alloc/alloc_traits.hpp>

#include <container/split_buffer>
#include <container/array>
#include <container/vector>
#include <container/static_vector>

#include <range_queries/prefix_vector>
#include <range_queries/prefix_array>
#include <range_queries/fenwick_tree>
#include <range_queries/segment_tree>


#define CUSTOM_CAPACITY 8
#define CUSTOM_VALUE    1


namespace nplib_test
{


struct some_addable_data
{
        float val_;
        unsigned index_;

        some_addable_data (                           ) : val_(   0 ), index_(     0 ) {}
        some_addable_data ( float val                 ) : val_( val ), index_(     0 ) {}
        some_addable_data ( float val, unsigned index ) : val_( val ), index_( index ) {}

        some_addable_data & operator+= ( some_addable_data const & other ) noexcept
        {
                val_   += other.val_;
                index_ += other.index_;

                return *this;
        }
        some_addable_data operator+ ( some_addable_data const & other ) const
        {
                return { val_ + other.val_, index_ + other.index_ };
        }
        some_addable_data & operator-= ( some_addable_data const & other ) noexcept
        {
                val_   -= other.val_;
                index_ -= other.index_;

                return *this;
        }
        some_addable_data operator- ( some_addable_data const & other ) const
        {
                return { val_ - other.val_, index_ - other.index_ };
        }
};

template< typename T, bool C >
class iterator
        : public npl::iterator< C, T >
{
        using _base = npl::iterator< C, T >;
public:
        iterator ( typename _base::pointer _ptr_ ) : _base( _ptr_ ) {}
};

template< typename T, bool C, typename Allocator >
class input_iterator
        : public npl::iterator< C, T >
{
        using _base = npl::iterator< C, T >;
public:
        using iterator_category = npl::input_iterator_tag ;

        input_iterator ( typename _base::pointer _ptr_ ) : _base( _ptr_ ) {}
};


} // namespace nplib_test
