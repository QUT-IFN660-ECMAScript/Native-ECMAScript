#pragma once
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
extern int global_var;

inline unsigned int getNewRegister() {
    return global_var++;
}

#include "node.hpp"
#include "script.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "../type/type.hpp"
#include "../runtime/core.hpp"
#include "../runtime/console.hpp"
