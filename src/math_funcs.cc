/* Copyright (C) 2000 Patrick Eriksson <patrick@rss.chalmers.se>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

/*-----------------------------------------------------------------------
FILE:      math_funcs.cc

INCLUDES:  This file contains basic mathematical functions.

           The functions are of different types:
	    1. Element-wise application of common scalar functions 
            2. Boolean functions
	    3. Creation of common vectors
	    4. Interpolation routines
	    5. Integration routines
            6. Conversion between vector and matrix types

FUNCTIONS: sqrt, exp, log, min, max, first, last 
           any
           linspace, nlinspace and nlogspace
           interp_lin, interp_lin_row
           integr_lin
           to_matrix, to_vector

HISTORY:   27.06.1999 Created by Patrick Eriksson.
	   23.03.2000 Stefan Buehler: Adapted to new ARRAY<>, VECTOR and
	              MATRIX types.
           07.04.2000 Patrick Eriksson: Added text for doc++ and nlogspace.
           12.04.2000 Patrick Eriksson: Allowed interpolation of decreasing 
                      grids.
-----------------------------------------------------------------------*/

#include "arts.h"
#include "math_funcs.h"



//
// Basic mathematical vector and vector functions:
//   Vectors: SQRT, EXP, LOG, MIN, MAX, FIRST and LAST
//   Matrices: EXP and LOG
//

void sqrt( VECTOR& y, const VECTOR& x )   // vector parameter version
{
  int n = x.dim();
  y.newsize(n);
  for ( int i=1; i<=n; i++ )
    y(i) = sqrt(x(i));
}

VECTOR sqrt( const VECTOR& x )            // vector return version
{
  VECTOR y;
  sqrt( y, x );
  return y; 
}

void exp( VECTOR& y, const VECTOR& x )   // vector parameter version
{
  int n = x.dim();
  y.newsize(n);
  for ( int i=1; i<=n; i++ )
    y(i) = exp(x(i));
}

VECTOR exp( const VECTOR& x )            // vector return version
{
  VECTOR y;
  exp( y, x );
  return y; 
}

void exp( MATRIX& Y, const MATRIX& X )   // matrix parameter version
{
  int m = X.dim(1);
  int n = X.dim(2);
  Y.newsize(m,n);
  for ( int i=1; i<=m; i++ ) {
    for ( int j=1; j<=n; j++ )
      Y(i,j) = exp(X(i,j)); }
}

MATRIX exp( const MATRIX& X )            // matrix return version
{
  MATRIX Y;
  exp( Y, X );
  return Y; 
}

void log( VECTOR& y, const VECTOR& x )   // vector parameter version
{
  int n = x.dim();
  y.newsize(n);
  for ( int i=1; i<=n; i++ )
    y(i) = log(x(i));
}

VECTOR log( const VECTOR& x )            // vector return version
{
  VECTOR y;
  log( y, x );
  return y; 
}

void log( MATRIX& Y, const MATRIX& X )   // matrix parameter version
{
  int m = X.dim(1);
  int n = X.dim(2);
  Y.newsize(m,n);
  for ( int i=1; i<=m; i++ ) {
    for ( int j=1; j<=n; j++ )
      Y(i,j) = log(X(i,j)); }
}

MATRIX log( const MATRIX& X )            // matrix return version
{
  MATRIX Y;
  log( Y, X );
  return Y; 
}

Numeric min( const VECTOR& x )
{
  int n = x.dim();
  Numeric y=x(1);
  for ( int i=2; i<=n; i++ )
  {
    if ( x(i) < y )
      y = x(i);
  }
  return y; 
}

Numeric min( const MATRIX& A )
{
  int r = A.dim(1);
  int c = A.dim(2);

  Numeric y=A(1,1);

  for ( int i=2; i<=r; i++ )
    for ( int s=2; s<=c; s++ )
      {
	if ( A(i,s) < y )
	  y = A(i,s);
      }
  return y; 
}

Numeric max( const VECTOR& x )
{
  int n = x.dim();
  Numeric y=x(1);
  for ( int i=2; i<=n; i++ )
  {
    if ( x(i) > y )
      y = x(i);
  }
  return y; 
}

Numeric max( const MATRIX& A )
{
  int r = A.dim(1);
  int c = A.dim(2);

  Numeric y=A(1,1);

  for ( int i=2; i<=r; i++ )
    for ( int s=2; s<=c; s++ )
      {
	if ( A(i,s) > y )
	  y = A(i,s);
      }
  return y; 
}


Numeric first( const VECTOR& x )
{
  return x(1); 
}

Numeric last( const VECTOR& x )
{
  return x(x.dim()); 
}



//
// Logical vector functions
//   ANY
//

bool any( const ARRAY<int>& x )         // True if any element of x != 0
{
  for ( size_t i=1; i<=x.dim(); i++ ) {
    if ( x(i) )
      return true;
  }
  return false;
}



//
// Functions to generate vectors
//   LINSPACE, NLINSPACE, and LOGNSPACE
//   

void linspace(                      
              VECTOR&     x,           
        const Numeric     start,    
        const Numeric     stop,        
        const Numeric     step )
{
  int n = (int) floor( (stop-start)/step ) + 1;
  if ( n<1 )
    n=1;
  x.newsize(n);
  for ( int i=1; i<=n; i++ )
    x(i) = start + (i-1)*step;
}

VECTOR linspace(                 // As above but return version
        const Numeric start, 
        const Numeric stop,  
        const Numeric step )
{
  VECTOR x;
  linspace( x, start, stop, step );
  return x; 
}
      
void nlinspace(         
              VECTOR&     x, 
        const Numeric     start,     
        const Numeric     stop,        
        const int         n )
{
  if ( n<2 )
    throw runtime_error("NLINSPACE: The number of points must be > 1"); 
  x.newsize(n);
  Numeric step = (stop-start)/(n-1) ;
  for ( int i=1; i<=n; i++ )
    x(i) = start + (i-1)*step;
}

VECTOR nlinspace(                 // As above but return version
        const Numeric start, 
        const Numeric stop,  
        const int     n )
{
  VECTOR x;
  nlinspace( x, start, stop, n );
  return x; 
}                     

void nlogspace(         
              VECTOR&     x, 
        const Numeric     start,     
        const Numeric     stop,        
        const int         n )
{
  if ( n<2 )
    throw runtime_error("NLOGSPACE: The number of points must be > 1"); 
  if ( (start<=0) || (stop<=0) )
    throw runtime_error("NLOGSPACE: Only positive numbers are allowed"); 
  x.newsize(n);
  Numeric a = log(start);
  Numeric step = (log(stop)-a)/(n-1) ;
  x(1) = start;
  for ( int i=2; i<n; i++ )
    x(i) = exp(a + (i-1)*step);
  x(n) = stop;
}

VECTOR nlogspace(                 // As above but return version
        const Numeric start, 
        const Numeric stop,  
        const int     n )
{
  VECTOR x;
  nlogspace( x, start, stop, n );
  return x; 
}                     



//
// Interpolation routines.
//   Vectors:  INTERP_LIN
//   Matrices: INTERP_LIN_ROW
//
//   All the functions assume that the interpolation points, XI, are ordered.
//   However, the values can be in increasing or decreasing order.
//

// Local help function to check input grids
//   12.04.00 Patrick Eriksson
//
int interp_check(                  
	const VECTOR&  x,        
        const VECTOR&  xi,
        const size_t   n_y )
{
  size_t  n  = x.dim();
  size_t  ni = xi.dim();
  int     order=1;          // flag for pos. or neg. order, assume pos.

  // Determine the order, -1=decreasing and 1=increasing
  if ( x(1) > x(n) )
    order = -1;

  if ( n < 2 )
    throw runtime_error("Vector length for interpolation must be >= 2");

  if ( n != n_y ) 
    throw runtime_error("Sizes of input data to interpolation do not match");

  if ( (order*xi(1)<order*x(1)) || (order*xi(ni)>order*x(n)) ) 
    {
      ostringstream os;
      os << "Interpolation points must be inside the original range.\n"
	 << "Int.:  xi(1) = " << xi(1) << ", xi(ni) = " << xi(ni) << '\n'
	 << "Orig.: x(1)  = " << x(1)  << ", x(n)   = " << x(n);
      throw runtime_error(os.str());
    }

  for (size_t i=1; i<n; i++ )
  {
    if ( order*x(i+1) < order*x(i) ) 
      throw runtime_error("Original interpolation grid must be ordered");
  }

  for (size_t i=1; i<ni; i++ )
  {
    if ( order*xi(i+1) < order*xi(i) ) 
      throw runtime_error("Interpolation points must be ordered");
  }

  return order;
}
    
void interp_lin(                  // Linear interpolation of a vector
              VECTOR&  yi,
        const VECTOR&  x, 
        const VECTOR&  y, 
        const VECTOR&  xi )
{
  // Check grids and get order of grids
  int order = interp_check( x, xi, y.dim() ); 

  size_t        i, j=1, n=xi.dim();
  Numeric       w;
  yi.newsize(n); 

  for ( i=1; i<=n; i++ )
  {
    for( ;  order*x(j+1) < order*xi(i); j++ ) {}
    w = (xi(i)-x(j)) / (x(j+1)-x(j));
    yi(i) = y(j) + w * (y(j+1)-y(j)); 
  }
}      

VECTOR interp_lin(               // As above but return version
        const VECTOR&  x, 
        const VECTOR&  y, 
        const VECTOR&  xi )
{
  VECTOR yi; 
  interp_lin( yi, x, y, xi );
  return yi;
}        

Numeric interp_lin(              // As above but for only one point
        const VECTOR&  x, 
        const VECTOR&  y, 
        const Numeric  xi )
{
  VECTOR yi(1); 
  interp_lin( yi, x, y, xi );
  return yi(1);
}        

void interp_lin_row(    
              MATRIX&  Yi,
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi )
{
  // Check grids and get order of grids
  int order = interp_check( x, xi, Y.dim(2) ); 

  int        k, j=1, n=xi.dim(), nrow=Y.dim(1);
  Numeric    w;
  Yi.newsize( nrow, n ); 

  for (int i=1; i<=n; i++ )
  {
    for( ;  order*x(j+1) < order*xi(i); j++ ) {}
    w = (xi(i)-x(j)) / (x(j+1)-x(j));
    for( k=1; k<=nrow; k++ )
      Yi(k,i) = Y(k,j) + w * (Y(k,j+1)-Y(k,j)); 
  }
}        

MATRIX interp_lin_row(       // As above but return version
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi )
{
  MATRIX Yi; 
  interp_lin_row( Yi, x, Y, xi );
  return Yi;
}        


void interp_lin_col(    
              MATRIX&  Yi,
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi )
{
  // Check grids and get order of grids
  int order = interp_check( x, xi, Y.dim(1) ); 

  int        k, j=1, n=xi.dim(), ncol=Y.dim(2);
  Numeric    w;
  Yi.newsize( n, ncol ); 

  for (int i=1; i<=n; i++ )
  {
    for( ;  order*x(j+1) < order*xi(i); j++ ) {}
    w = (xi(i)-x(j)) / (x(j+1)-x(j));
    for( k=1; k<=ncol; k++ )
      Yi(i,k) = Y(j,k) + w * (Y(j+1,k)-Y(j,k)); 
  }
}        

MATRIX interp_lin_col(       // As above but return version
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi )
{
  MATRIX Yi; 
  interp_lin_col( Yi, x, Y, xi );
  return Yi;
}        



//
// Integration functions for vectors and matrices
//    INTEGR_LIN
//

Numeric integr_lin(                // Integrates Y over X assuming that Y is
        const VECTOR&  x,          // linear between the given points
        const VECTOR&  y )
{
  size_t i, n = x.dim();
  Numeric w=0.0; 

  if ( n < 2 )
    throw runtime_error("INTEGR_LIN: Vector length must be >= 2");
  if ( n != y.dim() )
    throw runtime_error("INTEGR_LIN: Sizes of input data do not match");

  for( i=1; i<n; i++ )
    w += (x(i+1)-x(i)) * (y(i)+y(i+1))/2.0;
  return w;
}

void integr_lin(                    // As above but parameter version
              Numeric&  w,
        const VECTOR&   x,  
        const VECTOR&   y ) 
{
  w = integr_lin( x, y );
}

void integr_lin(                // Integrates the rows of M assuming that
              MATRIX&  W,       // the functions are linear between the
        const VECTOR&  x,       // given points
        const MATRIX&  M )   
{
  size_t i,j,rows= M.dim(1), cols= M.dim(2);
  Numeric w; 
  W.newsize(rows,1);
  W = 0.0;

  if ( cols < 2 )
    throw runtime_error("Vector length for integration must be >= 2");
  if ( cols != x.dim() )
    throw runtime_error("Sizes of input data for integration do not match");

  for ( i=1; i<cols; i++ ) 
  {
    w = ( x(i+1) - x(i) ) / 2.0;
    for ( j=1; j<=rows; j++ )
      W(j,1) += w * ( M(j,i) + M(j,i+1) );
  }
}

MATRIX integr_lin(              // As above but return version
        const VECTOR&  x,  
        const MATRIX&  M ) 
{
  MATRIX W;
  integr_lin( W, x, M );
  return W;
}



//
// Vector to matrix conversion:
//   to_matrix
//

void to_matrix(MATRIX& W, const VECTOR& x)
{
  // FIXME: I'm sure this can be made more efficient when TNT has more
  // functionality.  
  W.newsize(x.dim(),1);
  for (size_t i=1; i<=x.dim() ; ++i)
    {
      W(i,1) = x(i);
    }
}

MATRIX to_matrix(const VECTOR& x)
{
  MATRIX W;
  to_matrix(W,x);
  return W;
}



//
// Matrix to vector conversion:
//   to_vector
//

void to_vector(VECTOR& x, const MATRIX& W)
{
  // FIXME: I'm sure this can be made more efficient when TNT has more
  // functionality.

  // Check if one of the dimensions of W is 1:
  if ( 1 == W.dim(2) )
    {
      col(x,1,W);
    }
  else if ( 1 == W.dim(1) )
    {
      row(x,1,W);
    }
  else
    throw runtime_error("You tried to convert a matrix to a vector,\n"
			"but none of the dimensions is 1.");
}

VECTOR to_vector(const MATRIX& W)
{
  VECTOR x;
  to_vector(x,W);
  return x;
}


void row(VECTOR& x,
	 size_t i,
	 const MATRIX& A)
{
  // Make sure that i is legal:
  assert ( i >  0        );
  assert ( i <= A.dim(1) );

  const size_t n = A.dim(2);
  x.newsize(n);
  for ( size_t j=1; j<=n; ++j )
    {
      x(j) = A(i,j);
    }
}

VECTOR row(size_t i,
	   const MATRIX& A)
{
  VECTOR x;
  row(x,i,A);
  return x;
}

void col(VECTOR& x,
	 size_t i,
	 const MATRIX& A)
{
  // Make sure that i is legal:
  assert ( i >  0        );
  assert ( i <= A.dim(2) );

  const size_t n = A.dim(1);
  x.newsize(n);
  for ( size_t j=1; j<=n; ++j )
    {
      x(j) = A(j,i);
    }
}

VECTOR col(size_t i,
	   const MATRIX& A)
{
  VECTOR x;
  col(x,i,A);
  return x;
}


void row(MATRIX& X,
	 size_t i,
	 size_t k,
	 const MATRIX& A)
{
  // Make sure that i and k are legal:
  assert ( i >  0        );
  assert ( i <= A.dim(1) );
  assert ( k >  0        );
  assert ( k <= A.dim(1) );
  assert ( k >= i        );

  const size_t n = A.dim(2);
  const size_t m = k-i + 1;
  X.newsize(m,n);
  for ( size_t j=0; j<n; ++j )
    for ( size_t l=0; l<m; ++l )
      {
	X[l][j] = A(i+l,j+1);
      }
}

MATRIX row(size_t i,
	    size_t k,
	    const MATRIX& A)
{
  MATRIX X;
  row(X,i,k,A);
  return X;
}

void col(MATRIX& X,
	  size_t i,
	  size_t k,
	  const MATRIX& A)
{
  // Make sure that i and k are legal:
  assert ( i >  0        );
  assert ( i <= A.dim(2) );
  assert ( k >  0        );
  assert ( k <= A.dim(2) );
  assert ( k >= i        );

  const size_t n = A.dim(1);
  const size_t m = k-i + 1;
  X.newsize(n,m);
  for ( size_t j=0; j<n; ++j )
    for ( size_t l=0; l<m; ++l )
      {
	X[j][l] = A(j+1,i+l);
      }
}

MATRIX col(size_t i,
	    size_t k,
	    const MATRIX& A)
{
  MATRIX X;
  col(X,i,k,A);
  return X;
}

