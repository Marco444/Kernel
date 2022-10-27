// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

static char buffer[64] = {'0'};

// static uint8_t *const video = (uint8_t *)0xB8000;
// static uint8_t *currentVideoFD0 = (uint8_t *)0xB8000;
// static uint8_t *currentVideoFD1 = (uint8_t *)0xB8000;
// static uint8_t *currentVideoFD2 = (uint8_t *)0xB8000 + 82;

static uint8_t *const videoBegin = (uint8_t *)0xB8000;
static uint8_t *const video = (uint8_t *)0xB8000 + COLUMS;
static uint8_t *currentVideoFD0 = (uint8_t *)0xB8000 + COLUMS;
static uint8_t *currentVideoFD1 = (uint8_t *)0xB8000 + COLUMS;
static uint8_t *currentVideoFD2 = (uint8_t *)0xB8000 + COLUMS + 82;

static int actualFd = 0;

static const uint32_t width = 80;
void resetVideoFD1();
static const uint32_t height = 25;

///////////////////////////////////////////////////
// Aux declaration //
/////////////////////////////////////////////////
void ncPrintChar_Format(char character, int fd, int format);
void resetVideoFD1();
void resetVideoFD2();
void ncClearFD1();
void ncClearFD2();

void ncPrint(const char *string) {
  int i;

  for (i = 0; string[i] != 0; i++)
    ncPrintChar(string[i], 0);
}

void ncPrintAtFD(const char *string, int fd) {
  int i;

  for (i = 0; string[i] != 0; i++)
    ncPrintChar(string[i], fd);
}

void ncPrintAtFD_Format(const char *string, int fd, int format) {
  int i;

  for (i = 0; string[i] != 0; i++)
    ncPrintChar_Format(string[i], fd, format);
}

void ncPrintChar(char character, int fd) {

  switch (fd) {
  case 0:
    ncPrintFD0Char(character);
    break;

  case 1:
    ncPrintFD1Char(character);
    break;

  case 2:
    ncPrintFD2Char(character);
    break;

  default:
    ncPrintFD0Char(character);
    break;
  }
}

void ncPrintChar_Format(char character, int fd, int format) {

  switch (fd) {
  case 0:
    ncPrintFD0Char_Format(character, format);
    break;

  case 1:
    ncPrintFD1Char_Format(character, format);
    break;

  case 2:
    ncPrintFD2Char_Format(character, format);
    break;

  default:
    ncPrintFD0Char_Format(character, format);
    break;
  }
}

void open(int fd) {
  ncClear();
  actualFd = fd;
}

void close(int fd) {
  actualFd = (fd) ? 0 : 1;
  ncClear();
}

// -------------------------------------------------------------------------------
// FD0
// -------------------------------------------------------------------------------
void ncPrintFD0(char *string) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD0Char_Format(string[i], WHITE);
}

void ncPrintFD0_Format(char *string, char format) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD0Char_Format(string[i], format);
}

void ncPrintFD0Char(char character) { ncPrintFD0Char_Format(character, WHITE); }

void ncPrintFD0Char_Format(char character, char format) {
  if (character == '\n') {
    currentVideoFD0 += COLUMS - ((currentVideoFD0 - video) % COLUMS);
  } else if (character == '\b') {
    if (currentVideoFD0 != video)
      currentVideoFD0 -= 2;
    *currentVideoFD0 = ' ';
    *(currentVideoFD0 + 1) = WHITE;
  } else {
    *currentVideoFD0 = character;
    *(currentVideoFD0 + 1) = format;
    currentVideoFD0 += 2;
  }

  if ((currentVideoFD0 - video) >= (4000 - COLUMS)) {
    resetVideo(0);
  }
}

// -------------------------------------------------------------------------------
// FD1
// -------------------------------------------------------------------------------
void ncPrintFD1(char *string) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD1Char_Format(string[i], WHITE);
}

void ncPrintFD1_Format(char *string, char format) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD1Char_Format(string[i], format);
}

void ncPrintFD1Char(char character) { ncPrintFD1Char_Format(character, WHITE); }

void ncPrintFD1Char_Format(char character, char format) {
  if (character == '\n') {
    currentVideoFD1 += OFFSET - ((currentVideoFD1 - video) % OFFSET);
  } else if (character == '\b') {
    if (currentVideoFD1 != video)
      currentVideoFD1 -= 2;
    *currentVideoFD1 = ' ';
    *(currentVideoFD1 + 1) = WHITE;
  } else {
    *currentVideoFD1 = character;
    *(currentVideoFD1 + 1) = format;
    currentVideoFD1 += 2;
  }

  if (!((currentVideoFD1 - video) % OFFSET)) {
    currentVideoFD1 += OFFSET;
  }

  if ((currentVideoFD1 - video) >= (3920 - COLUMS)) {
    resetVideoFD1();
  }
}

// -------------------------------------------------------------------------------
// FD2
// -------------------------------------------------------------------------------
void ncPrintFD2(char *string) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD2Char_Format(string[i], WHITE);
}

void ncPrintFD2_Format(char *string, char format) {
  int i;
  for (i = 0; string[i] != 0; i++)
    ncPrintFD2Char_Format(string[i], format);
}

void ncPrintFD2Char(char character) { ncPrintFD2Char_Format(character, WHITE); }

void ncPrintFD2Char_Format(char character, char format) {
  if (character == '\n') {
    currentVideoFD2 += OFFSET - ((currentVideoFD2 - video) % OFFSET);
  } else {
    *currentVideoFD2 = character;
    *(currentVideoFD2 + 1) = format;
    currentVideoFD2 += 2;
  }
  if (!((currentVideoFD2 - video) % OFFSET)) {
    currentVideoFD2 += OFFSET + 2;
  }

  if ((currentVideoFD2 - video) >= (4000 - COLUMS)) {
    resetVideoFD2();
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
void resetVideo(int fd) {
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

void resetVideoFD1() {

  // Leer una linea y escribirla un renglon arriba
  for (int i = 0; i < (((height - 1) * width * 2) - COLUMS); i++) {
    if (i % 160 < OFFSET)
      *(video + i) = *(video + i + COLUMS);
  }

  // Limpio la ultima linea que sera donde comienza a escribir
  currentVideoFD1 = videoBegin + ((height - 1) * width * 2);
  // Limpio la ultima linea que sera donde comienza a escribir
  for (int i = 0; i < OFFSET; i += 2) {
    *(currentVideoFD1 + i) = ' ';
    *(currentVideoFD1 + i + 1) = WHITE;
  }
}

void resetVideoFD2() {
  // Leer una linea y escribirla un renglon arriba
  for (int i = 0; i < (3840 - COLUMS); i++) {
    if (i % 160 >= OFFSET)
      *(video + i) = *(video + i + COLUMS);
  }

  currentVideoFD2 = videoBegin + ((height - 1) * width * 2) + OFFSET + 2;
  // Limpio la ultima linea que sera donde comienza a escribir
  for (int i = 0; i < OFFSET; i += 2) {
    *(currentVideoFD2 + i) = ' ';
    *(currentVideoFD2 + i + 1) = WHITE;
  }
}

// -------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------
void ncNewline(int FD) {
  do {
    ncPrintChar(' ', 0);
  } while ((uint64_t)(currentVideoFD0 - video) % (width * 2) != 0);
}

// -------------------------------------------------------------------------------
// 	NUMEROS
// -------------------------------------------------------------------------------
void ncPrintDec(uint64_t value) { ncPrintBase(value, 10); }

void ncPrintDecAtFD(uint64_t value, int fd) {
  uintToBase(value, buffer, 10);
  switch (fd) {
  case 0:
    ncPrintFD0(buffer);
    break;
  case 1:
    ncPrintFD1(buffer);
    break;
  case 2:
    ncPrintFD2(buffer);
    break;
  default:
    break;
  }
}

void ncPrintHex(uint64_t value) { ncPrintBase(value, 16); }

void ncPrintHexAtFD(uint64_t value, int fd) {
  uintToBase(value, buffer, 16);
  switch (fd) {
  case 0:
    ncPrintFD0(buffer);
    break;
  case 1:
    ncPrintFD1(buffer);
    break;
  case 2:
    ncPrintFD2(buffer);
    break;
  default:
    break;
  }
}

void ncPrintBin(uint64_t value) { ncPrintBase(value, 2); }

void ncPrintBinAtFD(uint64_t value, int fd) {
  uintToBase(value, buffer, 2);
  switch (fd) {
  case 0:
    ncPrintFD0(buffer);
    break;
  case 1:
    ncPrintFD1(buffer);
    break;
  case 2:
    ncPrintFD2(buffer);
    break;
  default:
    break;
  }
}

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
  currentVideoFD1 = video;
  currentVideoFD2 = video + 82;
}

void fdClear(int fd) {
  if (fd == 0) {
    ncClear();
    currentVideoFD0 = video;
  } else if (fd == 1) {
    ncClearFD1();
    currentVideoFD1 = video;
  } else {
    ncClearFD2();
    currentVideoFD2 = video + 82;
  }
}

void ncClearFD1() {
  for (int i = 0; i < (height * 2) * (width * 2); i += 2) {
    if (i % 160 < OFFSET) {
      video[i] = ' ';
      video[i + 1] = BLACK_BACKGROUND;
    }
  }
}

void ncClearFD2() {
  for (int i = 0; i < (height * 2) * (width * 2); i += 2) {
    if (i % 160 >= OFFSET) {
      video[i] = ' ';
      video[i + 1] = BLACK_BACKGROUND;
    }
  }
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
