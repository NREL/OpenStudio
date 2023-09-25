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
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../../model/CurveQuadLinear.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilHeatingWaterToAirHeatPumpEquationFit(CoilHeatingWaterToAirHeatPumpEquationFit& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;

    // Make sure the modelObject gets put into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit, modelObject);

    // Object Name
    //std::string baseName = idfObject.name().get();

    // Water Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName, node->name().get());
      }
    }

    // Water Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName, node->name().get());
      }
    }

    // Air Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, node->name().get());
      }
    }

    // Air Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, node->name().get());
      }
    }

    // Rated Air Flow Rate
    if (modelObject.isRatedAirFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "Autosize");
    } else if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, value.get());
    }

    // Rated Water Flow Rate
    if (modelObject.isRatedWaterFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "Autosize");
    } else if ((value = modelObject.ratedWaterFlowRate())) {
      idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, value.get());
    }

    // Rated Heating Capacity
    if (modelObject.isRatedHeatingCapacityAutosized()) {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity, "Autosize");
    } else if ((value = modelObject.ratedHeatingCapacity())) {
      idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity, value.get());
    }

    // Heating Coefficient of Performance
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCOP, modelObject.ratedHeatingCoefficientofPerformance());

    // Rated Entering Water Temperature
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature,
                        modelObject.ratedEnteringWaterTemperature());

    // Rated Entering Air Dry-Bulb Temperature
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature,
                        modelObject.ratedEnteringAirDryBulbTemperature());

    // Ratio of Rated Heating Capacity to Rated Cooling Capacity
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity,
                        modelObject.ratioofRatedHeatingCapacitytoRatedCoolingCapacity());

    // Heating Capacity Curve Name
    {
      auto curve = modelObject.heatingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName, _curve->nameString());
      }
    }

    // Heating Power Consumption Curve Name
    {
      auto curve = modelObject.heatingPowerConsumptionCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName, _curve->nameString());
      }
    }

    // Part Load Fraction Correlation Curve Name
    {
      auto curve = modelObject.partLoadFractionCorrelationCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::PartLoadFractionCorrelationCurveName, _curve->nameString());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
