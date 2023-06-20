//
//
//      natprolib
//      alloc_traits.hpp
//

#pragma once

#include <util.hpp>
#include <_traits/base_traits.hpp>


namespace npl
{


template< typename Alloc >
struct allocator_traits ;

template< typename T >
class allocator ;

template< typename T >
using default_allocator_t = allocator< T > ;


#define NPL_ALLOCATOR_TRAITS_HAS(NAME, PROPERTY)                                                              \
        template< typename T, typename = void > struct NAME : false_type {} ;                                  \
        template< typename T >                  struct NAME< T, void_t< typename T:: PROPERTY > > : true_type {}

//=====================================================================
//      _pointer
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_pointer, pointer );
template< typename T, typename Alloc,
          typename RawAlloc = remove_reference_t< Alloc >,
          bool = _has_pointer< RawAlloc >::value >
struct _pointer
{
        using type = typename RawAlloc::pointer ;
};
template< typename T, typename Alloc, typename RawAlloc >
struct _pointer< T, Alloc, RawAlloc, false > { using type = T * ; };

//=====================================================================
//      _const_pointer
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_const_pointer, const_pointer );
template< typename T, typename Ptr, typename Alloc,
          typename RawAlloc = remove_reference_t< Alloc >,
          bool = _has_const_pointer< RawAlloc >::value >
struct _const_pointer
{
        using type = typename RawAlloc::const_pointer ;
};
template< typename T, typename Ptr, typename Alloc, typename RawAlloc >
struct _const_pointer< T, Ptr, Alloc, RawAlloc, false >
{
        using type = typename std::pointer_traits< Ptr >::template rebind< T const > ;
};

//=====================================================================
//      _void_pointer
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_void_pointer, void_pointer );
template< typename Ptr, typename Alloc,
          bool = _has_void_pointer< Alloc >::value >
struct _void_pointer
{
        using type = typename Alloc::void_pointer ;
};
template< typename Ptr, typename Alloc >
struct _void_pointer< Ptr, Alloc, false >
{
        using type = typename std::pointer_traits< Ptr >::template rebind< void > ;
};

//=====================================================================
//      _const_void_pointer
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_const_void_pointer, const_void_pointer );
template< typename Ptr, typename Alloc,
          bool = _has_const_void_pointer< Alloc >::value >
struct _const_void_pointer
{
        using type = typename Alloc::const_void_pointer ;
};
template< typename Ptr, typename Alloc >
struct _const_void_pointer< Ptr, Alloc, false >
{
        using type = typename std::pointer_traits< Ptr >::template rebind< void const > ;
};

//=====================================================================
//      _size_type
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_size_type, size_type );
template< typename Alloc, typename DiffType, bool = _has_size_type< Alloc >::value >
struct _size_type : std::make_unsigned< DiffType > {} ;
template< typename Alloc, typename DiffType >
struct _size_type< Alloc, DiffType, true >
{
        using type = typename Alloc::size_type ;
};

//=====================================================================
//      _alloc_traits_difference_type
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_alloc_traits_difference_type, difference_type );
template< typename Alloc, typename Ptr, bool = _has_alloc_traits_difference_type< Alloc >::value >
struct _alloc_traits_difference_type
{
        using type = typename std::pointer_traits< Ptr >::difference_type ;
};
template< typename Alloc, typename Ptr >
struct _alloc_traits_difference_type< Alloc, Ptr, true >
{
        using type = typename Alloc::difference_type ;
};

//=====================================================================
//      _propagate_on_container_copy_assignment
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_propagate_on_container_copy_assignment, propagate_on_container_copy_assignment );
template< typename Alloc, bool = _has_propagate_on_container_copy_assignment< Alloc >::value >
struct _propagate_on_container_copy_assignment : false_type {} ;
template< typename Alloc >
struct _propagate_on_container_copy_assignment< Alloc, true >
{
        using type = typename Alloc::propagate_on_container_copy_assignment ;
};

//=====================================================================
//      _propagate_on_container_move_assignment
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_propagate_on_container_move_assignment, propagate_on_container_move_assignment );
template< typename Alloc, bool = _has_propagate_on_container_move_assignment< Alloc >::value >
struct _propagate_on_container_move_assignment : false_type {} ;
template< typename Alloc >
struct _propagate_on_container_move_assignment< Alloc, true >
{
        using type = typename Alloc::propagate_on_container_move_assignment ;
};

//=====================================================================
//      _propagate_on_container_swap
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_propagate_on_container_swap, propagate_on_container_swap );
template< typename Alloc, bool = _has_propagate_on_container_swap< Alloc >::value >
struct _propagate_on_container_swap : false_type {} ;
template< typename Alloc >
struct _propagate_on_container_swap< Alloc, true >
{
        using type = typename Alloc::propagate_on_container_swap ;
};

//=====================================================================
//      _is_always_equal
//=====================================================================

NPL_ALLOCATOR_TRAITS_HAS( _has_is_always_equal, is_always_equal );
template< typename Alloc, bool = _has_is_always_equal< Alloc >::value >
struct _is_always_equal : is_empty< Alloc > {} ;
template< typename Alloc >
struct _is_always_equal< Alloc, true >
{
        using type = typename Alloc::is_always_equal ;
};

//=====================================================================
//      _allocator_traits_rebind
//=====================================================================

template< typename T, typename U, typename = void >
struct _has_rebind_other : false_type {} ;
template< typename T, typename U >
struct _has_rebind_other< T, U, void_t< typename T::template rebind< U >::other > > : true_type {} ;

template< typename T, typename U, bool = _has_rebind_other< T, U >::value >
struct _allocator_traits_rebind
{
        static_assert( _has_rebind_other< T, U >::value, "allocator has to implement rebind!" );
        using type = typename T::template rebind< U >::other ;
};
template< template< typename, typename... > typename Alloc, typename T, typename... Args, typename U >
struct _allocator_traits_rebind< Alloc< T, Args... >, U, true >
{
        using type = typename Alloc< T, Args... >::template rebind< U >::other ;
};
template< template< typename, typename... > typename Alloc, typename T, typename... Args, typename U >
struct _allocator_traits_rebind< Alloc< T, Args... >, U, false >
{
        using type = Alloc< U, Args... >;
};

template< typename Alloc, typename T >
using _allocator_traits_rebind_t = typename _allocator_traits_rebind< Alloc, T >::type ;

//=====================================================================
//      _has_allocate_hint
//=====================================================================

template< typename Alloc, typename SizeType, typename ConstVoidPtr, typename = void >
struct _has_allocate_hint : false_type {} ;

template< typename Alloc, typename SizeType, typename ConstVoidPtr >
struct _has_allocate_hint< Alloc, SizeType, ConstVoidPtr, decltype(
                ( void ) declval< Alloc >().allocate( declval< SizeType >(), declval< ConstVoidPtr>() )
                ) > : true_type {} ;

//=====================================================================
//      _has_construct
//=====================================================================

template< typename Alloc, typename... Args,
          typename = decltype( declval< Alloc >().construct( declval< Args >()... ) ) >
static true_type _test_has_construct( int ) ;
template< typename Alloc, typename... >
static false_type _test_has_construct( ... ) ;

template< typename Alloc, typename... Args >
struct _has_construct : decltype( _test_has_construct< Alloc, Args... >( 0 ) ) {} ;

template< typename Alloc, typename... Args >
inline constexpr bool _has_construct_v = _has_construct< Alloc, Args... >::value ;

//=====================================================================
//      _has_destroy
//=====================================================================

template< typename Alloc, typename Ptr, typename = void >
struct _has_destroy : false_type {} ;

template< typename Alloc, typename Ptr >
struct _has_destroy< Alloc, Ptr, decltype(
                ( void ) declval< Alloc >().destroy( declval< Ptr >() )
                ) > : true_type {} ;

//=====================================================================
//      _has_max_size
//=====================================================================

template< typename Alloc, typename = void >
struct _has_max_size : false_type {} ;
template< typename Alloc >
struct _has_max_size< Alloc, decltype( ( void ) declval< Alloc & >().max_size() ) > : true_type {} ;

//=====================================================================
//      _has_select_on_container_copy_construction
//=====================================================================

template< typename Alloc, typename = void >
struct _has_select_on_container_copy_construction : false_type {} ;

template< typename Alloc >
struct _has_select_on_container_copy_construction< Alloc, decltype(
                ( void ) declval< Alloc >().select_on_container_copy_construction()
                ) > : true_type {} ;


template< typename Alloc >
struct allocator_traits
{
        using     allocator_type = Alloc ;
        using         value_type = typename allocator_type::value_type ;
        using            pointer = typename            _pointer< value_type,          allocator_type >::type ;
        using      const_pointer = typename      _const_pointer< value_type, pointer, allocator_type >::type ;
        using       void_pointer = typename       _void_pointer<             pointer, allocator_type >::type ;
        using const_void_pointer = typename _const_void_pointer<             pointer, allocator_type >::type ;

        using difference_type = typename _alloc_traits_difference_type< allocator_type,         pointer >::type ;
        using       size_type = typename                    _size_type< allocator_type, difference_type >::type ;

        using is_always_equal                        = typename _is_always_equal                       < allocator_type >::type ;
        using propagate_on_container_swap            = typename _propagate_on_container_swap           < allocator_type >::type ;
        using propagate_on_container_copy_assignment = typename _propagate_on_container_copy_assignment< allocator_type >::type ;
        using propagate_on_container_move_assignment = typename _propagate_on_container_move_assignment< allocator_type >::type ;

        template< typename T >
        using rebind_alloc = _allocator_traits_rebind_t< allocator_type, T > ;
        template< typename T >
        using rebind_traits = allocator_traits< rebind_alloc< T > > ;

        NPL_NODISCARD inline constexpr static
        pointer allocate ( allocator_type & _a_, size_type _n_ )
        {
                return _a_.allocate( _n_ );
        }

        template< typename A = Alloc,
                  typename = enable_if_t< _has_allocate_hint< A, size_type, const_void_pointer >::value > >
        NPL_NODISCARD inline constexpr static
        pointer allocate ( allocator_type & _a_, size_type _n_, const_void_pointer _hint_ )
        {
                return _a_.allocate( _n_, _hint_ );
        }
        template< typename A = Alloc, typename = void,
                  typename = enable_if_t< !_has_allocate_hint< A, size_type, const_void_pointer >::value > >
        NPL_NODISCARD inline constexpr static
        pointer allocate ( allocator_type & _a_, size_type _n_, const_void_pointer )
        {
                return _a_.allocate( _n_ );
        }

        inline constexpr static
        void deallocate ( allocator_type & _a_, pointer _ptr_, size_type _n_ ) noexcept
        {
                _a_.deallocate( _ptr_, _n_ );
        }

        template< typename T, typename... Args,
                  typename = enable_if_t< _has_construct< allocator_type, T *, Args... >::value > >
        inline constexpr static
        void construct ( allocator_type & _a_, T * _ptr_, Args&&... _args_ )
        {
                _a_.construct( _ptr_, NPL_FWD( _args_ )... );
        }
        template< typename T, typename... Args, typename = void,
                  typename = enable_if_t< !_has_construct< allocator_type, T *, Args... >::value > >
        inline constexpr static
        void construct ( allocator_type &, T * _ptr_, Args&&... _args_ )
        {
                ::new ( ( void * ) _ptr_ ) T( NPL_FWD( _args_ )... );
        }

        template< typename T,
                  typename = enable_if_t< _has_destroy< allocator_type, T * >::value > >
        inline constexpr static
        void destroy ( allocator_type & _a_, T * _ptr_ )
        {
                _a_.destroy( _ptr_ );
        }
        template< typename T, typename = void,
                  typename = enable_if_t< !_has_destroy< allocator_type, T * >::value > >
        inline constexpr static
        void destroy ( allocator_type &, T * _ptr_ )
        {
                _ptr_->~T();
        }

        template< typename A = Alloc,
                  typename = enable_if_t< _has_max_size< A const >::value > >
        inline constexpr static
        size_type max_size ( allocator_type const & _a_ ) noexcept
        {
                return _a_.max_size();
        }
        template< typename A = Alloc, typename = void,
                  typename = enable_if_t< !_has_max_size< A const >::value > >
        inline constexpr static
        size_type max_size ( allocator_type const & ) noexcept
        {
                return std::numeric_limits< size_type >::max() / sizeof( value_type );
        }

        template< typename A = Alloc,
                  typename = enable_if_t< _has_select_on_container_copy_construction< A const >::value > >
        inline constexpr static
        allocator_type select_on_container_copy_construction ( allocator_type const & _a_ )
        {
                return _a_.select_on_container_copy_construction();
        }

        template< typename A = Alloc, typename = void,
                  typename = enable_if_t< !_has_select_on_container_copy_construction< A const >::value > >
        inline constexpr static
        allocator_type select_on_container_copy_construction ( allocator_type const & _a_ )
        {
                return _a_;
        }
};


template< typename Traits, typename T >
using _rebind_alloc = typename Traits::template rebind_alloc< T > ;

//=====================================================================
//      is_allocator
//=====================================================================

template< typename Alloc, typename = void, typename = void >
struct is_allocator : false_type {} ;

template< typename Alloc >
struct is_allocator
<
        Alloc,
        typename void_t< typename Alloc::value_type >::type,
        typename void_t< decltype( declval< Alloc & >().allocate( size_t( 0 ) ) ) >::type
>
        : true_type {} ;

//=====================================================================
//      is_default_allocator
//=====================================================================

template< typename T >
struct is_default_allocator : false_type {} ;

template< typename T >
struct is_default_allocator< default_allocator_t< T > > : true_type {} ;

template< typename T >
inline constexpr bool is_default_allocator_v = is_default_allocator< T >::value ;

//=====================================================================
//      is_cpp17_move_insertable
//=====================================================================

template< typename Alloc,
          bool = _has_construct_v
                 <
                        Alloc,
                        typename Alloc::value_type *,
                        typename Alloc::value_type &&
                 >
                 &&
                 !is_default_allocator_v< Alloc >
>
struct is_cpp17_move_insertable ;

template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, true > : true_type {} ;
template< typename Alloc >
struct is_cpp17_move_insertable< Alloc, false > : is_move_constructible< typename Alloc::value_type > {} ;

template< typename Alloc >
inline constexpr bool is_cpp17_move_insertable_v = is_cpp17_move_insertable< Alloc >::value ;

//=====================================================================
//      noexcept_move_assign_container
//=====================================================================

template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
struct noexcept_move_assign_container : bool_constant
                                        <
                                                Traits::propagate_on_container_move_assignment::value ||
                                                Traits::is_always_equal::value
                                        > {} ;
template< typename Alloc, typename Traits = std::allocator_traits< Alloc > >
inline constexpr bool noexcept_move_assign_container_v = noexcept_move_assign_container< Alloc, Traits >::value ;


} // namespace npl
