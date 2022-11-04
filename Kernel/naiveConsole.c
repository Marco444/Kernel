// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/naiveConsole.h"

#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

static char buffer[64] = {'0'};

static uint8_t *const videoBegin = (uint8_t *)0xB8000;
static uint8_t *const video = (uint8_t *)0xB8000 + COLUMS;
static uint8_t *currentVideoFD0 = (uint8_t *)0xB8000 + COLUMS;

static const uint32_t width = 80;
static const uint32_t height = 25;

void ncPrintFormat(const char *string, char format) {
  for (int i = 0; string[i] != 0; i++) ncPrintCharFormat(string[i], format);
}

void ncPrint(const char *string) {
  for (int i = 0; string[i] != 0; i++) ncPrintCharFormat(string[i], WHITE);
}

void ncPrintChar(char ch) { ncPrintCharFormat(ch, WHITE); }

void ncPrintCharFormat(char character, char format) {
  if (character == '\n') {
    currentVideoFD0 += COLUMS - ((currentVideoFD0 - video) % COLUMS);
  } else if (character == '\b') {
    if (currentVideoFD0 != video) currentVideoFD0 -= 2;
    *currentVideoFD0 = ' ';
    *(currentVideoFD0 + 1) = WHITE;
  } else {
    *currentVideoFD0 = character;
    *(currentVideoFD0 + 1) = format;
    currentVideoFD0 += 2;
  }

  if ((currentVideoFD0 - video) >= (4000 - COLUMS)) {
    resetVideo();
  }
}

// -------------------------------------------------------------------------------
// 	Header
// -------------------------------------------------------------------------------

void ncPrintHeader(char *string, int format) {
  int i = 0;
  for (; i < 160 && string[i / 2] != 0; i += 2) {
    videoBegin[i] = string[i / 2];
    videoBegin[i + 1] = format;
  }

  for (; i < 160; i += 2) {
    videoBegin[i] = ' ';
    videoBegin[i + 1] = format;
  }
}

void ncClearHeader() {
  for (int i = 0; i < 160; i += 2) {
    videoBegin[i] = ' ';
    videoBegin[i + 1] = WHITE;
  }
}

// -------------------------------------------------------------------------------
// 	reset video
// -------------------------------------------------------------------------------
void resetVideo() {
  // Leer una linea y escribirla un renglon arriba
  for (int i = 0; i < (((height - 1) * width * 2) - COLUMS); i++) {
    *(video + i) = *(video + i + COLUMS);
  }

  // Limpio la ultima linea que sera donde comienza a escribir
  for (int i = 0; i < OFFSET; i++) {
    *(video + (3840 - COLUMS) + i * 2) = ' ';
    *(video + (3840 - COLUMS) + i * 2 + 1) = BLACK_BACKGROUND | WHITE;
  }
  currentVideoFD0 = currentVideoFD0 - COLUMS;
}

// -------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------
void ncNewline() {
  do {
    ncPrintChar(' ');
  } while ((uint64_t)(currentVideoFD0 - video) % (width * 2) != 0);
}

// -------------------------------------------------------------------------------
// 	NUMEROS
// -------------------------------------------------------------------------------
void ncPrintDec(int value) {
  if (value < 0) {
    ncPrint("-");
    value = value * (-1);
  }

  ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value) { ncPrintBase(value, 16); }

void ncPrintBin(uint64_t value) { ncPrintBase(value, 2); }

void ncPrintBase(uint64_t value, uint32_t base) {
  uintToBase(value, buffer, base);
  ncPrint(buffer);
}

//--------------------------------------------------------------------------
//	Clear
//--------------------------------------------------------------------------
void ncClear() {
  int i;

  for (i = 0; i < (height * 2) * (width * 2); i += 2) {
    video[i] = ' ';
    video[i + 1] = BLACK_BACKGROUND;
  }
  currentVideoFD0 = video;
}

void fdClear(int fd) {
  ncClear();
  currentVideoFD0 = video;
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}
