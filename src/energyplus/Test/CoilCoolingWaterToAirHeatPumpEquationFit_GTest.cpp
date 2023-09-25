/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveQuadLinear.hpp"
#include "../../model/CurveQuintLinear.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingWaterToAirHeatPumpEquationFit) {

  ForwardTranslator ft;

  Model m;
  CoilCoolingWaterToAirHeatPumpEquationFit coil(m);

  coil.setName("My CoilCoolingWaterToAirHeatPumpEquationFit");

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  coil.waterInletModelObject()->setName("Coil Water Inlet Node");
  coil.waterOutletModelObject()->setName("Coil Water Outlet Node");

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ASSERT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.inletNode()->setName("Coil Air Inlet Node"));
  ASSERT_TRUE(unitary.outletNode());
  EXPECT_TRUE(unitary.outletNode()->setName("Coil Air Outlet Node"));

  EXPECT_TRUE(coil.setRatedAirFlowRate(0.6));
  EXPECT_TRUE(coil.setRatedWaterFlowRate(0.07));
  EXPECT_TRUE(coil.setRatedTotalCoolingCapacity(4000.0));
  EXPECT_TRUE(coil.setRatedSensibleCoolingCapacity(3700.0));
  EXPECT_TRUE(coil.setRatedCoolingCoefficientofPerformance(0.9));
  EXPECT_TRUE(coil.setRatedEnteringWaterTemperature(31.0));
  EXPECT_TRUE(coil.setRatedEnteringAirDryBulbTemperature(26.0));
  EXPECT_TRUE(coil.setRatedEnteringAirWetBulbTemperature(18.7));

  coil.totalCoolingCapacityCurve().setName("TotCoolCapCurve");
  coil.sensibleCoolingCapacityCurve().setName("SensCoolCapCurve");
  coil.coolingPowerConsumptionCurve().setName("CoolPowerCurve");
  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  EXPECT_TRUE(coil.setNominalTimeforCondensateRemovaltoBegin(1.9));
  EXPECT_TRUE(coil.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.08));
  EXPECT_TRUE(coil.setMaximumCyclingRate(3.5));
  EXPECT_TRUE(coil.setLatentCapacityTimeConstant(65.0));
  EXPECT_TRUE(coil.setFanDelayTime(90.0));

  const Workspace w = ft.translateModel(m);
  const auto idf_coils = w.getObjectsByType(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit);
  ASSERT_EQ(1u, idf_coils.size());

  const auto& idf_coil = idf_coils.front();

  EXPECT_EQ("My CoilCoolingWaterToAirHeatPumpEquationFit", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::Name).get());
  EXPECT_EQ("Coil Water Inlet Node", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName).get());
  EXPECT_EQ("Coil Water Outlet Node", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName).get());
  EXPECT_EQ("Coil Air Inlet Node", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get());
  EXPECT_EQ("Coil Air Outlet Node", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get());
  EXPECT_EQ(0.6, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate).get());
  EXPECT_EQ(0.07, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity).get());
  EXPECT_EQ(3700.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity).get());
  EXPECT_EQ(0.9, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP).get());
  EXPECT_EQ(31.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature).get());
  EXPECT_EQ(26.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature).get());
  EXPECT_EQ(18.7, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature).get());
  EXPECT_EQ("TotCoolCapCurve", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName).get());
  EXPECT_EQ("SensCoolCapCurve", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName).get());
  EXPECT_EQ("CoolPowerCurve", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::PartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ(1.9, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin).get());
  EXPECT_EQ(
    0.08,
    idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity).get());
  EXPECT_EQ(3.5, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::MaximumCyclingRate).get());
  EXPECT_EQ(65.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::LatentCapacityTimeConstant).get());
  EXPECT_EQ(90.0, idf_coil.getDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::FanDelayTime).get());
}
