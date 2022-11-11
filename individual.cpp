#include "stdafx.h"
#include "individual.h"

Individual::Individual( ){
	loc.x = 0;
	loc.y = 0;
	alive = 0;
	adult = 0;
  age = 0;
  genetics = NULL;
	genetics = new Genome();
}

Individual::~Individual( ){
	if( genetics ) delete genetics;
	genetics = NULL;
}

void Individual::Move( coord a ){
	loc = a;
}

void Individual::Kill( ){
	alive = 0;
	genetics->Clear();
}

void Individual::Revive( ){
	if( !alive ){
		alive = 1;
    genetics->Clear();
    //genetics->Mutate(RAND_INT(100));
    genetics->Mutate(genetics->Poisson(LAMBDA));
  }
}

void Individual::Revive( coord a ){
	if( !alive ){
		loc = a;
		Revive();
	}
}

void Individual::Reproduce( Individual *a,Individual *b ) {
	genetics->Reproduce(a->GetGenome(),b->GetGenome());
  alive = 1;
  adult = 0;
}

int Individual::Select( void ) {
#ifdef FIXED_ADULT_RATE
  if( RAND(1.0) > ADULT_RATE ) return 1;
#endif
#ifdef ABSOLUT_SELECTION
#ifdef SQUARED_FIT
  _FAST double fit = BaseFitness();
  if( RAND(1.0) > (fit*fit) ) return 1;
#else
  if( RAND(1.0) > BaseFitness() ) return 1;
#endif
#endif
  return 0;
//#ifdef FIXED_ADULT_RATE
//  if( adult ) return RAND(1.0)<ADULT_RATE?1:0;
//#endif
//  if( RAND(1.0) < Fitness() ){
//    adult = 1;
//    return 1;
//  }
//return 0;
}

void Individual::Mutate( void ){
  genetics->Mutate(1);
}

void Individual::Mutate( cint num ){
  genetics->Mutate(num);
}

void Individual::SetGenome( Genome *_g ) {
  if( genetics ) delete genetics;
  genetics = NULL;
  genetics = _g;
  this->Mutate(0);
}

cint Individual::Age( void ){
  return ++age;
}

//Individual Individual::operator = (Individual a){
//  // This may not work correctly due to sharing
//  // the genome.
//	loc = a.GetCoord();
//	alive = (cint)a.Alive();
//  if( genetics ) delete genetics;
//  genetics = a.GetGenome();
//	return *this;
//}

//####################################################################

sint Individual::MoveRadius( void ) const {
  return MOVE_RADIUS;
}

sint Individual::ReproductionRadius( void ) const {
	return REPRODUCTION_RADIUS;
}

coord Individual::GetCoord( void ) const {
	return loc;
}

sint Individual::Alive( void ) const {
	return alive;
}

double Individual::Fitness( void ) const {
  return genetics->Fitness(); // + (AGE_FUNC(age) - 1.0);
}

double Individual::BaseFitness( void ) const {
  return genetics->BaseFitness(); // + (AGE_FUNC(age) - 1.0);
}

Genome *Individual::GetGenome( void ) const {
	return genetics; 
}

void Individual::Print( void ) const {
	if( alive ) cout << "O";
	else cout << "-";
}