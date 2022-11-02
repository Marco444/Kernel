#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>
#define COLUMS 160
#define OFFSET 80

/*
 *  VGA text mode: codigos para formato de texto (color y fondo)
 */
#define BLACK 0x0
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define LIGHT_GRAY 0x07
#define DARK_GRAY 0x08
#define LIGHT_BLUE 0x09
#define LIGHT_GREEN 0x0a
#define LIGHT_CYAN 0x0b
#define LIGHT_RED 0x0c
#define PINK 0x0d
#define YELLOW 0x0e
#define WHITE 0x0f
#define BLACK_BACKGROUND 0x00
#define BLUE_BACKGROUND 0x10
#define GREEN_BACKGROUND 0x20
#define CYAN_BACKGROUND 0x30
#define RED_BACKGROUND 0x40
#define MAGENTA_BACKGROUND 0x50
#define BROWN_BACKGROUND 0x60
#define LIGHT_GRAY_BACKGROUND 0x70
#define DARK_GRAY_BACKGROUND 0x80
#define LIGHT_BLUE_BACKGROUND 0x90
#define LIGHT_GREEN_BACKGROUND 0xa0
#define LIGHT_CYAN_BACKGROUND 0xb0
#define LIGHT_RED_BACKGROUND 0xc0
#define PINK_BACKGROUND 0xd0
#define YELLOW_BACKGROUND 0xe0
#define WHITE_BACKGROUND 0xf0

/*
    Funcion la cual imprime en el fd indicado lo que
    hay en String debe estar pues la usa al inicializar
    el kernel.

    Params: const char * string lo que se va a imprimir
            fd
    Retunr: void
*/
void ncPrintAtFD(const char *string);

/*
    Funcion la cual imprime en el fd indicado lo que
    hay en String debe estar pues la usa al inicializar
    el kernel. Permite determinar el formato del mismo.

    Params: const char * string lo que se va a imprimir
            fd
            formato
    Retunr: void
*/
void ncPrintAtFD_Format(const char *string, int format);

/*
    Funcion la cual imprime en el FD0 lo que hay en String
    debe estar pues la usa al inicializar el kernel

    Params: const char * string lo que se va a imprimir
    Retunr: void
*/
void ncPrint(const char *string);
/*
    Funcion la cual va a imprimir un caracter a la vez en FD0

    Params: char caracter
    Return: void
*/
void ncPrintChar(char character);
/*
    Funcion la cual te genera una nueva linea en el FD0

    Params: void
    Return: void
*/
void ncNewline();
/*
    Funcion la cual te imprime un numero decimal

    Params: int value
    Return: void
*/
void ncPrintDec(int value);

void ncPrintDecAtFD(uint64_t value);
/*
    Funcion la cual te imprime un numero hexa

    Params: int value
    Return: void
*/
void ncPrintHex(uint64_t value);

void ncPrintHexAtFD(uint64_t value);

/*
    Funcion la cual te imprime un numero binario

    Params: int value
    Return: void
*/
void ncPrintBin(uint64_t value);

void ncPrintBinAtFD(uint64_t value);

/*
    Funcion la cual te imprime un numero en cualquier base

    Params: int value, int base
    Return: void
*/
void ncPrintBase(uint64_t value, uint32_t base);
/*
    Funcion la cual borra todo el contenido de la pantalla

    Params: void
    Return: void
*/
void ncClear();

/*
    Funcion la cual borra todo el contenido de la pantalla
    en un fd determinado.

    Params: void
    Return: void
*/
void fdClear(int fd);

/*
    Funcion la cual resetea los FD y limpia la pantalla

    Params: void
    Return: void
*/
void resetVideo();
/*
    Funcion la cual imprime en el FD0 lo que haya en String

    Params: char *
    Return: void
*/

// ---------------------------------------------------
// FD0
// ---------------------------------------------------
void ncPrintFD0(char *string);
/*
    Funcion la cual imprime un char en el FD0

    Params: char *
    Return: void
*/

void ncPrintFD0_Format(char *string, char format);

void ncPrintFD0Char(char character);
/*
    Funcion la cual imprime un char en el FD1

    Params: char *
    Return: void
*/
void ncPrintFD0Char_Format(char character, char format);

// ---------------------------------------------------
// FD1
// ---------------------------------------------------

/*
    Funcion la cual imprime en el FD2 lo que haya en String

    Params: char *
    Return: void
*/

// ---------------------------------------------------
// FD2
// ---------------------------------------------------
/*
    Funcion la cual imprime en el FD2 lo que haya en String

    Params: char *
    Return: void
*/

/*
    Funcion la cual se fija si es un enter y genera el \n

    Params: char c, int FD
    Return: void
*/

// ---------------------------------------------------
//
// ---------------------------------------------------
int checkEnter(char c);
/*
    Funcion la cual hace la sysCall de open

    Params: int Fd
    Return: void
*/
void openFD(int fd);
/*
    Funcion la cual hace la sysCall de exit

    Params: int Fd
    Return: void
*/
void closeFD(int fd);
/*
    Funcion la cual imprime en el header de pantalla

    Params: char* string, int format
    Return: void
*/
void ncPrintHeader(char *string, int format);
/*
    Funcion la cual limpia el header de pantalla

    Params: voi
    Return: void
*/
void ncClearHeader();

#endif
