//
//	mem.h
//	natprolib
//
//	Created by Edhem Avdagic on 15/06/2022
//	Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cstring>

#include "traits.h"


namespace npl
{

namespace mem
{


template< typename T, typename Alloc >
struct _temp_value
{
	using _alloc_traits = std::allocator_traits< Alloc >;
	using _v_t = typename std::aligned_storage< sizeof( T ), alignof( T )>::type;

	_v_t _v;
	Alloc & _a;

	T * _addr () { return reinterpret_cast< T * >( addressof( _v ) ); }
	T &   get () { return *_addr(); }

	template< typename... Args >
	_temp_value ( Alloc & _alloc_, Args&&... _args_ ) : _a( _alloc_ )
	{
		_alloc_traits::construct( _a, reinterpret_cast< T * >( addressof( _v ) ), std::forward< Args >( _args_ )... );
	}

	~_temp_value () { _alloc_traits::destroy( _a, _addr() ); }
};

template< typename Alloc >
void _swap_allocator ( Alloc & _a1_, Alloc & _a2_, std::true_type ) noexcept
{
        using std::swap;
        swap( _a1_, _a2_ );
}

template< typename Alloc >
inline
void _swap_allocator ( Alloc &, Alloc &, std::false_type ) noexcept {}

template< typename Alloc >
inline
void _swap_allocator ( Alloc & _a1_, Alloc & _a2_ ) noexcept
{
        _swap_allocator( _a1_, _a2_, std::integral_constant< bool, std::allocator_traits< Alloc >::propagate_on_container_swap::value >() );
}

template< typename Alloc, typename Ptr >
void _construct_forward_with_exception_guarantees ( Alloc & _a_, Ptr _begin1_, Ptr _end1_, Ptr & _begin2_ )
{
        static_assert( is_cpp17_move_insertable_v< Alloc >,
                        "The specified type does not meet the requirements of Cpp17MoveInsertable" );
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
                ( is_default_allocator_v< Alloc > || !has_construct_v< Alloc, T*, T > ) &&
                  std::is_trivially_move_constructible_v< T >
        >
>
void _construct_forward_with_exception_guarantees ( Alloc &, T* _begin1_, T* _end1_, T* & _begin2_ )
{
        std::ptrdiff_t n = _end1_ - _begin1_;

        if( n > 0 )
        {
		memcpy( _begin2_, _begin1_, n * sizeof( T ) );
                _begin2_ += n;
        }
}

template< typename Alloc, typename Ptr >
void _construct_backward_with_exception_guarantees ( Alloc & _a_, Ptr _begin1_, Ptr _end1_, Ptr & _end2_ )
{
        static_assert( is_cpp17_move_insertable_v< Alloc >,
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
                ( is_default_allocator_v< Alloc > || !has_construct_v< Alloc, T*, T > ) &&
                  std::is_trivially_move_constructible_v< T >
        >
>
void _construct_backward_with_exception_guarantees ( Alloc &, T* _begin1_, T* _end1_, T* & _end2_ )
{
        std::ptrdiff_t n = _end1_ - _begin1_;
        _end2_ -= n;

        if( n > 0 )
        {
		memcpy( static_cast< void* >( _end2_ ), static_cast< void const* >( _begin1_ ), n * sizeof( T ) );
        }
}


} // namespace mem

} // namespace npl
