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
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "../../model/CurveQuadLinear.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitHeating(HeatPumpWaterToWaterEquationFitHeating& modelObject) {
    // createRegisterAndNameIdfObject will add it to m_map, m_idfObjects, and name it
    // The fact that it is added to m_map will avoid a recursion issue when it has a companion cooling coil (both FT methods call each other)
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_WaterToWater_EquationFit_Heating, modelObject);

    boost::optional<double> optvalue;

    if (auto value = modelObject.supplyOutletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.supplyInletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName, value->name().get());
    }

    if (auto value = modelObject.demandOutletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.demandInletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName, value->name().get());
    }

    if (modelObject.isReferenceLoadSideFlowRateAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, "Autosize");
    } else if ((optvalue = modelObject.referenceLoadSideFlowRate())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, optvalue.get());
    }

    if (modelObject.isReferenceSourceSideFlowRateAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, "Autosize");
    } else if ((optvalue = modelObject.referenceSourceSideFlowRate())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, optvalue.get());
    }

    if (modelObject.isRatedHeatingCapacityAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, "Autosize");
    } else if ((optvalue = modelObject.ratedHeatingCapacity())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, optvalue.get());
    }

    if (modelObject.isRatedHeatingPowerConsumptionAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, "Autosize");
    } else if ((optvalue = modelObject.ratedHeatingPowerConsumption())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, optvalue.get());
    }

    // Heating Capacity Curve Name
    {
      auto curve = modelObject.heatingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCurveName, _curve->nameString());
      }
    }

    // Heating Compressor Power Curve Name
    {
      auto curve = modelObject.heatingCompressorPowerCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCurveName, _curve->nameString());
      }
    }

    {
      auto value = modelObject.referenceCoefficientofPerformance();
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceCoefficientofPerformance, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::SizingFactor, value);
    }

    boost::optional<HeatPumpWaterToWaterEquationFitCooling> companion = modelObject.companionCoolingHeatPump();

    if (companion) {
      boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
      if (_companion) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName, _companion->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
