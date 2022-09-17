#ifndef STDIO_H
#define STDIO_H
#include "Windows.h"
#include "math.h"

/*
 * Recibe un entero y lo imprime
 */
void putInteger(unsigned int num, Window window);

/*
 * Recibe una window a la cual hacer una nueva linea
 */
void newLine(Window window);

/*
 * Recibe una cadena de caracteres y las imprima 
 */
void puts_(char * string, Window window);

/*
 * Recibe un caracter y lo imprime 
 */
void putc_(char c, Window window);

/*
 * Recibe una cadena de caracteres, su formato y las imprima 
 */
void putsf_(char * string, char format, Window window);

/*
 * Recibe un caracter, su formato y lo imprime
 */
void putcf_(char c, char format, Window window);

void putHex(int num, Window window);

void putBin(int num, Window window);

/*
 * Recibe una ventana a la cual borrara el ultimo caracter
 */
void deleteChar(Window window);

/*
 * Recibe una ventana donde leera el buffer de teclado, y en el caso de no 
 * haber nada, lo esperara.
 */
void getKey(Window window, char buffer[1]);
void printHeader(char * string, int format);
#endif
