#include "stdafx.h"
#include "defs.h"
#include "grid.h"

Grid::Grid( ){
  sint i;
  sint c;
  popMax = GRID_WIDTH*GRID_HEIGHT;
  popSize = 0;
  //  map = (Individual **)calloc(GRID_WIDTH,sizeof(Individual *));
  map = new (Individual (**[GRID_WIDTH]));
  for( i=0;i<GRID_WIDTH;i++ ){
    map[i] = new (Individual(*[GRID_HEIGHT]));
    for( c=0;c<GRID_HEIGHT;c++ )
      map[i][c] = new Individual();
  }

  srand((int)time(NULL));
}

Grid::~Grid( ){
  sint i,c;
  for( i=0;i<GRID_WIDTH;i++ ){
    for( c=0;c<GRID_HEIGHT;c++ )
      delete map[i][c];
    delete [] map[i];
  }
  delete [] map;
}

void Grid::Initialize( void ){
  int i,j;
  coord c;
  popSize = 0;
  for( i=0;i<GRID_WIDTH;i++ ){
    for( j=0;j<GRID_HEIGHT;j++ ){
      c.x = i; c.y =j;
      map[i][j]->Kill();
      map[i][j]->Revive(c);
      popSize++;
    }
  }
}

// ######################################################

void Grid::Move( Individual *s,Individual *d ){
  // Move always just swaps the two for now...
  coord o = s->GetCoord();
  coord p = d->GetCoord();
  Swap( o,p );
}

void Grid::Kill( Individual *v ){
  coord c = v->GetCoord();
  if( v->Alive() ) popSize--;
  v->Kill();
}
void Grid::Kill( coord c ){
  Kill(GetIndividual(c));
}

void Grid::Revive( Individual *v ) {
  coord c = v->GetCoord();
  if( !v->Alive() ) popSize++;
  v->Revive(c);
}
void Grid::Revive( coord c ) {
  Revive(GetIndividual(c));
}

//void Grid::SetIndividual( Individual *a,Individual *b ) {
//  // This function is f'd up, don't use it.
//  coord c = a->GetCoord();
//  Individual *t;
//  if( map[c.x][c.y] != a ){
//    // This is wrong, but it shouldn't happen.
//    cout << "ERROR: BAD SWAP!@#!" << endl;
//  }
//  b->Move(c);
//  t = map[c.x][c.y];
//  map[c.x][c.y] = b;
//  b = t;
//}


// ######################################################

sint Grid::GetPopMax( ) const {
  return popMax;
}

sint Grid::GetPopSize( ) const {
  return popSize;
}

Individual *Grid::GetIndividual( coord c ) const {
  return (map[c.x][c.y]);
}

iList Grid::GetIndividualList( ) const {
  iList l;
  _FAST sint i,j;

  l.list = new (Individual (*[popSize]));
  l.count = 0;
  for( i=0;i<GRID_WIDTH;i++ ){
    for( j=0;j<GRID_HEIGHT;j++ ){
      if( map[i][j]->Alive() ){
        l.list[l.count] = (map[i][j]);
        l.count++;
      }
    }
  }
  return l;
}

iList Grid::GetOpenList( ) const {
  iList l;
  _FAST sint i,j;

  l.list = new (Individual (*[popMax-popSize]));
  l.count = 0;
  for( i=0;i<GRID_WIDTH;i++ ){
    for( j=0;j<GRID_HEIGHT;j++ ){
      if( !(map[i][j]->Alive()) ){
        l.list[l.count] = (map[i][j]);
        l.count++;
      }
    }
  }
  return l;
}

iList Grid::GetShuffledIndividualList( ) const {
  iList l;
  _FAST sint r,i,c;
  Individual *t;

  l = GetIndividualList( );
  c = l.count - 1;
  for( i=0;i<l.count;i++ ){
    r = RAND_INT(c);
    t = l.list[r];
    l.list[r] = l.list[i];
    l.list[i] = t;
  }
  return l;
}

iList Grid::GetShuffledOpenList( ) const {
  iList l;
  _FAST sint r,i,c;
  Individual *t;

  l = GetOpenList( );
  c = l.count - 1;
  for( i=0;i<l.count;i++ ){
    r = RAND_INT(c);
    t = l.list[r];
    l.list[r] = l.list[i];
    l.list[i] = t;
  }
  return l;
}

/*
iList Grid::GetOpenNeighbors( Individual *v,sint r ) const {
return GetOpenNeighbors( v->GetCoord(),r );
}
*/
Individual *Grid::GetNeighbor( Individual *v,sint r ) const {
  return GetNeighbor( v->GetCoord(),r );
}

Individual *Grid::GetOpenNeighbor( Individual *v,sint r ) const {
  return GetOpenNeighbor( v->GetCoord(),r );
}


/*
iList Grid::GetNeighbors( Individual *v,sint r ) const {
return GetNeighbors( v->GetCoord(),r );
}
*/
/*
iList Grid::GetOpenNeighbors( coord c,sint r ) const {
_FAST int xmin,xmax,ymin,ymax;
_FAST int i,j;
iList l;

i = 2*r + 1;
i *= i;
l.list = new (Individual (*[i]));
l.count = 0;
xmin = c.x - r;
xmax = c.x + r;
ymin = c.y - r;
ymax = c.y + r;
//  cout<<"xmin: "<<xmin<<" ymin: "<<ymin<<" xmax: "<<xmax<<" ymax: "<<ymax<<endl;
if( xmin < 0 ) xmin += GRID_WIDTH;
if( ymin < 0 ) ymin += GRID_HEIGHT;
if( xmax >= GRID_WIDTH ) xmax -= GRID_WIDTH;
if( ymax >= GRID_HEIGHT ) ymax -= GRID_HEIGHT;
xmax++;
ymax++;
//  cout<<"xmin: "<<xmin<<" ymin: "<<ymin<<" xmax: "<<xmax<<" ymax: "<<ymax<<endl;
i = xmin;
do {
if( i >= GRID_WIDTH ) i -= GRID_WIDTH;
//    cout << endl;
j = ymin;
do {
if( j >= GRID_HEIGHT ) j -= GRID_HEIGHT;
//      cout<<"i: "<<i<<" j: "<<j<<" "<<map[i][j].Alive()<<endl;
if( c.x == i && c.y == j ){
// Do nothing...
}
else {
if( !(map[i][j])->Alive() ){
l.list[l.count] = (map[i][j]);
l.count++;
}
}
j++;
} while( j != ymax );
i++;
//    cout << "i: " << i << endl;
} while( i != xmax );
//  l.list = (Individual **)realloc(l.list,l.count*sizeof(Individual *));
return l;
}
*/ 
Individual *Grid::GetOpenNeighbor( coord c,sint r ) const {
  _FAST int i,j; // Relative locs
  _FAST int x,y; // Absolute locs
  _FAST int t;   // Temp var.
  _FAST Individual *_i = NULL;

  /* This method starts at x-r,y-r, and goes through
   * the area until an opening is found - biased towards
   * topside, however that shouldn't have a negative *
   * effect (I hope)*/
  t = r;
  x = c.x;
  y = c.y;
  for( i=(x-r);i<=(int)(x+r);i++ ){
    for( j=(y-r);j<=(int)(y+r);j++ ){
      if( i == x && j == y ) continue;
      if( i < 0 ) i += GRID_WIDTH;
      else if( i >= GRID_WIDTH ) i -= GRID_WIDTH;
      if( j < 0 ) j += GRID_HEIGHT;
      else if( j >= GRID_HEIGHT ) j -= GRID_HEIGHT;
      c.x = i;
      c.y = j;
      _i = GetIndividual(c);
      if( !_i->Alive() ) return _i;
      else _i = NULL;
    }
  }
  return _i;
}

Individual *Grid::GetNeighbor( coord c,sint r ) const {
  _FAST int i,j; // Relative locs
  _FAST int x,y,tx,ty; // Absolute locs
  _FAST int t;   // Temp var.
  _FAST int rstep; // Number of steps in ring
  _FAST Individual *_i = NULL;

  /* This method randomly picks locations within the 
  * radius until a living individual is found or 
  * (2*r+1)^2 guesses are made. */
  t = r;
  x = c.x;
  y = c.y;
  rstep = 2*t+1;
  rstep *= rstep;
  rstep--;
  while( rstep ){
redo:
    i = RAND_INT(1);
    j = RAND_INT(1);
    if( !i ) i = -1;
    if( !j ) j = -1;
    i *= RAND_INT(t);
    j *= RAND_INT(t);
    if( !i && !j ) goto redo;
    tx = x + i;
    ty = y + j;
    if( tx < 0 ) tx += GRID_WIDTH;
    else if( tx >= GRID_WIDTH ) tx -= GRID_WIDTH;
    if( ty < 0 ) ty += GRID_HEIGHT;
    else if( ty >= GRID_HEIGHT ) ty -= GRID_HEIGHT;
    c.x = tx;
    c.y = ty;
    _i = GetIndividual(c);
    if( _i->Alive() ) return _i;
    else _i = NULL;
    rstep--;
  }
  /* This method does a search from closest proximity
  * and outwards. Randomly ccw/cw, and random starting
  * position in each 'ring'. i.e.:
  9 . . . v 
  _ 3 2<1 .
  _ 4 X 8 . 
  _ 5 6 7 .
  _ _ _ . <
  */
  /* 
  _FAST int rrad; // Current ring being searched
  _FAST int side; // Number of steps on a side minus 1
  _FAST int rcnt; // Current step
  _FAST int cw; // Going clockwise?
  rrad = 1;

  cw = RAND_INT(1);
  while( rrad <= (int)r ){
  rcnt = 0;
  rstep = 8*rrad;
  side = 2*rrad;

  t = RAND_INT(rstep-1);
  while( rcnt < rstep ){
  rcnt++;
  switch( t / side ){
  case 0:
  i = (t % side) - rrad;
  j = -rrad;
  break;
  case 1:
  i = rrad;
  j = (t % side) - rrad;
  break;
  case 2:
  i = rrad - (t % side);
  j = rrad;
  break;
  case 3:
  i = -rrad;
  j = rrad - (t % side);
  break;
  }
  tx = x + i;
  ty = y + j;
  if( tx < 0 ) tx += GRID_WIDTH;
  else if( tx >= GRID_WIDTH ) tx -= GRID_WIDTH;
  if( ty < 0 ) ty += GRID_HEIGHT;
  else if( ty >= GRID_HEIGHT ) ty -= GRID_HEIGHT;

  c.x = tx;
  c.y = ty;
  _i = GetIndividual(c);
  if( _i->Alive() ) return _i;

  if( cw ) t++;
  else t--;
  if( t >= rstep ) t = 0;
  else if( t < 0 ) t = rstep-1;
  }
  rrad++;
  } */
  return _i;
}

/*
iList Grid::GetNeighbors( coord c,sint r ) const {
int xmin,xmax,ymin,ymax;
int i,j;
iList l;

i = 2*r + 1;
i *= i;
l.list = new (Individual (*[i]));
l.count = 0;
xmin = c.x - r;
xmax = c.x + r;
ymin = c.y - r;
ymax = c.y + r;
//  cout<<"xmin: "<<xmin<<" ymin: "<<ymin<<" xmax: "<<xmax<<" ymax: "<<ymax<<endl;
if( xmin < 0 ) xmin += GRID_WIDTH;
if( ymin < 0 ) ymin += GRID_HEIGHT;
if( xmax >= GRID_WIDTH ) xmax -= GRID_WIDTH;
if( ymax >= GRID_HEIGHT ) ymax -= GRID_HEIGHT;
xmax++;
ymax++;
//  cout<<"xmin: "<<xmin<<" ymin: "<<ymin<<" xmax: "<<xmax<<" ymax: "<<ymax<<endl;
i = xmin;
do {
if( i >= GRID_WIDTH ) i -= GRID_WIDTH;
//    cout << endl;
j = ymin;
do {
if( j >= GRID_HEIGHT ) j -= GRID_HEIGHT;
//      cout<<"i: "<<i<<" j: "<<j<<" "<<map[i][j].Alive()<<endl;
if( c.x == i && c.y == j ){
// Do nothing...
}
else {
if( (map[i][j])->Alive() ){
l.list[l.count] = (map[i][j]);
l.count++;
}
}
j++;
} while( j != ymax );
i++;
//    cout << "i: " << i << endl;
} while( i != xmax );
return l;
} */
// ######################################################


void Grid::PrintGrid( ) const {
  int i,j;
  for( i=0;i<GRID_HEIGHT;i++ ){
    cout << endl;
    for( j=0;j<GRID_WIDTH;j++ ){
      map[j][i]->Print();
    }
  }
  cout << endl;
}

double Grid::AverageFitness( ) const {
  _FAST iList l = GetIndividualList( );
  _FAST sint i;
  _FAST double f = 0.0;
  for( i=0;i<l.count;i++ ){
    f += l.list[i]->Fitness();
  }
  if( l.count ) f /= l.count;
  else {
    delete [] l.list;
    return 0.0;
  }
  delete [] l.list;
  return f;
}

double Grid::AverageBaseFitness( ) const {
  _FAST iList l = GetIndividualList( );
  _FAST sint i;
  _FAST double f = 0.0;
  for( i=0;i<l.count;i++ ){
    f += l.list[i]->BaseFitness();
  }
  if( l.count ) f /= l.count;
  else {
    delete [] l.list;
    return 0.0;
  }
  delete [] l.list;
  return f;
}

// ######################################################

void Grid::Swap( coord a,coord b ){
  Individual *ia = map[a.x][a.y];
  Individual *s;
  map[a.x][a.y] = map[b.x][b.y];
  map[b.x][b.y] = ia;
  s = GetIndividual(a);
  s->Move(a);
  s = GetIndividual(b);
  s->Move(b);
}
