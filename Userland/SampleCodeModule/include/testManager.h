#include <string.h>

#include "_stdlib.h"
#include "_string.h"
#include "constants.h"
#include "stdio.h"
#include "testUtil.h"

/* Aloca, rellena, verifica y libera memoria dinamica constantemente */
void testMM(int iterations, int maxMemory);

/* Crea, bloquea y mata procesos constantemente */
void testProcesses(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

/* Administra los tests, recibe como argumento el nombre del test junto
 * con los argumentos correspondientes */
void testManagerRun(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

/* Crea procesos que incrementan y decrementan una variable global, con y sin
 * uso de semaforos (para asi verificar el funcionamiento de los mismos) */
void testSync(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

/* Creo procesos y modifica su prioridad con el comando nice para asi
 * verificar visualmente el funcionamiento de las prioridades del scheduler */
void testPrio();
