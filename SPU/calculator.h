#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <math.h>
#include "string.h"
#include "stack.h"

const int startCodeSize = 16;
struct code_t{
    int size;
    int capacity;
    int *data;
};

struct registers{
    int regs_num;
    int regs[8];
};

struct processor{
    stack_t stack;
    code_t  code;
    int extantion_point;
    registers regs;
};

enum func_name{
    halt_func  = 0,
    push_func  = 1,
    add_func   = 2,
    mul_func   = 3,
    sub_func   = 4,
    div_func   = 5,
    sqrt_func  = 6,
    dump_func  = 7,
    out_func   = 8,
    in_func    = 9,
    pushr_func = 33,
    popr_func  = 42
};

int init_code(code_t *data);
int init_regs(registers *reg);
int run_code(processor *proc);
int read_file(processor *proc);
int add_command(code_t *data, int new_elem, int command_pos);

#endif