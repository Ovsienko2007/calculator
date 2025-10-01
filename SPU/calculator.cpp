#include "calculator.h"

static int console_push(stack_t *stk, error_t *err);
static int console_add(stack_t *stk, error_t *err);
static int console_mull(stack_t *stk, error_t *err);
static int console_sub(stack_t *stk, error_t *err);
static int console_div(stack_t *stk, error_t *err);
static int console_sqrt(stack_t *stk, error_t *err);
static int console_out(stack_t *stk, error_t *err);

void console(){
    int command = 0;

    error_t err = no_error; 
    
    stack_t stk = {};
    init_stack(&stk, &err);
    if (err != no_error) return;
    
    bool work_status = true;
    while (work_status){
        err = no_error;
        scanf("%d", &command);

        switch (command){ // TODO make enum commands
       // ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠛⢉⢉⠉⠉⠻⣿⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⣿⣿⣿⠟⠠⡰⣕⣗⣷⣧⣀⣅⠘⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⣿⣿⠃⣠⣳⣟⣿⣿⣷⣿⡿⣜⠄⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⡿⠁⠄⣳⢷⣿⣿⣿⣿⡿⣝⠖⠄⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⠃⠄⢢⡹⣿⢷⣯⢿⢷⡫⣗⠍⢰⣿⣿⣿⣿⣿
       // ⣿⣿⣿⡏⢀⢄⠤⣁⠋⠿⣗⣟⡯⡏⢎⠁⢸⣿⣿⣿⣿⣿
       // ⣿⣿⣿⠄⢔⢕⣯⣿⣿⡲⡤⡄⡤⠄⡀⢠⣿⣿⣿⣿⣿⣿
       // ⣿⣿⠇⠠⡳⣯⣿⣿⣾⢵⣫⢎⢎⠆⢀⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⠄⢨⣫⣿⣿⡿⣿⣻⢎⡗⡕⡅⢸⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⠄⢜⢾⣾⣿⣿⣟⣗⢯⡪⡳⡀⢸⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⠄⢸⢽⣿⣷⣿⣻⡮⡧⡳⡱⡁⢸⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⡄⢨⣻⣽⣿⣟⣿⣞⣗⡽⡸⡐⢸⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⡇⢀⢗⣿⣿⣿⣿⡿⣞⡵⡣⣊⢸⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⣿⡀⡣⣗⣿⣿⣿⣿⣯⡯⡺⣼⠎⣿⣿⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣧⠐⡵⣻⣟⣯⣿⣷⣟⣝⢞⡿⢹⣿⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⡆⢘⡺⣽⢿⣻⣿⣗⡷⣹⢩⢃⢿⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⣷⠄⠪⣯⣟⣿⢯⣿⣻⣜⢎⢆⠜⣿⣿⣿⣿⣿
       // ⣿⣿⣿⣿⣿⡆⠄⢣⣻⣽⣿⣿⣟⣾⡮⡺⡸⠸⣿⣿⣿⣿
       // ⣿⣿⡿⠛⠉⠁⠄⢕⡳⣽⡾⣿⢽⣯⡿⣮⢚⣅⠹⣿⣿⣿
       // ⡿⠋⠄⠄⠄⠄⢀⠒⠝⣞⢿⡿⣿⣽⢿⡽⣧⣳⡅⠌⠻⣿
       // ⠁⠄⠄⠄⠄⠄⠐⡐⠱⡱⣻⡻⣝⣮⣟⣿⣻⣟⣻⡺⣊
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