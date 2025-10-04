#include "calculator.h"

processor init_processor(bool *error);

int main(){
    bool init_error = false;

    processor proc = init_processor(&init_error);
    if (init_error) return 1;

    read_file(&proc);
    
    
    return run_code(&proc);
}

processor init_processor(bool *error){
    processor proc = {
        .stack           = {},
        .code            = {},
        .extantion_point = 0,
        .regs            = {}
    };
    if (init_code(&proc.code)){
        if (error) *error = true;
    }
    if (init_stack(&proc.stack)){
        if (error) *error = true;
    }
    if (init_regs(&proc.regs)){
        if (error) *error = true;
    }
    return proc;
}