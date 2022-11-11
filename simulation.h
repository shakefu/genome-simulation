#include "defs.h"
#include "grid.h"
#include <fstream>
#include <iomanip>
#include <sstream>

#ifndef _SIMULATION_H
#define _SIMULATION_H

typedef struct s_stats {
  sint year;
  sint t_selected;
  sint t_unmoved;
  sint t_surviving;
  sint t_fammove;
  sint y_selected;
  sint y_unmoved;
  sint y_surviving;
  sint y_fammove;
  sint y_remaining;
  sint y_popsize;
  double avg_fitness;
  double y_fitshort;
  double y_fitlong;
  sint y_short;
  sint y_long;
  double y_longdist;
} sStats;

class Simulation {
public:
  Simulation( );
  ~Simulation( );

  void Year( void );  // Carries out everything needing to be done in a year
  void NaturalSelection( void ); // Kills individuals not strong enough
  void EverybodyReproduce( void ); // Causes reproduction
  Grid *GetGrid( void ); // Returns the grid of the simulation
  sint RelativeFAM( double x ); // Ditto

  string GetFName( void ) const;
  void Print( void ) const; // Prints out the grid.

#ifdef OUTPUT_STATS
  void OutputStats( void );
  void OutputData( void );
#endif

//# No longer in use #
//  void EverybodyMove( void ); // Everybody moves
//  sint RelativeFitness( double x,double cmp ); // Tricky relative thing
//  sint RelativeSelection( double x ); // Gyar, relative fitness kludge


  //# Statistics #
  sStats stats;
  sStats oldStats;
  sint rYear;
  // Jury-rig
  sint OUTPUT_FREQUENCY;
private:
  //ofstream output;
  Grid *g;
  sint statTag;
  double fitThresh;
  //double ffitThresh;
  //sint relYear;
  //sint relCount;
  //double *relFitness;
  //iList offspring;
};

#endif//_SIMULATION_H