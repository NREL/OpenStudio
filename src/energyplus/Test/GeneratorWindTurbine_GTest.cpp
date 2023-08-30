/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorWindTurbine.hpp"
#include "../../model/GeneratorWindTurbine_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

#include <utilities/idd/Generator_WindTurbine_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <boost/algorithm/string/predicate.hpp>
#include <resources.hxx>
#include <sstream>
#include <vector>

// Debug
#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_GeneratorWindTurbine) {
  Model m;

  ElectricLoadCenterDistribution elcd(m);
  GeneratorWindTurbine generator(m);
  ScheduleConstant schedule(m);

  generator.setAvailabilitySchedule(schedule);
  elcd.addGenerator(generator);

  // Need to set buss type accordingly, or it won't be translated
  elcd.setElectricalBussType("AlternatingCurrent");

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Schedule_Constant).size());

  WorkspaceObjectVector idf_generators(w.getObjectsByType(IddObjectType::Generator_WindTurbine));
  EXPECT_EQ(1u, idf_generators.size());
  WorkspaceObject idf_generator(idf_generators[0]);

  EXPECT_EQ("Schedule Constant 1", idf_generator.getString(Generator_WindTurbineFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("HorizontalAxisWindTurbine", idf_generator.getString(Generator_WindTurbineFields::RotorType, false).get());
  EXPECT_EQ("VariableSpeedVariablePitch", idf_generator.getString(Generator_WindTurbineFields::PowerControl, false).get());
  EXPECT_EQ(41.0, idf_generator.getDouble(Generator_WindTurbineFields::RatedRotorSpeed, false).get());
  EXPECT_EQ(19.2, idf_generator.getDouble(Generator_WindTurbineFields::RotorDiameter, false).get());
  EXPECT_EQ(30.5, idf_generator.getDouble(Generator_WindTurbineFields::OverallHeight, false).get());
  EXPECT_EQ(3, idf_generator.getInt(Generator_WindTurbineFields::NumberofBlades, false).get());
  EXPECT_EQ(55000.0, idf_generator.getDouble(Generator_WindTurbineFields::RatedPower, false).get());
  EXPECT_EQ(11.0, idf_generator.getDouble(Generator_WindTurbineFields::RatedWindSpeed, false).get());
  EXPECT_EQ(3.5, idf_generator.getDouble(Generator_WindTurbineFields::CutInWindSpeed, false).get());
  EXPECT_EQ(25.0, idf_generator.getDouble(Generator_WindTurbineFields::CutOutWindSpeed, false).get());
  EXPECT_EQ(0.835, idf_generator.getDouble(Generator_WindTurbineFields::FractionsystemEfficiency, false).get());
  EXPECT_EQ(5.0, idf_generator.getDouble(Generator_WindTurbineFields::MaximumTipSpeedRatio, false).get());
  EXPECT_EQ(0.25, idf_generator.getDouble(Generator_WindTurbineFields::MaximumPowerCoefficient, false).get());
  EXPECT_EQ("", idf_generator.getString(Generator_WindTurbineFields::AnnualLocalAverageWindSpeed, false).get());
  EXPECT_EQ(50.0, idf_generator.getDouble(Generator_WindTurbineFields::HeightforLocalAverageWindSpeed, false).get());
  EXPECT_EQ(2.08, idf_generator.getDouble(Generator_WindTurbineFields::BladeChordArea, false).get());
  EXPECT_EQ(0.9, idf_generator.getDouble(Generator_WindTurbineFields::BladeDragCoefficient, false).get());
  EXPECT_EQ(0.05, idf_generator.getDouble(Generator_WindTurbineFields::BladeLiftCoefficient, false).get());
  EXPECT_EQ(0.5176, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC1, false).get());
  EXPECT_EQ(116.0, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC2, false).get());
  EXPECT_EQ(0.4, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC3, false).get());
  EXPECT_EQ(0.0, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC4, false).get());
  EXPECT_EQ(5.0, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC5, false).get());
  EXPECT_EQ(21.0, idf_generator.getDouble(Generator_WindTurbineFields::PowerCoefficientC6, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_GeneratorWindTurbine) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  // electric load center distribution
  openstudio::IdfObject idfObject1(openstudio::IddObjectType::ElectricLoadCenter_Distribution);
  idfObject1.setString(ElectricLoadCenter_DistributionFields::Name, "Electric Load Center Distribution 1");

  openstudio::WorkspaceObject epELCD = workspace.addObject(idfObject1).get();

  // generator wind turbine
  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Generator_WindTurbine);
  idfObject2.setString(Generator_WindTurbineFields::Name, "Generator Wind Turbine 1");

  openstudio::WorkspaceObject epGenerator = workspace.addObject(idfObject2).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ElectricLoadCenterDistribution> elcds = model.getConcreteModelObjects<ElectricLoadCenterDistribution>();
  /*   ASSERT_EQ(1u, elcds.size());
  ElectricLoadCenterDistribution elcd = elcds[0];
  EXPECT_EQ("Electric Load Center Distribution 1", elcd.name().get());
  ASSERT_EQ(1u, elcd.generators().size()); */

  std::vector<GeneratorWindTurbine> generators = model.getConcreteModelObjects<GeneratorWindTurbine>();
  ASSERT_EQ(1u, generators.size());
  GeneratorWindTurbine generator = generators[0];
  EXPECT_EQ("Generator Wind Turbine 1", generator.name().get());
  EXPECT_TRUE(generator.availabilitySchedule());

  /* EXPECT_EQ(generator.name().get(), elcd.generators()[0].name().get()); */
}
