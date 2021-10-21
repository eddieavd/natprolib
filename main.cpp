//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "include/natprolib.h"


int main ()
{
    natprolib::range_queries::prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

    assert( prefix.range( 0, 4 ) == 5 );

    
    
    return 0;
}
