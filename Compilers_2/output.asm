%include "io.inc"
section .data
    x db 0
    z db ""
    y db 0
section .text
    global main
main:
    ; Assignment to z
    mov ebx, z ; Get the address of the variable
    mov eax, "www" ; String content
    mov [ebx], eax ; Store the result in the variable
    ; Assignment to y
    mov ebx, y ; Get the address of the variable
    mov eax, 1 ; Load constant 1 to eax
    mov [ebx], eax ; Store the result in the variable
    ; Condition Operator
    mov eax, [x] ; Load variable x to eax
    ; Assignment to x
    mov ebx, x ; Get the address of the variable
    mov edx, [x] ; Load variable x to edx
    mov eax, ebx
    mov edx, 1 ; Load constant 1 to edx
    sub eax, edx ; Subtract the result from eax
    mov [ebx], eax ; Store the result in the variable
    jmp endif
else:
    ; Assignment to z
    mov ebx, z ; Get the address of the variable
    mov eax, "gg" ; String content
    mov [ebx], eax ; Store the result in the variable
endif:
    ; Assignment to y
    mov ebx, y ; Get the address of the variable
    mov eax, 0 ; Load constant 0 to eax
    mov [ebx], eax ; Store the result in the variable
    xor eax, eax
    ret