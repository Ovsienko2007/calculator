    IN
    POP RAX

    PUSH 1

    CALL :1

    OUT
    HALT

:1
    PUSH  1
    PUSH  RAX
    JA :3     ; [RBX] < RAX

    PUSH  RAX

    PUSH  RAX
    PUSH  1
    SUB
    POP   RAX

    CALL :1
    MUL

:3
    RET