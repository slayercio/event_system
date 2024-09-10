#pragma once
#include <iostream>
#include <sstream>

#ifdef _MSC_VER
#   define EVS_FUNCSIG __FUNCSIG__
#else
#   define EVS_FUNCSIG __PRETTY_FUNCTION__
#endif

#ifdef _DEBUG
#define EVS_DEBUG_EXPR(expression) \
{ \
    auto ret = expression; \
    std::ostringstream oss; \
    oss << #expression << " => " << ret << "\n"; \
    std::cout << oss.str(); \
}

#define EVS_DEBUG(msg) \
{ \
    std::ostringstream oss; \
    oss << "[DEBUG]: " << msg << "\n"; \
    std::cout << oss.str(); \
}

#define EVS_DEBUG_CALL() \
{ \
    std::ostringstream oss; \
    oss << EVS_FUNCSIG << " called!" << "\n"; \
    std::cout << oss.str(); \
}
#else
#define EVS_DEBUG(msg)
#define EVS_DEBUG_EXPR(expression)
#endif
