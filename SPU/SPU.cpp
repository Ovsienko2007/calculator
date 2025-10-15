#include "SPU.h"

static int run_pushr(processor *proc, error_t *err);
static int run_popr(processor *proc, error_t *err);
static int run_in(processor *proc, error_t *err);
static int run_push(processor *proc, error_t *err);
static int run_add(processor *proc, error_t *err);
static int run_mul(processor *proc, error_t *err);
static int run_sub(processor *proc, error_t *err);
static int run_div(processor *proc, error_t *err);
static int run_sqrt(processor *proc, error_t *err);
static int run_out(processor *proc, error_t *err);
static int run_jump(processor *proc, error_t *err, int func);

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
        case jmp_func: 
        case jb_func: case jbe_func:
        case ja_func: case jae_func:
        case je_func: case jne_func:
            run_jump(proc, &command_err, command);
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
    FILE *stream = fopen("byte_code", "r");
    fscanf(stream, "%d", &proc->code.size);
    int new_command = 0;
    for (int command_pos = 0; command_pos < proc->code.size; command_pos++){
        fscanf(stream, "%d", &new_command);
        add_command(&proc->code, new_command, command_pos);
    }
    fclose(stream);
    return 0;
}

int add_command(code_t *data, int new_elem, int command_pos){
    if (data == NULL || data->data==NULL) return 1;
    data->data[command_pos] = new_elem;

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
        .capacity = startCodeSize,
        .data = (int *)calloc(startCodeSize, sizeof(int))
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