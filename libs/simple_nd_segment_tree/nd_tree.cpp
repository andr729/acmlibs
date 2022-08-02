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
RangeND<n> range_maker_aux(const std::array<std::pair<dim_t, dim_t>, org>& arr, int index) {
    RangeND<n> out;
    if constexpr(n >= 1) {
        out.from = arr[index].first;
        out.to = arr[index].second;
        out.next = range_maker_aux<n - 1, org>(arr, index + 1);
    }
    return out;
}

template<uint n>
RangeND<n> range_maker(const std::array<std::pair<dim_t, dim_t>, n>& arr) {
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
    typedef typename ValType::QueryType QueryType;
    ShapeND<n> shape;
    TreeNDAux<n, ValType> tree;
public:
    TreeND(ShapeND<n> shape):
        shape(shape),
        tree(shape) {
    }

    void query(RangeND<n> range, QueryType& query) {

    }
};

enum QueryType {
    Add, GetMax
};

struct QueryMessage {
    QueryType type;
    int64_t val;
};

struct AddSubMaxValue {
    typedef QueryMessage QueryType;
    
    int64_t max;
    int64_t lazy_add;

    AddSubMaxValue() { max = 0; lazy_add = 0; }

    // it might be needed to add left_range, right_range here:
    void update_from_children(const AddSubMaxValue& left, const AddSubMaxValue& right) {
        max = std::max(left.max, right.max);
        lazy_add = 0;
    }

    void propagate_down(AddSubMaxValue& left, AddSubMaxValue& right) {
        left.add(lazy_add);
        right.add(lazy_add);
        lazy_add = 0;
    }

    // when my shape covers query range:
    void query(QueryMessage& mess) {
        switch (mess.type) {
            case GetMax:
                mess.val = std::max(mess.val, max);
                break;
            case Add:
                add(mess.val);
                break;
        }
    }

    template<uint n>
    static AddSubMaxValue make_leaf(ShapeND<n> pos) {
        return AddSubMaxValue();
    }

private:
    void add(int64_t v) {
        max += v;
        lazy_add += v;
    }
};


int main() {
    TreeND<3, AddSubMaxValue> test_tree(shape_maker<3>({56, 3, 3}));
    TreeND<3, AddSubMaxValue> test_tree_2(ShapeND<3>({56, 3, 3}));

    QueryMessage mess;
    test_tree_2.query(RangeND<3>({{{51, 55}, {1, 1}, {1, 2}}}), mess);
    std::cout << mess.val << "\n";
}