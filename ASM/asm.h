#ifndef ASM_H_
#define ASM_H_

#include <stdio.h>
#include "file_read.h"

const int startCodeSize = 16;
const int maxRegLen     = 8;
const int regsNum       = 8;
const int leabelNum     = 10;
const int maxCommandLen = 20;
const int maxLabelLen   = 8;
const int minLabelsLen  = 8;

struct labels_arr{
    int capacity;
    int size;
    int *data;
};

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

enum get_reg_mod{
    push_reg,
    pop_reg,
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

struct bytecode{
    int size;
    int capacity;
    int *data;
};

struct labels{
    int labels_value[leabelNum];
    bool all_labels_added;
    labels_arr labels;
};

struct instruct{
    const char *command;
    instr_name instr;
};

struct registor{
    const char *str_reg;
    regs reg;
};

registor registor_arr[] = {
    {"RAX"    , rax},
    {"RBX"    , rbx},
    {"RCX"    , rcx},
    {"RDX"    , rdx},
    {"REX"    , rex},
    {"RFX"    , rfx},
    {"RGX"    , rgx},
    {"HUI"    , hui},
};

instruct instruct_arr[] = {
    {"ADD" , add_func },
    {"MUL" , mul_func },
    {"SUB" , sub_func },
    {"DIV" , div_func },
    {"SQRT", sqrt_func},
    {"DUMP", dump_func},
    {"OUT" , out_func },
    {"IN"  , in_func  },   
};

instruct jump_func[] = {
    {"JMP", jmp_func},
    {"JB" , jb_func },
    {"JBE", jbe_func},
    {"JA" , ja_func },
    {"JAE", jae_func},
    {"JE" , je_func },
    {"JNE", jne_func},
};

int find_int_len(int num);
bool run_first_pass(bytecode *buffer, data_text *program, const char *file_name, labels *labels_arr);

void print_error(const char *file, int line, assembler_error error);
void del_comment(char *line);

int get_reg_name(data_text *program, int line, assembler_error *error);

int init_code(bytecode *data);
int add_command(bytecode *data, int new_elem);

bool is_str_space_characters(char *str);

int add_simple_instructs(char *command, bytecode *buffer, assembler_error *error);
int add_push_instruct(bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line);

int add_pushr_instruct(bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line, instr_name instr);

void check_argument(int argc, char **argv, const char **file_name, const char **file_out_name);

void add_new_label(bytecode *buffer, labels *labels_arr, data_text *program, 
                    const char *command, int line, int command_len, assembler_error *error);

int add_jump_instruct(char *command, bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line, labels *labels_arr);

int  get_jump_line(char *label, labels *labels_arr, assembler_error *error);


bool run_second_pass(bytecode *buffer, labels labels_arr);
int  add_label(labels *labels_arr, int new_elem);

#endif