/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/constanze
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef CONSTANZE_SEGMENT_TREE_HPP
#define CONSTANZE_SEGMENT_TREE_HPP

#include <cstddef>
#include <array>
#include <ostream>

// TODO: remove this
#include <iostream>

namespace constanze {

namespace detail {

constexpr bool is_power_of_n(std::size_t n) {
    while (n <= 1) {
        if (n%2 == 1)
            return false;
        else
            n /= 2;
    }
    return true;
}


};

template<typename T, std::size_t N, typename F>
struct segment_tree {
    using is_power = typename std::enable_if<detail::is_power_of_n(N)>::type;
    using value_type = T;
    using element_type = T;

    explicit segment_tree(std::array<T, N> const& arr, F const& f) :
        f{f}
    {
        for (std::size_t i=0; i<N; ++i)
            data[N-1+i] = arr[i];

        for (int i=N-2; i>=0; --i)
            data[i] = f(data[2*i+1], data[2*i+2]);
    }

    value_type const& get(std::size_t i) const {
        return data[N-1+i];
    }
    void update(std::size_t i, value_type const& v) {
        data[N-1+i] = v;
        update((i-1)/2);
    }
    void update(std::size_t i, value_type&& v) {
        data[N-1+i] = std::move(v);
        update((i-1)/2);
    }

    T min_in_range(int l, int r) const {
        return min_in_range_impl(l, r, 0, 0, N-1);
    }

    T min_in_range_impl(std::size_t l, std::size_t r, std::size_t i, std::size_t ll, std::size_t rr) const {
        if (l <= ll && rr <= r)
            return data[i];

        if (l >= (ll+rr)/2+1)
            return min_in_range_impl(l, r, i*2+2, (ll+rr)/2+1, rr);

        if (r <= (ll+rr)/2)
            return min_in_range_impl(l, r, i*2+1, ll, (ll+rr)/2);

        auto left_v = min_in_range_impl(l, r, i*2+1, ll, (ll+rr)/2);
        auto right_v = min_in_range_impl(l, r, i*2+2, (ll+rr)/2+1, rr);
        return f(left_v, right_v);
    }

    std::array<value_type, 2*N-1> array() const { return data; }
private:

    void update(std::size_t i) {
        data[N-1+i] = f(data[2*i+1], data[2*i+2]);
        if (i != 0)
            update((i-1)/2);
    }

    std::array<value_type, 2*N-1> data;
    F const& f;
};

template<typename T, std::size_t N, typename F>
segment_tree<T, N, F> make_segment_tree(std::array<T, N> const& data, F const& f) {
    return segment_tree<T, N, F>{data, f};
}

template<typename T, std::size_t N, typename F>
std::ostream& operator<<(std::ostream& os, segment_tree<T, N, F> const& tree) {
    for (auto const& e : tree.array())
        os << e << ",";
    os << std::flush;
    return os;
}

};

#endif

