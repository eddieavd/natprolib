//
//  test_range_queries.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "test_range_queries.hpp"

#define DEFAULT_CAPACITY 16

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
void prefix_range ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.range( 0, 4 ) == 5 );
    assert( prefix.range( 0, 1 ) == 2 );
    assert( prefix.range( 3, 3 ) == 1 );
    assert( prefix.range( 2, 4 ) == 3 );
}
void prefix_at ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.at( 0 ) == 1 );
    assert( prefix.at( 2 ) == 3 );
    assert( prefix.at( 4 ) == 5 );
}
void prefix_element_at ()
{
    prefix_array< int > prefix( { 1, 2, 3, 4, 5 } );
    
    assert( prefix.element_at( 0 ) == 1 );
    assert( prefix.element_at( 2 ) == 3 );
    assert( prefix.element_at( 4 ) == 5 );
}
void prefix_push_back ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.size() == 5 );
    
    prefix.push_back( 1 );
    
    assert( prefix.size() == 6 );
    assert( prefix.range( 0, 5 ) == 6 );
}
void prefix_emplace_back ()
{
    prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );
    
    assert( prefix.size() == 5 );
    
    prefix.emplace_back( 1 );
    
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
    std::cout << "PASSED prefix_default_constructor\n";
    
    prefix_reserve_constructor();
    std::cout << "PASSED prefix_reserve_constructor\n";
    
    prefix_pointer_constructor();
    std::cout << "PASSED prefix_pointer_constructor\n";
    
    prefix_iterator_constructor();
    std::cout << "PASSED prefix_iterator_constructor\n";
    
    prefix_range();
    std::cout << "PASSED prefix_range\n";
    
    prefix_at();
    std::cout << "PASSED prefix_at\n";
    
    prefix_element_at();
    std::cout << "PASSED prefix_element_at\n";
    
    prefix_init_list_constructor();
    std::cout << "PASSED prefix_init_list_constructor\n";
    
    prefix_push_back();
    std::cout << "PASSED prefix_push_back\n";
    
    prefix_emplace_back();
    std::cout << "PASSED prefix_emplace_back\n";
    
    prefix_push_array();
    std::cout << "PASSED prefix_push_array\n";
    
    prefix_reserve();
    std::cout << "PASSED prefix_reserve\n";
}

void fenwick_default_constructor ()
{
    fenwick_tree< int > ftree;
    
    assert( ftree.size() == 0 );
    assert( ftree.capacity() == DEFAULT_CAPACITY );
}

void fenwick_reserve_constructor ()
{
    fenwick_tree< int > ftree( 5 );
    
    assert( ftree.size() == 0 );
    assert( ftree.capacity() == 5 );
}

void fenwick_pointer_constructor ()
{
    int array[] = { 1, 1, 1, 1, 1 };
    int * ptr = array;
    
    fenwick_tree< int > ftree( &ptr, 5 );
    
    assert( ftree.size() == 5 );
    assert( ftree.range( 0, 4 ) == 5 );
    assert( ftree.at( 2 ) == 1 );
}

void fenwick_iterator_constructor ()
{
    std::vector< int > vec( { 1, 1, 1, 1, 1 } );
    
    fenwick_tree< int > ftree( vec.begin(), vec.end() );
    
    assert( ftree.size() == 5 );
    assert( ftree.range( 0, 4 ) == 5 );
    assert( ftree.at( 2 ) == 1 );
}

void fenwick_init_list_constructor ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.size() == 5 );
    assert( ftree.range( 0, 4 ) == 5 );
    assert( ftree.at( 2 ) == 1 );
}

void fenwick_range ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.range( 0, 4 ) == 5 );
    assert( ftree.range( 0, 1 ) == 2 );
    assert( ftree.range( 2, 2 ) == 1 );
    assert( ftree.range( 2, 4 ) == 3 );
}

void fenwick_at ()
{
    fenwick_tree< int > ftree( { 1, 2, 3, 4, 5 } );
    
    assert( ftree.at( 0 ) == 1 );
    assert( ftree.at( 2 ) == 3 );
    assert( ftree.at( 4 ) == 5 );
}

void fenwick_update ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.range( 0, 4 ) == 5 );
    
    ftree.update( 3, 2 );
    
    assert( ftree.at( 2 ) == 3 );
    assert( ftree.range( 0, 4 ) == 7 );
}

void fenwick_add ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.range( 0, 4 ) == 5 );
    
    ftree.add( 2, 2 );
    
    assert( ftree.at( 2 ) == 3 );
    assert( ftree.range( 0, 4 ) == 7 );
}

void fenwick_push_back ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.size() == 5 );
    assert( ftree.range( 0, 4 ) == 5 );
    
    ftree.push_back( 1 );
    
    assert( ftree.size() == 6 );
    assert( ftree.range( 0, 5 ) == 6 );
}

void fenwick_emplace_back ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.size() == 5 );
    
    ftree.emplace_back( 1 );
    
    assert( ftree.size() == 6 );
    assert( ftree.range( 0, 5 ) == 6 );
}

void fenwick_push_array ()
{
    fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );
    
    assert( ftree.size() == 5 );
    
    ftree.push_array( 1, 1, 1 );
    
    assert( ftree.size() == 8 );
    assert( ftree.range( 0, 7 ) == 8 );
}

void fenwick_reserve ()
{
    fenwick_tree< int > ftree;
    
    assert( ftree.size() == 0 );
    assert( ftree.capacity() == DEFAULT_CAPACITY );
    
    ftree.reserve( DEFAULT_CAPACITY * 2 );
    
    assert( ftree.size() == 0 );
    assert( ftree.capacity() == ( DEFAULT_CAPACITY * 2 ) );
}

void test_fenwick ()
{
    fenwick_default_constructor();
    std::cout << "PASSED fenwick_default_constructor\n";
    
    fenwick_reserve_constructor();
    std::cout << "PASSED fenwick_reserve_constructor\n";
    
    fenwick_pointer_constructor();
    std::cout << "PASSED fenwick_pointer_constructor\n";
    
    fenwick_iterator_constructor();
    std::cout << "PASSED fenwick_iterator_constructor\n";
    
    fenwick_init_list_constructor();
    std::cout << "PASSED fenwick_init_list_constructor\n";
    
    fenwick_range();
    std::cout << "PASSED fenwick_range\n";
    
    fenwick_at();
    std::cout << "PASSED fenwick_at\n";
    
    fenwick_update();
    std::cout << "PASSED fenwick_update\n";
    
    fenwick_add();
    std::cout << "PASSED fenwick_add\n";
    
    fenwick_push_back();
    std::cout << "PASSED fenwick_push_back\n";
    
    fenwick_emplace_back();
    std::cout << "PASSED fenwick_emplace_back\n";
    
    fenwick_push_array();
    std::cout << "PASSED fenwick_push_array\n";
    
    fenwick_reserve();
    std::cout << "PASSED fenwick_reserve\n";
}
    
    
void segment_default_constructor ()
{
    segment_tree< int > segtree( 5, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 0 );
    assert( segtree.capacity() == 5 );
}
void segment_pointer_constructor ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 5 );
    assert( segtree.capacity() == 5 );
}
void segment_iterator_constructor ()
{
    std::vector< int > vec { 1, 3, 2, 7, 4 };
    
    segment_tree< int > segtree( vec.begin(), vec.end(), []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 5 );
    assert( segtree.capacity() == 5 );
}
void segment_range2 ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.range( 0, 4 ) == 7 );
    assert( segtree.range( 0, 1 ) == 3 );
    assert( segtree.range( 1, 3 ) == 7 );
    assert( segtree.range( 4, 4 ) == 4 );
}
void segment_range ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );
    
    assert( segtree.range( 0, 4 ) == 1 );
    assert( segtree.range( 0, 1 ) == 1 );
    assert( segtree.range( 1, 3 ) == 2 );
    assert( segtree.range( 4, 4 ) == 4 );
}
void segment_at ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.at( 1 ) == 3 );
    assert( segtree.at( 2 ) == 2 );
    assert( segtree.at( 4 ) == 4 );
}
void segment_push_back ()
{
    struct some_data
    {
        int x;
        int y;
        std::string hash;
        
        some_data () : x{ 0 }, y{ 0 } { hash = "0"; };
        some_data ( int x_, int y_ ) : x{ x_ }, y{ y_ } { hash = std::to_string( x * y ); }
        
        some_data ( some_data const &  other ) : x{ other.x }, y{ other.y }, hash{ other.hash } {}
        some_data ( some_data       && other ) noexcept : x{ other.x }, y{ other.y }, hash{ std::move( other.hash ) } {}
        
        some_data& operator= ( some_data const & other )
        {
            return *this = some_data( other );
        }
        
        some_data& operator= ( some_data && other ) noexcept
        {
            x = other.x;
            y = other.y;
            std::swap( hash, other.hash );
            
            return *this;
        }
        
        
        bool operator< ( some_data const & rhs ) const { return ( x + y ) <  ( rhs.x + rhs.y ); }
        bool operator==( some_data const & rhs ) const { return ( x + y ) == ( rhs.x + rhs.y ); }
    };
    
    segment_tree< some_data > segtree( 4, []( some_data const & lhs, some_data const & rhs ){ return lhs < rhs ? rhs : lhs; } );
    
    assert( segtree.size() == 0 );
    assert( segtree.capacity() == 4 );
    
    segtree.push_back( { 1, 1 } );
    segtree.push_back( { 2, 2 } );
    segtree.push_back( { 3, 3 } );
    segtree.push_back( { 4, 4 } );
    
    assert( segtree.size() == 4 );
    assert( segtree.capacity() == 4 );
    
    some_data tmp( 4, 4 );
    assert( segtree.range( 0, 3 ) == tmp );
}
void segment_push_back_vector ()
{
    std::vector< int > vec1( { 1, 2, 3 } );
    std::vector< int > vec2( { 4, 5, 6 } );
    std::vector< int > vec3( { 7, 8, 9 } );

    segment_tree< std::vector< int > > segtree( 3, []( std::vector< int > const & lhs, std::vector< int > const & rhs ){ return lhs.at( 0 ) > rhs.at( 0 ) ? lhs : rhs; } );

    assert( segtree.range( 0, 2 ) == vec3 );
}
void segment_emplace_back ()
{
    struct some_data
    {
        int x;
        int y;
        std::string hash;
        
        some_data () : x{ 0 }, y{ 0 } { hash = "0"; };
        some_data ( int x_, int y_ ) : x{ x_ }, y{ y_ } { hash = std::to_string( x * y ); }
        
        bool operator< ( some_data const & rhs ) const { return ( x + y ) <  ( rhs.x + rhs.y ); }
        bool operator==( some_data const & rhs ) const { return ( x + y ) == ( rhs.x + rhs.y ); }
    };
    
    segment_tree< some_data > segtree( 4, []( some_data const & lhs, some_data const & rhs ){ return lhs < rhs ? lhs : rhs; } );
    
    assert( segtree.size() == 0 );
    assert( segtree.capacity() == 4 );
    
    segtree.emplace_back( 1, 1 );
    segtree.emplace_back( 2, 2 );
    segtree.emplace_back( 3, 3 );
    segtree.emplace_back( 4, 4 );
    
    assert( segtree.size() == 4 );
    assert( segtree.capacity() == 4 );
    
    some_data tmp( 1, 1 );
    assert( segtree.range( 0, 3 ) == tmp );
}
void segment_update ()
{
    int array[] = { 1, 3, 2, 7, 4 };
    int * ptr = array;
    
    segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? rhs : lhs; } );
    
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
    
    segment_tree< node > segtree( array.begin(), array.end(),
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
    std::cout << "PASSED segment_default_constructor\n";
    
    segment_pointer_constructor();
    std::cout << "PASSED segment_pointer_constructor\n";
    
    segment_iterator_constructor();
    std::cout << "PASSED segment_iterator_constrcutor\n";
    
    segment_range();
    segment_range2();
    std::cout << "PASSED segment_range\n";
    
    segment_at();
    std::cout << "PASSED segment_at\n";
    
    segment_push_back();
    std::cout << "PASSED segment_push_back\n";
    
    segment_emplace_back();
    std::cout << "PASSED segment_emplace_back\n";
    
    segment_update();
    std::cout << "PASSED segment_update\n";
    
    segment_parent_builder();
    std::cout << "PASSED segment_parent_builder\n";

    // segment_push_back_vector();
    // std::cout << "PASSED segment_push_back_vector\n";
}


} // namespace test_range_queries
