//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include <iostream>
#include <cstdio>
#include <vector>

#include "tests/test_range_queries.hpp"


struct S
{
    int val = 3;
    
    S () { printf( "default\n" ); };
    S ( int _val_ ) : val { _val_ } { printf( "ctor\n" ); }
    
    S ( S && ) { printf( "move\n" ); }
    S ( S const & ) { printf( "copy\n" ); }
    S & operator= ( S const & rhs ) { printf( "copy asg\n" ); val = rhs.val; return *this; }
    S & operator= ( S && rhs ) { printf( "move asg\n" ); val = rhs.val; return *this; }
    
    ~S () { printf( "dtor\n" ); };
    
};

int main ()
{
    
//    S arr[ 4 ] = { { 3 }, { 2 }, { 1 }, { 0 }  };
//
//    for( auto i = 0; i < 4; i++ )
//    {
//        std::cout << arr[ i ].val << std::endl;
//    }
//
//    natprolib::range_queries::segment_tree< S > segtree( 4, []( S const & lhs, S const & rhs ){ return S(); } );
//
//    segtree.print();

    
    std::vector< int > vec;
    
    vec.emplace_back( 1 );

    
    
    
    
    
    return 0;
}


//      cmake!!!!!
//      doxygen!!!

