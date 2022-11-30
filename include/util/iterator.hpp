//
//
//      natprolib
//      iterator.hpp
//

#pragma once

#include "util.hpp"
#include "traits.hpp"


namespace npl
{


struct random_access_iterator_tag {};

template< typename Iter, bool > struct _iterator_traits_impl {};

template< typename Iter >
struct _iterator_traits_impl< Iter, true >
{
        using       difference_type = typename Iter::difference_type;
        using            value_type = typename Iter::value_type;
        using               pointer = typename Iter::pointer;
        using             reference = typename Iter::reference;
        using     iterator_category = typename Iter::iterator_category;
        using npl_iterator_category = typename Iter::npl_iterator_category;
};

template< typename Iter, bool > struct _iterator_traits {};

template< typename Iter >
struct _iterator_traits< Iter, true >
        : _iterator_traits_impl
          <
                Iter,
                std::is_convertible_v< typename Iter::npl_iterator_category, random_access_iterator_tag >
          >
{};

template< typename T >
struct _has_npl_iterator_typedefs
{
private:
        struct _two { char _lx; char _lxx; };
        template< typename U > static _two _test( ... );
        template< typename U > static char _test( typename _void_t< typename U::npl_iterator_category >::type* = 0,
                                                  typename _void_t< typename U::iterator_category     >::type* = 0,
                                                  typename _void_t< typename U::difference_type       >::type* = 0,
                                                  typename _void_t< typename U::value_type            >::type* = 0,
                                                  typename _void_t< typename U::reference             >::type* = 0,
                                                  typename _void_t< typename U::pointer               >::type* = 0
        );
public:
        static bool const value = sizeof( _test< T >( 0, 0, 0, 0, 0, 0 ) ) == 1;
};

template< typename Iter >
inline constexpr bool _has_npl_iterator_typedefs_v = _has_npl_iterator_typedefs< Iter >::value;

template< typename Iter >
struct iterator_traits
        : _iterator_traits< Iter, _has_npl_iterator_typedefs_v< Iter > >
{
        using _primary_template = iterator_traits;
};

template< typename T >
struct has_npl_iterator_category
{
private:
        struct _two { char _lx; char _lxx; };
        template< typename U > static _two _test( ... );
        template< typename U > static char _test( typename _void_t< typename U::npl_iterator_category >::type* = 0 );
public:
        static bool const value = sizeof( _test< T >( 0 ) ) == 1;
};

template< typename T >
inline constexpr bool has_npl_iterator_category_v = has_npl_iterator_category< T >::value;

template< typename T, typename U, bool = has_npl_iterator_category< iterator_traits< T > >::value >
struct has_same_npl_iterator_category
        : public std::integral_constant< bool, std::is_same_v< typename iterator_traits< T >::npl_iterator_category, U > > {};

template< typename T, typename U >
struct has_same_npl_iterator_category< T, U, false > : public std::false_type {};

template< typename T >
struct iterator_traits< T* >
{
        using       difference_type = std::ptrdiff_t;
        using            value_type = std::remove_cv_t< T >;
        using               pointer = T *;
        using             reference = T &;
        using     iterator_category = std::random_access_iterator_tag;
        using npl_iterator_category =      random_access_iterator_tag;
};


template< bool C, typename T >
class iterator
{
        friend class iterator< !C, T >;
public:
        using                 _self = iterator;
        using            value_type = T;
        using       difference_type = std::ptrdiff_t;
        using               pointer = std::conditional_t< C, value_type const *, value_type * >;
        using             reference = std::conditional_t< C, value_type const &, value_type & >;
        using         const_pointer = value_type const *;
        using       const_reference = value_type const &;
        using     iterator_category = std::random_access_iterator_tag;
        using npl_iterator_category =      random_access_iterator_tag;

        constexpr pointer     operator-> (     ) const noexcept {   return std::addressof( operator*() ); }
        constexpr reference   operator*  (     ) const noexcept {                           return *ptr_; }
        constexpr iterator  & operator++ (     )       noexcept {                   ptr_++; return *this; }
        constexpr iterator  & operator-- (     )       noexcept {                   ptr_--; return *this; }
        constexpr iterator    operator++ ( int )       noexcept { auto it = *this; ++*this; return    it; }
        constexpr iterator    operator-- ( int )       noexcept { auto it = *this; --*this; return    it; }

        template< bool R >
        constexpr
        bool operator== ( iterator< R, T > const & rhs ) const noexcept
        { return ptr_ == rhs.ptr_; }

        template< bool R >
        constexpr
        bool operator!= ( iterator< R, T > const & rhs ) const noexcept
        { return !operator==( rhs ); }

        template< bool R >
        constexpr
        difference_type operator+ ( iterator< R, T > const & rhs ) const noexcept
        { return static_cast< difference_type >( ptr_ + rhs.ptr_ ); }

        template< bool R >
        constexpr
        difference_type operator- ( iterator< R, T > const & rhs ) const noexcept
        { return static_cast< difference_type >( ptr_ - rhs.ptr_ ); }

        template< bool R >
        constexpr
        iterator & operator+= ( iterator< R, T > const & rhs ) noexcept
        { ptr_ += rhs.ptr_; return *this; }

        template< bool R >
        constexpr
        iterator & operator-= ( iterator< R, T > const & rhs ) noexcept
        { ptr_ -= rhs.ptr_; return *this; }

        constexpr
        iterator & operator+= ( difference_type const _offset_ ) noexcept
        { ptr_ += _offset_; return *this; }

        constexpr
        iterator & operator-= ( difference_type const _offset_ ) noexcept
        { ptr_ -= _offset_; return *this; }

        constexpr
        iterator operator+ ( difference_type const _offset_ ) const noexcept
        { return iterator( ptr_ + _offset_ ); }

        constexpr
        iterator operator- ( difference_type const _offset_ ) const noexcept
        { return iterator( ptr_ - _offset_ ); }

        constexpr
        operator iterator< true, T > () const noexcept
        { return iterator< true, T >{ ptr_ }; }

        /*
        constexpr
        std::enable_if_t
        <
                is_ptr_to_const_v< pointer >,
                pointer
        >
        raw () const noexcept
        { return ptr_; }

        constexpr
        std::enable_if_t
        <
                !is_ptr_to_const_v< pointer >,
                pointer
        >
        raw () noexcept
        { return ptr_; }
        */

        constexpr pointer raw () noexcept
        { return ptr_; }

        constexpr const_pointer raw () const noexcept
        { return ptr_; }

protected:
        pointer ptr_;

        explicit constexpr iterator ( pointer _ptr_ ) : ptr_( _ptr_ ) {}
};

template< typename Iter >
class reverse_iter
{
public:
        using                 _base = Iter;
        using            value_type = typename _base::value_type;
        using       difference_type = typename _base::difference_type;
        using               pointer = typename _base::pointer;
        using         const_pointer = pointer;
        using             reference = typename _base::reference;
        using       const_reference = reference;
        using     iterator_category = typename _base::iterator_category;

        constexpr reference       operator*  (     ) const noexcept { auto it = iter_;          return *--it; }
        constexpr reverse_iter  & operator++ (     )       noexcept {                 iter_-- ; return *this; }
        constexpr reverse_iter  & operator-- (     )       noexcept {                 iter_++ ; return *this; }
        constexpr reverse_iter    operator++ ( int )       noexcept { auto it = *this; --*this; return    it; }
        constexpr reverse_iter    operator-- ( int )       noexcept { auto it = *this; ++*this; return    it; }

        constexpr bool operator== ( reverse_iter const & _other_ ) const noexcept
        { return iter_ == _other_.iter_; }

        constexpr bool operator!= ( reverse_iter const & _other_ ) const noexcept
        { return !operator==( _other_ ); }

        constexpr difference_type operator+ ( reverse_iter< Iter > const & _other_ ) const noexcept
        { return iter_ - _other_.iter_; }

        constexpr difference_type operator- ( reverse_iter< Iter > const & _other_ ) const noexcept
        { return _other_.iter_ - iter_; }

        constexpr reverse_iter & operator+= ( reverse_iter< Iter > const & _other_ ) noexcept
        { iter_ -= _other_.iter_; return *this; }

        constexpr reverse_iter & operator-= ( reverse_iter< Iter > const & _other_ ) noexcept
        { iter_ += _other_.iter_; return *this; }

        constexpr reverse_iter operator+ ( difference_type const _count_ ) const noexcept
        { return reverse_iter( iter_ - _count_ ); }

        constexpr reverse_iter operator- ( difference_type const _count_ ) const noexcept
        { return reverse_iter( iter_ + _count_ ); }

        constexpr reverse_iter & operator+= ( difference_type const _count_ ) noexcept
        { iter_ -= _count_; return *this; }

        constexpr reverse_iter & operator-= ( difference_type const _count_ ) noexcept
        { iter_ += _count_; return *this; }

        constexpr reverse_iter ( Iter const & _iter_ ) : iter_( _iter_ ) {}
protected:
        Iter iter_;
};


} // namespace npl
