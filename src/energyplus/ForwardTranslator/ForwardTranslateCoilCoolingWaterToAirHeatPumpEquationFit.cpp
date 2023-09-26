/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../../model/Curve.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilCoolingWaterToAirHeatPumpEquationFit(CoilCoolingWaterToAirHeatPumpEquationFit& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;

    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit, modelObject);

    // Object Name
    //std::string baseName = idfObject.name().get();

    //  Water Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName, node->name().get());
      }
    }

    // Water Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName, node->name().get());
      }
    }

    // Air Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, node->name().get());
      }
    }

    // Air Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, node->name().get());
      }
    }

    // Rated Air Flow Rate
    if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, *value);
    } else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "Autosize");
    }

    // Rated Water Flow Rate
    if ((value = modelObject.ratedWaterFlowRate())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, *value);
    } else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "Autosize");
    }

    // Rated Total Cooling Capacity
    if ((value = modelObject.ratedTotalCoolingCapacity())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, *value);
    } else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, "Autosize");
    }

    // Rated Sensible Cooling Capacity
    if ((value = modelObject.ratedSensibleCoolingCapacity())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, *value);
    } else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, "Autosize");
    }

    // Rated Cooling Coefficient of Performance
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP, modelObject.ratedCoolingCoefficientofPerformance());

    // Rated Entering Water Temperature
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature,
                        modelObject.ratedEnteringWaterTemperature());

    // Rated Entering Air Dry-Bulb Temperature
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature,
                        modelObject.ratedEnteringAirDryBulbTemperature());

    // Rated Entering Air Wet-Bulb Temperature
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature,
                        modelObject.ratedEnteringAirWetBulbTemperature());

    // Total Cooling Capacity Curve Name
    {
      auto curve = modelObject.totalCoolingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName, _curve->nameString());
      }
    }

    // Sensible Cooling Capacity Curve Name
    {
      auto curve = modelObject.sensibleCoolingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName, _curve->nameString());
      }
    }

    // Cooling Power Consumption Curve Name
    {
      auto curve = modelObject.coolingPowerConsumptionCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName, _curve->nameString());
      }
    }

    // Nominal Time for Condensate Removal to Begin
    if ((value = modelObject.nominalTimeforCondensateRemovaltoBegin())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, *value);
    }

    // Ratio of Initial Moisture Evaporation Rate and Steady State Latent
    if ((value = modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity())) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                          *value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
