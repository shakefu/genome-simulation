// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "simulation.h"
#include "genome.h"

int main( ){
  srand((int)time(NULL));
  //########################################
  //bset a,b,c,t;

  //for( int i=0;i<20;i++ ){
  //  a.flip((size_t)RAND(C_LENGTH-1));
  //  b.flip((size_t)RAND(C_LENGTH-1));
  //}
  //cout << a << endl << b << endl;
  //
  //for( int i=0,l;i<5;i++ ){
  //  l = (int)RAND(C_LENGTH-1);
  //  cout << "l: " << l << endl;
  //  c = (a << l >> l) | (b >> (C_LENGTH-l) << (C_LENGTH-l) );
  //  cout << a << endl << b << endl << c << endl << endl;
  //}
  //PAUSE;
  //########################################
  //Chromosome *a,*b,*c;

  //a = new Chromosome();
  //b = new Chromosome();
  //c = new Chromosome();

  //for( int i=0;i<10;i++ ){
  //  a->Mutate();
  //  b->Mutate();
  //}

  //for( int i=0;i<10;i++ ){
  //  c->Recombine(a,b);
  //  a->PrintASCII();
  //  b->PrintASCII();
  //  c->PrintASCII();
  //  cout << endl;
  //}

  //PAUSE;
  //delete a;
  //delete b;
  //delete c;
  //########################################
  //Genome *a,*b,*c,*t;

  //a = new Genome();
  //b = new Genome();
  //c = new Genome();

  //for( int i=0;i<10;i++ ){
  //  a->Mutate(5);
  //  b->Mutate(5);
  //  a->PrintASCII();
  //  b->PrintASCII();
  //  cout << endl;
  //  c->Reproduce(a,b);
  //  c->PrintASCII();
  //  cout << endl;
  //  t = a;
  //  a = b;
  //  b = c;
  //  c = t;
  //}
  //PAUSE;
  // delete a;
  // delete b;
  // delete c;
  //########################################
  //Individual *a,*b,*n,*t;
  //time_t ts,te;

  //cout << "C_LENGTH = " << C_LENGTH << endl
  //	<< "C_COUNT = " << C_COUNT << endl;

  //a = new Individual();
  //b = new Individual();
  //n = new Individual();
  //a->Revive();
  //b->Revive();
  //n->Kill();
  // for( int i=0;i<35;i++ ){
  //   a->GetGenome()->Mutate();
  //   b->GetGenome()->Mutate();
  // }
  //PAUSE;
  //ts = time(NULL);
  //for( int i=0;i<10000;i++ ){
  //	n->Revive();
  //	n->Reproduce(a,b);
  //	n->GetGenome()->Mutate();
  //	a->Kill();
  //	t = a;
  //	a = b;
  //	b = n;
  //	n = t;
  //}
  //te = time(NULL);
  //cout << "Time taken: " << (int)(te-ts) << endl;
  //PAUSE;
  //delete a;
  //delete b;
  //delete n;

  //########################################
  Simulation *s;
  Grid *g;
  sStats st;
  time_t ts,te,tc = time(NULL);

  s = new Simulation();
  g = s->GetGrid();
  cout << "Pop size: " << g->GetPopSize() << endl;
  cout << "Average fitness: " << g->AverageFitness() << endl;
  s->NaturalSelection();
  cout << "Pop size: " << g->GetPopSize() << endl;
  cout << "Average fitness: " << g->AverageFitness() << endl;
  PAUSE;
  ts = time(NULL);
  for( int i=0;i<YEARS_RUN;i++ ){
    s->Year();
    if( !((i+1)%100) || !g->GetPopSize() || g->GetPopSize() == MAX_POP ){
      st = s->stats;
      cout << endl;
      cout << "Year: " << st.year << " Population size: " << g->GetPopSize() << endl;
      cout << "Average fitness: " << g->AverageFitness() << endl;
      cout << "Average base fitness: " << g->AverageBaseFitness() << endl;
      cout << "Time taken: " << (long)time(NULL)-(long)tc << endl;
      cout << "Offspring that didn't make it: " << st.y_selected << endl
        << "Offspring that couldn't find a home: " << st.y_unmoved << endl
        << "Offspring that made it: " << st.y_surviving << endl
        << "Offspring with FAM movements: " << st.y_fammove << endl
        << "Total offspring produced: " << st.y_selected+st.y_surviving+st.y_unmoved << endl
        << endl;
      cout.flush();
      tc = time(NULL);
    }
    if( !g->GetPopSize() || g->GetPopSize() == 10000 ) break;
  }
  te = time(NULL);
  cout << "Pop size: " << g->GetPopSize() << endl;
  cout << "Average fitness: " << g->AverageFitness() << endl;
  cout << "Time taken: " << (int)(te-ts) << endl;
  delete s;
  cout << "Simulation deleted." << endl;
  PAUSE;
  //#######################################
  //Genome *me;
  //double t = 0.0;
  //int count = 500;
  //int foo;

  //me = new Genome();
  //for( int i=0;i<count;i++ ){
  //  foo = me->Poisson(LAMBDA);
  //  t += foo;
  //  cout << foo << endl;
  //}
  //t /= count;
  //cout << "Average: " << t << endl;
  //delete me;
  //PAUSE;
  //#######################################
  //iList l;
  //time_t ts,te;
  //Simulation *s;
  //Grid *g;
  //s = new Simulation();
  //g = s->GetGrid();
  //ts = time(NULL);
  //for( int i=0;i<10000;i++ ){
  //  l = g->GetIndividualList();
  //  delete [] l.list;
  //  l = g->GetShuffledIndividualList();
  //  delete [] l.list;
  //  l = g->GetShuffledOpenList();
  //  delete [] l.list;
  //}
  //te = time(NULL);
  //cout << "Time taken: " << (double)(te-ts)/100.0 << endl;
  //PAUSE;
  //######################################
  //int i,j;
  //int x,y;
  //int t;
  //int rrad;
  //int rstep;
  //int side;
  //int rcnt;
  //int cw;
  //sint r;
  //coord c;
  //Individual *_i = NULL;

  //r = 2;
  //c.x = 10;
  //c.y = 10;

  //rrad = 1;
  //x = c.x;
  //y = c.y;
  //cw = 0;
  //while( rrad <= (int)r ){
  //  rcnt = 0;
  //  rstep = 8*rrad;
  //  side = 2*rrad;
  //
  //  t = RAND_INT(rstep);
  //  while( rcnt < rstep ){
  //    rcnt++;
  //    switch( t / side ){
  //      case 0:
  //        i = (t % side) - rrad;
  //        j = -rrad;
  //        cout << "Case 0 ";
  //        break;
  //      case 1:
  //        i = rrad;
  //        j = (t % side) - rrad;
  //        cout << "Case 1 ";
  //        break;
  //      case 2:
  //        i = rrad - (t % side);
  //        j = rrad;
  //        cout << "Case 2 ";
  //        break;
  //      case 3:
  //        i = -rrad;
  //        j = rrad - (t % side);
  //        cout << "Case 3 ";
  //        break;
  //    }
  //    c.x = x + i;
  //    c.y = y + j;
  //    if( c.x < 0 ) c.x += GRID_WIDTH;
  //    else if( c.x >= GRID_WIDTH ) c.x -= GRID_WIDTH;
  //    if( c.y < 0 ) c.y += GRID_HEIGHT;
  //    else if( c.y >= GRID_HEIGHT ) c.y += GRID_HEIGHT;

  //    printf("%2d %2d %2d %2d (%2d,%2d)\n",rrad,rcnt,t,rstep,c.x,c.y);
  //    if( cw ) t++;
  //    else t--;
  //    if( t > rstep ) t = 0;
  //    else if( t < 0 ) t = rstep-1;
  //  }
  //  cout << "Next ring. " << endl;
  //  rrad++;
  //}
  //PAUSE;
  //####################################################
  //Simulation *sim;
  //Grid *grid;
  //coord c;
  //Individual *i0,*i1;
  //Genome *gen;

  //sim = new Simulation();
  //grid = sim->GetGrid();
  //gen = new Genome();

  //for( int i=0;i<100;i++ ){
  //  c.x = RAND_INT(GRID_WIDTH-1);
  //  c.y = RAND_INT(GRID_HEIGHT-1);
  //  i0 = grid->GetIndividual(c);
  //  c.x = RAND_INT(GRID_WIDTH-1);
  //  c.y = RAND_INT(GRID_HEIGHT-1);
  //  i1 = grid->GetIndividual(c);

  //  gen->Reproduce(i0->GetGenome(),i1->GetGenome());

  //  if( !(i%10) )
  //    cout << "i0 all\ti0 get\ti1 all\ti1 get"
  //    << "\tgen all\tgen get" << endl;
  //  cout << i0->GetGenome()->GetMoveAlleles() << "\t  "
  //    << i0->GetGenome()->GetMoveAllele() << "\t  "
  //    << i1->GetGenome()->GetMoveAlleles() << "\t  "
  //    << i1->GetGenome()->GetMoveAllele() << "\t  "
  //    << gen->GetMoveAlleles() << "\t  "
  //    << gen->GetMoveAllele() << "\t  "
  //    << endl;
  //}

  //delete sim;
  //PAUSE;
  //####################################################
  //_FAST int i,t,j;

  //for( j=0;j<20;j++ ){
  //  for( t=0,i=0;i<100;i++ ){
  //    t += RAND_INT(1);
  //  }
  //  cout << (double)t / (double)100.0 << endl;
  //}
  //PAUSE;
  //###################################################
//  Simulation *s;
//  Grid *g;
//  sint i;
//  s = new Simulation();
//  g = s->GetGrid();
//  for( i=0;i<100;i++ ){
//    s->Year();
//    s->OutputData();
////    r = s->RelativeSelection(0.20);
//    cout << "Pop: " << g->GetPopSize() << endl;
//  }
//  PAUSE;
  //###################################################

  return 0;
}
