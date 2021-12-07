//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include <iostream>
#include <vector>
#include <set>

#include "include/natprolib.h"


int main ()
{
	natprolib::range_queries::prefix_array< int > prefix( { 1, 1, 1, 1, 1 } );

	std::cout << prefix.range( 0, 4 ) << std::endl;




    return 0;
}
