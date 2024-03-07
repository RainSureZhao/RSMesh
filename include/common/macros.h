//
// Created by RainSure on 2023/10/25.
//

#ifndef RSMESH_MACROS_H
#define RSMESH_MACROS_H

#include <cassert>

#define RSMESH_ASSERT(X) assert(X)

#define REMESH_NEVER_REACH(X) assert(false)

#define RSMESH_LIKELY(X) (X)
#define RSMESH_UNLIKELY(X) (X)

namespace rsmesh {
    /**
     * \brief 用于标记不可达的代码
     */
    inline void RSMESH_UNREACHABLE() {
        assert(false);
    }
}

#endif //RSMESH_MACROS_H
