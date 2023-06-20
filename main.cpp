//
//
//      natprolib
//      main.cpp
//

#include <vector>

#include <_iter/iter_traits.hpp>
#include <_traits/npl_traits.hpp>

#include <container/vector>
#include <range_queries/prefix_vector>


int main ()
{
        static_assert( npl::is_container_v< npl::prefix_vector< int, npl::allocator< int > > > );
        static_assert( npl::is_2d_container_v< npl::vector< npl::prefix_vector< int > > > );

        using     iter_t = npl::vector< int >::iterator ;
        using std_iter_t = std::vector< int >::iterator ;

        static_assert( npl::is_same_v< typename npl::iterator_traits<     iter_t >::iterator_category, npl::random_access_iterator_tag > );
        static_assert( npl::is_same_v< typename npl::iterator_traits< std_iter_t >::iterator_category, std::random_access_iterator_tag > );

        static_assert(                npl::is_convertible_v< npl::integral_constant< bool,  true >, std::integral_constant< bool,  true > >   );
        static_assert(                npl::is_convertible_v< std::integral_constant< bool, false >, npl::integral_constant< bool, false > >   );
        static_assert( npl::is_not_v< npl::is_convertible_v< npl::integral_constant< bool,  true >, std::integral_constant< bool, false > > > );
        static_assert( npl::is_not_v< npl::is_convertible_v< std::integral_constant< bool, false >, npl::integral_constant< bool,  true > > > );


        return 0;
}
