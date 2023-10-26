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
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitCooling(HeatPumpWaterToWaterEquationFitCooling& modelObject) {
    // createRegisterAndNameIdfObject will add it to m_map, m_idfObjects, and name it
    // The fact that it is added to m_map will avoid a recursion issue when it has a companion heating coil (both FT methods call each other)
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_WaterToWater_EquationFit_Cooling, modelObject);

    boost::optional<double> optvalue;

    if (auto value = modelObject.supplyOutletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.supplyInletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName, value->name().get());
    }

    if (auto value = modelObject.demandOutletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.demandInletModelObject()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName, value->name().get());
    }

    if (modelObject.isReferenceLoadSideFlowRateAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, "Autosize");
    } else if ((optvalue = modelObject.referenceLoadSideFlowRate())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, optvalue.get());
    }

    if (modelObject.isReferenceSourceSideFlowRateAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, "Autosize");
    } else if ((optvalue = modelObject.referenceSourceSideFlowRate())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, optvalue.get());
    }

    if (modelObject.isRatedCoolingCapacityAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, "Autosize");
    } else if ((optvalue = modelObject.ratedCoolingCapacity())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, optvalue.get());
    }

    if (modelObject.isRatedCoolingPowerConsumptionAutosized()) {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, "Autosize");
    } else if ((optvalue = modelObject.ratedCoolingPowerConsumption())) {
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, optvalue.get());
    }

    // Cooling Capacity Curve Name
    {
      auto curve = modelObject.coolingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCurveName, _curve->nameString());
      }
    }

    // Cooling Compressor Power Curve Name
    {
      auto curve = modelObject.coolingCompressorPowerCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCurveName, _curve->nameString());
      }
    }

    {
      auto value = modelObject.referenceCoefficientofPerformance();
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoefficientofPerformance, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::SizingFactor, value);
    }

    boost::optional<HeatPumpWaterToWaterEquationFitHeating> companion = modelObject.companionHeatingHeatPump();

    if (companion) {
      boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
      if (_companion) {
        idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName, _companion->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
