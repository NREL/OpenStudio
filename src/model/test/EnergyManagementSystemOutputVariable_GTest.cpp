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
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemActuator.hpp"
#include "../EnergyManagementSystemGlobalVariable.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemSubroutine.hpp"
#include "../EnergyManagementSystemOutputVariable.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"
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

TEST_F(ModelFixture, EMSOutputVariable_EMSOutputVariable) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add global variable
  EnergyManagementSystemGlobalVariable var(model, "globalVar");

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);
  FanConstantVolume fan2(model, s);

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  // add output variable
  EnergyManagementSystemOutputVariable outvar(model, "globalVar");
  //setname
  outvar.setName("outputVar");
  EXPECT_EQ("outputVar", outvar.nameString());
  //check BCVTB
  EXPECT_FALSE(outvar.exportToBCVTB());
  EXPECT_TRUE(outvar.isExportToBCVTBDefaulted());
  outvar.setExportToBCVTB(true);
  EXPECT_TRUE(outvar.exportToBCVTB());
  outvar.resetExportToBCVTB();
  EXPECT_FALSE(outvar.exportToBCVTB());

  //variable name
  bool varset = outvar.setEMSVariableName("badVariable");
  EXPECT_EQ(true, varset);
  varset = outvar.setEMSVariableName("globalVar");
  EXPECT_EQ(true, varset);

  //set units
  //outvar.setUnits("C");
  //EXPECT_EQ("C", outvar.units().get());
  //check name has units
  //EXPECT_EQ("output var [C]", outvar.nameString());
  //update frequency
  bool update = outvar.setUpdateFrequency("BadValue");
  EXPECT_EQ(false, update);
  outvar.setUpdateFrequency("ZoneTimestep");
  EXPECT_EQ("ZoneTimestep", outvar.updateFrequency());
  outvar.setUpdateFrequency("SystemTimestep");
  EXPECT_EQ("SystemTimestep", outvar.updateFrequency());
  //type of data in variable
  update = outvar.setTypeOfDataInVariable("BadValue");
  EXPECT_EQ(false, update);
  outvar.setTypeOfDataInVariable("Averaged");
  EXPECT_EQ("Averaged", outvar.typeOfDataInVariable());
  outvar.setTypeOfDataInVariable("Summed");
  EXPECT_EQ("Summed", outvar.typeOfDataInVariable());
  //program or subroutine name
  EXPECT_EQ("", outvar.emsProgramOrSubroutineName());
  //bool setprogram = outvar.setEMSProgramorSubroutineName("program name");
  //expect false since program doesnt exist
  //EXPECT_EQ(false, setprogram);

  // add program
  EnergyManagementSystemProgram program(model);
  program.setName("program 1");
  bool setprogram = outvar.setEMSProgramOrSubroutineName(program);
  EXPECT_EQ(true, setprogram);
  EXPECT_EQ("program_1", outvar.emsProgramOrSubroutineName());
  EXPECT_EQ(program, outvar.emsProgram().get());

  // add output variable actuator
  EnergyManagementSystemOutputVariable outvar_act(model, fanActuator);
  //setname
  outvar_act.setName("outputVar act");
  EXPECT_EQ("outputVar act", outvar_act.nameString());

  // add output variable sensor
  EnergyManagementSystemOutputVariable outvar_sen(model, OATdbSensor);

  outvar_sen.setName("outputVar sen");
  EXPECT_EQ("outputVar sen", outvar_sen.nameString());

  ASSERT_TRUE(outvar_sen.emsVariableObject());
  ASSERT_TRUE(outvar_sen.emsSensor());
  EXPECT_EQ(outvar_sen.emsVariableObject().get(), outvar_sen.emsSensor().get());
}
