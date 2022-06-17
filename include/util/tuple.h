//
//	tuple.h
//	natprolib
//
//	Created by Edhem Avdagic on 16/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cstddef>


namespace npl
{


template< std::size_t... > struct _tuple_indices {};

template< typename IdxType, IdxType... Values >
struct _integer_sequence
{
        template< template< typename OIdxType, OIdxType... > typename ToIndexSeq, typename ToIndexType >
        using _convert = ToIndexSeq< ToIndexType, Values... >;

        template< std::size_t S >
        using _to_tuple_indices = _tuple_indices< ( Values + S )... >;
};

namespace _detail
{

template< typename T, std::size_t... Extra > struct _repeat;

template< typename T, T... N, std::size_t... Extra > struct _repeat< _integer_sequence< T, N... >, Extra... >
{
        using type = _integer_sequence< T,
                                        N...,
                                        sizeof...( N ) + N...,
                                        2 * sizeof...( N ) + N...,
                                        3 * sizeof...( N ) + N...,
                                        4 * sizeof...( N ) + N...,
                                        5 * sizeof...( N ) + N...,
                                        6 * sizeof...( N ) + N...,
                                        7 * sizeof...( N ) + N...,
                                        Extra... >;
};

template< typename N > struct _parity;
template< typename N > struct _make : _parity< N % 8 >::template _pmake< N > {};

template<> struct _make< 0 > { using type = _integer_sequence< std::size_t                      >; };
template<> struct _make< 1 > { using type = _integer_sequence< std::size_t, 0                   >; };
template<> struct _make< 2 > { using type = _integer_sequence< std::size_t, 0, 1                >; };
template<> struct _make< 3 > { using type = _integer_sequence< std::size_t, 0, 1, 2             >; };
template<> struct _make< 4 > { using type = _integer_sequence< std::size_t, 0, 1, 2, 3          >; };
template<> struct _make< 5 > { using type = _integer_sequence< std::size_t, 0, 1, 2, 3, 4       >; };
template<> struct _make< 6 > { using type = _integer_sequence< std::size_t, 0, 1, 2, 3, 4, 5    >; };
template<> struct _make< 7 > { using type = _integer_sequence< std::size_t, 0, 1, 2, 3, 4, 5, 6 >; };

template<> struct _parity< 0 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type                                                  > {}; };
template<> struct _parity< 1 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,                                           N - 1 > {}; };
template<> struct _parity< 2 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,                                    N - 2, N - 1 > {}; };
template<> struct _parity< 3 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,                             N - 3, N - 2, N - 1 > {}; };
template<> struct _parity< 4 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,                      N - 4, N - 3, N - 2, N - 1 > {}; };
template<> struct _parity< 5 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,               N - 5, N - 4, N - 3, N - 2, N - 1 > {}; };
template<> struct _parity< 6 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type,        N - 6, N - 5, N - 4, N - 3, N - 2, N - 1 > {}; };
template<> struct _parity< 7 > { template< std::size_t N > struct _pmake : _repeat< typename _make< N / 8 >::type, N - 7, N - 6, N - 5, N - 4, N - 3, N - 2, N - 1 > {}; };

} // namespace _detail

#if __has_builtin(__make_integer_seq)
template< std::size_t E, std::size_t S >
using _make_indices_imp = typename __make_integer_seq< _integer_sequence, std::size_t, E - S >::template _to_tuple_indices< S >;
#else
template< std::size_t E, std::size_t S >
using _make_indices_imp = typename _detail::_make< E - S >::type::template _to_tuple_indices< S >;
#endif

template< std::size_t E, std::size_t S = 0 >
struct _make_tuple_indices
{
        static_assert( S <= E, "_make_tuple_indices input error" );
        using type = _make_indices_imp< E, S >;
};


} // namespace npl
