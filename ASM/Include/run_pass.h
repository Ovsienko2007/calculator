#ifndef RUN_PASS_H
#define RUN_PASS_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "add_instructs.h"
#include "file_read.h"

const int maxCommandLen = 20;

int run_commands(bytecode *buffer, data_text *program, const char *file_name, labels *labels_arr);
bool run_first_pass(bytecode *buffer, data_text *program, const char *file_name, labels *labels_arr);
bool run_second_pass(bytecode *buffer, labels *labels_arr);

void print_error(const char *file, int line, assembler_error error);
#endif