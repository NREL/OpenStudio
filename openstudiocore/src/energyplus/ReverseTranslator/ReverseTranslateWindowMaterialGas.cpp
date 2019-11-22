/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Gas.hpp"
#include "../../model/Gas_Impl.hpp"

#include <utilities/idd/WindowMaterial_Gas_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateWindowMaterialGas(
    const WorkspaceObject& workspaceObject)
{
  OptionalModelObject result;
  Gas gas(m_model);
  OptionalString optS = workspaceObject.name();
  if(optS) {
    gas.setName(*optS);
  }

  optS = workspaceObject.getString(WindowMaterial_GasFields::GasType);
  if (optS) {
    gas.setGasType(*optS);
  }

  OptionalDouble d1 = workspaceObject.getDouble(WindowMaterial_GasFields::Thickness);
  if (d1) {
    gas.setThickness(*d1);
  }

  d1 = workspaceObject.getDouble(WindowMaterial_GasFields::ConductivityCoefficientA);
  OptionalDouble d2 = workspaceObject.getDouble(WindowMaterial_GasFields::ConductivityCoefficientB);
  OptionalDouble d3 = workspaceObject.getDouble(WindowMaterial_GasFields::ConductivityCoefficientC);
  if (d1 || d2 || d3) {
    if( !d1 ) { d1 = 0.0; }
    if( !d2 ) { d2 = 0.0; }
    if( !d3 ) { d3 = 0.0; }
    gas.setCustomConductivity(*d1,*d2,*d3);
  }

  d1 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientA);
  d2 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientB);
  d3 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientC);
  if (d1 || d2 || d3) {
    if( !d1 ) { d1 = 0.0; }
    if( !d2 ) { d2 = 0.0; }
    if( !d3 ) { d3 = 0.0; }
    gas.setCustomViscosity(*d1,*d2,*d3);
  }

  d1 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientA);
  d2 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientB);
  d3 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientC);
  if (d1 || d2 || d3) {
    if( !d1 ) { d1 = 0.0; }
    if( !d2 ) { d2 = 0.0; }
    if( !d3 ) { d3 = 0.0; }
    gas.setCustomSpecificHeat(*d1,*d2,*d3);
  }

  d1 = workspaceObject.getDouble(WindowMaterial_GasFields::MolecularWeight);
  if (d1) {
    gas.setCustomMolecularWeight(*d1);
  }

  result = gas;
  return result;
}

} // energyplus
} // openstudio
