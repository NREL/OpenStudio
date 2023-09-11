/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/Boiler_HotWater_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_BoilerHotWater) {
  Model m;

  PlantLoop plant_loop(m);

  BoilerHotWater bhw(m);

  EXPECT_TRUE(bhw.setFuelType("Propane"));
  EXPECT_TRUE(bhw.setNominalCapacity(1.0));
  EXPECT_TRUE(bhw.setNominalThermalEfficiency(2.0));
  EXPECT_TRUE(bhw.setEfficiencyCurveTemperatureEvaluationVariable("EnteringBoiler"));
  CurveQuadratic curve1(m);
  EXPECT_TRUE(bhw.setNormalizedBoilerEfficiencyCurve(curve1));
  EXPECT_TRUE(bhw.setDesignWaterFlowRate(3.0));
  EXPECT_TRUE(bhw.setMinimumPartLoadRatio(4.0));
  EXPECT_TRUE(bhw.setMaximumPartLoadRatio(5.0));
  EXPECT_TRUE(bhw.setOptimumPartLoadRatio(6.0));
  EXPECT_TRUE(bhw.setWaterOutletUpperTemperatureLimit(7.0));
  EXPECT_TRUE(bhw.setBoilerFlowMode("LeavingSetpointModulated"));
  EXPECT_TRUE(bhw.setParasiticElectricLoad(8.0));
  EXPECT_TRUE(bhw.setSizingFactor(9.0));
  EXPECT_TRUE(bhw.setEndUseSubcategory("Test"));
  EXPECT_TRUE(bhw.setOffCycleParasiticFuelLoad(10.0));

  EXPECT_TRUE(plant_loop.addSupplyBranchForComponent(bhw));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Boiler_HotWater).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Curve_Quadratic).size());

  IdfObject idf_bhw = w.getObjectsByType(IddObjectType::Boiler_HotWater)[0];

  EXPECT_EQ(bhw.nameString(), idf_bhw.getString(Boiler_HotWaterFields::Name, false).get());
  EXPECT_EQ("Propane", idf_bhw.getString(Boiler_HotWaterFields::FuelType, false).get());
  EXPECT_EQ(1.0, idf_bhw.getDouble(Boiler_HotWaterFields::NominalCapacity, false).get());
  EXPECT_EQ(2.0, idf_bhw.getDouble(Boiler_HotWaterFields::NominalThermalEfficiency, false).get());
  EXPECT_EQ("EnteringBoiler", idf_bhw.getString(Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, false).get());
  EXPECT_EQ(curve1.nameString(), idf_bhw.getString(Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName, false).get());
  EXPECT_EQ(3.0, idf_bhw.getDouble(Boiler_HotWaterFields::DesignWaterFlowRate, false).get());
  EXPECT_EQ(4.0, idf_bhw.getDouble(Boiler_HotWaterFields::MinimumPartLoadRatio, false).get());
  EXPECT_EQ(5.0, idf_bhw.getDouble(Boiler_HotWaterFields::MaximumPartLoadRatio, false).get());
  EXPECT_EQ(6.0, idf_bhw.getDouble(Boiler_HotWaterFields::OptimumPartLoadRatio, false).get());
  EXPECT_EQ(bhw.inletModelObject().get().nameString(), idf_bhw.getString(Boiler_HotWaterFields::BoilerWaterInletNodeName, false).get());
  EXPECT_EQ(bhw.outletModelObject().get().nameString(), idf_bhw.getString(Boiler_HotWaterFields::BoilerWaterOutletNodeName, false).get());
  EXPECT_EQ(7.0, idf_bhw.getDouble(Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, false).get());
  EXPECT_EQ("LeavingSetpointModulated", idf_bhw.getString(Boiler_HotWaterFields::BoilerFlowMode, false).get());
  EXPECT_EQ(8.0, idf_bhw.getDouble(Boiler_HotWaterFields::OnCycleParasiticElectricLoad, false).get());
  EXPECT_EQ(9.0, idf_bhw.getDouble(Boiler_HotWaterFields::SizingFactor, false).get());
  EXPECT_EQ("Test", idf_bhw.getString(Boiler_HotWaterFields::EndUseSubcategory, false).get());
  EXPECT_EQ(10.0, idf_bhw.getDouble(Boiler_HotWaterFields::OffCycleParasiticFuelLoad, false).get());
}
