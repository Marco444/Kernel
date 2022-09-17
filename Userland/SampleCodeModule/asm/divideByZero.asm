GLOBAL divideByZeroAsm


section .text
    %macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro
divideByZeroAsm:
    
    ;push rsp 
    ;push rbp
    ;push rbx
    ;push r12
    ;push r13
    ;push r15

    pushState

    mov rax, 0
    mov rbx, 2
    mov rcx, 3
    mov rdx, 4
    mov rsi, 5
    mov rdi, 6
    ;mov rbp, 7
    ;mov rsp, 7
    mov r8, 8
    mov r9, 9
    mov r10, 10
    mov r11, 11
    mov r12, 12
    mov r13, 13
    mov r14, 14
    mov r15, 15
    
    div rax
    
    ;pop r15
    ;pop r13
    ;pop r12
    ;pop rbx
    ;pop rbp
    ;pop rsp

    popState

    ret



