//
// Created by 赵润朔 on 2024/1/14.
//

#ifndef RSMESH_SUM_ACCUMULATOR_H
#define RSMESH_SUM_ACCUMULATOR_H

#include <cstddef>
#include <type_traits>

namespace rsmesh::numeric {
    template<class Floating, typename std::enable_if<std::is_floating_point<Floating>::value, std::nullptr_t>::type = nullptr>
    class sum_accumulator {
    public:
        sum_accumulator() : sum_(), correction_() {}
        Floating get() const {return sum_;}
        sum_accumulator& operator+=(Floating f) {
            auto summand = f + correction_;
            auto next_sum = sum_ + summand;
            correction_ = summand - (next_sum - sum_);
            sum_ = next_sum;
            return *this;
        }
    private:
        Floating sum_;
        Floating correction_;
    };
} // namespace rsmesh::numeric

#endif //RSMESH_SUM_ACCUMULATOR_H
