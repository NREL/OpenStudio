/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SimpleGlazing.hpp"
#include "../../model/SimpleGlazing_Impl.hpp"

#include <utilities/idd/WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateWindowMaterialSimpleGlazingSystem(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    SimpleGlazing simpleGlazing(m_model);
    OptionalString optS = workspaceObject.name();
    if (optS) {
      simpleGlazing.setName(*optS);
    }

    OptionalDouble d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::UFactor);
    if (d) {
      simpleGlazing.setUFactor(*d);
    }

    d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient);
    if (d) {
      simpleGlazing.setSolarHeatGainCoefficient(*d);
    }

    d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance);
    if (d) {
      simpleGlazing.setVisibleTransmittance(*d);
    }

    result = simpleGlazing;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
