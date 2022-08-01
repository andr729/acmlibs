/**
 * @file nd_tree.cpp
 * @brief TBD
 */

#include <iostream>

using dim_t = uint64_t;

// simple range types:

template<uint n>
struct RangeND {
    dim_t from, to; // inclusive
    RangeND<n - 1> next;
};

template<>
struct RangeND<0> {
    dim_t from, to;
};

template<uint n>
struct ShapeND {
    dim_t size;
    ShapeND<n - 1> next;
};

template<>
struct ShapeND<0> {
    dim_t size;
};

template<uint n, class ValType>
struct TreeNode {

};

template<class ValType>
struct TreeNode<0, ValType> {
    
};

template<uint n, class ValType>
struct TreeNDAux {
    const ShapeND<n>& shape;
    TreeND(const  ShapeND<n>& shape): shape(shape) {

    }
};

// template<class ValType>
// struct TreeNDAux<0, ValType> {

// };

template<uint n, class ValType>
class TreeND {
    ShapeND<n> shape;
    TreeNDAux<n> tree;
    TreeND(ShapeND<n> shape):
        shape(shape),
        tree(shape) {
    }
};



int main() {
    RangeND<3> hmm;
}