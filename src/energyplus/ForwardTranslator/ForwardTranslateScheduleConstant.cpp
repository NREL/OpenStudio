/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleConstant(ScheduleConstant& modelObject) {
    IdfObject scheduleConstant = createAndRegisterIdfObject(openstudio::IddObjectType::Schedule_Constant, modelObject);

    scheduleConstant.setName(modelObject.name().get());
    scheduleConstant.setString(Schedule_ConstantFields::ScheduleTypeLimitsName,
                               modelObject.getString(OS_Schedule_ConstantFields::ScheduleTypeLimitsName, true).get());
    scheduleConstant.setDouble(Schedule_ConstantFields::HourlyValue, modelObject.value());

    return boost::optional<IdfObject>(scheduleConstant);
  }

}  // namespace energyplus

}  // namespace openstudio
