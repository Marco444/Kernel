#ifndef PHYLO_H
#define PHYLO_H

#define MAX_PHYLOS 10
#define MIN_PHYLOS 5

#define MAX_FORKS MAX_PHYLOS
#define MIN_FORKS MIN_PHYLOS

#define EATING 1
#define THINKING 2

#define ADD 'a'
#define REMOVE 'r'
#define QUIT 'q'

void initializePhylos(int argc, char * argv[]);
void philosopher(int argc, char argv[20][20]);

#endif