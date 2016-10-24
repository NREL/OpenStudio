/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include <algorithm>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../Schedule.hpp"
#include "../EnergyManagementSystemActuator.hpp"
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemProgramCallingManager.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/UUID.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSProgramCallingManager_EMSProgramCallingManager)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  //add actuator on fan
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  //add program
  EnergyManagementSystemProgram fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle() ) + " / 15.0 !- This is nonsense\r\nSET " + toString(fanActuator.handle() ) + " = 250 * mult !- More nonsense";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  std::string line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\n";
  std::string line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //set body
  fan_program_1.setBody(fan_program_1_body);

  //check body
  boost::optional<std::string> body = fan_program_1.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  // Create a second program from a vector of lines
  EnergyManagementSystemProgram fan_program_2(model);
  programName = fan.name().get() + "Pressure Rise Program by Line";
  fan_program_2.setName(programName);
  //create program by individual lines
  std::string line1 = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\n";
  std::string line2 = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\r\n";

  fan_program_2.addLine(line1);
  fan_program_2.addLine(line2);

  //this is what the body should look like with 2 /n's and compare TRUE
  fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\n";
  line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //check body
  body = fan_program_2.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  // # Create a third program from vector of lines
  EnergyManagementSystemProgram fan_program_3(model);
  programName = fan.name().get() + "Pressure Rise Program by Vector of Lines";
  fan_program_3.setName(programName);

  std::vector<std::string> vectorOfLines;
  line1 = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\n";
  line2 = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\r\n";
  vectorOfLines.push_back(line1);
  vectorOfLines.push_back(line2);
  fan_program_3.setLines(vectorOfLines);

  //this is what the body should look like with 2 /n's and compare TRUE
  fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\n";
  line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //check body
  body = fan_program_3.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //start program calling manager testing
  EnergyManagementSystemProgramCallingManager fan_pcm(model);
  fan_pcm.setName("Best Program Calling Manager");
  //get calling points
  std::vector<std::string> calling_points = fan_pcm.validCallingPointValues();
  
  //check if BeginNewEnviornment is in callingpoints.
  bool finds = (std::find(calling_points.begin(), calling_points.end(), "BeginNewEnvironment") != calling_points.end());
  EXPECT_EQ(true, finds);

  bool callpt = fan_pcm.setCallingPoint("BeginNewEnvironment");
  EXPECT_EQ(true, callpt);
  EXPECT_EQ("BeginNewEnvironment", fan_pcm.callingPoint());

  //should start with 0 programs
  std::vector<EnergyManagementSystemProgram> programs = fan_pcm.programs();
  EXPECT_EQ(0,programs.size());
  //should not be initialized yet
  boost::optional<EnergyManagementSystemProgram> program = fan_pcm.getProgram(1);
  EXPECT_EQ(false, program.is_initialized());

  bool add_result = fan_pcm.addProgram(fan_program_1);
  EXPECT_EQ(true, add_result);
  add_result = fan_pcm.addProgram(fan_program_2);
  EXPECT_EQ(true, add_result);
  EXPECT_EQ(2, fan_pcm.programs().size());
  EXPECT_EQ(fan_program_2.nameString(), fan_pcm.programs()[1].nameString());
  //set program one slot too big, will return false
  bool insert_result = fan_pcm.setProgram(fan_program_3, 3);
  EXPECT_EQ(false, insert_result);
  //still only 2 programs
  EXPECT_EQ(2, fan_pcm.programs().size());
  //set program in last slot
  insert_result = fan_pcm.setProgram(fan_program_3, 2);
  EXPECT_EQ(true, insert_result);
  //3 programs
  EXPECT_EQ(3, fan_pcm.programs().size());
  //check program name
  program = fan_pcm.getProgram(0);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_1.nameString(), program.get().nameString());
  //check program name
  program = fan_pcm.getProgram(1);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_2.nameString(),program.get().nameString());
  //check program name
  program = fan_pcm.getProgram(2);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_3.nameString(), program.get().nameString());
  //set program 3 in program 2 slot moving the rest down
  insert_result = fan_pcm.setProgram(fan_program_3, 1);
  EXPECT_EQ(true, insert_result);
  //now 4 programs
  EXPECT_EQ(4, fan_pcm.programs().size());
  //check program name is now program 3's
  program = fan_pcm.getProgram(1);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_3.nameString(), program.get().nameString());
  //erase program 3 in 2nd slot
  bool erase_prog = fan_pcm.eraseProgram(1);
  EXPECT_EQ(true, erase_prog);
  //check program name is back to program 2's
  program = fan_pcm.getProgram(1);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_2.nameString(), program.get().nameString());

  //erase all programs
  fan_pcm.erasePrograms();
  //now 0 programs
  EXPECT_EQ(0, fan_pcm.programs().size());
  
  //create vector of programs
  std::vector<EnergyManagementSystemProgram> new_programs;
  new_programs.push_back(fan_program_1);
  new_programs.push_back(fan_program_2);
  new_programs.push_back(fan_program_3);
  //set vector of programs
  insert_result = fan_pcm.setPrograms(new_programs);
  //3 programs
  EXPECT_EQ(3, fan_pcm.programs().size());
  //check program name
  program = fan_pcm.getProgram(0);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_1.nameString(), program.get().nameString());
  //check program name
  program = fan_pcm.getProgram(1);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_2.nameString(), program.get().nameString());
  //check program name
  program = fan_pcm.getProgram(2);
  EXPECT_EQ(true, program.is_initialized());
  EXPECT_EQ(fan_program_3.nameString(), program.get().nameString());

  model.save(toPath("./EMS_pcm.osm"), true);
}

