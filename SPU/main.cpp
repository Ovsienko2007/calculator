#include "SPU.h"

processor init_processor(bool *error, RAM const *ram);
int destroy_processor(processor *proc);

RAM init_ram(bool *error);
int destroy_ram(RAM const *ram);

int main(){
    bool init_error = false;
    RAM ram = init_ram(&init_error);
    if (init_error){
        printf("RAM was not initialized!");
        return 1;
    }

    processor proc = init_processor(&init_error, &ram);
    if (init_error){
        printf("SPU was not initialized!");
        return 1;
    }

    if (init_error) return 1;

    read_file(&proc);

    run_code(&proc);

    destroy_processor(&proc);
}

processor init_processor(bool *error, RAM const *ram){
    processor proc = {
        .stack           = {},
        .code            = {},
        .extantion_point = 0,
        .regs            = {},
        .ret_arr         = {},
        .ram             = ram,
    };
    if (init_regs(&proc.regs)){
        if (error) *error = true;
    }
    if (init_code(&proc.code)){
        if (error) *error = true;
    }
    if (init_stack(&proc.stack)){
        if (error) *error = true;
    }
    if (init_stack(&proc.ret_arr)){
        if (error) *error = true;
    }
    return proc;
}

int destroy_processor(processor *proc){
    destroy_stack(&(proc->stack));
    destroy_regs (&(proc->regs));
    destroy_code (&(proc->code));
    destroy_ram    (proc->ram);

    proc->extantion_point = 0;
    return 0;
}

RAM init_ram(bool *error){
    RAM ram = {
        .size = 50,
        .data = (int *)calloc(50, sizeof(int)),
    };
    if (ram.data == NULL) *error = true;

    return ram;
}

int destroy_ram(RAM const *ram){
    if (ram == NULL) return -1;

    free(ram->data);

    return 0;
}