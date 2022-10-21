
#ifndef KEYBOARD_H_
#define KEYBOARD_H_
/*
    Funcion la cual recibe un caracter gracias a una interrupcion de teclado
    y lo guarda en el buffer

    Argumentos: char c
    Return: void
*/
void saveBuffer(char c);
/*
    Funcion la cual lee desde la memoria de teclado la tecla
    que fue apretada

    Argumentos:
    Return: la tecla que fue apretada en hexa
*/
extern char readKey();

/*
    Funcion la cual es llamada desde la syscall de read que lo que hace es
   devolver el proximo char del buffer

    Argumentos: buffer en donde va a copiar dicho caracter
    Return: void
*/
void getBufferChar(char *sysBuffer);

/*
    Tabla la cual segun el valor que se ingresa devuelve el
    valor en ascii
*/
char getValue(int c);

#endif