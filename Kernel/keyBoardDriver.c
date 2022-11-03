// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <constKeyboar.h>
#include <keyBoard.h>
#include <naiveConsole.h>
#include <stdint.h>

#include "include/naiveConsole.h"
#include "include/semaphores.h"
#define NULL_ 0l
#define MAX_BUFFER 255
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36

static char buffer[MAX_BUFFER];
static unsigned int size = 0;
static unsigned int actualPos = 0;
static char *keyMap[] = {scancodeLToAscii, scancodeUToAscii};
static char keyMapRow = 0;

Semaphore semStdin;

void initKeyboard() { semStdin = semOpen(getNextAvailableSemaphore()); }

void saveBuffer(char code) {
  // ncPrint("presionaste tecla");
  semSignal(semStdin);

  if (code < 0x80 && code > 0) {  // Key pressed
    if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
      keyMapRow |= 0x01;
    } else if (keyMap[(int)keyMapRow][(int)code] != 0) {
      buffer[size] = keyMap[(int)keyMapRow][(int)code];
      size = (size == 254) ? 0 : size + 1;
    }
  } else {
    // Key released
    code -= 0x80;
    if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
      keyMapRow = 0;
    }
  }
}

void getBufferChar(char *sysBuffer) {
  // semWait(semStdin);

  // copio el valor actual del buffer a mi
  // variable de salida
  sysBuffer[0] = buffer[actualPos];

  // chequeo si no la habia leido, si es asi la marco como
  // leida y
  if (buffer[actualPos] != 0) buffer[actualPos] = NULL_;

  // actualPos = (actualPos + 1) % MAX_BUFFER;
  if (actualPos == MAX_BUFFER - 1)
    actualPos = 0;
  else
    actualPos++;
}
char getValue(int c) { return scancodeLToAscii[c]; }
