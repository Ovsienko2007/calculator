#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <math.h>
#include "string.h"
#include "stack.h"

enum func_name{
    out_func  = 0,
    push_func = 1,
    add_func  = 2,
    mul_func  = 3,
    sub_func  = 4,
    div_func  = 5,
    sqrt_func = 6,
    dump_func = 7
};

const int max_command_len = 16;

void console();

#endif