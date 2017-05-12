# Constanze

Constanze is a C++ header-only library for Algorithm and Data-Structures

# Examples

UnionFind
```cpp
#include <constanze/union-find.hpp>

int main() {
    constanze::union_find<int> uf_tree;
    uf_tree
        .add({1, 3})
        .add({2, 4, 6})
        .add(5);

    std::cout << std::boolalpha;
    std::cout << "2 and 6: " << uf_tree.find(2, 6) << std::endl; // true
    std::cout << "2 and 3: " << uf_tree.find(2, 3) << std::endl; // false
    std::cout << uf_tree << std::endl; // [1, 3][2, 4, 6][5]
    uf_tree.unite(1, 5);
    std::cout << uf_tree << std::endl; // [1, 3, 5][2, 4, 6]
    uf_tree.unite(1, 2);
    std::cout << uf_tree << std::endl; // [1, 2, 3, 4, 5, 6]
}
```

SegmentTree
```cpp
#include <constanze/segment-tree.hpp>
#include <iostream>

int main() {
    std::array<int, 8> data {
        2, 6, 3, 0, 4, 5, 12, 52
    };
    auto tree = constanze::make_segment_tree(
            data,
            [](int const& lhs, int const& rhs) {
                if (lhs < rhs)
                    return lhs;
                else
                    return rhs;
            });
    std::cout << tree.min_in_range(5, 7) << std::endl; // 5
}

```

AVL tree
```cpp
#include "constanze/avl-tree.hpp"
#include <iostream>

int main() {
    using tree_t = constanze::avl_tree<int>;
    auto tree = tree_t{3};
    while (true) {
        std::cout << tree;
        std::string input;
        std::cout << "next value: " << std::flush;
        std::cin >> input;
        if (input == "quit" || input == "q")
            break;
        tree.insert(std::stoi(input));
    }
}
```

# Copyright
Copyright (C) 2017 akitsu sanae.  
Distributed under the Boost Software License, Version 1.0. 
(See accompanying file LICENSE or copy at http://www.boost/org/LICENSE_1_0.txt)  


