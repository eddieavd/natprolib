//
//
//      natprolib
//      _alloc.hpp
//

#pragma once

#include <util.hpp>
#include <_traits/base_traits.hpp>

#include <_alloc/alloc_traits.hpp>


namespace npl
{


namespace alloc
{


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"


template< typename T >
struct block_t
{
        T *    ptr_ ;
        size_t len_ ;

        constexpr bool operator== ( block_t const & _other_ ) const noexcept
        { return ptr_ == _other_.ptr_ && len_ == _other_.len_; }
};


class mallocator
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

        NPL_NODISCARD constexpr size_type total_mem () const noexcept { return 4 * 1024 * 1024; }
        NPL_NODISCARD constexpr size_type  free_mem () const noexcept { return 4 * 1024 * 1024; }

        NPL_NODISCARD constexpr size_type max_size () const noexcept { return total_mem(); }

        NPL_NODISCARD constexpr bool owns ( block_type const & ) const noexcept
        { return true; }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _size_ ) noexcept
        {
                pointer data = malloc( _size_ );

                return data ? block_type{    data, _size_ }
                            : block_type{ nullptr,      0 };
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::mallocator::allocate_all: unsupported operation" );
                return { nullptr, 0 };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_size_ ) noexcept
        {
                if( _new_size_ <= _block_.len_ ) { return _block_; }

                pointer data = realloc( _block_.ptr_, _new_size_ );

                return data ? block_type{    data, _new_size_ }
                            : block_type{ nullptr,          0 };
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::mallocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::mallocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( owns( _block_ ), "alloc::mallocator::deallocate: allocator does not own block" );

                free( _block_.ptr_ );
        }
        constexpr void deallocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::mallocator::deallocate_all: unsupported operation" );
        }
};


template< typename T >
class typed_mallocator
{
        mallocator alloc_ ;

        static constexpr size_t value_type_size { sizeof( T ) } ;
public:
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
        { return alloc_.owns( _block_ ); }

        NPL_NODISCARD constexpr block_type allocate ( size_type const _count_ ) noexcept
        {
                block_type allocation { static_cast< value_type * >( alloc_.allocate( _count_ * value_type_size ).ptr_ ), _count_ };

                return allocation.ptr_ ? block_type{ allocation.ptr_, _count_ }
                                       : block_type{         nullptr,       0 };
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mallocator::allocate_all: unsupported operation" );
                return { nullptr, 0 };
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_count_ ) noexcept
        {
                if( _new_count_ <= _block_.len_ ) { return _block_; }

                block_type allocation { alloc_.reallocate( { _block_.ptr_, _block_.len_ * value_type_size }, _new_count_ * value_type_size ) };

                return allocation.ptr_ ?  block_type{ allocation.ptr_, _new_count_ }
                                       : _block_                                   ;
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mallocator::aligned_allocate: unimplemented" );
                return { nullptr, 0 };
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const &, size_type, unsigned ) noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mallocator::aligned_reallocate: unimplemented" );
                return { nullptr, 0 };
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                alloc_.deallocate( { _block_.ptr_, _block_.len_ * value_type_size } );
        }
        constexpr void deallocate_all () noexcept
        {
                NPL_CONSTEXPR_ASSERT( false, "alloc::typed_mallocator::deallocate_all: unsupported operation" );
        }
};


#pragma GCC diagnostic pop


template< typename Primary, typename Fallback >
class fallback_allocator
        : private Primary
        , private Fallback
{
        using _primary  = Primary  ;
        using _fallback = Fallback ;

        static_assert( is_same_v< typename allocator_traits< _primary > ::        value_type     ,
                                  typename allocator_traits< _fallback> ::        value_type > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::           pointer     ,
                                  typename allocator_traits< _fallback> ::           pointer > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::     const_pointer     ,
                                  typename allocator_traits< _fallback> ::     const_pointer > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::      void_pointer     ,
                                  typename allocator_traits< _fallback> ::      void_pointer > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::const_void_pointer     ,
                                  typename allocator_traits< _fallback> ::const_void_pointer > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::   difference_type     ,
                                  typename allocator_traits< _fallback> ::   difference_type > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::         size_type     ,
                                  typename allocator_traits< _fallback> ::         size_type > ) ;
        static_assert( is_same_v< typename allocator_traits< _primary > ::        block_type     ,
                                  typename allocator_traits< _fallback> ::        block_type > ) ;
public:
        static constexpr size_t value_type_size { Primary::value_type_size } ;

        using         value_type = typename allocator_traits< _primary >::        value_type ;
        using            pointer = typename allocator_traits< _primary >::           pointer ;
        using      const_pointer = typename allocator_traits< _primary >::     const_pointer ;
        using       void_pointer = typename allocator_traits< _primary >::      void_pointer ;
        using const_void_pointer = typename allocator_traits< _primary >::const_void_pointer ;
        using    difference_type = typename allocator_traits< _primary >::   difference_type ;
        using          size_type = typename allocator_traits< _primary >::         size_type ;
        using         block_type = typename allocator_traits< _primary >::        block_type ;

        using is_always_equal                        = conjunction_t< typename allocator_traits< _primary  >::is_always_equal                          ,
                                                                      typename allocator_traits< _fallback >::is_always_equal                        > ;
        using propagate_on_container_swap            = conjunction_t< typename allocator_traits< _primary  >::propagate_on_container_swap              ,
                                                                      typename allocator_traits< _fallback >::propagate_on_container_swap            > ;
        using propagate_on_container_copy_assignment = conjunction_t< typename allocator_traits< _primary  >::propagate_on_container_copy_assignment   ,
                                                                      typename allocator_traits< _fallback >::propagate_on_container_copy_assignment > ;
        using propagate_on_container_move_assignment = conjunction_t< typename allocator_traits< _primary  >::propagate_on_container_move_assignment   ,
                                                                      typename allocator_traits< _fallback >::propagate_on_container_move_assignment > ;

        NPL_NODISCARD constexpr size_type total_mem () const noexcept
        {
                return _primary::total_mem() + _fallback::total_mem() ;
        }
        NPL_NODISCARD constexpr size_type free_mem () const noexcept
        {
                return _primary::free_mem() + _fallback::free_mem() ;
        }
        NPL_NODISCARD constexpr size_type max_size () const noexcept
        {
                return _primary::max_size() + _fallback::max_size() ;
        }

        NPL_NODISCARD constexpr bool owns ( block_type const & _block_ ) const noexcept
        {
                return     _primary ::owns( _block_ )
                        || _fallback::owns( _block_ ) ;
        }

        NPL_NODISCARD constexpr block_type allocate ( size_type _size_ ) noexcept
        {
                block_type block = _primary ::allocate( _size_ );
                if( !block.ptr_ )
                           block = _fallback::allocate( _size_ );
                return block;
        }
        NPL_NODISCARD constexpr block_type allocate_all () noexcept
        {
                block_type block = _primary ::allocate_all();
                if( !block.ptr_ )
                           block = _fallback::allocate_all();
                return block;
        }

        NPL_NODISCARD constexpr block_type reallocate ( block_type const & _block_, size_type _new_size_ ) noexcept
        {
                if( _primary::owns( _block_ ) )
                        return _primary ::reallocate( _block_, _new_size_ );
                else
                        return _fallback::reallocate( _block_, _new_size_ );
        }

        NPL_NODISCARD constexpr block_type aligned_allocate ( size_type _size_, unsigned _align_ ) noexcept
        {
                block_type block = _primary ::aligned_allocate( _size_, _align_ );
                if( !block.ptr_ )
                           block = _fallback::aligned_allocate( _size_, _align_ );
                return block;
        }
        NPL_NODISCARD constexpr block_type aligned_reallocate ( block_type const & _block_, size_type _new_size_, unsigned _align_ ) noexcept
        {
                if( _primary::owns( _block_ ) )
                        return _primary ::aligned_reallocate( _block_, _new_size_, _align_ );
                else
                        return _fallback::aligned_reallocate( _block_, _new_size_, _align_ );
        }

        constexpr void deallocate ( block_type const & _block_ ) noexcept
        {
                if( _primary::owns( _block_ ) )
                        _primary ::deallocate( _block_ );
                else
                        _fallback::deallocate( _block_ );
        }
        constexpr void deallocate_all () noexcept
        {
                _primary ::deallocate_all();
                _fallback::deallocate_all();
        }
};


} // namespace alloc


} // namespace npl
