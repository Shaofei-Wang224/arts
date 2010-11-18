/* Copyright (C) 2003-2008 Cory Davis <cory@met.ed.ac.uk>
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA. */

#ifndef montecarlo_h
#define montecarlo_h

/*===========================================================================
  === External declarations
  ===========================================================================*/
#include <stdexcept>
#include <cmath>
#include "messages.h"
#include "arts.h"
#include "ppath.h"
#include "matpackI.h"
#include "special_interp.h"
#include "check_input.h"
#include "rte.h"
#include "lin_alg.h"
#include "logic.h"
#include "optproperties.h"
#include "physics_funcs.h"
#include "xml_io.h"
#include "rng.h"
#include "cloudbox.h"

extern const Numeric DEG2RAD;
extern const Numeric RAD2DEG;
extern const Numeric PI;

void clear_rt_vars_at_gp(Workspace&              ws,
                         MatrixView&             ext_mat_mono,
                         VectorView&             abs_vec_mono,
                         Numeric&                temperature,
                         const Agenda&           opt_prop_gas_agenda,
                         const Agenda&           abs_scalar_gas_agenda,
                         const Index&            f_index,
                         const GridPos&          gp_p,
                         const GridPos&          gp_lat,
                         const GridPos&          gp_lon,
                         const ConstVectorView   p_grid,
                         const ConstTensor3View  t_field,
                         const ConstTensor4View  vmr_field);

void cloudy_rt_vars_at_gp(
                       Workspace&            ws,
                       MatrixView&           ext_mat_mono,
                       VectorView&           abs_vec_mono,
                       VectorView&           pnd_vec,
                       Numeric&              temperature,
                       const Agenda&         opt_prop_gas_agenda,
                       const Agenda&         abs_scalar_gas_agenda,
                       const Index&          stokes_dim,
                       const Index&          f_index,
                       const GridPos         gp_p,
                       const GridPos         gp_lat,
                       const GridPos         gp_lon,
                       const ConstVectorView    p_grid_cloud,
                       const ConstTensor3View   t_field_cloud,
                       const ConstTensor4View   vmr_field_cloud,
                       const Tensor4&           pnd_field,
                       const ArrayOfSingleScatteringData& scat_data_mono,
                       const ArrayOfIndex&                cloudbox_limits,
                       const Vector&            rte_los
                       );

void cloud_atm_vars_by_gp(
                          VectorView pressure,
                          VectorView temperature,
                          MatrixView vmr,
                          MatrixView pnd,
                          const ArrayOfGridPos& gp_p,
                          const ArrayOfGridPos& gp_lat,
                          const ArrayOfGridPos& gp_lon,
                          const ArrayOfIndex& cloudbox_limits,
                          const ConstVectorView p_grid_cloud,
                          const ConstTensor3View   t_field_cloud,
                          const ConstTensor4View   vmr_field_cloud,
                          const ConstTensor4View   pnd_field
);

void Cloudbox_ppathCalc(Workspace&      ws,
                        //  Output:
                        Ppath&          ppath,
                        Ppath&          ppath_step,
                        //  Input:
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
                        const Vector&         rte_los,
                        const Index& z_field_is_1D);


void cum_l_stepCalc(
                      Vector& cum_l_step,
                      const Ppath& ppath
                      );

void findZ11max(Vector& Z11maxvector,
        const  ArrayOfSingleScatteringData& scat_data_mono);

bool is_anyptype30(const ArrayOfSingleScatteringData& scat_data_mono);

void iwp_cloud_opt_pathCalc(Workspace& ws,
                            Numeric& iwp,
                            Numeric& cloud_opt_path,
                            //input
                            const Vector&         rte_pos,
                            const Vector&         rte_los,
                            const Agenda&         ppath_step_agenda,
                            const Vector&         p_grid,
                            const Vector&         lat_grid, 
                            const Vector&         lon_grid, 
                            const Matrix&         r_geoid, 
                            const Matrix&         z_surface,
                            const Tensor3&        z_field, 
                            const Tensor3&        t_field, 
                            const Tensor4&        vmr_field, 
                            const ArrayOfIndex&   cloudbox_limits, 
                            const Tensor4&        pnd_field,
                            const ArrayOfSingleScatteringData& scat_data_mono,
                            const Vector&          particle_masses
                            );


void matrix_exp_p30(MatrixView& M,
                    ConstMatrixView& A);

void mcPathTraceGeneral(Workspace&            ws,
                        MatrixView&           evol_op,
                        Vector&               abs_vec_mono,
                        Numeric&              temperature,
                        MatrixView&           ext_mat_mono,
                        Rng&                  rng,
                        Vector&               rte_pos,
                        Vector&               rte_los,
                        Vector&               pnd_vec,
                        Numeric&              g,
                        Ppath&                ppath_step,
                        Index&                termination_flag,
                        bool&                 inside_cloud,
                        //Numeric&              rte_pressure,
                        //Vector&               rte_vmr_list,
                        const Agenda&         opt_prop_gas_agenda,
                        const Agenda&         abs_scalar_gas_agenda,
                        const Index&          stokes_dim,
                        const Index&          f_index,
                        const Vector&         p_grid,
                        const Vector&         lat_grid,
                        const Vector&         lon_grid,
                        const Tensor3&        z_field,
                        const Matrix&         r_geoid,
                        const Matrix&         z_surface,
                        const Tensor3&        t_field,
                        const Tensor4&        vmr_field,
                        const ArrayOfIndex&   cloudbox_limits,
                        const Tensor4&        pnd_field,
                        const ArrayOfSingleScatteringData& scat_data_mono,
                        const Index&          z_field_is_1D);

void mcPathTraceIPA(Workspace&            ws,
                    MatrixView&           evol_op,
                    Vector&               abs_vec_mono,
                    Numeric&              temperature,
                    MatrixView&           ext_mat_mono,
                    Rng&                  rng,
                    Vector&               rte_pos,
                    Vector&               rte_los,
                    Vector&               pnd_vec,
                    Numeric&              g,
                    Index&                termination_flag,
                    bool&                 inside_cloud,
                    //Numeric&              rte_pressure,
                    //Vector&               rte_vmr_list,
                    const Agenda&         opt_prop_gas_agenda,
                    const Agenda&         abs_scalar_gas_agenda,
                    const Index&          stokes_dim,
                    const Index&          f_index,
                    const Vector&         p_grid,
                    const Vector&         lat_grid,
                    const Vector&         lon_grid,
                    const Tensor3&        z_field,
                    const Matrix&         r_geoid,
                    const Matrix&         z_surface,
                    const Tensor3&        t_field,
                    const Tensor4&        vmr_field,
                    const ArrayOfIndex&   cloudbox_limits,
                    const Tensor4&        pnd_field,
                    const ArrayOfSingleScatteringData& scat_data_mono,
                    const Index&          z_field_is_1D,
                    const Ppath&          ppath);

void opt_propCalc(
                  MatrixView& K,
                  VectorView& K_abs,
                  const Numeric za,
                  const Numeric aa,
                  const ArrayOfSingleScatteringData& scat_data_mono,
                  const Index&          stokes_dim,
                  const VectorView& pnd_vec,
                  const Numeric& rte_temperature
                  );

void opt_propExtract(
                     MatrixView& K_spt,
                     VectorView& K_abs_spt,
                     const SingleScatteringData& scat_data,
                     const Numeric& za,
                     const Numeric& aa,
                     const Numeric& rte_temperature,
                     const Index& stokes_dim
                     );

void pha_mat_singleCalc(
                        MatrixView& Z,                  
                        Numeric za_sca, 
                        Numeric aa_sca, 
                        Numeric za_inc, 
                        Numeric aa_inc,
                        const ArrayOfSingleScatteringData& scat_data_mono,
                        const Index&          stokes_dim,
                        const VectorView& pnd_vec,
                        const Numeric& rte_temperature
                        );

void pha_mat_singleExtract(
                           MatrixView& Z_spt,
                           const SingleScatteringData& scat_data,
                           const Numeric& za_sca,
                           const Numeric& aa_sca,
                           const Numeric& za_inc,
                           const Numeric& aa_inc,
                           const Numeric& rte_temperature,
                           const Index& stokes_dim
                           );


void Sample_los (
                   VectorView& new_rte_los,
                   Numeric& g_los_csc_theta,
                   MatrixView& Z,
                   Rng& rng,
                   const VectorView& rte_los,
                   const ArrayOfSingleScatteringData& scat_data_mono,
                   const Index&          stokes_dim,
                   const VectorView& pnd_vec,
                   const bool& anyptype30,
                   const VectorView& Z11maxvector,
                   Numeric Csca,
                   const Numeric& rte_temperature
                   );


#endif  // montecarlo_h
