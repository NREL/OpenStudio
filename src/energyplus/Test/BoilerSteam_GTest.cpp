/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/BoilerSteam.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/Boiler_Steam_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_BoilerSteam) {
  Model m;

  PlantLoop plant_loop(m);

  BoilerSteam bs(m);

  EXPECT_TRUE(bs.setFuelType("Electricity"));
  EXPECT_TRUE(bs.setMaximumOperatingPressure(10.03));
  EXPECT_TRUE(bs.setTheoreticalEfficiency(0.5));
  EXPECT_TRUE(bs.setDesignOutletSteamTemperature(10.03));
  EXPECT_TRUE(bs.setNominalCapacity(10.03));
  EXPECT_TRUE(bs.setMinimumPartLoadRatio(1.0));
  EXPECT_TRUE(bs.setMaximumPartLoadRatio(1.0));
  EXPECT_TRUE(bs.setOptimumPartLoadRatio(1.0));
  EXPECT_TRUE(bs.setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  EXPECT_TRUE(bs.setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  EXPECT_TRUE(bs.setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  EXPECT_TRUE(bs.setSizingFactor(0.5));
  EXPECT_TRUE(bs.setEndUseSubcategory("SteamBoiler"));

  EXPECT_TRUE(plant_loop.addSupplyBranchForComponent(bs));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Boiler_Steam).size());
  ASSERT_EQ(0u, w.getObjectsByType(IddObjectType::Pipe_Adiabatic).size());
  ASSERT_EQ(6u, w.getObjectsByType(IddObjectType::Pipe_Adiabatic_Steam).size());
  IdfObject idf_bs = w.getObjectsByType(IddObjectType::Boiler_Steam)[0];

  EXPECT_EQ(bs.nameString(), idf_bs.getString(Boiler_SteamFields::Name, false).get());
  EXPECT_EQ("Electricity", idf_bs.getString(Boiler_SteamFields::FuelType, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::MaximumOperatingPressure, false).get());
  EXPECT_EQ(0.5, idf_bs.getDouble(Boiler_SteamFields::TheoreticalEfficiency, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::DesignOutletSteamTemperature, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::NominalCapacity, false).get());
  EXPECT_EQ(1.0, idf_bs.getDouble(Boiler_SteamFields::MinimumPartLoadRatio, false).get());
  EXPECT_EQ(1.0, idf_bs.getDouble(Boiler_SteamFields::MaximumPartLoadRatio, false).get());
  EXPECT_EQ(1.0, idf_bs.getDouble(Boiler_SteamFields::OptimumPartLoadRatio, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, false).get());
  EXPECT_EQ(10.03, idf_bs.getDouble(Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, false).get());
  EXPECT_EQ(bs.inletModelObject().get().nameString(), idf_bs.getString(Boiler_SteamFields::WaterInletNodeName, false).get());
  EXPECT_EQ(bs.outletModelObject().get().nameString(), idf_bs.getString(Boiler_SteamFields::SteamOutletNodeName, false).get());
  EXPECT_EQ(0.5, idf_bs.getDouble(Boiler_SteamFields::SizingFactor, false).get());
  EXPECT_EQ("SteamBoiler", idf_bs.getString(Boiler_SteamFields::EndUseSubcategory, false).get());
}
