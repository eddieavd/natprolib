//
//
//      natprolib
//      dynamic_allocator.hpp
//

#pragma once

#include <_alloc/_alloc.hpp>
#include <_alloc/_mmap_alloc.hpp>


namespace npl
{


template< typename T >
class dynamic_allocator
{
public:
        using _base_alloc_type = alloc::typed_mmap_allocator< T >     ;
        using     alloc_traits = allocator_traits< _base_alloc_type > ;

        using      value_type = typename alloc_traits::     value_type ;
        using difference_type = typename alloc_traits::difference_type ;
        using       size_type = typename alloc_traits::      size_type ;
        using      block_type = typename alloc_traits::     block_type ;

        using            pointer = typename alloc_traits::           pointer ;
        using      const_pointer = typename alloc_traits::     const_pointer ;
        using       void_pointer = typename alloc_traits::      void_pointer ;
        using const_void_pointer = typename alloc_traits::const_void_pointer ;

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return alloc_traits::total_mem( alloc_ ); }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return alloc_traits:: free_mem( alloc_ ); }
        NPL_NODISCARD constexpr size_type  max_size () const noexcept { return alloc_traits:: max_size( alloc_ ); }

        NPL_NODISCARD constexpr bool operator== ( dynamic_allocator const & _other_ ) const noexcept
        { return alloc_ == _other_.alloc_; }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        { return alloc_traits::owns( alloc_, _block_ ); }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _count_ ) noexcept
        { return alloc_traits::allocate( alloc_, _count_ ); }

        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        { return alloc_traits::allocate_all( alloc_ ); }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_count_ ) noexcept
        { return alloc_traits::reallocate( alloc_, _block_, _new_count_ ); }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type const _count_, unsigned const _align_ ) noexcept
        { return alloc_traits::aligned_allocate( alloc_, _count_, _align_ ); }

        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const & _block_, size_type const _new_count_, unsigned const _align_ ) noexcept
        { return alloc_traits::aligned_reallocate( alloc_, _block_, _new_count_, _align_ ); }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        { alloc_traits::deallocate( alloc_, _block_ ); }

        constexpr void deallocate_all () noexcept
        { alloc_traits::deallocate_all( alloc_ ); }
private:
        _base_alloc_type alloc_ ;
};


} // namespace npl
