%include "stateEngine.inc"
GLOBAL loadContext
;---------------------------------------------------------------------------
;   Funcion la cual va a armar el stack para un nuevo prceos
;---------------------------------------------------------------------------
;   Argumentos: rdi -> window rsi -> argV rdx -> argC   rcx-> rsp r8 -> funcPointer
;---------------------------------------------------------------------------
loadContext:
    push rbp
    mov rbp,rsp

    mov rsp,rcx
	; pusheo el stack segment
	push 0x0
	; pusheo el rsp
	push rcx
	;pusheo los flags
	push 0x202
	; pusheo el code segment
	push 0x8
	; lo pusheo para terminar el stack de interrupcion
	push r8
	pushState
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