/*!
  \file   agenda_wrappers.h
  \author Simon Pfreundschuh <simonpf@chalmers.se>
  \date   Thu October 06 17:09:00 2016

  \brief Wrappers for the inversion_iterate_agenda in order to them as invlib
  forward models.
*/
#ifndef agenda_wrapper_h
#define agenda_wrapper_h

#include "oem.h"

//! Wrapper class for forward model.
/*!
  Wrapper class for the inversion_iterate_agendaExecute function to implement
  the forward model interface used by the non-linear oem function in oem.cc.
  The object is constructed with the pointers to the variables used as arguments
  for the function and then simply forwards the calls made to
  ForwardModel::evaluate() and ForwardModel::evaluate_jacobian() to
  inversion_iterate_agendaExecute.

 */
class AgendaWrapper
{
    Workspace *ws;
    OEMMatrix jacobian;
    const Agenda *inversion_iterate_agenda;
public:

    const unsigned int m,n;

//! Create inversion_iterate_agendaExecute wrapper.
/*!
  Initializes the wrapper object for the inversion_iterate_agendaExecute
  method. The object forwards the evaluate() and evaluate_jacobian() calls
  made by the iterative OEM methods to inversion_iterate_agendaExecute using
  the arguments provided to the constructor.

  \param ws_ Pointer to the workspace argument of the agenda execution function.
  function.
  \param inversion_iterate_agenda_ Pointer to the x argument of the agenda
  execution function.

*/
    AgendaWrapper( Workspace *ws_,
                   Matrix &jacobian_,
                   const Agenda *inversion_iterate_agenda_ ) :
        ws(ws_),
        jacobian(jacobian_),
        inversion_iterate_agenda( inversion_iterate_agenda_ ),
        m( (unsigned int) jacobian.nrows()),
        n( (unsigned int) jacobian.ncols())
        {}

//! Evaluate forward model and compute Jacobian.
/*!

  Forwards the call to evaluate_jacobian() and evaluate() that is made by
  Gauss-Newton and Levenberg-Marquardt OEM methods using the variables pointed
  to by the pointers provided to the constructor as arguments.

  \param[out] y The measurement vector y = K(x) for the current state vector x
  as computed by the forward model.
  \param[out] J The Jacobian Ki=d/dx(K(x)) of the forward model.
  \param[in] x The current state vector x.
*/
    OEMMatrix & Jacobian(const OEMVector &xi, OEMVector &yi)
    {
        inversion_iterate_agendaExecute( *ws, yi, jacobian, xi, 1,
                                         *inversion_iterate_agenda );
        return jacobian;
    }

//! Evaluate forward model.
/*!

  Forwards the call to evaluate that is made by Gauss-Newton and
  Levenberg-Marquardt OEM methods to the function pointers provided.

  \param[out] y The measurement vector y = K(x) for the current state vector x.
  \param[in] x The current state vector x.
*/
    OEMVector evaluate(const OEMVector &xi)
    {
        OEMVector yi; yi.resize(m);
        Matrix dummy;
        inversion_iterate_agendaExecute( *ws, yi, dummy, xi, 1,
                                         *inversion_iterate_agenda );
        return yi;
    }

};

#endif // agenda_wrappers_h
