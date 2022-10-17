
GLOBAL _cli
GLOBAL _sti
GLOBAL _hlt
GLOBAL initialiseContextSchedluer
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL movCero
GLOBAL loadSampleCodeModule
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq06Handler
GLOBAL _exception0Handler
GLOBAL _exception06Handler

%include "contextEngine.inc"
%include "stateEngine.inc"
EXTERN allocMemory
EXTERN freeMemory 
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
EXTERN updateRsp
SECTION .text

initialiseContextSchedluer:
	mov byte [contextOwner],0
	mov byte [aux],0
	call initialiseContextSchedluerEngine
	ret




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
; loadtaskHandler - Se utiliza para cargar un propseso a la pcb
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------
loadtaskHandler:

	
	;loadTask contextLoading 	; rsp+16 estan los
	; mov rdi, contextLoading
	mov [aux2],rdi 
	call loadFirstContext
	; VOY A ARMARLE EL ESTACK DE INTERRUPCIONES, RECIBO EL RSP POR RAX
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

	; me guardo el rsp actual
	mov rdi,rsp
	; pongo el rsp apuntar al nuevo stack
	mov rsp,rax
	; pusheo el stack segment
	push 0x0
	; pusheo el rsp
	push rax
	;pusheo los flags
	push 0x202
	; pusheo el code segment
	push 0x8
	; muevo el puntero que me habia guardado
	mov rcx,[aux2]
	; lo pusheo para terminar el stack de interrupcion
	push rcx     
	; me guardo el rsp del nuevo stack
	mov [aux2],rsp
	; muevo al rsp viejo
	mov rsp,rdi
	; recupero los registros para obtener los parametros
	popState
	; muevo los parametros a los resgistros donde deben ir
	mov rdi,rsi
	mov rsi,rdx
	mov rdx,rcx
	; le pusheo el stado al anterior pues cuando haga el contexto va a hacer un pop del mismo
	pushState
	; recupero el rsp del nuevo stack
	mov rsp,[aux2]
	; le pusheo los registros
	pushState
	; llamo a quien me cambia el contexto
	call switchContext
	; obtengo el rsp desde la funcion que me cambia el contexto
	mov rsp,rax
	popState
	iretq

;-------------------------------------------------------------------------------
; loadSampleCodeModule - Esta funcion va a ser llamada desde kernel.c para poner
;  al sample code module a la lista de procesos
;-------------------------------------------------------------------------------
; @argumentos:  rdi -> puntero a la funcion
;-------------------------------------------------------------------------
loadSampleCodeModule:
	push rbx
	push rbp
	mov rbp,rsp
	mov rbx,rdi
	call loadFirstContext  	
	; me devuelve en rax el stack pointer
	; guardo el rsp actual
	; me muevo al nuevo stack
	mov rsp,rax
	;Armo el stack de interrupcion
	push 0x0
	; pusheo el rsp
	push rax
	;pusheo los flags
	push 0x202
	; pusheo el code segment
	push 0x8
	; muevo el puntero que me habia guardado
	push rbx
	; pusheo el estado para que en el cambio de contexto lo pueda popear    
	pushState
	; guardo el parametro para actualizar el rsp
	mov rdi,rsp
	; actualizo el rsp
	call updateRsp
	; vuelvo al stack anterior
	mov rsp,rbp
	;hago
	pop rbp
	pop rbx
	ret

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
	cmp rax,8					    ; ahora comienzo el switch de las syscalls,
	je loadtaskHandler				    	; si es 8 es la de loadSO
	cmp rax,24					  ;TODO SYCALL DE ALLOC
	je allocMemorySyscall
	cmp rax,25					  ;TODO SYCALL DE free 
	je freeMemorySyscall
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
	pushState
	mov rdi,rsp
	call switchContext				; llamo a la funcion de C que me va a guardar el contexto y copiar el siguiente
	mov rsp,rax
	mov al, 20h						; Indicamos al PIC que termino la interrupcion
	out 20h, al						; Indicamos al PIC que termino la interrupcion
	popState
	call _sti						; activo interrupciones
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


freeMemorySyscall:
	call freeMemory 
	popState
	iretq

allocMemorySyscall:
	call allocMemory
	popState
	iretq
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

	aux2 resq 1
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

	aux3 resq 1
