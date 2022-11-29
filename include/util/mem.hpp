//
//
//      natprolib
//      mem.hpp
//

#pragma once

#include <cstring>

#include "util.hpp"
#include "traits.hpp"


namespace npl
{


namespace mem
{


struct _default_init_tag {};
struct   _value_init_tag {};

template< typename T >
constexpr T* to_address ( T * _ptr_ ) noexcept
{
        static_assert( !std::is_function_v< T > );
        return _ptr_;
}

template< typename T >
constexpr auto to_address ( T const & _ptr_ ) noexcept
{
        return std::pointer_traits< T >::to_address( _ptr_ );
}

template< typename T, typename... Args >
constexpr T * construct_at ( T * _ptr_, Args&&... _args_ )
{
        return new( _ptr_ ) T{ NPL_FWD( _args_ )... };
}

template< typename Alloc >
void _swap_allocator ( Alloc & _lhs_, Alloc & _rhs_, std::true_type ) noexcept
{
        using std::swap;
        swap( _lhs_, _rhs_ );
}

template< typename Alloc >
inline
void _swap_allocator ( Alloc &, Alloc &, std::false_type ) noexcept {}

template< typename Alloc >
inline
void _swap_allocator ( Alloc & _lhs_, Alloc & _rhs_ ) noexcept
{
        _swap_allocator( _lhs_, _rhs_, std::integral_constant< bool, std::allocator_traits< Alloc >::propagate_on_container_swap::value >() );
}

template< typename InputIterator, typename OutputIterator >
inline
OutputIterator
_move_forward ( InputIterator _first_, InputIterator _last_, OutputIterator _dest_ )
{
        for( ; _first_ != _last_; ++_first_, ( void ) ++_dest_ )
        {
                *_dest_ = NPL_MOVE( *_first_ );
        }
        return _dest_;
}

template< typename Alloc, typename Ptr >
static
void _construct_forward_with_exception_guarantees ( Alloc & _alloc_, Ptr _begin1_, Ptr _end1_, Ptr & _begin2_ )
{
        static_assert( is_cpp17_move_insertable_v< Alloc >,
                        "The specified type does not meet the requirements of cpp17_move_insertable" );
        using _alloc_traits = std::allocator_traits< Alloc >;

        for( ; _begin1_ != _end1_; ++_begin1_, ( void ) ++_begin2_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _begin2_ ), NPL_MOVE_IF_NOEXCEPT( *_begin1_ ) );
        }
}

template< typename Alloc, typename T,
          typename std::enable_if_t
          <
                ( is_default_allocator_v< Alloc > || !has_construct_v< Alloc, T*, T > ) &&
                  std::is_trivially_move_constructible_v< T >
          >
>
static
void _construct_forward_with_exception_guarantees ( Alloc &, T * _begin1_, T * _end1_, T* & _begin2_ )
{
        std::ptrdiff_t count = _end1_ - _begin1_;

        if( count > 0 )
        {
                std::memcpy( _begin2_, _begin1_, count * sizeof( T ) );
                _begin2_ += count;
        }
}

template< typename Alloc, typename Ptr >
static
void _construct_backward_with_exception_guarantees ( Alloc & _alloc_, Ptr _begin1_, Ptr _end1_, Ptr _end2_ )
{
        static_assert( is_cpp17_move_insertable_v< Alloc >,
                        "The specified type does not meet the requirements of cpp17_move_insertable" );
        using _alloc_traits = std::allocator_traits< Alloc >;

        while( _end1_ != _begin1_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _end2_ - 1 ), NPL_MOVE_IF_NOEXCEPT( *--_end1_ ) );
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
static
void _construct_backward_with_exception_guarantees ( [[ maybe_unused ]] Alloc &, T * _begin1_, T * _end1_, T * & _end2_ )
{
        std::ptrdiff_t count = _end1_ - _begin1_;
        _end2_ -= count;

        if( count > 0 )
        {
                std::memcpy( static_cast< void* >( _end2_ ), static_cast< void const * >( _begin1_ ), count * sizeof( T ) );
        }
}

template< typename Alloc, typename Iter, typename Ptr >
inline
static
void _construct_range_forward ( Alloc & _alloc_, Iter _begin1_, Iter _end1_, Ptr & _begin2_ )
{
        using _alloc_traits = typename std::allocator_traits< Alloc >;

        for( ; _begin1_ != _end1_; ++_begin1_, ( void ) ++_begin2_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _begin2_ ), *_begin1_ );
        }
}

template< typename Alloc, typename SrcT, typename DestT,
          typename RawSrcT  = typename std::remove_const_t< SrcT >,
          typename RawDestT = typename std::remove_const_t< DestT >
          >
inline
static
typename std::enable_if_t
<
        std::is_trivially_move_constructible_v< DestT > &&
        std::is_same_v< RawSrcT, RawDestT > &&
        (  is_default_allocator_v< Alloc > ||
          !has_construct_v< Alloc, DestT*, SrcT& > ),
        void
>
_construct_range_forward ( [[ maybe_unused ]] Alloc & _alloc_, SrcT * _begin1_, SrcT * _end1_, DestT* & _begin2_ )
{
        std::ptrdiff_t _count_ = _end1_ - _begin1_;

        if( _count_ > 0 )
        {
                std::memcpy( const_cast< RawDestT * >( _begin2_ ), _begin1_, _count_ * sizeof( DestT ) );
                _begin2_ += _count_;
        }
}



} // namespace mem


} // namespace npl

























