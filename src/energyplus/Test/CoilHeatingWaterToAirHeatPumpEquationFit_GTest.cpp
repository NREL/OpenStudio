/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveQuadLinear.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingWaterToAirHeatPumpEquationFit) {

  ForwardTranslator ft;

  Model m;
  CoilHeatingWaterToAirHeatPumpEquationFit coil(m);

  coil.setName("My CoilHeatingWaterToAirHeatPumpEquationFit");

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  coil.waterInletModelObject()->setName("Coil Water Inlet Node");
  coil.waterOutletModelObject()->setName("Coil Water Outlet Node");

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setHeatingCoil(coil);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ASSERT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.inletNode()->setName("Coil Air Inlet Node"));
  ASSERT_TRUE(unitary.outletNode());
  EXPECT_TRUE(unitary.outletNode()->setName("Coil Air Outlet Node"));

  EXPECT_TRUE(coil.setRatedAirFlowRate(0.6));
  EXPECT_TRUE(coil.setRatedWaterFlowRate(0.07));
  EXPECT_TRUE(coil.setRatedHeatingCapacity(4000.0));
  EXPECT_TRUE(coil.setRatedHeatingCoefficientofPerformance(0.9));
  EXPECT_TRUE(coil.setRatedEnteringWaterTemperature(23.0));
  EXPECT_TRUE(coil.setRatedEnteringAirDryBulbTemperature(19.0));
  EXPECT_TRUE(coil.setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(1.2));

  coil.heatingCapacityCurve().setName("HeatCapCurve");
  coil.heatingPowerConsumptionCurve().setName("HeatPowerCurve");
  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  const Workspace w = ft.translateModel(m);
  const auto idf_coils = w.getObjectsByType(IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit);
  ASSERT_EQ(1u, idf_coils.size());

  const auto& idf_coil = idf_coils.front();

  EXPECT_EQ("My CoilHeatingWaterToAirHeatPumpEquationFit", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::Name).get());
  EXPECT_EQ("Coil Water Inlet Node", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName).get());
  EXPECT_EQ("Coil Water Outlet Node", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName).get());
  EXPECT_EQ("Coil Air Inlet Node", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName).get());
  EXPECT_EQ("Coil Air Outlet Node", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName).get());
  EXPECT_EQ(0.6, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate).get());
  EXPECT_EQ(0.07, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity).get());
  EXPECT_EQ(0.9, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCOP).get());
  EXPECT_EQ(23.0, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature).get());
  EXPECT_EQ(19.0, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature).get());
  EXPECT_EQ(1.2, idf_coil.getDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity).get());
  EXPECT_EQ("HeatCapCurve", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName).get());
  EXPECT_EQ("HeatPowerCurve", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::PartLoadFractionCorrelationCurveName).get());
}
