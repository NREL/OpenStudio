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
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpPlantLoopEIRHeating(HeatPumpPlantLoopEIRHeating& modelObject) {
    // createRegisterAndNameIdfObject will add it to m_map, m_idfObjects, and name it
    // The fact that it is added to m_map will avoid a recursion issue when it has a companion cooling coil (both FT methods call each other)
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_PlantLoop_EIR_Heating, modelObject);

    boost::optional<double> optvalue;

    if (auto value = modelObject.supplyOutletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.supplyInletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, value->name().get());
    }

    std::string condenserType = modelObject.condenserType();
    idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, condenserType);

    if (openstudio::istringEqual(condenserType, "WaterSource")) {

      if (auto value = modelObject.demandOutletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, value->name().get());
      }

      if (auto value = modelObject.demandInletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, value->name().get());
      }

    } else {
      // AirSource
      {
        // Name the source side outlet node
        auto name = modelObject.nameString() + " Outlet Node For Source Side";
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, name);
      }

      {
        // Create an OutdoorAir:NodeList for the source side inlet conditions to be set directly from weather file
        IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
        auto name = modelObject.nameString() + " Inlet Node For Source Side";
        oaNodeListIdf.setString(0, name);
        m_idfObjects.push_back(oaNodeListIdf);
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, name);
      }
    }

    boost::optional<HeatPumpPlantLoopEIRCooling> companion = modelObject.companionCoolingHeatPump();
    if (companion) {
      boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
      if (_companion) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName, _companion->name().get());
      }
    }

    if (modelObject.isLoadSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.loadSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.isSourceSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.sourceSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.isReferenceCapacityAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, "Autosize");
    } else if ((optvalue = modelObject.referenceCapacity())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, optvalue.get());
    }

    {
      auto value = modelObject.referenceCoefficientofPerformance();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, value);
    }

    {
      auto curve = modelObject.capacityModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName, _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                            _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
