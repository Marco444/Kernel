#ifndef PHYLO_H
#define PHYLO_H

//#include "include/constants.h"

#define MAX_PHYLOS 10
#define MIN_PHYLOS 5

#define MAX_FORKS MAX_PHYLOS
#define MIN_FORKS MIN_PHYLOS

#define EATING 1
#define THINKING 2
#define HUNGRY 3
#define LEFT 4
#define RIGHT 5
#define BOTH 6

#define ADD 'a'
#define REMOVE 'r'
#define QUIT 'q'

void initializePhylos(int argc, char argv[20][20]);
void philosopher(int argc, char argv[20][20]);

#endif