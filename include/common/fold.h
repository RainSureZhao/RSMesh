//
// Created by RainSure on 2023/10/26.
//

#ifndef RSMESH_FOLD_H
#define RSMESH_FOLD_H

#include <utility>

namespace rsmesh {
    namespace common {
        /**
         * 模板递归终止函数，此时只有一个参数，直接返回init
         * @tparam BinaryFunction 二元函数
         * @tparam T 参数类型
         * @param f 二元函数形参
         * @param init 初始值
         * @return 直接返回初始值
         */
        template<class BinaryFunction, class T>
        auto fold_left(BinaryFunction f, const T& init) {
            return init;
        }
        /**
         * 模板递归函数，从当前的参数列表中取出前两个参数，然后调用二元函数f，将结果作为新的参数，递归调用下去，最终得到全部参数参与计算的结果
         * @tparam BinaryFunction 二元函数类型
         * @tparam T1 形参类型1
         * @tparam T2 形参类型2
         * @tparam Ts 模板参数包，匹配零个或多个额外类型的参数
         * @param f 二元函数实参
         * @param x1 形参1
         * @param x2 形参2
         * @param xs 形参包
         * @return 返回全部参数从左到右调用二元函数f计算的结果
         */
        template<class BinaryFunction, class T1, class T2, class... Ts>
        auto fold_left(BinaryFunction f, T1&& x1, T2&& x2, Ts&&... xs) {
            return fold_left(f, f(std::forward<T1>(x1), std::forward<T2>(x2)), std::forward<Ts>(xs)...);
        }
        /**
         * 模板递归终止函数，此时只有一个参数，直接返回init
         * @tparam BinaryFunction 二元函数类型
         * @tparam T 形参类型
         * @param f 二元函数形参，此时无用
         * @param init 形参，此时直接返回
         * @return 返回init
         */
        template<class BinaryFunction, class T>
        auto fold_right(BinaryFunction f, const T& init) {
            return init;
        }

        /**
         * 模板递归函数，计算当前形参包的结果然后和x1进行计算，得到全部参数从右向左调用二元函数f计算的结果
         * @tparam BinaryFunction 二元函数类型
         * @tparam T1 形参类型1
         * @tparam T2 形参类型2
         * @tparam Ts 模板参数包，匹配零个或多个额外类型的参数
         * @param f 二元函数形参
         * @param x1 形参1
         * @param x2 形参2
         * @param xs 模板参数包
         * @return
         */
        template<class BinaryFunction, class T1, class T2, class... Ts>
        auto fold_right(BinaryFunction f, T1&& x1, T2&& x2, Ts&&... xs) {
            return f(std::forward<T1>(x1), fold_right(f, std::forward<T2>(x2), std::forward<Ts>(xs)...));
        }
    } // common
} // rsmesh


#endif //RSMESH_FOLD_H
