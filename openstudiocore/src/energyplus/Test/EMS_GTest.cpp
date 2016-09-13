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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/OutputEnergyManagementSystem.hpp"
#include "../../model/OutputEnergyManagementSystem_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemSensor.hpp"
#include "../../model/EnergyManagementSystemSensor_Impl.hpp"
#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/EnergyManagementSystemActuator_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Program_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Program_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/OS_Output_EnergyManagementSystem_FieldEnums.hxx>
#include <utilities/idd/Output_EnergyManagementSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <QThread>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


TEST_F(EnergyPlusFixture,ForwardTranslatorSensor1_EMS) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model);
  OATdbSensor.setName("OATdb Sensor");
  OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Variable)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("OATdb Sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ("", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor1.osm"), true);
  workspace.save(toPath("./EMS_sensor1.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensor2_EMS) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);

  // add light sensor on zone1
  EnergyManagementSystemSensor lights(model);
  lights.setName("Light Sensor");
  lights.setOutputVariable(lightsElectricPower);
  lights.setKeyName(zone1.name().get());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Variable)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("Light Sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ(zone1.name().get(), object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("Zone Lights Electric Power", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor2.osm"), true);
  workspace.save(toPath("./EMS_sensor2.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensoronMeter_EMS) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // create meter
  OutputMeter meter(model);
  meter.setName("test meter");
  meter.setMeterFileOnly(false);

  //add sensor to meter
  EnergyManagementSystemSensor meter_sensor(model);
  meter_sensor.setName("meter sensor");
  meter_sensor.setOutputMeter(meter);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Meter)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("meter sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ("", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("test meter", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor_meter.osm"), true);
  workspace.save(toPath("./EMS_sensor_meter.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorActuator_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  // add actuator
  EnergyManagementSystemActuator fanActuator(fan);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);
  std::string fanControlType = "Fan Pressure Rise";
  fanActuator.setActuatedComponentControlType(fanControlType);
  std::string ComponentType = "Fan";
  fanActuator.setActuatedComponentType(ComponentType);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Fan_ConstantVolume)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ(fanName, object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ(fan.name().get(), object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(fanControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorProgram_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model);
  OATdbSensor.setName("OATdb Sensor");
  OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  //add actuator on fan
  EnergyManagementSystemActuator fanActuator(fan);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);
  std::string fanControlType = "Fan Pressure Rise";
  fanActuator.setActuatedComponentControlType(fanControlType);
  std::string ComponentType = "Fan";
  fanActuator.setActuatedComponentType(ComponentType);

  //add program
  EnergyManagementSystemProgram fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment\n";
  //the added lines should compare TRUE to below
  std::string line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\n";
  std::string line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //set body
  fan_program_1.setBody(fan_program_1_body);

  //check body
  boost::optional<std::string> body = fan_program_1.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //check lines
  boost::optional<std::vector<std::string>> lines = fan_program_1.lines();
  EXPECT_EQ(4, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[0]);
  EXPECT_EQ(line2_test, lines.get()[1]);

  model.save(toPath("./EMS_program.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());

  workspace.save(toPath("./EMS_program.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSubroutine_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model);
  OATdbSensor.setName("OATdb Sensor");
  OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  //add actuator on fan
  EnergyManagementSystemActuator fanActuator(fan);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);
  std::string fanControlType = "Fan Pressure Rise";
  fanActuator.setActuatedComponentControlType(fanControlType);
  std::string ComponentType = "Fan";
  fanActuator.setActuatedComponentType(ComponentType);

  //add program
  EnergyManagementSystemSubroutine fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment\n";
  //the added lines should compare TRUE to below
  std::string line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " / 15.0 !- This is !nonsense\n";
  std::string line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //set body
  fan_program_1.setBody(fan_program_1_body);

  //check body
  boost::optional<std::string> body = fan_program_1.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //check lines
  boost::optional<std::vector<std::string>> lines = fan_program_1.lines();
  EXPECT_EQ(4, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[0]);
  EXPECT_EQ(line2_test, lines.get()[1]);

  model.save(toPath("./EMS_subroutine.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());

  workspace.save(toPath("./EMS_subroutine.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorProgramCallingManager_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model);
  OATdbSensor.setName("OATdb Sensor");
  OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  //add actuator on fan
  EnergyManagementSystemActuator fanActuator(fan);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);
  std::string fanControlType = "Fan Pressure Rise";
  fanActuator.setActuatedComponentControlType(fanControlType);
  std::string ComponentType = "Fan";
  fanActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());

  //add program
  EnergyManagementSystemProgram fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense";
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
  EXPECT_EQ("BeginNewEnvironment", fan_pcm.callingPoint().get());

  //should start with 0 programs
  std::vector<EnergyManagementSystemProgram> programs = fan_pcm.programs();
  EXPECT_EQ(0, programs.size());
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
  EXPECT_EQ(fan_program_2.nameString(), program.get().nameString());
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
  //check for null programs
  std::vector<unsigned> nullProgs = fan_pcm.nullPrograms();
  //should not be any if erasePrograms reindexed correctly
  EXPECT_EQ(0, nullProgs.size());
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

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}

TEST_F(EnergyPlusFixture, noForwardTranslatorOutput_EMS) {
  Model model;

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Output_EnergyManagementSystem).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslatorOutput_EMS) {
  Model model;
  OutputEnergyManagementSystem oEMS = model.getUniqueModelObject<OutputEnergyManagementSystem>();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Output_EnergyManagementSystem).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::Output_EnergyManagementSystem)[0];
  //ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting));
  //EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting));
  //ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting));
  //EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting));
  //ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel));
  //EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel));

}