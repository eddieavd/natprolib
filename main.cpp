//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include <iostream>

#include "include/natprolib.h"


int main ()
{
	constexpr auto prefix = natprolib::meta_range_queries::make_prefix( { 1, 1, 1 } );

	static_assert( prefix.range( 0, 2 ) == 3 );

	constexpr auto ftree = natprolib::meta_range_queries::make_ftree( { 1, 1, 1 } );

	static_assert( ftree.range( 0, 2 ) == 3 );



	return 0;
}
