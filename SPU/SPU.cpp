#include "SPU.h"

static int run_pushr(processor *proc, error_t *err);
static int run_popr (processor *proc, error_t *err);
static int run_in   (processor *proc, error_t *err);
static int run_push (processor *proc, error_t *err);
static int run_add  (processor *proc, error_t *err);
static int run_mul  (processor *proc, error_t *err);
static int run_sub  (processor *proc, error_t *err);
static int run_div  (processor *proc, error_t *err);
static int run_sqrt (processor *proc, error_t *err);
static int run_out  (processor *proc, error_t *err);
static int run_jump (processor *proc, error_t *err, int func);
static int run_call (processor *proc, error_t *err);
static int run_ret  (processor *proc, error_t *err);
static int run_popm (processor *proc, error_t *err);
static int run_pushm(processor *proc, error_t *err);
static int run_show (processor *proc, error_t *err);

static int run_pow_func   (processor *proc, error_t *err);
static int run_is_e_func  (processor *proc, error_t *err);
static int run_is_ne_func (processor *proc, error_t *err);
static int run_is_be_func (processor *proc, error_t *err);
static int run_is_b_func  (processor *proc, error_t *err);
static int run_is_ae_func (processor *proc, error_t *err);
static int run_is_a_func  (processor *proc, error_t *err);
static int run_and_func   (processor *proc, error_t *err);
static int run_or_func    (processor *proc, error_t *err);

static void run_clear();

int run_code(processor *proc){
    error_t command_err = no_error;
    int command = 0;
    
    bool work_status = true;
    while (proc->extantion_point < proc->code.size && work_status){
        command_err = no_error;
        bool need_step = true;
        command = proc->code.data[proc->extantion_point];

        switch (command){ //TODO rebuild
        case push_func:
            run_push(proc, &command_err);
            break;
        case add_func:
            run_add(proc, &command_err);
            break;
        case mul_func:
            run_mul(proc, &command_err);
            break;
        case sub_func:
            run_sub(proc, &command_err);
            break;
        case div_func:
            run_div(proc, &command_err);
            break;
        case sqrt_func:
            run_sqrt(proc, &command_err);
            break;
        case dump_func:
            USER_DUMP(&proc->stack);
            break;
        case pow_func  : 
            run_pow_func   (proc, &command_err);
            break; 
        case is_e_func : 
            run_is_e_func  (proc, &command_err);
            break; 
        case is_ne_func: 
            run_is_ne_func (proc, &command_err);
            break; 
        case is_be_func: 
            run_is_be_func (proc, &command_err);
            break; 
        case is_b_func : 
            run_is_b_func  (proc, &command_err);
            break; 
        case is_ae_func: 
            run_is_ae_func (proc, &command_err);
            break; 
        case is_a_func : 
            run_is_a_func  (proc, &command_err);
            break; 
        case and_func  : 
            run_and_func   (proc, &command_err);
            break; 
        case or_func   : 
            run_or_func    (proc, &command_err);
            break; 
        case out_func:
            run_out(proc, &command_err);
            break;
        case halt_func:
            work_status = false;
            break;
        case in_func:
            run_in(proc, &command_err);
            break;
        case pushr_func:
            run_pushr(proc, &command_err);
            break;
        case popr_func:
            run_popr(proc, &command_err);
            break;
        case pushm_func:
            run_pushm(proc, &command_err);
            break;
        case popm_func:
            run_popm(proc, &command_err);
            break;
        case show_func:
            run_show(proc, &command_err);
            break;
        case clear_func:
            run_clear();
            break;
        case jmp_func: 
        case jb_func: case jbe_func:
        case ja_func: case jae_func:
        case je_func: case jne_func:
            run_jump(proc, &command_err, command);
            need_step = false;
            break;
        case call_func:
            run_call(proc, &command_err);
            need_step = false;
            break;
        case ret_func:
            run_ret(proc, &command_err);
            need_step = false;
            break;

        default:
            break;
        }
        
        if (command_err){
            printf("ERROR!\n");
            return 1;
        }

        if (need_step){
            proc->extantion_point++;
        }
    }
    return work_status;
}


static int run_push(processor *proc, error_t *err){
    proc->extantion_point++;
    stackElemType new_elem = proc->code.data[proc->extantion_point];

    push_stack(&proc->stack, new_elem, err);
    if (*err) return 1;
    return 0;
}

static int run_pushr(processor *proc, error_t *err){
    proc->extantion_point++;
    if (proc->code.data[proc->extantion_point] > proc->regs.regs_num  ||
        proc->code.data[proc->extantion_point] < 0) return 1;

    stackElemType new_elem = proc->regs.regs[proc->code.data[proc->extantion_point]];

    push_stack(&proc->stack, new_elem, err);
    if (*err) return 1;
    return 0;
}
static int run_popr(processor *proc, error_t *err){
    proc->extantion_point++;
    int new_reg_value = pop_stack(&proc->stack, err);

    if (*err) return 1;
    if (proc->code.data[proc->extantion_point] > proc->regs.regs_num  ||
        proc->code.data[proc->extantion_point] < 0) return 1;

    proc->regs.regs[proc->code.data[proc->extantion_point]] = new_reg_value;

    return 0;
}

static int run_pushm(processor *proc, error_t *err){
    proc->extantion_point++;
    if (proc->code.data[proc->extantion_point] > proc->ram->size ||
        proc->code.data[proc->extantion_point] < 0) return 1;

    stackElemType new_elem = proc->ram->data[proc->regs.regs[proc->code.data[proc->extantion_point]]];

    push_stack(&proc->stack, new_elem, err);
    if (*err) return 1;
    return 0;
}

static int run_popm(processor *proc, error_t *err){
    proc->extantion_point++;
    int new_reg_value = pop_stack(&proc->stack, err);

    if (*err) return 1;
    if (proc->code.data[proc->extantion_point] > proc->ram->size  ||
        proc->code.data[proc->extantion_point] < 0) return 1;

    proc->ram->data[proc->regs.regs[proc->code.data[proc->extantion_point]]] = new_reg_value;

    return 0;
}

static int run_in(processor *proc, error_t *err){
    stackElemType new_elem = 0;
    scanf("%d", &new_elem);
    push_stack(&proc->stack, new_elem, err);

    if (*err) return 1;
    return 0;
}

static int run_add(processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 + num_2);
    return 0;
}

static int run_pow_func   (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, (int)pow(num_1, num_2));
    return 0;
}

static int run_is_e_func  (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 == num_2);
    return 0;
}

static int run_is_ne_func (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 != num_2);
    return 0;
}

static int run_is_be_func (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 <= num_2);
    return 0;
}

static int run_is_b_func  (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 < num_2);
    return 0;
}

static int run_is_ae_func (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 >= num_2);
    return 0;
}

static int run_is_a_func  (processor *proc, error_t *err){
    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 > num_2);
    return 0;
}

static int run_and_func   (processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 && num_2);
    return 0;
}

static int run_or_func    (processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 || num_2);
    return 0;
}


static int run_sub(processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_2 - num_1);
    return 0;
}

static int run_mul(processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    push_stack(&proc->stack, num_1 * num_2);
    return 0;
}

static int run_div(processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType num_2 = pop_stack(&proc->stack, err);
    if (*err || num_1 == 0) return 1;

    push_stack(&proc->stack, num_2 / num_1);
    return 0;
}

static int run_sqrt(processor *proc, error_t *err){
    stackElemType num_1 = pop_stack(&proc->stack, err);
    if (*err || num_1 < 0) return 1;

    push_stack(&proc->stack, (stackElemType)(sqrt(num_1)));
    return 0;
}

static int run_out(processor *proc, error_t *err){
    while (proc->stack.size != 0){
        stackElemType num = pop_stack(&proc->stack, err);
        if (*err) return 1;
        printf("%d ", num);
    }
    printf("\n");
    return 0;
}

int read_file(processor *proc){
    int code_size = 0;

    FILE *stream = fopen("byte_code", "r");
    if (stream == NULL) return -1;
    fscanf(stream, "%d", &code_size);
    int new_command = 0;
    for (int command_pos = 0; command_pos < code_size; command_pos++){
        fscanf(stream, "%d", &new_command);
        add_command(&proc->code, new_command, command_pos);
    }
    fclose(stream);
    return 0;
}

int add_command(code_t *data, int new_elem, int command_pos){
    if (data == NULL || data->data==NULL) return 1;
    data->data[command_pos] = new_elem;
    data->size++;

    if (data->size == data->capacity){
        data->capacity *= 2;
        int *new_data = (int *)realloc(data->data, data->capacity * sizeof(int));
        if (!new_data) return 1;
        data->data = new_data;
    }

    return 0;
}

int init_code(code_t *data){
    if (data == NULL) return 1;

    *data = {
        .size = 0,
        .capacity = kStartCodeSize,
        .data = (int *)calloc(kStartCodeSize, sizeof(int))
    };

    if (data->data == NULL) return 1;
    return 0;
}

int destroy_code(code_t *data){
    if (data == NULL) return -1;

    free(data->data);
    data->capacity = 0;
    data->size = 0;

    return 0;
}

int init_regs(registers *reg){
    if (reg == NULL) return 1;

    *reg = {
        .regs_num = 8,
        .regs = {},
    };

    return 0;
}

int destroy_regs(registers *reg){
    if (reg == NULL) return -1;
    reg->regs_num = 0;

    return 0;
}

static int run_jump(processor *proc, error_t *err, int func){
    if (proc == NULL){
        *err = null_data_ptr;
        return 0;
    }
    
    if (func == jmp_func){
        proc->extantion_point = proc->code.data[proc->extantion_point + 1];
        return 0;
    }
    
    stackElemType par2 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    stackElemType par1 = pop_stack(&proc->stack, err);
    if (*err) return 1;

    for (size_t check_num = 0; check_num < sizeof(jump_func) / sizeof(jump_func[0]); check_num++){
        if (func == jump_func[check_num].instr){
            if (jump_func[check_num].func(par1, par2)){
                proc->extantion_point = proc->code.data[proc->extantion_point + 1];
            }
            else proc->extantion_point += 2;
            return 0;
        }
    }
    return 0;
}

static int run_ret(processor *proc, error_t *err){
    if (proc == NULL){
        *err = null_data_ptr;
        return 0;
    }
    
    int new_extantial_point = pop_stack(&proc->ret_arr, err);
    if (*err != no_error) return -1;

    proc->extantion_point = new_extantial_point;
    return 0;
}

static int run_call(processor *proc, error_t *err){
    if (proc == NULL){
        *err = null_data_ptr;
        return 0;
    }
    
    int new_extantial_point = proc->code.data[proc->extantion_point + 1];
    push_stack(&proc->ret_arr, proc->extantion_point + 2, err);
    if (*err != no_error) return -1;

    proc->extantion_point = new_extantial_point;
    return 0;
}
static int run_show(processor *proc, error_t *err){
    if (proc == NULL){
        *err = null_data_ptr;
        return 0;
    }
    
    for (int y = 0; y < yImgSize; y++){
        for (int x = 0; x < xImgSize; x++){
            unsigned int out_elem = proc->ram->data[y * xImgSize + x];
            int r = out_elem / 256 / 256 / 256; //TODO remake (I am stupid)
            int g = out_elem / 256 / 256 % 256;
            int b = out_elem / 256 % 256;
            int value = out_elem % 256;

            printf("\033[48;2;%d;%d;%dm", r, g, b);
            printf("%c", value);
            printf("\033[0m");
        }
        printf("\n");
    }

    return 0;
}
static void run_clear(){
    fflush(stdout);
    usleep(frameUpdateTime);

    printf("\033[H");
    return;
}

bool check_jb (int a, int b) { return a <  b; }
bool check_jbe(int a, int b) { return a <= b; }
bool check_ja (int a, int b) { return a >  b; }
bool check_jae(int a, int b) { return a >= b; }
bool check_je (int a, int b) { return a == b; }
bool check_jne(int a, int b) { return a != b; }