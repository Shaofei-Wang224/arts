
/* Copyright (C) 2003 Cory Davis <cory@met.ed.ac.uk>
                            
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
  === File description 
  ===========================================================================*/

/*!
  \file   m_montecarlo.cc
  \author Cory Davis <cory@met.ed.ac.uk>
  \date   2003-06-19 

  \brief  Workspace functions for the solution of cloud-box radiative transfer 
by Monte Carlo methods.  All of these functions refer to 3D calculations

  These functions are listed in the doxygen documentation as entries of the
  file auto_md.h.
*/
/*===========================================================================
  === External declarations
  ===========================================================================*/

#include "messages.h"
#include "arts.h"
#include "ppath.h"
#include "matpackI.h"
#include "special_interp.h"
#include "check_input.h"
#include <stdexcept>
#include <cmath>
#include "rte.h"
#include "lin_alg.h"
#include "auto_md.h"
#include "logic.h"
#include "physics_funcs.h"
#include "xml_io.h"
#include "montecarlo.h"
#include "rng.h"
#include <ctime>
#include <fstream>

extern const Numeric DEG2RAD;
extern const Numeric RAD2DEG;
extern const Numeric PI;

/*===========================================================================
  === The functions (in alphabetical order)
  ===========================================================================*/


//! Cloudbox_ppath_calc
/*! 
  This function performs the same task as ppath_calc, except inside the
  cloudbox.  It has been derived from the clear sky version.  See the 
  online help (arts -d FUNCTION_NAME) for a description of parameters.
   
\author Cory Davis
\date 2003-06-19
  
*/
/*
void Cloudbox_ppathCalc(
        // WS Output:
              Ppath&          ppath,
              Ppath&          ppath_step,
        // WS Input:
        const Agenda&         ppath_step_agenda,
        const Index&          atmosphere_dim,
        const Vector&         p_grid,
        const Vector&         lat_grid,
        const Vector&         lon_grid,
        const Tensor3&        z_field,
        const Matrix&         r_geoid,
        const Matrix&         z_surface,
        const ArrayOfIndex&   cloudbox_limits,
        const Vector&         rte_pos,
        const Vector&         rte_los)
{
  // This function is a WSM but it is normally only called from RteCalc. 
  // For that reason, this function does not repeat input checks that are
  // performed in RteCalc, it only performs checks regarding the sensor 
  // position and LOS.

  //--- Check input -----------------------------------------------------------

  // Sensor position and LOS
  //
  chk_vector_length( "rte_pos", rte_pos, atmosphere_dim );
  chk_if_over_0( "sensor radius", rte_pos[0] );
  if( atmosphere_dim < 3 )
    {
        ostringstream os;
        os << "cloudbox_ppath_calc only works for a 3D atmosphere";
        throw runtime_error( os.str() );
    }
  else
    {
      chk_if_in_range( "sensor latitude", rte_pos[1], -90, 90 );
      chk_if_in_range( "sensor longitude", rte_pos[2], -360, 360 );
      chk_vector_length( "rte_los", rte_los, 2 );
      chk_if_in_range( "sensor zenith angle", rte_los[0], 0, 180 );
      chk_if_in_range( "sensor azimuth angle", rte_los[1], -180, 180 );
    }
  
  //--- End: Check input ------------------------------------------------------


  // Some messages
  out2 << "  -------------------------------------\n";
  out2 << "  sensor radius          : " << rte_pos[0]/1e3 << " km\n";
  if( atmosphere_dim == 3 )
    out2 << "  sensor longitude       : " << rte_pos[2] << "\n";
  out2 << "  sensor zenith angle    : " << rte_los[0] << "\n";
  if( atmosphere_dim == 3 )
    out2 << "  sensor azimuth angle   : " << rte_los[1] << "\n";
  
  
  
  // Initiate the partial Ppath structure. 

  //
  cloudbox_ppath_start_stepping( ppath_step, atmosphere_dim, p_grid, lat_grid, 
                      lon_grid, z_field, r_geoid, z_surface, rte_pos, rte_los );

  out2 << "  -------------------------------------\n";

  // Perform propagation path steps until the starting point is found, which
  // is flagged by ppath_step by setting the background field.
  //
  // The results of each step, returned by ppath_step_agenda as a new 
  // ppath_step, are stored as an array of Ppath structures.
  //
  Array<Ppath>   ppath_array;
  ppath_array.push_back( ppath_step );
  // 
  Index   np = ppath_step.np;   // Counter for number of points of the path
  Index   istep = 0;            // Counter for number of steps
//

  
  while( !ppath_what_background( ppath_step ) )
    {

      // Call ppath_step agenda. 
      // The new path step is added to *ppath_array* last in the while block
      //
      istep++;
      //
      ppath_step_agenda.execute( true );

      // Before everything is tested carefully, we consider more than 1000
      // path points to be an indication on that the calcululations have
      // got stuck in an infinite loop.
      if( istep > 5000 )
        {
          throw logic_error(
             "5000 path points have been reached. Is this an infinite loop?" );
        }
      //     cout << "istep = " << istep << "\n";
      // Number of points in returned path step
      const Index n = ppath_step.np;

      // Increase the total number
      np += n - 1;
      
      // Check if there is an intersection with the cloud box boundary,
        //remembering that this function will only be called within the
        //cloud box
      
      double ipos = double( ppath_step.gp_p[n-1].idx ) + 
                                                    ppath_step.gp_p[n-1].fd[0];
      if( ipos <= double( cloudbox_limits[0] )  || 
                                        ipos >= double( cloudbox_limits[1] ) )
        { ppath_set_background( ppath_step, 3 ); }
      else   
          {
          ipos = double( ppath_step.gp_lat[n-1].idx ) + 
                                              ppath_step.gp_lat[n-1].fd[0];
          if( ipos <= double( cloudbox_limits[2] )  || 
                                    ipos >= double( cloudbox_limits[3] ) )
             { ppath_set_background( ppath_step, 3 ); }
          else
         {
           ipos = double( ppath_step.gp_lon[n-1].idx ) + 
                                              ppath_step.gp_lon[n-1].fd[0];
              if( ipos <= double( cloudbox_limits[4] )  || 
                                    ipos >= double( cloudbox_limits[5] ) )
                    { ppath_set_background( ppath_step, 3 ); } 
            }
        }
        
      // Put new ppath_step in ppath_array
      ppath_array.push_back( ppath_step );
      //     cout <<"what background? "<< ppath_what_background( ppath_step )<< "\n";
    } // End path steps
  
 
  // Combine all structures in ppath_array to form the return Ppath structure.
  //
  ppath_init_structure( ppath, atmosphere_dim, np );
  //
  np = 0;   // Now used as counter for points moved to ppath
  //
  for( Index i=0; i<ppath_array.nelem(); i++ )
    {
      // For the first structure, the first point shall be included, but the
      // first structure can also be empty. 
      // For later structures, the first point shall not be included, but
      // there will always be at least two points.
      // Only the first structure can be empty.

      Index n = ppath_array[i].np;

      if( n )
        {
          // First index to include
          Index i1 = 1;
          if( i == 0 )
            { i1 = 0; }
          else
            { assert( n > 1 ); }

          // Vectors and matrices that can be handled by ranges.
          ppath.z[ Range(np,n-i1) ] = ppath_array[i].z[ Range(i1,n-i1) ];
          ppath.pos( Range(np,n-i1), joker ) = 
                                   ppath_array[i].pos( Range(i1,n-i1), joker );
          ppath.los( Range(np,n-i1), joker ) = 
                                   ppath_array[i].los( Range(i1,n-i1), joker );

          // For i==1, there is no defined l_step. For higher i, all 
          // values in l_step shall be copied.
          if( i > 0 )
            { ppath.l_step[ Range(np-1,n-1) ] = ppath_array[i].l_step; }

          // Grid positions must be handled by a loop
          for( Index j=i1; j<n; j++ )
            { ppath.gp_p[np+j-i1] = ppath_array[i].gp_p[j]; }
          if( atmosphere_dim >= 2 )
            {
              for( Index j=i1; j<n; j++ )
                { ppath.gp_lat[np+j-i1] = ppath_array[i].gp_lat[j]; }
            }
          if( atmosphere_dim == 3 )
            {
              for( Index j=i1; j<n; j++ )
                { ppath.gp_lon[np+j-i1] = ppath_array[i].gp_lon[j]; }
            }

          // Fields just set once
          if( ppath_array[i].tan_pos.nelem() )
            {
              ppath.tan_pos.resize( ppath_array[i].tan_pos.nelem() );
              ppath.tan_pos               = ppath_array[i].tan_pos; 
            }
          if( ppath_array[i].geom_tan_pos.nelem() )
            {
              ppath.geom_tan_pos.resize( ppath_array[i].tan_pos.nelem() );
              ppath.geom_tan_pos          = ppath_array[i].geom_tan_pos; 
            }

          // Increase number of points done
          np += n - i1;
         
        }
    }  
  ppath.method     = ppath_step.method;
  ppath.refraction = ppath_step.refraction;
  ppath.constant   = ppath_step.constant;
  ppath.background = ppath_step.background;

  

  out3 << "  number of path steps  : " << istep           << "\n";
  out3 << "  number of path points : " << ppath.z.nelem() << "\n";


  // If refraction has been considered, make a simple check that the
  // refraction at the top of the atmosphere is sufficiently close to 1.
  if( ppath.refraction  &&  min( z_field(z_field.npages()-1,0,0) ) < 60e3 )
    {
      out2 << "  *** WARNING****\n" 
           << "  The calculated propagation path can be inexact as the "
           << "atmosphere\n  only extends to " 
           <<  min( z_field(z_field.npages()-1,0,0) ) << " km. \n" 
           << "  The importance of this depends on the observation "
           << "geometry.\n  It is recommended that the top of the atmosphere "
           << "is not below 60 km.\n";
    }
}
*/

//! scat_iPutMonteCarlo
/*! 
calculates interface Tensors scat_i_p, scat_i_lat, and scat_i_lon.  This is
the equivalent of scat_iPut for use after ScatteringMonteCarlo and before a final
call to RteCalc.  See the online help (arts -d FUNCTION_NAME) for a description 
of parameters.
   
\author Cory Davis
\date 2003-06-30
  
*/

void scat_iPutMonteCarlo(
                         Tensor7& scat_i_p,
                         Tensor7& scat_i_lat,
                         Tensor7& scat_i_lon,
                         const Matrix& i_rte,
                         const Index& stokes_dim,
                         const Vector& f_grid,
                         const ArrayOfIndex& cloudbox_limits,
                         const Vector& scat_za_grid,
                         const Vector& scat_aa_grid
                         )
{

  Vector I = i_rte(0,joker);
   Index Nf = f_grid.nelem();
   Index Np_cloud = cloudbox_limits[1] - cloudbox_limits[0] + 1;

   Index Nza = scat_za_grid.nelem();
   
   Index Ni = stokes_dim;
   Index Nlat_cloud = cloudbox_limits[3] - cloudbox_limits[2] + 1; 
   Index Nlon_cloud = cloudbox_limits[5] - cloudbox_limits[4] + 1;
   Index Naa = scat_aa_grid.nelem();
   
   scat_i_p.resize(Nf, 2, Nlat_cloud, Nlon_cloud, Nza, Naa, Ni);
   scat_i_lat.resize(Nf, Np_cloud, 2, Nlon_cloud, Nza, Naa, Ni);
   scat_i_lon.resize(Nf, Np_cloud, Nlat_cloud, 2, Nza, Naa, Ni);


  for(Index i = 0;i<stokes_dim;i++)
    {
      scat_i_p(joker,joker,joker,joker,joker,joker,i)=I[i];
      scat_i_lat(joker,joker,joker,joker,joker,joker,i)=I[i];
      scat_i_lon(joker,joker,joker,joker,joker,joker,i)=I[i];
    }
}








//! ScatteringMonteCarlo
/*! 
 
   This workspace method uses a Monte Carlo method to calculate the monochromatic
   radiance leaving the cloudbox in one direction.  
   The theoretical basis for this algorithm can be found at 
   http://www.met.ed.ac.uk/~cory/davis_MRad04.pdf
   
   See the online help 
   (arts -d FUNCTION_NAME) for a description of parameters.
 

   \author Cory Davis
   \date   2003-06-19
*/

void ScatteringMonteCarlo (
                           // WS Output:
                           Ppath&                ppath,
                           Ppath&                ppath_step,
                           Vector&               i_montecarlo_error,
                           Vector&               rte_pos,
                           Vector&               rte_los,
                           //Stuff needed by RteCalc
                           GridPos&              rte_gp_p,
                           GridPos&              rte_gp_lat,
                           GridPos&              rte_gp_lon,
			   // Matrix&               i_space,
                           //Matrix&               surface_emission,
                           //Matrix&               surface_los, 
                           //Tensor4&              surface_refl_coeffs,
                           Matrix&               iy,
                           //Vector&               scat_za_grid,
                           //Vector&               scat_aa_grid,
                           Numeric&              rte_pressure,
                           Numeric&              rte_temperature,
                           Vector&               rte_vmr_list,
                           //Other Stuff
                           Tensor3&              ext_mat,
                           Matrix&               abs_vec,
                           Index&                f_index,
                          
                           // WS Input:
                           const Agenda&         ppath_step_agenda,
                           const Index&          atmosphere_dim,
                           const Vector&         p_grid,
                           const Vector&         lat_grid,
                           const Vector&         lon_grid,
                           const Tensor3&        z_field,
                           const Matrix&         r_geoid,
                           const Matrix&         z_surface,
                           const ArrayOfIndex&   cloudbox_limits,
                           const Index&          stokes_dim,
                           //Stuff needed by RteCalc
                           const Agenda&         rte_agenda,
                           const Agenda&         iy_space_agenda,
                           const Agenda&         iy_surface_agenda,
                           const Tensor3&        t_field,
                           const Vector&         f_grid,
                           //Stuff needed by TArrayCalc
                           const Agenda& opt_prop_gas_agenda,
                           const Agenda& scalar_gas_absorption_agenda,
                           const Tensor4&   vmr_field,
                           //Other Stuff
                           const ArrayOfSingleScatteringData& scat_data_mono,
                           const Tensor4& pnd_field,
                           // Control Parameters:
                           const Index& maxiter,
                           const Index& rng_seed,
                           const Index& record_ppathcloud,
                           const Index& record_ppath,
                           const Index& silent,
                           const Index& record_histdata,
                           const String& histdata_filename
                           )

{               
  //INTERNAL DECLARATIONS/////////////////////////////////////////////////
  Matrix identity(stokes_dim,stokes_dim,0.0); //The identity matrix
  for (Index i=0; i<stokes_dim; i++){identity(i,i)=1.0;}

  Matrix Q(stokes_dim,stokes_dim);//defined in eq 12 of ref1
  Matrix T(stokes_dim,stokes_dim);//evolution operator (symbol O in ref1)
  Matrix K(stokes_dim,stokes_dim);//extinction matrix
  bool keepgoing; // flag indicating whether to stop tracing a photons path
  Index scattering_order;       //k in ref1
  Index photon_number;          //i in ref1
  Vector new_rte_los(2);        //Stores new line of sight
  Ppath ppathLOS;               // propagation path in the original line of sight
  Ppath ppathcloud;             //prop. path inside cloud box
  Numeric g;             //Probability density for pathlength sampling
  Numeric pathlength;           //\Delta s in ref1
  ArrayOfMatrix TArrayLOS;//array of evolution operators along 
                                //original line of sight                
  ArrayOfMatrix TArray;//array of evolution operators for higher 
                                //scattering orders
  ArrayOfMatrix ext_matArray;//array of extinction matrices along 
                                //propagation path
  ArrayOfMatrix ext_matArrayLOS;//array of extinction matrices along
                                //original LOS
  ArrayOfVector abs_vecArray;//array of abs. coeff. vectors along 
                                //propagation path
  ArrayOfVector abs_vecArrayLOS;//array of abs. coeff. vectors along 
                                //original LOS
  Vector Isum(stokes_dim,0.0);//Sum of all photon contributions to the Stokes vector
  Vector Isquaredsum(stokes_dim,0.0);//Used to estimate error
  Vector Iboundary(stokes_dim,0.0);//Incoming Stokes vector at the cloudbox boundary
  Vector IboundaryLOScontri(stokes_dim,0.0);//1st term RHS eq 2 ref1.
  Matrix Z(stokes_dim,stokes_dim,0.0);//bulk phase matrix
  Matrix q(stokes_dim,stokes_dim,0.0);//Eqs. 12-13 ref1
  Matrix newQ(stokes_dim,stokes_dim,0.0);//Eq 12 ref1
  Vector cum_l_step;//vector of cumulative distance along a propagation path
  Vector cum_l_stepLOS;// "    "  for the original LOS
  Vector t_ppath;//vector of temperatures along a propagation path
  Vector t_ppathLOS;//" "  " along the original LOS
  Matrix pnd_ppath;//ppath.np by Nptypes matrix of pasrticle number density
  Matrix pnd_ppathLOS;// "   "    "
  ArrayOfGridPos pathlength_gp(1);//for optical properties along a propagation 
                                //path according to pathlength
  Vector K_abs(stokes_dim);//absorption coefficient vector
  Vector I(stokes_dim);//Cloudbox exit Stokes Vector
  i_montecarlo_error.resize(stokes_dim);//Error in Cloudbox exit Stokes vector
  Rng rng;                      //Random Nuimber generator
  Vector I_i(stokes_dim);//photon contribution to Stokes vector
  Vector boundarycontri(stokes_dim);//eq 16 ref1
  Numeric g_los_csc_theta;//eq. 11 ref1 divided by sin(\theta) 
  Numeric albedo;//eq. 9 ref1
  Numeric dist_to_boundary; //Distance to the far boundary of the cloudbox
  Index N_pt=pnd_field.nbooks();//Number of particle types
  Vector pnd_vec(N_pt); //Vector of particle number densities used at each point
  time_t start_time=time(NULL);
  Numeric za_scat;//zenith angle of scattering direction
  Vector Z11maxvector;//Vector holding the maximum phase function for each 
  //particle type. Used in Rejection method for sampling incident direction
  //////////////////////////////////////////////////////////////////////////////
  bool anyptype30=is_anyptype30(scat_data_mono);
  if (anyptype30)
    {
      findZ11max(Z11maxvector,scat_data_mono);
    }
  //If necessary, open file for histogram data output
  ofstream histfile;
  if (record_histdata==1)
    {
      const char* p = histdata_filename.c_str();
      histfile.open(p,ios::out);
    }

  //if rng_seed is < 0, keep time based seed, otherwise...
  if(rng_seed>=0){rng.seed(rng_seed);}
  Agenda iy_cloudbox_agenda;
  Cloudbox_ppath_rteCalc(ppathLOS, ppath, ppath_step, rte_pos, rte_los, 
                         cum_l_stepLOS, TArrayLOS, ext_matArrayLOS, 
                         abs_vecArrayLOS,t_ppathLOS, ext_mat, abs_vec, rte_pressure, 
                         rte_temperature, rte_vmr_list, iy, rte_gp_p, 
                         rte_gp_lat, rte_gp_lon, f_index, pnd_ppathLOS, 
                         ppath_step_agenda, atmosphere_dim, 
                         p_grid, lat_grid, lon_grid, z_field, r_geoid, z_surface, 
                         cloudbox_limits, record_ppathcloud, record_ppath, 
                         opt_prop_gas_agenda, 
                         scalar_gas_absorption_agenda, stokes_dim, t_field, 
                         vmr_field, rte_agenda, iy_space_agenda, 
                         iy_surface_agenda, iy_cloudbox_agenda,f_grid, 0, 0,
			 pnd_field,scat_data_mono);
 
  mult(IboundaryLOScontri,TArrayLOS[TArrayLOS.nelem()-1],iy(0,joker));
  for (Index i = 0;i<stokes_dim;i++){assert(!isnan(IboundaryLOScontri[i]));}
  
  //Begin Main Loop
  for (photon_number=1; photon_number<=maxiter; photon_number++)
    {
      keepgoing=true; //flag indicating whether to continue tracing a photon path
      scattering_order=0;       //scattering order
      Q=identity;               //identity matrix
      I_i=0.0;
      boundarycontri=0.0;
      //while the reversed traced photon path remains in the cloud box
      //
      TArray=TArrayLOS;
      ext_matArray=ext_matArrayLOS;
      abs_vecArray=abs_vecArrayLOS;
      ppathcloud=ppathLOS;
      cum_l_step=cum_l_stepLOS;
      t_ppath=t_ppathLOS;
      pnd_ppath=pnd_ppathLOS;
      dist_to_boundary=cum_l_step[ppathcloud.np-1];
         
      if (silent==0){cout<<"photon_number = "<<photon_number<<"\n";}
      while (keepgoing)
        {
           if (scattering_order>0)
            {
              //We need to calculate a new propagation path. In the future, we may be 
              //able to take some shortcuts here
              //Cloudbox_ppathCalc(ppathcloud,ppath_step,ppath_step_agenda,atmosphere_dim,
              //                   p_grid,lat_grid,lon_grid,z_field,r_geoid,z_surface,
	      //                    cloudbox_limits, rte_pos,rte_los);
	      ppath_calc(ppathcloud,ppath_step,ppath_step_agenda,atmosphere_dim,
			 p_grid,lat_grid,lon_grid,z_field,r_geoid,z_surface,1,
			 cloudbox_limits, rte_pos,rte_los,0);
	      
              if (record_ppathcloud){ppathRecordMC(ppathcloud,"ppathcloud",
                                                   photon_number,scattering_order);}
                              
              cum_l_stepCalc(cum_l_step,ppathcloud);
  
              //Calculate array of transmittance matrices
              TArrayCalc(TArray, ext_matArray, abs_vecArray, t_ppath, ext_mat, abs_vec, 
			 rte_pressure, rte_temperature, 
                         rte_vmr_list, pnd_ppath, ppathcloud, opt_prop_gas_agenda, 
                         scalar_gas_absorption_agenda, stokes_dim, 
                         p_grid, lat_grid, lon_grid, t_field, vmr_field, atmosphere_dim,
                         pnd_field, scat_data_mono);


              /////////////////////////////////////////////////////////////////////
              dist_to_boundary=cum_l_step[ppathcloud.np-1];
         
              //              Iboundary=i_rte(0,joker);
              Sample_ppathlength (pathlength,g,rng,TArray,cum_l_step);
            }
          else
            {
              Sample_ppathlengthLOS (pathlength,g,rng,TArray,cum_l_step);
            }
          assert(cum_l_step.nelem()==ppathcloud.np);
          assert(TArray.nelem()==ppathcloud.np);
          if (pathlength>dist_to_boundary)
            //Then the path has left the cloud box
            {
              assert (scattering_order>0); //scattering/emission should be 
                                           //forced in original line of sight
              //Get incoming//////
              montecarloGetIncoming(iy,rte_pos,rte_los,rte_gp_p,
                        rte_gp_lat,rte_gp_lon,ppath,ppath_step,
			ppath_step_agenda,
                        rte_agenda,iy_space_agenda,iy_surface_agenda,
				    iy_cloudbox_agenda,
                        p_grid,lat_grid,lon_grid,z_field,r_geoid,
                        z_surface,cloudbox_limits,ppathcloud,atmosphere_dim,
                        f_grid,stokes_dim);
              
              if (record_ppath)
                {
                  ppathRecordMC(ppath,"ppath",photon_number,scattering_order);
                }
              
              f_index=0;//For some strange reason f_index is set to -1 in RteStandard
              Iboundary=iy(0,joker);
              ////////////////////
              T=TArray[ppathcloud.np-1];
              mult(boundarycontri,T,Iboundary);
              mult(I_i,Q,boundarycontri);
              I_i/=g;
              keepgoing=false; //stop here. New photon.
            }
          else
            {
               //we have another scattering/emission point
              //Interpolate T, s_0, etc from ppath and Tarray
              interpTArray(T, K_abs, rte_temperature, K, rte_pos, rte_los, pnd_vec,
                           pathlength_gp,TArray, 
                           ext_matArray,abs_vecArray, t_ppath, pnd_ppath,
                           cum_l_step,pathlength, 
                           stokes_dim, ppathcloud);
              //Estimate single scattering albedo
              albedo=1-K_abs[0]/K(0,0);
              //cout<<"albedo = "<<albedo<<" K(0,0) = "<<K(0,0)<<" K_abs[0] = "<<K_abs[0]<<"\n";
              //determine whether photon is emitted or scattered
              if (rng.draw()>albedo)
                {
                  //Calculate emission
                  Numeric planck_value = planck( f_grid[f_index], rte_temperature );
                  Vector emission=K_abs;
                  emission*=planck_value;
                  Vector emissioncontri(stokes_dim);
                  mult(emissioncontri,T,emission);
                  emissioncontri/=(g*(1-albedo));//yuck!
                  mult(I_i,Q,emissioncontri);
                  keepgoing=false;
                   
                }
              else
                {
                  //Sample new line of sight.
                  
                  Sample_los (new_rte_los,g_los_csc_theta,Z,rng,rte_los,
                              scat_data_mono,stokes_dim,
                              pnd_vec,anyptype30,Z11maxvector,K(0,0)-K_abs[0],rte_temperature);
                                           
                  Z/=g*g_los_csc_theta*albedo;
                  
                  mult(q,T,Z);
                  mult(newQ,Q,q);
                  Q=newQ;
                  scattering_order+=1;
                  za_scat=180-rte_los[0];
                  rte_los=new_rte_los;
                  if (silent==0){cout <<"photon_number = "<<photon_number << 
                                   ", scattering_order = " <<scattering_order <<"\n";}
                }

            }
 
        }
      Isum += I_i;
      if (record_histdata==1){histfile << I_i << "\n";}
      for(Index j=0; j<stokes_dim; j++)
        {
          assert(!isnan(I_i[j]));
          Isquaredsum[j] += I_i[j]*I_i[j];
        }
      
      
      if (photon_number==500)
        {
          cout <<"Estimated execution time for ScatteringMonteCarlo: " << 
            (Numeric)(time(NULL)-start_time)*maxiter/500 <<" seconds.\n";
        }
    }
  
  I=Isum;
  I/=maxiter;
  for(Index j=0; j<stokes_dim; j++) 
    {
      i_montecarlo_error[j]=sqrt((Isquaredsum[j]/maxiter-I[j]*I[j])/maxiter);
    }
  
  I+=IboundaryLOScontri;
  iy(0,joker)=I;
}               




//! rte_posShift
/*! 
   shifts rte_pos and rte_los, and rte_gp_XXX to the end of ppath.

   \author Cory Davis
   \date   2003-07-19
*/


void rte_posShift(
                  Vector&         rte_pos,
                  Vector&         rte_los,
                  GridPos&        rte_gp_p,
                  GridPos&        rte_gp_lat,
                  GridPos&        rte_gp_lon,
                  const Ppath&    ppath,
                  const Index&    atmosphere_dim)
{
  const Index np      = ppath.np;
  
  rte_pos.resize( atmosphere_dim );
  rte_pos = ppath.pos(np-1,Range(0,atmosphere_dim));
  rte_los.resize( ppath.los.ncols() );
  rte_los = ppath.los(np-1,joker);
  gridpos_copy( rte_gp_p, ppath.gp_p[np-1] );
  if( atmosphere_dim > 1 )
    { gridpos_copy( rte_gp_lat, ppath.gp_lat[np-1] ); }
  if( atmosphere_dim > 2 )
    { gridpos_copy( rte_gp_lon, ppath.gp_lon[np-1] ); }
}  



