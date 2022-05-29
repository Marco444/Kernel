GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysMemFrom
GLOBAL sysRegs
GLOBAL generateDivByZero
GLOBAL generateInvalidOpCode
GLOBAL sysOpen
GLOBAL sysClose

GLOBAL loadProces
GLOBAL exit
section .text

;------------------------------
;   Rutina de asm que realiza 
;   la syscall write
;------------------------------
sysWrite:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    push rdi        ; buffer
    push rsi        ; fd

    
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
loadProces:
    push rbp
    mov rbp,rsp

    mov rax,8
    int 80h

    leave
    ret
    
exit:
    push rbp
    mov rbp,rsp

    mov rax,99
    int 80h

    mov rsp,rbp
    pop rbp
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