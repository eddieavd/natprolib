//
//
//      natprolib
//      _mmap_alloc.hpp
//

#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <_alloc/_alloc.hpp>

#include <container/static_vector>


namespace npl
{


namespace alloc
{


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"


template< typename T = void >
struct page_t
{
        size_t len_ {       0 } ;
        T *    mem_ { nullptr } ;
};


class mmap_allocator
{
public:
        using         value_type = void               ;
        using            pointer = value_type       * ;
        using      const_pointer = value_type const * ;
        using       void_pointer = void             * ;
        using const_void_pointer = void       const * ;

        using difference_type = npl::       ptrdiff_t ;
        using       size_type = npl::          size_t ;
        using      block_type = block_t< value_type > ;
        using       page_type =  page_t< value_type > ;

        static constexpr size_type default_page_size { 1024 * 1024 } ;

        constexpr mmap_allocator (                            ) : page_( _allocate_page( default_page_size ) )
                                                                , free_list_( 1, { page_.mem_, page_.len_ } ) {}
        constexpr mmap_allocator ( size_type const _mem_size_ ) : page_( _allocate_page(        _mem_size_ ) )
                                                                , free_list_( 1, { page_.mem_, page_.len_ } ) {}

        constexpr ~mmap_allocator () noexcept
        {
                if( page_.mem_ ) _deallocate_page( page_ );
        }

        NPL_NODISCARD constexpr size_type max_size () const noexcept
        {
                return std::numeric_limits< size_type >::max();
        }

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return max_size(); }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return max_size(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        {
                return  ( _block_.ptr_ >=                        page_.mem_                ) &&
                        ( _block_.ptr_ <  static_cast< char * >( page_.mem_ ) + page_.len_ )  ;
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
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::mmap_allocator::allocate_all: unimplemented" );
                return { nullptr, 0 };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type const _new_size_ ) noexcept
        {
                ( void )    _block_ ;
                ( void ) _new_size_ ;
                return      _block_ ;
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type const _size_, unsigned const _align_ ) noexcept
        {
                ( void )  _size_ ;
                ( void ) _align_ ;
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const & _block_, size_type const _new_size_, unsigned const _align_ ) noexcept
        {
                ( void )    _block_ ;
                ( void ) _new_size_ ;
                ( void )    _align_ ;
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
//                NPL_CONSTEXPR_ASSERT( owns( _block_ ), "alloc::mmap_allocator::deallocate: allocator does not own block" );

                if( owns( _block_ ) ) free_list_.push_back( _block_ );

                _merge_surrounding( free_list_.size() - 1 );
        }
        constexpr void deallocate_all () noexcept
        {
                free_list_.clear();
                free_list_.push_back( block_type{ page_.mem_, page_.len_ } );
        }
private:
        page_type page_ ;

        static_vector< block_type, 1024 > free_list_ ;

        NPL_NODISCARD constexpr page_type _allocate_page ( size_type const _page_size_ )
        {
                NPL_CONSTEXPR_ASSERT( _page_size_ < max_size(), "alloc::mmap_allocator::_allocate_page: not enough memory" );

                void_pointer ptr = ::mmap( 0, _page_size_, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );

                NPL_CONSTEXPR_ASSERT( ptr != MAP_FAILED, "alloc::mmap_allocator::_allocate_page: failed allocating page" );

                return page_type{ _page_size_, ptr };
        }
        constexpr void _deallocate_page ( page_type const & _page_ ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( _page_.mem_ != nullptr, "alloc::mmap_allocator::_deallocate_page: tried deallocating nullptr" );

                NPL_CONSTEXPR_ASSERT( ( ::munmap( _page_.mem_, _page_.len_ ) == 0 ),
                                        "alloc::mmap_allocator::_deallocate_page: failed deallocating page" );
        }

        NPL_NODISCARD constexpr size_type _find_extension ( block_type const & _block_, size_type const _new_size_ ) const noexcept
        {
                ( void )    _block_ ;
                ( void ) _new_size_ ;
                return 0 ;
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
                                        "alloc::mmap_allocator::_merge_blocks: internal error" );

                return block_type{ _lhs_.ptr_, _lhs_.len_ + _rhs_.len_ };
        }
        constexpr void _merge_surrounding ( size_type _block_pos_ ) noexcept
        {
                block_type block { free_list_.at( _block_pos_ ) };

                for( size_type i = 0; i < free_list_.size(); ++i )
                {
                        block_type other { free_list_.at( i ) };

                        if( static_cast< char * >( other.ptr_ ) +other.len_ == block.ptr_ )
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

template< typename T >
class typed_mmap_allocator
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

        using base_alloc_type = mmap_allocator ;

        constexpr typed_mmap_allocator () : alloc_( base_alloc_type::default_page_size ) {}

        constexpr typed_mmap_allocator ( size_type const _page_size_ ) : alloc_( _page_size_ ) {}

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return alloc_.total_mem(); }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return alloc_. free_mem(); }
        NPL_NODISCARD constexpr size_type  max_size () const noexcept { return alloc_. max_size(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        { return alloc_.owns( _block_ ); }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _count_ ) noexcept
        {
                auto void_block = alloc_.allocate( _count_ * value_type_size );
                return void_block.ptr_ ? block_type{ static_cast< pointer >( void_block.ptr_ ), _count_ }
                                       : block_type{ nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mmap_allocator::allocate_all: unimplemented" );
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
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mmap_allocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mmap_allocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                alloc_.deallocate( { _block_.ptr_, _block_.len_ * value_type_size } );
        }
        constexpr void deallocate_all () noexcept { alloc_.deallocate_all(); }
private:
        mmap_allocator alloc_ ;
};


#pragma GCC diagnostic pop


} // namespace alloc


} // namespace npl
