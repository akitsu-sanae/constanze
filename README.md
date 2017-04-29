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

using namespace keika;

Result<double> divide(double a, double b) {
    if (b == 0.0)
        return Result<double>::error("division by zero!!");
    else
        return a / b;
}

int main() {
    std::cout << divide(1.0, 1.0) << std::endl; // "Result::ok(1)"
    std::cout << divide(1.0, 0.0) << std::endl; // "Result::error(division by zero!!)"
}
```

# Copyright
Copyright (C) 2017 akitsu sanae.  
Distributed under the Boost Software License, Version 1.0. 
(See accompanying file LICENSE or copy at http://www.boost/org/LICENSE_1_0.txt)  


