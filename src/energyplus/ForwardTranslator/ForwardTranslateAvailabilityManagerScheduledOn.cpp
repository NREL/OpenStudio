/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AvailabilityManagerScheduledOn.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/AvailabilityManager_ScheduledOn_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerScheduledOn(AvailabilityManagerScheduledOn& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::AvailabilityManager_ScheduledOn, modelObject);

    // Schedule
    {
      Schedule sch = modelObject.schedule();
      idfObject.setString(AvailabilityManager_ScheduledOnFields::ScheduleName, sch.name().get());
    }
    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
