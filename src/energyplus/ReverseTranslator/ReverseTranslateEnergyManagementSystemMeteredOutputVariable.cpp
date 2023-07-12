/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemMeteredOutputVariable.hpp"
#include "../../model/EnergyManagementSystemMeteredOutputVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_MeteredOutputVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemMeteredOutputVariable(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_MeteredOutputVariable) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_MeteredOutputVariable");
      return boost::none;
    }

    //make sure all other objects are translated first except below
    for (const WorkspaceObject& workspaceObject : m_workspace.objects()) {
      if ((workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Program)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_ProgramCallingManager)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_MeteredOutputVariable)
          && (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_OutputVariable)) {
        translateAndMapWorkspaceObject(workspaceObject);
      }
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::Name);
    if (!s) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_MeteredOutputVariable has no Name");
      return boost::none;
    }
    openstudio::model::EnergyManagementSystemMeteredOutputVariable emsOutputVariable(m_model);
    emsOutputVariable.setName(*s);

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": UpdateFrequency not set");
      return boost::none;
    } else {
      emsOutputVariable.setUpdateFrequency(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::ResourceType);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": ResourceType not set");
      return boost::none;
    } else {
      emsOutputVariable.setResourceType(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::GroupType);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": GroupType not set");
      return boost::none;
    } else {
      emsOutputVariable.setGroupType(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": EndUseCategory not set");
      return boost::none;
    } else {
      emsOutputVariable.setEndUseCategory(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory);
    if (s) {
      emsOutputVariable.setEndUseSubcategory(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::Units);
    if (s) {
      emsOutputVariable.setUnits(*s);
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName);
    if (s) {
      Workspace workspace = workspaceObject.workspace();
      for (WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
        if (modelObject) {
          if (modelObject.get().iddObjectType() == IddObjectType::OS_EnergyManagementSystem_Program) {
            auto program = modelObject.get().cast<EnergyManagementSystemProgram>();
            emsOutputVariable.setEMSProgramOrSubroutineName(program);
          } else if (modelObject.get().iddObjectType() == IddObjectType::OS_EnergyManagementSystem_Subroutine) {
            auto subroutine = modelObject.get().cast<EnergyManagementSystemSubroutine>();
            emsOutputVariable.setEMSProgramOrSubroutineName(subroutine);
          }
          break;
        }
      }
    }

    s = workspaceObject.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName);
    if (!s) {
      LOG(Error, emsOutputVariable.nameString() + ": EMSVariableName not set");
      return boost::none;
    } else {
      bool result = false;
      Workspace workspace = workspaceObject.workspace();
      //look for GlobalVariables, translate and check if there is a name match since GV's dont have name field.
      for (WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
        if (modelObject) {
          if (modelObject.get().cast<EnergyManagementSystemGlobalVariable>().name() == s) {
            result = emsOutputVariable.setEMSVariableName(*s);
            if (result) {
              return emsOutputVariable;
            } else {
              return boost::none;
            }
          }
        }
      }
      //look for name match on other (EMS) objects.
      for (WorkspaceObject& wsObject : workspace.getObjectsByName(*s)) {
        boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
        if (modelObject) {
          result = emsOutputVariable.setEMSVariableName(*s);
          if (result) {
            return emsOutputVariable;
          } else {
            return boost::none;
          }
        }
      }
    }

    return emsOutputVariable;
  }

}  // namespace energyplus

}  // namespace openstudio
