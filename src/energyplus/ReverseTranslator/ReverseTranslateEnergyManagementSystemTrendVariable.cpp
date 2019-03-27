/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/EnergyManagementSystemTrendVariable.hpp"
#include "../../model/EnergyManagementSystemTrendVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_TrendVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEnergyManagementSystemTrendVariable(const WorkspaceObject & workspaceObject)
{
  if (workspaceObject.iddObject().type() != IddObjectType::EnergyManagementSystem_TrendVariable) {
    LOG(Error, "WorkspaceObject is not IddObjectType: EnergyManagementSystem_TrendVariable");
    return boost::none;
  }

  OptionalString s = workspaceObject.getString(EnergyManagementSystem_TrendVariableFields::Name);
  if(!s){
    LOG(Error, "WorkspaceObject EnergyManagementSystem_TrendVariable has no Name");
    return boost::none;
  }

  openstudio::model::EnergyManagementSystemTrendVariable emsTrendVariable(m_model);
  emsTrendVariable.setName(*s);

  boost::optional<int> i = workspaceObject.getInt(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged);
  if (!i) {
    LOG(Error, emsTrendVariable.nameString() + ": NumberofTimestepstobeLogged not set");
    return boost::none;
  } else {
    emsTrendVariable.setNumberOfTimestepsToBeLogged(*i);
  }

  s = workspaceObject.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName);
  if (!s) {
    LOG(Error, emsTrendVariable.nameString() + ": EMSVariableName not set");
    return boost::none;
  } else {
    bool result = false;
    Workspace workspace = workspaceObject.workspace();
    //look for GlobalVariables, translate and check if there is a name match since GV's dont have name field.
    for (WorkspaceObject& wsObject : workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)) {
      boost::optional<model::ModelObject> modelObject = translateAndMapWorkspaceObject(wsObject);
      if (modelObject) {
        if (modelObject.get().cast<EnergyManagementSystemGlobalVariable>().name() == s) {
          result = emsTrendVariable.setEMSVariableName(*s);
          if (result) {
            return emsTrendVariable;
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
        result = emsTrendVariable.setEMSVariableName(*s);
        if (result) {
          return emsTrendVariable;
        } else {
          return boost::none;
        }
      }
    }
  }

  return emsTrendVariable;
}

} // energyplus

} // openstudio

