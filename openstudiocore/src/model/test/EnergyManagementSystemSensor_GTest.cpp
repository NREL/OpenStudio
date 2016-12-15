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

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
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


