/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceSchedule.hpp"
#include "../../model/ExternalInterfaceSchedule_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleTypeLimits.hpp"

#include <utilities/idd/ExternalInterface_Schedule_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceSchedule(ExternalInterfaceSchedule& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_Schedule);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    const boost::optional<ScheduleTypeLimits> schedule = modelObject.scheduleTypeLimits();
    if (schedule.is_initialized()) {
      idfObject.setString(ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, schedule.get().nameString());
    }

    d = modelObject.initialValue();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_ScheduleFields::InitialValue, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
