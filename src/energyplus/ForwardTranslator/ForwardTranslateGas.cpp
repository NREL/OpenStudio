/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Gas.hpp"

#include <utilities/idd/WindowMaterial_Gas_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGas(Gas& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_Gas);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowMaterial_GasFields::Name, modelObject.name().get());

    idfObject.setString(WindowMaterial_GasFields::GasType, modelObject.gasType());

    idfObject.setDouble(WindowMaterial_GasFields::Thickness, modelObject.thickness());

    if (istringEqual("Custom", modelObject.gasType())) {

      if (modelObject.customConductivityCoefficientA()) {
        idfObject.setDouble(WindowMaterial_GasFields::ConductivityCoefficientA, modelObject.customConductivityCoefficientA().get());
      } else {
        LOG(Error, "Missing required input 'Conductivity Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customConductivityCoefficientB()) {
        idfObject.setDouble(WindowMaterial_GasFields::ConductivityCoefficientB, modelObject.customConductivityCoefficientB().get());
      } else {
        LOG(Error, "Missing required input 'Conductivity Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customViscosityCoefficientA()) {
        idfObject.setDouble(WindowMaterial_GasFields::ViscosityCoefficientA, modelObject.customViscosityCoefficientA().get());
      } else {
        LOG(Error, "Missing required input 'Viscosity Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customViscosityCoefficientB()) {
        idfObject.setDouble(WindowMaterial_GasFields::ViscosityCoefficientB, modelObject.customViscosityCoefficientB().get());
      } else {
        LOG(Error, "Missing required input 'Viscosity Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customSpecificHeatCoefficientA()) {
        idfObject.setDouble(WindowMaterial_GasFields::SpecificHeatCoefficientA, modelObject.customSpecificHeatCoefficientA().get());
      } else {
        LOG(Error, "Missing required input 'Specific Heat Coefficient A' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customSpecificHeatCoefficientB()) {
        idfObject.setDouble(WindowMaterial_GasFields::SpecificHeatCoefficientB, modelObject.customSpecificHeatCoefficientB().get());
      } else {
        LOG(Error, "Missing required input 'Specific Heat Coefficient B' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }

      if (modelObject.customMolecularWeight()) {
        idfObject.setDouble(WindowMaterial_GasFields::MolecularWeight, modelObject.customMolecularWeight().get());
      } else {
        LOG(Error, "Missing required input 'Molecular Weight' for WindowMaterial:Gas named '" << modelObject.name().get() << "'");
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
