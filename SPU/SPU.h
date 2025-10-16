#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <math.h>
#include "string.h"
#include "stack.h"

const int startCodeSize = 16;
const int maxRegLen     = 8;

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
    popr_func  = 42,
    jmp_func   = 50,
    jb_func    = 51,
    jbe_func   = 52,
    ja_func    = 53,
    jae_func   = 54,
    je_func    = 55,
    jne_func   = 56,
};

struct code_t{
    int size;
    int capacity;
    int *data;
};

struct registers{
    int regs_num;
    int regs[maxRegLen];
};

struct processor{
    code_t  code;
    int extantion_point;
    registers regs;
    stack_t stack;
};



int init_code(code_t *data);
int init_regs(registers *reg);

int destroy_code(code_t *data);
int destroy_regs(registers *reg);

int run_code(processor *proc);
int read_file(processor *proc);
int add_command(code_t *data, int new_elem, int command_pos);

typedef bool (*compare_func)(int, int);

struct jump_instruct {
    func_name instr;
    compare_func func;
};

bool check_jb(int a, int b);
bool check_jbe(int a, int b);
bool check_ja(int a, int b);
bool check_jae(int a, int b);
bool check_je(int a, int b);
bool check_jne(int a, int b);

static const jump_instruct jump_func[] = {    
    {jb_func, check_jb},
    {jbe_func, check_jbe},
    {ja_func, check_ja},
    {jae_func, check_jae},
    {je_func, check_je},
    {jne_func, check_jne},
};

#endif