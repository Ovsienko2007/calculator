#include "add_instructs.h"

static int is_number(const char *str);

int add_command(bytecode *data, int new_elem){
    if (data == NULL || data->data == NULL) return 1;
    
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

int add_simple_instructs(char *command, bytecode *buffer, assembler_error *error){
    for (size_t num_check = 0; num_check < sizeof(instruct_arr) / sizeof(instruct_arr[0]); num_check++){
        if (strcmp(command, instruct_arr[num_check].command) == 0){
            if (add_command(buffer, instruct_arr[num_check].instr)) *error = inside_error;
            return 1;
        }
    }
    return 0;
}

int add_push_instruct(bytecode *buffer, assembler_error *error,
                      data_text *program, int line){
    if (add_command(buffer, push_func)) *error = inside_error;

    int new_elem      = 0;
    int new_elem_size = 0;

    if (sscanf(program->text.lines[line], "%d%n", &new_elem, &new_elem_size) == 0){
        *error = incorrect_par;
    }
    else{
        if (add_command(buffer, new_elem)) *error = inside_error;
        program->text.lines[line] += new_elem_size;
    }

    return 1;
}

int add_pushr_popr_instruct(bytecode *buffer, assembler_error *error,
                      data_text *program, int line, instr_name instr){
    if (add_command(buffer, instr)) *error = inside_error;   

    int reg = get_reg_name(program, line, error, standard_mod);

    if (!*error && add_command(buffer, reg)) *error = inside_error;

    return 1;
}

int add_pushm_popm_instruct(bytecode *buffer, assembler_error *error,
                      data_text *program, int line, instr_name instr){
    if (add_command(buffer, instr)) *error = inside_error;   

    int reg = get_reg_name(program, line, error, in_brackets_mod);

    if (!*error && add_command(buffer, reg)) *error = inside_error;

    return 1;
}

int get_reg_name(data_text *program, int line, assembler_error *error, get_reg_mod mod){
    regs new_reg = no_reg;
    int reg_len = 0;
    char reg_str[maxRegLen] = {};
    char *reg;

    if (sscanf(program->text.lines[line], "%7s%n", reg_str, &reg_len) == 0){
        *error = incorrect_par;
        return -1;
    }
    reg = reg_str;
    if (mod == in_brackets_mod){
        if (reg[0] == '['){
            reg = reg_str + 1;
        }
        else (*error = no_such_reg);
        
        if (strchr(reg, ']')){
            reg[strchr(reg, ']') - reg] = '\0';
        }
        else (*error = no_such_reg);
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
        if (is_number(label + 1) && 0 <= atoi(label + 1)){
            if ((size_t)atoi(label + 1) > labels_arr->labels_pos_arr.size || labels_arr->labels_pos_arr.data[atoi(label + 1)] == -1){
                labels_arr->all_labels_added = false;

                labels_arr->all_labels.data[labels_arr->all_labels.size] = atoi(label + 1);
                labels_arr->all_labels.size++;

                if (labels_arr->all_labels.size == labels_arr->all_labels.capacity){
                    labels_arr->all_labels.capacity *= 2;

                    labels_arr->all_labels.data = (int *)realloc(labels_arr->all_labels.data, labels_arr->all_labels.capacity * sizeof(int));
                }

                return -1;
            }
            
            return labels_arr->labels_pos_arr.data[atoi(label + 1)];
        }
        else {
            *error = incorrect_par;
            return -1;        
        }
    }

    *error = incorrect_par;
    return -1;
}



void add_new_label(bytecode *buffer, labels *labels_arr, data_text *program, 
                    const char *command, int line, int command_len, assembler_error *error){ //TODO
    if (is_number(command + 1) && 0 <= atoi(command + 1)){
        add_label_to_arr(labels_arr, (int)buffer->size, (size_t)atoi(command + 1), error);
    }
    else{
        *error = incorrect_par;
    }
    program->text.lines[line] += command_len;
}

int add_label_to_arr(labels *labels_arr, int new_elem, size_t pos, assembler_error *error){
    assert(labels_arr);

    if (labels_arr->labels_pos_arr.size < pos){
        labels_arr->labels_pos_arr.data = (int *)realloc(labels_arr->labels_pos_arr.data, (pos + 1) * sizeof(int));

        for (; labels_arr->labels_pos_arr.size < pos + 1; labels_arr->labels_pos_arr.size++){
            labels_arr->labels_pos_arr.data[labels_arr->labels_pos_arr.size] = -1;
        }
    }

    if (labels_arr->labels_pos_arr.data[pos] != -1) *error = incorrect_par;

    labels_arr->labels_pos_arr.data[pos] = new_elem;

    return 0;
}

static int is_number(const char *str){
    int num;
    return sscanf(str, "%d", &num) == 1;
}