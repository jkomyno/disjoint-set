# Disjoint Set

[![Build Status](https://travis-ci.org/jkomyno/disjoint-set.svg?branch=master)](https://travis-ci.org/jkomyno/disjoint-set)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/19388989c62742ba8353c577cd92b39c)](https://www.codacy.com/manual/jkomyno/disjoint-set?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jkomyno/disjoint-set&amp;utm_campaign=Badge_Grade)

This package provides a header-only, generic and dependency-free C++17 implementation of [Disjoint Sets](https://en.wikipedia.org/wiki/Disjoint-set_data_structure).
It exposes the namespace `disjoint_set`.

**DISCLAIMER**: This simple implementation only allows unsigned numerical elements from `0` to `N-1`, where `N` is the cardinality of elements you want to store in a Disjoint Set.

## Table of Contents

-   [Disjoint Set operations](#disjoint-set-operations)
-   [Union-by-size Disjoint Set](#union-by-size-disjoint-set)
-   [Union-by-rank compressed Disjoint Set](#union-by-rank-compressed-disjoint-set)
    -   [Example usage of Disjoint Set](#example-usage-of-disjoint-set)
-   [Best practices adopted](#best-practices-adopted)
-   [Test](#test)
-   [Contributing](#contributing)

## Disjoint Set operations

The contract defined in the abstract class [`DisjointSetBase`](./disjoint_set/DisjointSetBase.h) declares the following methods:

-   `std::size_t find(parameter_t element)`: returns the index of `element`.
-   `void unite(parameter_t x, parameter_t y)`: unites the sets of `x` and `y`.
-   `bool are_connected(parameter_t x, parameter_t y)`: returns true iff elements x and y are in the same set.

parameter_t is either `T` (pass-by-value) or `const T&` (pass-by-constant-reference), where `T` is the template argument of `DisjointSetBase`,
depending on whether `T` is a trivial numeric type or not. It's a little optimization that prevents using constant references where pass-by-value would be much cheaper.

## Union-by-size Disjoint Set

[`DisjointSet`](./disjoint_set/DisjointSet.h) contains a concrete DisjointSetBase implementation that uses the `union-by-size` policy.
The asymptotic time complexity of its methods are defined in the table below.

| Method              | Time     | Description                                             |
| ------------------- | -------- | ------------------------------------------------------- |
| find(element)       | O(logN)  | returns the index of `element`.                         |
| unite(x, y)         | O(2logN) | unites the sets of `x` and `y` and updates their sizes. |
| are_connected(x, y) | O(2logN) | returns true iff elements x and y are in the same set.  |

## Union-by-rank compressed Disjoint Set

[`DisjointSetCompressed`](./disjoint_set/DisjointSetCompressed.h) contains a concrete DisjointSetBase implementation that uses the `union-by-rank` policy
paired with [`path-compression`](https://en.wikipedia.org/wiki/Disjoint-set_data_structure#Path_compression). The compression technique that was used is [`path-splitting`](https://en.wikipedia.org/wiki/Disjoint-set_data_structure#Path_splitting).
The asymptotic time complexity of its methods are defined in the table below.

| Method              | Time        | Description                                             |
| ------------------- | ----------- | ------------------------------------------------------- |
| find(element)       | O(lg\*(N))  | returns the index of `element`.                         |
| unite(x, y)         | O(2lg\*(N)) | unites the sets of `x` and `y` and updates their sizes. |
| are_connected(x, y) | O(2lg\*(N)) | returns true iff elements x and y are in the same set.  |

lg\* indicates the [Iterated Logarithm](https://en.wikipedia.org/wiki/Iterated_logarithm).

### Example usage of Disjoint Set

```c++
#include <iostream>
#include <vector>
#include "disjoint_set/DisjointSet.h"

int main() {
    std::vector<std::size_t> vec{0, 1, 2, 3, 4, 5};

    disjoint_set::DisjointSet d_set(vec);

    /**
     * Set representation:
     *     ______        ______        ______        ______        ______        ______     
     *   *        *    *        *    *        *    *        *    *        *    *        *  
     *  *    0     *  *    1     *  *    2     *  *    3     *  *    4     *  *    5     * 
     *  *          *  *          *  *          *  *          *  *          *  *          * 
     *   * ______ *    * ______ *    * ______ *    * ______ *    * ______ *    * ______ *  
     */

    d_set.unite(0, 1);

    /**
     * Set representation:
     *     ______        ______        ______        ______        ______     
     *   *        *    *        *    *        *    *        *    *        *  
     *  *   0, 1   *  *    2     *  *    3     *  *    4     *  *    5     * 
     *  *          *  *          *  *          *  *          *  *          * 
     *   * ______ *    * ______ *    * ______ *    * ______ *    * ______ *  
     */

    d_set.unite(2, 3);

    /**
     * Set representation:
     *     ______        ______        ______        ______     
     *   *        *    *        *    *        *    *        *  
     *  *   0, 1   *  *   2, 3   *  *    4     *  *    5     * 
     *  *          *  *          *  *          *  *          * 
     *   * ______ *    * ______ *    * ______ *    * ______ *  
     */

    d_set.unite(2, 0);

    /**
     * Set representation:
     *     ______        ______        ______     
     *   *        *    *        *    *        *  
     *  *   0, 1,  *  *    4     *  *    5     * 
     *  *   2, 3   *  *          *  *          * 
     *   * ______ *    * ______ *    * ______ *  
     */

    auto bool_to_string = [](const bool v) {
        return v ? "true" : "false";
    };

    std::cout << "Are 0 and 1 connected? " << bool_to_string(d_set.are_connected(0, 1))
              << std::endl;
    std::cout << "Are 2 and 3 connected? " << bool_to_string(d_set.are_connected(2, 3))
              << std::endl;
    std::cout << "Are 3 and 1 connected? " << bool_to_string(d_set.are_connected(3, 1))
              << std::endl;
    std::cout << "Are 4 and 5 connected? " << bool_to_string(d_set.are_connected(4, 5))
              << std::endl;

    // OUTPUT:
    // Are 0 and 1 connected? true
    // Are 2 and 3 connected? true
    // Are 3 and 1 connected? true
    // Are 4 and 5 connected? false
}
```

## Best practices adopted

-   Most of the methods offered by this library are marked with `noexcept`.
-   Every overriden method is marked with `override`.
-   This library doesn't perform any heap allocation, at least not directly. Everything is stack-allocated
    and built around containers of the standard library (`std::vector`).

## Test

This project has been developer with Microsoft Visual Studio 2019, but it supports multiplatform builds via [`Bazel`](https://bazel.build/).

The [./tests](./tests) folder contains some unit tests. Coverage isn't 100% as of now, though these libraries have worked well in practice
with more than 90k elements.
To run the [`googletest`](https://github.com/google/googletest) test suite, just execute the following command:

-   `bazel test //..`

A note for Windows Users: run that command either in Powershell or in CMD, because it won't work in Git Bash.

## Contributing

Contributions, issues and feature requests are welcome!
The code is short and throughly commented, so you should feel quite comfortable looking at it.
If you have any doubt or suggestion, please open an issue.

Don't be afraid to show your support. Give a ⭐️ if this project helped or inspired you!.
