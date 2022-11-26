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

#include <util/util.hpp>
#include <util/traits.hpp>
#include <util/mem.hpp>
#include <util/split_buffer.hpp>

#include <container/vector>

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

template< typename T, bool C, typename Allocator >
class iterator
        : public npl::iterator< T, C, Allocator >
{
        using _base = npl::iterator< T, C, Allocator >;
public:
        iterator ( typename _base::pointer _ptr_ ) : _base( _ptr_ ) {}
};

template< typename T, bool C, typename Allocator >
class input_iterator
        : public npl::iterator< T, C, Allocator >
{
        using _base = npl::iterator< T, C, Allocator >;
public:
        using iterator_category = std::input_iterator_tag;

        input_iterator ( typename _base::pointer _ptr_ ) : _base( _ptr_ ) {}
};


} // namespace nplib_test
