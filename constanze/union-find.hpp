/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/constanze
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef CONSTANZE_UNION_FIND_HPP
#define CONSTANZE_UNION_FIND_HPP

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
        auto found = data.find(e);
        if (found == data.end())
            throw std::logic_error{"element not found"};
        if (found->second == nullptr)
            return 0;
        else
            return 1 + depth(*found->second);
    }
    element_t root(element_t e) {
        auto found = data.find(e);
        if (found == data.end())
            throw std::logic_error{"element not found"};
        if (found->second == nullptr)
            return e;
        auto result = root(*found->second);
        found->second = std::make_shared<T>(result);
        return result;
    }

    element_t root(element_t e) const {
        auto found = data.find(e);
        if (found == data.end())
            throw std::logic_error{"element not found"};
        if (found->second == nullptr)
            return e;
        return root(*found->second);
    }

    self& add(element_t const& v) {
        data[v] = nullptr;
        return *this;
    }
    self& add(std::initializer_list<element_t> const& list) {
        for (auto const& v : list) {
            if (&v == list.begin())
                data[v] = nullptr;
            else
                data[v] = std::make_shared<element_t>(*list.begin());
        }
        return *this;
    }
    bool exist(element_t const& e) const {
        return data.find(e) != data.end();
    }

    self& unite(element_t const& lhs, element_t const& rhs) {
        auto lhs_root = root(lhs);
        auto rhs_root =  root(rhs);
        if (depth(lhs) < depth(rhs))
            data[lhs_root] = std::make_shared<element_t>(rhs_root);
        else
            data[rhs_root] = std::make_shared<element_t>(lhs_root);
        return *this;
    }
    bool find(element_t const& lhs, element_t const& rhs) {
        return root(lhs) == root(rhs);
    }

    std::map<element_t, std::shared_ptr<element_t>> data; // element to parent
};

template<typename T> inline static std::ostream&
operator<<(std::ostream& os, union_find<T> const& uf) {
    //     root     nodes
    std::map<T, std::vector<T>> groups;
    for (auto const& e : uf.data) {
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
