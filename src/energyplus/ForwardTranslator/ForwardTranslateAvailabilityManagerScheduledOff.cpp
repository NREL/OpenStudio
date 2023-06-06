/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AvailabilityManagerScheduledOff.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/AvailabilityManager_ScheduledOff_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerScheduledOff(AvailabilityManagerScheduledOff& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::AvailabilityManager_ScheduledOff, modelObject);

    // Schedule
    {
      Schedule sch = modelObject.schedule();
      idfObject.setString(AvailabilityManager_ScheduledOffFields::ScheduleName, sch.name().get());
    }
    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
