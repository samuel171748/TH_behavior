//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AqPhaseDiffusion.h"
#include "MooseVariable.h"
#include "libmesh/quadrature.h"
#include <limits>

registerMooseObject("corrosionApp", AqPhaseDiffusion);

InputParameters
AqPhaseDiffusion::validParams()
{
  InputParameters params = Diffusion::validParams();
  params.addParam<unsigned int>(
      "phase", 0, "Use the Darcy velocity of this fluid phase");
  params.addRequiredParam<UserObjectName>(
      "PorousFlowDictator", "The UserObject that holds the list of PorousFlow variable names");
  params.addRequiredCoupledVar("T", "Temperature of each quadrature point");
  params.addRequiredParam<MaterialPropertyName>("D_aq", "The diffusion coefficient of aqeous species");
  params.addRequiredParam<MaterialPropertyName>("EA_aq", "The activation energy of aqeous species");
  params.addClassDescription("The diffusion kernel of O2 gaseous / aqueous phase");
  return params;
}

AqPhaseDiffusion::AqPhaseDiffusion(const InputParameters & parameters)
 : Diffusion(parameters),
   _dictator(getUserObject<PorousFlowDictator>("PorousFlowDictator")),
   _porosity(getMaterialProperty<Real>("porosity")),
   _tortuosity(getMaterialProperty<Real>("tortuosity")),
   _ph(getParam<unsigned int>("phase")),
   _D_aq(getMaterialProperty<Real>("D_aq")),
   _EA_aq(getMaterialProperty<Real>("EA_aq")),
   _T(coupledValue("T")),
   _T_id(coupled("T")),
   _fluid_saturation_nodal(getMaterialProperty<std::vector<Real>>("PorousFlow_saturation_nodal"))
{
}

Real
AqPhaseDiffusion::computeQpResidual()
{
  Real R = 8.314;

  return _grad_test[_i][_qp] * _porosity[_qp] * _tortuosity[_qp] * _D_aq[_qp] * exp(-_EA_aq[_qp] / (R * _T[_qp])) * _fluid_saturation_nodal[_i][_ph] * _grad_u[_qp];
}

Real
AqPhaseDiffusion::computeQpJacobian()
{
  Real R = 8.314;

  return _grad_test[_i][_qp] * _porosity[_qp] * _tortuosity[_qp] * _D_aq[_qp] * exp(-_EA_aq[_qp] / (R * _T[_qp])) * _fluid_saturation_nodal[_i][_ph] * _grad_phi[_j][_qp];
}

Real
AqPhaseDiffusion::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real R = 8.314;

//  if (jvar == _T_id)
//	return _grad_test[_i][_qp] * _porosity[_qp] * _tortuosity[_qp] * _D_aq[_qp] * _EA_aq[_qp] / (R * _T[_qp] * _T[_qp]) * _phi[_j][_qp] * exp(-_EA_aq[_qp] / (R * _T[_qp])) * _fluid_saturation_nodal[_i][_ph] * _grad_u[_qp];
//  else
        return 0.0;
}
