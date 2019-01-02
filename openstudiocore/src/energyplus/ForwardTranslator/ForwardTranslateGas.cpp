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

#include "../ForwardTranslator.hpp"

#include "../../model/Gas.hpp"

#include <utilities/idd/WindowMaterial_Gas_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGas( Gas & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::WindowMaterial_Gas);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_GasFields::Name, modelObject.name().get());

  idfObject.setString(WindowMaterial_GasFields::GasType, modelObject.gasType());

  idfObject.setDouble(WindowMaterial_GasFields::Thickness, modelObject.thickness());

  if (istringEqual("Custom", modelObject.gasType())){

    if (modelObject.customConductivityCoefficientA()){
      idfObject.setDouble(WindowMaterial_GasFields::ConductivityCoefficientA, modelObject.customConductivityCoefficientA().get());
    }else{
      LOG(Error, "Missing required input 'Conductivity Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customConductivityCoefficientB()){
      idfObject.setDouble(WindowMaterial_GasFields::ConductivityCoefficientB, modelObject.customConductivityCoefficientB().get());
    }else{
      LOG(Error, "Missing required input 'Conductivity Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customViscosityCoefficientA()){
      idfObject.setDouble(WindowMaterial_GasFields::ViscosityCoefficientA, modelObject.customViscosityCoefficientA().get());
    }else{
      LOG(Error, "Missing required input 'Viscosity Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customViscosityCoefficientB()){
      idfObject.setDouble(WindowMaterial_GasFields::ViscosityCoefficientB, modelObject.customViscosityCoefficientB().get());
    }else{
      LOG(Error, "Missing required input 'Viscosity Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customSpecificHeatCoefficientA()){
      idfObject.setDouble(WindowMaterial_GasFields::SpecificHeatCoefficientA, modelObject.customSpecificHeatCoefficientA().get());
    }else{
      LOG(Error, "Missing required input 'Specific Heat Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customSpecificHeatCoefficientB()){
      idfObject.setDouble(WindowMaterial_GasFields::SpecificHeatCoefficientB, modelObject.customSpecificHeatCoefficientB().get());
    }else{
      LOG(Error, "Missing required input 'Specific Heat Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

    if (modelObject.customMolecularWeight()){
      idfObject.setDouble(WindowMaterial_GasFields::MolecularWeight, modelObject.customMolecularWeight().get());
    }else{
      LOG(Error, "Missing required input 'Molecular Weight' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
    }

  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

