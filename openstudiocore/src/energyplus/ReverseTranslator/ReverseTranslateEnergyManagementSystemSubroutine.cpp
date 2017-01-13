/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemSubroutine(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_Subroutine) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_Subroutine");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_SubroutineFields::Name);
  if (!s) {
    LOG(Error, "WorkspaceObject EnergyManagementSystem_Subroutine has no Name");
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

  openstudio::model::EnergyManagementSystemSubroutine emsProgram(m_model);
  emsProgram.setName(*s);

  //get all model objects so we can do name / uid substitution
  const std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<model::ModelObject>();
  size_t pos, len;
  std::string newline, uid;

  unsigned n = workspaceObject.numExtensibleGroups();
  OptionalString line;
  for (unsigned i = 0; i < n; ++i) {
    line = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getString(EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine);
    if (line) {
      newline = line.get();
      //split line on whitespaces to get look for modelobject names
      std::vector<std::string> results = splitString(line.get(), ' ');
      for (size_t j = 0; j < results.size(); j++) {
        for (size_t k = 0; k < modelObjects.size(); k++) {
          if (modelObjects.at(k).name()) {
            //check if program item is the name of a model object
            if (modelObjects.at(k).name().get() == results.at(j)) {
              pos = newline.find(results.at(j));
              len = results.at(j).length();
              uid = toString(modelObjects.at(k).handle());
              //replace modelobject name with handle
              newline.replace(pos, len, uid);
            }
          }
        }
      }
      emsProgram.addLine(newline);
    }
  }
  return emsProgram;
}

} // energyplus

} // openstudio

