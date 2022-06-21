//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "include/natprolib.h"


using namespace npl::meta_rq;

int main ()
{
	constexpr auto prefix = make_prefix< int, 4 >( 1 );
	constexpr auto ftree  = make_ftree < int, 4 >( 1 );

	static_assert( prefix.range( 0, 3 ) == 4 );
	static_assert(  ftree.range( 0, 3 ) == 4 );

	constexpr auto prefix2d = make_prefix_2d< int, 4, 4 >( 1 );

	static_assert( prefix2d.range( 0, 0, 3, 3 ) == 16 );




	return 0;
}
