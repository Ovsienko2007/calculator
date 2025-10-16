#ifndef ASM_H_
#define ASM_H_

#include <stdio.h>
#include "file_read.h"
#include "run_pass.h"


const int startCodeSize = 16;

int init_code(bytecode *data);

void check_argument(int argc, char **argv, const char **file_name, const char **file_out_name);

#endif