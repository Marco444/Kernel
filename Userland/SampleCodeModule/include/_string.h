#ifndef STRING_H
#define STRING_H
#define IS_DIGIT(c)  ((c) >= '0' && (c)<='9')
#define IS_LETTER(c) (((c) >= 'a' && (c)<='z') || ((c) >= 'A' && (c)<='Z'))

#define NULL_ 0L
#define BACKSPACE '\b'
#define ENTER '\n'
#define TAB '\t'
#define SPACE ' '
#define PIPE '|'
#define DOUBLE_TAB "     "

/*
 * Devuelve la string ingresada como un entero
 */
int atoi_(char * str);

/*
 * Devuelve el tamanño de una string
 */
int strlen_(char * s);

/*
 * Devuelve el indice donde substr es una subtring de str, -1 sino existe
 */
int substring(char *str, char *substr);

/*
 * Recibe un texto donde busca el primer indice donde aparece la string patron
 * como una substring. 
 */
int KMP(char *text, char *pattern, int textLen, int patternLen);

/*
 * Recibe dos strings y las compara
 */
int strcmp_(char * str1, char * str2);

/*
 * Recibe una string y devuelve el indice de la primer letra y
 * despues de la ultima letra. OJO QUE TIENE QUE PODER MODIFICAR A LA STRING
 * NO PUEDE SER UNA STRING LITERAL
 */
int removeTrailingSpaces(char * str);
unsigned int atoh_(char * str);
#endif
