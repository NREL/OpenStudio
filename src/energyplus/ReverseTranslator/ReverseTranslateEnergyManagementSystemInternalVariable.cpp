/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemInternalVariable.hpp"
#include "../../model/EnergyManagementSystemInternalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_InternalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemInternalVariable(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_InternalVariable) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_InternalVariable");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::Name);
    if (!s) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_InternalVariable has no Name.");
      return boost::none;
    }

    openstudio::model::EnergyManagementSystemInternalVariable emsInternalVariable(m_model);
    emsInternalVariable.setName(*s);

    s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType);
    if (s) {
      emsInternalVariable.setInternalDataType(*s);
    }
    s = workspaceObject.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName);
    if (s) {
      emsInternalVariable.setInternalDataIndexKeyName(*s);
    }

    return emsInternalVariable;
  }

}  // namespace energyplus

}  // namespace openstudio
