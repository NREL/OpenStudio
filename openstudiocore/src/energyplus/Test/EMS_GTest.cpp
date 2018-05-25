/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Component.hpp"
#include "../../model/CFactorUndergroundWallConstruction.hpp"
#include "../../model/CFactorUndergroundWallConstruction_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/DefaultSubSurfaceConstructions.hpp"
#include "../../model/DefaultSubSurfaceConstructions_Impl.hpp"
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/FFactorGroundFloorConstruction.hpp"
#include "../../model/FFactorGroundFloorConstruction_Impl.hpp"
#include "../../model/WindowDataFile.hpp"
#include "../../model/WindowDataFile_Impl.hpp"
#include "../../model/StandardsInformationConstruction.hpp"
#include "../../model/StandardsInformationConstruction_Impl.hpp"
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
#include "../../model/EnergyManagementSystemConstructionIndexVariable.hpp"
#include "../../model/EnergyManagementSystemConstructionIndexVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "../../model/EnergyManagementSystemCurveOrTableIndexVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemMeteredOutputVariable.hpp"
#include "../../model/EnergyManagementSystemMeteredOutputVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/EnergyManagementSystemSubroutine.hpp"
#include "../../model/EnergyManagementSystemSubroutine_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemOutputVariable.hpp"
#include "../../model/EnergyManagementSystemOutputVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemTrendVariable.hpp"
#include "../../model/EnergyManagementSystemTrendVariable_Impl.hpp"
#include "../../model/EnergyManagementSystemInternalVariable.hpp"
#include "../../model/EnergyManagementSystemInternalVariable_Impl.hpp"

#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/AirGap.hpp"
#include "../../model/AirWallMaterial.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/StandardGlazing.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/SpaceLoad.hpp"
#include "../../model/SpaceLoad_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/LifeCycleCost_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/OtherEquipmentDefinition.hpp"
#include "../../model/OtherEquipmentDefinition_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "../../model/AvailabilityManagerHighTemperatureTurnOff_Impl.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

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
#include <utilities/idd/OS_EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Subroutine_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/OS_Output_EnergyManagementSystem_FieldEnums.hxx>
#include <utilities/idd/Output_EnergyManagementSystem_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_TrendVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_TrendVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_InternalVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_InternalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_CurveOrTableIndexVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_CurveOrTableIndexVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_MeteredOutputVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_MeteredOutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
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
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Variable)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("OATdb_Sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ("", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor1.osm"), true);
  workspace.save(toPath("./EMS_sensor1.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensorRename_EMS) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  Schedule s = model.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
  CurveQuadratic partLoadFractionCorrelationCurve(model);
  // make cooling coil
  CoilCoolingDXSingleSpeed dx_coil(model, s,
    totalCoolingCapacityFunctionofTemperatureCurve,
    totalCoolingCapacityFunctionofFlowFractionCurve,
    energyInputRatioFunctionofTemperatureCurve,
    energyInputRatioFunctionofFlowFractionCurve,
    partLoadFractionCorrelationCurve);

  dx_coil.setName("Coil Name Before Change");
  EXPECT_EQ("Coil Name Before Change",dx_coil.nameString());

  // add sensor
  EnergyManagementSystemSensor sensor(model, "Cooling Coil Runtime Fraction");
  sensor.setKeyName(toString(dx_coil.handle()));

  //change name
  dx_coil.setName("New Coil Name");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed)[0];

  EXPECT_EQ("New Coil Name", object.nameString());

  model.save(toPath("./EMS_sensorRename.osm"), true);
  workspace.save(toPath("./EMS_sensorRename.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor1_EMS) {

  openstudio::path idfPath = toPath("./EMS_sensor1.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_sensor1T.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensor2_EMS) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);

  // add light sensor on zone1
  EnergyManagementSystemSensor lights(model, lightsElectricPower);
  lights.setName("Light Sensor");
 // lights.setOutputVariable(lightsElectricPower);
  lights.setKeyName(zone1.name().get());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Variable)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("Light_Sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ(zone1.name().get(), object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("Zone Lights Electric Power", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor2.osm"), true);
  workspace.save(toPath("./EMS_sensor2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor2_EMS) {

  openstudio::path idfPath = toPath("./EMS_sensor2.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_sensor2T.osm"), true);
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
  EnergyManagementSystemSensor meter_sensor(model, meter);
  meter_sensor.setName("meter sensor");
  //meter_sensor.setOutputMeter(meter);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Meter)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("meter_sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ("", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("test meter", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  model.save(toPath("./EMS_sensor_meter.osm"), true);
  workspace.save(toPath("./EMS_sensor_meter.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor_Meter_EMS) {

  openstudio::path idfPath = toPath("./EMS_sensor_meter.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_sensor_meterT.osm"), true);

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
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Fan_ConstantVolume)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Fan_Constant_Volume_1Press_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ(fan.name().get(), object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(fanControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorActuator_EMS) {

  openstudio::path idfPath = toPath("./EMS_example.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_exampleT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorWeatherActuator_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // weatherFile
  // getUniqueModelObject will instantiate the object if not found
  WeatherFile weatherFile = model.getUniqueModelObject<WeatherFile>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<WeatherFile>());

  // add actuator
  std::string controlType = "Outdoor Dry Bulb";
  std::string componentType = "Weather Data";
  EnergyManagementSystemActuator weatherActuator(weatherFile, componentType, controlType);
  std::string actuatorName = "Weather_Actuator";
  weatherActuator.setName(actuatorName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ(actuatorName, object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ("Environment", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(componentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(controlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_weatheractuator.osm"), true);
  workspace.save(toPath("./EMS_weatheractuator.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorWeatherActuator2_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // get site
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Site>());
  //OptionalSite oSite = model.getOptionalUniqueModelObject<Site>();
  //EXPECT_TRUE(oSite);

  // add actuator
  std::string controlType = "Outdoor Dry Bulb";
  std::string componentType = "Weather Data";
  EnergyManagementSystemActuator weatherActuator(site, componentType, controlType);
  std::string actuatorName = "Weather_Actuator";
  weatherActuator.setName(actuatorName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ(actuatorName, object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ("Environment", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(componentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(controlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_weatheractuator2.osm"), true);
  workspace.save(toPath("./EMS_weatheractuator2.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_Space_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  GasEquipmentDefinition gasEquipDef(model);
  GasEquipment gasEquip(gasEquipDef);
  gasEquip.setName("Gas Equip");
  //add to space and not spacetype
  space.setGasEquipmentPower(10, gasEquip);

  // add actuator
  std::string ControlType = "Gas Power Level";
  std::string ComponentType = "GasEquipment";
  EnergyManagementSystemActuator fanActuator(gasEquip, ComponentType, ControlType);
  std::string actName = "Gas Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];


  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Gas_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  //Should not have TZ in the name since the load is attached to a single space
  EXPECT_EQ("Gas Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  //EXPECT_EQ("Thermal Zone Gas Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_SpaceType_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  GasEquipmentDefinition gasEquipDef(model);
  GasEquipment gasEquip(gasEquipDef);
  gasEquip.setName("Gas Equip");
  gasEquipDef.setDesignLevel(10);
  //add to spacetype and not space
  gasEquip.setSpaceType(spaceType);

  // add actuator
  std::string ControlType = "Gas Power Level";
  std::string ComponentType = "GasEquipment";
  EnergyManagementSystemActuator fanActuator(gasEquip, ComponentType, ControlType);
  std::string actName = "Gas Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];


  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Gas_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  //Should have TZ in the name since the load is attached to a spacetype with only 1 space
  EXPECT_EQ("Thermal Zone Gas Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_Space2_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  ElectricEquipmentDefinition elecEquipDef(model);
  ElectricEquipment elecEquip(elecEquipDef);
  elecEquip.setName("Electric Equip");
  //add to space and not spacetype
  space.setElectricEquipmentPower(10, elecEquip);

  // add actuator
  std::string ControlType = "Electric Level";
  std::string ComponentType = "ElectricEquipment";
  EnergyManagementSystemActuator fanActuator(elecEquip, ComponentType, ControlType);
  std::string actName = "Electric Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];


  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  //Should not have TZ in the name since the load is attached to a single space
  EXPECT_EQ("Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_SpaceType2_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  ElectricEquipmentDefinition elecEquipDef(model);
  ElectricEquipment elecEquip(elecEquipDef);
  elecEquip.setName("Electric Equip");
  elecEquipDef.setDesignLevel(10);
  //add to spacetype and not space
  elecEquip.setSpaceType(spaceType);

  // add actuator
  std::string ControlType = "Electric Level";
  std::string ComponentType = "ElectricEquipment";
  EnergyManagementSystemActuator fanActuator(elecEquip, ComponentType, ControlType);
  std::string actName = "Electric Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];


  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  //Should have TZ in the name since the load is attached to a spacetype with only 1 space
  EXPECT_EQ("Thermal Zone Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad3_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  GasEquipmentDefinition gasEquipDef(model);
  GasEquipment gasEquip(gasEquipDef);
  gasEquip.setName("Gas Equip");
  //dont assign it to a space
  //space.setGasEquipmentPower(10, gasEquip);

  // add actuator
  std::string ControlType = "Gas Power Level";
  std::string ComponentType = "GasEquipment";
  EnergyManagementSystemActuator fanActuator(gasEquip, ComponentType, ControlType);
  std::string actName = "Gas Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(1u, forwardTranslator.errors().size());
  // should now be 0 since the actuator should not translate since the SpaceLoad got deleted on FT
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_SpaceTypes_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone 1");
  ThermalZone zone2(model);
  zone2.setName("Thermal Zone 2");

  SpaceType spaceType(model);

  Space space1(model);
  space1.setThermalZone(zone1);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setThermalZone(zone2);
  space2.setSpaceType(spaceType);

  ElectricEquipmentDefinition elecEquipDef(model);
  ElectricEquipment elecEquip(elecEquipDef);
  elecEquip.setName("Electric Equip");
  elecEquipDef.setDesignLevel(10);
  //set SpaceLoad object to the SpaceType
  elecEquip.setSpaceType(spaceType);

  // add actuator
  std::string ControlType = "Electric Level";
  std::string ComponentType = "ElectricEquipment";
  EnergyManagementSystemActuator fanActuator(elecEquip, ComponentType, ControlType);
  std::string actName = "Electric Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //expect a warning since there are 2 spaces with the same spaceType
  EXPECT_EQ(1u, forwardTranslator.warnings().size());
  //expect 1 actuator since there are 2 spaces with the same spaceType but only 1 will get translated right now
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  // cannot guarantee the order of which TZ gets translated first
  //EXPECT_EQ("Thermal Zone 2 Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_multi_spaces_spacetypes.osm"), true);
  workspace.save(toPath("./EMS_multi_spaces_spacetypes.idf"), true);
}
/*
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad4_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Thermal Zone");

  Space space(model);
  SpaceType spaceType(model);
  space.setThermalZone(zone1);
  space.setSpaceType(spaceType);

  OtherEquipmentDefinition otherEquipDef(model);
  OtherEquipment otherEquip(otherEquipDef);
  otherEquip.setName("Other Equip");
  space.setOtherEquipmentPower(10, otherEquip);

  // add actuator
  std::string ControlType = "Power Level";
  std::string ComponentType = "OtherEquipment";
  EnergyManagementSystemActuator fanActuator(otherEquip, ComponentType, ControlType);
  std::string actName = "Other Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];


  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
  EXPECT_EQ("Other_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
  EXPECT_EQ("Thermal Zone Other Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
  EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
  EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
} */
TEST_F(EnergyPlusFixture, ForwardTranslatorProgram_EMS) {
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
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ProgramFields::Name, false));
  EXPECT_EQ("Fan_Constant_Volume_1Pressure_Rise_Program_by_Body", object.getString(EnergyManagementSystem_ProgramFields::Name, false).get());

  workspace.save(toPath("./EMS_program.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorProgram_EMS) {

  openstudio::path idfPath = toPath("./EMS_program.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_programT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorSubroutine_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);

  //add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor really really really really really long name");
 // OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  //add actuator on fan
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName + "really really really really really long name");

  //add program
  EnergyManagementSystemSubroutine fan_program_1(model);
  std::string programName = fan.name().get() + "Pressure Rise Program by Body";
  fan_program_1.setName(programName);
  //this body has /r/n in it
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " extra!- This is !nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " extra !- This is !nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n" + "no comment\n" + "crap !comment!comment\n";
  //the added lines should compare TRUE to below
  std::string line1_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle()) + " extra !- This is !nonsense\n";
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
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Subroutine).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Subroutine)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SubroutineFields::Name, false));
  EXPECT_EQ("Fan_Constant_Volume_1Pressure_Rise_Program_by_Body", object.getString(EnergyManagementSystem_SubroutineFields::Name, false).get());

  workspace.save(toPath("./EMS_subroutine.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSubroutine_EMS) {

  openstudio::path idfPath = toPath("./EMS_subroutine.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_subroutineT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorProgramCallingManager_EMS) {
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
  EXPECT_EQ("BeginNewEnvironment", fan_pcm.callingPoint());

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

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ProgramCallingManagerFields::Name, false));
  EXPECT_EQ("Best Program Calling Manager", object.getString(EnergyManagementSystem_ProgramCallingManagerFields::Name, false).get());

  model.save(toPath("./EMS_PCM.osm"), true);
  workspace.save(toPath("./EMS_PCM.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorProgramCallingManager_EMS) {

  openstudio::path idfPath = toPath("./EMS_PCM.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_PCMT.osm"), true);

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
  ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting, false));
  EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting, false).get());
  ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting, false));
  EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting, false).get());
  ASSERT_TRUE(object.getString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel, false));
  EXPECT_EQ("None", object.getString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel, false).get());

  model.save(toPath("./EMS_output.osm"), true);
  workspace.save(toPath("./EMS_output.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorOutput_EMS) {

  openstudio::path idfPath = toPath("./EMS_output.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_outputT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorGlobalVariable_EMS) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable var(model, "glob_var");
  EXPECT_EQ("glob_var", var.nameString());

  // add global variable
  EnergyManagementSystemGlobalVariable var2(model, "glob_var_2");
  EXPECT_EQ("glob_var_2", var2.nameString());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)[0];
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false));
  //EXPECT_EQ(var.nameString(), object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false).get());

  model.save(toPath("./EMS_GlobalVariable.osm"), true);
  workspace.save(toPath("./EMS_GlobalVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorGlobalVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_GlobalVariable.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_GlobalVariableT.osm"), true);

}

/*
TEST_F(EnergyPlusFixture, ReverseTranslatorGlobalVariable2_EMS) {

  openstudio::path idfPath = toPath("./EMS_GlobalVariable2.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_GlobalVariable2T.osm"), true);

}
*/
TEST_F(EnergyPlusFixture, ForwardTranslatorOutputVariable_EMS) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable var(model,"glob_var");
  EXPECT_EQ("glob_var", var.nameString());

  // add global variable
  EnergyManagementSystemGlobalVariable var2(model, "glob_var_2");
  EXPECT_EQ("glob_var_2", var2.nameString());

  // add output variable
  EnergyManagementSystemOutputVariable outvar(model, "glob_var");
  //setname
  outvar.setName("outputVar");
  EXPECT_EQ("outputVar", outvar.nameString());

  bool varset = outvar.setEMSVariableName("glob_var");
  EXPECT_EQ(true, varset);

  // add output variable using modelObject
  EnergyManagementSystemOutputVariable outvar2(model, var2);
  //setname
  outvar2.setName("outputVar2");
  EXPECT_EQ("outputVar2", outvar2.nameString());
  EXPECT_EQ(outvar2.emsVariableName(), toString(var2.handle()));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable).size());

  //WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable)[0];
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false));
  //EXPECT_EQ(var.nameString(), object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false).get());

  //model.save(toPath("./EMS_OutputVariable.osm"), true);
  //workspace.save(toPath("./EMS_OutputVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorOutputVariable2_EMS) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable var(model, "glob_var");
  EXPECT_EQ("glob_var", var.nameString());

  // add global variable
  EnergyManagementSystemGlobalVariable var2(model, "glob_var_2");
  EXPECT_EQ("glob_var_2", var2.nameString());

  // add output variable using modelObject
  EnergyManagementSystemOutputVariable outvar(model, var);
  EXPECT_EQ(outvar.emsVariableName(), toString(var.handle()));
  // add output variable using modelObject
  EnergyManagementSystemOutputVariable outvar2(model, var2);
  EXPECT_EQ(outvar2.emsVariableName(), toString(var2.handle()));
  //remove attached modelObject and test if the EMSOutVar does not get translated.
  var2.remove();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  //should only be 1 since the other attached modelObject was deleted
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable).size());

  //WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable)[0];
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false));
  //EXPECT_EQ(var.nameString(), object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false).get());

  model.save(toPath("./EMS_OutputVariable.osm"), true);
  workspace.save(toPath("./EMS_OutputVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorOutputVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_OutputVariable.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_OutputVariableT.osm"), true);

}
TEST_F(EnergyPlusFixture, ForwardTranslatorTrendVariable_EMS) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable globvar(model, "glob_var");

  // add trend variable
  EnergyManagementSystemTrendVariable var(model, "glob_var");
  var.setName("TestName");
  EXPECT_EQ("TestName", var.name().get());
  var.setEMSVariableName("glob_var");
  EXPECT_EQ("glob_var", var.emsVariableName());

  var.setNumberOfTimestepsToBeLogged(2);
  EXPECT_EQ(2, var.numberOfTimestepsToBeLogged());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_TrendVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_TrendVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName, false));
  EXPECT_EQ(globvar.nameString(), object.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName, false).get());
  ASSERT_TRUE(object.getDouble(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, false));
  EXPECT_EQ(2, object.getDouble(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, false).get());

  model.save(toPath("./EMS_TrendVariable.osm"), true);
  workspace.save(toPath("./EMS_TrendVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTrendVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_TrendVariable.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_TrendVariableT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorInternalVariable_EMS) {
  Model model;

  // add internal variable
  EnergyManagementSystemInternalVariable var(model, "TestName");
  var.setName("Test name");
  var.setInternalDataIndexKeyName("TestName");
  //var.setInternalDataType("TestName");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false));
  EXPECT_EQ("TestName", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false));
  EXPECT_EQ("TestName", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false).get());

  model.save(toPath("./EMS_InternalVariable.osm"), true);
  workspace.save(toPath("./EMS_InternalVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorInternalVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_InternalVariable.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_InternalVariableT.osm"), true);


}
TEST_F(EnergyPlusFixture, ForwardTranslatorConstructionIndexVariable_EMS) {
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  Construction construction(layers);

  EnergyManagementSystemConstructionIndexVariable emsCIV(model);
  emsCIV.setConstructionObject(construction);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ConstructionIndexVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ConstructionIndexVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, false));
  EXPECT_EQ(construction.nameString(), object.getString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, false).get());

  model.save(toPath("./EMS_constructiontest.osm"), true);
  workspace.save(toPath("./EMS_constructiontest.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorConstructionIndexVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_constructiontest.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_constructiontestT.osm"), true);


}
TEST_F(EnergyPlusFixture, ForwardTranslatorCurveOrTableIndexVariable_EMS) {
  Model model;

  // Create a curve
  CurveBiquadratic c1(model);

  EnergyManagementSystemCurveOrTableIndexVariable emsCurve(model);
  emsCurve.setCurveOrTableObject(c1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, false));
  EXPECT_EQ(c1.nameString(), object.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, false).get());

  model.save(toPath("./EMS_curvetest.osm"), true);
  workspace.save(toPath("./EMS_curvetest.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorCurveOrTableIndexVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_curvetest.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_curvetestT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorMeteredOutputVariable_EMS) {
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
  meteredoutvar.setUpdateFrequency("ZoneTimestep");
  meteredoutvar.setEMSProgramOrSubroutineName(program_1);
  meteredoutvar.setEMSProgramOrSubroutineName(subroutine_1);
  meteredoutvar.setResourceType("NaturalGas");
  meteredoutvar.setGroupType("HVAC");
  meteredoutvar.setEndUseCategory("Heating");
  meteredoutvar.setEndUseSubcategory("Madeup");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false));
  EXPECT_EQ(OATdbSensor.name().get(), object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false));
  EXPECT_EQ("ZoneTimestep", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false));
  EXPECT_EQ(subroutine_1.name().get(), object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::ResourceType, false));
  EXPECT_EQ("NaturalGas", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::ResourceType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::GroupType, false));
  EXPECT_EQ("HVAC", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::GroupType, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory, false));
  EXPECT_EQ("Heating", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, false));
  EXPECT_EQ("Madeup", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, false).get());
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::Units, false));
  //EXPECT_EQ("", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::Units, false).get());

  model.save(toPath("./EMS_meteredoutvar.osm"), true);
  workspace.save(toPath("./EMS_meteredoutvar.idf"), true);
}
TEST_F(EnergyPlusFixture, ReverseTranslatorMeteredOutputVariable_EMS) {

  openstudio::path idfPath = toPath("./EMS_meteredoutvar.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_meteredoutvarT.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorTrendVariable2_EMS) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable globvar(model, "glob_var");

  // add trend variable
  EnergyManagementSystemTrendVariable var(model, "glob_var");
  var.setName("TestName");
  EXPECT_EQ("TestName", var.name().get());
  //var.setEMSVariableName("glob_var");
  EXPECT_EQ("glob_var", var.emsVariableName());

  var.setNumberOfTimestepsToBeLogged(2);
  EXPECT_EQ(2, var.numberOfTimestepsToBeLogged());

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  AirGap air(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(air);
  layers.push_back(interior);

  Construction construction(layers);

  EnergyManagementSystemConstructionIndexVariable emsCIV(model);
  emsCIV.setConstructionObject(construction);

  // add trend variable
  EnergyManagementSystemTrendVariable var2(model, emsCIV.name().get());
  var2.setName("TestName2");
  EXPECT_EQ("TestName2", var2.name().get());
  //var2.setEMSVariableName(emsCIV.name().get());
  EXPECT_EQ(emsCIV.name().get(), var2.emsVariableName());

  var2.setNumberOfTimestepsToBeLogged(3);
  EXPECT_EQ(3, var2.numberOfTimestepsToBeLogged());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  model.save(toPath("./EMS_TrendVariable2.osm"), true);
  workspace.save(toPath("./EMS_TrendVariable2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTrendVariable2_EMS) {

  openstudio::path idfPath = toPath("./EMS_TrendVariable2.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save(toPath("./EMS_TrendVariable2T.osm"), true);

}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensorDelete_EMS) {

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

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  //there should be 1 sensor
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];

  //model.save(toPath("./EMS_SensorBeforeDelete.osm"), true);
  //workspace.save(toPath("./EMS_SensorBeforeDelete.idf"), true);

  avm.remove();
  // 0 avm in the model
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  //sensor still has keyName as avm UUID string (will not FT though eventually)
  EXPECT_EQ(key, sensor.keyName());

  Workspace workspaceDelete = forwardTranslator.translateModel(model);

  ASSERT_EQ(0u, workspaceDelete.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  //model.save(toPath("./EMS_SensorDelete.osm"), true);
  //workspaceDelete.save(toPath("./EMS_SensorDelete.idf"), true);

}
