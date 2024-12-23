/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/StringHelpers.hpp"

#include <utilities/idd/EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEnergyManagementSystemSubroutine(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Subroutine");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(EnergyManagementSystem_SubroutineFields::Name);
    if (!s) {
      LOG(Error, "WorkspaceObject EnergyManagementSystem_Subroutine has no Name");
      return boost::none;
    }

    // Make sure we translate the objects that can be referenced here
    for (const WorkspaceObject& workspaceObject : m_workspace.objects()) {

      // Note: JM 2018-08-17
      // I think an EMS:Subroutine can reference another EMS:Subroutine, we might get problems from that:
      // The one that is being referenced would need be translated before the one that references before the name/uuid substitution happen.
      // But it's harder to control that order*, and this is really an edge case though, so not handling it.
      // * Can't add this condition without getting into a loop:
      // (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_Subroutine)
      if (
        // These I'm sure we do need.
        (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_Actuator)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_Sensor)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_ConstructionIndexVariable)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_GlobalVariable)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_InternalVariable)
        || (workspaceObject.iddObject().type() == IddObjectType::EnergyManagementSystem_TrendVariable)) {
        translateAndMapWorkspaceObject(workspaceObject);
      }
    }
    openstudio::model::EnergyManagementSystemSubroutine emsProgram(m_model);
    emsProgram.setName(*s);

    // Get all model objects that can be referenced int he EMS Program so we can do name / uid substitution
    const std::vector<IddObjectType> validIddObjectTypes{IddObjectType::OS_EnergyManagementSystem_Subroutine,
                                                         IddObjectType::OS_EnergyManagementSystem_Actuator,
                                                         IddObjectType::OS_EnergyManagementSystem_Sensor,
                                                         IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable,
                                                         IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable,
                                                         IddObjectType::OS_EnergyManagementSystem_GlobalVariable,
                                                         IddObjectType::OS_EnergyManagementSystem_InternalVariable,
                                                         IddObjectType::OS_EnergyManagementSystem_TrendVariable};

    std::vector<model::ModelObject> modelObjects;
    for (const model::ModelObject& mo : m_model.modelObjects()) {
      if (std::find(validIddObjectTypes.begin(), validIddObjectTypes.end(), mo.iddObjectType()) != validIddObjectTypes.end()) {
        modelObjects.push_back(mo);
      }
    }

    // Now, we should do the actual name/uid substitution on all lines of the program

    size_t pos;
    size_t len;
    std::string newline;
    std::string uid;

    unsigned n = workspaceObject.numExtensibleGroups();
    OptionalString line;

    // Loop on each line of the program
    for (unsigned i = 0; i < n; ++i) {
      line = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getString(
        EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
      if (line) {
        newline = line.get();

        // Split line to get 'tokens' and look for ModelObject names
        // splitEMSLineToTokens returns already sanitized tokens (excludes reserved keywords, blank ones, functions, removes parenthesis, etc)
        std::vector<std::string> tokens = splitEMSLineToTokens(newline);

        for (std::string& token : tokens) {
          for (const model::ModelObject& mo : modelObjects) {
            // Check if program item is the name of a model object
            boost::optional<std::string> _name = mo.name();
            if (_name && (_name.get() == token)) {
              // replace model object's name with its handle
              pos = newline.find(token);
              len = token.length();
              uid = toString(mo.handle());
              newline.replace(pos, len, uid);
              // Now that we have done the replacement, no need to keep looping.
              // Plus, we should break out of the nested loop and go to the next "j"
              // Otherwise pos could become giberish if there's another object named the same
              // since it won't be able to find the already-replaced string (this shouldn't happen though)
              break;
            }
          }  // end loop on all modelObjects

        }  // end loop on all results in line
        emsProgram.addLine(newline);
      }  // end if(line)
    }  // End loop on each line of the program

    return emsProgram;
  }

}  // namespace energyplus

}  // namespace openstudio
