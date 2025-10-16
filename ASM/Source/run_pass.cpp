#include "run_pass.h"

static void del_comment(char *line);
static bool is_str_space_characters(char *str);

int run_commands(bytecode *buffer, data_text *program, const char *file_name, labels *labels_arr){

    
    if (!run_first_pass(buffer, program, file_name, labels_arr)) return 1;
    if (labels_arr->all_labels_added){
        labels_arr->all_labels_added = true;
        if (run_second_pass(buffer, *labels_arr)) return 1;
    }
    if (!labels_arr->all_labels_added){
        printf("UNDIFIENT LABEL! FUCK YOU!!!!!!!!\n");
        return 0;
    }

    return 0;
}

bool run_first_pass(bytecode *buffer, data_text *program, const char *file_name, labels *labels_arr){
    char command[maxCommandLen] = {};
    bool program_ended = false;
    bool no_errors = true;

    int line = 0;
    for (line = 0; line < program->text.lines_count; line++){
        del_comment(program->text.lines[line]);
        if (is_str_space_characters(program->text.lines[line])){
            continue;
        }
        char *start = program->text.lines[line];

        assembler_error error = no_error;
        int command_len = 0;
        sscanf(program->text.lines[line], "%s%n", command, &command_len);

        if (add_simple_instructs(command, buffer, &error)){
            program->text.lines[line] += command_len;
        }
        else if (add_jump_instruct(command, buffer, command_len, &error, program, line, labels_arr));
        else if (strcmp(command, "HALT")  == 0){
            if (add_command(buffer, halt_func)) error = inside_error;
            else program_ended = true;
            break;
        }
        else if (strcmp(command, "PUSH") == 0){
            add_push_instruct(buffer, command_len, &error, program, line);
        }
        else if (strcmp(command, "PUSHR") == 0){
            add_pushr_instruct(buffer, command_len, &error, program, line, pushr_func);
        }
        else if (strcmp(command, "POPR") == 0){
            add_pushr_instruct(buffer, command_len, &error, program, line, popr_func);
        }
        else if (command[0] == ':'){
            add_new_label(buffer, labels_arr, program, command, line, command_len, &error);
        }
        else{
            error = no_command;
        }

        if (!error && !is_str_space_characters(program->text.lines[line])){
            error = extra_parameter;
        }

        if (error){
            no_errors = false;
            print_error(file_name, line, error);
        }
        program->text.lines[line] = start;

    }

    if (!program_ended){
        no_errors = false;
        print_error(file_name, line, program_not_finished);
        printf("The assembly was not finished\n");
    }
    return no_errors;
}

bool run_second_pass(bytecode *buffer, labels labels_arr){
    int label_pos = 0;
    int line = 0;
    for (line = 0; line < buffer->size;){
        switch (buffer->data[line]){
            case halt_func:
                return 0;
            case push_func: case pushr_func: case popr_func:
                line += 2;
                break;
            case add_func: case mul_func:  case sub_func:
            case div_func: case sqrt_func: case dump_func:
            case out_func: case in_func:
                line += 1;
                break;
            case jmp_func: case jb_func: case jbe_func: case ja_func: 
            case jae_func: case je_func: case jne_func:
                if (0 > labels_arr.labels.data[label_pos] || labels_arr.labels.data[label_pos] > 9){
                    return 1;
                }
                buffer->data[line + 1] = labels_arr.labels_value[labels_arr.labels.data[label_pos]];
                label_pos++;
                line += 2;
                break;
            default:
                break;
        }
    }
    return 1;
}

static void del_comment(char *line){
    char *comment_start = strchr(line, ';');
    if (comment_start != NULL){
        comment_start[0] = '\0';
    }
}

static bool is_str_space_characters(char *str){
    return strlen(str) == strspn(str, " \t\n\r\f\v");
}

void print_error(const char *file, int line, assembler_error error){
    printf("Error %s:%d\n", file, line + 1);
    switch (error){
        case no_error: break;
        case no_command:
            printf("There is no such command!\n");
            break;
        case incorrect_par:
            printf("Incorrect parameter!\n");
            break;
        case no_such_reg:
            printf("There is not such registor!\n");
            break;
        case uninitialized_reg:
            printf("This register has not been initialized!\n");
            break;
        case program_not_finished:
            printf("Programm did not finished!\n");
            break;
        case extra_parameter:
            printf("Extra parametr!\n");
            break;
        case inside_error:
            printf("Some inside error!\n");
            break;
        default:
            break;
    }
}