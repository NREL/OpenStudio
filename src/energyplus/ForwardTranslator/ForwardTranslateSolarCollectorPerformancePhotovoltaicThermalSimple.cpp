/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformancePhotovoltaicThermalSimple(
    model::SolarCollectorPerformancePhotovoltaicThermalSimple& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_Simple);

    m_idfObjects.push_back(idfObject);

    boost::optional<double> d;

    idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::Name, modelObject.name().get());

    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FractionofSurfaceAreawithActiveThermalCollector,
                        modelObject.fractionOfSurfaceAreaWithActiveThermalCollector());

    std::string thermalConversionEfficiencyInputModeType = modelObject.thermalConversionEfficiencyInputModeType();
    idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType,
                        thermalConversionEfficiencyInputModeType);

    if (istringEqual("Scheduled", thermalConversionEfficiencyInputModeType)) {
      boost::optional<Schedule> schedule = modelObject.thermalConversionEfficiencySchedule();
      if (schedule) {
        boost::optional<IdfObject> relatedIdfObject = translateAndMapModelObject(*schedule);
        OS_ASSERT(relatedIdfObject);
        idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName,
                            relatedIdfObject->name().get());
      } else {
        LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '"
                     << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Scheduled but has no schedule attached.");
      }
    } else {
      d = modelObject.thermalConversionEfficiency();
      if (d) {
        idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, *d);
      } else {
        LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '"
                     << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Fixed but has no value set.");
      }
    }

    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance, modelObject.frontSurfaceEmittance());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
