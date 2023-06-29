/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpaceInfiltrationEffectiveLeakageArea(SpaceInfiltrationEffectiveLeakageArea& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneInfiltration_EffectiveLeakageArea, modelObject);
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, false);  // We do not allow spaceType!
    idfObject.setString(ZoneInfiltration_EffectiveLeakageAreaFields::ZoneorSpaceName, parentIdfObject.nameString());

    boost::optional<Schedule> schedule = modelObject.schedule();
    if (schedule) {
      idfObject.setString(ZoneInfiltration_EffectiveLeakageAreaFields::ScheduleName, schedule->name().get());
    }

    OptionalDouble d = modelObject.effectiveAirLeakageArea();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea, *d);
    }

    d = modelObject.stackCoefficient();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_EffectiveLeakageAreaFields::StackCoefficient, *d);
    }

    d = modelObject.windCoefficient();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_EffectiveLeakageAreaFields::WindCoefficient, *d);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
