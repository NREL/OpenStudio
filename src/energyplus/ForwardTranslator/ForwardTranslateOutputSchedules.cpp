/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputSchedules.hpp"

#include <utilities/idd/Output_Schedules_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputSchedules(OutputSchedules& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_Schedules, modelObject);

    idfObject.setString(Output_SchedulesFields::KeyField, modelObject.keyField());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
