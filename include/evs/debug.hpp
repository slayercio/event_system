#pragma once
#include <iostream>

#ifdef _DEBUG
#define EVS_DEBUG_EXPR(expression) \
{ \
    auto ret = expression; \
    std::cout << #expression << " => " << ret << std::endl; \
}

#define EVS_DEBUG(msg) \
{ \
    std::cout << "[DEBUG]: " << msg << std::endl; \
}

#define EVS_DEBUG_CALL() \
{ \
    std::cout << __PRETTY_FUNCTION__ << " called!" << std::endl; \
}
#else
#define EVS_DEBUG(msg)
#define EVS_DEBUG_EXPR(expression)
#endif
