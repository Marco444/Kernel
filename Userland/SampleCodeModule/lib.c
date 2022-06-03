#include "include/lib.h"
#include "include/Windows.h"
#include <stdint.h>


int integerOverflowAddition(int a, int b) {
    return !(0 < (a + b) && (a + b)  < MAX_INT- 1);
}

int longOverflowAddition(unsigned long long a, unsigned long long b) {
    return !(0 < (a + b) && (a + b)  < MAX_UNSIGNED_LONG_LONG -  1);
}

int atoi_(char * str) {
  int result;
  int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+'))
  {
      if (*str == '-')
        puiss = puiss * -1;
      str++;
  }
  while ((*str >= '0') && (*str <= '9'))
  {
      result = (result * 10) + ((*str) - '0');
      str++;
  }
  return (result * puiss);
}

void puts_(char * string, Window window){
    sysWrite(window, string);
}

void putsf_(char * string, char format, Window window){
    sysWriteFormat(window, string, format);
}

int strlen_(const char *str) {
    const char *s;

    for (s = str; *s; ++s);
    return (int) (s - str);
}


int KMP(const char *text, const char *pattern, int textLen, int patternLen) {
    // base case 1: pattern is NULL or empty
    if (*pattern == '\0' || patternLen == 0) return 0;

    // base case 2: text is NULL, or text's length is less than that of pattern's
    if (*text == '\0' || patternLen > textLen) return -1;

    // next[i] stores the index of the next best partial match
    int next[patternLen + 1];

    for (int i = 0; i < patternLen + 1; i++) {
        next[i] = 0;
    }

    for (int i = 1; i < patternLen; i++) {
        int j = next[i + 1];

        while (j > 0 && pattern[j] != pattern[i]) {
            j = next[j];
        }

        if (j > 0 || pattern[j] == pattern[i]) {
            next[i + 1] = j + 1;
        }
    }

    for (int i = 0, j = 0; i < textLen; i++) {
        if (*(text + i) == *(pattern + j)) {
            if (++j == patternLen) return i - j + 1;
        } else if (j > 0) {
            j = next[j];
            i--;    // since `i` will be incremented in the next iteration
        }
    }

    return -1;
}


int substring(char *str, char *substr) {
    return KMP(str, substr, strlen_(str), strlen_(substr));
}


int isLongerThan(const char *s, int size) {

    for (int i = 0; i < size; i++)
        if (s[i] == NULL_)
            return 0;

    return 1;
}

int removeTrailingSpaces(char *str) {

    //variable para recorrer toda la string
    int i = 0;

    //borro todos los caracteres (si o si mis comandos
    //comienzan con una letra)
    while (str[i] != NULL_ && !IS_LETTER(str[i])) i++;

    //almaceno el indice del primer caracter letra
    int start = i;

    //defino un indice para mantener el indice
    //del ultimo caracter que no es una letra
    int lastLetterIdx = start;

    //Voy por todos los caracteres manteniendo el indice
    //del ultimo digito o letra (recordar que puedo recibir
    //numeros como parametros)
    while (str[i++] != NULL_)
        if (IS_LETTER(str[i]) || IS_DIGIT(str[i]))
            lastLetterIdx = i;

    //como ya lei un comando entero lo seteo, defino el final de mi string
    str[lastLetterIdx + 1] = '\0';

    return start;
}


static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void putInteger(unsigned int num, Window window) {
    /*char strInverse[10], numToStr[11];
    int strLen = 0;

    for (; num > 0; num /= 10, strLen++) {
        int unit = num % 10;
        strInverse[strLen] = unit + 0x30;
    }

    for (int i = strLen; i > 0; i--) {
        numToStr[strLen - i] = strInverse[i - 1];
    }

    numToStr[strLen] = '\0';

    puts_(numToStr, window);*/

    char numToStr[64];
    uintToBase(num, numToStr, 10);
    puts_(numToStr, window);

}

void putHex(int num, Window window){
    char numToStr[11];
    uintToBase(num, numToStr, 16);
    puts_(numToStr, window);
}

int isPrime(int num) {
    int i;

    for (i = 2; i <= num - 1; i++)
        if (num % i == 0)
            return 0;

    return i == num;
}

void newLine(Window window) {
    putc_(ENTER, window);
}

int strcmp_(const char *str1, const char *str2) {

    if (str1 == NULL_ || str2 == NULL_) return -1;//exit("Se compararon strings NULLS");

    const unsigned char *p1 = (const unsigned char *) str1;
    const unsigned char *p2 = (const unsigned char *) str2;

    while (*p1 && *p1 == *p2) ++p1, ++p2;

    return (*p1 > *p2) - (*p2 > *p1);
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


void getTime(char * buffer){
    sysTime(buffer);
}

void readMem(char * buffer, uint8_t * from, int cant){
    //sysReadMem(buffer, from, cant);
    sysReadMem(from, buffer, cant);
}


extern void getRegs(long * buffer);

void getRegsSnapshot(uint64_t * buffer){
    sysGetRegsSnapshot(buffer);
}