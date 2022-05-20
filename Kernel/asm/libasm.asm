GLOBAL cpuVendor
GLOBAL readKey
GLOBAL get_rtc_seconds
GLOBAL get_rtc_minutes
GLOBAL get_rtc_hours
GLOBAL get_rtc_day
GLOBAL get_rtc_weekday
GLOBAL get_rtc_month
GLOBAL get_rtc_year

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;------------------------------------------------
;	Funcion la cual lee de la memoria de teclado
;	la tecla por la cual se interrumpio
;------------------------------------------------
;	Argumentos:
;------------------------------------------------
;	Retorno: numero hexa que representa la tecla
;------------------------------------------------
readKey:
	push rbp
	mov rbp,rsp
	xor rax,rax
.loop:
	in al,64h		 ; preguntar la diferencia de poner 64 h y 0x64
	and al, 0x01     ; Output register 60h should only be read IIF Bit 0 of status port is set to 1.
	cmp al, 0
	je .loop
	in al,60h
	mov rsp,rbp
	pop rbp
	ret

;---------------------------------------------------
;	Funcion la cual retorna los segundos del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Segundos del sistema (en rax)
;---------------------------------------------------
get_rtc_seconds:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0       ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret


;---------------------------------------------------
;	Funcion la cual retorna los minutos del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: - 
;---------------------------------------------------
;	Retorno: Minutos del sistema (en rax)
;---------------------------------------------------
get_rtc_minutes:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x2       ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret


;---------------------------------------------------
;	Funcion la cual retorna los segundos del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Hora del sistema (en rax)
;---------------------------------------------------
get_rtc_hours:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x4       ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret

;---------------------------------------------------
;	Funcion la cual retorna el dia del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Segundos del sistema (en rax)
;---------------------------------------------------
get_rtc_day:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x07    ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret

;---------------------------------------------------
;	Funcion la cual retorna el dia de la semana 
;	del sistema mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Segundos del sistema (en rax)
;---------------------------------------------------
get_rtc_weekday:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x07    ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret

;---------------------------------------------------
;	Funcion la cual retorna el mes del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Segundos del sistema (en rax)
;---------------------------------------------------
get_rtc_month:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x8     ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret

;---------------------------------------------------
;	Funcion la cual retorna el anio del sistema
;	mediante el rtc (real time clock)
;---------------------------------------------------
;	Argumentos: -
;---------------------------------------------------
;	Retorno: Segundos del sistema (en rax)
;---------------------------------------------------
get_rtc_year:
    push rbp        ; Stack frame
    mov rbp, rsp    ; Stack frame

    push rbx        ; Preservar rbx

    mov rax, 0      ; Lleno de 0 el rax

    mov al, 0x9    ; Indico el registro de rtc
    out 70h, al     ; Indico el registro de rtc
    in al, 71h      ; Leo el rtc

    pop rbx         ; Preservar rbx
    
    mov rsp, rbp   ; Stack frame
	pop rbp         ; Stack frame

    ret
