
#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_
#include <stdint.h>
/*
    Interrup hander la cual llama a el masterHander con un 0 como parametro
    pues es la interrupcion de timerTick

    Argumentos: void
    Return: void
*/
void _irq00Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 1 como parametro
    pues es una interrupcion de teclado

    Argumentos: void
    Return: void
*/
void _irq01Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 2 como parametro
    pues es la cascada del pic

    Argumentos: void
    Return: void
*/
void _irq02Handler(void);

void _irq03Handler(void);

void _irq04Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 5 como parametro
    pues es la interrupcion del usb

    Argumentos: void
    Return: void
*/
void _irq05Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 6 como parametro
    pues es una interrupcion de syscall

    Argumentos: void
    Return: void
*/
void _irq06Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 0 como parametro
    pues es la zero division exception

    Argumentos: void
    Return: void
*/
void _exception0Handler(void);
/*
    Interrup hander la cual llama a el masterHander con un 6 como parametro
    pues es la invalid opcode exception

    Argumentos: void
    Return: void
*/
void _exception06Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

/*
    Funcion la cual nos deja masckear el pic para recibir o no interrupciones

    Argumentos: uint8_t el cual es la mascara
    Return: void
*/
void picMasterMask(uint8_t mask);
/*
    Funcion la cual nos deja masckear el segundo pick para recibir o no
   interrupciones

    Argumentos: uint8_t el cual es la mascara
    Return: void
*/
void picSlaveMask(uint8_t mask);

// Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPTS_H_ */
