/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemGlobalVariable(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_GlobalVariable) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_GlobalVariable");
      return boost::none;
    }

    if (workspaceObject.numExtensibleGroups() > 1) {
      LOG(Warn, "EnergyManagementSystem GlobalVariable has more than 1 value.");
    }

    OptionalModelObject result;
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      boost::optional<std::string> value = eg.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName);
      if (value) {
        openstudio::model::EnergyManagementSystemGlobalVariable emsGlobalVariable(m_model, value.get());
        if (!result) {
          result = emsGlobalVariable;
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
