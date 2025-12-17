#ifndef ADD_INSTRUCTS_H_
#define ADD_INSTRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_read.h"

const int leabelNum     = 10;
const int minLabelsLen  = 8;

const int maxRegLen     = 8;
const int regsNum       = 8;
const int maxLabelLen   = 8;

enum instr_name{
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
    pushr_func = 31,
    popr_func  = 32,
    pushm_func = 41,
    popm_func  = 42,
    jmp_func   = 50,
    jb_func    = 51,
    jbe_func   = 52,
    ja_func    = 53,
    jae_func   = 54,
    je_func    = 55,
    jne_func   = 56,
    call_func  = 60,
    ret_func   = 61,
    show_func  = 100,
    clear_func = 101,
};

enum assembler_error{
    no_error             = 0,
    no_command           = 1,
    incorrect_par        = 2,
    no_such_reg          = 3,
    uninitialized_reg    = 4,
    program_not_finished = 5,
    extra_parameter      = 6,
    inside_error         = 7,
};

enum regs{
    no_reg = -1,
    rax    = 0,
    rbx    = 1,
    rcx    = 2,
    rdx    = 3,
    rex    = 4,
    rfx    = 5,
    rgx    = 6,
    hui    = 7,
};

enum get_reg_mod{
    standard_mod,
    in_brackets_mod,
};

struct instruct{
    const char *command;
    instr_name  instr;
};

struct registor{
    const char *str_reg;
    regs        reg;
};

struct labels_pos_arr_t{
    size_t  size;
    int *data;
};

struct labels_arr{
    size_t  size;
    size_t  capacity;
    int *data;
};

struct labels{
    bool            all_labels_added;
    labels_arr      all_labels;
    labels_pos_arr_t  labels_pos_arr;
};

struct bytecode{
    size_t size;
    size_t capacity;
    int *data;
};

static const registor registor_arr[] = {
    {"RAX"    , rax},
    {"RBX"    , rbx},
    {"RCX"    , rcx},
    {"RDX"    , rdx},
    {"REX"    , rex},
    {"RFX"    , rfx},
    {"RGX"    , rgx},
    {"HUI"    , hui},
};

static const instruct instruct_arr[] = {
    {"ADD" ,  add_func },
    {"MUL" ,  mul_func },
    {"SUB" ,  sub_func },
    {"DIV" ,  div_func },
    {"SQRT",  sqrt_func},
    {"DUMP",  dump_func},
    {"OUT" ,  out_func },
    {"IN"  ,  in_func  },
    {"RET" ,  ret_func },
    {"SHOW",  show_func},
    {"CLEAR", clear_func}
};

static const instruct jump_func[] = {
    {"JMP",  jmp_func },
    {"JB" ,  jb_func  },
    {"JBE",  jbe_func },
    {"JA" ,  ja_func  },
    {"JAE",  jae_func },
    {"JE" ,  je_func  },
    {"JNE",  jne_func },
    {"CALL", call_func},
};

int add_command(bytecode *data, int new_elem);
int add_simple_instructs(char *command, bytecode *buffer, assembler_error *error);
int add_push_instruct(bytecode *buffer, assembler_error *error,
                      data_text *program, int line);
int add_pushr_popr_instruct(bytecode *buffer, assembler_error *error, 
                      data_text *program, int line, instr_name instr);
int add_pushm_popm_instruct(bytecode *buffer, assembler_error *error,
                      data_text *program, int line, instr_name instr);
int add_jump_instruct(char *command, bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line, labels *labels_arr);


int get_jump_line(char *label, labels *labels_arr, assembler_error *error);
void add_new_label(bytecode *buffer, labels *labels_arr, data_text *program, 
                    const char *command, int line, int command_len, assembler_error *error);
int get_reg_name(data_text *program, int line, assembler_error *error, get_reg_mod mod);
int add_label_to_arr(labels *labels_arr, int new_elem, size_t pos, assembler_error *error);

#endif