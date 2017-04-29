/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/constanze
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef CONSTANZE_HPP
#define CONSTANZE_HPP

#include <cstddef>
#include <memory>
#include <map>
#include <vector>
#include <iostream>

namespace constanze {

template<typename T>
struct union_find {
    using self = union_find;
    using element_t = T;
    using value_t = T;

    std::size_t depth(element_t e) const {
        std::size_t result = 0;
        while (true) {
            auto found = edges.find(e);
            if (found == edges.end())
                throw std::logic_error{"element not found"};
            if (found->second == nullptr)
                break;
            e = *found->second;
            ++result;
        }
        return result;
    }
    element_t root(element_t e) const {
        while (true) {
            auto found = edges.find(e);
            if (found == edges.end())
                throw std::logic_error{"element not found"};
            if (found->second == nullptr)
                break;
            e = *found->second;
        }
        return e;
    }

    self& add(element_t const& v) {
        edges[v] = nullptr;
        return *this;
    }
    self& add(std::initializer_list<element_t> const& list) {
        for (auto const& v : list) {
            if (&v == list.begin())
                edges[v] = nullptr;
            else
                edges[v] = std::make_shared<element_t>(*list.begin());
        }
        return *this;
    }

    self& unite(element_t const& lhs, element_t const& rhs) {
        auto lhs_root = root(lhs);
        auto rhs_root =  root(rhs);
        if (depth(lhs) < depth(rhs))
            edges[lhs_root] = std::make_shared<element_t>(rhs_root);
        else
            edges[rhs_root] = std::make_shared<element_t>(lhs_root);
        return *this;
    }
    bool find(element_t const& lhs, element_t const& rhs) const {
        return root(lhs) == root(rhs);
    }

    std::map<element_t, std::shared_ptr<element_t>> edges; // element to parent
};

template<typename T> inline static std::ostream&
operator<<(std::ostream& os, union_find<T> const& uf) {
    //     root     nodes
    std::map<T, std::vector<T>> groups;
    for (auto const& e : uf.edges) {
        auto root = uf.root(e.first);
        groups[root].push_back(e.first);
    }
    for (auto const& group : groups) {
        os << "[";
        for (auto it=group.second.begin(); it!=group.second.end(); ++it) {
            if (it != group.second.begin())
                os << ", ";
            os << *it;
        }
        os << "]";
    }
    return os;
}

}

#endif
