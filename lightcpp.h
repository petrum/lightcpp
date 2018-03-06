#pragma once

#include <stdexcept>
#include <sstream>
#include <iostream>

template <typename T>
T enforce(T t, const char* e, const char* f, int n)
{
    if (!t)
    {
        std::ostringstream o;
        o << "The '" << e << "' expression has failed in " << f << ":" << n;
        throw std::runtime_error(o.str());
    }
}

#define ENFORCE(expr) enforce(expr, #expr, __FILE__, __LINE__)

#define STD_FUNC_BEGIN try {
#define STD_FUNC_END } catch (const std::exception& e) \
{\
    std::cerr << e.what() << std::endl;\
}\
catch (...)\
{\
    std::cerr << "Non standard error" << std::endl;\
}

