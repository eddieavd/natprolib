//
//
//      natprolib
//      _frag_alloc.hpp
//

#pragma once

#include <_alloc/_alloc.hpp>

#include <container/static_vector>


namespace npl
{


namespace alloc
{


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"


template< size_t MemSize = 1024 * 1024 >
class frag_allocator
{
public:
        using         value_type = void         ;
        using            pointer = void       * ;
        using      const_pointer = void const * ;
        using       void_pointer = void       * ;
        using const_void_pointer = void const * ;

        using difference_type = npl::       ptrdiff_t ;
        using       size_type = npl::          size_t ;
        using      block_type = block_t< value_type > ;

        constexpr frag_allocator () noexcept : free_list_( 1, { mem_, MemSize } ) {}

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return          MemSize; }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return _calc_free_mem(); }

        NPL_NODISCARD constexpr size_type max_size () const noexcept { return total_mem(); }

        NPL_NODISCARD constexpr bool operator== ( frag_allocator const & _other_ ) const noexcept
        {
                return mem_ == _other_.mem_ && free_list_ == _other_.free_list_;
        }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        {
                return  ( _block_.ptr_ >= mem_           ) &&
                        ( _block_.ptr_ <  mem_ + MemSize )  ;
        }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _size_ ) noexcept
        {
                size_type best_pos = _find_best_block( _size_ );
                block_type best = free_list_.at( best_pos );

                free_list_.erase( best );

                if( best.len_ > _size_ )
                {
                        free_list_.push_back( { static_cast< char * >( best.ptr_ ) + _size_,
                                                best.len_ - _size_ } );
                        _merge_surrounding( free_list_.size() - 1 );
                }
                best.len_ = _size_;
                return best;
        }
        NPL_NODISCARD constexpr block_type new_allocate ( size_type const _size_ ) noexcept
        {
                size_type best_pos = _find_best_block( _size_ );
                block_type & best = free_list_.at( best_pos );
                block_type allocation{ best.ptr_, _size_ };

                if( best.len_ == _size_ )
                {
                        free_list_.erase( best );
                }
                else
                {
                        size_type diff = best.len_ - _size_;
                        best.ptr_ = static_cast< char * >( best.ptr_ ) + diff;
                        best.len_ -= diff;
                }
                return allocation;
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::frag_allocator::allocate_all: unsupported operation" );
                return { nullptr, 0 };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_size_ ) noexcept
        {
                size_type extend_pos = _find_extension( _block_, _new_size_ );

                if( extend_pos < free_list_.size() )
                {
                        block_type & extension = free_list_.at( extend_pos );
                        block_type reallocation { _block_.ptr_, _new_size_ };

                        if( extension.len_ + _block_.len_ == _new_size_ )
                        {
                                free_list_.erase( extension );
                        }
                        else
                        {
                                size_type diff = _block_.len_ + extension.len_ - _new_size_;
                                extension.ptr_ = static_cast< char * >( extension.ptr_ ) + diff;
                                extension.len_ -= diff;
                        }
                        return reallocation;
                }
                else
                {
                        block_type reallocation = allocate( _new_size_ );
                        memcpy( reallocation.ptr_, _block_.ptr_, _block_.len_ );
                        free_list_.push_back( _block_ );
                        _merge_surrounding( free_list_.size() - 1 );
                        return reallocation;
                }
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type const, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::frag_allocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type const, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::frag_allocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
//                NPL_CONSTEXPR_ASSERT( owns( _block_ ), "alloc::frag_allocator::deallocate: allocator does not own block" );

                if( owns( _block_ ) ) free_list_.push_back( _block_ );

                _merge_surrounding( free_list_.size() - 1 );
        }
        constexpr void deallocate_all () noexcept
        {
                free_list_.clear();
                free_list_.push_back( block_type{ mem_, MemSize } );
        }
private:
        char mem_[ MemSize + 1 ] ;

        static_vector< block_type, 1024 > free_list_ ;

        NPL_NODISCARD constexpr size_type _calc_free_mem () const noexcept
        {
                return 0;
        }
        NPL_NODISCARD constexpr size_type _find_extension ( block_type const & _block_, size_type const _new_size_ ) const noexcept
        {
                ( void )    _block_ ;
                ( void ) _new_size_ ;

                return free_list_.size();
        }
        NPL_NODISCARD constexpr size_type _find_best_block ( size_type const _size_ ) const noexcept
        {
                size_type  best_pos { 0 } ;
                block_type best           ;

                for( size_type i = 0; i < free_list_.size(); ++i )
                {
                        block_type block = free_list_.at( i );

                        if( block.len_ >= _size_ && block.len_ < best.len_ )
                        {
                                best = block;
                                best_pos = i;
                        }
                }
                return best_pos;
        }
        NPL_NODISCARD constexpr block_type _merge_blocks ( block_type const & _lhs_, block_type const & _rhs_ ) const noexcept
        {
                NPL_CONSTEXPR_ASSERT( static_cast< char * >( _lhs_.ptr_ ) + _lhs_.len_ == _rhs_.ptr_,
                                "alloc::frag_allocator::_merge_blocks: internal error" );

                return block_type{ _lhs_.ptr_, _lhs_.len_ + _rhs_.len_ } ;
        }
        constexpr void _merge_surrounding ( size_type _block_pos_ ) noexcept
        {
                block_type block = free_list_.at( _block_pos_ );

                for( size_type i = 0; i < free_list_.size(); ++i )
                {
                        block_type other = free_list_.at( i );

                        if( static_cast< char * >( other.ptr_ ) + other.len_ == block.ptr_ )
                        {
                                block_type merged = _merge_blocks( other, block );

                                free_list_.erase( block );
                                free_list_.erase( other );
                                free_list_.push_back( merged );

                                block = free_list_.back();
                                _block_pos_ = free_list_.size() - 1;
                        }
                        if( static_cast< char * >( block.ptr_ ) + block.len_ == other.ptr_ )
                        {
                                block_type merged = _merge_blocks( block, other );

                                free_list_.erase( block );
                                free_list_.erase( other );
                                free_list_.push_back( merged );

                                block = free_list_.back();
                                _block_pos_ = free_list_.size() - 1;
                        }
                }
        }
};


template< typename T, size_t MemSize = 1024 * 1024 / sizeof( T ) >
class typed_frag_allocator
{
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

        NPL_NODISCARD constexpr bool operator== ( typed_frag_allocator const & _other_ ) const noexcept
        { return alloc_ == _other_.alloc_; }

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
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_frag_allocator::allocate_all: unsupported operation" );
                return { nullptr, 0 };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_count_ ) noexcept
        {
                block_type reallocation = alloc_.reallocate( { _block_.ptr_, _block_.len_ * value_type_size },
                                                                _new_count_ * value_type_size );

                return block_type{ reallocation.ptr_, reallocation.len_ / value_type_size };
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_frag_allocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_frag_allocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                alloc_.deallocate( { _block_.ptr_, _block_.len_ * value_type_size } );
        }
        constexpr void deallocate_all () noexcept { alloc_.deallocate_all(); }
private:
        frag_allocator< MemSize * sizeof( T ) > alloc_ ;
};


#pragma GCC diagnostic pop


} // namespace alloc


} // namespace npl
