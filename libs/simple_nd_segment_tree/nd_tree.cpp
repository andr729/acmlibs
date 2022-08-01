/**
 * @file nd_tree.cpp
 * @brief TBD
 */

#include <iostream>
#include <array>
#include <initializer_list>

using dim_t = uint64_t;

// forward:
template<uint n>
struct ShapeND;

template<uint n>
struct RangeND;

template<uint n, uint org>
ShapeND<n> shape_maker_aux(const std::array<dim_t, org>& arr, int index);

template<uint n>
ShapeND<n> shape_maker(const std::array<dim_t, n>& arr);

template<uint n, uint org>
RangeND<n> range_maker_aux(const std::array<std::pair<dim_t, dim_t>, org>& arr, int index);

template<uint n>
RangeND<n> range_maker(const std::array<std::pair<dim_t, dim_t>, n>& arr);

// simple range types:

template<uint n>
struct RangeND {
    dim_t from, to; // inclusive
    RangeND<n - 1> next;
    RangeND() {}
    RangeND(const std::array<std::pair<dim_t, dim_t>, n>& arr) {
        *this = range_maker<n>(arr);
    }
};

template<>
struct RangeND<0> {};

template<uint n>
struct ShapeND {
    dim_t size;
    ShapeND<n - 1> next;
    ShapeND() {}
    ShapeND(const std::array<dim_t, n>& arr) {
        *this = shape_maker<n>(arr);
    }
};

template<>
struct ShapeND<0> {};

template<uint n, uint org>
ShapeND<n> shape_maker_aux(const std::array<dim_t, org>& arr, int index) {
    ShapeND<n> out;
    if constexpr(n >= 1) {
        out.size = arr[index];
        out.next = shape_maker_aux<n - 1, org>(arr, index + 1);
    }
    return out;
}

template<uint n>
ShapeND<n> shape_maker(const std::array<dim_t, n>& arr) {
    return shape_maker_aux<n, n>(arr, 0);
}

template<uint n, uint org>
RangeND<n> range_maker_aux(const std::array<dim_t, org>& arr, int index) {
    RangeND<n> out;
    if constexpr(n >= 1) {
        out.from = arr[index].first;
        out.to = arr[index].second;
        out.next = range_maker_aux<n - 1, org>(arr, index + 1);
    }
    return out;
}

template<uint n>
RangeND<n> range_maker(const std::array<dim_t, n>& arr) {
    return range_maker_aux<n, n>(arr, 0);
}

template<uint n, class ValType>
struct TreeNode;

// main tree logic:

template<uint n, class ValType>
struct TreeNDAux {
    const ShapeND<n>& shape;
    TreeNDAux(const  ShapeND<n>& shape): shape(shape) {

    }
};

// main tree logic, node:

template<uint n, class ValType>
struct TreeNode {
    static_assert(n > 1);
    TreeNDAux<n - 1, ValType> sub_tree;
};

template<class ValType>
struct TreeNode<1, ValType> {
    ValType leaf_val;
};

// top-level wrapper + interface:
// todo: ValType concept
template<uint n, class ValType>
class TreeND {
    ShapeND<n> shape;
    TreeNDAux<n, ValType> tree;
public:
    TreeND(ShapeND<n> shape):
        shape(shape),
        tree(shape) {
    }
};



int main() {
    TreeND<3, int> test_tree(shape_maker<3>({56, 3, 3}));
    TreeND<3, int> test_tree_2(ShapeND<3>({56, 3, 3}));
}