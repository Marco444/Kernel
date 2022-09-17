#include "include/stdio.h"


/* assembly functions definitions */
void sysRead(Window window, char * buffer);
void sysWriteHeaderFormat(char * string, int format);
void sysWrite(Window window, char * string);
void sysWriteFormat(Window window, char * string, int format);

void puts_(char * string, Window window){
    sysWrite(window, string);
}

void putsf_(char * string, char format, Window window){
    sysWriteFormat(window, string, format);
}

void putc_(char c, Window window){
    char character[2] = {c, 0};
    sysWrite(window, character);
}


void putcf_(char c, char format, Window window){
    char character[2] = {c, 0};
    sysWriteFormat(window, character, format);
}

void deleteChar(Window window){
    putc_('\b', window);
}



void getKey(Window window, char * buffer){
    sysRead(window, buffer);
}

void putInteger(unsigned int num, Window window) {
    char numToStr[64];
    uintToBase(num, numToStr, 10);
    puts_(numToStr, window);

}

void putHex(int num, Window window){
    char numToStr[11];
    uintToBase(num, numToStr, 16);
    puts_(numToStr, window);
}

void putBin(int num, Window window){
    char numToStr[11];
    uintToBase(num, numToStr, 2);
    puts_(numToStr, window);
}
void printHeader(char * string, int format){
    sysWriteHeaderFormat(string, format);
}


void newLine(Window window) {
    putc_('\n', window);
}
