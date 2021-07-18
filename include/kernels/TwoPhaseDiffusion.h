//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "PorousFlowDictator.h"
#include "Diffusion.h"


class TwoPhaseDiffusion : public Diffusion
{
public:
  static InputParameters validParams();

  TwoPhaseDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  /// The fluid component index
  const unsigned int _ph;

  /// PorousFlowDicator UserObject
  const PorousFlowDictator & _dictator;

  /// Variables for second order derivatives
  const VariableSecond & _second_u;

  /// Porosity at the nodes, but it can depend on grad(variables) which are actually evaluated at the qps
  const MaterialProperty<Real> & _porosity;

  /// Tortuosity at the nodess
  const MaterialProperty<Real> & _tortuosity;

  /// Calculation of diffusion coefficient 

  /// Nodal fluid saturation
  const MaterialProperty<std::vector<Real>> & _fluid_saturation_nodal;

  /// coupled Variable
  const VariableValue & _T;
  unsigned _T_id;

  const MaterialProperty<Real> & _D_gas;
  const MaterialProperty<Real> & _D_aq;
  const MaterialProperty<Real> & _EA_gas;
  const MaterialProperty<Real> & _EA_aq;
};
