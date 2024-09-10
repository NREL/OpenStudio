/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/HeatPump_PlantLoop_EIR_Cooling_FieldEnums.hxx>
#include <utilities/idd/HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpPlantLoopEIR_AirSource) {

  Model m;

  PlantLoop plant_loop_plhp_clg(m);
  PlantLoop plant_loop_plhp_htg(m);

  HeatPumpPlantLoopEIRCooling plhp_clg(m);
  EXPECT_TRUE(plhp_clg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_clg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_clg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_clg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_clg.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(plhp_clg.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));
  EXPECT_TRUE(plhp_clg.setControlType("Setpoint"));
  EXPECT_TRUE(plhp_clg.setFlowMode("VariableSpeedPumping"));
  EXPECT_TRUE(plhp_clg.setMinimumPartLoadRatio(6.0));
  EXPECT_TRUE(plhp_clg.setMinimumSourceInletTemperature(7.0));
  EXPECT_TRUE(plhp_clg.setMaximumSourceInletTemperature(8.0));
  CurveQuadratic curve4(m);
  EXPECT_TRUE(plhp_clg.setMinimumSupplyWaterTemperatureCurve(curve4));
  CurveQuadratic curve5(m);
  EXPECT_TRUE(plhp_clg.setMaximumSupplyWaterTemperatureCurve(curve5));
  EXPECT_TRUE(plhp_clg.setMaximumHeatRecoveryOutletTemperature(9.0));
  CurveBiquadratic curve6(m);
  EXPECT_TRUE(plhp_clg.setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(curve6));
  CurveBiquadratic curve7(m);
  EXPECT_TRUE(plhp_clg.setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve7));
  CurveQuadratic curve8(m);
  EXPECT_TRUE(plhp_clg.setThermosiphonCapacityFractionCurve(curve8));
  EXPECT_TRUE(plhp_clg.setThermosiphonMinimumTemperatureDifference(10.0));

  HeatPumpPlantLoopEIRHeating plhp_htg(m);
  EXPECT_TRUE(plhp_htg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_htg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_htg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_htg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_htg.setSizingFactor(5.0));
  CurveBiquadratic curve9(m);
  EXPECT_TRUE(plhp_htg.setCapacityModifierFunctionofTemperatureCurve(curve9));
  CurveBiquadratic curve10(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve10));
  CurveQuadratic curve11(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve11));
  EXPECT_TRUE(plhp_htg.setHeatingToCoolingCapacitySizingRatio(6.0));
  EXPECT_TRUE(plhp_htg.setHeatPumpSizingMethod("HeatingCapacity"));
  EXPECT_TRUE(plhp_htg.setControlType("Setpoint"));
  EXPECT_TRUE(plhp_htg.setFlowMode("VariableSpeedPumping"));
  EXPECT_TRUE(plhp_htg.setMinimumPartLoadRatio(7.0));
  EXPECT_TRUE(plhp_htg.setMinimumSourceInletTemperature(8.0));
  EXPECT_TRUE(plhp_htg.setMaximumSourceInletTemperature(9.0));
  CurveQuadratic curve12(m);
  EXPECT_TRUE(plhp_htg.setMinimumSupplyWaterTemperatureCurve(curve12));
  CurveQuadratic curve13(m);
  EXPECT_TRUE(plhp_htg.setMaximumSupplyWaterTemperatureCurve(curve13));
  CurveQuadratic curve14(m);
  EXPECT_TRUE(plhp_htg.setDryOutdoorCorrectionFactorCurve(curve14));
  EXPECT_TRUE(plhp_htg.setMaximumOutdoorDryBulbTemperatureForDefrostOperation(10.0));
  EXPECT_TRUE(plhp_htg.setHeatPumpDefrostControl("Timed"));
  EXPECT_TRUE(plhp_htg.setHeatPumpDefrostTimePeriodFraction(11.0));
  CurveBiquadratic curve15(m);
  EXPECT_TRUE(plhp_htg.setDefrostEnergyInputRatioFunctionofTemperatureCurve(curve15));
  CurveQuadratic curve16(m);
  EXPECT_TRUE(plhp_htg.setTimedEmpiricalDefrostFrequencyCurve(curve16));
  CurveQuadratic curve17(m);
  EXPECT_TRUE(plhp_htg.setTimedEmpiricalDefrostHeatLoadPenaltyCurve(curve17));
  CurveQuadratic curve18(m);
  EXPECT_TRUE(plhp_htg.setTimedEmpiricalDefrostHeatInputEnergyFractionCurve(curve18));
  EXPECT_TRUE(plhp_htg.setMinimumHeatRecoveryOutletTemperature(12.0));
  CurveBiquadratic curve19(m);
  EXPECT_TRUE(plhp_htg.setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(curve19));
  CurveBiquadratic curve20(m);
  EXPECT_TRUE(plhp_htg.setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve20));

  EXPECT_TRUE(plhp_clg.setCompanionHeatingHeatPump(plhp_htg));
  EXPECT_TRUE(plhp_htg.setCompanionCoolingHeatPump(plhp_clg));

  EXPECT_TRUE(plant_loop_plhp_clg.addSupplyBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_plhp_htg.addSupplyBranchForComponent(plhp_htg));

  // Didn't connect them to the demand side of a loop, so should be AirSource
  EXPECT_EQ("AirSource", plhp_clg.condenserType());
  EXPECT_EQ("AirSource", plhp_htg.condenserType());

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  {
    WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Cooling));
    EXPECT_EQ(1u, idf_ccs.size());
    WorkspaceObject idf_cc(idf_ccs[0]);

    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Cooling 1 Inlet Node For Source Side",
              idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Cooling 1 Outlet Node For Source Side",
              idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("AirSource", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_htg.nameString(), idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
    EXPECT_EQ("Setpoint", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::ControlType, false).get());
    EXPECT_EQ("VariableSpeedPumping", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::FlowMode, false).get());
    EXPECT_EQ(6.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::MinimumPartLoadRatio, false).get());
    EXPECT_EQ(7.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::MinimumSourceInletTemperature, false).get());
    EXPECT_EQ(8.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::MaximumSourceInletTemperature, false).get());
    boost::optional<WorkspaceObject> woCurve4(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurveName));
    EXPECT_TRUE(woCurve4);
    boost::optional<WorkspaceObject> woCurve5(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurveName));
    EXPECT_TRUE(woCurve5);
    EXPECT_EQ(9.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::MaximumHeatRecoveryOutletTemperature, false).get());
    boost::optional<WorkspaceObject> woCurve6(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve6);
    boost::optional<WorkspaceObject> woCurve7(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve7);
    boost::optional<WorkspaceObject> woCurve8(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonCapacityFractionCurveName));
    EXPECT_TRUE(woCurve8);
    EXPECT_EQ(10.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonMinimumTemperatureDifference, false).get());
  }

  {
    WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Heating));
    EXPECT_EQ(1u, idf_hcs.size());
    WorkspaceObject idf_hc(idf_hcs[0]);

    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Heating 1 Inlet Node For Source Side",
              idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("Heat Pump Plant Loop EIR Heating 1 Outlet Node For Source Side",
              idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("AirSource", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_clg.nameString(), idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
    EXPECT_EQ(6.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::HeatingToCoolingCapacitySizingRatio, false).get());
    EXPECT_EQ("HeatingCapacity", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpSizingMethod, false).get());
    EXPECT_EQ("Setpoint", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::ControlType, false).get());
    EXPECT_EQ("VariableSpeedPumping", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::FlowMode, false).get());
    EXPECT_EQ(7.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumPartLoadRatio, false).get());
    EXPECT_EQ(8.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumSourceInletTemperature, false).get());
    EXPECT_EQ(9.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::MaximumSourceInletTemperature, false).get());
    boost::optional<WorkspaceObject> woCurve4(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::MinimumSupplyWaterTemperatureCurveName));
    EXPECT_TRUE(woCurve4);
    boost::optional<WorkspaceObject> woCurve5(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::MaximumSupplyWaterTemperatureCurveName));
    EXPECT_TRUE(woCurve5);
    boost::optional<WorkspaceObject> woCurve6(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::DryOutdoorCorrectionFactorCurveName));
    EXPECT_TRUE(woCurve6);
    EXPECT_EQ(10.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::MaximumOutdoorDryBulbTemperatureForDefrostOperation, false).get());
    EXPECT_EQ("Timed", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostControl, false).get());
    EXPECT_EQ(11.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostTimePeriodFraction, false).get());
    boost::optional<WorkspaceObject> woCurve7(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve7);
    boost::optional<WorkspaceObject> woCurve8(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostFrequencyCurveName));
    EXPECT_TRUE(woCurve8);
    boost::optional<WorkspaceObject> woCurve9(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatLoadPenaltyCurveName));
    EXPECT_TRUE(woCurve9);
    boost::optional<WorkspaceObject> woCurve10(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatInputEnergyFractionCurveName));
    EXPECT_TRUE(woCurve10);
    EXPECT_EQ(12.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::MinimumHeatRecoveryOutletTemperature, false).get());
    boost::optional<WorkspaceObject> woCurve11(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve11);
    boost::optional<WorkspaceObject> woCurve12(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve12);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpPlantLoopEIR_WaterSource) {

  Model m;

  PlantLoop plant_loop_cup_clg(m);
  PlantLoop plant_loop_cup_htg(m);
  PlantLoop plant_loop_plhp_clg(m);
  PlantLoop plant_loop_plhp_htg(m);

  HeatPumpPlantLoopEIRCooling plhp_clg(m);

  EXPECT_TRUE(plhp_clg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_clg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_clg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_clg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_clg.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(plhp_clg.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(plhp_clg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));

  HeatPumpPlantLoopEIRHeating plhp_htg(m);

  EXPECT_TRUE(plhp_htg.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(plhp_htg.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(plhp_htg.setReferenceCapacity(3.0));
  EXPECT_TRUE(plhp_htg.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(plhp_htg.setSizingFactor(5.0));
  CurveBiquadratic curve4(m);
  EXPECT_TRUE(plhp_htg.setCapacityModifierFunctionofTemperatureCurve(curve4));
  CurveBiquadratic curve5(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve5));
  CurveQuadratic curve6(m);
  EXPECT_TRUE(plhp_htg.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve6));

  EXPECT_TRUE(plhp_clg.setCompanionHeatingHeatPump(plhp_htg));
  EXPECT_TRUE(plhp_htg.setCompanionCoolingHeatPump(plhp_clg));

  EXPECT_TRUE(plant_loop_cup_clg.addDemandBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_plhp_clg.addSupplyBranchForComponent(plhp_clg));
  EXPECT_TRUE(plant_loop_cup_htg.addDemandBranchForComponent(plhp_htg));
  EXPECT_TRUE(plant_loop_plhp_htg.addSupplyBranchForComponent(plhp_htg));

  // Connected them to the demand side of a loop, so should be WaterSource
  EXPECT_EQ("WaterSource", plhp_htg.condenserType());
  EXPECT_EQ("WaterSource", plhp_clg.condenserType());

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  {
    WorkspaceObjectVector idf_ccs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Cooling));
    EXPECT_EQ(1u, idf_ccs.size());
    WorkspaceObject idf_cc(idf_ccs[0]);

    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, false).get());
    EXPECT_NE("", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("WaterSource", idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_htg.nameString(), idf_cc.getString(HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_cc.getDouble(HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_cc.getTarget(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }

  {
    WorkspaceObjectVector idf_hcs(w.getObjectsByType(IddObjectType::HeatPump_PlantLoop_EIR_Heating));
    EXPECT_EQ(1u, idf_hcs.size());
    WorkspaceObject idf_hc(idf_hcs[0]);

    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName, false).get());
    EXPECT_NE("", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName, false).get());
    EXPECT_EQ("WaterSource", idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, false).get());
    EXPECT_EQ(plhp_clg.nameString(), idf_hc.getString(HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName).get());
    EXPECT_EQ(1.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, false).get());
    EXPECT_EQ(2.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, false).get());
    EXPECT_EQ(3.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, false).get());
    EXPECT_EQ(4.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, false).get());
    EXPECT_EQ(5.0, idf_hc.getDouble(HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, false).get());
    boost::optional<WorkspaceObject> woCurve1(idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve1);
    EXPECT_EQ(woCurve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve2(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName));
    EXPECT_TRUE(woCurve2);
    EXPECT_EQ(woCurve2->iddObject().type(), IddObjectType::Curve_Biquadratic);
    boost::optional<WorkspaceObject> woCurve3(
      idf_hc.getTarget(HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName));
    EXPECT_TRUE(woCurve3);
    EXPECT_EQ(woCurve3->iddObject().type(), IddObjectType::Curve_Quadratic);
  }
}
