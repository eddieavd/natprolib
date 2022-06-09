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

	constexpr auto prefix_3 = make_prefix< int >( 1 );

	static_assert( prefix_3.size() == 16 );
	static_assert( prefix_3.capacity() == 16 );

	constexpr auto prefix_4 = make_prefix< int, 5 >( 1 );

	static_assert( prefix_4.size() == 5 );
	static_assert( prefix_4.capacity() == 5 );
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

void prefix_2d_make_prefix_2d ()
{
	constexpr auto prefix2d_1 = make_prefix_2d< int, 2, 2 >( { 1, 1, 1, 1 } );

	static_assert( prefix2d_1.size_x() == 2 );
	static_assert( prefix2d_1.size_y() == 0 );
	static_assert( prefix2d_1.capacity_x() == 2 );
	static_assert( prefix2d_1.capacity_y() == 2 );

	constexpr auto prefix2d_2 = make_prefix_2d< int >( 1 );

	static_assert( prefix2d_2.size_x() == DEFAULT_CAPACITY );
	static_assert( prefix2d_2.size_y() == 0 );
	static_assert( prefix2d_2.capacity_x() == DEFAULT_CAPACITY );
	static_assert( prefix2d_2.capacity_y() == DEFAULT_CAPACITY );

	constexpr auto prefix2d_3 = make_prefix_2d< int, 2, 2 >( 1 );

	static_assert( prefix2d_3.size_x() == 2 );
	static_assert( prefix2d_3.size_y() == 0 );
	static_assert( prefix2d_3.capacity_x() == 2 );
	static_assert( prefix2d_3.capacity_y() == 2 );
}
void prefix_2d_range ()
{
	constexpr auto prefix = make_prefix_2d< int, 4, 4 >( 1 );

	static_assert( prefix.range( 0, 0, 3, 3 ) == 16 );
	static_assert( prefix.range( 1, 1, 2, 2 ) ==  4 );
	static_assert( prefix.range( 0, 1, 1, 3 ) ==  6 );
	static_assert( prefix.range( 1, 0, 3, 1 ) ==  6 );
}
void prefix_2d_at ()
{
	constexpr auto prefix = make_prefix_2d< int, 4, 4 >( 1 );

	static_assert( prefix.at( 0, 0 ) ==  1 );
	static_assert( prefix.at( 1, 1 ) ==  4 );
	static_assert( prefix.at( 2, 2 ) ==  9 );
	static_assert( prefix.at( 3, 3 ) == 16 );
	static_assert( prefix.at( 1, 2 ) ==  6 );
}
void prefix_2d_element_at ()
{
	constexpr auto prefix = make_prefix_2d< int, 4, 4 >( 1 );

	static_assert( prefix.element_at( 0, 0 ) == 1 );
	static_assert( prefix.element_at( 1, 1 ) == 1 );
	static_assert( prefix.element_at( 2, 2 ) == 1 );
	static_assert( prefix.element_at( 3, 3 ) == 1 );
	static_assert( prefix.element_at( 0, 1 ) == 1 );
	static_assert( prefix.element_at( 2, 0 ) == 1 );
}
constexpr int sum2d ()
{
	int sum = 0;
	constexpr auto prefix = make_prefix_2d< int, 2, 2 >( 1 );

	for( int x : prefix )
	{
		sum += x;
	}

	return sum;
}
void prefix_2d_iterator ()
{
//	constexpr auto sum = sum2d();

//	static_assert( sum == 9 );
}

void ftree_make_ftree ()
{
	constexpr auto ftree_1 = make_ftree( { 1, 1, 1, 1, 1 } );

	static_assert( ftree_1.size() == 5 );
	static_assert( ftree_1.capacity() == DEFAULT_CAPACITY );

	constexpr auto ftree_2 = make_ftree< int, 5 >( { 1, 1, 1, 1, 1 } );

	static_assert( ftree_2.size() == 5 );
	static_assert( ftree_2.capacity() == 5 );

	constexpr auto ftree_3 = make_ftree( 1 );

	static_assert( ftree_3.size() == 16 );
	static_assert( ftree_3.capacity() == 16 );

	constexpr auto ftree_4 = make_ftree< int, 5 >( 1 );

	static_assert( ftree_4.size() == 5 );
	static_assert( ftree_4.capacity() == 5 );
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
