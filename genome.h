#include "defs.h"
#include <set>

#ifndef _GENOME_H
#define _GENOME_H

class Genome {
public:
  Genome( );
  ~Genome( );

  void Reproduce( const Genome *l,const Genome *r ); // Turns two genomes into one
  void Mutate( void );      // Mutates once
  void Mutate( cint num );  // Mutates num times
  void Clear( void );       // Clears all mutations

  cint GetMoveAllele( void ) const; // Randomly picks a move allele
  cint GetMoveAlleles( void ) const;
  void GetChromosome( cint num,bset &tmp ) const;
  cint GetMutationCount( void ) const;  // Total number of mutations in genome
  double Fitness( void ) const; // UNUSED Returns fitness
  double BaseFitness( void ) const; // Returns different fit formula
  unsigned short Poisson( double lambda );
  void Print( void ) const; // Prints out genome stats
  void PrintASCII( void ) const; // Prints out an ascii representation of the genome
private:
  bset *left;
  bset *right;
  cint doubleCount; // Two deleted alleles
  cint singleCount; // One deleted allele
  double fitness;
  double baseFitness;
  cint famAllele;
};

#endif//_GENOME_H