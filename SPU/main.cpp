#include "SPU.h"

processor init_processor(bool *error);
int destroy_processor(processor *proc);

int main(){
    bool init_error = false;

    processor proc = init_processor(&init_error);

    if (init_error) return 1;

    read_file(&proc);

    run_code(&proc);

    destroy_processor(&proc);
}

processor init_processor(bool *error){
    processor proc = {
        .code            = {},
        .extantion_point = 0,
        .regs            = {},
        .stack           = {},
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
    return proc;
}

int destroy_processor(processor *proc){
    destroy_stack(&(proc->stack));
    destroy_regs(&(proc->regs));
    destroy_code(&(proc->code));

    proc->extantion_point = 0;
    return 0;
}