/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit) {

  ForwardTranslator ft;

  Model m;
  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);

  {
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData speed(m);
    speed.setName("DX VSD Speed 1");
    EXPECT_TRUE(speed.setName("DX VSD Speed 1"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedTotalCoolingCapacity(4000.0));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedSensibleHeatRatio(0.75));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedCoolingCOP(4.5));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(0.5));
    EXPECT_TRUE(speed.setReferenceUnitRatedWaterFlowRate(0.04));
    speed.totalCoolingCapacityFunctionofTemperatureCurve().setName("Speed1 CoolCapFT");
    speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().setName("Speed1 TotCoolCapfAirFlow");
    speed.totalCoolingCapacityFunctionofWaterFlowFractionCurve().setName("Speed1 TotHeatCapfWaterFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed1 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed1 EIRfAirFlow");
    speed.energyInputRatioFunctionofWaterFlowFractionCurve().setName("Speed1 EIRfWaterFlow");
    EXPECT_TRUE(speed.setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.1));
    speed.wasteHeatFunctionofTemperatureCurve().setName("Speed1 WasteHeatFT");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  {
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData speed(m);
    speed.setName("DX VSD Speed 2");
    EXPECT_TRUE(speed.setName("DX VSD Speed 2"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedTotalCoolingCapacity(8000.0));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedSensibleHeatRatio(0.79));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedCoolingCOP(4.0));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(1.0));
    EXPECT_TRUE(speed.setReferenceUnitRatedWaterFlowRate(0.08));
    speed.totalCoolingCapacityFunctionofTemperatureCurve().setName("Speed2 CoolCapFT");
    speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().setName("Speed2 TotCoolCapfAirFlow");
    speed.totalCoolingCapacityFunctionofWaterFlowFractionCurve().setName("Speed2 TotHeatCapfWaterFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed2 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed2 EIRfAirFlow");
    speed.energyInputRatioFunctionofWaterFlowFractionCurve().setName("Speed2 EIRfWaterFlow");
    EXPECT_TRUE(speed.setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.2));
    speed.wasteHeatFunctionofTemperatureCurve().setName("Speed2 WasteHeatFT");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  coil.setName("My CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  coil.waterInletModelObject()->setName("Coil Water Inlet Node");
  coil.waterOutletModelObject()->setName("Coil Water Outlet Node");

  AirLoopHVACUnitarySystem unitary(m);
  EXPECT_TRUE(unitary.setCoolingCoil(coil));

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(unitary.addToNode(supplyOutletNode));

  ASSERT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.inletNode()->setName("Coil Air Inlet Node"));
  ASSERT_TRUE(unitary.outletNode());
  EXPECT_TRUE(unitary.outletNode()->setName("Coil Air Outlet Node"));

  EXPECT_TRUE(coil.setNominalSpeedLevel(2));
  EXPECT_TRUE(coil.setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(8000.0));
  EXPECT_TRUE(coil.setRatedAirFlowRateAtSelectedNominalSpeedLevel(1.0));
  EXPECT_TRUE(coil.setRatedWaterFlowRateAtSelectedNominalSpeedLevel(0.08));

  EXPECT_TRUE(coil.setNominalTimeforCondensatetoBeginLeavingtheCoil(1.9));
  EXPECT_TRUE(coil.setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0.08));
  EXPECT_TRUE(coil.setMaximumCyclingRate(3.5));
  EXPECT_TRUE(coil.setLatentCapacityTimeConstant(65.0));
  EXPECT_TRUE(coil.setFanDelayTime(90.0));
  EXPECT_TRUE(coil.setUseHotGasReheat(true));

  coil.energyPartLoadFractionCurve().setName("CoilPLFCurve");

  const Workspace w = ft.translateModel(m);
  const auto idf_coils = w.getObjectsByType(IddObjectType::Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit);
  ASSERT_EQ(1u, idf_coils.size());

  const auto& idf_coil = idf_coils.front();

  EXPECT_EQ("My CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit",
            idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::Name).get());
  EXPECT_EQ("Coil Water Inlet Node",
            idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName).get());
  EXPECT_EQ("Coil Water Outlet Node",
            idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName).get());
  EXPECT_EQ("Coil Air Inlet Node", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName).get());
  EXPECT_EQ("Coil Air Outlet Node",
            idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel).get());
  EXPECT_EQ(
    8000.0,
    idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel)
      .get());
  EXPECT_EQ(1.0,
            idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ(0.08,
            idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ(1.9,
            idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil).get());
  EXPECT_EQ(
    0.08,
    idf_coil
      .getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity)
      .get());
  EXPECT_EQ(3.5, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::MaximumCyclingRate).get());
  EXPECT_EQ(65.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::LatentCapacityTimeConstant).get());
  EXPECT_EQ(90.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::FanDelayTime).get());
  EXPECT_EQ(1, idf_coil.getInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::FlagforUsingHotGasReheat0or1).get());  // This is dumb
  EXPECT_EQ("CoilPLFCurve",
            idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NumberofSpeeds).get());
  ASSERT_EQ(2, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto speeds = coil.speeds();
  for (size_t i = 0; i < egs.size(); ++i) {
    auto eg = egs[i].cast<WorkspaceExtensibleGroup>();  // Casting for getTarget
    const auto speed = speeds[i];

    EXPECT_EQ(
      speed.referenceUnitGrossRatedTotalCoolingCapacity(),
      eg.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedTotalCoolingCapacity).get());
    EXPECT_EQ(
      speed.referenceUnitGrossRatedSensibleHeatRatio(),
      eg.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedSensibleHeatRatio).get());
    EXPECT_EQ(speed.referenceUnitGrossRatedCoolingCOP(),
              eg.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedCoolingCOP).get());
    EXPECT_EQ(speed.referenceUnitRatedAirFlowRate(),
              eg.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedAirFlowRate).get());
    EXPECT_EQ(speed.referenceUnitRatedWaterFlowRate(),
              eg.getDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate).get());

    EXPECT_EQ(
      speed.totalCoolingCapacityFunctionofTemperatureCurve().nameString(),
      eg.getTarget(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
        ->nameString());
    EXPECT_EQ(
      speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().nameString(),
      eg.getTarget(
          Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName)
        ->nameString());
    EXPECT_EQ(
      speed.totalCoolingCapacityFunctionofWaterFlowFractionCurve().nameString(),
      eg.getTarget(
          Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofWaterFlowFractionCurveName)
        ->nameString());

    EXPECT_EQ(
      speed.energyInputRatioFunctionofTemperatureCurve().nameString(),
      eg.getTarget(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName)
        ->nameString());
    EXPECT_EQ(
      speed.energyInputRatioFunctionofAirFlowFractionCurve().nameString(),
      eg.getTarget(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName)
        ->nameString());
    EXPECT_EQ(speed.energyInputRatioFunctionofWaterFlowFractionCurve().nameString(),
              eg.getTarget(
                  Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofWaterFlowFractionCurveName)
                ->nameString());

    EXPECT_EQ(
      speed.referenceUnitWasteHeatFractionofInputPowerAtRatedConditions(),
      eg.getDouble(
          Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions)
        .get());

    EXPECT_EQ(speed.wasteHeatFunctionofTemperatureCurve().nameString(),
              eg.getTarget(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName)
                ->nameString());
  }
}

