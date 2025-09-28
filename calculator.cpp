#include "calculator.h"

static int console_push(stack_t *stk, error_t *err);
static int console_add(stack_t *stk, error_t *err);
static int console_mull(stack_t *stk, error_t *err);
static int console_sub(stack_t *stk, error_t *err);
static int console_div(stack_t *stk, error_t *err);
static int console_sqrt(stack_t *stk, error_t *err);
static int console_out(stack_t *stk, error_t *err);

void console(){
    char command[max_command_len];
    char format[max_command_len];

    error_t err = no_error; 
    stack_t stk = {};
    init_stack(&stk, &err);
    if (err != no_error) return;

    while (true){
        sprintf(format, "%%%ds", max_command_len - 1);
        scanf(format, command);

        if (strcmp(command, "PUSH") == 0){
            if (console_push(&stk, &err)) printf("ERROR!\n");
        }
        if (strcmp(command, "ADD") == 0){
            if (console_add(&stk, &err)) printf("ERROR!\n");
        }
        if (strcmp(command, "MULL") == 0){
            if (console_mull(&stk, &err)) printf("ERROR!\n");
        }
        if (strcmp(command, "SUB") == 0){
            if (console_sub(&stk, &err)) printf("ERROR!\n");
        }
        if (strcmp(command, "DIV") == 0){
            if (console_div(&stk, &err)) printf("ERROR!\n");
        }
        if (strcmp(command, "SQRT") == 0){
            if (console_sqrt(&stk, &err)) printf("ERROR!\n");
        }

        if (strcmp(command, "DUMP") == 0){
            USER_DUMP(&stk, no_error);
        }
        if (strcmp(command, "OUT") == 0){
            if (console_out(&stk, &err)) printf("ERROR!\n");
            break;
        }
    }

    destroy_stack(&stk, &err);
    if (err){
        printf("ERROR!");
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

static int console_mull(stack_t *stk, error_t *err){
    stackElemType num_1 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }
    printf("%d\n", num_1);

    stackElemType num_2 = pop_stack(stk, err);
    if (*err){
        push_stack(stk, num_1);
        return 1;
    }
    printf("%d %d\n", num_1, num_2);
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
    printf("%d\n", stk->size);
    while (stk->size != 0){
        stackElemType num = pop_stack(stk, err);
        if (*err){
            printf("sdsaxcdASdSD");
            return 1;
        }
        printf("%d ", num);
    }
    printf("\n");
    return 0;
}