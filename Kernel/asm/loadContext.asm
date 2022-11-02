GLOBAL loadContext
;---------------------------------------------------------------------------
;   Funcion la cual va a armar el stack para un nuevo prceos
;---------------------------------------------------------------------------
;   Argumentos: rdi -> argC  rsi -> argV   rdx-> rsp rcx -> funcPointer
;---------------------------------------------------------------------------
loadContext:
    push rbp
    mov rbp,rsp
	
    mov rsp,rdx
	and rsp,-16
	; pusheo el stack segment
	push 0x0
	; pusheo el rsp
	push rdx
	;pusheo los flags
	push 0x202
	; pusheo el code segment
	push 0x8
	; lo pusheo para terminar el stack de interrupcion
	push rcx

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
    mov rax,rsp
    mov rsp,rbp
    pop rbp
    ret
	;-----------------------------------------------------------------------
	; pusheo los registros especiales, al entrar aca voy a tener el rsp
	; -------------------------------------
	;  error code   						 
	; -------------------------------------
	;  Instruction Pointer (RIP - 8 bytes)      <= rsp
	; -------------------------------------
	;  Code Segment (CS - 2 bytes)        
	; -------------------------------------
	;  Register Flags (RFLAGS - 8 bytes) 
	; -------------------------------------
	;  Stack Pointer (RSP - 8 bytes)       
	; -------------------------------------
	;  Stack Segment (SS - 2 bytes)
	; -------------------------------------