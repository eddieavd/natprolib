//
//
//      natprolib
//      iterator.hpp
//

#pragma once

#include <util.hpp>
#include <mem.hpp>
#include <_iter/iter_traits.hpp>


namespace npl
{


template< bool C, typename T >
class iterator
{
        friend class iterator< !C, T >;
public:
        using             _self = iterator;
        using        value_type = T;
        using   difference_type = ptrdiff_t;
        using           pointer = conditional_t< C, value_type const *, value_type * >;
        using         reference = conditional_t< C, value_type const &, value_type & >;
        using     const_pointer = value_type const *;
        using   const_reference = value_type const &;
        using iterator_category = random_access_iterator_tag;

        constexpr pointer     operator-> (     ) const noexcept {   return mem::addressof( operator*() ); }
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
        using             _base = Iter;
        using        value_type = typename _base::value_type;
        using   difference_type = typename _base::difference_type;
        using           pointer = typename _base::pointer;
        using     const_pointer = pointer;
        using         reference = typename _base::reference;
        using   const_reference = reference;
        using iterator_category = typename _base::iterator_category;

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
