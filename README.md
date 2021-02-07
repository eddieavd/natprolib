# natprolib<br/>

**natprolib** is a library consisting of various containers and algorithms commonly used in competitive programming, implemented in a wanna-be STL style to be more accessible for general use.  

## range queries<br/>

The **range queries** and **meta range queries** headers provide containers which are specially optimized for performing queries and updates on a list of elements.  
#### prefix array<br/>

When tasked with processing a large amount of sum queries on a static array, the fastest approach is using a prefix sum array. Each value in the prefix sum array equals the sum of values in the original array up to that position, i.e., the value at position _k_ equals _sum( 0, k )_. The prefix sum array can be constructed in O(n) time. Considering the array [ 1, 3, 4, 8, 6, 1, 4, 2 ] the corresponding prefix array would be [ 1, 4, 8, 16, 22, 23, 27, 29 ]. Since the prefix array contains all values of _sum( 0, k )_, we can calculate any value of _sum( a, b )_ in O(1) as follows:  
_sum ( a, b ) = sum ( 0, b ) - sum( 0, a - 1 )_ for _a != 0_
