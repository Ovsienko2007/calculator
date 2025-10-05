; Ввод
IN         ; a
IN         ; b
IN         ; c
POPR RCX
POPR RBX
POPR RAX
;___________

; дискриминант
PUSHR RBX
PUSHR RBX
MUL

PUSH 4
PUSHR RAX
PUSHR RCX
MUL
MUL
SUB
SQRT
POPR HUI
;____________

;1 корень
PUSH 0
PUSHR RBX
SUB
PUSHR HUI
SUB
PUSH 2
PUSHR RAX
MUL
DIV
;____________

;_2 корень_
PUSH 0
PUSHR RBX
SUB
PUSHR HUI
ADD
PUSH 2
PUSHR RAX
MUL
DIV
;_________

OUT       ;вывод
HALT