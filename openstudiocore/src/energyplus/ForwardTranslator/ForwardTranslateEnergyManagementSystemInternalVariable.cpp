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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemInternalVariable.hpp"
#include "../../model/EnergyManagementSystemInternalVariable_Impl.hpp"


#include <utilities/idd/EnergyManagementSystem_InternalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemInternalVariable(EnergyManagementSystemInternalVariable & modelObject)
{
  boost::optional<int> i;
  boost::optional<std::string> s;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_InternalVariable);
  m_idfObjects.push_back(idfObject);
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
  // look for UID and if found replace with object name; otherwise just use string value
  s = modelObject.internalDataIndexKeyName();
  if (s) {
    //find uids
    const int subs[] = { 1 };
    std::string newline = s.get();
    std::string possible_uid;
    size_t pos;
    bool is_uuid = false;
    const Model m = modelObject.model();
    boost::optional<ModelObject> mObject;

    boost::sregex_token_iterator j(s.get().begin(), s.get().end(), uuidInString(), subs);

    while (j != boost::sregex_token_iterator()) {
      possible_uid = *j++;
      //look to see if uid is in the model and return the object
      UUID uid = toUUID(possible_uid);
      mObject = m.getModelObject<model::ModelObject>(uid);
      if (mObject) {
        is_uuid = true;
        //replace uid with namestring
        pos = newline.find(possible_uid);
        if (pos + 38 <= newline.length()) {
          newline.replace(pos, 38, mObject.get().nameString());
          idfObject.setString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, newline);
        }
      }
      else {
        //did not find an object with the UID so do not FT
        LOG(Error, "InternalIndexDataKeyName for EMS:InternalVariable '" << modelObject.nameString() << "' is UID but does not exist, it will not be translated.");
        return boost::none;
      }
    }
    //string is not a uuid so just translate its value for the EMSVariableName
    if (!is_uuid) {
      idfObject.setString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, s.get());
    }
  }
  s = modelObject.internalDataType();
  if (s) {
    idfObject.setString(EnergyManagementSystem_InternalVariableFields::InternalDataType, s.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

