#include "stdafx.h"
#include "genome.h"

Genome::Genome( ){
  left = NULL;
  right = NULL;
  left = new bset[C_COUNT];
  right = new bset[C_COUNT];
  doubleCount = 0;
  singleCount = 0;
  famAllele = 0;
  if( RAND(1.0) < STARTING_FAM_RATE ) famAllele++;
  if( RAND(1.0) < STARTING_FAM_RATE ) famAllele++;
  Clear();
}

Genome::~Genome( ){
  delete [] left;
  delete [] right;

  left = NULL;
  right = NULL;
}

void Genome::GetChromosome( cint num,bset &tmp ) const {
  _FAST cint c,e;

  c = C_LENGTH - 2;
  c = RAND_INT(c) + 1;
  e = C_LENGTH - c;
  if( RAND_INT(1) )
    tmp = (left[num]<<c>>c) | (right[num]>>e<<e);
  else
    tmp = (right[num]<<c>>c) | (left[num]>>e<<e);
}

void Genome::Reproduce( const Genome *l,const Genome *r ) {
  _FAST cint i;
  for( i=0;i<C_COUNT;i++ ){
    if( RAND_INT(1) ){
      l->GetChromosome(i,left[i]);
      r->GetChromosome(i,right[i]);
    }
    else {
      r->GetChromosome(i,left[i]);
      l->GetChromosome(i,right[i]);
    }
  }
  Mutate(Poisson(LAMBDA));
  famAllele = l->GetMoveAllele() + r->GetMoveAllele();
}


void Genome::Mutate( void ) {
  Mutate(1);
}

void Genome::Mutate( cint num ) {
  _FAST cint n,i,c;
  bset tmp;
  for( n=0;n<num;n++ ){
    i = RAND_INT(C_COUNT-1);
    c = RAND_INT(C_LENGTH-1);
    if( RAND_INT(1) ) left[i].set(c,true);
    else right[i].set(c,true);
  }
  c = 0;
  n = 0;
  for( i=0;i<C_COUNT;i++ ){
    tmp = left[i] & right[i];
    c += (cint)tmp.count();
    tmp = left[i] ^ right[i];
    n += (cint)tmp.count();
  }
  doubleCount = c;
  singleCount = n;
  fitness = FITNESS_FUNC(doubleCount);
  baseFitness = BASE_FIT_FUNC((FIT_SINGLE*singleCount)+doubleCount);
}

void Genome::Clear( void ) {
  _FAST cint i;
  for( i=0;i<C_COUNT;i++ ){
    left[i].reset();
    right[i].reset();
  }
  Mutate(0);
}

cint Genome::GetMoveAllele( void ) const {
  if( famAllele == 2 ) return 1;
  else if( famAllele == 1 ) return RAND_INT(1);
  else return 0;
}

cint Genome::GetMoveAlleles( void ) const {
  return famAllele;
}

cint Genome::GetMutationCount( void ) const {
  return singleCount+doubleCount;
}

double Genome::Fitness( void ) const {
  return fitness;
}

double Genome::BaseFitness( void ) const {
  return baseFitness;
}
  
unsigned short Genome::Poisson( double lambda ) {
  _FAST float u = (float)RAND(1.0);
  _FAST float e = (float)exp(-1.0*lambda);
  _FAST int i=0,k=0,j=1;
  _FAST double h=1.0,current=e;
  while( i > -1 ){
    if (u < current) return i;
    i++;
    k++;
    j=j*k;
    h=h*lambda;
    current=current+(e*h/j);
  }
  return 0;
}
/* Lilach's poisson function
 *** 
int individual::poisson(double lambda)
{
  float u=tal_random::uniform(1);
  float e=exp(-1.0*lambda);
  int i=0,k=0,j=1;
  double h=1.0,current=e;
  while (i>-1)
  {
        if (u<current) return i;
        i++;
        k++;
        j=j*k;
        h=h*lambda;
        current=current+(e*h/j);
  }
}
*/


////####################################################
//
//void Genome::Clear( void ){
//	for( int i=0;i<C_COUNT;i++ ){
//		left[i]->Clear();
//		right[i]->Clear();
//	}
//  famAllele = RAND_INT(2);
//  fitness = STARTING_FITNESS;
//}
//
//cint Genome::GetMoveAllele( void ) const {
//	if( famAllele == 2 ) return 1;
//  else if( famAllele == 1 ) return RAND_INT(1);
//  else return 0;
//}
//
//cint Genome::GetMoveAlleles( void ) const {
//  return famAllele;
//}
//
//Chromosome **Genome::GetGamete( Chromosome **g ) const {
//  cint i;
//
//  for( i=0;i<C_COUNT;i++ ){
//    g[i]->Recombine(left[i],right[i]);
//  }
//  return g;
//}
//
//unsigned short Genome::Poisson( double lambda ) {
//  _FAST float u = (float)RAND(1.0);
//  _FAST float e = (float)exp(-lambda);
//  _FAST cint i=0,k=0,j=1;
//  _FAST double h=1.0,current=e;
//  while( i > -1 ){
//    if (u<current) return i;
//    i++;
//    k++;
//    j=j*k;
//    h=h*lambda;
//    current=current+(e*h/j);
//  }
//  return 0;
//}
//
//void Genome::Reproduce( const Genome *l,const Genome *r ) {
//  Clear();
//  if( RAND(2.0) < 1.0 ){
//    left = l->GetGamete(left);
//    right = r->GetGamete(right);
//    famAllele = l->GetMoveAllele() + r->GetMoveAllele();
//  }
//  else {
//    left = r->GetGamete(left);
//    right = l->GetGamete(right);
//    famAllele = l->GetMoveAllele() + r->GetMoveAllele();
//  }
//  Mutate(Poisson(LAMBDA));
//}
//
//void Genome::Mutate( cint num ) {
//  _FAST cint c,w;
//  for( int i=0;i<num;i++ ){
//    c = (cint)RAND(C_COUNT-1);
//    w = (cint)RAND(2);
//    if( w ){
//      left[c]->Mutate();
//    }
//    else right[c]->Mutate();
//  }
//  c = GetMutationCount();
//  fitness = FITNESS_FUNC(c);
//  baseFitness = BASE_FIT_FUNC(c);
//}
//
//cint Genome::GetMutationCount( void ) const {
//  _FAST cint i,t;
//  t = 0;
//  for( i=0;i<C_COUNT;i++ ){
//    t += left[i]->GetMutationCount();
//    t += right[i]->GetMutationCount();
//  }
//  return t;
//}
//
//double Genome::Fitness( void ) const {
//  return fitness;
//}
//
//double Genome::BaseFitness( void ) const {
//  return baseFitness;
//}
//
//void Genome::Print( void ) const {
//  cout << "Genome mutations: " << GetMutationCount() << endl;
//  for( int i=0;i<C_COUNT;i++ ){
//    printf("%2dl",i);
//    left[i]->Print();
//    printf("%2dr",i);
//    right[i]->Print();
//  }
//}
//
//void Genome::PrintASCII( void ) const {
////  cout << "Genome mutations: " << GetMutationCount() << endl;
//  for( int i=0;i<C_COUNT;i++ ){
//    printf("%2dl",i);
//    left[i]->PrintASCII();
//    printf("%2dr",i);
//    right[i]->PrintASCII();
//  }
//}
//
//// #########################################################
//Chromosome::Chromosome() {
//  count = 0;
//}
//
//Chromosome::~Chromosome() {
//}
//
//void Chromosome::Clear( void ){
//  count = 0;
//  mSet.reset();
//}
//
//void Chromosome::Mutate( void ) {
//  cint c;
//  c = C_LENGTH - 2;
//  c = (cint)RAND(c) + 1;
//  Mutate(c);
//}
//
//void Chromosome::Mutate( cint pos ) {
//  mSet.set(pos,true);
//  count = (cint)mSet.count();
//}
//
//void Chromosome::Recombine( const Chromosome *l,const Chromosome *r ) {
//  _FAST cint c; // Loci of cross-over
//  bset bl(*l->GetMutref()),br(*r->GetMutref());
//
//  c = C_LENGTH - 2;
//  c = (cint)RAND_INT(c) + 1;
//  if( RAND(2.0) < 1.0 ){
//    mSet = (bl<<c>>c) | (br>>(C_LENGTH-c)<<(C_LENGTH-c));
//  }
//  else {
//    mSet = (br<<c>>c) | (bl>>(C_LENGTH-c)<<(C_LENGTH-c));
//  }
//  count = (cint)mSet.count();
//}
//
//cint Chromosome::GetMutationCount( void ) const {
//  return count;
//}
//
//bset Chromosome::GetMutations( void ) const {
//  return mSet;
//}
//
//bset *Chromosome::GetMutref( void ) const {
//  return mSet;
//}
//
//void Chromosome::Print( void ) const {
//  cout << (int)mSet.count() << " mutations." << endl;
//}
//
//void Chromosome::PrintASCII( void ) const {
//  cout << mSet << endl;
//}