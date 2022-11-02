GLOBAL sysGetRegsSnapshot
GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysMemFrom
GLOBAL sysRegs
GLOBAL generateDivByZero
GLOBAL generateInvalidOpCode
GLOBAL sysOpen
GLOBAL sysClose
GLOBAL sysClearScreen
GLOBAL sysWriteFormat
GLOBAL sysWriteHeaderFormat
GLOBAL sysReadMem
GLOBAL SysProcesses
GLOBAL loadSO
GLOBAL loadProcess
GLOBAL waitPid
GLOBAL exit_
GLOBAL sysReloadProcess
GLOBAL printMemFrom
GLOBAL sysPauseProcess
GLOBAL sysAlloc
GLOBAL sysFree
GLOBAL sysMemoryDump 
GLOBAL sysPsDump
GLOBAL sysPipesDump
GLOBAL sysBlockProcess
GLOBAL sysNiceProcess
GLOBAL sysKillProcess
GLOBAL sysUnblockProcess
GLOBAL semOpen
GLOBAL semClose
GLOBAL semWait
GLOBAL semSignal
GLOBAL myYield
GLOBAL sysPipe
GLOBAL sysDup2
GLOBAL sysGetCurrentPid
section .text




;------------------------------
;   Rutina de asm que realiza 
;   la syscall write
;------------------------------
sysWrite:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    ;push rdi        ; buffer
    ;push rsi        ; fd

    
    ;mov rsi, [2 argumento]     ; buffer
    mov rax,1       ; Numero de syscall
    ;mov rdi,0 [1 argumento]    ; fd
    int 80h         ; "Che kernel"
    
    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret    

;------------------------------
; exit - hace la syscall de sysExit 
;------------------------------
; Parametro: un char* buffer, que toma solo un caracter de la lectura.
;------------------------------
exit_:
    push rbp
    mov rbp,rsp

    mov rax, 99
    int 80h

    leave
    ret


;------------------------------
; sysPause - hace la syscall de pausar un proceso
;------------------------------
; Parametro: el fd del programa a pausar
;------------------------------
sysPauseProcess:
    push rbp
    mov rbp,rsp

    mov rax, 98
    ; rdi -> fd
    int 80h

    leave
    ret


waitPid:
    push rbp
    mov rbp,rsp

    mov rax, 15
    ; rdi -> fd
    int 80h

    leave
    ret

sysPipesDump:
    push rbp
    mov rbp,rsp
    mov rax, 200 
    int 80h

    leave
    ret

sysFree:
    push rbp
    mov rbp,rsp
    mov rax, 25
    int 80h

    leave
    ret
sysKillProcess:
    push rbp
    mov rbp,rsp
    mov rax, 12
    int 80h

    leave
    ret

sysGetCurrentPid:
    push rbp
    mov rbp,rsp
    mov rax, 17
    int 80h

    leave
    ret

sysBlockProcess:
    push rbp
    mov rbp,rsp
    mov rax, 11
    int 80h

    leave
    ret

sysNiceProcess:
    push rbp
    mov rbp,rsp
    mov rax, 14
    int 80h

    leave
    ret

sysUnblockProcess:
    push rbp
    mov rbp,rsp
    mov rax, 13
    int 80h

    leave
    ret
 sysPsDump:
    push rbp
    mov rbp,rsp
    mov rax, 21
    ; rdi -> fd
    int 80h

    leave
    ret

sysMemoryDump:
    push rbp
    mov rbp,rsp
    mov rax, 26
    ; rdi -> fd
    int 80h

    leave
    ret

sysDup2:
    push rbp
    mov rbp,rsp
    mov rax, 223
    int 80h

    leave
    ret

sysPipe:
    push rbp
    mov rbp,rsp
    mov rax, 222
    int 80h

    leave
    ret

sysAlloc:
    push rbp
    mov rbp,rsp
    mov rax, 24
    ; rdi -> fd
    int 80h

    leave
    ret
;------------------------------
;   Rutina de asm que realiza 
;   la syscall write de header
;   de pantalla con formato.
;------------------------------
sysWriteHeaderFormat:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    ;push rdi        ; buffer
    ;push rsi        ; fd

    
    
    mov rax,125       ; Numero de syscall
;   
    mov rdx, rsi    ; 2 arg -> formato
    mov rsi, rdi    ; 1 arg -> buffer
    int 80h         ; "Che kernel"
    
    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret    

;------------------------------
; Rutina de asm que realiza 
; la syscall read
;------------------------------
; Parametro: un char* buffer, que toma solo un caracter de la lectura.
;------------------------------
sysRead:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    ;mov rsi,[2 argumento]  ; buffer
    mov rax,0       ; Numero de syscall
    ;mov rdi,[1 argumento]  ; fd
    mov rdx,4       ; Longitud (not supported)
    int 80h         ; "Che Kernel"
    
    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame
    
    ret

;------------------------------
;   Rutina de asm que realiza 
;   la syscall de obtener la fecha en string buffer
;------------------------------
sysTime:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rsi, rdi    ; El primer parametro de la funcion lo paso por rsi
    mov rax, 120    ; Numero de syscall
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame
    
    ret
;-------------------------------------------
;   rutina la cual hace la syscall de load una funcion
;   argumentos: puntero a la funcion,fd, int argc, args
;-------------------------------------------
loadProcess:
    push rbp
    mov rbp,rsp

    mov rax,9
    int 80h

    leave
    ret

loadSO:
    push rbp
    mov rbp,rsp

    mov rax,8
    int 80h

    leave
    ret
    

;------------------------------
;   Rutina de asm que realiza 
;   la syscall de open para un fd
;------------------------------
sysOpen:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame
    push rbx        ; Preservar rbx

    mov rax, 2      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx
    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

;------------------------------
;   Rutina de asm que realiza 
;   la syscall de close para un fd
;------------------------------
sysClose:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 300      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

sysClearScreen:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 121      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

;------------------------------------------------------
; Sycall la cual la memoria desd
;--------------------------------------------------------
; Argumentos:
;   rdi -> posicion desde donde imprimir la memoria
;   rsi -> file descriptor a donde imprimirla
;------------------------------------------------------
printMemFrom:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 133    ; Nuemro de syscall
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret
;------------------------------------------------------
; Sycall la cual te devuelve la cantidad de procesos corriendo en un momento
;--------------------------------------------------------
;    Retorno la cantidad de procesos
;------------------------------------------------------
SysProcesses:
    push rbp
    mov rbp,rsp

    mov rax,10
    int 80h

    mov rsp,rbp
    pop rbp
    ret
;--------------------------------------------------------
;   Sycall la cual mandandole el PID de un proceso lo relodea
;---------------------------------------------------------
;   @argumentos: int PID
;--------------------------------------------------------
sysReloadProcess:
    push rbp
    mov rbp,rsp

    mov rax,97
    int 80h

    mov rsp,rbp
    pop rbp
    ret


sysGetRegsSnapshot:
    push rbp                ; Stack frame
    mov rbp,rsp             ; Stack frame

    mov rax,124             ; Numero de syscall
    mov rsi, rdi            ; Le paso como 2 argumento mi 1 argumento (buffer)
    int 80h                 ; "Che kernel"

    mov rsp,rbp             ; Stack frame
    pop rbp                 ; Stack frame
    ret

; -------------------------------------------------
;   Semaphores Syscalls
; -------------------------------------------------
semOpen:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 126      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

semClose:
push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 127      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

semWait:
push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 128      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

semSignal:
push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 129      ; Numero de syscall
    ; En rdi ya tengo el parametro fd
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

myYield:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 16     ; Numero de syscall
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret
