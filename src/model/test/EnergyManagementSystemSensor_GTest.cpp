/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

TEST_F(ModelFixture, EMSSensor_EMSSensor) {
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
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature.handle(), OATdbSensor.outputVariable().get().handle());
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature, OATdbSensor.outputVariable().get());
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
  EXPECT_EQ(zoneTemperature, zoneSensor.outputVariable().get());
  EXPECT_EQ(zone1.nameString(), zoneSensor.keyName());

  // add Zone Lights Electricity Rate to both zones
  OutputVariable lightsElectricPower("Zone Lights Electricity Rate", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electricity Rate", lightsElectricPower.variableName());

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
  EXPECT_EQ(meter, meter_sensor.outputMeter().get());
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
  OutputVariable outvar1("VRF Heat Pump Heating Electricity Energy", model);
  outvar1.setName("residential mini split vrf heat energy output var");
  outvar1.setKeyValue("");
  //EXPECT_EQ("", outvar1.keyValue());
  EXPECT_EQ("VRF Heat Pump Heating Electricity Energy", outvar1.variableName());
  EXPECT_EQ("residential mini split vrf heat energy output var", outvar1.nameString());

  // add sensor 1
  EnergyManagementSystemSensor sensor1(model, outvar1);
  sensor1.setName("residential_mini_split_vrf_energy_sensor");
  sensor1.setKeyName("living zone Multi Split Heat Pump");

  // add output variable 2
  OutputVariable outvar2("VRF Heat Pump Heating Electricity Energy", model);
  outvar2.setName("residential mini split|unit 2 vrf heat energy output var");
  outvar2.setKeyValue("");
  //EXPECT_EQ("", outvar2.keyValue());
  EXPECT_EQ("VRF Heat Pump Heating Electricity Energy", outvar2.variableName());
  EXPECT_EQ("residential mini split|unit 2 vrf heat energy output var", outvar2.nameString());

  // add sensor
  EnergyManagementSystemSensor sensor2(model, outvar2);
  sensor2.setName("residential_mini_split_unit_2_vrf_energy_sensor");
  sensor2.setKeyName("living zone|unit 2 Multi Split Heat Pump");

  // model.save(toPath("./EMS_sensortest.osm"), true);

  outvar1.remove();
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<EnergyManagementSystemSensor>().size());
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
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<EnergyManagementSystemSensor>().size());
  // 1 avm in the model
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  // model.save(toPath("./EMS_sensor_delete_test.osm"), true);

  avm.remove();
  // 0 avm in the model
  EXPECT_EQ(static_cast<unsigned>(0), model.getConcreteModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  //sensor still has keyName as avm UUID string (will not FT though eventually)
  EXPECT_EQ(key, sensor.keyName());
}
