/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Gas.hpp"
#include "../../model/Gas_Impl.hpp"

#include <utilities/idd/WindowMaterial_Gas_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateWindowMaterialGas(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    Gas gas(m_model);
    OptionalString optS = workspaceObject.name();
    if (optS) {
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
      if (!d1) {
        d1 = 0.0;
      }
      if (!d2) {
        d2 = 0.0;
      }
      if (!d3) {
        d3 = 0.0;
      }
      gas.setCustomConductivity(*d1, *d2, *d3);
    }

    d1 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientA);
    d2 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientB);
    d3 = workspaceObject.getDouble(WindowMaterial_GasFields::ViscosityCoefficientC);
    if (d1 || d2 || d3) {
      if (!d1) {
        d1 = 0.0;
      }
      if (!d2) {
        d2 = 0.0;
      }
      if (!d3) {
        d3 = 0.0;
      }
      gas.setCustomViscosity(*d1, *d2, *d3);
    }

    d1 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientA);
    d2 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientB);
    d3 = workspaceObject.getDouble(WindowMaterial_GasFields::SpecificHeatCoefficientC);
    if (d1 || d2 || d3) {
      if (!d1) {
        d1 = 0.0;
      }
      if (!d2) {
        d2 = 0.0;
      }
      if (!d3) {
        d3 = 0.0;
      }
      gas.setCustomSpecificHeat(*d1, *d2, *d3);
    }

    d1 = workspaceObject.getDouble(WindowMaterial_GasFields::MolecularWeight);
    if (d1) {
      gas.setCustomMolecularWeight(*d1);
    }

    result = gas;
    return result;
  }

}  // namespace energyplus
}  // namespace openstudio
