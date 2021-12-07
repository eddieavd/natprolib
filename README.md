# natprolib<br/>

**natprolib** is a library consisting of various containers and algorithms commonly used in competitive programming, implemented in a wannabe STL style to be more accessible for general use.  

## table of contents

* [range queries](#range_queries)
    * [prefix array](#prefix_array)
    * [fenwick tree](#fenwick_tree)
    * [segment tree](#segment_tree)
* [meta range queries](#meta_range_queries)
    * [meta prefix array](#meta_prefix_array)
    * [meta fenwick tree](#meta_ftree)

## <a name = "range_queries"></a> range queries<br/>

The **range queries** header provides containers which are specially optimized for performing queries and updates on a list of elements.  

#### <a name = "prefix_array"></a> prefix array<br/>

When tasked with processing a large amount of sum queries on a static array, the fastest approach is using a prefix sum array. 
Each value in the prefix sum array equals the sum of values in the original array up to that position, i.e., the value at position _k_ equals _sum( 0, k )_. 
Considering the array [ 1, 2, 3, 4, 5 ], the corresponding prefix array would be [ 1, 3, 6, 10, 15 ].  
The prefix sum array can be constructed in O( n ) time.  
Since the prefix array contains all values of _sum( 0, k )_, we can calculate any value of _sum( a, b )_ in O( 1 ) as follows:  
_sum ( a, b ) = sum ( 0, b ) - sum( 0, a - 1 )_ for _a != 0_  

#### <a name = "fenwick_tree"></a> fenwick tree<br/>

The Fenwick tree, also known as a binary indexed tree expands on the prefix array by providing O(log _n_) updates to elements in the array. The payoff comes in the form of slower queries which now also run in O(log _n_). Even though it's called a tree, it's usually represented as a one-indexed array.  
Let _p( k )_ denote the largest power of two that divides _k_. The binary indexed tree is stored as an array _tree_ such that  
_tree[ k ] = sum( k - p( k ) + 1, k )_,  
i.e., each position _k_ contains the sum of values in a range of the original array whose length is _p( k )_ and that ends at position _k_.  
For example, since _p( 6 ) = 2_, _tree[ 6 ]_ contains the value of _sum( 5, 6 )_.  
_<sub>pics will go here i promise</sub>_  
  
Since any range can be divided into log _n_ of the ranges whose sums we calculated, any value of _sum( 1, k )_ can be calculated in O(log _n_) time.
For example, when calculating the sum on range [ 1, 7 ], we'd use the following ranges:  
_sum( 1, 7 ) = sum( 1, 4 ) + sum( 5, 6 ) + sum( 7, 7 )_  
Just like with the prefix array, we can use the same approach to calculating sums on a range [ a, b ] where _a != 1_:  
_sum( a, b ) = sum( 1, b ) - sum( 1, a - 1 )_  
When updating values, several elements of the array need to be changed. However, since each array element belongs to log(_n_) ranges in the Fenwick tree, updating values can be done in O(log _n_) time.  

#### <a name = "segment_tree"></a> segment tree<br/>

The segment tree is a data structure which allows for both range queries and single value updates, both in O(log _n_) time. 
Unlike the previous entries which only support sum queries, the segment tree can be used for minimum/maximum queries, sum queries as well as with any other user-defined comparator (called _parent_builder_).<sup>1</sup>  
It is implemented as a binary tree such that the nodes on the bottom level of the tree correspond to the array elements, and the other nodes contain information needed for processing range queries.  
  
![segment tree](https://github.com/eddieavd/img/blob/main/segtree.png)  
<sup>example segment tree used for sum queries</sup>  
It is assumed that the input array has a size equal to a power of two. If that is not the case, the original array is extended to the closest power of two. 
For the sake of efficiency the tree is stored as an array of _2n_ elements (where _n_ is the size of the original array extended to the closest power of two) with _tree[ 1 ]_ being the root node and _tree[ 2k ]_ and _tree[ 2k+1 ]_ being the children of _tree[ k ]_.    
<br/><sup>1</sup> The tests contain an example showing the usage of a segment tree for retreiving the second largest element on a specified range.  

## <a name = "meta_range_queries"></a> meta range queries<br/>

The idea behind **meta range queries** is to provide the same functionality as the **range queries** containers, but be fully evaluated _at compile time_.    

#### <a name = "meta_prefix_array"></a> meta prefix array <br/>

If we have a static array of integers known at compile time and we want to calculate sums on different ranges of the array (either in compile time or runtime), we could do the following:  

```C++
int main ()
{
    constexpr auto prefix = make_prefix< int, 5 >( { 1, 1, 1, 1, 1 } );
    int range = prefix.range( 0, 4 );
    
    return 0;
}
```  
Looking at the generated assembly:  
```ASM
main:
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-48], 1    ;    prefix[ 0 ] aka prefix.range( 0, 0 )
        mov     DWORD PTR [rbp-44], 2    ;    prefix[ 1 ] aka prefix.range( 0, 1 )
        mov     DWORD PTR [rbp-40], 3    ;    prefix[ 2 ] aka prefix.range( 0, 2 )
        mov     DWORD PTR [rbp-36], 4    ;    prefix[ 3 ] aka prefix.range( 0, 3 )
        mov     DWORD PTR [rbp-32], 5    ;    prefix[ 4 ] aka prefix.range( 0, 4 )
        mov     QWORD PTR [rbp-24], 5    ;    prefix.size
        mov     QWORD PTR [rbp-16], 5    ;    prefix.capacity
        mov     DWORD PTR [rbp-4], 5     ;    int range = prefix.range( 0, 4 )
        mov     eax, 0
        pop     rbp
        ret
```
we can see that the prefix array gets constructed in compile time and the only thing left to do in runtime is write/read the calculated values to/from memory.    

Iterating over the prefix array in compile time is also possible; the following code:  

```C++
constexpr int sum_of_ranges ()
{
    int sum = 0;
    constexpr auto prefix = make_prefix< int, 5 >( { 1, 1, 1, 1, 1 } );
    
    for( auto x : prefix )
    {
        sum += x;
    }
    
    return sum;
}
int main ()
{
    int sum = sum_of_ranges();
    
    
    return 0;
}
```
would get compiled down to:  
```ASM
main:
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], 15    ;    int sum = sum_of_ranges()
        mov     eax, 0
        pop     rbp
        ret
```    

#### <a name = "meta_ftree"></a> meta fenwick tree <br/>

Still a major work in progress (as is everything else here lol), currently only allows queries, no updates to the underlying data. In other words, you get all the drawbacks but none of the benefits compared to using a prefix array (refer to [fenwick tree](#fenwick_tree) for more details).
