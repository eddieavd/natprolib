//
//
//      natprolib
//      operations.hpp
//

#pragma once

#include <util.hpp>


namespace npl
{


template< typename Arg, typename Res >
struct _unary_function
{
        using argument_type = Arg ;
        using   result_type = Res ;
};

template< typename Arg1, typename Arg2, typename Res >
struct _binary_function
{
        using  first_argument_type = Arg1 ;
        using second_argument_type = Arg2 ;
        using          result_type = Res  ;
};


//=====================================================================
//      less
//=====================================================================

template< typename T = void >
struct less
        : _binary_function< T, T, bool >
{
        using result_type = bool ;

        inline constexpr bool operator() ( T const & _lhs_, T const & _rhs_ ) const
        { return _lhs_ < _rhs_; }
};

template<>
struct less< void >
{
        template< typename T1, typename T2 >
        inline constexpr
        auto operator() ( T1 && _lhs_, T2 && _rhs_ ) const
                noexcept( noexcept( NPL_FWD( _lhs_ ) < NPL_FWD( _rhs_ ) ) )
                -> decltype(        NPL_FWD( _lhs_ ) < NPL_FWD( _rhs_ ) )
                {  return           NPL_FWD( _lhs_ ) < NPL_FWD( _rhs_ ) ; }
};


} // namespace npl
