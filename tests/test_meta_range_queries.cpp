//
//  test_meta_range_queries.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 05/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "test_meta_range_queries.hpp"

using namespace natprolib::meta_range_queries;


namespace test_meta_range_queries
{


void prefix_make_prefix ()
{
	constexpr auto prefix_1 = make_prefix( { 1, 1, 1, 1, 1 } );

	static_assert( prefix_1.size() == 5 );
	static_assert( prefix_1.capacity() == 16 );

	constexpr auto prefix_2 = make_prefix< int, 5 >( { 1, 1, 1, 1, 1 } );

	static_assert( prefix_2.size() == 5 );
	static_assert( prefix_2.capacity() == 5 );
}
void prefix_range ()
{
	constexpr auto prefix = make_prefix( { 1, 1, 1, 1, 1 } );

	static_assert( prefix.range( 0, 4 ) == 5 );
	static_assert( prefix.range( 1, 3 ) == 3 );
	static_assert( prefix.range( 0, 0 ) == 1 );
}
void prefix_at ()
{
	constexpr auto prefix = make_prefix( { 1, 1, 1 } );

	static_assert( prefix.at( 0 ) == 1 );
	static_assert( prefix.at( 1 ) == 2 );
	static_assert( prefix.at( 2 ) == 3 );
}
void prefix_element_at ()
{
	constexpr auto prefix = make_prefix( { 1, 1, 1 } );

	static_assert( prefix.element_at( 0 ) == 1 );
	static_assert( prefix.element_at( 1 ) == 1 );
	static_assert( prefix.element_at( 2 ) == 1 );
}
constexpr int sumsum ()
{
	int sum = 0;
	constexpr auto prefix = make_prefix< int, 3 >( { 1, 1, 1 } );

	for( int x : prefix )
	{
		sum += x;
	}
	return sum;
}
void prefix_iterator ()
{
	constexpr auto sum = sumsum();
	static_assert( sum == 6 );
}

void ftree_make_ftree ()
{
	constexpr auto ftree_1 = make_ftree( { 1, 1, 1, 1, 1 } );

	static_assert( ftree_1.size() == 5 );
	static_assert( ftree_1.capacity() == DEFAULT_CAPACITY );

	constexpr auto ftree_2 = make_ftree< int, 5 >( { 1, 1, 1, 1, 1 } );

	static_assert( ftree_2.size() == 5 );
	static_assert( ftree_2.capacity() == 5 );
}
void ftree_range ()
{
	constexpr auto ftree = make_ftree( { 1, 1, 1, 1, 1 } );

	static_assert( ftree.range( 0, 4 ) == 5 );
	static_assert( ftree.range( 1, 3 ) == 3 );
	static_assert( ftree.range( 0, 0 ) == 1 );
}
void ftree_at ()
{
	constexpr auto ftree = make_ftree( { 1, 1, 1 } );

	static_assert( ftree.at( 0 ) == 1 );
	static_assert( ftree.at( 1 ) == 1 );
	static_assert( ftree.at( 2 ) == 1 );
}


} // namespace test_meta_range_queries
