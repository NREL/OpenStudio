/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/Material_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateMaterial(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    if (workspaceObject.iddObject().type() != IddObjectType::Material) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Material");
      return result;
    }

    openstudio::model::StandardOpaqueMaterial mat(m_model);
    OptionalString optS = workspaceObject.name();
    if (optS) {
      mat.setName(*optS);
    }

    optS = workspaceObject.getString(MaterialFields::Roughness);
    if (optS) {
      mat.setRoughness(*optS);
    }

    OptionalDouble od = workspaceObject.getDouble(MaterialFields::Thickness);
    if (od) {
      mat.setThickness(*od);
    }

    od = workspaceObject.getDouble(MaterialFields::Conductivity);
    if (od) {
      mat.setThermalConductivity(*od);
    }

    od = workspaceObject.getDouble(MaterialFields::Density);
    if (od) {
      mat.setDensity(*od);
    }

    od = workspaceObject.getDouble(MaterialFields::SpecificHeat);
    if (od) {
      mat.setSpecificHeat(*od);
    }

    od = workspaceObject.getDouble(MaterialFields::ThermalAbsorptance);
    if (od) {
      mat.setThermalAbsorptance(od);
    }

    od = workspaceObject.getDouble(MaterialFields::SolarAbsorptance);
    if (od) {
      mat.setSolarAbsorptance(od);
    }

    od = workspaceObject.getDouble(MaterialFields::VisibleAbsorptance);
    if (od) {
      mat.setVisibleAbsorptance(od);
    }

    result = mat;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
