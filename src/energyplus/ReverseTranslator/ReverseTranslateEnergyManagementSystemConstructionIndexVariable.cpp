/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemConstructionIndexVariable.hpp"
#include "../../model/EnergyManagementSystemConstructionIndexVariable_Impl.hpp"
//#include "../../model/Construction.hpp"

#include <utilities/idd/EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemConstructionIndexVariable(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ConstructionIndexVariable) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_ConstructionIndexVariable");
      return boost::none;
    }

    OptionalString s1 = workspaceObject.getString(EnergyManagementSystem_ConstructionIndexVariableFields::Name);
    if (!s1) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_ConstructionIndexVariable has no Name");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName);
    if (!s) {
      LOG(Error, workspaceObject.nameString() + ": has no ConstructionObjectName");
      return boost::none;
    }

    Workspace workspace = workspaceObject.workspace();

    if (s) {
      //std::vector<WorkspaceObject> wsObjects = workspace.getObjectsByTypeAndName(IddObjectType::Construction, *s);
      std::vector<WorkspaceObject> wsObjects = workspace.getObjectsByName(*s);
      if (wsObjects.size() > 1) {
        LOG(Error, workspaceObject.nameString() + ": Construction is not unique.  More than 1 object with that name.");
        return boost::none;
      }
      if (wsObjects.empty()) {
        LOG(Error, workspaceObject.nameString() + ": Construction not found.");
        return boost::none;
      } else {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObjects[0]);
        if (modelObject) {
          openstudio::model::EnergyManagementSystemConstructionIndexVariable emsConstructionIndexVariable(m_model);
          emsConstructionIndexVariable.setName(*s1);
          emsConstructionIndexVariable.setConstructionObject(modelObject.get());
          return emsConstructionIndexVariable;
        }
      }
    }
    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
