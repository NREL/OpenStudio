/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
