DEPRECATED  
Development continued here: https://github.com/eddieavd/uti

<pre>
                       █████                                 ████   ███  █████  
                      ░░███                                 ░░███  ░░░  ░░███  
 ████████    ██████   ███████   ████████  ████████   ██████  ░███  ████  ░███████  
░░███░░███  ░░░░░███ ░░░███░   ░░███░░███░░███░░███ ███░░███ ░███ ░░███  ░███░░███  
 ░███ ░███   ███████   ░███     ░███ ░███ ░███ ░░░ ░███ ░███ ░███  ░███  ░███ ░███  
 ░███ ░███  ███░░███   ░███ ███ ░███ ░███ ░███     ░███ ░███ ░███  ░███  ░███ ░███  
 ████ █████░░████████  ░░█████  ░███████  █████    ░░██████  █████ █████ ████████  
░░░░ ░░░░░  ░░░░░░░░    ░░░░░   ░███░░░  ░░░░░      ░░░░░░  ░░░░░ ░░░░░ ░░░░░░░░  
                                ░███  
                                █████  
                               ░░░░░  
</pre>

**natprolib** is a library consisting of various containers and algorithms commonly used in competitive programming, implemented in a wannabe STL style to be more accessible for general use.  



## table of contents

* [range queries](#range_queries)
    * [prefix array](#prefix_array)
    * [prefix vector](#prefix_vector)
    * [fenwick tree](#fenwick_tree)
    * [segment tree](#segment_tree)
* [algorithms](#algorithms)
    * [algo1](#algo1)
    * [algo2](#algo2)


## <a name = "range_queries"></a> range queries<br/>

The **range queries** header provides containers which are specially optimized for performing queries and updates on a list of elements.  

#### <a name = "prefix_array"></a> prefix array<br/>

When tasked with processing a large amount of sum queries on a static array, the fastest approach is using a prefix sum array. 
Each value in the prefix sum array equals the sum of values in the original array up to that position, i.e., the value at position ```k``` equals ```sum( 0, k )```.  
Considering the array  
```[ 1, 1, 1, 0, 1 ]```  
the corresponding prefix array would be  
```[ 1, 2, 3, 3, 4 ]```.  
The prefix sum array can be constructed in _O( n )_ time.  
Since the prefix array contains all values of ```sum( 0, k )```, we can calculate any value of ```sum( a, b )``` in _O( 1 )_ as follows:  
```sum ( a, b ) = sum ( 0, b ) - sum( 0, a - 1 )``` for ```a != 0```  


#### <a name = "prefix_vector"></a> prefix vector<br/>

The prefix vector is a vector-like implementation of the prefix array.

#### example

Let's say you have a 2D vector of pixels. The following code constructs an integral image and performs a mean blur with a 32 pixel blur radius.

```c++
{
    constexpr int blur_radius = 32 ;

    using    image_t = npl::       vector< npl::       vector< unsigned char > > ;
    using integral_t = npl::prefix_vector< npl::prefix_vector< unsigned long > > ;
    
    image_t      source_image = load_pixels();
    image_t     blurred_image;
    integral_t integral_image;

    for( size_t i = 0; i < source_image.height(); ++i )
    {
        integral_image.emplace_back( source_image.at( i ).begin(),
                                     source_image.at( i ).end() );
    }
    for( size_t i = blur_radius / 2; i < integral_image.height() - blur_radius / 2; ++i )
    {
        blurred_image.emplace_back();

        for( size_t j = blur_radius / 2; j < integral_image.width() - blur_radius / 2; ++i )
        {
            // sets value of new pixel to
            // average value of pixels within blur_radius distance from current pixel
            // sum of neighboring pixels calculated in constant time
            blurred_image.at( i - blur_radius / 2, j - blur_radius / 2 ) =
                integral_image.range( i - blur_radius / 2, j - blur_radius / 2,
                                      i + blur_radius / 2, j + blur_radius / 2 )
                / ( blur_radius * blur_radius );
        }
    }
}
```

#### <a name = "fenwick_tree"></a> fenwick tree<br/>

The Fenwick tree, also known as a binary indexed tree expands on the prefix array by providing _O(log n)_ updates to elements in the array. The payoff comes in the form of slower queries which now also run in _O(log n)_. Even though it's called a tree, it's usually represented as a one-indexed array.  
Let ```p( k )``` denote the largest power of two that divides ```k```. The binary indexed tree is stored as an array ```tree``` such that  
```tree[ k ] = sum( k - p( k ) + 1, k )```,  
i.e., each position ```k``` contains the sum of values in a range of the original array whose length is ```p( k )``` and that ends at position ```k```.  
For example, since ```p( 6 ) = 2```, ```tree[ 6 ]``` contains the value of ```sum( 5, 6 )```.  
_<sub>pics will go here i promise</sub>_  
  
Since any range can be divided into ```log n``` of the ranges whose sums we calculated, any value of ```sum( 1, k )``` can be calculated in _O(log n)_ time.
For example, when calculating the sum on range ```[ 1, 7 ]```, we'd use the following ranges:  
```sum( 1, 7 ) = sum( 1, 4 ) + sum( 5, 6 ) + sum( 7, 7 )```  
Just like with the prefix array, we can use the same approach to calculating sums on a range ```[ a, b ]``` where ```a != 1```:  
```sum( a, b ) = sum( 1, b ) - sum( 1, a - 1 )```  
When updating values, several elements of the array need to be changed. However, since each array element belongs to log(_n_) ranges in the Fenwick tree, updating values can be done in _O(log n)_ time.  

#### <a name = "segment_tree"></a> segment tree<br/>

The segment tree is a data structure which allows for both range queries and single value updates, both in _O(log n)_ time. 
Unlike the previous entries which only support sum queries, the segment tree can be used for minimum/maximum queries, sum queries as well as with any other user-defined comparator (called ```parent_builder```).<sup>1</sup>  
It is implemented as a binary tree such that the nodes on the bottom level of the tree correspond to the array elements, and the other nodes contain information needed for processing range queries.  
  
![segment tree](https://github.com/eddieavd/img/blob/main/segtree.png)  
<sup>example segment tree used for sum queries</sup>  
It is assumed that the input array has a size equal to a power of two. If that is not the case, the original array is extended to the closest power of two. 
For the sake of efficiency the tree is stored as an array of ```2n``` elements (where ```n``` is the size of the original array extended to the closest power of two) with ```tree[ 1 ]``` being the root node and ```tree[ 2k ]``` and ```tree[ 2k+1 ]``` being the children of ```tree[ k ]```.    
<br/><sup>1</sup> The tests contain an example showing the usage of a segment tree for retreiving the second largest element on a specified range.  

## <a name = "algorithms"></a> algorithms<br/>

#### <a name = "algo1"></a> algo1<br/>
#### <a name = "algo2"></a> algo2<br/>

## philosophy

**natprolib** is a playground for learning about the STL and template metaprogramming by reimplementing the minimum set of STL features necessary to make natprolib independent.  
It's not necessarily intended to improve on all of those features, but suggestions for more elegant implementations are very welcome!
