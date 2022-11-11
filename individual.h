#include "defs.h"
#include "genome.h"

#ifndef _INDIVIDUAL_H
#define _INDIVIDUAL_H

class Individual {
public:
  Individual( );
  ~Individual( );

  void Move( coord a ); // Moves to a (do not call directly, use grid)
  void Kill( void );    // Kills the individual
  void Revive( void );  // Revives the individual
  void Revive( coord a ); // Revives at a new location (don't call directly)
  void Reproduce( Individual *a,Individual *b ); // Turns two individuals' data into one
  int Select( void );   // Returns 0 if it should live, otherwise should be killed.
  void Mutate( void );  // Mutates the Ind.'s genome
  void Mutate( cint num ); // Mutates num times
  void SetGenome( Genome *_g ); // Sets the individual's genome.
  cint Age( void ); // Ages the individual.

//  Individual operator = ( Individual a ); // Copy func.

  sint MoveRadius( void ) const; // Returns this individual's move radius
  sint ReproductionRadius( void ) const; // Returns this reproduction radius
  coord GetCoord( void ) const; // Current location
  sint Alive( void ) const; // Alive or dead (1 = live)
  double Fitness( void ) const; // UNUSED Fitness 0.0-1.0
  double BaseFitness( void ) const; // Different fit function
  Genome *GetGenome( void ) const; // Returns genetics

  void Print( void ) const; // Prints O or -

private:
  coord loc;
  cint alive;
  cint adult;
  cint age;
  Genome *genetics;
};

// Typedef for lists... yey!
typedef struct s_ilist {
  Individual **list;
  sint count;
} iList;

/* Not used... *
typedef struct s_dilist {
//  s_dilist *prev;
  s_dilist *next;
  Individual *cur;
} ditem;

class LinkediList {
public:
  LinkediList( ){
    head = NULL;
    tail = NULL;
    length = 0;
  };
  LinkediList( iList &_i ){
    head = NULL;
    tail = NULL;
    length = 0;
    for( sint i=0;i<_i.count;i++ ){
      push(_i.list[i]);
    }
  };
  ~LinkediList( ){
    ditem *_o;
    if( head ){
      do {
        _o = head->next;
        delete head;
        head = _o;
      } while( _o );
      tail = NULL;
    }
  };

  void push( Individual *_i ){
    if( !head ){
      head = new ditem;
 //     head->prev = NULL;
      head->next = NULL;
      tail = head;
      tail->cur = _i;
    }
    else {
      tail->next = new ditem;
//      tail->next->prev = tail;
      tail = tail->next;
      tail->cur = _i;
    }
    length++;
  };

  Individual *pop( void ){
    Individual *_i;
    ditem *_o;
    if( !head ) return NULL;
    else if( head == tail ){
      _i = head->cur;
      delete head;
      head = NULL;
      delete tail;
      tail = NULL;
      length--;
      return _i;
    }
    else {
      _i = head->cur;
      _o = head->next;
      delete head;
      head = _o;
//      head->prev = NULL;
      length--;
      return _i;
    }
  };

private:
  ditem *head;
  ditem *tail;
  sint length;
};
*/

#endif//_INDIVIDUAL_H