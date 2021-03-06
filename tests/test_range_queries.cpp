//
//  test_range_queries.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "test_range_queries.hpp"

#define DEFAULT_CAPACITY 8

using namespace natprolib::range_queries;


namespace test_range_queries
{


void prefix_default_constructor ()
{
    prefix_array< int > prefix;
    
    assert( prefix.size() == 0 );
    assert( prefix.capacity() == DEFAULT_CAPACITY );
}
void prefix_reserve_constructor ()
{
    prefix_array< int > prefix( 5 );
    
    assert( prefix.size() == 0 );
    assert( prefix.capacity() == 5 );
}
void prefix_pointer_constructor ()
{
    int array[] = { 1, 1, 1, 1, 1 };
    int * ptr = array;
    
    prefix_array< int > prefix( &ptr, 5 );
    
    assert( prefix.size() == 5 );
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.element_at( 2 ) == 1 );
}
void prefix_iterator_constructor ()
{
    std::vector< int > vec( { 1, 1, 1, 1, 1 } );
    
    prefix_array< int > prefix( vec.begin(), vec.end() );
    
    assert( prefix.size() == 5 );
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.element_at( 2 ) == 1 );
}
void prefix_init_list_constructor ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.size() == 5 );
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.element_at( 2 ) == 1 );
}
void prefix_push_back ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.size() == 5 );
    
    prefix.push_back( 1 );
    
    assert( prefix.size() == 6 );
    assert( prefix.range( 0, 5 ) == 6 );
}
void prefix_push_array ()
{
    prefix_array< int > prefix;
    
    prefix.push_array( 1, 1, 1, 1, 1 );
    
    assert( prefix.size() == 5 );
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.element_at( 2 ) == 1 );
}
void prefix_reserve ()
{
    prefix_array< int > prefix;
    
    assert( prefix.size() == 0 );
    assert( prefix.capacity() == DEFAULT_CAPACITY );
    
    prefix.push_array( 1, 1, 1, 1, 1 );
    
    prefix.reserve( 2 * DEFAULT_CAPACITY );
    
    assert( prefix.capacity() == 2 * DEFAULT_CAPACITY );
    assert( prefix.size() == 5 );
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.element_at( 2 ) == 1 );
}
void test_prefix ()
{
    prefix_default_constructor();
    prefix_reserve_constructor();
    prefix_pointer_constructor();
    prefix_iterator_constructor();
    prefix_init_list_constructor();
    prefix_push_back();
    prefix_push_array();
    prefix_reserve();
}
    
    
void segment_default_constructor ()
{
    segment_tree< int > segtree( 5, 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 5 );
}
void segment_pointer_constructor ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 5 );
}
void segment_iterator_constructor ()
{
    std::vector< int > vec { 1, 3, 2, 7, 4 };
    
    segment_tree< int > segtree( vec.begin(), vec.end(), 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 5 );
}
void segment_range ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.range( 0, 4 ) == 7 );
    assert( segtree.range( 1, 2 ) == 3 );
    assert( segtree.range( 4, 4 ) == 4 );
}
void segment_at ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.at( 1 ) == 3 );
    assert( segtree.at( 2 ) == 2 );
    assert( segtree.at( 4 ) == 4 );
}
void segment_update ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, 0, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.range( 0, 4 ) == 7 );
    
    segtree.update( 2, 8 );
    
    assert( segtree.range( 0, 4 ) == 8 );
}
void segment_parent_builder ()
{
    // example for retrieving the second largest element on a range
    
    struct node
    {
        int max;
        int diff;
        
        bool operator<  ( node const & rhs ) const { return max < rhs.max; }
        bool operator>  ( node const & rhs ) const { return max > rhs.max; }
        bool operator== ( node const & rhs ) const { return max == rhs.max && diff == rhs.diff; }
    };
    
    std::vector< node > array
    {
        { 1, 0 },
        { 3, 0 },
        { 2, 0 },
        { 4, 0 },
        { 2, 0 },
        { 3, 0 },
        { 3, 0 },
        { 2, 0 }
    };
    
    segment_tree< node > segtree( array.begin(), array.end(), { 0, 0 },
        [] ( node const & l_child, node const & r_child ) -> node
        {
            if( l_child.max == r_child.max )
            {
                return { l_child.max, 0 };
            }
            else
            {
                int a, b, c, d;
                a = l_child.max;
                b = a - l_child.diff;
                c = r_child.max;
                d = c - r_child.diff;
                
                std::set< int > set { a, b, c, d };
                
                auto it = set.rbegin();
                int max = *it++;
                int sec = *it;
                
                return { max, max - sec };
            }
        }
    );
    
    node max { 4, 1 };
    assert( segtree.range( 0, 7 ) == max );
}
void test_segment ()
{
    segment_default_constructor();
    segment_pointer_constructor();
    segment_iterator_constructor();
    segment_range();
    segment_at();
    segment_update();
    segment_parent_builder();
}


} // namespace test_range_queries
