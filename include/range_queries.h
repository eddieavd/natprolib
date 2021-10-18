//
//  range_queries.h
//  natprolib
//
//  Created by Edhem Avdagic on 04/02/2021.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cmath>
#include <memory>
#include <type_traits>
#include <stdexcept>

#ifdef _WIN32
#   include <windows.h>
#else
#   include <unistd.h>
#endif


namespace natprolib
{

namespace range_queries
{
          
#define MIN_CAPACITY 16


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
    
    prefix_array ( prefix_array const &  ) = delete;
    prefix_array ( prefix_array       && ) = delete;
    
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
    prefix_array ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() }
    {
        alloc();
        
        for( auto it = _list_.begin(); it != _list_.end(); it++ )
        {
            push_back( *it );
        }
    }
    template< class Iterator >
    prefix_array ( Iterator const & begin, Iterator const & end ) : capacity_ { MIN_CAPACITY }
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
    
    template< class... Args >
    void emplace_back ( Args&&... args )
    {
        if( capacity_ <= size_ ) { resize(); }
        
        if( size_ == 0 )
        {
            head_[ size_ ] = T ( args... );
        }
        else
        {
            head_[ size_ ] = T ( args... );
            head_[ size_ ] += head_[ size_ - 1 ];
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


template< typename T, typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree;

template< typename T, bool C,
          typename = std::enable_if_t< std::is_arithmetic_v< T > > >
class fenwick_tree_iterator
{
    friend class fenwick_tree< T >;
    friend class fenwick_tree_iterator< T, !C >;
    
    using pointer   = std::conditional_t< C, T const *, T * >;
    using reference = std::conditional_t< C, T const &, T & >;
    
    pointer ptr_;
    
    explicit fenwick_tree_iterator ( pointer _ptr_ ) : ptr_ { _ptr_ } {};
    
public:
    reference   operator*  (     ) const { return *ptr_; }
    auto      & operator++ (     )       { ptr_++; return *this; }
    auto        operator++ ( int )       { auto it = *this; ++*this; return it; }
    
    template< bool R >
    bool operator== ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
    { return ptr_ == rhs.ptr_; }
    
    template< bool R >
    bool operator!= ( fenwick_tree_iterator< T, R > const & rhs ) const noexcept
    { return ptr_ != rhs.ptr_; }
    
    operator fenwick_tree_iterator< T, true > () const
    { return fenwick_tree_iterator< T, true >{ ptr_ }; }
};

template< typename T, typename U >
class fenwick_tree
{
    using     value_type = T;
    using       iterator = fenwick_tree_iterator< T, false >;
    using const_iterator = fenwick_tree_iterator< T,  true >;
    
private:
    T           * head_     { nullptr };
    std::size_t   size_           { 0 };
    std::size_t   capacity_       { 0 };
    
    void alloc  (                        ) {}
    void alloc  ( std::size_t _capacity_ ) {}
    void resize (                        ) {}
    void resize ( std::size_t _capacity_ ) {}
    
#ifdef _WIN32
    std::size_t max_size () const
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof( status );
        GlobalMemoryStatusEx( &status );
        
        return status.ullTotalPhys;
    }
#else
    std::size_t max_size () const
    {
        long pages     = sysconf( _SC_PHYS_PAGES );
        long page_size = sysconf( _SC_PAGE_SIZE );
        
        return pages * page_size;
    }
#endif
    
    inline std::size_t p ( std::size_t const k ) const noexcept { return k & -k; }
    
    inline bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < size_; }
    
    T sum_to_index ( std::size_t const _index_ ) const {}
    
public:
    auto begin ()       { return       iterator{ head_         }; }
    auto   end ()       { return       iterator{ head_ + size_ }; }
    auto begin () const { return const_iterator{ head_         }; }
    auto   end () const { return const_iterator{ head_ + size_ }; }
    
    inline T const & operator[] ( std::size_t const _index_ ) const noexcept {}
    
    fenwick_tree (                                           ) : capacity_ {             0 } { alloc(           ); }
    fenwick_tree (              std::size_t const _capacity_ ) : capacity_ {    _capacity_ } { alloc( capacity_ ); }
    fenwick_tree ( T ** _head_, std::size_t const _capacity_ ) : capacity_ {    _capacity_ } {}
    fenwick_tree ( std::initializer_list< T > const & _list_ ) : capacity_ { _list_.size() } {}
    template< class Iterator >
    fenwick_tree ( Iterator const & begin, Iterator const & end ) : capacity_ { std::distance( begin, end ) } {}
    
    inline std::size_t     size () const noexcept { return size_ - 1; }
    inline std::size_t capacity () const noexcept { return capacity_ - 1; }
    
    T range ( std::size_t const _x_, std::size_t const _y_ ) const {}
    
    void update ( T const _value_, std::size_t const _index_ ) {}
    void    add ( T const _value_, std::size_t const _index_ ) {}
    
    void reserve ( std::size_t const _capacity_ ) { resize( _capacity_ ); }
    
    void push_back ( T _value_ ) {}
    
    ~fenwick_tree () { free( head_ ); }
};
          

template< typename T, typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree;

template< typename T, bool C,
          typename = std::enable_if_t< std::is_default_constructible_v< T > > >
class segment_tree_iterator
{
    //  TODO: implement
};

template< typename T, typename U >
class segment_tree
{
    using     value_type = T;
    using       iterator = segment_tree_iterator< T, false >;
    using const_iterator = segment_tree_iterator< T,  true >;
    
private:
    T           * head_;
    std::size_t   size_;
    std::size_t   og_size_;
    std::size_t   capacity_;
    
    std::function< T ( T, T ) > parent_builder_;
    
    void alloc ( std::size_t const _capacity_ )
    {
        og_size_  = _capacity_;
        size_     = round_up_to_pow_2( _capacity_ );
        capacity_ = 2 * size_;
        head_     = ( T* ) std::malloc( sizeof( T ) * capacity_ );
        
        for( auto i = 0; i < capacity_; i++ )
        {
            head_[ i ] = T();
        }
    }
    
    //  the fuck is this
    void alloc2 ( std::size_t const _capacity_ )
    {
        og_size_  = _capacity_;
        size_     = 0;
        capacity_ = 2 * round_up_to_pow_2( _capacity_ );
//        head_ = new T[ capacity_ ];
        head_     = ( T* ) std::malloc( sizeof( T ) * capacity_ );
//        head_ = ( T* ) std::calloc( 3, sizeof( T ) * capacity_ );
        
        for( auto i = capacity_ / 2; i < capacity_; i++ )
        {
            head_[ i ] = { 5 };
        }
    }
    
    std::size_t round_up_to_pow_2 ( std::size_t const _size_ ) const noexcept
    {
        auto log2 = std::log2( _size_ );
        
        if( log2 != std::floor( log2 ) )
        {
            auto pow = std::ceil( log2 );
            return std::pow( 2, pow );
        }
        return _size_;
    }
    
    void construct_tree ()
    {
        for( auto i = size_ - 1; i > 0; i-- )
        {
            head_[ i ] = parent_builder_( head_[ 2 * i ], head_[ 2 * i + 1 ] );
        }
    }
    
    bool is_index_in_range ( std::size_t const _index_ ) const noexcept { return _index_ < og_size_; }
    
public:
    auto begin ()       { return       iterator{ head_ + size_            }; }
    auto   end ()       { return       iterator{ head_ + size_ + og_size_ }; }
    auto begin () const { return const_iterator{ head_ + size_            }; }
    auto   end () const { return const_iterator{ head_ + size_ + og_size_ }; }
    
    inline T const & operator[] ( std::size_t const _index_ ) const noexcept { return head_[ size_ + _index_ ]; }
    
    inline bool operator== ( segment_tree< T > const & rhs ) const { return head_ == rhs.head_; }
    inline bool operator!= ( segment_tree< T > const & rhs ) const { return head_ != rhs.head_; }
    
    segment_tree ( std::size_t _capacity_ = MIN_CAPACITY, std::function< T ( T, T ) > _pb_ = [](){ return T(); } ) : parent_builder_ { _pb_ } { alloc( _capacity_ ); }
    segment_tree ( T ** _head_, std::size_t _capacity_, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ }
    {
        alloc( _capacity_ );
        
        for( auto i = 0; i < _capacity_; i++ )
        {
            head_[ size_ + i ] = ( *_head_ )[ i ];
        }
        *_head_ = nullptr;
        construct_tree();
    }
    template< class Iterator >
    segment_tree ( Iterator begin, Iterator const & end, std::function< T ( T, T ) > _pb_ ) : parent_builder_ { _pb_ }
    {
        alloc( std::distance( begin, end ) );
        
        for( auto i = 0; begin != end; i++ )
        {
            head_[ size_ + i ] = *begin;
            begin++;
        }
        construct_tree();
    }
    
    void push_back ( T _value_ )
    {
        if( size_ >= capacity_ / 2 )
        {
            return;
        }
        
        head_[ capacity_ / 2 + size_ ] = _value_;
        
//        construct_tree();
    }
    
    inline std::size_t size () const noexcept { return og_size_; }
    
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
    inline T range ( std::size_t _x_, std::size_t _y_ ) const
    {
        if( !is_index_in_range( _x_ ) || !is_index_in_range( _y_ ) )
        {
            throw std::out_of_range( "index out of bounds" );
        }
        
        _x_ += size_;
        _y_ += size_;
        
        T res = T();
        
        while( _x_ <= _y_ )
        {
            if( _x_ % 2 == 1 ) res = parent_builder_( res, head_[ _x_++ ] );
            if( _y_ % 2 == 0 ) res = parent_builder_( res, head_[ _y_-- ] );
            _x_ /= 2;
            _y_ /= 2;
        }
        return res;
    }
    
    void update ( std::size_t _index_, T const _value_ )
    {
        if( !is_index_in_range( _index_ ) )
        {
            throw std::out_of_range( "index out of bounds" );
        }
        
        _index_ += size_;
        
        head_[ _index_ ] = _value_;
        
        for( _index_ /= 2; _index_ >= 1; _index_ /= 2 )
        {
            head_[ _index_ ] = parent_builder_( head_[ 2 * _index_ ], head_[ 2 * _index_ + 1 ] );
        }
    }
    
//    void print ()
//    {
//        for( auto i = 0; i < capacity_; i++ )
//        {
//            std::cout << head_[ i ].val << std::endl;
//        }
//    }
    
    void set_parent_builder ( std::function< T ( T, T ) > _pb_ ) { parent_builder_ = _pb_; construct_tree(); }
    
    ~segment_tree () { free( head_ ); }
};


//  TODO

template< typename T >
class lazy_segtree;

template< typename T >
class dynamic_segtree;

template< typename T >
class persistent_segtree;




} // namespace range_queries

} // namespace natprolib
