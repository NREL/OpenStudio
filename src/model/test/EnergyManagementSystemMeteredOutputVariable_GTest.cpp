/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../EnergyManagementSystemMeteredOutputVariable.hpp"
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemSubroutine.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSMeteredOutputVariable) {
  Model model;
  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);
  EXPECT_EQ("*", siteOutdoorAirDrybulbTemperature.keyValue());
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", siteOutdoorAirDrybulbTemperature.variableName());

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add program
  EnergyManagementSystemProgram program_1(model);
  program_1.setName("program one");

  //add program
  EnergyManagementSystemSubroutine subroutine_1(model);
  subroutine_1.setName("subroutine one");

  // add metered output variable
  EnergyManagementSystemMeteredOutputVariable meteredoutvar(model, OATdbSensor.name().get());

  //meteredoutvar.setEMSVariableName(OATdbSensor.name().get());
  EXPECT_EQ(meteredoutvar.emsVariableName(), OATdbSensor.name().get());
  meteredoutvar.setUpdateFrequency("ZoneTimestep");
  EXPECT_EQ("ZoneTimestep", meteredoutvar.updateFrequency());
  meteredoutvar.setEMSProgramOrSubroutineName(program_1);
  EXPECT_EQ(program_1.name().get(), meteredoutvar.emsProgramOrSubroutineName());
  EXPECT_EQ(program_1, meteredoutvar.emsProgram().get());
  meteredoutvar.resetEMSProgramOrSubroutineName();
  EXPECT_EQ("", meteredoutvar.emsProgramOrSubroutineName());
  meteredoutvar.setEMSProgramOrSubroutineName(subroutine_1);
  EXPECT_EQ(subroutine_1.name().get(), meteredoutvar.emsProgramOrSubroutineName());
  EXPECT_EQ(subroutine_1, meteredoutvar.emsSubroutine().get());
  meteredoutvar.setResourceType("NaturalGas");
  EXPECT_EQ("NaturalGas", meteredoutvar.resourceType());
  meteredoutvar.setGroupType("HVAC");
  EXPECT_EQ("HVAC", meteredoutvar.groupType());
  meteredoutvar.setEndUseCategory("Heating");
  EXPECT_EQ("Heating", meteredoutvar.endUseCategory());
  meteredoutvar.setEndUseSubcategory("Madeup");
  EXPECT_EQ("Madeup", meteredoutvar.endUseSubcategory());

  // model.save(toPath("./EMS_meteredoutvar.osm"), true);

  // add output variable sensor
  EnergyManagementSystemMeteredOutputVariable outvar_sen(model, OATdbSensor);
  //setname
  outvar_sen.setName("outputVar sen");
  EXPECT_EQ("outputVar sen", outvar_sen.nameString());

  ASSERT_TRUE(outvar_sen.emsVariableObject());
  ASSERT_TRUE(outvar_sen.emsSensor());
  EXPECT_EQ(outvar_sen.emsVariableObject().get(), outvar_sen.emsSensor().get());
}
