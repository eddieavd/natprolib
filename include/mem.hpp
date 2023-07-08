//
//
//      natprolib
//      mem.hpp
//

#pragma once

#include <cstddef>
#include <cstring>

#include <util.hpp>
#include <_traits/base_traits.hpp>
#include <_traits/npl_traits.hpp>
#include <_alloc/alloc_traits.hpp>
#include <_iter/iter_traits.hpp>
#include <algorithm.hpp>


#define NPL_HAS_NO_SIZED_DEALLOCATION

#define NPL_SMOL_MEMCPY 16
#define NPL_MEMCPY_SIZE ( sizeof( unsigned long long ) )


namespace npl
{


namespace mem
{


template< typename T >
constexpr T* to_address ( T * _ptr_ ) noexcept
{
        static_assert( !is_function_v< T > );
        return _ptr_;
}

template< typename T >
constexpr auto to_address ( T const & _ptr_ ) noexcept
{
        return std::pointer_traits< T >::to_address( _ptr_ );
}

template< typename T >
inline constexpr
T * addressof ( T & _x_ ) noexcept
{
        return __builtin_addressof( _x_ );
}

template< typename T >
inline constexpr void * voidify( T & _from_ )
{
        return const_cast< void * >( static_cast< void const volatile * >( addressof( _from_ ) ) );
}

template< typename T, typename... Args, typename = decltype( ::new( declval< void * >() ) T( declval< Args >()... ) ) >
constexpr T * construct_at ( T * _location_, Args&&... _args_ )
{
#if NPL_STD_VER >= 20
        return std::construct_at( _location_, NPL_FWD( _args_ )... );
#else
        NPL_CONSTEXPR_ASSERT( _location_ != nullptr, "null pointer passed to construct_at" );
        return ::new ( voidify( *_location_ ) ) T( NPL_FWD( _args_ )... );
#endif
}

template< typename Iter, typename... Args >
constexpr auto construct_at ( Iter _it_, Args&&... _args_ ) -> _iter_value_type< Iter > *
{
        return mem::construct_at( _it_.raw(), NPL_FWD( _args_ )... );
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

template< typename T >
inline
constexpr void destruct_at ( T * _ptr_ )
{
        NPL_CONSTEXPR_ASSERT( _ptr_, "mem::destruct_at: nullptr passed to destruct_at" );

        _ptr_->~T();
}

template< typename Iter >
inline
constexpr void destruct_at ( Iter _iter_ )
{
        destruct_at( _iter_.raw() );
}

#pragma GCC diagnostic pop

inline constexpr bool _is_overaligned_for_new ( size_t _align_ ) noexcept
{
        return _align_ > npl::alignment_of< npl::max_align_t >::value ;
}

template< typename... Args >
void * _libnpl_operator_new ( Args... _args_ )
{
#if __has_builtin(__builtin_operator_new) && __has_builtin(__builtin_operator_delete)
        return __builtin_operator_new( _args_... );
#else
        return ::operator new( _args_... );
#endif
}

template< typename... Args >
void _libnpl_operator_delete ( Args... _args_ )
{
#if __has_builtin(__builtin_operator_new) && __has_builtin(__builtin_operator_delete)
        __builtin_operator_delete( _args_... );
#else
        ::operator delete( _args_... );
#endif
}

inline void * _libnpl_allocate ( size_t _size_, [[ maybe_unused ]] size_t _align_ )
{
#ifndef NPL_HAS_NO_ALIGNED_ALLOCATION
        if( _is_overaligned_for_new( _align_ ) )
        {
                std::align_val_t const align_val = static_cast< std::align_val_t >( _align_ );
                return _libnpl_operator_new( _size_, align_val );
        }
#endif
        return _libnpl_operator_new( _size_ );
}

template< typename... Args >
inline void _do_deallocate_handle_size ( void * _ptr_, [[ maybe_unused ]] size_t _size_, Args... _args_ )
{
#ifdef NPL_HAS_NO_SIZED_DEALLOCATION
        return _libnpl_operator_delete( _ptr_, _args_... );
#else
        return _libnpl_operator_delete( _ptr_, _size_, _args_... );
#endif
}

inline void _libnpl_deallocate ( void * _ptr_, size_t _size_, [[ maybe_unused ]] size_t _align_ )
{
#if defined( NPL_HAS_NO_ALIGNED_ALLOCATION )
        return _do_deallocate_handle_size( _ptr_, _size_ );
#else
        if( _is_overaligned_for_new( _align_ ) )
        {
                std::align_val_t const align_val = static_cast< std::align_val_t >( _align_ );
                return _do_deallocate_handle_size( _ptr_, _size_, align_val );
        }
        else
        {
                return _do_deallocate_handle_size( _ptr_, _size_ );
        }
#endif
}

inline void _libnpl_deallocate_unsized ( void * _ptr_, [[ maybe_unused ]] size_t _align_ )
{
#if defined( NPL_HAS_NO_ALIGNED_ALLOCATION )
        return _libnpl_operator_delete( _ptr_ );
#else
        if( _is_overaligned_for_new( _align_ ) )
        {
                std::align_val_t const align_val = static_cast< std::align_val_t >( _align_ );
                return _libnpl_operator_delete( _ptr_, align_val );
        }
        else
        {
                return _libnpl_operator_delete( _ptr_ );
        }
#endif
}

template< typename Alloc >
void _swap_allocator ( Alloc & _lhs_, Alloc & _rhs_, true_type ) noexcept
{
        using npl::swap;
        swap( _lhs_, _rhs_ );
}

template< typename Alloc >
inline
void _swap_allocator ( Alloc &, Alloc &, false_type ) noexcept {}

template< typename Alloc >
inline
void _swap_allocator ( Alloc & _lhs_, Alloc & _rhs_ ) noexcept
{
        _swap_allocator( _lhs_, _rhs_, bool_constant< allocator_traits< Alloc >::propagate_on_container_swap::value >() );
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
        using _alloc_traits = allocator_traits< Alloc >;

        for( ; _begin1_ != _end1_; ++_begin1_, ( void ) ++_begin2_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _begin2_ ), NPL_MOVE_IF_NOEXCEPT( *_begin1_ ) );
        }
}

template< typename Alloc, typename T,
          enable_if_t
          <
                ( is_default_allocator_v< Alloc > || !_has_construct_v< Alloc, T*, T > ) &&
                  is_trivially_move_constructible_v< T >
          >
>
static
void _construct_forward_with_exception_guarantees ( Alloc &, T * _begin1_, T * _end1_, T* & _begin2_ )
{
        ptrdiff_t count = _end1_ - _begin1_;

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
        using _alloc_traits = allocator_traits< Alloc >;

        while( _end1_ != _begin1_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _end2_ - 1 ), NPL_MOVE_IF_NOEXCEPT( *--_end1_ ) );
                --_end2_;
        }
}

template< typename Alloc, typename T,
          typename = enable_if_t
          <
                ( is_default_allocator_v< Alloc > || !_has_construct_v< Alloc, T*, T > ) &&
                  is_trivially_move_constructible_v< T >
          >
>
static
void _construct_backward_with_exception_guarantees ( [[ maybe_unused ]] Alloc &, T * _begin1_, T * _end1_, T * & _end2_ )
{
        ptrdiff_t count = _end1_ - _begin1_;
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
        using _alloc_traits = allocator_traits< Alloc >;

        for( ; _begin1_ != _end1_; ++_begin1_, ( void ) ++_begin2_ )
        {
                _alloc_traits::construct( _alloc_, mem::to_address( _begin2_ ), *_begin1_ );
        }
}

template< typename Alloc, typename SrcT, typename DestT,
          typename RawSrcT  = remove_const_t< SrcT >,
          typename RawDestT = remove_const_t< DestT >
          >
inline
static
enable_if_t
<
        is_trivially_move_constructible_v< DestT > &&
        is_same_v< RawSrcT, RawDestT > &&
        (  is_default_allocator_v< Alloc > ||
          !_has_construct_v< Alloc, DestT*, SrcT& > ),
        void
>
_construct_range_forward ( [[ maybe_unused ]] Alloc & _alloc_, SrcT * _begin1_, SrcT * _end1_, DestT* & _begin2_ )
{
        ptrdiff_t _count_ = _end1_ - _begin1_;

        if( _count_ > 0 )
        {
                std::memcpy( const_cast< RawDestT * >( _begin2_ ), _begin1_, _count_ * sizeof( DestT ) );
                _begin2_ += _count_;
        }
}


} // namespace mem


} // namespace npl
