#include "calculator.h"

// TODO WHY CONSOLE

static int console_push(stack_t *stk, error_t *err);
static int console_add(stack_t *stk, error_t *err);
static int console_mul(stack_t *stk, error_t *err);
static int console_sub(stack_t *stk, error_t *err);
static int console_div(stack_t *stk, error_t *err);
static int console_sqrt(stack_t *stk, error_t *err);
static int console_out(stack_t *stk, error_t *err);

void console(){
    int command = 0;

    error_t command_err = no_error; 
    
    stack_t stk = {};
    init_stack(&stk, &command_err);
    if (command_err != no_error) return;
    
    bool work_status = true;
    while (work_status){
        command_err = no_error;
        scanf("%d", &command);

        switch (command){
        case push_func:
            console_push(&stk, &command_err);
            break;
        case add_func:
            console_add(&stk, &command_err);
            break;
        case mul_func:
            console_mul(&stk, &command_err);
            break;
        case sub_func:
            console_sub(&stk, &command_err);
            break;
        case div_func:
            console_div(&stk, &command_err);
            break;
        case sqrt_func:
            console_sqrt(&stk, &command_err);
            break;
        case dump_func:
            USER_DUMP(&stk);
            break;
        case out_func:
            console_out(&stk, &command_err);
            destroy_stack(&stk, &command_err);
            work_status = false;
            break;
        default:
            break;
        }
        if (command_err){
            printf("ERROR!\n");
        }
    }
}

static int console_push(stack_t *stk, error_t *err){
    stackElemType new_elem = 0;
    scanf("%d", &new_elem); 
    push_stack(stk, new_elem, err);
    if (*err) return 1;
    return 0;
}

static int console_add(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    stackElemType num_2 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    push_stack(stk, num_1 + num_2);
    return 0;
}

static int console_sub(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    stackElemType num_2 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    push_stack(stk, num_2 - num_1);
    return 0;
}

static int console_mul(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    stackElemType num_2 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }
    push_stack(stk, num_1 * num_2);
    return 0;
}

static int console_div(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }

    stackElemType num_2 = pop_stack(stk, err);
    if (*err || num_1 == 0){
        push_stack(stk, num_1);
        return 1;
    }

    push_stack(stk, num_2 / num_1);
    return 0;
}

static int console_sqrt(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err || num_1 < 0){
        push_stack(stk, num_1);
        return 1;
    }

    push_stack(stk, (stackElemType)(sqrt(num_1)));
    return 0;
}

static int console_out(stack_t *stk, error_t *err){
    while (stk->size != 0){
        stackElemType num = pop_stack(stk, err);
        if (*err){
            printf("asdfd");
            return 1;
        }
        printf("%d ", num);
    }
    destroy_stack(stk);
    return 0;
}