//
// Created by RainSure on 2023/10/26.
//

#ifndef RSMESH_PI_H
#define RSMESH_PI_H

#include <cmath>

namespace rsmesh {
    namespace common {
        /**
         * @brief 计算pi的值
         * @tparam T 模板参数
         * @return 返回pi的值
         */
        template<class T>
        T pi() {
            return T(4.0) * std::atan(T(1.0)); // 4 * pi / 4
        }
    } // common
} // rsmesh


#endif //RSMESH_PI_H
