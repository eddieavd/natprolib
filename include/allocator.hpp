//
//
//      natprolib
//      allocator.hpp
//

#pragma once

#include <util.hpp>
#include <mem.hpp>
#include <_traits/base_traits.hpp>


namespace npl
{


template< typename T >
class allocator
{
        static_assert( !is_volatile_v< T >, "npl::allocator does not support volatile types" );
public:
        using size_type                              =    size_t ;
        using value_type                             =         T ;
        using difference_type                        = ptrdiff_t ;
        using propagate_on_container_move_assignment = true_type ;
        using is_always_equal                        = true_type ;

        using         pointer = value_type       * ;
        using   const_pointer = value_type const * ;
        using       reference = value_type       & ;
        using const_reference = value_type const & ;

        inline constexpr allocator () noexcept = default ;

        template< typename U >
        inline constexpr allocator ( allocator< U > const & ) noexcept {}

        NPL_NODISCARD inline constexpr T * allocate ( size_t const _n_ )
        {
                NPL_ASSERT( _n_ <= allocator_traits< allocator >::max_size( *this ), "allocator::allocate: n > max_size" );

                if( std::is_constant_evaluated() )
                {
                        return static_cast< value_type * >( ::operator new( _n_ * sizeof( value_type ) ) );
                }
                else
                {
                        return static_cast< value_type * >( mem::_libnpl_allocate( _n_ * sizeof( value_type ), alignof( value_type ) ) );
                }
        }

        inline constexpr void deallocate ( value_type * _ptr_, size_t _n_ ) noexcept
        {
                if( std::is_constant_evaluated() )
                {
                        ::operator delete( _ptr_ );
                }
                else
                {
                        mem::_libnpl_deallocate( ( void * ) _ptr_, _n_ * sizeof( value_type ), alignof( value_type ) );
                }
        }

        template< typename U >
        struct rebind { using other = allocator< U >; };

        inline pointer address ( reference _x_ ) const noexcept
        {
                return std::addressof( _x_ );
        }

        inline const_pointer address ( const_reference _x_ ) const noexcept
        {
                return std::addressof( _x_ );
        }

        NPL_NODISCARD inline value_type * allocate ( size_t _n_, void const * ) { return allocate( _n_ ); }
        NPL_NODISCARD inline size_type    max_size () const noexcept { return size_type( ~0 ) / sizeof( value_type ); }

        template< typename U, typename... Args >
        inline void construct ( U * _ptr_, Args&&... _args_ )
        {
                ::new ( ( void * ) _ptr_ ) U( NPL_FWD( _args_ )... );
        }

        inline void destroy ( pointer _ptr_ ) { _ptr_->~value_type(); }
};


template< typename T >
class allocator< T const >
{
        static_assert( !is_volatile_v< T >, "npl::allocator does not support volatile types" );
public:
        using size_type                              =    size_t ;
        using value_type                             =         T ;
        using difference_type                        = ptrdiff_t ;
        using propagate_on_container_move_assignment = true_type ;
        using is_always_equal                        = true_type ;

        using         pointer = value_type       * ;
        using   const_pointer = value_type const * ;
        using       reference = value_type       & ;
        using const_reference = value_type const & ;

        inline constexpr allocator () noexcept = default ;

        template< typename U >
        inline constexpr allocator ( allocator< U > const & ) noexcept {}

        NPL_NODISCARD inline constexpr value_type const * allocate ( size_t _n_ )
        {
                NPL_ASSERT( _n_ <= allocator_traits< allocator >::max_size( *this ), "allocator::allocate: n > max_size" );

                if( std::is_constant_evaluated() )
                {
                        return static_cast< value_type * >( ::operator new( _n_ * sizeof( value_type ) ) );
                }
                else
                {
                        return static_cast< value_type * >( mem::_libnpl_allocate( _n_ * sizeof( value_type ), alignof( value_type ) ) );
                }
        }

        inline constexpr void deallocate ( value_type const * _ptr_, size_t _n_ )
        {
                if( std::is_constant_evaluated() )
                {
                        ::operator delete( const_cast< value_type * >( _ptr_ ) );
                }
                else
                {
                        mem::_libnpl_deallocate( ( void * ) const_cast< value_type * >( _ptr_ ), _n_ * sizeof( value_type ), alignof( value_type ) );
                }
        }

        template< typename U >
        struct rebind { using other = allocator< U >; };

        inline const_pointer address ( const_reference _x_ ) const noexcept { return std::addressof( _x_ ); }

        NPL_NODISCARD inline value_type const * allocate ( size_t _n_, void const * ) { return allocate( _n_ ); }

        inline size_type max_size () const noexcept { return size_type( ~0 ) / sizeof( value_type ); }

        template< typename U, typename... Args >
        inline void construct ( U * _ptr_, Args&&... _args_ )
        {
                ::new ( ( void * ) _ptr_ ) U( NPL_FWD( _args_ )... );
        }

        inline void destroy ( pointer _ptr_ ) { _ptr_->~value_type(); }
};


template< typename T, typename U >
inline constexpr bool operator== ( allocator< T > const &, allocator< U > const & ) noexcept { return true; }

template< typename T, typename U >
inline constexpr bool operator!= ( allocator< T > const &, allocator< U > const & ) noexcept { return false; }


} // namespace npl
