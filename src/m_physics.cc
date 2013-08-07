/* Copyright (C) 2002-2012
   Patrick Eriksson <Patrick.Eriksson@chalmers.se>
   Stefan Buehler   <sbuehler@ltu.se>
                            
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



/*===========================================================================
  ===  File description
  ===========================================================================*/

/*!
  \file   m_physics.cc
  \author Patrick Eriksson <Patrick.Eriksson@chalmers.se>
  \date   2002-08-20 

  \brief  Workspace methods of physical character.

  This file includes workspace methods for operations that have some
  connection to basic physics. Example of methods are:  <br>
  1. Setting WSV to hold blackbody radiation. <br>
  2. Conversion to brightness temperature.

  These functions are listed in the doxygen documentation as entries of the
  file auto_md.h.
*/



/*===========================================================================
  === External declarations
  ===========================================================================*/

#include "arts.h"
#include "auto_md.h"
#include "check_input.h"
#include "logic.h"
#include "math_funcs.h"
#include "messages.h"
#include "physics_funcs.h"

extern const Numeric COSMIC_BG_TEMP;
extern const Numeric TEMP_0_C;


/*===========================================================================
  === The functions (in alphabetical order)
  ===========================================================================*/


/* Workspace method: Doxygen documentation will be auto-generated */
void blackbody_radiationPlanck(
           Vector&   blackbody_radiation,
     const Vector&   f,
     const Numeric&  t,
     const Verbosity&)
{
  const Index   n = f.nelem();

  blackbody_radiation.resize(n);

  for( Index i=0; i<n; i++ )
    { blackbody_radiation[i] = planck( f[i], t ); }
}


/* Workspace method: Doxygen documentation will be auto-generated */
void complex_refr_indexWaterLiebe93(
                           Matrix&         complex_refr_index,
                           const Vector&   f_grid,
                           const Numeric&  t,
                           const Verbosity& verbosity)
{
  CREATE_OUT3;
  
  chk_if_in_range( "t", t, TEMP_0_C, TEMP_0_C+100 );
  chk_if_in_range( "min of f_grid", min(f_grid), 10e9, 1000e9 );
  chk_if_in_range( "max of f_grid", max(f_grid), 10e9, 1000e9 );

  out3 << "  Sets *complex_n* to model properties of liquid water,\n"
       << "  according to Liebe 1993\n";
  out3 << "     temperature      : " << t << " K.\n";

  const Index   nf = f_grid.nelem();

  complex_refr_index.resize( nf, 2 );

  // Implementation following epswater93.m (by C. Mätzler), part of Atmlab,
  // but numeric values strictly following the paper version (146, not 146.4)
  const Numeric   theta = 1 - 300 / t;
  const Numeric   e0    = 77.66 - 103.3 * theta;
  const Numeric   e1    = 0.0671 * e0;
  const Numeric   f1    = 20.2 + 146 * theta + 316 * theta * theta;
  const Numeric   e2    = 3.52;  
  const Numeric   f2    = 39.8 * f1;

  for( Index iv=0; iv<nf; iv++ )
    { 
      const Complex  ifGHz( 0.0, f_grid[iv]/1e9 );
          
      Complex n = sqrt( e2 + (e1-e2) / (Numeric(1.0)-ifGHz/f2) + 
                             (e0-e1) / (Numeric(1.0)-ifGHz/f1) );
    
      complex_refr_index(iv,0) = n.real();
      complex_refr_index(iv,1) = n.imag();
    }
}


/* Workspace method: Doxygen documentation will be auto-generated */
void MatrixCBR(// WS Output:
               Matrix&   m,
               // WS Input:
               const Index&    stokes_dim,
               // WS Generic Input:
               const Vector&   f,
               const Verbosity&)
{
  const Index n = f.nelem();

  if( n == 0 )
    throw runtime_error( "The given frequency vector is empty." );

  m.resize(n,stokes_dim);
  m = 0;

  for( Index i=0; i<n; i++ )
    { m(i,0) = planck( f[i], COSMIC_BG_TEMP ); }
}


/* Workspace method: Doxygen documentation will be auto-generated */
void MatrixPlanck(// WS Output:
                  Matrix&   m,
                  // WS Input:
                  const Index&    stokes_dim,
                  // WS Generic Input:
                  const Vector&   f,
                  const Numeric&  t,
                  const Verbosity& verbosity)
{
  CREATE_OUT2;
  
  const Index n = f.nelem();

  if( n == 0 )
    throw runtime_error( "The given frequency vector is empty." );

  out2 << "  Setting blackbody radiation for a temperature of " << t << " K.\n";

  m.resize(n,stokes_dim);
  m = 0;

  for( Index i=0; i<n; i++ )
    { m(i,0) = planck( f[i], t ); }
}




/* Workspace method: Doxygen documentation will be auto-generated */
void MatrixUnitIntensity(// WS Output:
                         Matrix&   m,
                         // WS Input:
                         const Index&    stokes_dim,
                         // WS Generic Input:
                         const Vector&   f,
                         const Verbosity& verbosity)
{
  CREATE_OUT2;
  
  const Index n = f.nelem();

  if( n == 0 )
    throw runtime_error( "The given frequency vector is empty." );

  out2 << "  Setting unpolarised radiation with an intensity of 1.\n";

  m.resize(n,stokes_dim);
  m = 0;

  for( Index i=0; i<n; i++ )
    { m(i,0) = 1.0; }
}


/* Workspace method: Doxygen documentation will be auto-generated */
void ParticleRefractiveIndexWaterLiebe93(// Generic output:
                                         GriddedField3& scat_ref_index,
                                         // Generic input:
                                         const Vector& scat_f_grid,
                                         const Vector& scat_t_grid,
                                         const Verbosity& verbosity)
{
    const Index nf = scat_f_grid.nelem();
    const Index nt = scat_t_grid.nelem();

    scat_ref_index.resize(nf, nt, 2);
    scat_ref_index.set_grid_name(0, "Frequency");
    scat_ref_index.set_grid(0, scat_f_grid);
    scat_ref_index.set_grid_name(1, "Temperature");
    scat_ref_index.set_grid(1, scat_t_grid);
    scat_ref_index.set_grid_name(2, "Complex");
    scat_ref_index.set_grid(2, MakeArray<String>("real", "imaginary"));

    Matrix complex_n;
    for (Index t = 0; t < nt; ++t)
    {
        complex_refr_indexWaterLiebe93(complex_n, scat_f_grid, scat_t_grid[t], verbosity);
        scat_ref_index.data(joker, t, joker) = complex_n;
    }
}


#ifdef ENABLE_REFICE
/* Workspace method: Doxygen documentation will be auto-generated */
void ParticleRefractiveIndexIceWarren84(// Generic output:
                                        GriddedField3& scat_ref_index,
                                        // Generic input:
                                        const Vector& scat_f_grid,
                                        const Vector& scat_t_grid,
                                        const Verbosity&)
{
    extern const Numeric SPEED_OF_LIGHT;
    const Index nf = scat_f_grid.nelem();
    const Index nt = scat_t_grid.nelem();

    // Frequency must be between 0.0443 to 8.600E+06 microns
    const Numeric f_min = 0.0443 * SPEED_OF_LIGHT;
    const Numeric f_max = 8.6e6 * SPEED_OF_LIGHT;
    chk_if_in_range("min of scat_f_grid", min(scat_f_grid), f_min, f_max);
    chk_if_in_range("max of scat_f_grid", max(scat_f_grid), f_min, f_max);

    // Temperature must be between 213.16 to 272.16 K
    const Numeric t_min = 213.16;
    const Numeric t_max = 272.16;
    chk_if_in_range("min of scat_t_grid", min(scat_t_grid), t_min, t_max);
    chk_if_in_range("max of scat_t_grid", max(scat_t_grid), t_min, t_max);

    scat_ref_index.resize(nf, nt, 2);
    scat_ref_index.set_grid_name(0, "Frequency");
    scat_ref_index.set_grid(0, scat_f_grid);
    scat_ref_index.set_grid_name(1, "Temperature");
    scat_ref_index.set_grid(1, scat_t_grid);
    scat_ref_index.set_grid_name(2, "Complex");
    scat_ref_index.set_grid(2, MakeArray<String>("real", "imaginary"));

    Complex n;
#pragma omp parallel for                 \
  if (!arts_omp_in_parallel() && nf > 1) \
  private(n)
    for (Index f = 0; f < nf; ++f)
        for (Index t = 0; t < nt; ++t)
        {
            n = refice_(scat_f_grid[f] / SPEED_OF_LIGHT, scat_t_grid[t]);
            scat_ref_index.data(f, t, 0) = n.real();
            scat_ref_index.data(f, t, 1) = n.imag();
        }
}

#else

/* Workspace method: Doxygen documentation will be auto-generated */
void ParticleRefractiveIndexIceWarren84(// Generic output:
                                        GriddedField3&,
                                        // Generic input:
                                        const Vector&,
                                        const Vector&,
                                        const Verbosity&)
{
    throw std::runtime_error("ARTS was compiled with Fortran support.");
}

#endif
