
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
GLOBAL timerTickInt
%include "contextEngine.inc"
%include "stateEngine.inc"

EXTERN psDump
EXTERN pipesDump
EXTERN allocMemory
EXTERN freeMemory 
EXTERN memoryDump 
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
EXTERN readMemoryTo
EXTERN updateRsp
EXTERN getProcesses
EXTERN blockProcess
EXTERN killProcess
EXTERN nice
EXTERN addWaitingQueue
EXTERN unblockProcess
EXTERN yield
SECTION .text





;-------------------------------------------------------------------------------
; exitSyscall - ejecuta el borrado del proceso desde donde se llamo de la tabla
; de procesos para el context switching
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------------
exitSyscall:								
	call exitProces
	mov rsp,rax
	popState
	call _sti
	iretq

;-------------------------------------------------------------------------------
; loadtaskHandler - Se utiliza para cargar un propseso a la pcb
;-------------------------------------------------------------------------------
; @argumentos:  
;-------------------------------------------------------------------------
loadtaskHandler:
	call loadFirstContext
	call _sti
	popStateWithOutRax
	iretq


;---------------------------------------------
;	Syscall la cual te termina la ejecucion de un proceso
;---------------------------------------------
;	@arguments: PID
;----------------------------------------------
sysPauseProces:
	;call pauseProces
	mov [aux],rax
	popStateWithOutRax
	iretq
;------------------------------------------------
;	Syscall la cual mata un programa
;------------------------------------------------
; 	@argumentos: PID
;------------------------------------------------
sysKillProcess:
	call killProcess
	popStateWithOutRax
	iretq


sysNiceProcess:
	call nice
	popStateWithOutRax
	iretq
;----------------------------------------------
;	Syscall la cual reloadea el proceso recibido por rdi
;----------------------------------------------
;	@argumentos: PID
;-----------------------------------------------
sysReloadProcess:
	call reloadProcess
	mov [aux],rax
	popStateWithOutRax
	iretq
;------------------------------------------------------------------------------------
;	syscall la cual devuelve la cantidad de procesos que se corren
;------------------------------------------------------------------------------------
; @argumentos:
;-----------------------------------------------------------------------------------
processRunning:
	call getProcesses
	popStateWithOutRax
	iretq
addWaitingQueueAsm:
	call addWaitingQueue
	popStateWithOutRax
	iretq
yieldAsm:
	call yield
	popStateWithOutRax
	iretq
;------------------------------------------------------------------------------------
;	syscall que imprime a pantalla posiciones de memoria
;------------------------------------------------------------------------------------
; @argumentos:
;-----------------------------------------------------------------------------------
printMemory:
	call readMemoryTo
	popStateWithOutRax
	iretq
blockProcessAsm:
	call blockProcess
	popStateWithOutRax
	iretq
killProcessAsm:
	call killProcess
	popStateWithOutRax
	iretq
unBlockProcessAsm:
	call unblockProcess
	popStateWithOutRax
	iretq
;-------------------------------------------------------------------------------
; Recibe un numero que determina el numero de interrupcion por hardware y mapea
; a la funcion que maneja esa interrupcion
;-------------------------------------------------------------------------------
%macro irqHandlerMaster 1
	call _cli					; desactivamos las interrupciones
	pushStateWithOutRax					; pusheamos todos los registros para preservarlos
    mov r10,%1					; almaceno el numero de la interrupcion 
	cmp  r10,6					; comparo 6 a ver si es una interrupcion de software
	je .syscallsJump			; 
	mov rdi,r8
	call irqDispatcher
	endHardwareInterrupt



.syscallsJump:
	cmp rax,8					    ; ahora comienzo el switch de las syscalls,
	je loadtaskHandler				    	; si es 8 es la de loadSO
	cmp rax,24					  
	je allocMemorySyscall
	cmp rax,25					
	je freeMemorySyscall
	cmp rax,26
	je memoryDumpSyscall 
	cmp rax, 200
	je pipesDumpSyscall
	cmp rax, 21
	je psDumpSyscall
	cmp rax,9
	je loadtaskHandler
	cmp rax, 14
	je sysNiceProcess
	cmp rax,11
	je blockProcessAsm
	cmp rax,12
	je killProcessAsm
	cmp rax,13
	je unBlockProcessAsm
	cmp rax,15
	je addWaitingQueueAsm
	cmp rax,16
	je yieldAsm
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

timerTickInt:
	int 0x20
	ret
;-------------------------------------------------------------------------------
;  endInterrupt - recupero los registros pusheados al stack, 
; habilita interrupciones y desarma el stack de interrupcion 
;-------------------------------------------------------------------------------
%macro endInterrupt 0
	popStateWithOutRax
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
	pushState
	mov rdi,rsp
	call switchContext				; llamo a la funcion de C que me va a guardar el contexto y copiar el siguiente
	mov rsp,rax
	mov al, 20h						; Indicamos al PIC que termino la interrupcion
	out 20h, al						; Indicamos al PIC que termino la interrupcion
	popState
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


memoryDumpSyscall:
	call memoryDump
	popStateWithOutRax
	iretq

freeMemorySyscall:
	call freeMemory 
	popStateWithOutRax
	iretq

pipesDumpSyscall:
	call pipesDump 
	popStateWithOutRax
	iretq

psDumpSyscall:
	call psDump 
	popStateWithOutRax
	iretq

allocMemorySyscall:
	call allocMemory
	popStateWithOutRax
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
