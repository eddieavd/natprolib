//
//  gtest_range_queries.hpp
//  natprolib
//
//  Created by Edhem Avdagic on 21/10/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "gtest_range_queries.hpp"

#define DEFAULT_CAPACITY 16
#define CUSTOM_CAPACITY 5

using namespace natprolib::range_queries;


TEST( PrefixDefaultConstruct, BasicAssertions )
{
	prefix_array< int > prefix;

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), DEFAULT_CAPACITY);
}
TEST( PrefixReserveConstruct, BasicAssertions )
{
	prefix_array< int > prefix( CUSTOM_CAPACITY );

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), CUSTOM_CAPACITY );
}
TEST( PrefixPointerConstruct, BasicAssertions )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	prefix_array< int > prefix( &ptr, 5 );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.capacity(), 5 );
}
TEST( PrefixIteratorConstruct, BasicAssertions )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	prefix_array< int > prefix( vec.begin(), vec.end() );

	EXPECT_EQ( prefix.size(), 5 );
}
TEST( PrefixInitListConstruct, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
}
TEST( PrefixRange, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.range( 0, 4 ), 5 );
	EXPECT_EQ( prefix.range( 0, 1 ), 2 );
	EXPECT_EQ( prefix.range( 3, 3 ), 1 );
	EXPECT_EQ( prefix.range( 2, 4 ), 3 );
}
TEST( PrefixAt, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.at( 0 ), 1 );
	EXPECT_EQ( prefix.at( 2 ), 3 );
	EXPECT_EQ( prefix.at( 4 ), 5 );
}
TEST( PrefixElementAt, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.element_at( 0 ), 1 );
	EXPECT_EQ( prefix.element_at( 2 ), 1 );
	EXPECT_EQ( prefix.element_at( 4 ), 1 );
}
TEST( PrefixPushBack, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );

	prefix.push_back( 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixEmplaceBack, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );

	prefix.emplace_back( 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixPushArray, BasicAssertions )
{
	prefix_array< int > prefix( { 1, 1, 1 } );

	EXPECT_EQ( prefix.size(), 3 );
	EXPECT_EQ( prefix.range( 0, 2 ), 3 );

	prefix.push_array( 1, 1, 1 );

	EXPECT_EQ( prefix.size(), 6 );
	EXPECT_EQ( prefix.range( 0, 5 ), 6 );
}
TEST( PrefixReserve, BasicAssertions )
{
	prefix_array< int > prefix;

	EXPECT_EQ( prefix.size(), 0 );
	EXPECT_EQ( prefix.capacity(), DEFAULT_CAPACITY );

	prefix.push_array( 1, 1, 1, 1, 1 );

	prefix.reserve( 2 * DEFAULT_CAPACITY );

	EXPECT_EQ( prefix.size(), 5 );
	EXPECT_EQ( prefix.range( 0, 4 ), 5 );
	EXPECT_EQ( prefix.capacity(), 2 * DEFAULT_CAPACITY );
}
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
TEST( FenwickDefaultConstruct, BasicAssertions )
{
	fenwick_tree< int > ftree;

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), DEFAULT_CAPACITY );
}
TEST( FenwickReserveConstruct, BasicAssertions )
{
	fenwick_tree< int > ftree( CUSTOM_CAPACITY );

	EXPECT_EQ( ftree.size(), 0 );
	EXPECT_EQ( ftree.capacity(), CUSTOM_CAPACITY );
}
TEST( FenwickPointerConstruct, BasicAssertions )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	fenwick_tree< int > ftree( &ptr, 5 );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickIteratorConstruct, BasicAssertions )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	fenwick_tree< int > ftree( vec.begin(), vec.end() );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickInitListConstruct, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
}
TEST( FenwickRange, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.range( 0, 4 ), 5 );
	EXPECT_EQ( ftree.range( 0, 1 ), 2 );
	EXPECT_EQ( ftree.range( 3, 3 ), 1 );
	EXPECT_EQ( ftree.range( 2, 4 ), 3 );
}
TEST( FenwickAt, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 2, 3, 4, 5 } );

	EXPECT_EQ( ftree.at( 0 ), 1 );
	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.at( 4 ), 5 );
}
TEST( FenwickUpdate, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.update( 3, 2 );

	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.range( 0, 4 ), 7 );
}
TEST( FenwickAdd, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.add( 2, 2 );

	EXPECT_EQ( ftree.at( 2 ), 3 );
	EXPECT_EQ( ftree.range( 0, 4 ), 7 );
}
TEST( FenwickPushBack, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.push_back( 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickEmplaceBack, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 5 );
	EXPECT_EQ( ftree.range( 0, 4 ), 5 );

	ftree.emplace_back( 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickPushArray, BasicAssertions )
{
	fenwick_tree< int > ftree( { 1, 1, 1 } );

	EXPECT_EQ( ftree.size(), 3 );
	EXPECT_EQ( ftree.range( 0, 2 ), 3 );

	ftree.push_array( 1, 1, 1 );

	EXPECT_EQ( ftree.size(), 6 );
	EXPECT_EQ( ftree.range( 0, 5 ), 6 );
}
TEST( FenwickReserve, BasicAssertions )
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
// TEST( SegmentDefaultConstruct, BasicAssertions )
// {
// 	segment_tree< int > segtree;

// 	EXPECT_EQ( segtree.size(), 0 );
// 	EXPECT_EQ( segtree.capacity(), DEFAULT_CAPACITY );
// }
TEST( SegmentParentBuilderConstruct, BasicAssertions )
{
	segment_tree< int > segtree( []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 0 );
	EXPECT_EQ( segtree.capacity(), DEFAULT_CAPACITY );
}
TEST( SegmentReserveConstruct, BasicAssertions )
{
	segment_tree< int > segtree( CUSTOM_CAPACITY, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 0 );
	EXPECT_EQ( segtree.capacity(), CUSTOM_CAPACITY );
}
TEST( SegmentPointerConstruct, BasicAssertions )
{
	int array[] = { 1, 1, 1, 1, 1 };
	int * ptr = array;

	segment_tree< int > segtree( &ptr, 5, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 5 );
	EXPECT_EQ( segtree.capacity(), 5 );
}
TEST( SegmentIteratorConstruct, BasicAssertions )
{
	std::vector< int > vec( { 1, 1, 1, 1, 1 } );

	segment_tree< int > segtree( vec.begin(), vec.end(), []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 5 );
}
TEST( SegmentInitListConstruct, BasicAssertions )
{
	segment_tree< int > segtree( { 1, 1, 1, 1, 1 }, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 5 );
}
TEST( SegmentRange, BasicAssertions )
{
	segment_tree< int > segtree( { 1, 2, 3, 4, 5 }, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.range( 0, 4 ), 1 );

	segtree.set_parent_builder( []( int lhs, int rhs ){ return lhs > rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.range( 0, 4 ), 5 );
}
TEST( SegmentPushBack, BasicAssertions )
{
	segment_tree< int > segtree( { 1, 2, 3, 4, 5 }, []( int lhs, int rhs ){ return lhs > rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.size(), 5 );
	EXPECT_EQ( segtree.range( 0, 4 ), 5 );

	segtree.push_back( 6 );

	EXPECT_EQ( segtree.size(), 6 );
	EXPECT_EQ( segtree.range( 0, 5 ), 6 );
}
TEST( SegmentEmplaceBack, BasicAssertions )
{
	struct other_data
	{
		int x;
		int y;
		std::string hash;

		other_data (				   ) : x{ 0 }, y{ 0 }, hash{ "0" } {}
		other_data ( int x_, int y_ ) : x{ x_ }, y{ y_ } { hash = std::to_string( x_ + y_ ); }

		other_data ( other_data const & other )
		{
			x = other.x;
			y = other.y;
			hash = other.hash;
		}
		other_data ( other_data && other )
		{
			x = other.x;
			y = other.y;
			hash = std::move( other.hash );
		}

		other_data & operator= ( other_data const & other )
		{
			x = other.x;
			y = other.y;
			hash = other.hash;

			return *this;
		}
		other_data & operator= ( other_data && other ) noexcept
		{
			x = other.x;
			y = other.y;
			hash = std::move( other.hash );

			return *this;
		}

		bool operator< ( other_data const & other ) const { return x <  other.x; }
		bool operator> ( other_data const & other ) const { return x >  other.x; }
		bool operator==( other_data const & other ) const { return x == other.x; }
		bool operator==( int other ) const { return x == other; }
	};

	struct some_data
	{
		int x;
		int y;

		some_data (				   ) : x{ 0 }, y{ 0 } {}
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

	segment_tree< some_data > segtree( 5, []( some_data const & lhs, some_data const & rhs ){ return lhs > rhs ? lhs : rhs; } );

	segtree.emplace_back( 1, 1 );
	segtree.emplace_back( 2, 2 );
	segtree.emplace_back( 3, 3 );

	EXPECT_EQ( segtree.size(), 3 );
	EXPECT_EQ( segtree.range( 0, 2 ), 3 );
}
// TEST( SegmentEmplaceVector, BasicAssertions )
// {
// 	std::vector< int > vec1( { 1, 2, 3 } );
// 	std::vector< int > vec2( { 3, 4, 5 } );
// 	std::vector< int > vec3( { 6, 7, 8 } );

// 	segment_tree< std::vector< int > > segtree( 3, []( std::vector< int > const & lhs, std::vector< int > const & rhs ) -> std::vector< int > const & { return lhs[ 0 ] > rhs[ 0 ] ? lhs : rhs; } );

// 	segtree.emplace_back( vec1 );
// 	segtree.emplace_back( vec2 );
// 	segtree.emplace_back( vec3 );

// 	EXPECT_EQ( segtree.range( 0, 2 ), vec3 );
// }
TEST( SegmentUpdate, BasicAssertions )
{
	segment_tree< int > segtree( { 1, 2, 3, 4, 5 }, []( int lhs, int rhs ){ return lhs < rhs ? lhs : rhs; } );

	EXPECT_EQ( segtree.range( 0, 4 ), 1 );

	segtree.update( 2, 0 );

	EXPECT_EQ( segtree.range( 0, 4 ), 0 );
}
TEST( SegmentParentBuilder, BasicAssertions )
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

    EXPECT_EQ( segtree.range( 0, 7 ), max );
}



































