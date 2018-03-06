#pragma once

#include <stdexept>

template <classname T>
T enforce(T t, const char* e, const char* f, int n)
{
    if (!t)
        throw std::runtime_error();
}
