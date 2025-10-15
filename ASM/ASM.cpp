#include "asm.h"

static int is_number(const char *str);
static void init_labels_value(int *arr, int size);

int main(int argc, char *argv[]){
    const char *file_name     = "";
    const char *file_out_name = "";
    check_argument(argc, argv, &file_name, &file_out_name);

    data_text program = init_data_text();

    if (!open_file(&program, file_name)){
        printf("File was not opened!\n");
        return 1;
    }

    bytecode buffer = {};
    init_code(&buffer);

    labels labels_arr = { // TODO remake
        .labels_value = {},
        .all_labels_added = true,
        .labels = {
            .capacity   = minLabelsLen,
            .size       = 0,
            .data = (int *)calloc(minLabelsLen, sizeof(int)),
        },
    };

    init_labels_value(labels_arr.labels_value, leabelNum);
    
    if (!run_first_pass(&buffer, &program, file_name, &labels_arr)) return 1;
    if (!labels_arr.all_labels_added){
        labels_arr.all_labels_added = true;
        if (run_second_pass(&buffer, labels_arr)) return 1;
    }
    
    if (!labels_arr.all_labels_added){
        printf("UNDIFIENT LABEL! FUCK YOU!!!!!!!!\n");
        return 0;
    }

    FILE *stream = fopen(file_out_name, "w");

    if (stream == NULL){
        printf("Output file was not opened!\n");
        return 1;
    }

    fprintf(stream, "%d ", buffer.size);
    for (int cmd_pos = 0; cmd_pos < buffer.size; cmd_pos++){
        fprintf(stream, "%d ", buffer.data[cmd_pos]);
    }
    fclose(stream);

    printf("COMPLETED\n");
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

void del_comment(char *line){
    char *comment_start = strchr(line, ';');
    if (comment_start != NULL){
        comment_start[0] = '\0';
    }
}

int init_code(bytecode *data){
    if (data == NULL) return 1;

    *data = {
        .size = 0,
        .capacity = startCodeSize,
        .data = (int *)calloc(startCodeSize, sizeof(int))
    };

    if (data->data == NULL) return 1;
    return 0;
}

int add_command(bytecode *data, int new_elem){
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

int get_reg_name(data_text *program, int line, assembler_error *error){
    regs new_reg = no_reg;
    int reg_len = 0;
    char reg[maxRegLen] = {};

    if (sscanf(program->text.lines[line], "%7s%n", reg, &reg_len) == 0){
        *error = incorrect_par;
        return -1;
    }
    program->text.lines[line] += reg_len;

    for (size_t num_reg = 0; num_reg < sizeof(registor_arr) / sizeof(registor_arr[0]);
         num_reg++){
        if (strcmp(reg, registor_arr[num_reg].str_reg) == 0){
            new_reg = registor_arr[num_reg].reg;
            break;
        }
    }

    if (0 > new_reg || new_reg > regsNum){
        *error = no_such_reg;
    }
    return new_reg;
}

int find_int_len(int num) {
    int ans = 0;

    if (num <= 0){
        ans++;
    }
    
    for (;num != 0; num/=10, ans++);
    
    return ans;
}

bool is_str_space_characters(char *str){
    return strlen(str) == strspn(str, " \t\n\r\f\v");
}

int add_simple_instructs(char *command, bytecode *buffer, assembler_error *error){
    for (size_t num_check = 0; num_check < sizeof(instruct_arr) / sizeof(instruct_arr[0]); num_check++){
        if (strcmp(command, instruct_arr[num_check].command) == 0){
            if (add_command(buffer, instruct_arr[num_check].instr)) *error = inside_error;
            return 1;
        }
    }
    return 0;
}

int add_push_instruct(bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line){
    if (add_command(buffer, push_func)) *error = inside_error;

    int new_elem      = 0;
    int new_elem_size = 0;

    program->text.lines[line] += command_len + 1;

    if (sscanf(program->text.lines[line], "%d%n", &new_elem, &new_elem_size) == 0){
        *error = incorrect_par;
    }
    else{
        if (add_command(buffer, new_elem)) *error = inside_error;
        program->text.lines[line] += new_elem_size;
    }

    return 1;
}

int add_pushr_instruct(bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line, instr_name instr){
    if (add_command(buffer, instr)) *error = inside_error;   
    program->text.lines[line] += command_len;

    int reg = get_reg_name(program, line, error);

    if (!*error && add_command(buffer, reg)) *error = inside_error;

    return 1;
}

int is_number(const char *str) {
    int num;
    return sscanf(str, "%d", &num) == 1;
}

void check_argument(int argc, char **argv, const char **file_name, const char **file_out_name){  
    if (argc > 1){
        *file_name = argv[1];
    }
    else{
        *file_name = "source.asm";
    }
    
    if (argc > 2){
        *file_out_name = argv[2];
    } 
    else{
        *file_out_name = "byte_code";
    }
}



int add_jump_instruct(char *command, bytecode *buffer, int command_len, assembler_error *error,
                      data_text *program, int line, labels *labels_arr){
    for (size_t check_func_num = 0; check_func_num < sizeof(jump_func) / sizeof(jump_func[0]);
         check_func_num++){
        if (strcmp(command, jump_func[check_func_num].command) == 0){
            if (add_command(buffer, jump_func[check_func_num].instr)) *error = inside_error;   
            program->text.lines[line] += command_len;

            char label_str[maxLabelLen] = "";
            int label_len = 0;
            sscanf(program->text.lines[line], "%7s%n", label_str, &label_len);
            program->text.lines[line] += label_len;

            int label = get_jump_line(label_str, labels_arr, error);
            if (add_command(buffer, label)) *error = inside_error;
            return 1;
        }
    }
    return 0;
}

int get_jump_line(char *label, labels *labels_arr, assembler_error *error){
    if (label[0] == ':'){
        if (is_number(label + 1) && 0 <= atoi(label + 1) && atoi(label + 1) < leabelNum){
            if (labels_arr->labels_value[atoi(label + 1)] == -1){
                labels_arr->all_labels_added = false;
            }
            add_label(labels_arr, atoi(label + 1));
            return labels_arr->labels_value[atoi(label + 1)];
        }
        else {
            *error = incorrect_par;
            return -1;        
        }
    }
    *error = incorrect_par;
    return -1;
}



static void init_labels_value(int *arr, int size){
    for(int i = 0; i < size; i++) {
        arr[i] = -1;
    }
}

void add_new_label(bytecode *buffer, labels *labels_arr, data_text *program, 
                    const char *command, int line, int command_len, assembler_error *error){
    if (is_number(command + 1) && 0 <= atoi(command + 1) && atoi(command + 1) <= 9){
        if (labels_arr->labels_value[atoi(command + 1)] == -1){
            labels_arr->labels_value[atoi(command + 1)] = buffer->size;
        }
        else{
            *error = incorrect_par;
        }
    } else{
        *error = incorrect_par;
    }
    program->text.lines[line] += command_len;
}

int add_label(labels *labels_arr, int new_elem){
    assert(labels_arr != NULL);

    labels_arr->labels.data[labels_arr->labels.size] = new_elem;
    labels_arr->labels.size++;

    if (labels_arr->labels.size == labels_arr->labels.capacity){
        labels_arr->labels.size *= 2;
        int *new_data = (int *)realloc(labels_arr->labels.data, (labels_arr->labels.size) * sizeof(int));
        if (new_data == NULL) return 1;
        labels_arr->labels.data = new_data;
    }
    return 0;
}
