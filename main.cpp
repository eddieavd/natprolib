//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include "include/range_queries/meta_range_queries.h"


int main ()
{
	constexpr auto prefix = npl::meta_rq::make_prefix< int, 4 >( 1 );
	constexpr auto ftree  = npl::meta_rq::make_ftree < int, 4 >( 1 );

	static_assert( prefix.range( 0, 3 ) == 4 );
	static_assert( ftree.range ( 0, 3 ) == 4 );

	constexpr auto prefix_2d = npl::meta_rq::make_prefix_2d< int, 4, 4 >( 1 );

	static_assert( prefix_2d.range( 0, 0, 3, 3 ) == 16 );



	return 0;
}
