#include "defs.h"
#include "individual.h"

#ifndef _GRID_H
#define _GRID_H

class Grid {
public:
  Grid( );
  ~Grid( );

  void Initialize( void );  // Initializes the grid
  void Move( Individual *s,Individual *d ); // Moves an individual (swaps)
  void Kill( Individual *v ); // Kills individual
  void Kill( coord c );       // Kills individual at c
  void Revive( Individual *v ); // Revives individual
  void Revive( coord c );     // Revives individual at c

  sint GetPopMax( ) const;  // Maximum allowable population
  sint GetPopSize( ) const; // Current population
  Individual *GetIndividual( coord c ) const; // Returns individual at c
  iList GetIndividualList( ) const; // All individuals in the grid
  iList GetOpenList( ) const; // All dead individuals in the grid (open spots)
  iList GetShuffledIndividualList( ) const; // Same but shuffled
  iList GetShuffledOpenList( ) const; // Same but shuffled
  Individual *GetNeighbor( Individual *v,sint r ) const; // Finds a random neighbor within radius r
  Individual *GetNeighbor( coord c,sint r ) const; // Finds a neighbor around c, radius r
  Individual *GetOpenNeighbor( Individual *v,sint r ) const; // Open spot within radius r
  Individual *GetOpenNeighbor( coord c,sint r ) const; // opening within radius r of c

  void PrintGrid( ) const;  // Prints out the grid
  double AverageFitness( ) const; // Returns the average fitness of the population
  double AverageBaseFitness( ) const; // Different fit formula

  // Unused methods
  //  void SetIndividual( Individual *a,Individual *b ); // Swaps pointers
  //  iList GetOpenNeighbors( Individual *v,sint r ) const; // Dead neighbors within radius r
  //  iList GetNeighbors( Individual *v,sint r ) const; // Neighbors within r
  //  iList GetOpenNeighbors( coord c,sint r ) const; // Dead neighbors around c, radius r
  //  iList GetNeighbors( coord c,sint r ) const; // Neighbors around c, radius r

private:
  void Swap( coord a,coord b ); // Swaps individuals at a and b

  sint popMax;
  sint popSize;
  Individual ***map;
};

#endif//_GRID_H