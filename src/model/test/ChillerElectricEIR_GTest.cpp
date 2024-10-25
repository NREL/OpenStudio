/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../ChillerElectricEIR.hpp"

#include "../CurveBiquadratic.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../DistrictCooling.hpp"
#include "../Mixer.hpp"
#include "../Mixer_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../ScheduleConstant.hpp"
#include "../Splitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ChillerElectricEIR_ChillerElectricEIR) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;

      CurveBiquadratic ccFofT(m);
      CurveBiquadratic eirToCorfOfT(m);
      CurveQuadratic eiToCorfOfPlr(m);

      ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ChillerElectricEIR_GettersSetters) {
  Model m;
  ChillerElectricEIR ch(m);

  ch.setName("My ChillerElectricEIR");

  // Reference Capacity: Required Double
  // Autosize
  ch.autosizeReferenceCapacity();
  EXPECT_TRUE(ch.isReferenceCapacityAutosized());
  // Set
  EXPECT_TRUE(ch.setReferenceCapacity(0.3));
  ASSERT_TRUE(ch.referenceCapacity());
  EXPECT_EQ(0.3, ch.referenceCapacity().get());
  // Bad Value
  EXPECT_FALSE(ch.setReferenceCapacity(-10.0));
  ASSERT_TRUE(ch.referenceCapacity());
  EXPECT_EQ(0.3, ch.referenceCapacity().get());
  EXPECT_FALSE(ch.isReferenceCapacityAutosized());

  // Reference COP: Required Double
  EXPECT_TRUE(ch.setReferenceCOP(0.4));
  EXPECT_EQ(0.4, ch.referenceCOP());
  // Bad Value
  EXPECT_FALSE(ch.setReferenceCOP(-10.0));
  EXPECT_EQ(0.4, ch.referenceCOP());

  // Reference Leaving Chilled Water Temperature: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isReferenceLeavingChilledWaterTemperatureDefaulted());
  // Set
  EXPECT_TRUE(ch.setReferenceLeavingChilledWaterTemperature(0.5));
  EXPECT_EQ(0.5, ch.referenceLeavingChilledWaterTemperature());
  EXPECT_FALSE(ch.isReferenceLeavingChilledWaterTemperatureDefaulted());
  // Reset
  ch.resetReferenceLeavingChilledWaterTemperature();
  EXPECT_TRUE(ch.isReferenceLeavingChilledWaterTemperatureDefaulted());

  // Reference Entering Condenser Fluid Temperature: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isReferenceEnteringCondenserFluidTemperatureDefaulted());
  // Set
  EXPECT_TRUE(ch.setReferenceEnteringCondenserFluidTemperature(0.6));
  EXPECT_EQ(0.6, ch.referenceEnteringCondenserFluidTemperature());
  EXPECT_FALSE(ch.isReferenceEnteringCondenserFluidTemperatureDefaulted());
  // Reset
  ch.resetReferenceEnteringCondenserFluidTemperature();
  EXPECT_TRUE(ch.isReferenceEnteringCondenserFluidTemperatureDefaulted());

  // Reference Chilled Water Flow Rate: Required Double
  // Autosize
  ch.autosizeReferenceChilledWaterFlowRate();
  EXPECT_TRUE(ch.isReferenceChilledWaterFlowRateAutosized());
  // Set
  EXPECT_TRUE(ch.setReferenceChilledWaterFlowRate(0.7));
  ASSERT_TRUE(ch.referenceChilledWaterFlowRate());
  EXPECT_EQ(0.7, ch.referenceChilledWaterFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setReferenceChilledWaterFlowRate(-10.0));
  ASSERT_TRUE(ch.referenceChilledWaterFlowRate());
  EXPECT_EQ(0.7, ch.referenceChilledWaterFlowRate().get());
  EXPECT_FALSE(ch.isReferenceChilledWaterFlowRateAutosized());

  // Reference Condenser Fluid Flow Rate: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isReferenceCondenserFluidFlowRateAutosized());
  // Set
  EXPECT_TRUE(ch.setReferenceCondenserFluidFlowRate(0.8));
  ASSERT_TRUE(ch.referenceCondenserFluidFlowRate());
  EXPECT_EQ(0.8, ch.referenceCondenserFluidFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setReferenceCondenserFluidFlowRate(-10.0));
  ASSERT_TRUE(ch.referenceCondenserFluidFlowRate());
  EXPECT_EQ(0.8, ch.referenceCondenserFluidFlowRate().get());
  // Autosize
  ch.autosizeReferenceCondenserFluidFlowRate();
  EXPECT_TRUE(ch.isReferenceCondenserFluidFlowRateAutosized());
  // Reset
  EXPECT_TRUE(ch.setReferenceCondenserFluidFlowRate(0.8));
  ch.resetReferenceCondenserFluidFlowRate();
  EXPECT_TRUE(ch.isReferenceCondenserFluidFlowRateAutosized());

  // Cooling Capacity Function of Temperature Curve Name: Required Object
  CurveBiquadratic ccFofT(m);
  EXPECT_TRUE(ch.setCoolingCapacityFunctionOfTemperature(ccFofT));
  EXPECT_EQ(ccFofT, ch.coolingCapacityFunctionOfTemperature());

  // Electric Input to Cooling Output Ratio Function of Temperature Curve Name: Required Object
  CurveBiquadratic eirToCorfOfT(m);
  EXPECT_TRUE(ch.setElectricInputToCoolingOutputRatioFunctionOfTemperature(eirToCorfOfT));
  EXPECT_EQ(eirToCorfOfT, ch.electricInputToCoolingOutputRatioFunctionOfTemperature());

  // Electric Input to Cooling Output Ratio Function of Part Load Ratio Curve Name: Required Object
  CurveQuadratic eiToCorfOfPlr(m);
  EXPECT_TRUE(ch.setElectricInputToCoolingOutputRatioFunctionOfPLR(eiToCorfOfPlr));
  EXPECT_EQ(eiToCorfOfPlr, ch.electricInputToCoolingOutputRatioFunctionOfPLR());

  // Minimum Part Load Ratio: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isMinimumPartLoadRatioDefaulted());
  // Set
  EXPECT_TRUE(ch.setMinimumPartLoadRatio(1.2));
  EXPECT_EQ(1.2, ch.minimumPartLoadRatio());
  EXPECT_FALSE(ch.isMinimumPartLoadRatioDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setMinimumPartLoadRatio(-10.0));
  EXPECT_EQ(1.2, ch.minimumPartLoadRatio());
  // Reset
  ch.resetMinimumPartLoadRatio();
  EXPECT_TRUE(ch.isMinimumPartLoadRatioDefaulted());

  // Maximum Part Load Ratio: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isMaximumPartLoadRatioDefaulted());
  // Set
  EXPECT_TRUE(ch.setMaximumPartLoadRatio(1.3));
  EXPECT_EQ(1.3, ch.maximumPartLoadRatio());
  EXPECT_FALSE(ch.isMaximumPartLoadRatioDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setMaximumPartLoadRatio(-10.0));
  EXPECT_EQ(1.3, ch.maximumPartLoadRatio());
  // Reset
  ch.resetMaximumPartLoadRatio();
  EXPECT_TRUE(ch.isMaximumPartLoadRatioDefaulted());

  // Optimum Part Load Ratio: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isOptimumPartLoadRatioDefaulted());
  // Set
  EXPECT_TRUE(ch.setOptimumPartLoadRatio(1.4));
  EXPECT_EQ(1.4, ch.optimumPartLoadRatio());
  EXPECT_FALSE(ch.isOptimumPartLoadRatioDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setOptimumPartLoadRatio(-10.0));
  EXPECT_EQ(1.4, ch.optimumPartLoadRatio());
  // Reset
  ch.resetOptimumPartLoadRatio();
  EXPECT_TRUE(ch.isOptimumPartLoadRatioDefaulted());

  // Minimum Unloading Ratio: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isMinimumUnloadingRatioDefaulted());
  // Set
  EXPECT_TRUE(ch.setMinimumUnloadingRatio(1.5));
  EXPECT_EQ(1.5, ch.minimumUnloadingRatio());
  EXPECT_FALSE(ch.isMinimumUnloadingRatioDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setMinimumUnloadingRatio(-10.0));
  EXPECT_EQ(1.5, ch.minimumUnloadingRatio());
  // Reset
  ch.resetMinimumUnloadingRatio();
  EXPECT_TRUE(ch.isMinimumUnloadingRatioDefaulted());

  // Chilled Water Inlet Node Name: Required Object
  // Chilled Water Outlet Node Name: Required Object

  // Condenser Inlet Node Name: Optional Object
  // Condenser Outlet Node Name: Optional Object

  // Condenser Type

  // Condenser Fan Power Ratio: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isCondenserFanPowerRatioDefaulted());
  // Set
  EXPECT_TRUE(ch.setCondenserFanPowerRatio(2.1));
  EXPECT_EQ(2.1, ch.condenserFanPowerRatio());
  EXPECT_FALSE(ch.isCondenserFanPowerRatioDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setCondenserFanPowerRatio(-10.0));
  EXPECT_EQ(2.1, ch.condenserFanPowerRatio());
  // Reset
  ch.resetCondenserFanPowerRatio();
  EXPECT_TRUE(ch.isCondenserFanPowerRatioDefaulted());

  // Fraction of Compressor Electric Consumption Rejected by Condenser: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted());
  // Set
  EXPECT_TRUE(ch.setFractionofCompressorElectricConsumptionRejectedbyCondenser(0.957));
  EXPECT_EQ(0.957, ch.fractionofCompressorElectricConsumptionRejectedbyCondenser());
  EXPECT_FALSE(ch.isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setFractionofCompressorElectricConsumptionRejectedbyCondenser(-10.0));
  EXPECT_EQ(0.957, ch.fractionofCompressorElectricConsumptionRejectedbyCondenser());
  // Reset
  ch.resetFractionofCompressorElectricConsumptionRejectedbyCondenser();
  EXPECT_TRUE(ch.isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted());

  // Leaving Chilled Water Lower Temperature Limit: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isLeavingChilledWaterLowerTemperatureLimitDefaulted());
  // Set
  EXPECT_TRUE(ch.setLeavingChilledWaterLowerTemperatureLimit(2.3));
  EXPECT_EQ(2.3, ch.leavingChilledWaterLowerTemperatureLimit());
  EXPECT_FALSE(ch.isLeavingChilledWaterLowerTemperatureLimitDefaulted());
  // Reset
  ch.resetLeavingChilledWaterLowerTemperatureLimit();
  EXPECT_TRUE(ch.isLeavingChilledWaterLowerTemperatureLimitDefaulted());

  // Chiller Flow Mode: Optional String
  // Default value from IDD
  EXPECT_TRUE(ch.isChillerFlowModeDefaulted());
  EXPECT_EQ("NotModulated", ch.chillerFlowMode());
  // Set
  EXPECT_TRUE(ch.setChillerFlowMode("ConstantFlow"));
  EXPECT_EQ("ConstantFlow", ch.chillerFlowMode());
  EXPECT_FALSE(ch.isChillerFlowModeDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setChillerFlowMode("BADENUM"));
  EXPECT_EQ("ConstantFlow", ch.chillerFlowMode());
  // Reset
  ch.resetChillerFlowMode();
  EXPECT_TRUE(ch.isChillerFlowModeDefaulted());

  // Design Heat Recovery Water Flow Rate: Required Double
  // Autosize
  ch.autosizeDesignHeatRecoveryWaterFlowRate();
  EXPECT_TRUE(ch.isDesignHeatRecoveryWaterFlowRateAutosized());
  // Set
  EXPECT_TRUE(ch.setDesignHeatRecoveryWaterFlowRate(2.5));
  ASSERT_TRUE(ch.designHeatRecoveryWaterFlowRate());
  EXPECT_EQ(2.5, ch.designHeatRecoveryWaterFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setDesignHeatRecoveryWaterFlowRate(-10.0));
  ASSERT_TRUE(ch.designHeatRecoveryWaterFlowRate());
  EXPECT_EQ(2.5, ch.designHeatRecoveryWaterFlowRate().get());
  EXPECT_FALSE(ch.isDesignHeatRecoveryWaterFlowRateAutosized());

  // Heat Recovery Inlet Node Name: Optional Object
  // Heat Recovery Outlet Node Name: Optional Object

  // Sizing Factor: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isSizingFactorDefaulted());
  // Set
  EXPECT_TRUE(ch.setSizingFactor(2.8));
  EXPECT_EQ(2.8, ch.sizingFactor());
  EXPECT_FALSE(ch.isSizingFactorDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setSizingFactor(-10.0));
  EXPECT_EQ(2.8, ch.sizingFactor());
  // Reset
  ch.resetSizingFactor();
  EXPECT_TRUE(ch.isSizingFactorDefaulted());

  // Basin Heater Capacity: Optional Double
  // Default value from IDD
  EXPECT_TRUE(ch.isBasinHeaterCapacityDefaulted());
  // Set
  EXPECT_TRUE(ch.setBasinHeaterCapacity(2.9));
  EXPECT_EQ(2.9, ch.basinHeaterCapacity());
  EXPECT_FALSE(ch.isBasinHeaterCapacityDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setBasinHeaterCapacity(-10.0));
  EXPECT_EQ(2.9, ch.basinHeaterCapacity());
  // Reset
  ch.resetBasinHeaterCapacity();
  EXPECT_TRUE(ch.isBasinHeaterCapacityDefaulted());

  // Basin Heater Setpoint Temperature: Optional Double
  // Default value from IDD
  EXPECT_FALSE(ch.isBasinHeaterSetpointTemperatureDefaulted());
  EXPECT_EQ(10.0, ch.basinHeaterSetpointTemperature());
  // Set
  EXPECT_TRUE(ch.setBasinHeaterSetpointTemperature(5.0));
  EXPECT_EQ(5.0, ch.basinHeaterSetpointTemperature());
  EXPECT_FALSE(ch.isBasinHeaterSetpointTemperatureDefaulted());
  // Bad Value
  EXPECT_FALSE(ch.setBasinHeaterSetpointTemperature(-8.0));
  EXPECT_EQ(5.0, ch.basinHeaterSetpointTemperature());
  // Reset
  ch.resetBasinHeaterSetpointTemperature();
  EXPECT_TRUE(ch.isBasinHeaterSetpointTemperatureDefaulted());
  EXPECT_EQ(2.0, ch.basinHeaterSetpointTemperature());

  // Basin Heater Operating Schedule Name: Optional Object
  ScheduleConstant basinHeaterOperatingSchedule(m);
  EXPECT_TRUE(ch.setBasinHeaterSchedule(basinHeaterOperatingSchedule));
  ASSERT_TRUE(ch.basinHeaterSchedule());
  EXPECT_EQ(basinHeaterOperatingSchedule, ch.basinHeaterSchedule().get());
  // Reset
  ch.resetBasinHeaterSchedule();
  EXPECT_FALSE(ch.basinHeaterSchedule());

  // Condenser Heat Recovery Relative Capacity Fraction: Required Double
  EXPECT_TRUE(ch.setCondenserHeatRecoveryRelativeCapacityFraction(0.97));
  EXPECT_EQ(0.97, ch.condenserHeatRecoveryRelativeCapacityFraction());
  // Bad Value
  EXPECT_FALSE(ch.setCondenserHeatRecoveryRelativeCapacityFraction(-10.0));
  EXPECT_EQ(0.97, ch.condenserHeatRecoveryRelativeCapacityFraction());

  // Heat Recovery Inlet High Temperature Limit Schedule Name: Optional Object
  ScheduleConstant heatRecoveryInletHighTemperatureLimitSchedule(m);
  EXPECT_TRUE(ch.setHeatRecoveryInletHighTemperatureLimitSchedule(heatRecoveryInletHighTemperatureLimitSchedule));
  ASSERT_TRUE(ch.heatRecoveryInletHighTemperatureLimitSchedule());
  EXPECT_EQ(heatRecoveryInletHighTemperatureLimitSchedule, ch.heatRecoveryInletHighTemperatureLimitSchedule().get());
  // Reset
  ch.resetHeatRecoveryInletHighTemperatureLimitSchedule();
  EXPECT_FALSE(ch.heatRecoveryInletHighTemperatureLimitSchedule());

  // Heat Recovery Leaving Temperature Setpoint Node Name: Optional Object

  // End-Use Subcategory: String
  EXPECT_EQ("General", ch.endUseSubcategory());
  // Set
  EXPECT_TRUE(ch.setEndUseSubcategory("Chillers"));
  EXPECT_EQ("Chillers", ch.endUseSubcategory());

  // Condenser Flow Control: Required String
  EXPECT_TRUE(ch.setCondenserFlowControl("ConstantFlow"));
  EXPECT_EQ("ConstantFlow", ch.condenserFlowControl());
  // Bad Value
  EXPECT_FALSE(ch.setCondenserFlowControl("BADENUM"));
  EXPECT_EQ("ConstantFlow", ch.condenserFlowControl());

  // Condenser Loop Flow Rate Fraction Function of Loop Part Load Ratio Curve Name: Optional Object
  CurveLinear condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(m);
  EXPECT_TRUE(ch.setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve));
  ASSERT_TRUE(ch.condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve());
  EXPECT_EQ(condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve, ch.condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve().get());
  // Reset
  ch.resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve();
  EXPECT_FALSE(ch.condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve());

  // Temperature Difference Across Condenser Schedule Name: Optional Object
  ScheduleConstant temperatureDifferenceAcrossCondenserSchedule(m);
  EXPECT_TRUE(ch.setTemperatureDifferenceAcrossCondenserSchedule(temperatureDifferenceAcrossCondenserSchedule));
  ASSERT_TRUE(ch.temperatureDifferenceAcrossCondenserSchedule());
  EXPECT_EQ(temperatureDifferenceAcrossCondenserSchedule, ch.temperatureDifferenceAcrossCondenserSchedule().get());
  // Reset
  ch.resetTemperatureDifferenceAcrossCondenserSchedule();
  EXPECT_FALSE(ch.temperatureDifferenceAcrossCondenserSchedule());

  // Condenser Minimum Flow Fraction: Required Double
  EXPECT_TRUE(ch.setCondenserMinimumFlowFraction(0.9));
  EXPECT_EQ(0.9, ch.condenserMinimumFlowFraction());

  // Thermosiphon Capacity Fraction Curve Name: Optional Object
  CurveLinear thermosiphonCapacityFractionCurve(m);
  EXPECT_TRUE(ch.setThermosiphonCapacityFractionCurve(thermosiphonCapacityFractionCurve));
  ASSERT_TRUE(ch.thermosiphonCapacityFractionCurve());
  EXPECT_EQ(thermosiphonCapacityFractionCurve, ch.thermosiphonCapacityFractionCurve().get());
  // Reset
  ch.resetThermosiphonCapacityFractionCurve();
  EXPECT_FALSE(ch.thermosiphonCapacityFractionCurve());

  // Thermosiphon Minimum Temperature Difference: Required Double
  EXPECT_TRUE(ch.setThermosiphonMinimumTemperatureDifference(4.1));
  EXPECT_EQ(4.1, ch.thermosiphonMinimumTemperatureDifference());
  // Bad Value
  EXPECT_FALSE(ch.setThermosiphonMinimumTemperatureDifference(-10.0));
  EXPECT_EQ(4.1, ch.thermosiphonMinimumTemperatureDifference());
}

TEST_F(ModelFixture, ChillerElectricEIR_HeatCoolFuelTypes) {
  Model m;
  ChillerElectricEIR ch(m);

  EXPECT_EQ(ComponentType(ComponentType::Cooling), ch.componentType());
  testFuelTypeEquality({FuelType::Electricity}, ch.coolingFuelTypes());
  // Add a CondenserWaterLoop with a component that has another fuel than electricity
  PlantLoop cndLoop(m);
  cndLoop.addDemandBranchForComponent(ch);
  testFuelTypeEquality({FuelType::Electricity}, ch.coolingFuelTypes());
  DistrictCooling dc(m);
  cndLoop.addSupplyBranchForComponent(dc);
  testFuelTypeEquality({FuelType::Electricity, FuelType::DistrictCooling}, ch.coolingFuelTypes());

  testFuelTypeEquality({}, ch.heatingFuelTypes());
  testAppGFuelTypeEquality({}, ch.appGHeatingFuelTypes());
}

// Add to the end of an empty supply side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricEIR_addToNode1) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Node supplyOutletNode = plantLoop.supplyOutletNode();
  Mixer supplyMixer = plantLoop.supplyMixer();

  EXPECT_TRUE(chiller.addToNode(supplyOutletNode));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  ASSERT_TRUE(chiller.supplyOutletModelObject());

  EXPECT_EQ(supplyOutletNode, chiller.supplyOutletModelObject().get());

  boost::optional<ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Mixer> mixer = mo->optionalCast<Mixer>();

  ASSERT_TRUE(mixer);
}

// Add to the front of an empty supply side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricEIR_addToNode2) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Node supplyInletNode = plantLoop.supplyInletNode();

  EXPECT_TRUE(chiller.addToNode(supplyInletNode));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  boost::optional<ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyInletNode, mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.supplySplitter(), mo.get());
}

// Add to the middle of the existing branch.
TEST_F(ModelFixture, ChillerElectricEIR_addToNode3) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Mixer supplyMixer = plantLoop.supplyMixer();
  Splitter supplySplitter = plantLoop.supplySplitter();

  boost::optional<ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = node->inletModelObject();

  EXPECT_TRUE(mo);

  EXPECT_EQ(mo.get(), supplySplitter);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer, mo.get());

  mo = supplySplitter.outletModelObject(0);

  boost::optional<ModelObject> mo2 = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);
  ASSERT_TRUE(mo2);

  boost::optional<HVACComponent> comp = mo->optionalCast<HVACComponent>();
  boost::optional<HVACComponent> comp2 = mo2->optionalCast<HVACComponent>();

  ASSERT_TRUE(comp);
  ASSERT_TRUE(comp2);

  std::vector<ModelObject> comps = plantLoop.supplyComponents(comp.get(), comp2.get());

  ASSERT_EQ(3u, comps.size());
}

// Add to new branch
TEST_F(ModelFixture, ChillerElectricEIR_PlantLoop_addSupplyBranch) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(chiller));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  Mixer supplyMixer = plantLoop.supplyMixer();

  boost::optional<ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer, mo.get());
}

// Add to the end of an empty demand side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricEIR_addToDemandNode1) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Node demandOutletNode = plantLoop.demandOutletNode();
  Mixer demandMixer = plantLoop.demandMixer();

  EXPECT_TRUE(chiller.addToNode(demandOutletNode));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  ASSERT_TRUE(chiller.demandOutletModelObject());

  EXPECT_EQ(demandOutletNode, chiller.demandOutletModelObject().get());

  boost::optional<ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Mixer> mixer = mo->optionalCast<Mixer>();

  ASSERT_TRUE(mixer);

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to the front of an empty demand side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricEIR_addToDemandNode2) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Node demandInletNode = plantLoop.demandInletNode();

  EXPECT_TRUE(chiller.addToNode(demandInletNode));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  boost::optional<ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandInletNode, mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.demandSplitter(), mo.get());

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to the middle of the existing branch.
TEST_F(ModelFixture, ChillerElectricEIR_addToDemandNode3) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer, mo.get());

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to new demand branch
TEST_F(ModelFixture, ChillerElectricEIR_PlantLoop_addDemandBranch) {
  Model m;

  PlantLoop plantLoop(m);

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addDemandBranchForComponent(chiller));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<Node> node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer, mo.get());

  ASSERT_TRUE(plantLoop.removeDemandBranchWithComponent(chiller));

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Check condenser type setting/defaulting
TEST_F(ModelFixture, ChillerElectricEIR_CondenserType) {
  Model m;

  PlantLoop pl1(m);
  PlantLoop pl2(m);

  ChillerElectricEIR ch(m);

  // By default, AirCooled (from IDD)
  EXPECT_EQ("AirCooled", ch.condenserType());

  // Not connected to a secondary plantLoop
  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));

  // Add to primary plant loop (on supply), behavior should be the same
  ASSERT_TRUE(pl1.addSupplyBranchForComponent(ch));
  // Should have stayed
  ASSERT_EQ("AirCooled", ch.condenserType());
  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));

  // Add to the Secondary plant loop (on demand), behavior should be reversed
  ASSERT_TRUE(pl2.addDemandBranchForComponent(ch));
  // Should have been automatically set to WaterCooled
  ASSERT_EQ("WaterCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("AirCooled"));
  ASSERT_FALSE(ch.setCondenserType("EvaporativelyCooled"));

  // Disconnect from the secondary plant Loop
  ASSERT_TRUE(ch.removeFromSecondaryPlantLoop());
  // Should have been automatically switched to AirCooled
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));
}

TEST_F(ModelFixture, ChillerElectricEIR_PlantLoopConnections) {
  Model model;
  ChillerElectricEIR chiller(model);

  // Chilled Water Loop: on the supply side
  {
    PlantLoop chwLoop(model);
    auto node = chwLoop.supplyOutletNode();
    EXPECT_TRUE(chiller.addToNode(node));
    auto plant = chiller.plantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(chwLoop.handle(), plant->handle());
    }
    // PlantLoop has 5 components on the supply side by default (3 Nodes, One splitter, One mixer)
    EXPECT_EQ(7u, chwLoop.supplyComponents().size());

    // test the convenience function
    auto plant_bis = chiller.chilledWaterLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(chwLoop.handle(), plant_bis->handle());
    }
  }

  // Condenser Loop: on the demand side
  {
    PlantLoop cndwLoop(model);
    auto node = cndwLoop.demandInletNode();
    EXPECT_TRUE(chiller.addToNode(node));
    auto plant = chiller.secondaryPlantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(cndwLoop.handle(), plant->handle());
    }

    EXPECT_EQ(7u, cndwLoop.demandComponents().size());

    // test the convenience function
    auto plant_bis = chiller.condenserWaterLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(cndwLoop.handle(), plant_bis->handle());
    }
  }

  // Heat Recovery Loop: on the demand side
  {
    PlantLoop hrLoop(model);
    auto node = hrLoop.demandOutletNode();
    EXPECT_TRUE(chiller.addToTertiaryNode(node));
    auto plant = chiller.tertiaryPlantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(hrLoop.handle(), plant->handle());
    }
    // test the convenience function
    auto plant_bis = chiller.heatRecoveryLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(hrLoop.handle(), plant_bis->handle());
    }

    EXPECT_EQ(7u, hrLoop.demandComponents().size());

    EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
    plant = chiller.tertiaryPlantLoop();
    EXPECT_FALSE(plant);
    EXPECT_EQ(5u, hrLoop.demandComponents().size());
  }
}

/* I have overriden the base class WaterToWaterComponent addToNode() and addToTertiaryNode()
 * addToNode will call addToTertiaryNode behind the scenes when needed
 * that is if you try to add it to the demand side of a plant loop when it already has a condenser water loop
 * it should add it to the tertiary(=HR) loop
 * This should work with addDemandBranchForComponent too
 * AddToTertiaryNode is overriden to not work when trying to add to a supply side node */
TEST_F(ModelFixture, ChillerElectricEIR_PlantLoopConnections_addToNodeOverride) {
  Model model;
  ChillerElectricEIR chiller(model);

  PlantLoop chwLoop(model);

  PlantLoop cndwLoop(model);
  PlantLoop cndwLoop2(model);
  auto c_demand_node2 = cndwLoop2.demandOutletNode();

  PlantLoop hrLoop(model);
  auto h_supply_node = hrLoop.supplyOutletNode();
  auto h_demand_node = hrLoop.demandInletNode();

  // Connect to the chw loop
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  EXPECT_FALSE(chiller.condenserWaterLoop());
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Connect to the condenser loop
  EXPECT_TRUE(cndwLoop.addDemandBranchForComponent(chiller));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());

  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Have a chw loop and no hr loop: should connect the hr loop if trying to add to demand side
  EXPECT_TRUE(hrLoop.addDemandBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());
  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());
  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());

  // Have a condenser loop and a hr loop: should reconnect the condenser loop
  // Try with addToNode instead
  EXPECT_TRUE(chiller.addToNode(c_demand_node2));

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());

  // Disconnect the tertiary (hr) loop
  EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Shouldn't accept tertiary connection to the supply side
  EXPECT_FALSE(chiller.addToTertiaryNode(h_supply_node));
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Try addToNode to the demand side of hr plant loop, should work to connect the hr loop
  EXPECT_TRUE(chiller.addToNode(h_demand_node));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());
}

TEST_F(ModelFixture, ChillerElectricEIR_ElectricInputToCoolingOutputRatioFunctionOfPLR) {
  // Test for #4611 - Allow non-Quadratic curves for the EIR-f-PLR for the Chiller:Electric:EIR object

  Model model;

  // test ctor 1
  {
    // ctor maintains backward compatibility
    ChillerElectricEIR chiller(model);
    ASSERT_TRUE(chiller.electricInputToCoolingOutputRatioFunctionOfPLR().optionalCast<CurveQuadratic>());
  }

  // test ctor 2
  {
    // ctor maintains backward compatibility
    CurveBiquadratic ccFofT(model);
    CurveBiquadratic eirToCorfOfT(model);
    CurveQuadratic eirToCorfOfPlr(model);
    ChillerElectricEIR chiller(model, ccFofT, eirToCorfOfT, eirToCorfOfPlr);
    ASSERT_TRUE(chiller.electricInputToCoolingOutputRatioFunctionOfPLR().optionalCast<CurveQuadratic>());
    EXPECT_EQ(ccFofT, chiller.coolingCapacityFunctionOfTemperature());
    EXPECT_EQ(eirToCorfOfT, chiller.electricInputToCoolingOutputRatioFunctionOfTemperature());
    EXPECT_EQ(eirToCorfOfPlr, chiller.electricInputToCoolingOutputRatioFunctionOfPLR());
  }
  {
    // ctor can now handle cubic curves
    CurveBiquadratic ccFofT2(model);
    CurveBiquadratic eirToCorfOfT2(model);
    CurveCubic eirToCorfOfPlr2(model);
    ChillerElectricEIR chiller(model, ccFofT2, eirToCorfOfT2, eirToCorfOfPlr2);
    ASSERT_TRUE(chiller.electricInputToCoolingOutputRatioFunctionOfPLR().optionalCast<CurveCubic>());
    EXPECT_EQ(ccFofT2, chiller.coolingCapacityFunctionOfTemperature());
    EXPECT_EQ(eirToCorfOfT2, chiller.electricInputToCoolingOutputRatioFunctionOfTemperature());
    EXPECT_EQ(eirToCorfOfPlr2, chiller.electricInputToCoolingOutputRatioFunctionOfPLR());
  }

  // test new setter/getter
  {
    ChillerElectricEIR chiller(model);

    // setter maintains backward compatibility
    CurveQuadratic curveQuadratic(model);
    EXPECT_TRUE(chiller.setElectricInputToCoolingOutputRatioFunctionOfPLR(curveQuadratic));
    ASSERT_TRUE(chiller.electricInputToCoolingOutputRatioFunctionOfPLR().optionalCast<CurveQuadratic>());
    EXPECT_EQ(curveQuadratic, chiller.electricInputToCoolingOutputRatioFunctionOfPLR());

    // setter can now handle cubic curves
    CurveCubic curveCubic(model);
    EXPECT_TRUE(chiller.setElectricInputToCoolingOutputRatioFunctionOfPLR(curveCubic));
    ASSERT_TRUE(chiller.electricInputToCoolingOutputRatioFunctionOfPLR().optionalCast<CurveCubic>());
    EXPECT_EQ(curveCubic, chiller.electricInputToCoolingOutputRatioFunctionOfPLR());
  }
}

TEST_F(ModelFixture, ChillerElectricEIR_HeatRecoverySetpointNode) {

  // Test for #4724
  Model model;
  ChillerElectricEIR chiller(model);

  // Chilled Water Loop: on the supply side
  PlantLoop chwLoop(model);
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(chiller));
  EXPECT_TRUE(chiller.plantLoop());
  EXPECT_TRUE(chiller.chilledWaterLoop());

  // Condenser Loop: on the demand side
  PlantLoop cndwLoop(model);
  EXPECT_TRUE(cndwLoop.addDemandBranchForComponent(chiller));
  EXPECT_TRUE(chiller.secondaryPlantLoop());
  EXPECT_TRUE(chiller.condenserWaterLoop());

  PlantLoop hrLoop(model);
  EXPECT_TRUE(hrLoop.addDemandBranchForComponent(chiller));
  EXPECT_TRUE(chiller.tertiaryPlantLoop());
  EXPECT_TRUE(chiller.heatRecoveryLoop());

  // Double check everything is fine now
  ASSERT_TRUE(chiller.plantLoop());
  EXPECT_EQ(chwLoop, chiller.plantLoop().get());
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.secondaryPlantLoop());
  EXPECT_EQ(cndwLoop, chiller.secondaryPlantLoop().get());
  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.tertiaryPlantLoop());
  EXPECT_EQ(hrLoop, chiller.tertiaryPlantLoop().get());
  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());

  // Get the Chiller Tertirary Outlet Node = HR Loop Outlet Node
  ASSERT_TRUE(chiller.tertiaryOutletModelObject());
  auto hrOutletNode = chiller.tertiaryOutletModelObject()->cast<Node>();
  // Actual test for #4724
  EXPECT_TRUE(chiller.setHeatRecoveryLeavingTemperatureSetpointNode(hrOutletNode));
  ASSERT_TRUE(chiller.heatRecoveryLeavingTemperatureSetpointNode());
  EXPECT_EQ(hrOutletNode, chiller.heatRecoveryLeavingTemperatureSetpointNode().get());
}
