#ifndef LIB_H
#define LIB_H

#define NULL 0

/* Definiciones de colores propias al manejo de la placa de video */
#include "vga.h"

/* Definiciones de las funciones de assembler que ejecutan las syscalls */
#include "syscalls.h"

/* Definiciones de funciones especificas al modo protegido de intel */
#include "mmu.h"

/* Definiciones de funciones propias que asemejan a math.h de C*/
#include "math.h"

/* Definiciones de funciones propias que asemejan a stdio.h de C*/
#include "stdio.h"

/* Definiciones de funciones propias que asemejan a string.h de C*/
#include "_string.h"

#endif
