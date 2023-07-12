//
//
//      natprolib
//      static_allocator.hpp
//

#pragma once

#include <_alloc/_alloc.hpp>
#include <_alloc/_stack_alloc.hpp>
#include <_alloc/_frag_alloc.hpp>


namespace npl
{


template< typename T, size_t MemSize >
class static_allocator
{
public:
        using _base_alloc_type = alloc::typed_frag_allocator< T, MemSize > ;

        using         value_type = typename _base_alloc_type::        value_type ;
        using            pointer = typename _base_alloc_type::           pointer ;
        using      const_pointer = typename _base_alloc_type::     const_pointer ;
        using       void_pointer = typename _base_alloc_type::      void_pointer ;
        using const_void_pointer = typename _base_alloc_type::const_void_pointer ;

        using difference_type = typename _base_alloc_type::difference_type ;
        using       size_type = typename _base_alloc_type::      size_type ;
        using      block_type = typename _base_alloc_type::     block_type ;

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return alloc_.total_mem(); }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return alloc_. free_mem(); }
        NPL_NODISCARD constexpr size_type  max_size () const noexcept { return alloc_. max_size(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        { return alloc_.owns( _block_ ); }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _count_ ) noexcept
        { return alloc_.allocate( _count_ ); }

        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        { return alloc_.allocate_all(); }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_count_ ) noexcept
        { return alloc_.reallocate( _block_, _new_count_ ); }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type const _count_, unsigned const _align_ ) noexcept
        { return alloc_.aligned_allocate( _count_, _align_ ); }

        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const & _block_, size_type const _new_count_, unsigned const _align_ ) noexcept
        { return alloc_.aligned_reallocate( _block_, _new_count_, _align_ ); }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        { alloc_.deallocate( _block_ ); }

        constexpr void deallocate_all () noexcept
        { alloc_.deallocate_all(); }
private:
        _base_alloc_type alloc_ ;
};


} // namespace npl
