//
//
//      natprolib
//      _stack_alloc.hpp
//

#pragma once

#include <iostream>

#include <_alloc/_alloc.hpp>


namespace npl
{


namespace alloc
{


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"


template< size_t MemSize = 1024 * 1024 >
class stack_allocator
{
        char   mem_[ MemSize ] ;
        char * ptr_{ mem_    } ;
public:
        using         value_type = void         ;
        using            pointer = void       * ;
        using      const_pointer = void const * ;
        using       void_pointer = void       * ;
        using const_void_pointer = void const * ;

        using difference_type = npl::       ptrdiff_t ;
        using       size_type = npl::          size_t ;
        using      block_type = block_t< value_type > ;

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return MemSize; }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return mem_ + MemSize - ptr_; }

        NPL_NODISCARD constexpr size_type max_size () const noexcept { return total_mem(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        {
                return  ( _block_.ptr_ >= mem_           ) &&
                        ( _block_.ptr_ <  mem_ + MemSize )  ;
        }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _size_ ) noexcept
        {
                if( ptr_ + _size_ > mem_ + MemSize ) return { nullptr, 0 };

                block_type block { ptr_, _size_ };
                ptr_ += _size_;
                return block;
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                if( ptr_ == mem_ + MemSize ) return { nullptr, 0 };

                block_type block { ptr_, mem_ + MemSize - ptr_ };
                ptr_ = mem_ + MemSize;
                return block;
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_size_ ) noexcept
        {
                if( _block_.ptr_ == ptr_ - _block_.len_ &&
                        free_mem() >= _new_size_ - _block_.len_ )
                {
                        ptr_ += _new_size_ - _block_.len_;
                        return { _block_.ptr_, _new_size_ };
                }
                return _block_;
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::stack_allocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::stack_allocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( owns( _block_ ), "alloc::stack_allocator::deallocate: allocator does not own block" );

                if( _block_.ptr_ == ptr_ - _block_.len_ )
                {
                        ptr_ -= _block_.len_;
                }
        }
        constexpr void deallocate_all () noexcept { ptr_ = mem_; }
};


template< typename T, size_t MemSize = 1024 * 1024 / sizeof( T ) >
class typed_stack_allocator
{
        stack_allocator< MemSize * sizeof( T ) > alloc_ ;
public:
        static constexpr size_t value_type_size { sizeof( T ) } ;

        using         value_type = T                  ;
        using            pointer = value_type       * ;
        using      const_pointer = value_type const * ;
        using       void_pointer = void             * ;
        using const_void_pointer = void       const * ;

        using difference_type = npl::       ptrdiff_t ;
        using       size_type = npl::          size_t ;
        using      block_type = block_t< value_type > ;

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return alloc_.total_mem(); }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return alloc_. free_mem(); }
        NPL_NODISCARD constexpr size_type  max_size () const noexcept { return alloc_. max_size(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        {
                return alloc_.owns( { _block_.ptr_, _block_.len_ * value_type_size } );
        }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _count_ ) noexcept
        {
                auto void_block = alloc_.allocate( _count_ * value_type_size );
                return void_block.ptr_ ? block_type{ static_cast< pointer >( void_block.ptr_ ), _count_ }
                                       : block_type{ nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                size_type free = alloc_.free_mem();
                size_type count = free / value_type_size;

                if( count < 1 ) return { nullptr, 0 };

                auto void_block = alloc_.allocate( count * value_type_size );
                return { static_cast< pointer >( void_block.ptr_ ), count };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_count_ ) noexcept
        {
                block_type reallocation = alloc_.reallocate( { _block_.ptr_, _block_.len_ * value_type_size },
                                                                _new_count_ * value_type_size );

                return block_type{ reallocation.ptr_, reallocation.len_ / value_type_size };
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_stack_allocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_stack_allocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                alloc_.deallocate( { _block_.ptr_, _block_.len_ * value_type_size } );
        }
        constexpr void deallocate_all () noexcept { alloc_.deallocate_all; }
};


#pragma GCC diagnostic pop


} // namespace alloc


} // namespace npl
