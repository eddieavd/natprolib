//
//  range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <memory>
#include <type_traits>
#include <stdexcept>

#ifdef _WIN32
#   include <windows.h>
#else
#   include <unistd.h>
#endif

#define MIN_CAPACITY 8


namespace natprolib
{

namespace range_queries
{


/**
 *  prefix array
 *  vector-like container limited to arithmetic types
 *  holds sum of all previous elements at each index
 *  allows for constant-time sum queries on any interval
 */
template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array;

template< typename T, bool C,
          typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class prefix_array_iterator
{
    friend class prefix_array< T >;
    friend class prefix_array_iterator< T, !C >;
    
    using pointer   = std::conditional_t< C, T const *, T * >;
    using reference = std::conditional_t< C, T const &, T & >;
    
    pointer ptr_;
    
    explicit prefix_array_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
    
public:
    reference   operator*  (     ) const { return *ptr_; }
    auto      & operator++ (     )       { ptr_++; return *this; }
    auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
    
    template< bool R >
    bool operator== ( prefix_array_iterator< T, R > const & rhs ) const
    { return ptr_ == rhs.ptr_; }
    
    template< bool R >
    bool operator!= ( prefix_array_iterator< T, R > const & rhs ) const
    { return ptr_ != rhs.ptr_; }
    
    operator prefix_array_iterator< T, true > () const
    { return prefix_array_iterator< T, true >{ ptr_ }; }
};
// raw pointers ahead, it's 2021, i deserve to be punished
template< typename T, typename U >
class prefix_array
{
    using     value_type = T;
    using       iterator = prefix_array_iterator< T, false >;
    using const_iterator = prefix_array_iterator< T,  true >;
    
private:
    T           * head_{ nullptr };
    std::size_t   size_      { 0 };
    std::size_t   capacity_  { 0 };
    
    void alloc  () { head_ = ( T* ) std::malloc( sizeof( T ) * capacity_ ); if( !head_ ) throw std::bad_alloc(); }
    void resize ()
    {
        auto new_cap = ( capacity_ + 1 ) * 1.618;  // golden ratio magic
        auto tmp = ( T* ) std::realloc( head_, sizeof( T ) * new_cap );
        
        if( tmp )
        {
            head_     = tmp;
            capacity_ = new_cap;
        }
    }
    void resize ( std::size_t const _capacity_ )
    {
        if( _capacity_ > max_size() )
        {
            throw std::out_of_range( "capacity > max_size" );
        }
        auto tmp = ( T* ) std::realloc( head_, sizeof( T ) * _capacity_ );
        
        if( tmp )
        {
            head_     = tmp;
            capacity_ = _capacity_;
        }
    }
    
    bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }
    
#ifdef _WIN32
    inline std::size_t max_size () const noexcept
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof( status );
        GlobalMemoryStatusEx( &status );
        
        return status.ullTotalPhys;
    }
#else
    inline std::size_t max_size () const noexcept
    {
        auto pages     = sysconf( _SC_PHYS_PAGES );
        auto page_size = sysconf( _SC_PAGE_SIZE  );
        
        return pages * page_size;
    }
#endif
    
public:
    auto begin ()       { return       iterator{ head_         }; }
    auto   end ()       { return       iterator{ head_ + size_ }; }
    auto begin () const { return const_iterator{ head_         }; }
    auto   end () const { return const_iterator{ head_ + size_ }; }
    
    /**
     *  @brief returns sum on range ( 0 ... _index_ )
     *  @param _index_ - index of last element in range
     */
    inline T const & operator[] ( std::size_t const _index_ ) const noexcept
    { return head_[ _index_ ]; }
    
    inline bool operator== ( prefix_array< T, U > const & rhs ) const noexcept
    { return head_ == rhs.head_; }
    
    inline bool operator!= ( prefix_array< T, U > const & rhs ) const noexcept
    { return head_ != rhs.head_; }
    
    prefix_array (                                           ) : capacity_ { MIN_CAPACITY } { alloc(); };
    prefix_array (              std::size_t const _capacity_ ) : capacity_ {   _capacity_ } { alloc(); };
    prefix_array ( T ** _head_, std::size_t const _capacity_ ) : capacity_ {   _capacity_ }
    {
        alloc();
        
        for( auto i = 0; i < _capacity_; i++ )
        {
            push_back( ( *_head_ )[ i ] );
        }
        *_head_ = nullptr;
    }
    prefix_array ( std::initializer_list< T > _list_ ) : capacity_ { _list_.size() }
    {
        alloc();
        
        for( auto it = _list_.begin(); it != _list_.end(); it++ )
        {
            push_back( *it );
        }
    }
    template< class Iterator >
    prefix_array ( Iterator const begin, Iterator const end ) : capacity_ { MIN_CAPACITY }
    {
        alloc();
        
        for( auto it = begin; it != end; it++ )
        {
            push_back( *it );
        }
    }
    
    inline std::size_t     size () const noexcept { return     size_; }
    inline std::size_t capacity () const noexcept { return capacity_; }
    
    /**
     *  @brief returns sum on range ( 0 ... _index_ )
     *  @param _index_ - index of last element in range
     */
    inline T const & at ( std::size_t const _index_ ) const
    {
        if( !is_index_in_range( _index_ ) )
        {
            throw std::out_of_range( "index out of bounds" );
        }
        else
        {
            return operator[]( _index_ );
        }
    }
    
    /**
     *  @brief returns sum of passed range
     *  @param _x_ - index of first element in range
     *  @param _y_ - index of last element in range
     */
    inline T range ( std::size_t const _x_, std::size_t const _y_ ) const
    {
        if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
        {
            throw std::out_of_range( "index out of bounds" );
        }
        else
        {
            return _x_ == 0 ?
                    operator[]( _y_ ) :
                    operator[]( _y_ ) - operator[]( _x_ - 1 );
        }
    }
    
    /**
     *  @brief returns original value of element
     *  @param _index_ - index of the element
     */
    inline T element_at ( std::size_t const _index_ ) const
    {
        if( !is_index_in_range( _index_ ) )
        {
            throw std::out_of_range( "index out of bounds" );
        }
        else
        {
            return _index_ == 0 ?
                    operator[]( _index_ ) :
                    operator[]( _index_ ) - operator[]( _index_ - 1 );
        }
    }
    
    void reserve ( std::size_t const _capacity_ ) { resize( _capacity_ ); }
    
    /**
     *  @brief inserts element at the end and adds previous sum
     *  @param _value_ - value to be pushed back
     */
    void push_back ( T _value_ )
    {
        if( capacity_ <= size_ ) { resize(); }
        
        if( size_ == 0 )
        {
            head_[ size_ ] = _value_;
        }
        else
        {
            head_[ size_ ] = head_[ size_ - 1 ] + _value_;
        }
        size_++;
    }
    void push_array ( T _value_ )
    {
        push_back( _value_ );
    }
    template< typename... Args >
    void push_array( T _value_, Args... _values_ )
    {
        push_back( _value_ );
        push_array( _values_... );
    }
    
    ~prefix_array () { free( head_ ); }
};

//  TODO

template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree;

template< typename T >
class segment_tree;

template< typename T >
class lazy_segtree;

template< typename T >
class dynamic_segtree;

template< typename T >
class persistent_segtree;




} // namespace range_queries

} // namespace natprolib
