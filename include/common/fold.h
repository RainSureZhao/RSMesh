//
// Created by 赵润朔 on 2023/10/26.
//

#ifndef RSMESH_FOLD_H
#define RSMESH_FOLD_H

#include <utility>

namespace rsmesh {
    namespace common {
        
        template<class BinaryFunction, class T>
        auto fold_left(BinaryFunction f, const T& init) {
            return init;
        }
        template<class BinaryFunction, class T1, class T2, class... Ts>
        auto fold_left(BinaryFunction f, T1&& x1, T2&& x2, Ts&&... xs) {
            return fold_left(f, f(std::forward<T1>(x1), std::forward<T2>(x2)), std::forward<Ts>(xs)...);
        }
        
        template<class BinaryFunction, class T>
        auto fold_right(BinaryFunction f, const T& init) {
            return init;
        }
        
        template<class BinaryFunction, class T1, class T2, class... Ts>
        auto fold_right(BinaryFunction f, T1&& x1, T2&& x2, Ts&&... xs) {
            return f(std::forward<T1>(x1), fold_right(f, std::forward<T2>(x2), std::forward<Ts>(xs)...));
        }
    } // common
} // rsmesh


#endif //RSMESH_FOLD_H
