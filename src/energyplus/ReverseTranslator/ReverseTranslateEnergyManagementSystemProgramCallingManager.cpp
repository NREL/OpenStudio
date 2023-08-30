/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemProgramCallingManager(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ProgramCallingManager) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_ProgramCallingManager");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_ProgramCallingManagerFields::Name);
    if (!s) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_ProgramCallingManager has no name");
      return boost::none;
    }

    //make sure all other objects are translated first except below
    for (const WorkspaceObject& workspaceObject : m_workspace.objects()) {
      if ((workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Program)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ProgramCallingManager)) {
        translateAndMapWorkspaceObject(workspaceObject);
      }
    }

    openstudio::model::EnergyManagementSystemProgramCallingManager emsProgramCallingManager(m_model);
    emsProgramCallingManager.setName(*s);

    s = workspaceObject.getString(EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint);
    if (!s) {
      LOG(Error, emsProgramCallingManager.nameString() + ": has no EnergyPlusModelCallingPoint");
      return boost::none;
    } else {
      emsProgramCallingManager.setCallingPoint(*s);
    }

    Workspace workspace = workspaceObject.workspace();

    unsigned n = workspaceObject.numExtensibleGroups();
    OptionalWorkspaceObject owo;
    OptionalModelObject omo;
    for (unsigned i = 0; i < n; ++i) {
      owo = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getTarget(
        EnergyManagementSystem_ProgramCallingManagerExtensibleFields::ProgramName);
      if (owo) {
        omo = translateAndMapWorkspaceObject(owo.get());
        if (omo) {
          emsProgramCallingManager.addProgram(omo.get().cast<EnergyManagementSystemProgram>());
        }
      }
    }
    return emsProgramCallingManager;
  }

}  // namespace energyplus

}  // namespace openstudio
