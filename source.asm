    IN
    POP RCX

    ; HUI = 0;
    PUSH RCX ;elem_size = size_y / 3; 
    PUSH 3
    DIV
    POP RDX
    
    PUSH RCX ;radius = elem_size / 2;
    PUSH 6
    DIV
    POP RBX
    
    PUSH RBX
    PUSH 2
    DIV
    PUSH 0
    JNE :5 ;radius % 2 != 0

    PUSH 1
    PUSH RAX
    ADD
    POP  RAX ;s += 1

:5
    PUSH RFX
    PUSH REX 
    PUSH 210
    DIV
    ADD
    POP RFX ; y += x / 210
    
    PUSH REX
    PUSH REX 
    PUSH 210
    DIV
    PUSH 210
    MUL
    SUB
    POP REX ; x %= 210

    PUSH 60
    PUSH RFX 
    JE :0 ;y == 60 

    CALL :7 ;add elem to []

    PUSH 1
    PUSH REX
    ADD
    POP REX ; x += 1

    PUSH 1 
    PUSH HUI
    ADD
    POP  HUI ;ans++

    JMP :5    ; x == 210

:0        
    SHOW
    HALT
; ______________________dist_from_center__________________________
:3 ;dist_from_center: RCX position -> position
    POP RGX

    PUSH RBX
    PUSH RBX 
    PUSH 2
    DIV
    PUSH 2
    MUL
    SUB ; radius % 2

    PUSH 1
    JE :4  ; radius % 2 == 1 

    PUSH RBX
    PUSH RGX
    JB :4  ; position < radius

    PUSH RGX 
    PUSH RBX
    SUB
    CALL :1
    PUSH 1
    ADD
    RET ;abs(position - radius) + 1

:4
    PUSH RBX
    PUSH RGX
    SUB
    RET ;radius - position
    
; _____________________________ABS_______________________________
:1  ;ABS: element -> abs(element)
    POP  RGX

    PUSH RGX
    PUSH 0
    JAE :2 ;RGM > 0

    PUSH 0
    PUSH RGX
    SUB
    RET ;-RGM

:2
    PUSH RGX
    RET ;RGM
; __________________________ADD_ELEM_______________________________
:7

    PUSH RFX
    CALL :3
    PUSH RFX
    CALL :3
    MUL  ; dist_from_center(radius, y) ^ 2

    PUSH REX
    PUSH 2
    DIV
    PUSH RBX
    SUB
    CALL :3
    PUSH REX
    PUSH 2
    DIV
    PUSH RBX
    SUB
    CALL :3
    MUL ; dist_from_center(radius, x - radius) ^ 2

    ADD

    PUSH RBX
    PUSH RBX
    MUL
    JB :6 ; dist_from_center(radius, y) ^ 2 + dist_from_center(radius, x - radius) ^ 2 < radius ** 2
    
    PUSH RBX 
    PUSH RFX
    JAE  :9 ;radius >= y

    PUSH RBX
    PUSH 5
    MUL
    PUSH RFX
    JBE  :9 ; 5 * radius <= y

    PUSH RBX
    PUSH 1
    ADD
    PUSH REX
    PUSH 2
    DIV
    JAE  :9 ; radius >= x

    PUSH RBX
    PUSH 3
    MUL
    PUSH REX
    PUSH 2
    DIV
    PUSH RAX
    ADD
    JBE  :9 ; 3 * radius <= y + s

    JMP :8 ;radius < y && y < 5 * radius && radius < x && x + s < 3 * radius
:9

    PUSH RFX

    PUSH RDX
    PUSH 2
    MUL
    SUB
    CALL :3
    PUSH RFX
    PUSH RDX
    PUSH 2
    MUL
    SUB
    CALL :3
    MUL  ; dist_from_center(radius, y- 2 * elem_size) ^ 2

    PUSH REX
    PUSH 2
    DIV
    CALL :3
    PUSH REX
    PUSH 2
    DIV
    CALL :3
    MUL ; dist_from_center(radius, x) ^ 2

    ADD

    PUSH RBX
    PUSH RBX
    MUL
    JB :8 ; dist_from_center(radius, y- 2 * elem_size) ^ 2 + dist_from_center(radius, x) ^ 2 < radius ** 2

    PUSH RFX
    PUSH RDX
    PUSH 2
    MUL
    SUB
    CALL :3
    PUSH RFX
    PUSH RDX
    PUSH 2
    MUL
    SUB
    CALL :3
    MUL  ; dist_from_center(radius, y- 2 * elem_size) ^ 2

    PUSH REX
    PUSH 2
    DIV
    PUSH RBX
    PUSH 2
    MUL
    SUB
    CALL :3
    PUSH REX
    PUSH 2
    DIV
    PUSH RBX
    PUSH 2
    MUL
    SUB
    CALL :3
    MUL ; dist_from_center(radius, x - 2 * radius)^ 2

    ADD

    PUSH RBX
    PUSH RBX
    MUL
    JB :8 ; dist_from_center(radius, y- 2 * elem_size) ^ 2 + dist_from_center(radius, x - 2 * radius) ^ 2 < radius ** 2

    PUSH 32
    POP [HUI]

    RET

:8
    PUSH 4279538464
    POP [HUI]

    RET

:6
    PUSH 3340076320
    POP [HUI]

    RET