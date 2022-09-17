
GLOBAL _cli
GLOBAL _sti
GLOBAL _hlt

GLOBAL initialiseContextSchedluer

GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL movCero

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq06Handler
GLOBAL _exception0Handler
GLOBAL _exception06Handler

EXTERN reloadProcess
EXTERN pauseProces
EXTERN killProcess
EXTERN int_21
EXTERN ncPrintFD0
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscalls
EXTERN loadFirstContext
EXTERN exitProces
EXTERN switchContext
EXTERN initialiseContextSchedluerEngine
EXTERN readMemoryTo

SECTION .text

initialiseContextSchedluer:
	mov byte [contextOwner],0
	mov byte [aux],0
	call initialiseContextSchedluerEngine
	ret

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

;-------------------------------------------------------------------------------
; Hace una copia de todos los registros generales en el stack frame donde es llamada,
; para preservarlos.
;-------------------------------------------------------------------------------
%macro pushState 0
	;rsp
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

;-------------------------------------------------------------------------------
; Recupera todos los registros generales pusheados anteriormente al stack
;-------------------------------------------------------------------------------
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
%macro popFirstTask 1
	mov rdi,[%1+40]		; alamceno el int fd como primer parametro de mi funcion a loadear
	mov rsi,[%1+32]		; alamceno el int argc como primer parametro de mi funcion a loadear
	mov rdx,[%1+24] 		; alamceno el char ** argv como primer parametro de mi funcion a loadear
	
	
	mov r10,[%1+128]
	mov [rsp],r10
	mov r10,[%1+56]
	mov [rsp+24],r10
%endmacro

;-------------------------------------------------------------------------------
; printMem - copia al buffer recibido por parametro la cantidad de posiciones de 
; memoria desde 
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; exitSyscall - ejecuta el borrado del proceso desde donde se llamo de la tabla
; de procesos para el context switching
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------------
exitSyscall:
	popState
	mov rdi,contextHolder				; paso el primer parametro para copiar el siguiente contexto
										; al exitear el proceso actual
	mov rsi,contextOwner			    ; paso el segundo parametro para actualizar duenio del contexto
										; que estoy copiando al context holder
	call exitProces						; llamo a la funcion de C que maneja el exit
	mov [aux],rax
	mov al, 20h							; signal pic EOI
	out 20h, al							; signal pic EOI
	popContext contextHolder			; actualizo el contexto actual al del proximo proceso a ejecutar
	call _sti
	iretq								; desarmo el stack frame de la interrupcion y hago el ret al proximo proceso

;-------------------------------------------------------------------------------
; loadtaskHandler - ejecuta el borrado del proceso desde donde se llamo de la tabla
; de procesos para el context switching
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------
loadtaskHandler:
	
	loadTask contextLoading 	; rsp+16 estan los
	mov rdi, contextLoading
	call loadFirstContext
	mov [aux],rax
	popState
	call _sti
	iretq

;-------------------------------------------------------------------------------
; LoadSO - cargo la primer task a mi tabla de procesos
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------
loadSO:
	popState

	loadTask contextLoading 	; loadeo la task recibida como primer parametro
	mov rdi, contextLoading		; copio el puntero a la posicion donde tengo el contexto de 
								; primer contexto
	call loadFirstContext
	call _sti
	popFirstTask contextLoading
	iretq
;---------------------------------------------
;	Syscall la cual te termina la ejecucion de un proceso
;---------------------------------------------
;	@arguments: PID
;----------------------------------------------
sysPauseProces:
	call pauseProces
	mov [aux],rax
	popState
	iretq
;------------------------------------------------
;	Syscall la cual mata un programa
;------------------------------------------------
; 	@argumentos: PID
;------------------------------------------------
sysKillProcess:
	call killProcess
	mov [aux],rax
	popState
	iretq
;----------------------------------------------
;	Syscall la cual reloadea el proceso recibido por rdi
;----------------------------------------------
;	@argumentos: PID
;-----------------------------------------------
sysReloadProcess:
	call reloadProcess
	mov [aux],rax
	popState
	iretq
;------------------------------------------------------------------------------------
;	syscall la cual devuelve la cantidad de procesos que se corren
;------------------------------------------------------------------------------------
; @argumentos:
;-----------------------------------------------------------------------------------
processRunning:
	popState
	mov rax,[aux]
	iretq

;------------------------------------------------------------------------------------
;	syscall que imprime a pantalla posiciones de memoria
;------------------------------------------------------------------------------------
; @argumentos:
;-----------------------------------------------------------------------------------
printMemory:
	call readMemoryTo
	popState
	iretq

;-------------------------------------------------------------------------------
; Recibe un numero que determina el numero de interrupcion por hardware y mapea
; a la funcion que maneja esa interrupcion
;-------------------------------------------------------------------------------
%macro irqHandlerMaster 1
	call _cli					; desactivamos las interrupciones
	pushState					; pusheamos todos los registros para preservarlos
    mov r8,%1					; almaceno el numero de la interrupcion 
	cmp  r8,6					; comparo 6 a ver si es una interrupcion de software
	je .syscallsJump			; 
	mov rdi,r8
	call irqDispatcher
	endHardwareInterrupt



.syscallsJump:
	cmp rax,8					; ahora comienzo el switch de las syscalls,
	je loadSO					; si es 8 es la de loadSO
	cmp rax,9
	je loadtaskHandler
	cmp rax,10
	je processRunning
	cmp rax,99					; si es 99 es la de exit
	je exitSyscall
	cmp rax,98					; si es 98 es la syscall de exitear un process
	je sysPauseProces
	cmp rax,97					; si es la 97 es la syscall de reloudear un proceso
	je sysReloadProcess		
	cmp rax,96					; si es la 96 es la syscall de killprocess
	je sysKillProcess
	cmp rax,133					; si es 133 syscall de imprimir memoria desde una posicion
	je printMemory
	mov rcx,rax					; si es otro entonces voy al switch de C
	call syscalls						
	endSoftwareInterrupt						
	
%endmacro

;-------------------------------------------------------------------------------
;  endInterrupt - recupero los registros pusheados al stack, 
; habilita interrupciones y desarma el stack de interrupcion 
;-------------------------------------------------------------------------------
%macro endInterrupt 0
	popState
	call _sti
	iretq
%endmacro

;-------------------------------------------------------------------------------
;  endHardwareInterrupt - comunico al PIC termino la interrupcion de hardware a
;  traves el I/0  los registros pusheados al stack, 
;  habilita interrupciones y desarma el stack de interrupcion 
;-------------------------------------------------------------------------------
%macro endHardwareInterrupt 0
	mov al, 20h
	out 20h, al
	endInterrupt
%endmacro

;--------------------------------------------------------
;
;--------------------------------------------------------
; Argumentos 
;--------------------------------------------------------
%macro exceptionHandler 1
	
	pushContext regsArray
	mov rdi, %1 				; Pasaje de 1 parametro -> Tipo de excepciom
	mov rsi, regsArray			; Pasaje de 2 parametro -> Arreglo de registros asi los imprimo desde C
	mov rdx, contextOwner		; Pasaje de 3 paranetri -> contexto actual (fd actual)
	call exceptionDispatcher	; Llamo al que maneja la excepcion en particular
	pushState
	jmp exitSyscall
%endmacro	


;-------------------------------------------------------------------------------
;  endSoftwareInterrupt - recupero los registros pusheados al stack, 
; habilita interrupciones y desarma el stack de interrupcion 
;-------------------------------------------------------------------------------
%macro endSoftwareInterrupt 0
	endInterrupt
%endmacro



;--------------------------------------------------------------------
; timerTickHandler - Responde a un numero fijo de timer ticks
; primero pushea el contexto actual, luego llama a una funcion en C
; que maneja el context switching desde un contextHolder y contextOwner
; y por ultimo 
;--------------------------------------------------------------------
; Argumentos: -
;--------------------------------------------------------------------
%macro timerTickHandler 1
	call _cli						; desactivo interrupciones
	pushContext contextHolder		; pusheo el contexto actual al contextHolder
	mov rdi, contextHolder			; pusheo como primer argumento el puntero al contexto actual
	mov rsi, contextOwner			; pusheo como segundo parametro el puntero 
	call switchContext				; llamo a la funcion de C que me va a guardar el contexto y copiar el siguiente
	mov al, 20h						; Indicamos al PIC que termino la interrupcion
	out 20h, al						; Indicamos al PIC que termino la interrupcion
	call _sti						; activo interrupciones
	popContext contextHolder		; copio el context holder a mis registros
	iretq
%endmacro

;--------------------------------------------------------
; keyBoardHandler - Esta funcion seteo en 0 el flag de responder a interrupciones
; maskeables y luego hace un hlt -> hace un sleep del micro
;--------------------------------------------------------
;	Argumentos: No recibe, pues solo se utiliza para una 
;	interrupcion.
;--------------------------------------------------------
%macro keyBoardHandler 1
	cli
	pushContext regsStore
	mov rdi, regsStore			; Le paso un puntero al arreglo de resgistros
	call int_21
	popContext regsStore
	sti
	mov al, 20h
	out 20h, al
	iretq
%endmacro

;--------------------------------------------------------
;	DEBUGGING
;--------------------------------------------------------
%macro nothing 1
	mov al, 20h
	out 20h, al
	iretq
%endmacro

;--------------------------------------------------------
; Esta funcion seteo en 0 el flag de responder a interrupciones
; maskeables y luego hace un hlt -> hace un sleep del micro
;--------------------------------------------------------
; Argumentos: -
;--------------------------------------------------------
_hlt:
	cli
	hlt
	ret

;--------------------------------------------------------
; Desabilita las interrupciones (setea el flag
; de interrupciones de hardware en cero)
; "External interrupts disabled at the end of the cli instruction 
; or from that point on until the interrupt flag is set."
;--------------------------------------------------------
; Argumentos: -
;--------------------------------------------------------
_cli:
	cli
	ret

;--------------------------------------------------------
; Habilita las interrupciones (setea el flag
; de interrupciones de hardware en cero)
;--------------------------------------------------------
; Argumentos: -
;--------------------------------------------------------
_sti:
	sti
	ret

;--------------------------------------------------------
; Esta funcion seteo la mascara del pic de interrupciones
; en base a su argumento
;--------------------------------------------------------
; Argumentos: un numero de 8 bits donde el n-esimo bit representa
; si el pin n del pic responde o no a interrupciones (0 si responder
; 1 sino)
;--------------------------------------------------------
picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h, al
    pop rbp
    retn

;--------------------------------------------------------
; Esta funcion seteo la mascara del pic en cascada con el pic master
;--------------------------------------------------------
; Argumentos: un numero de 8 bits donde el n-esimo bit representa
; si el pin n del pic responde o no a interrupciones (0 si responder
; 1 sino)
;--------------------------------------------------------
picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;--------------------------------------------------------------------
; FUNCIONES GLOBALES
;--------------------------------------------------------------------

; -----------------------------------------------------
; 8254 Timer (Timer Tick)
; -----------------------------------------------------
_irq00Handler:
	timerTickHandler 0

; -----------------------------------------------------
; Keyboard
; -----------------------------------------------------
_irq01Handler:
	keyBoardHandler 1
; -----------------------------------------------------
; Syscalls
; -----------------------------------------------------
_irq06Handler:
	irqHandlerMaster 6

;--------------------------------------------------------------------
;Cascade pic never called
;-----------------------------------------------------
_irq02Handler:
	irqHandlerMaster 2

;-----------------------------------------------------
;Serial Port 2 and 4
;-----------------------------------------------------
_irq03Handler:
	irqHandlerMaster 3

;-----------------------------------------------------
;Serial Port 1 and 3	
;-----------------------------------------------------
_irq04Handler:
	irqHandlerMaster 4

;-----------------------------------------------------
;	USB
;-----------------------------------------------------
_irq05Handler:
	irqHandlerMaster 5

;-----------------------------------------------------
;	Zero Division Exception
;-----------------------------------------------------
_exception0Handler:
	exceptionHandler 0

;Zero Division Exception
_exception06Handler:
	exceptionHandler 6


;-----------------------------------------------------
;	BSS
;-----------------------------------------------------
SECTION .bss
	;-----------------------------------------------------
	;	Seccion donde se guarda el contexto para la comunicacion con el back
	;-----------------------------------------------------
	aux resq 1

	;-----------------------------------------------------
	;	Seccion donde se guarda el contexto para la comunicacion con el back
	;-----------------------------------------------------
	contextHolder resq 18			
	;----------------------------------------------------
	;	Guardo el duenio del contexto
	;-----------------------------------------------------
	contextOwner resb 1		
	;-----------------------------------------------------
	;	Seccion donde se guarda el contexto para la comunicacion con el back
	;-----------------------------------------------------
	contextLoading resq 17			
	;-----------------------------------------------------
	;	Arreglo que guarda una copia de los registros 
	;	al momento de una excepcion.
	;-----------------------------------------------------
	regsArray resq 18
	;-----------------------------------------------------
	;	Arreglo que guarda una copia de los registros 
	;	al momento de pedir que saque un snapshot de regs.
	;-----------------------------------------------------
	regsStore resq 18
