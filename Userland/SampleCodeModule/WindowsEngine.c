// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/WindowsEngine.h"
#include "include/Windows.h"
#include "include/argumentsEngine.h"
#include "include/lib.h"

void windowsEngineInitialize(Window window, int argc,
                             char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  // Se inicia mi primera ventan
  windowStart(MAIN_WINDOW);
}

void windowsEngineDisplayControls(Window window) {

  // obs. se podria modalurisar con los define pero no nos dio el tiempo

  /*putsf_(" Sistema de ventanas:                                      \n"
         "   - Suspender ventana principal o izquierda: PRESIONAR 1  \n"
         "   - Reanudar  ventana principal o izquierda: PRESIONAR 3  \n"
         "   - Suspender ventana derecha :              PRESIONAR 2  \n"
         "   - Reanudar  vventana derecha:              PRESIONAR 4  \n"
         "   - Para matar cualquier proceso:            ENTER        "
  , MAGENTA_BACKGROUND | WHITE, window);*/

  newLine(window);
  putsf_("    ", WHITE, window);
  putsf_(" Sistema de ventanas:                                      \n",
         LIGHT_RED_BACKGROUND | WHITE, window);
  putsf_("    ", WHITE, window);
  putsf_("   - Suspender ventana principal o izquierda: PRESIONAR 1  \n",
         LIGHT_RED_BACKGROUND | WHITE, window);
  putsf_("    ", WHITE, window);
  putsf_("   - Reanudar  ventana principal o izquierda: PRESIONAR 3  \n",
         LIGHT_RED_BACKGROUND | WHITE, window);
  putsf_("    ", WHITE, window);
  putsf_("   - Suspender ventana derecha :              PRESIONAR 2  \n",
         LIGHT_RED_BACKGROUND | WHITE, window);
  putsf_("    ", WHITE, window);
  putsf_("   - Reanudar  vventana derecha:              PRESIONAR 4  \n",
         LIGHT_RED_BACKGROUND | WHITE, window);
  putsf_("    ", WHITE, window);
  putsf_("   - Para matar cualquier proceso:            ENTER        ",
         LIGHT_RED_BACKGROUND | WHITE, window);
}

void waitProcess() {

  char c;

  getKey(MAIN_WINDOW, &c);

  while (c != '\n') {

    if (c == LEFT_WINDOW_SUSPEND)
      sysKillProcess(LEFT_WINDOW);
    else if (c == RIGHT_WINDOW_SUSPEND)
      sysKillProcess(RIGHT_WINDOW);
    ;
    if (c == RESUME_LEFT_WINDOW)
      sysReloadProcess(LEFT_WINDOW);
    if (c == RESUME_RIGHT_WINDOW)
      sysReloadProcess(RIGHT_WINDOW);

    getKey(MAIN_WINDOW, &c);
  }

  exit(LEFT_WINDOW);
  exit(RIGHT_WINDOW);
  sysClearScreen(LEFT_WINDOW);
  sysClearScreen(RIGHT_WINDOW);
}

void waitProcessPipe() {

  printHeader("1: Pause left  2: Pause right   3: Restart left   4: Restart "
              "right   ENTER: exit",
              GREEN_BACKGROUND | WHITE);

  char c;
  // leo la key
  getKey(MAIN_WINDOW, &c);

  while (c != '\n') {

    if (c == LEFT_WINDOW_SUSPEND)
      sysPauseProcess(LEFT_WINDOW);
    else if (c == '2')
      sysPauseProcess(RIGHT_WINDOW);
    ;
    if (c == '3')
      sysReloadProcess(LEFT_WINDOW);
    if (c == '4')
      sysReloadProcess(RIGHT_WINDOW);

    getKey(MAIN_WINDOW, &c);
  }
  sysKillProcess(LEFT_WINDOW);
  sysKillProcess(RIGHT_WINDOW);
  sysClearScreen(LEFT_WINDOW);
  sysClearScreen(RIGHT_WINDOW);
}

void waitProcessMain(int pid) {

  char c;
  // leo la key
  getKey(MAIN_WINDOW, &c);

  while ((SysProcesses() > 1)) {
    printHeader(" ENTER: exit   1: Pause   3: Restart",
                GREEN_BACKGROUND | WHITE);
    if (c == MAIN_WINDOW_SUSPEND)
      sysPauseProcess(LEFT_WINDOW);
    if (c == '3')
      sysReloadProcess(LEFT_WINDOW);

    getKey(MAIN_WINDOW, &c);
  }
  sysKillProcess(pid);
  // sysClearScreen(MAIN_WINDOW);
}
