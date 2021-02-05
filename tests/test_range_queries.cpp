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


} // namespace test_range_queries
