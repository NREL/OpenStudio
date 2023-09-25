/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../Schedule.hpp"
#include "../EnergyManagementSystemActuator.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemSubroutine.hpp"
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

TEST_F(ModelFixture, EMSSubroutine_EMSSubroutine) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  // OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);
  fan.setName("fan");

  //add actuator on fan
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press_Actuator";
  fanActuator.setName(fanName);

  //add program
  EnergyManagementSystemSubroutine fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "   SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\n   SET "
                                   + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET "
                                      + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  std::string line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\n";
  std::string line2_test = "SET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n";

  //set body
  fan_program_1.setBody(fan_program_1_body);

  //check body
  boost::optional<std::string> body = fan_program_1.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //check lines
  boost::optional<std::vector<std::string>> lines = fan_program_1.lines();
  EXPECT_EQ(2, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[0]);
  EXPECT_EQ(line2_test, lines.get()[1]);

  EXPECT_EQ(2, fan_program_1.referencedObjects().size());
  EXPECT_EQ(true,
            (fan_program_1.referencedObjects()[0].nameString() == fanName) || (fan_program_1.referencedObjects()[0].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(true,
            (fan_program_1.referencedObjects()[1].nameString() == fanName) || (fan_program_1.referencedObjects()[1].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(0, fan_program_1.invalidReferencedObjects().size());
  fanActuator.remove();
  EXPECT_EQ(1, fan_program_1.invalidReferencedObjects().size());

  //add actuator on fan again
  fanControlType = "Fan Pressure Rise";
  ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator2(fan, ComponentType, fanControlType);
  fanName = fan.name().get() + "Press_Actuator";
  fanActuator2.setName(fanName);

  // Create a second program from a vector of lines
  EnergyManagementSystemSubroutine fan_program_2(model);
  programName = fan.name().get() + "Pressure Rise Program by Line";
  fan_program_2.setName(programName);
  //create program by individual lines
  std::string line1 = "   SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\n";
  std::string line2 = "   SET " + toString(fanActuator2.handle()) + " = 250 * mult !- More nonsense\r\n";

  fan_program_2.addLine(line1);
  fan_program_2.addLine(line2);

  //this is what the body should look like with 2 /n's and compare TRUE
  fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator2.handle())
                          + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\n";
  line2_test = "SET " + toString(fanActuator2.handle()) + " = 250 * mult !- More nonsense\n";

  //check body
  body = fan_program_2.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //check lines
  lines = fan_program_2.lines();
  EXPECT_EQ(2, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[0]);
  EXPECT_EQ(line2_test, lines.get()[1]);

  EXPECT_EQ(2, fan_program_2.referencedObjects().size());
  EXPECT_EQ(true,
            (fan_program_2.referencedObjects()[0].nameString() == fanName) || (fan_program_2.referencedObjects()[0].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(true,
            (fan_program_2.referencedObjects()[1].nameString() == fanName) || (fan_program_2.referencedObjects()[1].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(0, fan_program_2.invalidReferencedObjects().size());
  OATdbSensor.remove();
  EXPECT_EQ(1, fan_program_2.invalidReferencedObjects().size());

  //add sensor back in
  EnergyManagementSystemSensor OATdbSensor2(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor2.setName("OATdb Sensor");
  // OATdbSensor2.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  // # Create a third program from vector of lines
  EnergyManagementSystemSubroutine fan_program_3(model);
  programName = fan.name().get() + "Pressure Rise Program by Vector of Lines";
  fan_program_3.setName(programName);

  std::vector<std::string> vectorOfLines;
  line1 = "SET mult = " + toString(OATdbSensor2.handle()) + " / 15.0 !- This is nonsense\r\n";
  line2 = "SET " + toString(fanActuator2.handle()) + " = 250 * mult !- More nonsense\r\n";
  vectorOfLines.push_back(line1);
  vectorOfLines.push_back(line2);
  fan_program_3.setLines(vectorOfLines);

  //this is what the body should look like with 2 /n's and compare TRUE
  fan_program_body_test = "SET mult = " + toString(OATdbSensor2.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator2.handle())
                          + " = 250 * mult !- More nonsense\n";
  //the added lines should compare TRUE to below
  line1_test = "SET mult = " + toString(OATdbSensor2.handle()) + " / 15.0 !- This is nonsense\n";
  line2_test = "SET " + toString(fanActuator2.handle()) + " = 250 * mult !- More nonsense\n";

  //check body
  body = fan_program_3.body();
  EXPECT_EQ(fan_program_body_test, body.get());

  //check lines
  lines = fan_program_3.lines();
  EXPECT_EQ(2, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[0]);
  EXPECT_EQ(line2_test, lines.get()[1]);

  EXPECT_EQ(2, fan_program_3.referencedObjects().size());
  EXPECT_EQ(true,
            (fan_program_3.referencedObjects()[0].nameString() == fanName) || (fan_program_3.referencedObjects()[0].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(true,
            (fan_program_3.referencedObjects()[1].nameString() == fanName) || (fan_program_3.referencedObjects()[1].nameString() == "OATdb_Sensor"));
  EXPECT_EQ(0, fan_program_3.invalidReferencedObjects().size());
  OATdbSensor2.remove();
  EXPECT_EQ(1, fan_program_3.invalidReferencedObjects().size());
  fanActuator2.remove();
  EXPECT_EQ(2, fan_program_3.invalidReferencedObjects().size());

  lines = fan_program_3.lines();
  EXPECT_EQ(2, lines.get().size());
  fan_program_3.addLine(line1);
  lines = fan_program_3.lines();
  EXPECT_EQ(3, lines.get().size());
  EXPECT_EQ(line1_test, lines.get()[2]);
  fan_program_3.resetBody();
  body = fan_program_3.body();
  EXPECT_EQ("", body.get());
}
