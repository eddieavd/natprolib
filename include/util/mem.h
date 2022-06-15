//
//	mem.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include "traits.h"


namespace npl
{

namespace mem
{


//  TODO: implement
template< typename T1, typename T2 >
class compressed_pair;

struct _default_init_tag {};
struct _value_init_tag   {};

template< typename Alloc, typename Ptr >
void _construct_forward_with_exception_guarantees ( Alloc & _a_, Ptr _begin1_, Ptr _end1_, Ptr & _begin2_ )
{
        static_assert( is_cpp17_move_insertable< Alloc >::value,
                        "The specified type does not meet the requirements of Cpp17MoveInsertable" );
        static_assert( std::__is_cpp17_move_insertable< Alloc >::value,
                        "lol" );
        using _alloc_traits = std::allocator_traits< Alloc >;

        for( ; _begin1_ != _end1_; ++_begin1_, ( void )++_begin2_ )
        {
                _alloc_traits::construct( _a_, std::to_address( _begin2_ ),
#ifdef NPL_NO_EXCEPTIONS
                                std::move( *_begin1_ )
#else
                                std::move_if_noexcept( *_begin1_ )
#endif
                );
        }
}

template< typename Alloc, typename T,
          typename std::enable_if_t
        <
                ( is_default_allocator< Alloc >::value || !has_construct< Alloc, T*, T >::value ) &&
                  std::is_trivially_move_constructible_v< T >
        >
>
void _construct_forward_with_exception_guarantees ( Alloc &, T* _begin1_, T* _end1_, T* & _begin2_ )
{
        std::ptrdiff_t n = _end1_ - _begin1_;

        if( n > 0 )
        {
                std::memcpy( _begin2_, _begin1_, n * sizeof( T ) );
                _begin2_ += n;
        }
}

template< typename Alloc, typename Ptr >
void _construct_backward_with_exception_guarantees ( Alloc & _a_, Ptr _begin1_, Ptr _end1_, Ptr & _end2_ )
{
        static_assert( is_cpp17_move_insertable< Alloc >::value,
                        "The specified type does not meet the requirements of Cpp17MoveInsertable" );
        using _alloc_traits = std::allocator_traits< Alloc >;

        while( _end1_ != _begin1_ )
        {
                _alloc_traits::construct( _a_, std::to_address( _end2_ - 1 ),
#ifdef NPL_NO_EXCEPTIONS
                                std::move( *--_end1_ )
#else
                                std::move_if_noexcept( *--_end1_ )
#endif
                );
                --_end2_;
        }
}

template< typename Alloc, typename T,
          typename = typename std::enable_if_t
        <
                ( is_default_allocator< Alloc >::value || !has_construct< Alloc, T*, T >::value ) &&
                  std::is_trivially_move_constructible_v< T >
        >
>
void _construct_backward_with_exception_guarantees ( Alloc &, T* _begin1_, T* _end1_, T* & _end2_ )
{
        std::ptrdiff_t n = _end1_ - _begin1_;
        _end2_ -= n;

        if( n > 0 )
        {
                std::memcpy( static_cast< void* >( _end2_ ), static_cast< void const* >( _begin1_ ), n * sizeof( T ) );
        }
}


} // namespace mem

} // namespace npl
