/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../FanComponentModel.hpp"
#include "../FanComponentModel_Impl.hpp"

#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../Curve.hpp"
#include "../CurveExponentialSkewNormal.hpp"
#include "../CurveQuartic.hpp"
#include "../CurveFanPressureRise.hpp"
#include "../CurveRectangularHyperbola2.hpp"
#include "../CurveSigmoid.hpp"
#include "../CurveLinear.hpp"
#include "../CurveRectangularHyperbola1.hpp"
#include "../CurveExponentialDecay.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../PlantLoop.hpp"

#include "../AirLoopHVACUnitarySystem.hpp"

#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilCoolingWater.hpp"
#include "../FanOnOff.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanComponentModel_DefaultedCtor) {

  Model m;

  // Call Ctor that defaults all curves
  FanComponentModel fanComponentModel(m);

  // Required Curves
  EXPECT_NO_THROW(fanComponentModel.fanPressureRiseCurve());
  EXPECT_NO_THROW(fanComponentModel.ductStaticPressureResetCurve());
  EXPECT_NO_THROW(fanComponentModel.normalizedFanStaticEfficiencyCurveNonStallRegion());
  EXPECT_NO_THROW(fanComponentModel.normalizedFanStaticEfficiencyCurveStallRegion());
  EXPECT_NO_THROW(fanComponentModel.normalizedDimensionlessAirflowCurveNonStallRegion());
  EXPECT_NO_THROW(fanComponentModel.normalizedDimensionlessAirflowCurveStallRegion());

  // Optional Curves: Not initialized
  EXPECT_FALSE(fanComponentModel.maximumBeltEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
  EXPECT_FALSE(fanComponentModel.maximumMotorEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.normalizedMotorEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.vFDEfficiencyCurve());

  // Now we call the helper
  EXPECT_TRUE(fanComponentModel.assignDefaultOptionalCurves());
  EXPECT_TRUE(fanComponentModel.maximumBeltEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
  EXPECT_TRUE(fanComponentModel.maximumMotorEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.normalizedMotorEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.vFDEfficiencyCurve());
}

TEST_F(ModelFixture, FanComponentModel_ExplicitCurveCtor) {

  Model m;

  CurveFanPressureRise fanPressureRiseCurve(m);
  CurveLinear ductStaticPressureResetCurve(m);
  CurveExponentialSkewNormal normalizedFanStaticEfficiencyCurveNonStallRegion(m);
  CurveExponentialSkewNormal normalizedFanStaticEfficiencyCurveStallRegion(m);
  CurveSigmoid normalizedDimensionlessAirflowCurveNonStallRegion(m);
  CurveSigmoid normalizedDimensionlessAirflowCurveStallRegion(m);

  // Call Ctor that takes required curve objects
  FanComponentModel fanComponentModel(m, fanPressureRiseCurve, ductStaticPressureResetCurve, normalizedFanStaticEfficiencyCurveNonStallRegion,
                                      normalizedFanStaticEfficiencyCurveStallRegion, normalizedDimensionlessAirflowCurveNonStallRegion,
                                      normalizedDimensionlessAirflowCurveStallRegion);

  // Fan Pressure Rise Curve Name: Required Object
  EXPECT_EQ(fanPressureRiseCurve, fanComponentModel.fanPressureRiseCurve());

  // Duct Static Pressure Reset Curve Name: Required Object
  EXPECT_EQ(ductStaticPressureResetCurve, fanComponentModel.ductStaticPressureResetCurve());

  // Normalized Fan Static Efficiency Curve Name-Non-Stall Region: Required Object
  EXPECT_EQ(normalizedFanStaticEfficiencyCurveNonStallRegion, fanComponentModel.normalizedFanStaticEfficiencyCurveNonStallRegion());

  // Normalized Fan Static Efficiency Curve Name-Stall Region: Required Object
  EXPECT_EQ(normalizedFanStaticEfficiencyCurveStallRegion, fanComponentModel.normalizedFanStaticEfficiencyCurveStallRegion());

  // Normalized Dimensionless Airflow Curve Name-Non-Stall Region: Required Object
  EXPECT_EQ(normalizedDimensionlessAirflowCurveNonStallRegion, fanComponentModel.normalizedDimensionlessAirflowCurveNonStallRegion());

  // Normalized Dimensionless Airflow Curve Name-Stall Region: Required Object
  EXPECT_EQ(normalizedDimensionlessAirflowCurveStallRegion, fanComponentModel.normalizedDimensionlessAirflowCurveStallRegion());

  // Optional Curves: Not initialized
  EXPECT_FALSE(fanComponentModel.maximumBeltEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
  EXPECT_FALSE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
  EXPECT_FALSE(fanComponentModel.maximumMotorEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.normalizedMotorEfficiencyCurve());
  EXPECT_FALSE(fanComponentModel.vFDEfficiencyCurve());

  // Now we call the helper
  EXPECT_TRUE(fanComponentModel.assignDefaultOptionalCurves());
  EXPECT_TRUE(fanComponentModel.maximumBeltEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
  EXPECT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
  EXPECT_TRUE(fanComponentModel.maximumMotorEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.normalizedMotorEfficiencyCurve());
  EXPECT_TRUE(fanComponentModel.vFDEfficiencyCurve());
}

TEST_F(ModelFixture, FanComponentModel_GettersSetters) {
  Model m;
  FanComponentModel fanComponentModel(m);

  fanComponentModel.setName("My FanComponentModel");

  // Availability Schedule Name: Required Object
  ScheduleConstant obj(m);
  EXPECT_TRUE(fanComponentModel.setAvailabilitySchedule(obj));
  EXPECT_EQ(obj, fanComponentModel.availabilitySchedule());

  // Maximum Flow Rate: Required Double, Autosizable
  fanComponentModel.autosizeMaximumFlowRate();
  EXPECT_TRUE(fanComponentModel.isMaximumFlowRateAutosized());
  EXPECT_TRUE(fanComponentModel.setMaximumFlowRate(0.35));
  EXPECT_EQ(0.35, fanComponentModel.maximumFlowRate().get());
  EXPECT_FALSE(fanComponentModel.isMaximumFlowRateAutosized());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumFlowRate(-10.0));
  EXPECT_EQ(0.35, fanComponentModel.maximumFlowRate().get());

  // Minimum Flow Rate: Required Double, Autosizable
  fanComponentModel.autosizeMinimumFlowRate();
  EXPECT_TRUE(fanComponentModel.isMinimumFlowRateAutosized());
  EXPECT_TRUE(fanComponentModel.setMinimumFlowRate(0.11));
  EXPECT_EQ(0.11, fanComponentModel.minimumFlowRate().get());
  EXPECT_FALSE(fanComponentModel.isMinimumFlowRateAutosized());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMinimumFlowRate(-10.0));
  EXPECT_EQ(0.11, fanComponentModel.minimumFlowRate().get());

  // Fan Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setFanSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.fanSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.fanSizingFactor());

  // Fan Wheel Diameter: Required Double
  EXPECT_TRUE(fanComponentModel.setFanWheelDiameter(0.1));
  EXPECT_EQ(0.1, fanComponentModel.fanWheelDiameter());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanWheelDiameter(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.fanWheelDiameter());

  // Fan Outlet Area: Required Double
  EXPECT_TRUE(fanComponentModel.setFanOutletArea(0.1));
  EXPECT_EQ(0.1, fanComponentModel.fanOutletArea());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanOutletArea(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.fanOutletArea());

  // Maximum Fan Static Efficiency: Required Double
  EXPECT_TRUE(fanComponentModel.setMaximumFanStaticEfficiency(0.5));
  EXPECT_EQ(0.5, fanComponentModel.maximumFanStaticEfficiency());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumFanStaticEfficiency(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.maximumFanStaticEfficiency());

  // Euler Number at Maximum Fan Static Efficiency: Required Double
  EXPECT_TRUE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(0.1));
  EXPECT_EQ(0.1, fanComponentModel.eulerNumberatMaximumFanStaticEfficiency());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.eulerNumberatMaximumFanStaticEfficiency());

  // Maximum Dimensionless Fan Airflow: Required Double
  EXPECT_TRUE(fanComponentModel.setMaximumDimensionlessFanAirflow(0.1));
  EXPECT_EQ(0.1, fanComponentModel.maximumDimensionlessFanAirflow());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumDimensionlessFanAirflow(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.maximumDimensionlessFanAirflow());

  // Motor Fan Pulley Ratio: Required Double
  fanComponentModel.autosizeMotorFanPulleyRatio();
  EXPECT_TRUE(fanComponentModel.isMotorFanPulleyRatioAutosized());
  EXPECT_TRUE(fanComponentModel.setMotorFanPulleyRatio(0.1));
  ASSERT_TRUE(fanComponentModel.motorFanPulleyRatio());
  EXPECT_EQ(0.1, fanComponentModel.motorFanPulleyRatio().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorFanPulleyRatio(-10.0));
  ASSERT_TRUE(fanComponentModel.motorFanPulleyRatio());
  EXPECT_EQ(0.1, fanComponentModel.motorFanPulleyRatio().get());
  EXPECT_FALSE(fanComponentModel.isMotorFanPulleyRatioAutosized());

  // Belt Maximum Torque: Required Double
  fanComponentModel.autosizeBeltMaximumTorque();
  EXPECT_TRUE(fanComponentModel.isBeltMaximumTorqueAutosized());
  EXPECT_TRUE(fanComponentModel.setBeltMaximumTorque(0.1));
  ASSERT_TRUE(fanComponentModel.beltMaximumTorque());
  EXPECT_EQ(0.1, fanComponentModel.beltMaximumTorque().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltMaximumTorque(-10.0));
  ASSERT_TRUE(fanComponentModel.beltMaximumTorque());
  EXPECT_EQ(0.1, fanComponentModel.beltMaximumTorque().get());
  EXPECT_FALSE(fanComponentModel.isBeltMaximumTorqueAutosized());

  // Belt Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setBeltSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.beltSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.beltSizingFactor());

  // Belt Fractional Torque Transition: Required Double
  EXPECT_TRUE(fanComponentModel.setBeltFractionalTorqueTransition(0.5));
  EXPECT_EQ(0.5, fanComponentModel.beltFractionalTorqueTransition());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltFractionalTorqueTransition(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.beltFractionalTorqueTransition());

  // Motor Maximum Speed: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorMaximumSpeed(0.1));
  EXPECT_EQ(0.1, fanComponentModel.motorMaximumSpeed());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorMaximumSpeed(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.motorMaximumSpeed());

  // Maximum Motor Output Power: Required Double
  fanComponentModel.autosizeMaximumMotorOutputPower();
  EXPECT_TRUE(fanComponentModel.isMaximumMotorOutputPowerAutosized());
  EXPECT_TRUE(fanComponentModel.setMaximumMotorOutputPower(0.1));
  ASSERT_TRUE(fanComponentModel.maximumMotorOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumMotorOutputPower().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumMotorOutputPower(-10.0));
  ASSERT_TRUE(fanComponentModel.maximumMotorOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumMotorOutputPower().get());
  EXPECT_FALSE(fanComponentModel.isMaximumMotorOutputPowerAutosized());

  // Motor Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.motorSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.motorSizingFactor());

  // Motor In Airstream Fraction: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorInAirstreamFraction(0.5));
  EXPECT_EQ(0.5, fanComponentModel.motorInAirstreamFraction());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorInAirstreamFraction(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.motorInAirstreamFraction());

  // VFD Efficiency Type: Optional String
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyType("Speed"));
  EXPECT_EQ("Speed", fanComponentModel.vFDEfficiencyType());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setVFDEfficiencyType("BADENUM"));
  EXPECT_EQ("Speed", fanComponentModel.vFDEfficiencyType());

  // Maximum VFD Output Power: Required Double
  fanComponentModel.autosizeMaximumVFDOutputPower();
  EXPECT_TRUE(fanComponentModel.isMaximumVFDOutputPowerAutosized());
  EXPECT_TRUE(fanComponentModel.setMaximumVFDOutputPower(0.1));
  ASSERT_TRUE(fanComponentModel.maximumVFDOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumVFDOutputPower().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumVFDOutputPower(-10.0));
  ASSERT_TRUE(fanComponentModel.maximumVFDOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumVFDOutputPower().get());
  EXPECT_FALSE(fanComponentModel.isMaximumVFDOutputPowerAutosized());

  // VFD Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setVFDSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.vFDSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setVFDSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.vFDSizingFactor());

  // Fan Pressure Rise Curve Name: Required Object
  {
    CurveFanPressureRise obj(m);
    EXPECT_TRUE(fanComponentModel.setFanPressureRiseCurve(obj));
    EXPECT_EQ(obj, fanComponentModel.fanPressureRiseCurve());
  }

  // Duct Static Pressure Reset Curve Name: Required Object
  {
    CurveLinear obj(m);
    EXPECT_TRUE(fanComponentModel.setDuctStaticPressureResetCurve(obj));
    EXPECT_EQ(obj, fanComponentModel.ductStaticPressureResetCurve());
  }

  // Normalized Fan Static Efficiency Curve Name-Non-Stall Region: Required Object
  {
    CurveExponentialSkewNormal obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveNonStallRegion(obj));
    EXPECT_EQ(obj, fanComponentModel.normalizedFanStaticEfficiencyCurveNonStallRegion());
  }

  // Normalized Fan Static Efficiency Curve Name-Stall Region: Required Object
  {
    CurveExponentialSkewNormal obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveStallRegion(obj));
    EXPECT_EQ(obj, fanComponentModel.normalizedFanStaticEfficiencyCurveStallRegion());
  }

  // Normalized Dimensionless Airflow Curve Name-Non-Stall Region: Required Object
  {
    CurveSigmoid obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveNonStallRegion(obj));
    EXPECT_EQ(obj, fanComponentModel.normalizedDimensionlessAirflowCurveNonStallRegion());
  }

  // Normalized Dimensionless Airflow Curve Name-Stall Region: Required Object
  {
    CurveSigmoid obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveStallRegion(obj));
    EXPECT_EQ(obj, fanComponentModel.normalizedDimensionlessAirflowCurveStallRegion());
  }

  // Maximum Belt Efficiency Curve Name: Optional Object
  {
    CurveQuartic obj(m);
    EXPECT_TRUE(fanComponentModel.setMaximumBeltEfficiencyCurve(obj));
    ASSERT_TRUE(fanComponentModel.maximumBeltEfficiencyCurve());
    EXPECT_EQ(obj, fanComponentModel.maximumBeltEfficiencyCurve().get());
  }

  // Normalized Belt Efficiency Curve Name - Region 1: Optional Object
  {
    CurveRectangularHyperbola2 obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion1(obj));
    ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
    EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion1().get());
  }

  // Normalized Belt Efficiency Curve Name - Region 2: Optional Object
  {
    CurveExponentialDecay obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion2(obj));
    ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
    EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion2().get());
  }

  // Normalized Belt Efficiency Curve Name - Region 3: Optional Object
  {
    CurveRectangularHyperbola2 obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion3(obj));
    ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
    EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion3().get());
  }

  // Maximum Motor Efficiency Curve Name: Optional Object
  {
    CurveRectangularHyperbola1 obj(m);
    EXPECT_TRUE(fanComponentModel.setMaximumMotorEfficiencyCurve(obj));
    ASSERT_TRUE(fanComponentModel.maximumMotorEfficiencyCurve());
    EXPECT_EQ(obj, fanComponentModel.maximumMotorEfficiencyCurve().get());
  }

  // Normalized Motor Efficiency Curve Name: Optional Object
  {
    CurveRectangularHyperbola2 obj(m);
    EXPECT_TRUE(fanComponentModel.setNormalizedMotorEfficiencyCurve(obj));
    ASSERT_TRUE(fanComponentModel.normalizedMotorEfficiencyCurve());
    EXPECT_EQ(obj, fanComponentModel.normalizedMotorEfficiencyCurve().get());
  }

  // VFD Efficiency Curve Name: Optional Object
  {
    CurveRectangularHyperbola2 obj(m);
    EXPECT_TRUE(fanComponentModel.setVFDEfficiencyCurve(obj));
    ASSERT_TRUE(fanComponentModel.vFDEfficiencyCurve());
    EXPECT_EQ(obj, fanComponentModel.vFDEfficiencyCurve().get());
  }

  // End-Use Subcategory: Required String
  EXPECT_TRUE(fanComponentModel.setEndUseSubcategory("Fans"));
  EXPECT_EQ("Fans", fanComponentModel.endUseSubcategory());
}

TEST_F(ModelFixture, FanComponentModel_FullyDefinedExample) {

  // Taken from EnergyPlus/testfiles/5ZoneWarmestVFD_FCMAuto.idf, v9.4.0
  Model m;
  FanComponentModel fanComponentModel(m);

  CurveFanPressureRise vSDExample(m);
  vSDExample.setName("VSD Example");
  vSDExample.setCoefficient1C1(1446.75833497653);
  vSDExample.setCoefficient2C2(0.0);
  vSDExample.setCoefficient3C3(0.0);
  vSDExample.setCoefficient4C4(1.0);
  vSDExample.setMinimumValueofQfan(0.0);
  vSDExample.setMaximumValueofQfan(100.0);
  vSDExample.setMinimumValueofPsm(62.5);
  vSDExample.setMaximumValueofPsm(300.0);
  vSDExample.setMinimumCurveOutput(0.0);
  vSDExample.setMaximumCurveOutput(5000.0);

  CurveLinear diagnosticSPR(m);
  diagnosticSPR.setName("DiagnosticSPR");
  diagnosticSPR.setCoefficient1Constant(248.84);
  diagnosticSPR.setCoefficient2x(0.0);
  diagnosticSPR.setMinimumValueofx(0.0);
  diagnosticSPR.setMaximumValueofx(100.0);
  diagnosticSPR.setMinimumCurveOutput(62.5);
  diagnosticSPR.setMaximumCurveOutput(248.84);
  // diagnosticSPR.setInputUnitTypeforX("");
  // diagnosticSPR.setOutputUnitType("");

  CurveExponentialSkewNormal fanEff120CPLANormal(m);
  fanEff120CPLANormal.setName("FanEff120CPLANormal");
  fanEff120CPLANormal.setCoefficient1C1(0.072613);
  fanEff120CPLANormal.setCoefficient2C2(0.833213);
  fanEff120CPLANormal.setCoefficient3C3(0.0);
  fanEff120CPLANormal.setCoefficient4C4(0.013911);
  fanEff120CPLANormal.setMinimumValueofx(-4.0);
  fanEff120CPLANormal.setMaximumValueofx(5.0);
  fanEff120CPLANormal.setMinimumCurveOutput(0.1);
  fanEff120CPLANormal.setMaximumCurveOutput(1.0);
  // fanEff120CPLANormal.setInputUnitTypeforx("");
  // fanEff120CPLANormal.setOutputUnitType("");

  CurveExponentialSkewNormal fanEff120CPLAStall(m);
  fanEff120CPLAStall.setName("FanEff120CPLAStall");
  fanEff120CPLAStall.setCoefficient1C1(-1.674931);
  fanEff120CPLAStall.setCoefficient2C2(1.980182);
  fanEff120CPLAStall.setCoefficient3C3(0.0);
  fanEff120CPLAStall.setCoefficient4C4(1.84495);
  fanEff120CPLAStall.setMinimumValueofx(-4.0);
  fanEff120CPLAStall.setMaximumValueofx(5.0);
  fanEff120CPLAStall.setMinimumCurveOutput(0.1);
  fanEff120CPLAStall.setMaximumCurveOutput(1.0);
  // fanEff120CPLAStall.setInputUnitTypeforx("");
  // fanEff120CPLAStall.setOutputUnitType("");

  CurveSigmoid fanDimFlowNormal(m);
  fanDimFlowNormal.setName("FanDimFlowNormal");
  fanDimFlowNormal.setCoefficient1C1(0.0);
  fanDimFlowNormal.setCoefficient2C2(1.001423);
  fanDimFlowNormal.setCoefficient3C3(0.123935);
  fanDimFlowNormal.setCoefficient4C4(-0.476026);
  fanDimFlowNormal.setCoefficient5C5(1.0);
  fanDimFlowNormal.setMinimumValueofx(-4.0);
  fanDimFlowNormal.setMaximumValueofx(5.0);
  fanDimFlowNormal.setMinimumCurveOutput(0.05);
  fanDimFlowNormal.setMaximumCurveOutput(1.0);
  // fanDimFlowNormal.setInputUnitTypeforx("");
  // fanDimFlowNormal.setOutputUnitType("");

  CurveSigmoid fanDimFlowStall(m);
  fanDimFlowStall.setName("FanDimFlowStall");
  fanDimFlowStall.setCoefficient1C1(0.0);
  fanDimFlowStall.setCoefficient2C2(5.924993);
  fanDimFlowStall.setCoefficient3C3(-1.91636);
  fanDimFlowStall.setCoefficient4C4(-0.851779);
  fanDimFlowStall.setCoefficient5C5(1.0);
  fanDimFlowStall.setMinimumValueofx(-4.0);
  fanDimFlowStall.setMaximumValueofx(5.0);
  fanDimFlowStall.setMinimumCurveOutput(0.05);
  fanDimFlowStall.setMaximumCurveOutput(1.0);
  // fanDimFlowStall.setInputUnitTypeforx("");
  // fanDimFlowStall.setOutputUnitType("");

  CurveQuartic beltMaxEffMedium(m);
  beltMaxEffMedium.setName("BeltMaxEffMedium");
  beltMaxEffMedium.setCoefficient1Constant(-0.09504);
  beltMaxEffMedium.setCoefficient2x(0.03415);
  beltMaxEffMedium.setCoefficient3xPOW2(-0.008897);
  beltMaxEffMedium.setCoefficient4xPOW3(0.001159);
  beltMaxEffMedium.setCoefficient5xPOW4(-6.132e-05);
  beltMaxEffMedium.setMinimumValueofx(-1.2);
  beltMaxEffMedium.setMaximumValueofx(6.2);
  beltMaxEffMedium.setMinimumCurveOutput(-4.6);
  beltMaxEffMedium.setMaximumCurveOutput(0.0);
  // beltMaxEffMedium.setInputUnitTypeforX("");
  // beltMaxEffMedium.setOutputUnitType("");

  CurveRectangularHyperbola2 beltPartLoadRegion1(m);
  beltPartLoadRegion1.setName("BeltPartLoadRegion1");
  beltPartLoadRegion1.setCoefficient1C1(0.920797);
  beltPartLoadRegion1.setCoefficient2C2(0.0262686);
  beltPartLoadRegion1.setCoefficient3C3(0.151594);
  beltPartLoadRegion1.setMinimumValueofx(0.0);
  beltPartLoadRegion1.setMaximumValueofx(1.0);
  beltPartLoadRegion1.setMinimumCurveOutput(0.01);
  beltPartLoadRegion1.setMaximumCurveOutput(1.0);
  // beltPartLoadRegion1.setInputUnitTypeforx("");
  // beltPartLoadRegion1.setOutputUnitType("");

  CurveExponentialDecay beltPartLoadRegion2(m);
  beltPartLoadRegion2.setName("BeltPartLoadRegion2");
  beltPartLoadRegion2.setCoefficient1C1(1.011965);
  beltPartLoadRegion2.setCoefficient2C2(-0.339038);
  beltPartLoadRegion2.setCoefficient3C3(-3.43626);
  beltPartLoadRegion2.setMinimumValueofx(0.0);
  beltPartLoadRegion2.setMaximumValueofx(1.0);
  beltPartLoadRegion2.setMinimumCurveOutput(0.01);
  beltPartLoadRegion2.setMaximumCurveOutput(1.0);
  // beltPartLoadRegion2.setInputUnitTypeforx("");
  // beltPartLoadRegion2.setOutputUnitType("");

  CurveRectangularHyperbola2 beltPartLoadRegion3(m);
  beltPartLoadRegion3.setName("BeltPartLoadRegion3");
  beltPartLoadRegion3.setCoefficient1C1(1.037778);
  beltPartLoadRegion3.setCoefficient2C2(0.0103068);
  beltPartLoadRegion3.setCoefficient3C3(-0.0268146);
  beltPartLoadRegion3.setMinimumValueofx(0.0);
  beltPartLoadRegion3.setMaximumValueofx(1.0);
  beltPartLoadRegion3.setMinimumCurveOutput(0.01);
  beltPartLoadRegion3.setMaximumCurveOutput(1.0);
  // beltPartLoadRegion3.setInputUnitTypeforx("");
  // beltPartLoadRegion3.setOutputUnitType("");

  CurveRectangularHyperbola1 motorMaxEffAvg(m);
  motorMaxEffAvg.setName("MotorMaxEffAvg");
  motorMaxEffAvg.setCoefficient1C1(0.29228);
  motorMaxEffAvg.setCoefficient2C2(3.368739);
  motorMaxEffAvg.setCoefficient3C3(0.762471);
  motorMaxEffAvg.setMinimumValueofx(0.0);
  motorMaxEffAvg.setMaximumValueofx(7.6);
  motorMaxEffAvg.setMinimumCurveOutput(0.01);
  motorMaxEffAvg.setMaximumCurveOutput(1.0);
  // motorMaxEffAvg.setInputUnitTypeforx("");
  // motorMaxEffAvg.setOutputUnitType("");

  CurveRectangularHyperbola2 motorPartLoad(m);
  motorPartLoad.setName("MotorPartLoad");
  motorPartLoad.setCoefficient1C1(1.137209);
  motorPartLoad.setCoefficient2C2(0.0502359);
  motorPartLoad.setCoefficient3C3(-0.0891503);
  motorPartLoad.setMinimumValueofx(0.0);
  motorPartLoad.setMaximumValueofx(1.0);
  motorPartLoad.setMinimumCurveOutput(0.01);
  motorPartLoad.setMaximumCurveOutput(1.0);
  // motorPartLoad.setInputUnitTypeforx("");
  // motorPartLoad.setOutputUnitType("");

  CurveRectangularHyperbola2 vFDPartLoad(m);
  vFDPartLoad.setName("VFDPartLoad");
  vFDPartLoad.setCoefficient1C1(0.987405);
  vFDPartLoad.setCoefficient2C2(0.0155361);
  vFDPartLoad.setCoefficient3C3(-0.0059365);
  vFDPartLoad.setMinimumValueofx(0.0);
  vFDPartLoad.setMaximumValueofx(1.0);
  vFDPartLoad.setMinimumCurveOutput(0.01);
  vFDPartLoad.setMaximumCurveOutput(1.0);
  // vFDPartLoad.setInputUnitTypeforx("");
  // vFDPartLoad.setOutputUnitType("");

  fanComponentModel.setName("Supply Fan 1");
  fanComponentModel.autosizeMaximumFlowRate();
  EXPECT_TRUE(fanComponentModel.setMinimumFlowRate(0.0));
  EXPECT_TRUE(fanComponentModel.setFanSizingFactor(1.0));
  EXPECT_TRUE(fanComponentModel.setFanWheelDiameter(0.3048));
  EXPECT_TRUE(fanComponentModel.setFanOutletArea(0.0873288576));
  EXPECT_TRUE(fanComponentModel.setMaximumFanStaticEfficiency(0.514));
  EXPECT_TRUE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(9.76));
  EXPECT_TRUE(fanComponentModel.setMaximumDimensionlessFanAirflow(0.160331811647483));
  fanComponentModel.autosizeMotorFanPulleyRatio();
  fanComponentModel.autosizeBeltMaximumTorque();
  EXPECT_TRUE(fanComponentModel.setBeltSizingFactor(1.0));
  EXPECT_TRUE(fanComponentModel.setBeltFractionalTorqueTransition(0.167));
  EXPECT_TRUE(fanComponentModel.setMotorMaximumSpeed(1800.0));
  fanComponentModel.autosizeMaximumMotorOutputPower();
  EXPECT_TRUE(fanComponentModel.setMotorSizingFactor(1.0));
  EXPECT_TRUE(fanComponentModel.setMotorInAirstreamFraction(1.0));
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyType("Power"));
  fanComponentModel.autosizeMaximumVFDOutputPower();
  EXPECT_TRUE(fanComponentModel.setVFDSizingFactor(1.0));
  EXPECT_TRUE(fanComponentModel.setFanPressureRiseCurve(vSDExample));
  EXPECT_TRUE(fanComponentModel.setDuctStaticPressureResetCurve(diagnosticSPR));
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveNonStallRegion(fanEff120CPLANormal));
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveStallRegion(fanEff120CPLAStall));
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveNonStallRegion(fanDimFlowNormal));
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveStallRegion(fanDimFlowStall));
  EXPECT_TRUE(fanComponentModel.setMaximumBeltEfficiencyCurve(beltMaxEffMedium));
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion1(beltPartLoadRegion1));
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion2(beltPartLoadRegion2));
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion3(beltPartLoadRegion3));
  EXPECT_TRUE(fanComponentModel.setMaximumMotorEfficiencyCurve(motorMaxEffAvg));
  EXPECT_TRUE(fanComponentModel.setNormalizedMotorEfficiencyCurve(motorPartLoad));
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyCurve(vFDPartLoad));
}

// OS:AirLoopHVAC
TEST_F(ModelFixture, FanComponentModel_addToNode) {
  Model m;
  FanComponentModel fan(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(fan.airLoopHVAC());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.airLoopHVAC());
  ASSERT_TRUE(airLoop.supplyFan());
  EXPECT_EQ(fan, airLoop.supplyFan().get());

  Node demandNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(fan.addToNode(demandNode));
  // 5u: inlet splitter node mixer outlet.
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.airLoopHVAC());

  EXPECT_TRUE(fan.removeFromLoop());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_FALSE(fan.airLoopHVAC());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(fan.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(fan.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

/********************************************************************************************************************
*                                        H V A C    C O M P O N E N T S                                             *
********************************************************************************************************************/

// OS:AirLoopHVAC:UnitarySystem
TEST_F(ModelFixture, FanComponentModel_containingHVACComponent_AirLoopHVACUnitarySystem) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem unitary(m);
  FanComponentModel fan(m);
  CoilHeatingElectric heatingCoil(m);
  CoilHeatingElectric suppHeatingCoil(m);
  CoilCoolingWater coolingCoil(m);

  EXPECT_TRUE(unitary.setSupplyFan(fan));
  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), unitary.handle());
}

/********************************************************************************************************************
*                                 Z O N E    H V A C    C O M P O N E N T S                                         *
********************************************************************************************************************/

// OS:ZoneHVAC:EvaporativeCoolerUnit - Not wrapped in SDK yet
// TEST_F(ModelFixture, FanComponentModel_containingZoneHVACComponent_ZoneHVACEvaporativeCoolerUnit) {
//   Model m;
//   Schedule s = m.alwaysOnDiscreteSchedule();
//   FanComponentModel fan(m);
//
//   ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);
//
//   EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSupplyAirFan(fan));
//
//   boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
//   ASSERT_TRUE(component);
//   EXPECT_EQ(component.get().handle(), zoneHVACEvaporativeCoolerUnit.handle());
// }

/********************************************************************************************************************
*                                 N O T    S U P P O R T E D                                                        *
********************************************************************************************************************/

// Random example
TEST_F(ModelFixture, FanComponentModel_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanComponentModel fan(m);
  FanOnOff fanOnOff(m);
  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir unitary(m, s, fanOnOff, heatingCoil, coolingCoil, supplementalHeatingCoil);
  EXPECT_FALSE(unitary.setSupplyAirFan(fan));
}
