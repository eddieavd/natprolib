//
//  main.cpp
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#include <iostream>

#include "include/range_queries/fenwick_tree.h"


using namespace npl::rq;

int main ()
{
	/*
	{
		fenwick_tree< int > ftree( 10, 1 );

		for( std::size_t i = 0; i < 10; ++i )
		{
			std::cout << ftree[ i ] << " ";
		}
		std::cout << std::endl;
	}
	*/

	{
		fenwick_tree< int > ftree;

		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );
		ftree.push_back( 1 );

		for( std::size_t i = 0; i < ftree.size(); ++i )
		{
			std::cout << ftree[ i ] << " " << ftree.range( 0, i ) << std::endl;
		}

		std::cout << "-------------------\n";
	}






	return 0;
}
