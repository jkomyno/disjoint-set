#ifndef DISJOINT_SET_COMPRESSED_H
#define DISJOINT_SET_COMPRESSED_H

#include <utility>  // std::swap
#include <vector>   // std::vector

#include "DisjointSetBase.h"

namespace disjoint_set {

    /**
     * Compressed union-by-rank implementation of a Disjoint-Set.
     * DisjointSetCompressed accepts element of type T, which must be an unsigned integer type.
     */
    template <typename T>
    class DisjointSetCompressed : public DisjointSetBase<T> {
        using super = DisjointSetBase<T>;
        using parameter_t = typename super::parameter_t;

        // vector that keeps track of the rank of every element in the DisjointSet
        std::vector<std::size_t> ranks;

    public:
        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSetCompressed(const std::vector<T>& x_list) :
            super(x_list), ranks(x_list.size(), 0) {
        }  // fill ranks with value 0

        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSetCompressed(std::vector<T>&& x_list) :
            super(std::move(x_list)), ranks(x_list.size(), 0) {
        }  // fill ranks with value 0

        ~DisjointSetCompressed() = default;

        /**
         * Returns the index of the representative of the unique set containing the given item.
         * T must be cast-able to size_t.
         * find implements path compression via path splitting: every time find is called on a node,
         * it makes every node in the path point to its grandparent, effectively making the trees
         * flat or almost flat.
         * O(lg*(N)) (basically linear, lg*(265536) == 5)
         */
        [[nodiscard]] std::size_t find(parameter_t element) noexcept override {
            auto x = element;

            while (x != this->parents[x]) {
                // skip parent and point to grandparent
                this->parents[x] = this->parents[this->parents[x]];

                // move to grandparent
                x = this->parents[x];
            }

            return x;
        }

        /**
         * Unites the dynamic sets that contain x and y into a new set that is the union
         * of these two sets.
         * x should be different than y.
         * Uses union-by-rank policy: it makes the node whose set rank is smaller point to
         * the node whose set size is bigger, breaking ties arbitrarily. In case of tie,
         * the resulting set's rank is increased by 1.
         * The complexity of this method is the same of find + O(1)
         */
        void unite(parameter_t x, parameter_t y) noexcept override {
            auto i = find(x);
            auto j = find(y);

            if (i != j) {
                // if two sets are united and have different ranks,
                // the resulting set's rank is the larger of the two
                if (ranks[i] < ranks[j]) {
                    std::swap(i, j);
                }

                this->parents[j] = i;

                if (ranks[i] != ranks[j]) {
                    return;
                }

                // if two sets are united and have the same rank, the resulting
                // set's rank is one larger
                ++(ranks[i]);
            }
        }
    };

}  // namespace disjoint_set

#endif  // DISJOINT_SET_COMPRESSED_H
