#include <stdio.h>
#include "file_read.h"

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

int start_prog(data_text *program, const char *file_name);
void print_error(const char *file, int line);
void del_comment(char *line);

int main(){
    const char *file_name = "source.asm";
    data_text program = init_data_text();

    int check_open = open_file(&program, file_name);
    if (check_open) return 0;
    start_prog(&program, file_name);
}

int start_prog(data_text *program, const char *file_name){
    FILE * byte_code = fopen("byte_code.txt", "w");
    char command[20] = {};

    int line = 0;
    for (line = 0; line < program->text.lines_count; line++){
        del_comment(program->text.lines[line]);

        sscanf(program->text.lines[line], "%s", command);
        if (strcmp(command, "OUT")  == 0){
            fprintf(byte_code, "%d", out_func);
            break;
        }

        
        else if (strcmp(command, "PUSH") == 0){
            int new_elem = 0;
            program->text.lines[line] += strlen(command) + 1;
            if (sscanf(program->text.lines[line], "%d", &new_elem) == 0){
                print_error(file_name, line);
                return 1;
            }
            fprintf(byte_code, "%d %d", push_func, new_elem);
        }
        else if (strcmp(command, "ADD")  == 0){
            fprintf(byte_code, "%d", add_func);
        }
        else if (strcmp(command, "MUL")  == 0){
            fprintf(byte_code, "%d", mul_func);
        }
        else if (strcmp(command, "SUB")  == 0){
            fprintf(byte_code, "%d", sub_func);
        }
        else if (strcmp(command, "DIV")  == 0){
            fprintf(byte_code, "%d", div_func);
        }
        else if (strcmp(command, "SQRT") == 0){
            fprintf(byte_code, "%d", sqrt_func);
        }
        else if (strcmp(command, "DUMP") == 0){
            fprintf(byte_code, "%d", dump_func);
        }
        else{
            print_error(file_name, line);
            return 1;
        }
        fprintf(byte_code, "\n");
    }

    if (line >= program->text.lines_count){
        print_error(file_name, line);
        return 1;
    }

    fclose(byte_code);
    return 0;
}

void print_error(const char *file, int line){
    printf("error %s:%d\n", file, line);
}

void del_comment(char *line){
    char *comment_start = strchr(line, ';');
    if (comment_start != NULL){
        comment_start[0] = '\0';
    }
}