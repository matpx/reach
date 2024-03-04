#pragma once

#include <utils/panic.hpp>

#define PRECONDITION(COND)                                                                                             \
    if (!(COND)) {                                                                                                     \
        PANIC("Precondition ({}) is false", #COND);                                                                    \
    }

#define POSTCONDITION(COND)                                                                                            \
    if (!(COND)) {                                                                                                     \
        PANIC("Postcondition ({}) is false", #COND);                                                                   \
    }
