#ifndef STDIO_H
#define STDIO_H
#include "Windows.h"
#include "math.h"

#define EOF -1
#define STDOUT 0
#define STDIN 1

/*
 * Recibe una string para poner en la parte superior de la
 * shell junto con el formato
 */
void printHeader(char *string, int format);

/*
 * Imprime la leyenda default del sistema operativo
 */
void putDefaultHeader();

/*
 * Imprime un mensaje de error (actualmente stderror viene
 * a estar encapsulada en el header)
 */
void perror(char *str);

/*
 * Lee un caracter al buffer str desde el file descriptor fd
 */
void read(char *str, int fd);

/*
 * Escribe los caracteres del buffer str hasta el primer \0
 * en el fd recibido por argumento
 */
void write(char *str, int fd);

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

#endif
