//
//
//      natprolib
//      iter_traits.hpp
//

#pragma once

#include <_traits/base_traits.hpp>


namespace npl
{


template< typename Iter >
struct iterator_traits ;

struct         input_iterator_tag                                     {} ;
struct        output_iterator_tag                                     {} ;
struct       forward_iterator_tag : public         input_iterator_tag {} ;
struct bidirectional_iterator_tag : public       forward_iterator_tag {} ;
struct random_access_iterator_tag : public bidirectional_iterator_tag {} ;

template< typename Iter >
struct _iter_traits_cache
{
        using type = conditional<
                                        is_primary_template< iterator_traits< Iter > >::value,
                                        Iter,
                                        iterator_traits< Iter >
                                > ;
};
template< typename Iter >
using ITER_TRAITS = typename _iter_traits_cache< Iter >::type ;

struct _iter_concept_concept_test
{
        template< typename Iter >
        using _apply = typename ITER_TRAITS< Iter >::iterator_concept ;
};
struct _iter_concept_category_test
{
        template< typename Iter >
        using _apply = typename ITER_TRAITS< Iter >::iterator_category ;
};
struct _iter_concept_random_fallback
{
        template< typename Iter >
        using _apply = enable_if_t<
                                        is_primary_template_v< iterator_traits< Iter > >,
                                        random_access_iterator_tag
                                  > ;
};

template< typename Iter, typename Tester >
struct _test_iter_concept : is_valid_expansion< Tester::template _apply, Iter >, Tester {} ;

template< typename Iter >
struct _iter_concept_cache
{
        using type = disjunction< _test_iter_concept< Iter, _iter_concept_concept_test    >,
                                  _test_iter_concept< Iter, _iter_concept_category_test   >,
                                  _test_iter_concept< Iter, _iter_concept_random_fallback >
                                > ;
};

template< typename Iter >
using ITER_CONCEPT = typename _iter_concept_cache< Iter >::type::template _apply< Iter > ;

template< typename T >
struct _has_iterator_typedefs
{
private:
        template< typename U > static false_type _test ( ... ) ;
        template< typename U > static  true_type _test ( void_t< typename U::iterator_category >* = nullptr,
                                                         void_t< typename U::  difference_type >* = nullptr,
                                                         void_t< typename U::       value_type >* = nullptr,
                                                         void_t< typename U::        reference >* = nullptr,
                                                         void_t< typename U::          pointer >* = nullptr ) ;
public:
        static const bool value = decltype( _test< T >( 0, 0, 0, 0, 0 ) )::value ;
};

template< typename T >
struct _has_iterator_category
{
private:
        template< typename U > static false_type _test ( ... ) ;
        template< typename U > static  true_type _test ( typename U::iterator_category* = nullptr ) ;
public:
        static const bool value = decltype( _test< T >( nullptr ) )::value ;
};

template< typename T >
inline constexpr bool _has_iterator_category_v = _has_iterator_category< T >::value ;

template< typename T, typename U, bool = _has_iterator_category_v< iterator_traits< T > > >
struct has_same_iterator_category
        : public bool_constant< is_same_v< typename iterator_traits< T >::iterator_category, U > > {} ;

template< typename T >
struct _has_iterator_concept
{
private:
        template< typename U > static false_type _test ( ... ) ;
        template< typename U > static  true_type _test ( typename U::iterator_concept* = nullptr ) ;
public:
        static const bool value = decltype( _test< T >( nullptr ) )::value ;
};


template< typename Iter, bool > struct _iterator_traits      {} ;
template< typename Iter, bool > struct _iterator_traits_impl {} ;

template< typename Iter >
struct _iterator_traits_impl< Iter, true >
{
        using   difference_type = typename Iter::      difference_type ;
        using        value_type = typename Iter::           value_type ;
        using           pointer = typename Iter::              pointer ;
        using         reference = typename Iter::            reference ;
        using iterator_category = typename Iter::    iterator_category ;
};

template< typename Iter >
struct _iterator_traits< Iter, true >
        : _iterator_traits_impl
          <
                Iter,
                is_convertible< typename Iter::iterator_category,  input_iterator_tag >::value ||
                is_convertible< typename Iter::iterator_category, output_iterator_tag >::value
          >
{} ;

template< typename Iter >
struct iterator_traits
        : _iterator_traits< Iter, _has_iterator_typedefs< Iter >::value >
{
        using _primary_template = iterator_traits ;
};

template< typename T >
struct iterator_traits< T * >
{
        using   difference_type =                  ptrdiff_t ;
        using        value_type =           remove_cv_t< T > ;
        using           pointer =               value_type * ;
        using         reference =               value_type & ;
        using iterator_category = random_access_iterator_tag ;
};


//=====================================================================
//      iterator_category
//=====================================================================

template< typename Iterator >
using _iter_value_type = typename iterator_traits< Iterator >::value_type ;

template< typename T >
struct has_iterator_category
{
private:
        struct _two { char _lx; char _lxx; };
        template< typename U > static _two _test( ... ) ;
        template< typename U > static char _test( typename U::iterator_category* = 0 ) ;
public:
        static bool const value = sizeof( _test< T >( 0 ) ) == 1 ;
};

template< typename T, typename U, bool = has_iterator_category< iterator_traits< T > >::value >
struct has_iterator_category_convertible_to
        : bool_constant< is_convertible_v< typename iterator_traits< T >::iterator_category, U > > {} ;

template< typename T, typename U >
struct has_iterator_category_convertible_to< T, U, false > : false_type {} ;


template< typename T >
struct is_at_least_input_iterator : has_iterator_category_convertible_to< T, input_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_input_iterator_v = is_at_least_input_iterator< T >::value ;

template< typename T >
struct is_at_least_forward_iterator : has_iterator_category_convertible_to< T, forward_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_forward_iterator_v = is_at_least_forward_iterator< T >::value ;

template< typename T >
struct is_at_least_bidirectional_iterator : has_iterator_category_convertible_to< T, bidirectional_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_bidirectional_iterator_v = is_at_least_bidirectional_iterator< T >::value ;

template< typename T >
struct is_at_least_random_access_iterator : has_iterator_category_convertible_to< T, random_access_iterator_tag > {} ;
template< typename T >
inline constexpr bool is_at_least_random_access_iterator_v = is_at_least_random_access_iterator< T >::value ;

template< typename T >
struct is_exactly_random_access_iterator : conjunction_t< is_at_least_random_access_iterator< T >,
                                                            is_convertible< random_access_iterator_tag, typename iterator_traits< T >::iterator_category > > {} ;
template< typename T >
inline constexpr bool is_exactly_random_access_iterator_v = is_exactly_random_access_iterator< T >::value ;


template< typename InputIter >
inline constexpr
typename iterator_traits< InputIter >::difference_type
_distance ( InputIter _first_, InputIter _last_, input_iterator_tag )
{
        typename iterator_traits< InputIter >::difference_type dist( 0 );
        for( ; _first_ != _last_; ++_first_ )
        {
                ++dist;
        }
        return dist;
}

template< typename RandIter >
inline constexpr
typename iterator_traits< RandIter >::difference_type
_distance ( RandIter _first_, RandIter _last_, random_access_iterator_tag )
{
        return _last_ - _first_;
}

template< typename Iter >
inline constexpr
typename iterator_traits< Iter >::difference_type
distance ( Iter _first_, Iter _last_ )
{
        return _distance( _first_, _last_, typename iterator_traits< Iter >::iterator_category() );
}

template< typename InputIter >
constexpr void _advance ( InputIter & _iter_, typename iterator_traits< InputIter >::difference_type _dist_, input_iterator_tag )
{
        for( ; _dist_ > 0; --_dist_ )
        {
                ++_iter_;
        }
}

template< typename BiDirIter >
constexpr void _advance ( BiDirIter & _iter_, typename iterator_traits< BiDirIter >::difference_type _dist_, bidirectional_iterator_tag )
{
        if( _dist_ >= 0 )
        {
                for( ; _dist_ > 0; --_dist_ )
                {
                        ++_iter_;
                }
        }
        else
        {
                for( ; _dist_ < 0; ++_dist_ )
                {
                        --_iter_;
                }
        }
}

template< typename RandIter >
constexpr void _advance ( RandIter & _iter_, typename iterator_traits< RandIter >::difference_type _dist_, random_access_iterator_tag )
{
        _iter_ += _dist_;
}

template< typename Iter, typename Dist,
          typename IntegralDist = decltype( convert_to_integral( declval< Dist >() ) ),
          typename = enable_if_t< is_integral_v< IntegralDist > > >
constexpr void advance ( Iter & _iter_, Dist _dist_ )
{
        using diff_t = typename iterator_traits< Iter >::difference_type ;

        diff_t dist = static_cast< diff_t >( convert_to_integral( _dist_ ) );
        NPL_ASSERT( dist >= 0 || is_at_least_bidirectional_iterator_v< Iter >,
                        "attempt to advance( it, n ) with negative n on non-bidirectional iterator" );
        _advance( _iter_, _dist_, typename iterator_traits< Iter >::iterator_category() );
}


} // namespace npl
