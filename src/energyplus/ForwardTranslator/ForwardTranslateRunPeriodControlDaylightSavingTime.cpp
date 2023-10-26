/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/RunPeriodControlDaylightSavingTime.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRunPeriodControlDaylightSavingTime(RunPeriodControlDaylightSavingTime& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::RunPeriodControl_DaylightSavingTime);

    boost::optional<std::string> s;

    s = modelObject.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate);
    if (s) {
      std::string str = s.get();
      //auto index = str.find("5th", 0);
      //if (index != std::string::npos){
      //  str.replace(index, 3, "Last");
      //}

      idfObject.setString(RunPeriodControl_DaylightSavingTimeFields::StartDate, str);
    }

    s = modelObject.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate);
    if (s) {
      std::string str = s.get();
      //auto index = str.find("5th", 0);
      //if (index != std::string::npos){
      //  str.replace(index, 3, "Last");
      //}

      idfObject.setString(RunPeriodControl_DaylightSavingTimeFields::EndDate, str);
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
