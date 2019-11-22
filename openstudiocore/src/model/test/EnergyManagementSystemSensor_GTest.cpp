/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "../AvailabilityManagerHighTemperatureTurnOff_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemSensor_Impl.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"
#include "../Model.hpp"
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

TEST_F(ModelFixture, EMSSensor_EMSSensor)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);
  EXPECT_EQ("*", siteOutdoorAirDrybulbTemperature.keyValue());
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", siteOutdoorAirDrybulbTemperature.variableName());

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  EXPECT_EQ("OATdb_Sensor", OATdbSensor.nameString());
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature.handle(), OATdbSensor.outputVariable().get().handle() );
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature, OATdbSensor.outputVariable());
  EXPECT_EQ("", OATdbSensor.keyName());

  // add zone Temperature
  OutputVariable zoneTemperature("Zone Air Temperature", model);
  zoneTemperature.setKeyValue(zone1.nameString());
  EXPECT_EQ(zone1.nameString(), zoneTemperature.keyValue());
  EXPECT_EQ("Zone Air Temperature", zoneTemperature.variableName());

  // add sensor
  EnergyManagementSystemSensor zoneSensor(model, zoneTemperature);
  zoneSensor.setName("Zone Sensor");

  EXPECT_EQ("Zone_Sensor", zoneSensor.nameString());
  EXPECT_EQ(zoneTemperature.handle(), zoneSensor.outputVariable().get().handle());
  EXPECT_EQ(zoneTemperature, zoneSensor.outputVariable());
  EXPECT_EQ(zone1.nameString(), zoneSensor.keyName());

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electric Power", lightsElectricPower.variableName());

  // add light sensor on zone1
  EnergyManagementSystemSensor lights(model, lightsElectricPower);
  lights.setName("Light Sensor");
  //lights.setOutputVariable(lightsElectricPower);
  lights.setKeyName(zone1.name().get());

  EXPECT_EQ(zone1.name().get(), lights.keyName());
  EXPECT_EQ("Light_Sensor", lights.nameString());

  // create meter
  OutputMeter meter(model);
  meter.setName("test meter");

  //add sensor to meter
  EnergyManagementSystemSensor meter_sensor(model, meter);
  meter_sensor.setName("meter sensor");
  //meter_sensor.setOutputMeter(meter);

  EXPECT_EQ("meter_sensor", meter_sensor.nameString());
  EXPECT_EQ(meter.handle(), meter_sensor.outputMeter().get().handle());
  EXPECT_EQ(meter, meter_sensor.outputMeter());
  EXPECT_EQ("", meter_sensor.keyName());

  ASSERT_TRUE(OATdbSensor.outputVariable());
  ASSERT_FALSE(OATdbSensor.outputMeter());
  siteOutdoorAirDrybulbTemperature.remove();
  boost::optional<ModelObject> object = model.getModelObjectByName<ModelObject>("OATdb_Sensor");
  ASSERT_FALSE(object);

  ASSERT_TRUE(meter_sensor.outputMeter());
  ASSERT_FALSE(meter_sensor.outputVariable());
  meter.remove();
  object = model.getModelObjectByName<ModelObject>("meter sensor");
  ASSERT_FALSE(object);

  // add sensor by string
  EnergyManagementSystemSensor sensor_string(model, "Sensor String");
  sensor_string.setName("Sensor String Name");
  EXPECT_EQ("Sensor_String_Name", sensor_string.nameString());
  EXPECT_EQ("Sensor String", sensor_string.outputVariableOrMeterName());
}
TEST_F(ModelFixture, EMSSensorOutVar) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();


  // add output variable 1
  OutputVariable outvar1("VRF Heat Pump Heating Electric Energy", model);
  outvar1.setName("residential mini split vrf heat energy output var");
  outvar1.setKeyValue("");
  //EXPECT_EQ("", outvar1.keyValue());
  EXPECT_EQ("VRF Heat Pump Heating Electric Energy", outvar1.variableName());
  EXPECT_EQ("residential mini split vrf heat energy output var", outvar1.nameString());

  // add sensor 1
  EnergyManagementSystemSensor sensor1(model, outvar1);
  sensor1.setName("residential_mini_split_vrf_energy_sensor");
  sensor1.setKeyName("living zone Multi Split Heat Pump");

  // add output variable 2
  OutputVariable outvar2("VRF Heat Pump Heating Electric Energy", model);
  outvar2.setName("residential mini split|unit 2 vrf heat energy output var");
  outvar2.setKeyValue("");
  //EXPECT_EQ("", outvar2.keyValue());
  EXPECT_EQ("VRF Heat Pump Heating Electric Energy", outvar2.variableName());
  EXPECT_EQ("residential mini split|unit 2 vrf heat energy output var", outvar2.nameString());

  // add sensor
  EnergyManagementSystemSensor sensor2(model, outvar2);
  sensor2.setName("residential_mini_split_unit_2_vrf_energy_sensor");
  sensor2.setKeyName("living zone|unit 2 Multi Split Heat Pump");

  model.save(toPath("./EMS_sensortest.osm"), true);

  outvar1.remove();
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<EnergyManagementSystemSensor>().size());

}

TEST_F(ModelFixture, EMSSensorDelete) {
  Model model;
  PlantLoop plantLoop(model);
  AvailabilityManagerHighTemperatureTurnOff avm(model);
  avm.setSensorNode(model.outdoorAirNode());
  plantLoop.addAvailabilityManager(avm);
  std::vector<std::string> avm_names = avm.outputVariableNames();

  // add sensor 1
  EnergyManagementSystemSensor sensor(model, avm_names[0]);
  sensor.setKeyName(toString(avm.handle()));

  // Sensor attached to AVM
  std::string key = toString(avm.handle());
  EXPECT_EQ(key, sensor.keyName());
  // 1 sensor in the model
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<EnergyManagementSystemSensor>().size());
  // 1 avm in the model
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  model.save(toPath("./EMS_sensor_delete_test.osm"), true);

  avm.remove();
  // 0 avm in the model
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  //sensor still has keyName as avm UUID string (will not FT though eventually)
  EXPECT_EQ(key, sensor.keyName());
}


