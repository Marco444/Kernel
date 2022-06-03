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
GLOBAL sysReadMem
GLOBAL SysProcesses
GLOBAL loadSO
GLOBAL loadProcess
GLOBAL exit
GLOBAL sysKillProcess
GLOBAL sysReloadProcess
GLOBAL printMemFrom

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
    mov rdx,4       ; Longitud (not supported)
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
exit:
    push rbp
    mov rbp,rsp

    mov rax, 99
    int 80h

    leave
    ret


;------------------------------
; sysKillProcess - hace la syscall de matar un proceso
;------------------------------
; Parametro: el fd del programa a matar
;------------------------------
sysKillProcess:
    push rbp
    mov rbp,rsp

    mov rax, 98
    ; rdi -> fd
    int 80h

    leave
    ret

;------------------------------
;   Rutina de asm que realiza 
;   la syscall write con formato
;------------------------------
sysWriteFormat:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    ;push rdi        ; buffer
    ;push rsi        ; fd

    
    ;mov rsi, [2 argumento]     ; buffer
    mov rax,122       ; Numero de syscall
;    mov rdi,0     ; fd
    ;mov rdx,0x3       ; Longitud (not supported)
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

    mov rax, 3      ; Numero de syscall
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
;   rdi -> puntero al buffer de unsigned int donde copiar
;   rsi -> posicion desde donde se comienza a copiar
;   rdx -> cantidad de caracteres en copiar
;------------------------------------------------------
sysReadMem:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 123    ; Nuemro de syscall
    int 80h         ; "Che Kernel"

    pop rbx         ; Preservar rbx

    mov rsp, rbp    ; Stack frame
    pop rbp         ; Stack frame

    ret

printMemFrom:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 123    ; Nuemro de syscall
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