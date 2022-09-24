
%ifndef contextEngine_MAC 
%define contextEngine_MAC 
;-------------------------------------------------------------------------------
; Almaceno el contexto del proceso actual (definido como todos los registros 
; generales, asi como el RIP y los registros de flag) a la posicion de memoria
;-------------------------------------------------------------------------------
; Argumento: posicion de memoria a donde copiar el contexto
;-------------------------------------------------------------------------------
%macro pushContext 1

	;pusheo los registros generales a la posicion pasada como argumentos
	mov [%1], rax
	mov [%1+8], rbx
	mov [%1+16], rcx
	mov [%1+24], rdx
	mov [%1+32], rsi
	mov [%1+40], rdi
	mov [%1+48], rbp
	
	; un hueco de un registro 
	mov [%1+64], r8
	mov [%1+72], r9
	mov [%1+80], r10
	mov [%1+88], r11
	mov [%1+96], r12
	mov [%1+104], r13
	mov [%1+112], r14
	mov [%1+120], r15

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
	;  Stack Segment (SS - )
	; -------------------------------------

	mov rax, [rsp+24]			; almaceno el valor del RSP de la interrupcion en rax
	mov [%1+56], rax	        ; guardamos el valor del RSP en la tabla de contexto 
	mov rax, [rsp]				; guardamos la posicion del RIP

	mov [%1+128], rax	        ; lo guardamos en la posicion de memoria
	mov rax, [rsp+16]			; tomo del interrupt frame el valor de los flags
	mov [%1+136], rax	        ; lo guardo
%endmacro

;-------------------------------------------------------------------------------
; Recupero el contexto del proceso (definido como todos los registros 
; generales, asi como el RIP y los registros de flag)cuyo contexto almacene  a
; la posicion de memoria recibida como parametro
;-------------------------------------------------------------------------------
; Argumento: posicion de memoria a donde copiar el contexto
;-------------------------------------------------------------------------------
%macro popContext 1
	
	;popeo los registros especiales
	mov rax, [%1+56]					; almaceno en rax el valor que almacene de RSP de la interrupcion
	mov [rsp+24],rax					; piso la posicion del 
	mov rax, [%1+128]
	mov [rsp],rax
	mov rax, [%1+136]
	mov [rsp+16],rax

	;popeo los registros generales
	mov  rax,[%1]
	mov  rbx,[%1+8]
	mov  rcx,[%1+16]
	mov  rdx,[%1+24]
	mov  rsi,[%1+32]
	mov  rdi,[%1+40]
	mov  rbp,[%1+48]
	mov  r8,[%1+64]
	mov  r9,[%1+72]
	mov  r10,[%1+80]
	mov  r11,[%1+88]
	mov  r12,[%1+96]
	mov  r13,[%1+104]
	mov  r14,[%1+112]
	mov  r15,[%1+120]
%endmacro

;-------------------------------------------------------------------------------
; Recibe una posicion de memoria desde la cual va a comenzar el stack frame de mi 
; funcion
; loadProcess(cmd, window, 0, args);
;-------------------------------------------------------------------------------
%macro loadTask 1
	
	; ---- REGISTROS PUSHEADOS 			<= rsp
	; ---- STACK INTERRUPCION
	; ebp
	; ret
	; donde REGISTROS PUSHEADOS son 15 registros de 8 bytes, por lo tanto ocupan 120 bytes en mi stack
	; Por lo tanto rsp + 120 apunta al principio de mi stack de interrupcion
	; por ultimo, rsp + 120 + 16 apunta al registro de flags
	
	mov [%1+40], rsi		; alamceno el int fd como primer parametro de mi funcion a loadear
	mov [%1+32], rdx		; alamceno el int argc como primer parametro de mi funcion a loadear
	mov [%1+24], rcx		; alamceno el char ** argv como primer parametro de mi funcion a loadear
	
	mov [%1+128], rdi	 	; alamceno el puntero al comienzo de mi funcion en la posicion donde pusheo el RIP

	mov r15, [rsp + 136]	; almaceno el registro de flags
	mov [%1+136], r15 		; lo copio a la posicion donde despues pusheo mi context
	;mov r10,[rsp+24]
	
	;sub r10,1000
	;mov [%1+56],r10
%endmacro
%macro popFirstTask 1
	mov rdi,[%1+40]		; alamceno el int fd como primer parametro de mi funcion a loadear
	mov rsi,[%1+32]		; alamceno el int argc como primer parametro de mi funcion a loadear
	mov rdx,[%1+24] 		; alamceno el char ** argv como primer parametro de mi funcion a loadear
	
	
	mov r10,[%1+128]
	mov [rsp],r10
	mov r10,[%1+56]
	mov [rsp+24],r10
%endmacro


%endif
