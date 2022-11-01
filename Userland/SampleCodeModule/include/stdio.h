#ifndef STDIO_H
#define STDIO_H
#include "Windows.h"
#include "math.h"

#define STDOUT 0
#define STDIN 1
/*
 * Recibe un entero y lo imprime
 */
void putInteger(int num);

/*
 * una nueva linea
 */
void newLine();

/*
 * Recibe una cadena de caracteres y las imprima
 */
void puts_(char *string);

/*
 * Recibe un caracter y lo imprime
 */
void putc_(char c);

/*
 * Recibe una cadena de caracteres, su formato y las imprima
 */
void putsf_(char *string, char format);

/*
 * Recibe un caracter, su formato y lo imprime
 */
void putcf_(char c, char format);

void putHex(int num);

void putBin(int num);

/*
 * Recibe una ventana a la cual borrara el ultimo caracter
 */
void deleteChar();

/*
 * Recibe una ventana donde leera el buffer de teclado, y en el caso de no
 * haber nada, lo esperara.
 */
void getKey(char buffer[1]);
void printHeader(char *string, int format);
#endif
