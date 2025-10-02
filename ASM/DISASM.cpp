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

int main(){
    const char *file_name = "byte_code.txt";
    data_text program = init_data_text();

    int check_open = open_file(&program, file_name);
    if (check_open) return 0;
    start_prog(&program, file_name);
}

int start_prog(data_text *program, const char *file_name){
    FILE * stream = fopen("new_source.asm", "w");
    int command = 0;

    int line = 0;
    bool run_status = true;
    for (line = 0; line < program->text.lines_count && run_status; line++){
        sscanf(program->text.lines[line], "%d", &command);
        int new_elem = 0;
        switch (command){
        case push_func:
            program->text.lines[line] += 1;
            if (sscanf(program->text.lines[line], "%d", &new_elem) != 1){
                print_error(file_name, line);
                return 1;
            }
            fprintf(stream, "PUSH %d", new_elem);
            break;
        case add_func:
            fprintf(stream, "ADD");
            break;
        case mul_func:
            fprintf(stream, "MUL");
            break;
        case sub_func:
            fprintf(stream, "SUB");
            break;
        case div_func:
            fprintf(stream, "DIV");
            break;
        case sqrt_func:
            fprintf(stream, "SQRT");
            break;
        case dump_func:
            fprintf(stream, "DUMP");
            break;
        case out_func:
            fprintf(stream, "OUT");
            run_status = false;
            break;
        default:
            printf("ERROR");
            return 1;
        }

        if (run_status){
            fprintf(stream, "\n");
        }
    }

    if (run_status){
        print_error(file_name, line);
        return 1;
    }
    fclose(stream);
    return 0;
}

void print_error(const char *file, int line){
    printf("error %s:%d\n", file, line);
}