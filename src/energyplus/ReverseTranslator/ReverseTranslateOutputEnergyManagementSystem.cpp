/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputEnergyManagementSystem.hpp"
#include "../../model/OutputEnergyManagementSystem_Impl.hpp"

#include <utilities/idd/Output_EnergyManagementSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOutputEnergyManagementSystem(const WorkspaceObject& workspaceObject) {
    auto outputEMS = m_model.getUniqueModelObject<openstudio::model::OutputEnergyManagementSystem>();

    OptionalString s = workspaceObject.getString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting);
    if (s) {
      outputEMS.setActuatorAvailabilityDictionaryReporting(*s);
    }

    s = workspaceObject.getString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting);
    if (s) {
      outputEMS.setInternalVariableAvailabilityDictionaryReporting(*s);
    }

    s = workspaceObject.getString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel);
    if (s) {
      outputEMS.setEMSRuntimeLanguageDebugOutputLevel(*s);
    }

    return outputEMS;
  }

}  // namespace energyplus

}  // namespace openstudio
