/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemSensor(EnergyManagementSystemSensor& modelObject) {
    boost::optional<OutputVariable> d;
    boost::optional<OutputMeter> m;
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_Sensor);
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
        } else {
          //did not find an object with the UID so do not FT
          LOG(Error, "Key Name for Sensor '" << modelObject.nameString() << "' is UID but does not exist, it will not be translated.");
          return boost::none;
        }
      }
      idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, newline);
    }
    d = modelObject.outputVariable();
    if (d.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, d.get().variableName());
      m_idfObjects.push_back(idfObject);
      return idfObject;
    }
    m = modelObject.outputMeter();
    if (m.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, m.get().name());
      m_idfObjects.push_back(idfObject);
      return idfObject;
    }
    s = modelObject.outputVariableOrMeterName();
    if (s.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, s.get());
      m_idfObjects.push_back(idfObject);
      return idfObject;
    }
    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
