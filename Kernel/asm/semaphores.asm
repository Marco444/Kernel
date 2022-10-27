GLOBAL try_lock
GLOBAl unlock

; This routine will check the value untill it is 0
; rdi -> Pointer to the value we want to check
try_lock:
    mov rax, 0
    mov al, 1
    xchg al, [rdi]
    cmp al, 0
    ; Check what will do -> Shall we block a process the call int 0x20 for timer tick?
    jne try_lock
    ret

;   rdi -> Pointer to the value we want to set as 0
unlock:
    mov byte [rdi], 0
    ret