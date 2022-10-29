// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/_string.h"

int atoi_(char *str) {
  int result;
  int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+')) {
    if (*str == '-')
      puiss = puiss * -1;
    str++;
  }
  while ((*str >= '0') && (*str <= '9')) {
    result = (result * 10) + ((*str) - '0');
    str++;
  }
  return (result * puiss);
}

int strlen_(char *str) {
  char *s;

  for (s = str; *s; ++s)
    ;
  return (int)(s - str);
}

int KMP(char *text, char *pattern, int textLen, int patternLen) {
  // base case 1: pattern is NULL or empty
  if (*pattern == '\0' || patternLen == 0)
    return 0;

  // base case 2: text is NULL, or text's length is less than that of pattern's
  if (*text == '\0' || patternLen > textLen)
    return -1;

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
      if (++j == patternLen)
        return i - j + 1;
    } else if (j > 0) {
      j = next[j];
      i--; // since `i` will be incremented in the next iteration
    }
  }

  return -1;
}

int substring(char *str, char *substr) {
  return KMP(str, substr, strlen_(str), strlen_(substr));
}

int removeTrailingSpaces(char *str) {

  // variable para recorrer toda la string
  int i = 0;

  // borro todos los caracteres (si o si mis comandos
  // comienzan con una letra o un &)
  while (str[i] != NULL_ && !IS_LETTER(str[i]) && str[i] != '&')
    i++;

  // almaceno el indice del primer caracter letra
  int start = i;

  // defino un indice para mantener el indice
  // del ultimo caracter que no es una letra
  int lastLetterIdx = start;

  // Voy por todos los caracteres manteniendo el indice
  // del ultimo digito o letra (recordar que puedo recibir
  // numeros como parametros)
  while (str[i++] != NULL_)
    if (IS_LETTER(str[i]) || IS_DIGIT(str[i]))
      lastLetterIdx = i;

  // como ya lei un comando entero lo seteo, defino el final de mi string
  str[lastLetterIdx + 1] = '\0';

  return start;
}

int strcmp_(char *str1, char *str2) {

  if (str1 == NULL_ || str2 == NULL_)
    return -1; // exit("Se compararon strings NULLS");

  unsigned char *p1 = (unsigned char *)str1;
  unsigned char *p2 = (unsigned char *)str2;

  while (*p1 && *p1 == *p2)
    ++p1, ++p2;

  return (*p1 > *p2) - (*p2 > *p1);
}
unsigned int atoh_(char *str) {
  unsigned int result;
  unsigned int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+')) {
    if (*str == '-')
      puiss = puiss * -1;
    str++;
  }
  while (((*str >= '0') && (*str <= '9')) || ((*str) >= 'a' && (*str) <= 'f') ||
         ((*str) >= 'A' && (*str) <= 'F')) {
    switch (*str) {
    case 'a':
      result = (result * 16) + 10;
      break;
    case 'A':
      result = (result * 16) + 10;
      break;
    case 'b':
      result = (result * 16) + 11;
      break;
    case 'B':
      result = (result * 16) + 11;
      break;
    case 'c':
      result = (result * 16) + 12;
      break;
    case 'C':
      result = (result * 16) + 12;
      break;
    case 'd':
      result = (result * 16) + 13;
      break;
    case 'D':
      result = (result * 16) + 13;
      break;
    case 'e':
      result = (result * 16) + 14;
      break;
    case 'E':
      result = (result * 16) + 14;
      break;
    case 'f':
      result = (result * 16) + 15;
      break;
    case 'F':
      result = (result * 16) + 15;
      break;

    default:
      result = (result * 16) + ((*str) - '0');
      break;
    }
    str++;
  }
  return (unsigned int)(result * puiss);
}
