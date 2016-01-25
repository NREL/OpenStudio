/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

