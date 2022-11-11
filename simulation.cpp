#include "stdafx.h"
#include "defs.h"
#include "grid.h"
#include "simulation.h"

Simulation::Simulation( ){
  //sint _i = MAX_POP/2+1;
  //offspring.list = new (Individual (*[_i]));;
  //for( int i=0;i<_i;i++ )
  //  offspring.list[i] = new Individual();
  //offspring.count = 0;

  g = new Grid();
  g->Initialize();

  //relYear = 0;
  //relFitness = new double[MAX_POP];
  //for( int i=0;i<MAX_POP;i++ )
  //  relFitness[i] = 0.0;

  stats.year        = 0;
  stats.t_selected  = 0;
  stats.t_surviving = 0;
  stats.t_unmoved   = 0;
  stats.y_selected  = 0;
  stats.y_surviving = 0;
  stats.y_unmoved   = 0;
  stats.y_fammove   = 0;
  stats.y_popsize   = 0;
  stats.y_remaining = 0;
  oldStats = stats;
  statTag = (sint)time(NULL);

  OUTPUT_FREQUENCY = OUTPUT_FREQ;

  rYear = 0;
}

Simulation::~Simulation( ){
  delete g;
  g = NULL;
  //output.close();
  //delete [] relFitness;
  //delete [] offspring.list;
  //offspring.list = NULL;
  //offspring.count = 0;
}

#ifdef OUTPUT_STATS
//########################################################
// OUTPUT STUFF
//########################################################

string Simulation::GetFName( ) const {
  ostringstream s;
  string fname;
#ifdef FIXED_ADULT_RATE
  s << "F" << (int)(ADULT_RATE*100) << "_";
#endif
#ifdef FAM_DIFF
  s << "D";
#endif
#ifdef ABSOLUT_SELECTION
  s << "A";
#endif
#ifdef SQUARED_FIT
  s << "S";
#endif
#ifdef OPENING_MOVEMENT
  s << "O";
#endif
  s << "_";
  s << "FT" << (int)(FAM_THRESH*100) << "_";
  s << "MR" << MOVE_RADIUS << "_";
  s << "SF" << STARTING_FAM_RATE*100.0 << "_";
  s << "Y" << YEARS_RUN << "_";
  s << "D" << FAM_DELAY << "_";
  s << statTag;
  fname = s.str();
  fname = OUTPUT_PRE + fname + OUTPUT_POST;
  return fname;
}

void Simulation::OutputData( void ){
  iList l;
  sint i;
  sint f0 = 0,f1 = 0,f2 = 0,fm; // nonFAM, one FAM, two FAM
  sint tm0 = 0,tm1 = 0; // mutation counts
  double fd0 = 0.0,fd1 = 0.0; // nonFAM fitness, FAM fit
  double vd0 = 0.0,vd1 = 0.0; // nonFAM variance, FAM var
  double md0, md1; // average fitnesses
  double t, td0, td1;  // mutation averages
  string fname;
  ofstream output;

  fname = GetFName();

  l = g->GetIndividualList();

  // Round 1 - Compute the means, average number of mutations
  for( i=0;i<l.count;i++ ){
    fm = l.list[i]->GetGenome()->GetMoveAlleles();
    if( fm ){
      tm1 += l.list[i]->GetGenome()->GetMutationCount();
      fd1 += l.list[i]->BaseFitness();
      if( fm == 2 ) f2++;
      else if( fm == 1 ) f1++;
    }
    else { 
      tm0 += l.list[i]->GetGenome()->GetMutationCount();
      f0++; 
      fd0 += l.list[i]->BaseFitness(); 
    }
  }
  if( !f0 ){
    md0 = 0.0;
    td0 = 0.0;
  }
  else {
    md0 = fd0 / f0;
    td0 = (double)tm0 / (double)f0;
  }
  if( !(f1+f2) ){
    md1 = 0.0;
    td1 = 0.0;
  }
  else {
    md1 = fd1 / (f1+f2);
    td1 = (double)tm1 / (double)(f1+f2);
  }

  // Round 2 - Compute the variances
  for( i=0;i<l.count;i++ ){
    fm = l.list[i]->GetGenome()->GetMoveAlleles();
    if( fm ){
      t = (l.list[i]->BaseFitness() - md1);
      t *= t;
      vd1 += t;
    }
    else {
      t = (l.list[i]->BaseFitness() - md0);
      t *= t;
      vd0 += t;
    }
  }
  if( !(f1+f2-1) ) vd1 = 0.0;
  else vd1 /= f1 + f2 - 1;
  if( !(f0-1) ) vd0 = 0.0;
  else vd0 /= f0 - 1;
  delete [] l.list;

  if( !stats.y_long ){
    stats.y_fitlong = 0.0;
    stats.y_longdist = 0.0;
  }
  else {
    stats.y_fitlong /= stats.y_long;
    stats.y_longdist /= stats.y_long;
  }

  output.open(fname.c_str(),ofstream::out|ofstream::app);
  output << stats.year << "\t"
    << stats.y_popsize << "\t"
    << stats.y_remaining << "\t"
    << stats.t_selected - oldStats.t_selected << "\t"
    << stats.t_surviving - oldStats.t_surviving << "\t"
    << stats.t_unmoved - oldStats.t_unmoved << "\t"
    << stats.t_fammove - oldStats.t_fammove << "\t"
    << f0 << "\t"
    << f1 << "\t"
    << f2 << "\t"
    << stats.avg_fitness << "\t"
    << (md0) << "\t"
    << (md1) << "\t"
    << (vd0) << "\t"
    << (vd1) << "\t"
    << (td0) << "\t"
    << (td1) << "\t"
    << stats.y_long << "\t"
    << stats.y_short << "\t"
    << stats.y_fitlong << "\t"
    << (stats.y_fitshort / stats.y_short) << "\t"
    << stats.y_longdist << "\t"
    << endl;
  output.close();
  oldStats = stats;
}

void Simulation::OutputStats( void ){
  string fname;
  ofstream output;

  fname = GetFName();

  if( stats.year == 1 ){
    output.open(fname.c_str(),ofstream::out|ofstream::app);
    output << "Simuation Started\t" << statTag << endl
      << "GRID_HEIGHT\t" << GRID_HEIGHT << endl
      << "GRID_WIDTH\t" << GRID_WIDTH << endl
      << "YEARS_RUN\t" << YEARS_RUN << endl
      //<< "STARTING_FITNESS\t" << STARTING_FITNESS << endl
      << "STARTING_FAM_RATE\t" << STARTING_FAM_RATE << endl
      << "ADULT_RATE\t" << ADULT_RATE << endl
      << "MAX_OPENINGS\t" << MAX_OPENINGS << endl
      //      << "MUTATION_RATE\t" << MUTATION_RATE << endl
      << "MUTATION_EFF\t" << MUTATION_EFF << endl
      << "FAM_THRESH\t" << FAM_THRESH << endl
      << "MOVE_RADIUS\t" << MOVE_RADIUS << endl
      << "FAM_RADIUS\t" << FAM_RADIUS << endl
      << "REPRODUCTION_RADIUS\t" << REPRODUCTION_RADIUS << endl
      << "C_LENGTH\t" << C_LENGTH << endl
      << "C_COUNT\t" << C_COUNT << endl;
    output << "Options: " << endl
#ifdef FIXED_ADULT_RATE
      << "FIXED_ADULT_RATE, "
#endif
#ifdef FAM_DIFF
      << "FAM_DIFF, "
#endif
#ifdef ABSOLUT_SELECTION
      << "ABSOLUT_SELECTION, "
#endif
#ifdef SQUARED_FIT
      << "SQUARED_FIT, "
#endif
#ifdef OPENING_MOVEMENT
      << "OPENING_MOVEMENT, "
#endif
      << endl << endl;

    output << "Year\t"
      << "Pop Size (Before selection)\t"
      << "Pop Size (After selection)\t"
      << "Off. Selected\t"
      << "Off. Surviving\t"
      << "Off. Unmoved\t"
      << "FAM moves\t"
      << "nonFAM\t"
      << "One FAM\t"
      << "Two FAM\t"
      << "Avg Fit\t"
      << "Avg nonFAM Fit\t"
      << "Avg FAM Fit\t"
      << "nonFAM Variance\t"
      << "FAM variance\t"
      << "Avg nonFAM Muts\t"
      << "Avg FAM Muts\t"
      << "Long moves\t"
      << "Short moves\t"
      << "Long Fit\t"
      << "Short fit\t"
      << "Avg. Long dist\t"
      << endl;
    output.close();
    oldStats = stats;
  }
  if( !(stats.year % OUTPUT_FREQUENCY) || stats.year == 1 ){
    OutputData();
  }
}
#endif
//#########################################################
// SIMULATION FUNCS
//#########################################################
void Simulation::Year( void ){
  stats.year++;
  stats.y_selected  = 0;
  stats.y_surviving = 0;
  stats.y_unmoved   = 0;
  stats.y_fammove   = 0;
  stats.y_long      = 0;
  stats.y_longdist  = 0.0;
  stats.y_short     = 0;
  stats.y_fitshort  = 0.0;
  stats.y_fitlong   = 0.0;

  EverybodyReproduce();
  stats.avg_fitness = g->AverageBaseFitness();
  // EverybodyMove(); // Does nothing, anyway
#ifdef OUTPUT_STATS
  OutputStats();
#endif
  stats.y_popsize = g->GetPopSize();
  NaturalSelection();
  stats.y_remaining = g->GetPopSize();
  
  if( g->GetPopSize() < MAX_POP / 4 )
    OUTPUT_FREQUENCY = 1;
  else OUTPUT_FREQUENCY = OUTPUT_FREQ;

  if( !g->GetPopSize() ){
    OutputStats();
    cout << "Population died!" << endl;
  }
}

void Simulation::NaturalSelection( ){
  iList l = g->GetIndividualList();
  sint i;
  //for( i=0;i<l.count;i++ ){
  //  l.list[i]->Age();
  //  if( RelativeSelection(l.list[i]->BaseFitness()) //)
  //    || RAND(1.0) > ADULT_RATE )
  //    g->Kill(l.list[i]);
  //}
  //for( i=0;i<l.count;i++ ){
    //if( !((l.list[i])->Select()) ){
    //  g->Kill(l.list[i]);
    //}
    //if( !RelativeFitness(l.list[i]->Fitness(),(1.0 - ADULT_RATE)) ){
    //  g->Kill(l.list[i]);
    //}
    // Relative Fitness doesn't work 'cause of lack
    // of variation in the first few years
    //if( RelativeFitness(l.list[i]->Fitness()) < (1.0 - ADULT_RATE) )
    //  g->Kill(l.list[i]);
  //}
  //for( i=0;i<offspring.count;i++ ){
  //  if( !((offspring.list[i])->Select()) ){
  //    offspring.list[i]->Kill();
  //  }
  //}
  for( i=0;i<l.count;i++ ){
    if( l.list[i]->Select() ){
      g->Kill(l.list[i]);
    }
  }
  delete [] l.list;
}

void Simulation::EverybodyReproduce( void ){
  //  iList l,n,m;
  //  sint i,c,d;
  //  if( (10*g->GetPopSize()/g->GetPopMax()) < 5 ){
  //    l = g->GetShuffledIndividualList();
  //    for( i=0;i<l.count;i++ ){
  //      n = g->GetOpenNeighbors(l.list[i]->GetCoord(),
  //        l.list[i]->ReproductionRadius());
  //      if( !n.count ){
  //        delete [] n.list;
  //        continue;
  //      }
  //      c = (sint)RAND(n.count - 1);
  //      m = g->GetNeighbors(l.list[i]->GetCoord(),
  //        l.list[i]->ReproductionRadius());
  //      if( !m.count ){
  //        delete [] m.list;
  //        delete [] n.list;
  //        continue;
  //      }
  //      d = (sint)RAND(m.count - 1);
  //      // This'll have to be changed later...
  //      g->Revive(n.list[c]);
  //      n.list[c]->Reproduce(l.list[i],m.list[d]);
  //      delete [] n.list;
  //      delete [] m.list;
  //    }
  //  }
  //  else {
  //    l = g->GetShuffledOpenList();
  //    for( i=0;i<l.count;i++ ){
  //      n = g->GetNeighbors(l.list[i]->GetCoord(),
  //        l.list[i]->ReproductionRadius());
  //      if( !n.count ){
  //        delete [] n.list;
  //        continue;
  //      }
  //      c = (sint)RAND(n.count - 1);
  //      m = g->GetNeighbors(n.list[c]->GetCoord(),
  //        n.list[c]->ReproductionRadius());
  //      if( !m.count ){
  //        delete [] m.list;
  //        delete [] n.list;
  //        continue;
  //      }
  //      d = (sint)RAND(m.count - 1);
  //      // This'll have to be changed later...
  //      g->Revive(l.list[i]);
  //      l.list[i]->Reproduce(n.list[c],m.list[d]);
  //      delete [] n.list;
  //      delete [] m.list;
  //    }
  //  }
  //  delete [] l.list;
  //#############################################

  //iList l;
  //Individual *_p,*_o;
  //coord c,d;
  //int x,y;

  //l = g->GetShuffledIndividualList();

  //offspring.count = 0;

  //for( sint i=0;i<l.count;i++ ){
  //  c = l.list[i]->GetCoord();
  //  x = c.x;
  //  y = c.y;
  //  x = x - REPRODUCTION_RADIUS + RAND_INT(REPRODUCTION_RADIUS*2);
  //  y = y - REPRODUCTION_RADIUS + RAND_INT(REPRODUCTION_RADIUS*2);
  //  x = x<0?x+GRID_WIDTH:x>=GRID_WIDTH?x-GRID_WIDTH:x;
  //  y = y<0?y+GRID_HEIGHT:y>=GRID_HEIGHT?y-GRID_HEIGHT:y;
  //  d.x = x;
  //  d.y = y;
  //  _p = g->GetIndividual(d);
  //  if( !_p->CanReproduce() || !l.list[i]->CanReproduce() ){
  //    _p = NULL;
  //    continue;
  //  }
  //  _o = offspring.list[offspring.count];
  //  _o->Reproduce(l.list[i],_p);
  //  _o->Move(c);
  //  offspring.count++;
  //}
  //delete [] l.list; */
  _FAST iList openings;
  _FAST iList individuals;
  _FAST coord *clist;
  _FAST Individual *_m = NULL;
  _FAST Genome *_g = NULL;
  _FAST int ocount;
  _FAST coord c;
  _FAST int x,y,r;
  _FAST Individual *_i = NULL;
  _FAST sint i,pop = 0;
  _FAST int t;

  openings.list = NULL;
  individuals.list = NULL;
  clist = NULL;
#ifndef TEST1
  openings = g->GetShuffledOpenList();
  individuals = g->GetShuffledIndividualList();

  clist = new coord[openings.count];
  for( i=0;i<openings.count;i++ )
    clist[i] = openings.list[i]->GetCoord();
  
  ocount = openings.count;
wbegin:
#else
wbegin:
  delete [] clist;
  delete [] openings.list;
  delete [] individuals.list;
  openings = g->GetShuffledOpenList();
  individuals = g->GetShuffledIndividualList();

  clist = new coord[openings.count];
  for( i=0;i<openings.count;i++ )
    clist[i] = openings.list[i]->GetCoord();
  
  ocount = openings.count;
#endif

#ifdef OPENING_MOVEMENT
  for( i=0;i<openings.count;i++ ){
    if( openings.list[i] == NULL ) continue;
    c = clist[i];
    t = MOVE_RADIUS;
    t *= t;
    while( t ){
      t--;
      _m = g->GetNeighbor(c,MOVE_RADIUS);
      if( _m ) break;
    }
    if( !_m ) continue;
    t = REPRODUCTION_RADIUS;
    t *= t;
    while( t ){
      t--;
      _i = g->GetNeighbor(_m,REPRODUCTION_RADIUS);
      if( _i && _i != _m ) goto reproduce;
    }
    continue;
reproduce:

#else
  i = 0;
  while( ocount ){
    if( !(i < individuals.count) ) break;
    _i = individuals.list[i];
    c = _i->GetCoord();
    _m = g->GetNeighbor(c,_i->ReproductionRadius());
    if( !_m ){
      i++;
      continue;
    }
#endif
    //if( !_i->GetGenome()->GetMoveAlleles() &&
    //    !_m->GetGenome()->GetMoveAlleles() ){
    //  if( !g->GetOpenNeighbor(_i,MOVE_RADIUS) &&
    //      !g->GetOpenNeighbor(_m,MOVE_RADIUS) ){
    //    stats.t_unmoved++;
    //    stats.y_unmoved++;
    //    goto wend;
    //  }
    //}
    _g = new Genome();
    _g->Reproduce(_i->GetGenome(),_m->GetGenome());

    if( _g->BaseFitness() < RAND(1.0) ){
      stats.t_selected++;
      stats.y_selected++;
      goto wend;
    }
    if( stats.year > FAM_DELAY ){
#ifdef FAM_DIFF
      r = _g->GetMoveAlleles();
      if( r && RelativeFAM(_g->BaseFitness()) ){
        if( r == 2 ) r = FAM_RADIUS;
        else r = FAM_RADIUS/2;
      }
      else r = MOVE_RADIUS;
#else
      if( _g->GetMoveAlleles() &&
        RelativeFAM(_g->BaseFitness()) )
        r = FAM_RADIUS;
      else r = MOVE_RADIUS;
#endif
    }
    else r = MOVE_RADIUS;

    for( t=openings.count-1;t>=0;t-- ){
      if( !openings.list[t] ) continue;
      x = c.x - clist[t].x;
      x = abs(x);
      y = c.y - clist[t].y;
      y = abs(y);
      if( x <= r && y <= r ){
okdistance:
        _i = openings.list[t];
        g->Revive(_i);
        _i->SetGenome(_g);
        _g = NULL;
        openings.list[t] = NULL;
        ocount--;
        stats.t_surviving++;
        stats.y_surviving++;
        if( r > MOVE_RADIUS ){
          stats.t_fammove++;
          stats.y_fammove++;
          stats.y_long++;
          stats.y_longdist += sqrt((double)(x*x+y*y));
          stats.y_fitlong += _i->BaseFitness();
        }
        else {
          stats.y_short++;
          stats.y_fitshort += _i->BaseFitness();
        }
        goto wend;
      } // if 
      else {
        x = GRID_WIDTH - clist[t].x + x;
        y = GRID_HEIGHT - clist[t].y + y;
        if( x <= r && y <= r ) goto okdistance;
      }
    } // for
    stats.t_unmoved++;
    stats.y_unmoved++;
wend:
    delete _g;
    _g = NULL;
    _i = NULL;
    _m = NULL;
#ifndef OPENING_MOVEMENT
    i++;
#endif
  } // for/while
  if( pop != g->GetPopSize() && stats.y_selected < MAX_OFFSPRING && (double)ocount/MAX_POP > MAX_OPENINGS ){
    pop = g->GetPopSize();
    goto wbegin;
  }
  delete [] clist;
  delete [] openings.list;
  delete [] individuals.list;
}
//##########################################################
// ASSISTANT FUNCS
//##########################################################

Grid *Simulation::GetGrid( ){
  return g;
}

int __cdecl lcmp( const void *a, const void *b ){
  return (int)(100.0*( *(double*)a - *(double*)b ));
}

//sint Simulation::RelativeSelection( double x ){
//  // Uses 1.0 - ADULT_RATE as the comparison value.
//  // Returns 1 if below threshold.
//
//  if( rYear != stats.year ){
//    iList l;
//    sint i;
//    double *sortable;
//
//    rYear = stats.year;
//    fitThresh = 0.0;
//
//    l = g->GetIndividualList( );
//    sortable = new double[l.count];
//
//    for( i=0;i<l.count;i++ )
//      sortable[i] = l.list[i]->BaseFitness();
//
//    qsort(sortable,l.count,sizeof(double),lcmp);
//    
//    fitThresh = sortable[(int)(l.count*(1.0-ADULT_RATE))];
//    delete [] sortable;
//    delete [] l.list;
//    cout << " fitThresh == " << fitThresh << endl;
//  }
//  if( x < fitThresh ){
//    return 1;
//  }
//  return 0;
//}

sint Simulation::RelativeFAM( double x ){
  // Uses FAM_THRESH as the comparison value.
  // Returns 1 if below threshold.

  if( rYear != stats.year ){
    iList l;
    sint i;
    double *sortable;

    rYear = stats.year;
    l = g->GetIndividualList( );
    sortable = new double[l.count];

    for( i=0;i<l.count;i++ )
      sortable[i] = l.list[i]->BaseFitness();

    qsort(sortable,l.count,sizeof(double),lcmp);
    
    fitThresh = sortable[(int)(l.count*(FAM_THRESH))];
    //cout << "fitThresh: " << fitThresh 
    //  << "  low: " << low << endl;
    delete [] sortable;
    delete [] l.list;
  }
  if( x < fitThresh ) return 1;
  return 0;
}
//########################
// SOME OLD STUFF - SAVE
//########################
//void Simulation::EverybodyMove( ){
//iList l,n;
//sint i,c;
//if( (10*g->GetPopSize()/g->GetPopMax()) < 5 ){
//  // Mostly empty, use individuals.
//  l = g->GetShuffledIndividualList();
//  for( i=0;i<l.count;i++ ){
//    n = g->GetOpenNeighbors(l.list[i]->GetCoord(),MOVE_RADIUS);
//    if( !n.count ) continue;
//    c = n.count - 1;
//    g->Move(l.list[i],n.list[(sint)RAND(c)]);
//    delete [] n.list;
//  }
//}
//else {
//  // Mostly full, use openings.
//  l = g->GetShuffledOpenList();
//  for( i=0;i<l.count;i++ ){
//    if( l.list[i]->Alive() ) continue;
//    n = g->GetNeighbors(l.list[i]->GetCoord(),MOVE_RADIUS);
//    if( !n.count ) continue;
//    c = n.count - 1;
//    g->Move(l.list[i],n.list[(sint)RAND(c)]);
//    delete [] n.list;
//  }
//} /**/
//delete [] l.list;
//##########################################
//  iList l;
//  Individual *_i,*_s;
//
//  for( sint i=0;i<offspring.count;i++ ){
//    _i = offspring.list[i];
//    if( !_i->Alive() ) continue;
//    _s = g->GetIndividual(_i->GetCoord());
//    if( !_s->Alive() ){
//      g->SetIndividual(_s,offspring.list[i]);
//      _i->Kill();
//      continue;
//    }
//    l = g->GetOpenNeighbors(_s,_i->MoveRadius());
//    if( !l.count ) goto loop_end;
//    g->SetIndividual(l.list[RAND_INT(l.count-1)],offspring.list[i]);
//loop_end:
//    _i->Kill();
//    delete [] l.list;
//    l.count = 0;
//  }
//  for( i=0;i<offspring.count;i++ )
//    offspring.list[i]->Kill();
//  offspring.count = 0;
//}
//##########################################
//sint Simulation::RelativeFitness( double x,double cmp ){
//  iList l;
//  _FAST sint i;
//  static double t;
//
//  if( relYear != stats.year ){
//    //double vd0 = 0.0;
//    //double md0;
//    //sint f0 = 0;
//    //double fd0 = 0.0;
//
//    relYear = stats.year;
//
//    l = g->GetIndividualList();
//    relCount = l.count;
//
//    //// Round 1 - Compute the means, average number of mutations
//    //for( i=0;i<l.count;i++ ){
//    //  f0++; 
//    //  fd0 += l.list[i]->BaseFitness(); 
//    //}
//    //md0 = fd0 / f0;
//
//    //// Round 2 - Compute the variances
//    //for( i=0;i<l.count;i++ ){
//    //  t = (l.list[i]->BaseFitness() - md0);
//    //  t *= t;
//    //  vd0 += t;
//    //}
//    //vd0 /= f0 - 1;
//    //t = sqrt(vd0);
//    //if( t < 0.05 ) return (RAND(1.0)>cmp);
//
//    for( i=0;i<l.count;i++ )
//      relFitness[i] = l.list[i]->BaseFitness();
//
//    qsort(relFitness,l.count,sizeof(double),lcmp);
//
//    delete [] l.list;
//  }
//  //if( t < 0.05 ) return (RAND(1.0)>cmp);
//  i = 0;
//  while( x > relFitness[i] ){
//    if( ((double)i/(double)relCount) > cmp ) return 1;
//    i++;
//  }
//  return 0;
//}
//##############################
// Old way -- Lame.
//sint Simulation::RelativeFitness( double x,double cmp ){
//_FAST coord c;
//_FAST Individual *_i;
//_FAST int i;
//if( relYear != stats.year ){
//  relYear = stats.year;
//  for( i=0;i<RELFIT_SAMPLE;i++ ){
//    do {
//      c.x = RAND_INT(GRID_WIDTH-1);
//      c.y = RAND_INT(GRID_HEIGHT-1);
//      _i = g->GetIndividual(c);
//    } while( !_i->Alive() );
//    relFitness[i] = _i->Fitness();
//    qsort(relFitness,RELFIT_SAMPLE,sizeof(double),lcmp);
//  }
//}
//for( i=RELFIT_SAMPLE-1;i>=0;i-- ){
//  if( x > relFitness[i] )
//    return ((double)i)/RELFIT_SAMPLE+(1.0/RELFIT_SAMPLE);
//}
//return 0.0;
//}
//############################################
//iList l;
//sint i;
//static double vd0 = 0.0;
//static double md0;
//double c0,c1;
//double t,z,id;
//double sum = 0.0;

//if( stats.year != relYear ){
//  sint f0 = 0;
//  double fd0 = 0.0;
//  relYear = stats.year;
//  l = g->GetIndividualList();

//  // Round 1 - Compute the means, average number of mutations
//  for( i=0;i<l.count;i++ ){
//    f0++; 
//    fd0 += 100.0*l.list[i]->BaseFitness(); 
//  }
//  md0 = 100*fd0 / f0;

//  // Round 2 - Compute the variances
//  for( i=0;i<l.count;i++ ){
//    t = (100.0*l.list[i]->BaseFitness() - md0);
//    t *= t;
//    vd0 += t;
//  }
//  vd0 /= f0 - 1;
//  delete [] l.list;

//}
//t = sqrt(vd0);
//if( t < 5 ) return RAND(1.0);

//c0 = 2.0*vd0;
//c1 = 3.14159265359*c0;
//z = (x-md0)/t;
//t = -5.0;
//id = -t + z;
//id /= 25;
//for( i=0;i<25;i++ ){
//  sum += NORZ(t,c0,c1,md0)*id;
//  t += id;
//}
//}

void Simulation::Print( ) const {
  if( GRID_WIDTH > 70 || GRID_HEIGHT > 24 ){
    cout << "Won't print grid... too big." << endl;
  }
  else {
    g->PrintGrid();
  }
}