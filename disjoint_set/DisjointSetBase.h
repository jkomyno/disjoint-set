#ifndef DISJOINT_SET_BASE_H
#define DISJOINT_SET_BASE_H

#include <algorithm>    // std::transform
#include <type_traits>  // std::enable_if, std::is_unsigned, std::conditional, std::is_arithmetic
#include <vector>       // std::vector

namespace disjoint_set {

    /**
     * Abstract base class of Disjoint-Set.
     * DisjointSet accepts element of type T, which must be an unsigned integer type.
     */
    template <typename T, typename = typename std::enable_if<std::is_unsigned<T>::value>::type>
    class DisjointSetBase {

        // initialize class
        void init(const std::vector<T>& x_list) {
            parents.reserve(x_list.size());

            // initially every item is the parent of itself
            std::transform(x_list.cbegin(), x_list.cend(), std::back_inserter(parents),
                           [&x_list](const auto& x_elem) {
                               // pointer arithmetic to obtain the index of the current element in
                               // x_list
                               return static_cast<std::size_t>(&x_elem - &(x_list[0]));
                           });
        }

    protected:
        // pass by constant value if type T is a simple arithmetic type, otherwise pass by const
        // reference.
        using parameter_t =
            typename std::conditional<std::is_arithmetic<T>::value, const T, const T&>::type;

        // vector that keeps track of the parent of every element in the DisjointSet
        std::vector<std::size_t> parents;

    public:
        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSetBase(const std::vector<T>& x_list) {
            init(x_list);
        }

        // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
        explicit DisjointSetBase(std::vector<T>&& x_list) {
            init(x_list);
        }

        // it's a base class, so the destructor must be virtual
        virtual ~DisjointSetBase() = default;

        /**
         * Returns the index of the representative of the unique set containing the given item.
         * T must be cast-able to size_t.
         */
        [[nodiscard]] virtual std::size_t find(parameter_t element) noexcept = 0;

        /**
         * Unites the dynamic sets that contain x and y into a new set that is the union
         * of these two sets.
         * x should be different than y.
         * The complexity of this method is the one of find + O(1)
         */
        virtual void unite(parameter_t x, parameter_t y) noexcept = 0;

        /**
         * returns true iff elements x and y are in the same set.
         */
        [[nodiscard]] bool are_connected(parameter_t x, parameter_t y) noexcept {
            return find(x) == find(y);
        }
    };

}  // namespace disjoint_set

#endif  // DISJOINT_SET_BASE_H
