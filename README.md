# natprolib<br/>

**natprolib** is a library consisting of various containers and algorithms commonly used in competitive programming, implemented in a wanna-be STL style to be more accessible for general use.  

## range queries<br/>

The **range queries** and **meta range queries** headers provide containers which are specially optimized for performing queries and updates on a list of elements.  
#### prefix array<br/>

When tasked with processing a large amount of sum queries on a static array, the fastest approach is using a prefix sum array. Each value in the prefix sum array equals the sum of values in the original array up to that position, i.e., the value at position _k_ equals _sum( 0, k )_. The prefix sum array can be constructed in O(n) time. Considering the array [ 1, 3, 4, 8, 6, 1, 4, 2 ] the corresponding prefix array would be [ 1, 4, 8, 16, 22, 23, 27, 29 ]. Since the prefix array contains all values of _sum( 0, k )_, we can calculate any value of _sum( a, b )_ in O(1) as follows:  
_sum ( a, b ) = sum ( 0, b ) - sum( 0, a - 1 )_ for _a != 0_  

#### segment tree<br/>

The segment tree is a data structure which allows for both range queries and single value updates, both in O(logn) time. Unlike the prefix array which only supports sum queries, the segment tree can be used for minimum/maximum queries, sum queries as well as any other user-defined comparator (called _parent_builder_). The tests contain an example showing the usage of a segment tree for retreiving the second largest element on a specified range.  
It is implemented as a binary tree such that the nodes on the bottom level of the tree correspond to the array elements, and the other nodes contain information needed for processing range queries.  
![segment tree](https://github.com/eddieavd/img/blob/main/segtree.png)  
example segment tree used for sum queries  
It is assumed that the input array has a size equal to a power of two (if that is not the case, the original array is extended with _default_ elements to the closest power of two). For the sake of efficiency the tree is stored as an array of _2n_ elements (where _n_ is the size of the original array extended to the closest power of two) with _tree[ 1 ]_ being the root node and _tree[ 2k ]_ and _tree[ 2k+1 ]_ being the children of _tree[ k ]_.
