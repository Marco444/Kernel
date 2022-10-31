GLOBAL tryLock
GLOBAl unlock

; This routine will check the value untill it is 0
; rdi -> Pointer to the value we want to check
tryLock:
    mov rax, 0
    mov al, 1
    xchg al, [rdi]
    cmp al, 0
    jne tryLock
    ret


;   rdi -> Pointer to the value we want to set as 0
unlock:
    mov byte [rdi], 0
    ret