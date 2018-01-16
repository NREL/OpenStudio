/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemSensor.hpp"
#include "../../model/EnergyManagementSystemSensor_Impl.hpp"

#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/StringHelpers.hpp"

#include <utilities/idd/EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemSensor(EnergyManagementSystemSensor & modelObject)
{
  boost::optional<OutputVariable> d;
  boost::optional<OutputMeter> m;
  boost::optional<std::string> s;

  IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Sensor);
  m_idfObjects.push_back(idfObject);
  //Name
  s = modelObject.name();
  if (s) {
    idfObject.setName(*s);
  }
  s = modelObject.keyName();
  if (s) {
    //find uids
    const int subs[] = {1};
    std::string newline = s.get();
    std::string possible_uid;
    size_t pos;
    const Model m = modelObject.model();
    boost::optional<ModelObject> mObject;

    boost::sregex_token_iterator j(s.get().begin(), s.get().end(), uuidInString(), subs);

    while (j != boost::sregex_token_iterator()) {
      possible_uid = *j++;
      //look to see if uid is in the model and return the object
      UUID uid = toUUID(possible_uid);
      mObject = m.getModelObject<model::ModelObject>(uid);
      if (mObject) {
        //replace uid with namestring
        pos = newline.find(possible_uid);
        if (pos + 38 <= newline.length()) {
          newline.replace(pos, 38, mObject.get().nameString());
        }
      }
    }
    idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, newline);
  }
  d = modelObject.outputVariable();
  if (d.is_initialized()) {
    idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, d.get().variableName());
    return idfObject;
  }
  m = modelObject.outputMeter();
  if (m.is_initialized()){
    idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, m.get().name());
    return idfObject;
  }
  s = modelObject.outputVariableOrMeterName();
  if (s.is_initialized()) {
    idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, s.get());
    return idfObject;
  }
  return idfObject;
}

} // energyplus

} // openstudio

