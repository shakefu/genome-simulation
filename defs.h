#include "bitset.h"

#ifndef _DEFS_H
#define _DEFS_H
/* Behaviour options */
#define FIXED_ADULT_RATE // RAND(1.0) > ADULT_RATE kills
#undef FAM_DIFF  // diff. distances for 1 & 2 alleles
#define ABSOLUT_SELECTION // RAND(1.0) < BaseFitnesss kills
  #ifdef ABSOLUT_SELECTION
    #undef SQUARED_FIT // RAND(1.0) < BaseFitness^2 kills
  #endif
#define OUTPUT_STATS
  #ifdef OUTPUT_STATS
    #define OUTPUT_PRE "Hadany_"
    #define OUTPUT_POST ".txt"
    #define OUTPUT_FREQ 1 // Years between outputs
  #endif
#undef OPENING_MOVEMENT

#define TEST1 // Changes reproduction mem alloc

/* Simulation definitions */
#define YEARS_RUN 10000
#define FAM_DELAY 0

/* Definitions for grid structures */
#define GRID_WIDTH  100
#define GRID_HEIGHT 100
#define MAX_POP (GRID_WIDTH*GRID_HEIGHT)

/* Definitions for Individuals */
/* Fitness numbers */
#define ADULT_RATE        0.95  // Adult survival rate
#define MAX_OPENINGS      0.01  // Percent of grid that may be left open
#define MAX_OFFSPRING     MAX_POP*4 // Max number of offspring to try and produce
#define MUTATION_EFF      0.04   // Effect per double deletion
#define FIT_SINGLE        0.05  // multiplier for num. of single del.
#define FAM_THRESH        0.10  // Threshold for FAM
#define LAMBDA            1.1   // Used in Poisson dist.
#define RELFIT_SAMPLE     250   // UNUSED Number of individuals to sample
#define AGE_EFF           0.00  // UNUSED Effect per year alive
#define STARTING_FITNESS  0.95  // UNUSED Starting fitness
#define MUTATION_RATE     0.50  // UNUSED Percent chance to mutate

/* Distances - Must be less than (GRID/2)-1 (height or width, 
 * whichever is the least.) Otherwise it causes wrapping 
 * problems. */
#define MOVE_RADIUS       1     // Uniform radius
#define FAM_RADIUS        (GRID_WIDTH/2-1)     // Move radius for FAM
#define REPRODUCTION_RADIUS   2 // half height/width - 1


/* Definitions for chromosome structures */
#define STARTING_FAM_RATE 0.00 // Percent chance for FAM allele
#define C_LENGTH          1000  // Length of chr.
#define C_COUNT           2     // Number of chr.
#define C_FAM_CHR         1     // UNUSED
#define C_FAM_AL          128   // UNUSED


/* Macros */
#define PAUSE fflush(stdin); printf("\nPress Enter..."); getchar();
#define RAND(x)	((double)rand()/RAND_MAX*(x))
#define RAND_INT(x) (rand()%(x+1))
#define _FAST   register
#define _GRTH(x,y) (x>y?y:x)
#define BASE_FIT_FUNC(x) (\
  pow(1.0 - MUTATION_EFF,(double)x) )

/* Typedef's */
typedef unsigned long sint;
typedef unsigned short cint;
typedef bitset<C_LENGTH> bset;

typedef struct s_coord {
  sint x;
  sint y;
} coord;

/* Obsolete/Unused */
#define FITNESS_FUNC(x) (\
  pow(STARTING_FITNESS - MUTATION_EFF,(double)x) )

#define AGE_FUNC(x) (\
  pow(1.0 - AGE_EFF,(double)x) )

#define NORZ(x,c0,c1,m) (\
  1.0/c1*(pow(2.71828182846,(-pow((x-m),2.0)/c0)) ) )


#endif//_DEFS_H