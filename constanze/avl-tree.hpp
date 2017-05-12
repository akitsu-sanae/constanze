/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/constanze
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef CONSTANZE_AVL_TREE_HPP
#define CONSTANZE_AVL_TREE_HPP

#include <memory>
#include <ostream>

namespace constanze {

template<typename> struct avl_tree;

namespace detail {
template<typename T>
void print_avl(std::ostream&, avl_tree<T> const&, std::string const&);
}

template<typename T>
struct avl_tree {
    using element_type = T;
    using value_type = T;
    using child_type = std::unique_ptr<avl_tree<element_type>>;

    //
    //    construction functions
    //
    explicit avl_tree(element_type const& v) :
        value{v}
    {}

    explicit avl_tree(element_type const& v, child_type&& l, child_type r) :
        value{v},
        left{std::move(l)}, right{std::move(r)}
    {}

    static child_type with_left(element_type const& v, child_type&& left) {
        return std::make_unique<avl_tree<element_type>>(v, std::move(left), nullptr);
    }
    static child_type with_right(element_type const& v, child_type&& right) {
        return std::make_unique<avl_tree<element_type>>(v, nullptr, std::move(right));
    }

    // display
    template<typename U>
    friend void detail::print_avl(std::ostream&, avl_tree<U> const&, std::string const&);

    //
    // queries
    //
    element_type const* find(element_type const& v) const {
        if (v < value) {
            if (left)
                return left->find(v);
            else
                return nullptr;
        } else if(v > value) {
            if (right)
                return right->find(v);
            else
                return nullptr;
        } else {
            return &value;
        }
    }

    bool exist(element_type const& v) const {
        return find(v) != nullptr;
    }

    void insert(element_type const& v) {
        if (v < value) {
            if (left)
                left->insert(v);
            else
                left = std::make_unique<avl_tree<element_type>>(v);
        } else if(v > value) {
            if (right)
                right->insert(v);
            else
                right = std::make_unique<avl_tree<element_type>>(v);;
        } else {
            throw std::logic_error{"already exist"};
        }
        set_height();
        auto l = left? left->height : 0;
        auto r = right? right->height: 0;
        if (r - l >= 2) {
            auto l = right->left? right->left->height: 0;
            auto r = right->right? right->right->height: 0;
            if (l > r)
                right->rotate_right();
            rotate_left();
        } else if (r - l <= -2) {
            auto l = left->left? left->left->height: 0;
            auto r = left->right? left->right->height: 0;
            if (r > l)
                left->rotate_left();
            rotate_right();
        }
    }

private:
    void rotate_right() {
        if (!left)
            throw std::logic_error{"invalid rotation right"};
        right = std::make_unique<avl_tree<element_type>>(
                value,
                std::move(left->right),
                std::move(right));
        value = left->value;
        left = std::move(left->left);
        if (left)
            left->set_height();
        right->set_height();
    }

    void rotate_left() {
        if (!right)
            throw std::logic_error{"invalid rotation left"};
        left = std::make_unique<avl_tree<element_type>>(
                value,
                std::move(left),
                std::move(right->left));
        value = right->value;
        right = std::move(right->right);
        left->set_height();
        if (right)
            right->set_height();
    }

    void set_height() {
        if (left) left->set_height();
        if (right) right->set_height();

        if (left && right)
            height = std::max(left->height, right->height) + 1;
        else if (left)
            height = left->height + 1;
        else if (right)
            height = right->height + 1;
        else
            height = 1;
    }

    int height = 1;
    child_type left, right;
    element_type value;
};

namespace detail {
template<typename T>
inline static void print_avl(
        std::ostream& os,
        avl_tree<T> const& tree,
        std::string const& indent)
{
    std::string next_indent = indent + "    ";
    if (tree.left)
        print_avl(os, *tree.left, next_indent);
    os << indent << tree.value << std::endl;
    if (tree.right)
        print_avl(os, *tree.right, next_indent);
}
}

template<typename T>
inline static std::ostream& operator<<(std::ostream& os, avl_tree<T> const& tree) {
    detail::print_avl(os, tree, "");
}

}

#endif
