//
//
//      natprolib
//      split_buffer.hpp
//

#pragma once

#include <algorithm>

#include "util.hpp"
#include "traits.hpp"
#include "mem.hpp"


namespace npl
{


template< typename T, typename Allocator = std::allocator< T > >
struct split_buffer
{
private:
        split_buffer             ( split_buffer const & );
        split_buffer & operator= ( split_buffer const & );
public:
        using      value_type = T;
        using  allocator_type = Allocator;
        using       _alloc_rr = remove_reference_t< allocator_type >;
        using   _alloc_traits = std::allocator_traits< _alloc_rr >;
        using       reference = value_type &;
        using const_reference = value_type const &;
        using       size_type = typename _alloc_traits::size_type;
        using difference_type = typename _alloc_traits::difference_type;
        using         pointer = typename _alloc_traits::pointer;
        using   const_pointer = typename _alloc_traits::const_pointer;
        using        iterator = pointer;
        using  const_iterator = const_pointer;

        pointer        first_  ;
        pointer        begin_  ;
        pointer        end_    ;
        pointer        end_cap_;
        allocator_type alloc_  ;

        using _alloc_ref       = add_lvalue_reference_t< allocator_type >;
        using _alloc_const_ref = add_lvalue_reference_t< allocator_type >;

        _alloc_rr       & _alloc   ()       noexcept { return alloc_  ; }
        _alloc_rr const & _alloc   () const noexcept { return alloc_  ; }
        pointer         & _end_cap ()       noexcept { return end_cap_; }
        pointer   const & _end_cap () const noexcept { return end_cap_; }

        split_buffer () noexcept( is_nothrow_default_constructible_v< allocator_type > );

        explicit split_buffer ( _alloc_rr       & _alloc_ );
        explicit split_buffer ( _alloc_rr const & _alloc_ );

        split_buffer ( size_type const _capacity_, size_type const _start_, _alloc_rr & _alloc_ );

        ~split_buffer ();

        split_buffer ( split_buffer && _other_ )
                noexcept( is_nothrow_move_constructible_v< allocator_type > );
        split_buffer ( split_buffer && _other_, _alloc_rr const & _alloc_ );

        split_buffer & operator= ( split_buffer && _other_ )
                noexcept( ( _alloc_traits::propagate_on_container_move_assignment::value &&
                                        is_nothrow_move_assignable_v< allocator_type > ) ||
                                !_alloc_traits::propagate_on_container_move_assignment::value );

              iterator begin ()       noexcept { return begin_; }
              iterator   end ()       noexcept { return end_  ; }
        const_iterator begin () const noexcept { return begin_; }
        const_iterator   end () const noexcept { return end_  ; }

        void clear () noexcept { _destruct_at_end( begin_ ); }

        NPL_NODISCARD size_type     size () const noexcept { return static_cast< size_type >( end_     - begin_ ); }
        NPL_NODISCARD size_type capacity () const noexcept { return static_cast< size_type >( end_cap_ - first_ ); }
        NPL_NODISCARD bool         empty () const noexcept { return end_ == begin_;                                }

        size_type _front_spare () const noexcept { return static_cast< size_type >( begin_ - first_ ); }
        size_type  _back_spare () const noexcept { return static_cast< size_type >( end_cap_ - end_ ); }

              reference front ()       noexcept { return *begin_;       }
              reference  back ()       noexcept { return *( end_ - 1 ); }
        const_reference front () const noexcept { return *begin_;       }
        const_reference  back () const noexcept { return *( end_ - 1 ); }

        void reserve ( size_type const _count_ );

        void shrink_to_fit () noexcept;

        void push_front ( const_reference _val_ );
        void push_front ( value_type &&   _val_ );
        void push_back  ( const_reference _val_ );
        void push_back  ( value_type &&   _val_ );

        template< typename... Args >
        void emplace_back ( Args&&... _args_ );

        void pop_front () { _destruct_at_begin( begin_ + 1 ); }
        void pop_back  () { _destruct_at_end  ( end_   - 1 ); }

        void _construct_at_end ( size_type const _count_                        );
        void _construct_at_end ( size_type const _count_, const_reference _val_ );

        template< typename InputIterator >
        enable_if_t
        <
                 is_cpp17_input_iterator_v  < InputIterator > &&
                !is_cpp17_forward_iterator_v< InputIterator >,
                 void
        >
        _construct_at_end ( InputIterator _first_, InputIterator _last_ );

        template< typename ForwardIterator >
        enable_if_t
        <
                is_cpp17_forward_iterator_v< ForwardIterator >,
                void
        >
        _construct_at_end( ForwardIterator _first_, ForwardIterator _last_ );

        void _destruct_at_begin ( pointer _new_begin_ )
        { _destruct_at_begin( _new_begin_, is_trivially_destructible< value_type >() ); }

        void _destruct_at_begin ( pointer _new_begin_, false_type );
        void _destruct_at_begin ( pointer _new_begin_,  true_type );

        void _destruct_at_end ( pointer _new_last_ ) noexcept
        { _destruct_at_end( _new_last_, is_trivially_destructible< value_type >() ); }

        void _destruct_at_end ( pointer _new_last_, false_type ) noexcept;
        void _destruct_at_end ( pointer _new_last_,  true_type ) noexcept;

        void swap ( split_buffer & _other_ )
                noexcept( !_alloc_traits::propagate_on_container_swap::value ||
                                std::is_nothrow_swappable< _alloc_rr >::value );

        bool _invariants () const;

private:
        void _move_assign_alloc ( split_buffer & _buffer_, true_type )
                noexcept( is_nothrow_move_assignable_v< allocator_type > )
        {
                _alloc() = NPL_MOVE( _buffer_._alloc() );
        }
        void _move_assign_alloc ( split_buffer &, false_type ) noexcept {}

        struct _construct_transaction
        {
                explicit _construct_transaction ( pointer * _ptr_, size_type _count_ ) noexcept
                        : position_( *_ptr_ ), end_( *_ptr_ + _count_ ), dest_( _ptr_ )
                {}

                ~_construct_transaction ()
                {
                        *dest_ = position_;
                }

                pointer position_ ;
                pointer const end_;
        private:
                pointer * dest_;
        };
};

template< typename T, typename Allocator >
bool
split_buffer< T, Allocator >::_invariants () const
{
        if( first_ == nullptr )
        {
                if( begin_ != nullptr )
                {
                        return false;
                }
                if( end_ != nullptr )
                {
                        return false;
                }
                if( _end_cap() != nullptr )
                {
                        return false;
                }
        }
        else
        {
                if( begin_ < first_ )
                {
                        return false;
                }
                if( end_ < begin_ )
                {
                        return false;
                }
                if( _end_cap() < end_ )
                {
                        return false;
                }
        }
        return true;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::_construct_at_end ( size_type const _count_ )
{
        _construct_transaction tx( &this->end_, _count_ );

        for( ; tx.position_ != tx.end_; ++tx.position_ )
        {
                _alloc_traits::construct( this->_alloc(), mem::to_address( tx.position_ ) );
        }
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::_construct_at_end ( size_type const _count_, const_reference _val_ )
{
        _construct_transaction tx( &this->end_, _count_ );

        for( ; tx.position_ != tx.end_; ++tx.position_ )
        {
                _alloc_traits::construct( this->_alloc(), mem::to_address( tx.position_ ), _val_ );
        }
}

template< typename T, typename Allocator >
template< typename InputIterator >
enable_if_t
<
	 is_cpp17_input_iterator_v  < InputIterator > &&
	!is_cpp17_forward_iterator_v< InputIterator >,
	 void
>
split_buffer< T, Allocator >::_construct_at_end ( InputIterator _first_, InputIterator _last_ )
{
        _alloc_rr & alloc = this->_alloc();

        for( ; _first_ != _last_; ++_first_ )
        {
                if( end_ == end_cap_ )
                {
                        size_type old_cap = end_cap_ - _first_;
                        size_type new_cap = std::max< size_type >( 2 * old_cap, 8 );

                        split_buffer buf( new_cap, 0, alloc );

                        for( pointer p = begin_; p != end_; ++p, ++buf.end_ )
                        {
                                _alloc_traits::construct( buf._alloc(),
                                                mem::to_address( buf.end_ ), NPL_MOVE( *p ) );
                        }
                        swap( buf );
                }
                _alloc_traits::construct( alloc, mem::to_address( this->end_ ), *_first_ );
                ++this->end_;
        }
}

template< typename T, typename Allocator >
template< typename ForwardIterator >
enable_if_t
<
        is_cpp17_forward_iterator_v< ForwardIterator >,
        void
>
split_buffer< T, Allocator >::_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_ )
{
        _construct_transaction tx( &this->end_, std::distance( _first_, _last_ ) );

        for( ; tx.position_ != tx.end_; ++tx.position_, ++_first_ )
        {
                _alloc_traits::construct( this->_alloc(), mem::to_address( tx.position_ ), *_first_ );
        }
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_begin ( pointer _new_begin_, false_type )
{
        while( begin_ != _new_begin_ )
        {
                _alloc_traits::destroy( _alloc(), mem::to_address( begin_++ ) );
        }
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_begin ( pointer _new_begin_, true_type )
{
        begin_ = _new_begin_;
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_end ( pointer _new_last_, false_type ) noexcept
{
        while( end_ != _new_last_ )
        {
                _alloc_traits::destroy( _alloc(), mem::to_address( --end_ ) );
        }
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::_destruct_at_end ( pointer _new_last_, true_type ) noexcept
{
        end_ = _new_last_;
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( size_type const _capacity_, size_type const _start_, _alloc_rr & _alloc_ )
        : end_cap_( nullptr ), alloc_( _alloc_ )
{
        first_ = _capacity_ != 0 ? _alloc_traits::allocate( _alloc(), _capacity_ ) : nullptr;
        begin_ = end_ = first_ + _start_;
        end_cap_ = first_ + _capacity_;
}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ()
        noexcept( is_nothrow_default_constructible_v< allocator_type > )
        : first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr ), alloc_()
{}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ( _alloc_rr & _alloc_ )
        : first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr ), alloc_( _alloc_ )
{}

template< typename T, typename Allocator >
inline
split_buffer< T, Allocator >::split_buffer ( _alloc_rr const & _alloc_ )
        : first_( nullptr ), begin_( nullptr ), end_( nullptr ), end_cap_( nullptr ), alloc_( _alloc_ )
{}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::~split_buffer ()
{
        clear();
        if( first_ )
        {
                _alloc_traits::deallocate( _alloc(), first_, capacity() );
        }
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( split_buffer && _other_ )
        noexcept( is_nothrow_move_constructible_v< allocator_type > )
        : first_   ( NPL_MOVE( _other_.first_   ) ),
          begin_   ( NPL_MOVE( _other_.begin_   ) ),
          end_     ( NPL_MOVE( _other_.end_     ) ),
          end_cap_ ( NPL_MOVE( _other_,end_cap_ ) ),
          alloc_   ( NPL_MOVE( _other_.alloc_   ) )
{
        _other_.first_   = nullptr;
        _other_.begin_   = nullptr;
        _other_.end_     = nullptr;
        _other_.end_cap_ = nullptr;
}

template< typename T, typename Allocator >
split_buffer< T, Allocator >::split_buffer ( split_buffer && _other_, _alloc_rr const & _alloc_ )
        : alloc_( _alloc_ ), end_cap_( nullptr )
{
        if( _alloc_ == _other_._alloc() )
        {
                first_   = _other_.first_  ;
                begin_   = _other_.begin_  ;
                end_     = _other_.end_    ;
                end_cap_ = _other_.end_cap_;

                _other_.first_   = nullptr;
                _other_.begin_   = nullptr;
                _other_.end_     = nullptr;
                _other_.end_cap_ = nullptr;
        }
        else
        {
                size_type cap = _other_.size();

                first_ = _alloc_traits::allocate( _alloc(), cap );
                begin_ = end_ = first_;
                end_cap_ = first_ + cap;

                using move_iter = std::move_iterator< iterator >;
                _construct_at_end( move_iter( _other_.begin() ), move_iter( _other_.end() ) );
        }
}

template< typename T, typename Allocator >
split_buffer< T, Allocator > &
split_buffer< T, Allocator >::operator= ( split_buffer && _other_ )
        noexcept( ( _alloc_traits::propagate_on_container_move_assignment::value &&
                                is_nothrow_move_assignable_v< allocator_type > ) ||
                        !_alloc_traits::propagate_on_container_move_assignment::value )
{
        clear();
        shrink_to_fit();

        first_   = _other_.first_  ;
        begin_   = _other_.begin_  ;
        end_     = _other_.end_    ;
        end_cap_ = _other_.end_cap_;

        _move_assign_alloc( _other_, bool_constant< _alloc_traits::propagate_on_container_move_assignment::value >() );

        _other_.first_ = _other_.begin_ = _other_.end_ = _other_.end_cap_ = nullptr;
        return *this;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::swap ( split_buffer & _other_ )
        noexcept( !_alloc_traits::propagate_on_container_swap::value ||
                        std::is_nothrow_swappable< _alloc_rr >::value )
{
        std::swap( first_  , _other_.first_   );
        std::swap( begin_  , _other_.begin_   );
        std::swap( end_    , _other_.end_     );
        std::swap( end_cap_, _other_.end_cap_ );

        mem::_swap_allocator( _alloc(), _other_.alloc() );
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::reserve ( size_type const _count_ )
{
        if( _count_ < capacity() )
        {
                split_buffer< value_type, _alloc_rr & > tmp( _count_, 0, _alloc() );

                tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                       std::move_iterator< pointer >( end_   ) );

                std::swap( first_  , tmp.first_   );
                std::swap( begin_  , tmp.begin_   );
                std::swap( end_    , tmp.end_     );
                std::swap( end_cap_, tmp.end_cap_ );
        }
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::shrink_to_fit () noexcept
{
        if( capacity() > size() )
        {
                split_buffer< value_type, _alloc_rr & > tmp( size(), 0, _alloc() );

                tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                       std::move_iterator< pointer >( end_   ) );
                tmp.end_ = tmp.begin_ + ( end_ - begin_ );

                std::swap( first_  , tmp.first_   );
                std::swap( begin_  , tmp.begin_   );
                std::swap( end_    , tmp.end_     );
                std::swap( end_cap_, tmp.end_cap_ );
        }
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::push_front ( const_reference _val_ )
{
        if( begin_ == first_ )
        {
                if( end_ < end_cap_ )
                {
                        difference_type diff = end_cap_ - end_;

                        diff = ( diff + 1 ) / 2;
                        begin_ = std::move_backward( begin_, end_, end_ + diff );
                        end_ += diff;
                }
                else
                {
                        size_type cap = std::max< size_type >( 2 * static_cast< size_t >( end_cap_ - first_ ), 1 );

                        split_buffer< value_type, _alloc_rr & > tmp( cap, ( cap + 3 ) / 4, _alloc() );

                        tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                               std::move_iterator< pointer >( end_   ) );

                        std::swap( first_  , tmp.first_   );
                        std::swap( begin_  , tmp.begin_   );
                        std::swap( end_    , tmp.end_     );
                        std::swap( end_cap_, tmp.end_cap_ );
                }
        }
        _alloc_traits::construct( _alloc(), mem::to_address( begin_ - 1 ), _val_ );
        --begin_;
}

template< typename T, typename Allocator >
void
split_buffer< T, Allocator >::push_front ( value_type && _val_ )
{
        if( begin_ == first_ )
        {
                if( end_ < end_cap_ )
                {
                        difference_type diff = end_cap_ - end_;

                        diff = ( diff + 1 ) / 2;
                        begin_ = std::move_backward( begin_, end_, end_ + diff );
                        end_ += diff;
                }
                else
                {
                        size_type cap = std::max< size_type >( 2 * static_cast< size_t >( end_cap_ - first_ ), 1 );

                        split_buffer< value_type, _alloc_rr & > tmp( cap, ( cap + 3 ) / 4, _alloc() );

                        tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                               std::move_iterator< pointer >( end_   ) );

                        std::swap( first_  , tmp.first_   );
                        std::swap( begin_  , tmp.begin_   );
                        std::swap( end_    , tmp.end_     );
                        std::swap( end_cap_, tmp.end_cap_ );
                }
        }
        _alloc_traits::construct( _alloc(), mem::to_address( begin_ - 1 ), NPL_MOVE( _val_ ) );
        --begin_;
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::push_back ( const_reference _val_ )
{
        if( end_ == end_cap_ )
        {
                if( begin_ > first_ )
                {
                        difference_type diff = begin_ - first_;

                        diff = ( diff + 1 ) / 2;
                        end_ = mem::_move_forward( begin_, end_, begin_ - diff );
                        begin_ -= diff;
                }
                else
                {
                        size_type cap = std::max< size_type >( 2 * static_cast< size_t >( end_cap_ - first_ ), 1 );

                        split_buffer< value_type, _alloc_rr & > tmp( cap, cap / 4, _alloc() );

                        tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                               std::move_iterator< pointer >( end_   ) );

                        std::swap( first_  , tmp.first_   );
                        std::swap( begin_  , tmp.begin_   );
                        std::swap( end_    , tmp.end_     );
                        std::swap( end_cap_, tmp.end_cap_ );
                }
        }
        _alloc_traits::construct( _alloc(), mem::to_address( end_ ), _val_ );
        ++end_;
}

template< typename T, typename Allocator >
inline
void
split_buffer< T, Allocator >::push_back ( value_type && _val_ )
{
        if( end_ == end_cap_ )
        {
                if( begin_ > first_ )
                {
                        difference_type diff = begin_ - first_;

                        diff = ( diff + 1 ) / 2;
                        end_ = mem::_move_forward( begin_, end_, begin_ - diff );
                        begin_ -= diff;
                }
                else
                {
                        size_type cap = std::max< size_type >( 2 * static_cast< size_t >( end_cap_ - first_ ), 1 );

                        split_buffer< value_type, _alloc_rr & > tmp( cap, cap / 4, _alloc() );

                        tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                               std::move_iterator< pointer >( end_   ) );

                        std::swap( first_  , tmp.first_   );
                        std::swap( begin_  , tmp.begin_   );
                        std::swap( end_    , tmp.end_     );
                        std::swap( end_cap_, tmp.end_cap_ );
                }
        }
        _alloc_traits::construct( _alloc(), mem::to_address( end_ ), NPL_MOVE( _val_ ) );
        ++end_;
}

template< typename T, typename Allocator >
template< typename... Args >
void
split_buffer< T, Allocator >::emplace_back ( Args&&... _args_ )
{
        if( end_ == end_cap_ )
        {
                if( begin_ > first_ )
                {
                        difference_type diff = begin_ - first_;

                        diff = ( diff + 1 ) / 2;
                        end_ = mem::_move_forward( begin_, end_, begin_ - diff );
                        begin_ -= diff;
                }
                else
                {
                        size_type cap = std::max< size_type >( 2 * static_cast< size_t >( end_cap_ - first_ ), 1 );

                        split_buffer< value_type, _alloc_rr & > tmp( cap, cap / 4, _alloc() );

                        tmp._construct_at_end( std::move_iterator< pointer >( begin_ ),
                                               std::move_iterator< pointer >( end_   ) );

                        std::swap( first_  , tmp.first_   );
                        std::swap( begin_  , tmp.begin_   );
                        std::swap( end_    , tmp.end_     );
                        std::swap( end_cap_, tmp.end_cap_ );
                }
        }
        _alloc_traits::construct( _alloc(), mem::to_address( end_ ), NPL_FWD( _args_ )... );
        ++end_;
}

template< typename T, typename Allocator >
inline
void
swap ( split_buffer< T, Allocator > & _lhs_, split_buffer< T, Allocator > & _rhs_ )
        noexcept( noexcept( _lhs_.swap( _rhs_ ) ) )
{
        _lhs_.swap( _rhs_ );
}


} // namespace npl
