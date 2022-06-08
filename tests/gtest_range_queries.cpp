//
//  gtest_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 21/10/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "gtest_range_queries.hpp"

#define DEFAULT_CAPACITY 16
#define CUSTOM_CAPACITY 8

using namespace natprolib::range_queries;


TEST( PrefixTest, PrefixDefaultConstruct )
{
	prefix_array< int > prefix;

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), DEFAULT_CAPACITY);
}
TEST( PrefixTest, PrefixReserveConstruct )
{
	prefix_array< int > prefix( CUSTOM_CAPACITY );

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), CUSTOM_CAPACITY );
}
TEST( PrefixTest, PrefixPointerConstruct )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	prefix_array< int > prefix( &ptr, 5 );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.capacity(), 5 );
}
TEST( PrefixTest, PrefixFillConstruct )
{
	prefix_array< int > prefix( CUSTOM_CAPACITY, 1 );

	EXPECT_EQ( prefix.size(), CUSTOM_CAPACITY );
	EXPECT_EQ( prefix.capacity(), CUSTOM_CAPACITY );
}
TEST( PrefixTest, PrefixIteratorConstruct )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	prefix_array< int > prefix( vec.begin(), vec.end() );

	EXPECT_EQ( prefix.size(), 5 );
}
TEST( PrefixTest, PrefixInitListConstruct )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
}
TEST( PrefixTest, PrefixIterator )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	int sum = 0;

	for( auto val : prefix )
	{
		sum += val;
	}

	EXPECT_EQ( sum, 15 );
}
TEST( PrefixTest, PrefixRange )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.range( 0, 4 ), 5 );
	EXPECT_EQ( prefix.range( 0, 1 ), 2 );
	EXPECT_EQ( prefix.range( 3, 3 ), 1 );
	EXPECT_EQ( prefix.range( 2, 4 ), 3 );
}
TEST( PrefixTest, PrefixAt )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.at( 0 ), 1 );
	EXPECT_EQ( prefix.at( 2 ), 3 );
	EXPECT_EQ( prefix.at( 4 ), 5 );
}
TEST( PrefixTest, PrefixElementAt )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.element_at( 0 ), 1 );
	EXPECT_EQ( prefix.element_at( 2 ), 1 );
	EXPECT_EQ( prefix.element_at( 4 ), 1 );
}
TEST( PrefixTest, PrefixPushBack )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixTest, PrefixEmplaceBack )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );

	prefix.emplace_back( 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixTest, PrefixPushArray )
{
	prefix_array< int > prefix( { 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 3 );
	EXPECT_EQ( prefix.range( 0, 2 ), 3 );

	prefix.push_back( 1, 1, 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixTest, PrefixReserve )
{
	prefix_array< int > prefix;

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), DEFAULT_CAPACITY );

	prefix.push_back( 1, 1, 1, 1, 1 );

	prefix.reserve( 2 * DEFAULT_CAPACITY );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );
	EXPECT_EQ( prefix.capacity(), 2 * DEFAULT_CAPACITY );
}
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
TEST( Prefix2dTest, Prefix2dDefaultConstruct )
{
	prefix_2d< int > prefix;

	EXPECT_EQ( prefix.size_x(), 0 );
	EXPECT_EQ( prefix.size_y(), 0 );
	EXPECT_EQ( prefix.capacity_x(), DEFAULT_CAPACITY );
	EXPECT_EQ( prefix.capacity_y(), DEFAULT_CAPACITY );
}
TEST( Prefix2dTest, Prefix2dReserveConstruct )
{
	prefix_2d< int > prefix( CUSTOM_CAPACITY, CUSTOM_CAPACITY );

	EXPECT_EQ( prefix.size_x(), 0 );
	EXPECT_EQ( prefix.size_y(), 0 );
	EXPECT_EQ( prefix.capacity_x(), CUSTOM_CAPACITY );
	EXPECT_EQ( prefix.capacity_y(), CUSTOM_CAPACITY );
}
TEST( Prefix2dTest, Prefix2dFillDefaultConstruct )
{
	prefix_2d prefix( 1 );

	EXPECT_EQ( prefix.size_x(), DEFAULT_CAPACITY );
	EXPECT_EQ( prefix.size_y(), 0 );
	EXPECT_EQ( prefix.capacity_x(), DEFAULT_CAPACITY );
	EXPECT_EQ( prefix.capacity_y(), DEFAULT_CAPACITY );
}
TEST( Prefix2dTest, Prefix2dFillCustomConstruct )
{
	prefix_2d prefix( CUSTOM_CAPACITY, CUSTOM_CAPACITY, 1 );

	EXPECT_EQ( prefix.size_x(), CUSTOM_CAPACITY );
	EXPECT_EQ( prefix.size_y(), 0 );
	EXPECT_EQ( prefix.capacity_x(), CUSTOM_CAPACITY );
	EXPECT_EQ( prefix.capacity_y(), CUSTOM_CAPACITY );
}
TEST( Prefix2dTest, Prefix2dInitListConstruct )
{
	prefix_2d prefix( 2, 2, { 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size_x(), 2 );
	EXPECT_EQ( prefix.size_y(), 0 );
	EXPECT_EQ( prefix.capacity_x(), 2 );
	EXPECT_EQ( prefix.capacity_y(), 2 );
}
TEST( Prefix2dTest, Prefix2dIterator )
{
	prefix_2d prefix( 4, 4, 1 );

	int sum = 0;

	for( auto val : prefix )
	{
		sum += val;
	}

	EXPECT_EQ( sum, 100 );
}
TEST( Prefix2dTest, Prefix2dRange )
{
	prefix_2d prefix( 4, 4, 1 );

	EXPECT_EQ( prefix.range( 0, 0, 3, 3 ), 16 );
	EXPECT_EQ( prefix.range( 0, 0, 1, 2 ),  6 );
	EXPECT_EQ( prefix.range( 0, 2, 2, 3 ),  6 );
	EXPECT_EQ( prefix.range( 1, 1, 3, 3 ),  9 );
	EXPECT_EQ( prefix.range( 1, 2, 3, 3 ),  6 );
}
TEST( Prefix2dTest, Prefix2dAt )
{
	prefix_2d prefix( 4, 4, 1 );

	EXPECT_EQ( prefix.at( 0, 0 ),  1 );
	EXPECT_EQ( prefix.at( 1, 1 ),  4 );
	EXPECT_EQ( prefix.at( 2, 2 ),  9 );
	EXPECT_EQ( prefix.at( 3, 3 ), 16 );
	EXPECT_EQ( prefix.at( 2, 3 ), 12 );
	EXPECT_EQ( prefix.at( 3, 1 ),  8 );
}
TEST( Prefix2dTest, Prefix2dElementAt )
{
	prefix_2d prefix( 4, 4, 1 );

	EXPECT_EQ( prefix.element_at( 0, 0 ), 1 );
	EXPECT_EQ( prefix.element_at( 1, 1 ), 1 );
	EXPECT_EQ( prefix.element_at( 2, 2 ), 1 );
	EXPECT_EQ( prefix.element_at( 3, 3 ), 1 );
	EXPECT_EQ( prefix.element_at( 0, 2 ), 1 );
	EXPECT_EQ( prefix.element_at( 3, 1 ), 1 );
	EXPECT_EQ( prefix.element_at( 2, 0 ), 1 );
}
TEST( Prefix2dTest, Prefix2dPushBack )
{
	prefix_2d< int > prefix( 2, 2 );

	EXPECT_EQ( prefix.size_x(), 0 );
	EXPECT_EQ( prefix.size_y(), 0 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size_x(), 0 );
	EXPECT_EQ( prefix.size_y(), 1 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size_x(), 1 );
	EXPECT_EQ( prefix.size_y(), 0 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size_x(), 1 );
	EXPECT_EQ( prefix.size_y(), 1 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size_x(), 2 );
	EXPECT_EQ( prefix.size_y(), 0 );
}
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
TEST( FenwickTest, FenwickDefaultConstruct )
{
	fenwick_tree< int > ftree;

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), DEFAULT_CAPACITY );
}
TEST( FenwickTest, FenwickReserveConstruct )
{
	fenwick_tree< int > ftree( CUSTOM_CAPACITY );

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), CUSTOM_CAPACITY );
}
TEST( FenwickTest, FenwickPointerConstruct )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	fenwick_tree< int > ftree( &ptr, 5 );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickTest, FenwickFillConstruct )
{
	fenwick_tree< int > ftree( CUSTOM_CAPACITY, 1 );

	EXPECT_EQ( ftree.size(), CUSTOM_CAPACITY );
	EXPECT_EQ( ftree.capacity(), CUSTOM_CAPACITY );
}
TEST( FenwickTest, FenwickIteratorConstruct )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	fenwick_tree< int > ftree( vec.begin(), vec.end() );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickTest, FenwickInitListConstruct )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickTest, FenwickRange )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.range( 0, 4 ), 5 );
	EXPECT_EQ( ftree.range( 0, 1 ), 2 );
	EXPECT_EQ( ftree.range( 3, 3 ), 1 );
	EXPECT_EQ( ftree.range( 2, 4 ), 3 );
}
TEST( FenwickTest, FenwickAt )
{
	fenwick_tree< int > ftree( { 1, 2, 3, 4, 5 } );

	EXPECT_EQ( ftree.at( 0 ), 1 );
	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.at( 4 ), 5 );
}
TEST( FenwickTest, FenwickUpdate )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.update( 3, 2 );

	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.range( 0, 4 ), 7 );
}
TEST( FenwickTest, FenwickAdd )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.add( 2, 2 );

	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.range( 0, 4 ), 7 );
}
TEST( FenwickTest, FenwickPushBack )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.push_back( 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickTest, FenwickEmplaceBack )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.emplace_back( 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickTest, FenwickPushArray )
{
	fenwick_tree< int > ftree( { 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 3 );
	EXPECT_EQ( ftree.range( 0, 2 ), 3 );

	ftree.push_back( 1, 1, 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickTest, FenwickReserve )
{
	fenwick_tree< int > ftree;

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), DEFAULT_CAPACITY );

	ftree.reserve( 2 * DEFAULT_CAPACITY );

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), 2 * DEFAULT_CAPACITY );
}
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
TEST( SegmentTest, SegmentParentBuilderConstruct )
{
	segment_tree< int, decltype( pb1 ) > segtree( pb1 );

	EXPECT_EQ( segtree.size(), 0 );
	EXPECT_EQ( segtree.capacity(), DEFAULT_CAPACITY );
}
TEST( SegmentTest, SegmentReserveConstruct )
{
	segment_tree< int, decltype( pb1 ) > segtree( CUSTOM_CAPACITY, pb1 );

	EXPECT_EQ( segtree.size(), 0 );
	EXPECT_EQ( segtree.capacity(), CUSTOM_CAPACITY );
}
TEST( SegmentTest, SegmentPointerConstruct )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	segment_tree< int, decltype( pb1 ) > segtree( &ptr, 5, pb1 );

	EXPECT_EQ( segtree.size(), 5 );
	EXPECT_EQ( segtree.capacity(), 8 );  //  original array gets extended to closest power of two
}
TEST( SegmentTest, SegmentFillConstruct )
{
	segment_tree< int, decltype( pb1 ) > segtree( CUSTOM_CAPACITY, 1, pb1 );

	EXPECT_EQ( segtree.size(), CUSTOM_CAPACITY );
}
TEST( SegmentTest, SegmentIteratorConstruct )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	segment_tree< int, decltype( pb1 ) > segtree( vec.begin(), vec.end(), pb1 );

	EXPECT_EQ( segtree.size(), 5 );
}
TEST( SegmentTest, SegmentInitListConstruct )
{
	segment_tree< int, decltype( pb1 ) > segtree( { 1, 1, 1, 1, 1 }, pb1 );

	EXPECT_EQ( segtree.size(), 5 );
}
TEST( SegmentTest, SegmentIterator )
{
	segment_tree< int, decltype( pb1 ) > segtree( { 1, 1, 1, 1, 1 }, pb1 );

	int sum = 0;

	for( auto val : segtree )
	{
		sum += val;
	}

	EXPECT_EQ( sum, 5 );
}
TEST( SegmentTest, SegmentRange )
{
	segment_tree< int, decltype( pb1 ) > segtree( { 1, 2, 3, 4, 5 }, pb1 );

	EXPECT_EQ( segtree.range( 0, 4 ), 1 );
}
TEST( SegmentTest, SegmentPushBack )
{
	segment_tree< int, decltype( pb2 ) > segtree( { 1, 2, 3, 4, 5 }, pb2 );

	EXPECT_EQ( segtree.size(), 5 );
	EXPECT_EQ( segtree.range( 0, 4 ), 5 );

	segtree.push_back( 6 );

	EXPECT_EQ( segtree.size(), 6 );
	EXPECT_EQ( segtree.range( 0, 5 ), 6 );
}
TEST( SegmentTest, SegmentEmplaceBack )
{
	struct some_data
	{
		int x;
		int y;

		some_data (                ) : x{ 0  }, y{ 0  } {}
		some_data ( int x_, int y_ ) : x{ x_ }, y{ y_ } {}

		some_data ( some_data const & other )
		{
			x = other.x;
			y = other.y;
		}
		some_data ( some_data && other )
		{
			x = other.x;
			y = other.y;
		}

		some_data & operator= ( some_data const & other )
		{
			x = other.x;
			y = other.y;

			return *this;
		}
		some_data & operator= ( some_data && other ) noexcept
		{
			x = other.x;
			y = other.y;

			return *this;
		}

		bool operator< ( some_data const & other ) const { return x <  other.x; }
		bool operator> ( some_data const & other ) const { return x >  other.x; }
		bool operator==( some_data const & other ) const { return x == other.x; }
		bool operator==( int other ) const { return x == other; }
	};

	auto pb
	{
		[]( some_data const & lhs, some_data const & rhs )
		{
			return lhs > rhs ? lhs : rhs;
		}
	};

	segment_tree< some_data, decltype( pb ) > segtree( 5, pb );

	segtree.emplace_back( 1, 1 );
	segtree.emplace_back( 2, 2 );
	segtree.emplace_back( 3, 3 );

	EXPECT_EQ( segtree.size(), 3 );
	EXPECT_EQ( segtree.range( 0, 2 ), 3 );
}
TEST( SegmentTest, SegmentEmplaceVector )
{
	std::vector< int > vec1( {  1,  2,  3 } );
 	std::vector< int > vec2( {  4,  5,  6 } );
 	std::vector< int > vec3( {  7,  8,  9 } );
	std::vector< int > vec4( { 10, 11, 12 } );

	std::vector< std::vector< int > * > vecptr( { &vec1, &vec2, &vec3, &vec4 } );

 	segment_tree< std::vector< int > *, decltype( pb_vec ) > segtree( vecptr.begin(), vecptr.end(), pb_vec );

 	EXPECT_EQ( segtree.range( 0, 3 ), &vec4 );
}
TEST( SegmentTest, SegmentUpdate )
{
	segment_tree< int, decltype( pb1 ) > segtree( { 1, 2, 3, 4, 5 }, pb1 );

	EXPECT_EQ( segtree.range( 0, 4 ), 1 );

	segtree.update( 0, 2 );

	EXPECT_EQ( segtree.range( 0, 4 ), 0 );
}
TEST( SegmentTest, SegmentParentBuilder )
{
	struct node
	{
		int max;
		int diff;

		bool operator< ( node const & rhs ) const { return max < rhs.max; }
		bool operator> ( node const & rhs ) const { return max > rhs.max; }
		bool operator==( node const & rhs ) const { return max == rhs.max && diff == rhs.diff; }
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

	auto compare
	{
		[]( node const & l_child, node const & r_child )
		{
			if( l_child.max == r_child.max )
			{
				return node{ l_child.max, 0 };
			}
			else
			{
				int a, b, c, d;
				a = l_child.max;
				b = a - l_child.diff;
				c = r_child.max;
				d = c - r_child.diff;

				std::set< int > set{ a, b, c, d };

				auto it = set.rbegin();
				int max = *it++;
				int sec = *it;

				return node{ max, max - sec };
			}
		}
	};

	segment_tree< node, decltype( compare ) > segtree( array.begin(), array.end(), compare );

	node max { 4, 1 };  //  max.max - max.diff == second largest element

	EXPECT_EQ( segtree.range( 0, 7 ), max );

	int second = 3;
	node stree_max = segtree.range( 0, 7 );

	EXPECT_EQ( second, stree_max.max - stree_max.diff );
}
