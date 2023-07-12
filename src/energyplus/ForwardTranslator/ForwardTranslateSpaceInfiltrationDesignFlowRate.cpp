/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpaceInfiltrationDesignFlowRate(SpaceInfiltrationDesignFlowRate& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneInfiltration_DesignFlowRate, modelObject);
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, true);  // We **do** allow spaceType!
    idfObject.setString(ZoneInfiltration_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    boost::optional<Schedule> schedule = modelObject.schedule();
    if (schedule) {
      idfObject.setString(ZoneInfiltration_DesignFlowRateFields::ScheduleName, schedule->nameString());
    }

    std::string designFlowRateCalculationMethod = modelObject.designFlowRateCalculationMethod();
    if (istringEqual("Flow/Space", designFlowRateCalculationMethod)) {
      designFlowRateCalculationMethod = "Flow/Zone";
    }
    idfObject.setString(ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, designFlowRateCalculationMethod);

    OptionalDouble d = modelObject.designFlowRate();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::DesignFlowRate, *d);
    }

    d = modelObject.flowperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowRateperFloorArea, *d);
    }

    d = modelObject.flowperExteriorSurfaceArea();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowRateperExteriorSurfaceArea, *d);
    }

    d = modelObject.flowperExteriorWallArea();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowRateperExteriorSurfaceArea, *d);
    }

    d = modelObject.airChangesperHour();
    if (d) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::AirChangesperHour, *d);
    }

    if (!modelObject.isConstantTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::ConstantTermCoefficient, modelObject.constantTermCoefficient());
    }

    if (!modelObject.isTemperatureTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, modelObject.temperatureTermCoefficient());
    }

    if (!modelObject.isVelocityTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::VelocityTermCoefficient, modelObject.velocityTermCoefficient());
    }

    if (!modelObject.isVelocitySquaredTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, modelObject.velocitySquaredTermCoefficient());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
