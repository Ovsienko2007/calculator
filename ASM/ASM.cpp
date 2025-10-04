#include <stdio.h>
#include "file_read.h"

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

struct code{
    int size;
    int capacity;
    int *data;
};

const int startCodeSize = 16;
const int maxRegLen     = 8;

int find_int_len(int num);
int get_reg_name(data_text *program, int line);
int add_funcs(code *buffer, data_text *program, const char *file_name);
void print_error(const char *file, int line);
void del_comment(char *line);

int init_code(code *data);
int add_command(code *data, int new_elem);

int main(){
    const char *file_name = "source.asm";
    data_text program = init_data_text();

    int check_open = open_file(&program, file_name);
    if (check_open) return 1;

    code buffer = {};
    init_code(&buffer);
    
    if (add_funcs(&buffer, &program, file_name)) return 1;

    FILE *stream = fopen("byte_code.txt", "w");

    fprintf(stream, "%d ", buffer.size);
    for (int cmd_pos = 0; cmd_pos < buffer.size; cmd_pos++){
        fprintf(stream, "%d ", buffer.data[cmd_pos]);
    }
    fclose(stream);

    printf("COMPLITED\n");
    return 0;
}


int add_funcs(code *buffer, data_text *program, const char *file_name){
    char command[20] = {};
    bool program_ended = false;
    

    int line = 0;
    for (line = 0; line < program->text.lines_count; line++){
        del_comment(program->text.lines[line]);
        if (strlen(program->text.lines[line]) == strspn(program->text.lines[line], " \t\n\r\f\v")){
            continue;
        }
        bool error = false;
        bool step_complited = false;
        bool is_command = true;

        sscanf(program->text.lines[line], "%s", command);

        if (strcmp(command, "ADD")  == 0){
            if (add_command(buffer, add_func)) error = true;
        }
        else if (strcmp(command, "MUL")  == 0){
            if (add_command(buffer, mul_func)) error = true;
        }
        else if (strcmp(command, "SUB")  == 0){
            if (add_command(buffer, sub_func)) error = true;
        }
        else if (strcmp(command, "DIV")  == 0){
            if (add_command(buffer, div_func)) error = true;
        }
        else if (strcmp(command, "SQRT") == 0){
            if (add_command(buffer, sqrt_func)) error = true;
        }
        else if (strcmp(command, "DUMP") == 0){
            if (add_command(buffer, dump_func)) error = true;
        }
        else if (strcmp(command, "OUT")  == 0){
            if (add_command(buffer, out_func)) error = true;
        }
        else if (strcmp(command, "IN") == 0){
            if (add_command(buffer, in_func)) error = true;
        }
        else if (strcmp(command, "HALT")  == 0){
            if (add_command(buffer, halt_func)) error = true;
            program_ended = true;
            break;
        }
        else if (strcmp(command, "PUSH") == 0){
            if (add_command(buffer, push_func)) error = true;
            int new_elem = 0;
            program->text.lines[line] += strlen(command) + 1;

            if (sscanf(program->text.lines[line], "%d", &new_elem) == 0){
                error = true;
            }

            if (add_command(buffer, new_elem)) error = true;

            program->text.lines[line] += find_int_len(new_elem);
            step_complited = true;
        }
        else if (strcmp(command, "PUSHR") == 0){
            int reg = 0;
            if (add_command(buffer, pushr_func)) error = true;
            
            program->text.lines[line] += strlen(command);

            reg = get_reg_name(program, line);

            if (reg < 0){
                error = true;
            }

            if (add_command(buffer, reg)) error = true;
            step_complited = true;
        }
        else if (strcmp(command, "POPR") == 0){
            int reg = 0;
            if (add_command(buffer, popr_func)) error = true;
            
            program->text.lines[line] += strlen(command) + 1;

            reg = get_reg_name(program, line);

            if (reg < 0){
                error = true;
            }

            if (add_command(buffer, reg)) error = true;
            step_complited = true;
        }
        else{
            is_command = false;
        }
        
        if (!step_complited && is_command){
            program->text.lines[line] += strlen(command);
        }

        if (strlen(program->text.lines[line]) != strspn(program->text.lines[line], " \t\n\r\f\v")){
            error = true;
        }

        if (error){
            break;
        }
    }
    

    if (!program_ended){
        printf("The assembly is not finished");
        print_error(file_name, line);
        return 1;
    }
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

int init_code(code *data){
    
    if (data == NULL) return 1;

    *data = {
        .size = 0,
        .capacity = startCodeSize,
        .data = (int *)calloc(startCodeSize, sizeof(int))
    };

    if (data->data == NULL) return 1;
    return 0;
}

int add_command(code *data, int new_elem){
    if (data == NULL || data->data==NULL) return 1;
    
    data->data[data->size] = new_elem;
    data->size++;
    

    if (data->size == data->capacity){
        data->capacity *= 2;
        int *new_data = (int *)realloc(data->data, data->capacity * sizeof(int));
        data->data = new_data;
    }
    
    if (data->data == NULL) return 1;

    return 0;
}

int get_reg_name(data_text *program, int line){
    char reg[maxRegLen] = {};
    if (sscanf(program->text.lines[line], "%7s", reg) == 0){
        return -1;
    }
    program->text.lines[line] += strlen(reg);

    if (strcmp(reg, "RAX") == 0){
        return 0;
    }
    if (strcmp(reg, "RBX") == 0){
        return 1;
    }
    if (strcmp(reg, "RCX") == 0){
        return 2;
    }
    if (strcmp(reg, "RDX") == 0){
        return 3;
    }
    if (strcmp(reg, "REX") == 0){
        return 4;
    }
    if (strcmp(reg, "HUI") == 0){
        return 5;
    }

    return -1;
}

int find_int_len(int num) {
    int ans = 0;

    if (num < 0){
        ans++;
    }
    
    for (;num != 0; num/=10, ans++);
    
    return ans;
}
