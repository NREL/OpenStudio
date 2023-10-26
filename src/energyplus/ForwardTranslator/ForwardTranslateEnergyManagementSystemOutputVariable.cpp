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
#include "../../model/EnergyManagementSystemOutputVariable.hpp"
#include "../../model/EnergyManagementSystemOutputVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemOutputVariable(EnergyManagementSystemOutputVariable& modelObject) {
    boost::optional<std::string> m;
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_OutputVariable);

    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    s = modelObject.emsVariableName();
    if (s) {

      //find uids
      const int subs[] = {1};
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
            idfObject.setString(EnergyManagementSystem_OutputVariableFields::EMSVariableName, newline);
          }
        } else {
          //did not find an object with the UID so do not FT
          LOG(Error, "Key Name for EMS:OutputVariable '" << modelObject.nameString() << "' is UID but does not exist, it will not be translated.");
          return boost::none;
        }
      }
      //string is not a uuid so just translate its value for the EMSVariableName
      if (!is_uuid) {
        idfObject.setString(EnergyManagementSystem_OutputVariableFields::EMSVariableName, s.get());
      }
    }
    s = modelObject.typeOfDataInVariable();
    if (s.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable, s.get());
    }
    s = modelObject.updateFrequency();
    if (s.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_OutputVariableFields::UpdateFrequency, s.get());
    }
    m = modelObject.emsProgramOrSubroutineName();
    if (m.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, m.get());
    }
    m = modelObject.units();
    if (m.is_initialized()) {
      idfObject.setString(EnergyManagementSystem_OutputVariableFields::Units, m.get());
    }
    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
