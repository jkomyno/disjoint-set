#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <vector>  // std::vector

#include "DisjointSetBase.h"

namespace disjoint_set {

    /**
     * Simple union-by-size implementation of a Disjoint-Set.
     * DisjointSet accepts element of type T, which must be an unsigned integer type.
     */
    template <typename T>
    class DisjointSet : public DisjointSetBase<T> {
        using super = DisjointSetBase<T>;
        using parameter_t = typename super::parameter_t;

        // vector that keeps track of the size of every element in the DisjointSet
        std::vector<std::size_t> sizes;

    public:
        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSet(const std::vector<T>& x_list) :
            super(x_list), sizes(x_list.size(), 1) {
        }  // fill sizes with value 1

        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSet(std::vector<T>&& x_list) :
            super(std::move(x_list)), sizes(x_list.size(), 1) {
        }  // fill sizes with value 1

        ~DisjointSet() = default;

        /**
         * Returns the index of the representative of the unique set containing the given item.
         * T must be cast-able to size_t.
         * O(logN)
         */
        [[nodiscard]] std::size_t find(parameter_t element) noexcept override {
            auto x = element;

            while (x != this->parents[x]) {
                // move towards the parent
                x = this->parents[x];
            }
            return x;
        }

        /**
         * Unites the dynamic sets that contain x and y into a new set that is the union
         * of these two sets.
         * x should be different than y.
         * Uses union-by-size policy: it makes the node whose set size is smaller point to
         * the node whose set size is bigger, breaking ties arbitrarily.
         * The complexity of this method is the one of find + O(1)
         */
        void unite(parameter_t x, parameter_t y) noexcept override {
            const auto i = find(x);
            const auto j = find(y);

            // if the two nodes are the same set, do nothing
            if (i != j) {
                if (sizes[i] < sizes[j]) {
                    this->parents[i] = j;
                    sizes[j] += sizes[i];
                } else {
                    this->parents[j] = i;
                    sizes[i] += sizes[j];
                }
            }
        }
    };

}  // namespace disjoint_set

#endif  // DISJOINT_SET_H
