/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpaceInfiltrationFlowCoefficient(SpaceInfiltrationFlowCoefficient& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneInfiltration_FlowCoefficient, modelObject);
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, false);  // We do not allow spaceType!
    idfObject.setString(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName, parentIdfObject.nameString());

    boost::optional<Schedule> schedule_ = modelObject.schedule();
    if (!schedule_) {
      schedule_ = modelObject.model().alwaysOnDiscreteSchedule();
    }
    if (auto idf_sch_ = translateAndMapModelObject(schedule_.get())) {
      idfObject.setString(ZoneInfiltration_FlowCoefficientFields::ScheduleName, idf_sch_->nameString());
    }

    // Flow Coefficient: Required Double
    double flowCoefficient = modelObject.flowCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient, flowCoefficient);

    // Stack Coefficient: Required Double
    double stackCoefficient = modelObject.stackCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient, stackCoefficient);

    // Pressure Exponent: Optional Double
    double pressureExponent = modelObject.pressureExponent();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent, pressureExponent);

    // Wind Coefficient: Required Double
    double windCoefficient = modelObject.windCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient, windCoefficient);

    // Shelter Factor: Required Double
    double shelterFactor = modelObject.shelterFactor();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor, shelterFactor);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
