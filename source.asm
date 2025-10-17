CALL :0

IN
POP   RAX

IN
POP  [RAX]

CALL :1
HALT

:0  
    PUSH  50
    POP   RBX ;размер памяти

    PUSH  10
    POP   RDX ;размер строки

    PUSH  1
    POP   RCX ;позиция в строке
    RET

:1
    PUSH 0
    POP  RAX ;RAX = 0

    :2
        PUSH [RAX]

        PUSH  1    ;RAX++
        PUSH  RAX
        ADD
        POP   RAX

        CALL :3    ;проверка и вывод

        PUSH  RAX  ;RAX < RBX
        PUSH  RBX
        JB    :2
    RET

:3
    PUSH RCX
    PUSH RDX
    JNE  :4  ;RCX < RDX

        OUT

        PUSH  1    ;RCX = 1
        POP   RCX
        RET
    :4

    PUSH  1    ;RCX++
    PUSH  RCX
    ADD
    POP   RCX
    RET  