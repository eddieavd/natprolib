//
//
//      natprolib
//      move_iterator.hpp
//

#pragma once

#include <iterator.hpp>


namespace npl
{


template< typename Iter >
class move_iterator
{
public:
        using iterator_type = Iter ;
        using iterator_category = conditional<
                                                is_random_access_iterator_v< Iter >,
                                                random_access_iterator_tag,
                                                typename iterator_traits< Iter >::iterator_category
                                             > ;
        using      value_type = typename iterator_traits< iterator_type >::     value_type ;
        using difference_type = typename iterator_traits< iterator_type >::difference_type ;

        using pointer = iterator_type ;

        using _reference = typename iterator_traits< iterator_type >::reference ;
        using  reference = conditional_t<
                                        is_reference< _reference >::value,
                                        remove_reference_t< _reference > &&,
                                        _reference
                                        > ;

        constexpr explicit move_iterator ( iterator_type _iter_ )                                              : current_( NPL_MOVE( _iter_ ) ) {}
        constexpr          move_iterator (                      ) requires is_constructible_v< iterator_type > : current_(                    ) {}

        template< typename U,
                  typename = enable_if_t< !is_same_v< U, iterator_type > && is_convertible_v< U const &, iterator_type > > >
        constexpr move_iterator ( move_iterator< U > const & _other_ ) : current_( _other_.base() ) {}

        template< typename U,
                  typename = enable_if_t<
                                                !is_same_v< U, iterator_type > &&
                                                is_convertible_v< U const &, iterator_type > &&
                                                is_assignable_v< iterator_type &, U const & >
                                        > >
        constexpr move_iterator & operator= ( move_iterator< U > const & _other_ )
        {
                current_ = _other_.base();
                return *this;
        }

        constexpr iterator_type const & base () const &  noexcept { return           current_   ; }
        constexpr iterator_type         base ()       &&          { return NPL_MOVE( current_ ) ; }

        constexpr pointer operator-> () const { return current_; }

        constexpr reference operator* () const { return static_cast< reference >( *current_ ); }
        constexpr reference operator[] ( difference_type _index_ ) const { return static_cast< reference >( current_[ _index_ ] ); }


        constexpr move_iterator & operator++ () { ++current_; return *this; }
        constexpr move_iterator & operator-- () { --current_; return *this; }

        constexpr move_iterator operator++ ( int ) { move_iterator tmp( *this ); ++current_; return tmp; }
        constexpr move_iterator operator-- ( int ) { move_iterator tmp( *this ); --current_; return tmp; }

        constexpr move_iterator operator+ ( difference_type _n_ ) const { return move_iterator( current_ + _n_ ); }
        constexpr move_iterator operator- ( difference_type _n_ ) const { return move_iterator( current_ - _n_ ); }

        constexpr move_iterator & operator+= ( difference_type _n_ ) const { current_ += _n_; return *this; }
        constexpr move_iterator & operator-= ( difference_type _n_ ) const { current_ -= _n_; return *this; }
private:
        template< typename It2 > friend class move_iterator ;

        iterator_type current_ ;
};

template< typename Iter1, typename Iter2 >
inline constexpr bool operator== ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() == _y_.base();
}
template< typename Iter1, typename Iter2 >
inline constexpr bool operator!= ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() != _y_.base();
}
template< typename Iter1, typename Iter2 >
inline constexpr bool operator< ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() < _y_.base();
}
template< typename Iter1, typename Iter2 >
inline constexpr bool operator> ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() > _y_.base();
}
template< typename Iter1, typename Iter2 >
inline constexpr bool operator<= ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() <= _y_.base();
}
template< typename Iter1, typename Iter2 >
inline constexpr bool operator>= ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
{
        return _x_.base() >= _y_.base();
}

template< typename Iter1, typename Iter2 >
inline constexpr auto operator- ( move_iterator< Iter1 > const & _x_, move_iterator< Iter2 > const & _y_ )
        -> decltype( _x_.base() - _y_.base() )
{
        return _x_.base() - _y_.base();
}
template< typename Iter >
inline constexpr move_iterator< Iter > operator+ ( typename move_iterator< Iter >::difference_type _n_, move_iterator< Iter > const & _x_ )
{
        return move_iterator< Iter >( _x_.base() + _n_ );
}

template< typename Iter >
inline constexpr move_iterator< Iter > make_move_iterator ( Iter _iter_ )
{
        return move_iterator< Iter >( NPL_MOVE( _iter_ ) );
}


} // namespace npl
