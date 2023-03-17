/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
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
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
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
#include "../../utilities/core/Compare.hpp"

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
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <algorithm>
#include <sstream>
#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

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
  CoilCoolingDXSingleSpeed dx_coil(model, s, totalCoolingCapacityFunctionofTemperatureCurve, totalCoolingCapacityFunctionofFlowFractionCurve,
                                   energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                   partLoadFractionCorrelationCurve);

  dx_coil.setName("Coil Name Before Change");
  EXPECT_EQ("Coil Name Before Change", dx_coil.nameString());

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

  // model.save(toPath("./EMS_sensorRename.osm"), true);
  // workspace.save(toPath("./EMS_sensorRename.idf"), true);
}

Model prepareSensor1_EMS() {
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensor1_EMS) {

  Model model = prepareSensor1_EMS();

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
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature",
            object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  // model.save(toPath("./EMS_sensor1.osm"), true);
  // workspace.save(toPath("./EMS_sensor1.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor1_EMS) {

  Model model = prepareSensor1_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_sensor1T.osm"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor1_EMS_explicit) {
  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_zone1 = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone1);
  EXPECT_TRUE(_i_zone1->setName("Zone1"));
  OptionalWorkspaceObject _i_zone2 = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone2);
  EXPECT_TRUE(_i_zone2->setName("Zone2"));

  OptionalWorkspaceObject _i_outvar = w.addObject(IdfObject(IddObjectType::Output_Variable));
  ASSERT_TRUE(_i_outvar);
  EXPECT_TRUE(_i_outvar->setString(Output_VariableFields::VariableName, "Site Outdoor Air Drybulb Temperature"));

  OptionalWorkspaceObject _i_emsSensor = w.addObject(IdfObject(IddObjectType::EnergyManagementSystem_Sensor));
  ASSERT_TRUE(_i_emsSensor);
  EXPECT_TRUE(_i_emsSensor->setName("OATdb_Sensor"));
  EXPECT_TRUE(
    _i_emsSensor->setString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, "Site Outdoor Air Drybulb Temperature"));

  // To avoid other warnings, we add required objects
  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  // w.save(toPath("./EMS_sensor1.idf"), true);

  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_TRUE(reverseTranslator.errors().empty());
    EXPECT_TRUE(reverseTranslator.warnings().empty());

    std::vector<openstudio::model::EnergyManagementSystemSensor> emsSensors =
      model.getConcreteModelObjects<openstudio::model::EnergyManagementSystemSensor>();
    ASSERT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<openstudio::model::EnergyManagementSystemSensor>().size());

    openstudio::model::EnergyManagementSystemSensor emsSensor1 = model.getConcreteModelObjects<openstudio::model::EnergyManagementSystemSensor>()[0];
    EXPECT_EQ(_i_emsSensor->nameString(), emsSensor1.nameString());
    EXPECT_EQ("OATdb_Sensor", emsSensor1.nameString());
    boost::optional<openstudio::model::OutputVariable> _outvar = emsSensor1.outputVariable();
    ASSERT_TRUE(_outvar);
    EXPECT_EQ("Site Outdoor Air Drybulb Temperature", _outvar->variableName());
    EXPECT_EQ(_i_emsSensor->getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName).get(), _outvar->variableName());
  }
}

Model prepareSensor2_EMS() {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Zone 1");
  ThermalZone zone2(model);
  zone2.setName("Zone 2");

  // add Zone Lights Electricity Rate to both zones
  OutputVariable lightsElectricPower("Zone Lights Electricity Rate", model);

  // add light sensor on zone1
  EnergyManagementSystemSensor lights(model, lightsElectricPower);
  lights.setName("Light Sensor");
  // lights.setOutputVariable(lightsElectricPower);
  lights.setKeyName(zone1.name().get());

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSensor2_EMS) {

  Model model = prepareSensor2_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];
  WorkspaceObject outvar = workspace.getObjectsByType(IddObjectType::Output_Variable)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Name, false));
  EXPECT_EQ("Light_Sensor", object.getString(EnergyManagementSystem_SensorFields::Name, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false));
  EXPECT_EQ("Zone 1", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false));
  EXPECT_EQ("Zone Lights Electricity Rate", object.getString(EnergyManagementSystem_SensorFields::Output_VariableorOutput_MeterName, false).get());

  // model.save(toPath("./EMS_sensor2.osm"), true);
  // workspace.save(toPath("./EMS_sensor2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensor2_EMS) {

  Model model = prepareSensor1_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_sensor2T.osm"), true);
}

Model prepareSensoronMeter_EMS() {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Zone 1");
  ThermalZone zone2(model);
  zone2.setName("Zone 2");

  // create meter
  OutputMeter meter(model);
  meter.setName("test meter");
  meter.setMeterFileOnly(false);

  //add sensor to meter
  EnergyManagementSystemSensor meter_sensor(model, meter);
  meter_sensor.setName("meter sensor");
  //meter_sensor.setOutputMeter(meter);

  return model;
}
TEST_F(EnergyPlusFixture, ForwardTranslatorSensoronMeter_EMS) {

  Model model = prepareSensoronMeter_EMS();

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

  // model.save(toPath("./EMS_sensor_meter.osm"), true);
  // workspace.save(toPath("./EMS_sensor_meter.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSensoronMeter_EMS) {

  Model model = prepareSensoronMeter_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_sensor_meterT.osm"), true);
}
TEST_F(EnergyPlusFixture, ForwardReverseTranslatorActuator_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);

  // Assign it to a loop
  AirLoopHVAC a(model);
  Node supplyOutletNode = a.supplyOutletNode();
  fan.addToNode(supplyOutletNode);

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  EnergyManagementSystemActuator fanActuator(fan, ComponentType, fanControlType);
  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Fan_ConstantVolume).size());

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

  // model.save(toPath("./EMS_example.osm"), true);
  // workspace.save(toPath("./EMS_example.idf"), true);

  ReverseTranslator reverseTranslator;
  Model modelT = reverseTranslator.translateWorkspace(workspace);
  // modelT.save(toPath("./EMS_exampleT.osm"), true);
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

  // model.save(toPath("./EMS_weatheractuator.osm"), true);
  // workspace.save(toPath("./EMS_weatheractuator.idf"), true);
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

  // model.save(toPath("./EMS_weatheractuator2.osm"), true);
  // workspace.save(toPath("./EMS_weatheractuator2.idf"), true);
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
  std::string ControlType = "NaturalGas Rate";
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

  // model.save(toPath("./EMS_example.osm"), true);
  // workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_SpaceType_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Zone1");

  Space space(model);
  space.setName("Space1");
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
  std::string ControlType = "NaturalGas Rate";
  std::string ComponentType = "GasEquipment";
  EnergyManagementSystemActuator gasEquipmentActuator(gasEquip, ComponentType, ControlType);
  std::string actName = "Gas Equip Actuator";
  gasEquipmentActuator.setName(actName);

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Actuator 'Gas_Equip_Actuator' references a SpaceLoad 'Gas Equip' attached to the SpaceType 'Space Type 1' but you have turned off "
              "ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Zone1' since there is only 1.",
              ft.warnings().front().logMessage());
    EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Gas_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    //Should have TZ in the name since the load is attached to a spacetype with only 1 space
    EXPECT_EQ("Zone1 Gas Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Actuator 'Gas_Equip_Actuator' references a SpaceLoad 'Gas Equip' attached to the SpaceType 'Space Type 1' but you have turned on "
              "ForwardTranslation's Space Feature. Falling back to using the attached Space 'Space1' since there is only 1.",
              ft.warnings().front().logMessage());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Gas_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    //Should have Space in the name since the load is attached to a spacetype with only 1 space
    EXPECT_EQ("Space1 Gas Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }
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

  // model.save(toPath("./EMS_example.osm"), true);
  // workspace.save(toPath("./EMS_example.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuatorSpaceLoad_SpaceType2_EMS) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  zone1.setName("Zone1");

  Space space(model);
  space.setName("Space1");
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

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Actuator 'Electric_Equip_Actuator' references a SpaceLoad 'Electric Equip' attached to the SpaceType 'Space Type 1' but you have "
              "turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Zone1' since there is only 1.",
              ft.warnings().front().logMessage());

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    //Should have TZ in the name since the load is attached to a spacetype with only 1 space
    EXPECT_EQ("Zone1 Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Actuator 'Electric_Equip_Actuator' references a SpaceLoad 'Electric Equip' attached to the SpaceType 'Space Type 1' but you have "
              "turned on ForwardTranslation's Space Feature. Falling back to using the attached Space 'Space1' since there is only 1.",
              ft.warnings().front().logMessage());

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    //Should have Space in the name since the load is attached to a spacetype with only 1 space
    EXPECT_EQ("Space1 Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }
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
  std::string ControlType = "NaturalGas Rate";
  std::string ComponentType = "GasEquipment";
  EnergyManagementSystemActuator fanActuator(gasEquip, ComponentType, ControlType);
  std::string actName = "Gas Equip Actuator";
  fanActuator.setName(actName);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(1u, forwardTranslator.errors().size());
  // should now be 0 since the actuator should not translate since the SpaceLoad got deleted on FT
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

  // model.save(toPath("./EMS_example.osm"), true);
  // workspace.save(toPath("./EMS_example.idf"), true);
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
  space1.setName("Space1");
  space1.setThermalZone(zone1);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setName("Space2");
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

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    ft.setExcludeSpaceTranslation(true);
    Workspace workspace = ft.translateModel(model);
    //expect an error since there are 2 spaces with the same spaceType that resolve to two dfferent zones, so the fallback is less safe than it there
    // was only one, hence an error not a warning
    EXPECT_EQ(1, ft.errors().size());
    EXPECT_EQ("Actuator 'Electric_Equip_Actuator' references a SpaceLoad 'Electric Equip' attached to the SpaceType 'Space Type 1' but you have "
              "turned off ForwardTranslation's Space Feature. Your SpaceType ends up linked to multiples Thermal Zones. Falling back to using the "
              "first Thermal Zone 'Thermal Zone 1'.",
              ft.errors().front().logMessage());
    EXPECT_EQ(0, ft.warnings().size());
    //expect 1 actuator since there are 2 zones with the same spaceType but only 1 will get translated right now
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    EXPECT_EQ("Thermal Zone 1 Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace workspace = ft.translateModel(model);
    //expect an error since there are 2 spaces with the same spaceType that resolve to two dfferent zones
    EXPECT_EQ(1, ft.errors().size());
    EXPECT_EQ(
      "Actuator 'Electric_Equip_Actuator' references a SpaceLoad 'Electric Equip' attached to the SpaceType 'Space Type 1' but you have turned on "
      "ForwardTranslation's Space Feature. The Space Type has multiple spaces attached. Falling back to using the first Space 'Space1'.",
      ft.errors().front().logMessage());
    EXPECT_EQ(0, ft.warnings().size());
    //expect 1 actuator since there are 2 zones with the same spaceType but only 1 will get translated right now
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Electric_Equip_Actuator", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    EXPECT_EQ("Space1 Electric Equip", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }
}

Model prepareProgram_EMS() {

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
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle())
                                   + " / 15.0 !- This is !nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n"
                                   + "no comment\n" + "crap !comment!comment";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle())
                                      + " / 15.0 !- This is !nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n"
                                      + "no comment\n" + "crap !comment!comment\n";
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorProgram_EMS) {

  std::string fan_program_body_test;
  std::string line1_test;
  std::string line2_test;
  Model model = prepareProgram_EMS();
  // model.save(toPath("./EMS_program.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ProgramFields::Name, false));
  EXPECT_EQ("Fan_Constant_Volume_1Pressure_Rise_Program_by_Body", object.getString(EnergyManagementSystem_ProgramFields::Name, false).get());

  // workspace.save(toPath("./EMS_program.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorProgram_EMS) {

  Model model = prepareProgram_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_programT.osm"), true);
}

Model prepareSubroutine_EMS() {
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
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle())
                                   + " extra!- This is !nonsense\r\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n"
                                   + "no comment\n" + "crap !comment!comment";
  //this is what the body should look like with 2 /n's and compare TRUE
  std::string fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " + " + toString(fanActuator.handle())
                                      + " extra !- This is !nonsense\nSET " + toString(fanActuator.handle()) + " = 250 * mult !- More nonsense\n"
                                      + "no comment\n" + "crap !comment!comment\n";
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorSubroutine_EMS) {

  Model model = prepareSubroutine_EMS();
  // model.save(toPath("./EMS_subroutine.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Subroutine).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Subroutine)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_SubroutineFields::Name, false));
  EXPECT_EQ("Fan_Constant_Volume_1Pressure_Rise_Program_by_Body", object.getString(EnergyManagementSystem_SubroutineFields::Name, false).get());

  // workspace.save(toPath("./EMS_subroutine.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorSubroutine_EMS) {

  Model model = prepareSubroutine_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_subroutineT.osm"), true);
}

Model prepareCallingManager_EMS() {

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
  std::string fan_program_1_body = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\r\nSET "
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
  fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator.handle())
                          + " = 250 * mult !- More nonsense\n";
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
  fan_program_body_test = "SET mult = " + toString(OATdbSensor.handle()) + " / 15.0 !- This is nonsense\nSET " + toString(fanActuator.handle())
                          + " = 250 * mult !- More nonsense\n";
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorProgramCallingManager_EMS) {

  Model model = prepareCallingManager_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager)[0];

  ASSERT_TRUE(object.getString(EnergyManagementSystem_ProgramCallingManagerFields::Name, false));
  EXPECT_EQ("Best Program Calling Manager", object.getString(EnergyManagementSystem_ProgramCallingManagerFields::Name, false).get());

  // model.save(toPath("./EMS_PCM.osm"), true);
  // workspace.save(toPath("./EMS_PCM.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorProgramCallingManager_EMS) {

  Model model = prepareCallingManager_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_PCMT.osm"), true);
}

TEST_F(EnergyPlusFixture, noForwardTranslatorOutput_EMS) {
  Model model;

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Output_EnergyManagementSystem).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslatorOutput_EMS) {
  Model model;
  auto oEMS = model.getUniqueModelObject<OutputEnergyManagementSystem>();

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

  // model.save(toPath("./EMS_output.osm"), true);
  // workspace.save(toPath("./EMS_output.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorOutput_EMS) {

  Model model;
  auto oEMS = model.getUniqueModelObject<OutputEnergyManagementSystem>();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_outputT.osm"), true);
}

Model prepareGlobalVariable_EMS() {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable var(model, "glob_var");
  EXPECT_EQ("glob_var", var.nameString());

  // add global variable
  EnergyManagementSystemGlobalVariable var2(model, "glob_var_2");
  EXPECT_EQ("glob_var_2", var2.nameString());

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorGlobalVariable_EMS) {

  Model model = prepareGlobalVariable_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_GlobalVariable)[0];
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false));
  //EXPECT_EQ(var.nameString(), object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false).get());

  // model.save(toPath("./EMS_GlobalVariable.osm"), true);
  // workspace.save(toPath("./EMS_GlobalVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorGlobalVariable_EMS) {

  Model model = prepareGlobalVariable_EMS();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_GlobalVariableT.osm"), true);
}

/*
TEST_F(EnergyPlusFixture, ReverseTranslatorGlobalVariable2_EMS) {

  openstudio::path idfPath = toPath("./EMS_GlobalVariable2.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  // model.save(toPath("./EMS_GlobalVariable2T.osm"), true);

}
*/

Model prepareOutputVariable_EMS() {

  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable var(model, "glob_var");
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorOutputVariable_EMS) {

  Model model = prepareOutputVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable).size());

  //WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_OutputVariable)[0];
  //ASSERT_TRUE(object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false));
  //EXPECT_EQ(var.nameString(), object.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, false).get());

  // model.save(toPath("./EMS_OutputVariable.osm"), true);
  // workspace.save(toPath("./EMS_OutputVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorOutputVariable_EMS) {

  Model model = prepareOutputVariable_EMS();
  ForwardTranslator forwardTranslator;

  Workspace workspace = forwardTranslator.translateModel(model);
  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_OutputVariableT.osm"), true);
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

  // model.save(toPath("./EMS_OutputVariable.osm"), true);
  // workspace.save(toPath("./EMS_OutputVariable.idf"), true);
}

Model prepareTrendVariable_EMS() {
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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTrendVariable_EMS) {

  Model model = prepareTrendVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_TrendVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_TrendVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName, false));
  EXPECT_EQ("glob_var", object.getString(EnergyManagementSystem_TrendVariableFields::EMSVariableName, false).get());
  ASSERT_TRUE(object.getDouble(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, false));
  EXPECT_EQ(2, object.getDouble(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, false).get());

  // model.save(toPath("./EMS_TrendVariable.osm"), true);
  // workspace.save(toPath("./EMS_TrendVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTrendVariable_EMS) {

  Model model = prepareTrendVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_TrendVariableT.osm"), true);
}

Model prepareInternalVariable_EMS() {

  Model model;

  // add internal variable
  EnergyManagementSystemInternalVariable var(model, "TestName");
  var.setName("Test name");
  var.setInternalDataIndexKeyName("TestName");
  //var.setInternalDataType("TestName");

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorInternalVariable_EMS) {

  Model model = prepareInternalVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false));
  EXPECT_EQ("TestName", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false));
  EXPECT_EQ("TestName", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false).get());

  // model.save(toPath("./EMS_InternalVariable.osm"), true);
  // workspace.save(toPath("./EMS_InternalVariable.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorInternalVariable_EMS) {

  Model model = prepareInternalVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_InternalVariableT.osm"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorInternalVariable2_EMS) {
  Model model;
  //add zone
  ThermalZone zone(model);
  zone.setName("Test Zone");
  // add internal variable
  EnergyManagementSystemInternalVariable var(model, "Zone Air Volume");
  //test using UID in this field
  var.setInternalDataIndexKeyName(toString(zone.handle()));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_InternalVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false));
  //name string should match instead of UID of zone
  EXPECT_EQ("Test Zone", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false));
  EXPECT_EQ("Zone Air Volume", object.getString(EnergyManagementSystem_InternalVariableFields::InternalDataType, false).get());

  // model.save(toPath("./EMS_InternalVariable2.osm"), true);
  // workspace.save(toPath("./EMS_InternalVariable2.idf"), true);
}

Model prepareConstructionIndexVariable_EMS() {

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
  construction.setName("My Construction");

  EnergyManagementSystemConstructionIndexVariable emsCIV(model);
  emsCIV.setConstructionObject(construction);

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorConstructionIndexVariable_EMS) {

  Model model = prepareConstructionIndexVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ConstructionIndexVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ConstructionIndexVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, false));
  EXPECT_EQ("My Construction", object.getString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, false).get());

  // model.save(toPath("./EMS_constructiontest.osm"), true);
  // workspace.save(toPath("./EMS_constructiontest.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorConstructionIndexVariable_EMS) {

  Model model = prepareConstructionIndexVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_constructiontestT.osm"), true);
}

Model prepareCurveOrTableIndexVariable_EMS() {

  Model model;

  // Create a curve
  CurveBiquadratic c1(model);
  c1.setName("My CurveBiquadratic");

  EnergyManagementSystemCurveOrTableIndexVariable emsCurve(model);
  emsCurve.setCurveOrTableObject(c1);

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorCurveOrTableIndexVariable_EMS) {

  Model model = prepareCurveOrTableIndexVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_CurveOrTableIndexVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, false));
  EXPECT_EQ("My CurveBiquadratic", object.getString(EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, false).get());

  // model.save(toPath("./EMS_curvetest.osm"), true);
  // workspace.save(toPath("./EMS_curvetest.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorCurveOrTableIndexVariable_EMS) {

  Model model = prepareCurveOrTableIndexVariable_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_curvetestT.osm"), true);
}

Model prepareMeteredOutputVariable_EMS(bool withUID) {

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

  boost::optional<EnergyManagementSystemMeteredOutputVariable> _meteredoutvar;
  if (withUID) {
    // add metered output variable using UID
    _meteredoutvar = EnergyManagementSystemMeteredOutputVariable(model, toString(OATdbSensor.handle()));
  } else {
    // add metered output variable via name
    _meteredoutvar = EnergyManagementSystemMeteredOutputVariable(model, OATdbSensor.name().get());
  }

  //_meteredoutvar->setEMSVariableName(OATdbSensor.name().get());
  _meteredoutvar->setUpdateFrequency("ZoneTimestep");
  _meteredoutvar->setEMSProgramOrSubroutineName(program_1);
  _meteredoutvar->setEMSProgramOrSubroutineName(subroutine_1);
  _meteredoutvar->setResourceType("NaturalGas");
  _meteredoutvar->setGroupType("HVAC");
  _meteredoutvar->setEndUseCategory("Heating");
  _meteredoutvar->setEndUseSubcategory("Madeup");

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorMeteredOutputVariable_EMS) {

  Model model = prepareMeteredOutputVariable_EMS(false);  // Via name and not UID
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false));
  EXPECT_EQ("OATdb_Sensor", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false));
  EXPECT_EQ("ZoneTimestep", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false));
  EXPECT_EQ("subroutine_one", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false).get());
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

  // model.save(toPath("./EMS_meteredoutvar.osm"), true);
  // workspace.save(toPath("./EMS_meteredoutvar.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorMeteredOutputVariable2_EMS) {

  Model model = prepareMeteredOutputVariable_EMS(true);  // Via UID (not name)
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_MeteredOutputVariable)[0];
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false));
  //expect name string to be substituted from UID and match
  EXPECT_EQ("OATdb_Sensor", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false));
  EXPECT_EQ("ZoneTimestep", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, false).get());
  ASSERT_TRUE(object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false));
  EXPECT_EQ("subroutine_one", object.getString(EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, false).get());
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

  // model.save(toPath("./EMS_meteredoutvar2.osm"), true);
  // workspace.save(toPath("./EMS_meteredoutvar2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorMeteredOutputVariable_EMS) {

  Model model = prepareMeteredOutputVariable_EMS(true);  // Via UID (not name)
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_meteredoutvarT.osm"), true);
}

Model prepareTrendVariable2_EMS() {

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

  return model;
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTrendVariable2_EMS) {

  Model model = prepareTrendVariable2_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  // model.save(toPath("./EMS_TrendVariable2.osm"), true);
  // workspace.save(toPath("./EMS_TrendVariable2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTrendVariable2_EMS) {

  Model model = prepareTrendVariable2_EMS();
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ReverseTranslator reverseTranslator;
  Model m = reverseTranslator.translateWorkspace(workspace);
  // m.save(toPath("./EMS_TrendVariable2T.osm"), true);
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
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<EnergyManagementSystemSensor>().size());
  // 1 avm in the model
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  //there should be 1 sensor
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());
  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor)[0];

  //model.save(toPath("./EMS_SensorBeforeDelete.osm"), true);
  //workspace.save(toPath("./EMS_SensorBeforeDelete.idf"), true);

  avm.remove();
  // 0 avm in the model
  EXPECT_EQ(static_cast<unsigned>(0), model.getConcreteModelObjects<AvailabilityManagerHighTemperatureTurnOff>().size());
  //sensor still has keyName as avm UUID string (will not FT though eventually)
  EXPECT_EQ(key, sensor.keyName());

  Workspace workspaceDelete = forwardTranslator.translateModel(model);

  ASSERT_EQ(0u, workspaceDelete.getObjectsByType(IddObjectType::EnergyManagementSystem_Sensor).size());

  //model.save(toPath("./EMS_SensorDelete.osm"), true);
  //workspaceDelete.save(toPath("./EMS_SensorDelete.idf"), true);
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuator_exampleModel_Lights_EMS) {
  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names

  Model model = exampleModel();
  auto oems = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  oems.setActuatorAvailabilityDictionaryReporting("Verbose");

  std::vector<Lights> lights = model.getConcreteModelObjects<Lights>();
  std::vector<ElectricEquipment> electricEquipment = model.getConcreteModelObjects<ElectricEquipment>();
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  std::vector<ThermalZone> thermalZone = model.getConcreteModelObjects<ThermalZone>();
  boost::optional<Space> space4 = model.getConcreteModelObjectByName<Space>("Space 4");
  //this will create 1 zone in a zonelist

  //This is the EDD file for the spaceload actuators for this model so far
  /*
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 PEOPLE 1,People,Number of People,[each]
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 LIGHTS 1,Lights,Electricity Rate,[W]
  EnergyManagementSystem:Actuator Available,PRINTER,ElectricEquipment,Electricity Rate,[W]
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 ELECTRIC EQUIPMENT 1,ElectricEquipment,Electricity Rate,[W]
  */

  //actuator settings
  //std::string ComponentType = "ElectricEquipment";
  std::string ComponentType = "Lights";
  std::string ControlType = "Electricity Rate";
  EnergyManagementSystemActuator lightsActuator(lights[0], ComponentType, ControlType, space4.get());
  EXPECT_EQ(space4->handle(), lightsActuator.zoneOrSpace()->handle());

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    // We get only a warning, since we can recover from it easily
    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ(
      "Actuator 'Energy_Management_System_Actuator_1' references a SpaceLoad 'Lights 1' attached to the SpaceType 'Space Type 1' but you have turned "
      "off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Thermal Zone 1' since there is only 1.",
      ft.warnings().front().logMessage());

    //expect 1 actuator since there are 4 spaces with the same spaceType but only 1 will get translated right now
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Energy_Management_System_Actuator_1", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    EXPECT_EQ("Thermal Zone 1 Lights 1", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    // No warnings here (from EMS), the space is set
    Workspace workspace = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Object of type 'OS:ThermalZone' and named 'Thermal Zone 1' has DaylightingControl Objects assigned. The interior walls between Spaces "
              "will be merged. Make sure these are correctly Matched!",
              ft.warnings().front().logMessage());

    //expect 1 actuator since there are 4 spaces with the same spaceType but only 1 will get translated right now
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    WorkspaceObject object = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator)[0];

    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::Name, false));
    EXPECT_EQ("Energy_Management_System_Actuator_1", object.getString(EnergyManagementSystem_ActuatorFields::Name, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false));
    EXPECT_EQ("Space 4 Lights 1", object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false));
    EXPECT_EQ(ComponentType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get());
    ASSERT_TRUE(object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false));
    EXPECT_EQ(ControlType, object.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, false).get());
  }
}
TEST_F(EnergyPlusFixture, ForwardTranslatorActuator_exampleModel_Electric_EMS) {
  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist (or spaceList if m_forwardTranslatorOptions.excludeSpaceTranslation() = false) is created from the spaceType name,
  //and the zones in the list are the space.thermalzone names

  Model model = exampleModel();
  auto oems = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  oems.setActuatorAvailabilityDictionaryReporting("Verbose");

  std::vector<Lights> lights = model.getConcreteModelObjects<Lights>();
  std::vector<ElectricEquipment> electricEquipment = model.getConcreteModelObjects<ElectricEquipment>();
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  std::vector<ThermalZone> thermalZone = model.getConcreteModelObjects<ThermalZone>();
  boost::optional<Space> space4 = model.getConcreteModelObjectByName<Space>("Space 4");
  ASSERT_TRUE(space4);
  //this will create 1 zone in a zonelist

  //This is the EDD file for the spaceload actuators for this model so far
  /*
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 PEOPLE 1,People,Number of People,[each]
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 LIGHTS 1,Lights,Electricity Rate,[W]
  EnergyManagementSystem:Actuator Available,PRINTER,ElectricEquipment,Electricity Rate,[W]
  EnergyManagementSystem:Actuator Available,THERMAL ZONE 1 ELECTRIC EQUIPMENT 1,ElectricEquipment,Electricity Rate,[W]
  */

  //actuator settings
  std::string ComponentType = "ElectricEquipment";
  std::string ControlType = "Electricity Rate";
  //actuator on first electricEquipment object
  // NOTE: there is electricEquipment on a Space and one on a Space Type. I am VOLUNTARILY not setting the space4 for the Space Type one
  if (electricEquipment[0].space()) {
    EnergyManagementSystemActuator electricActuator0(electricEquipment[0], ComponentType, ControlType);
    EnergyManagementSystemActuator electricActuator1(electricEquipment[1], ComponentType, ControlType);  // , space4.get());
  } else {
    // 0 is on a SpaceType, 1 is on a Space
    EnergyManagementSystemActuator electricActuator1(electricEquipment[1], ComponentType, ControlType);
    EnergyManagementSystemActuator electricActuator0(electricEquipment[0], ComponentType, ControlType);  // , space4.get());
  }

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace workspace = ft.translateModel(model);

    // We get only a warning, since we can recover from it easily
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ(
      "Actuator 'Energy_Management_System_Actuator_2' references a SpaceLoad 'Electric Equipment 1' attached to the SpaceType 'Space Type 1' but you "
      "have turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Thermal Zone 1' since there is only 1.",
      ft.warnings().front().logMessage());

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuator since there are 4 spaces with the same spaceType but only 1 will get translated right now
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());
    EXPECT_EQ("Printer", actuatedNames.at(0));
    EXPECT_EQ("Thermal Zone 1 Electric Equipment 1", actuatedNames.at(1));
    //model.save(toPath("./EMS_actuator_exampleModel_electric.osm"), true);
    //workspace.save(toPath("./EMS_actuator_exampleModel_electric.idf"), true);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);
    Workspace workspace = ft.translateModel(model);

    // NOTE: the space is NOT set on the Space Type one, and it has multiple spaces, so there will be an error. It will end up selecting the Space 1
    // to set it though (it selects the Space sorted by name, so Space 1)
    EXPECT_EQ(1u, ft.errors().size());
    EXPECT_EQ("Actuator 'Energy_Management_System_Actuator_2' references a SpaceLoad 'Electric Equipment 1' attached to the SpaceType 'Space Type 1' "
              "but you have turned on ForwardTranslation's Space Feature. The Space Type has multiple spaces attached. Falling back to using the "
              "first Space 'Space 1'.",
              ft.errors().front().logMessage());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Object of type 'OS:ThermalZone' and named 'Thermal Zone 1' has DaylightingControl Objects assigned. The interior walls between Spaces "
              "will be merged. Make sure these are correctly Matched!",
              ft.warnings().front().logMessage());

    //expect 2 actuator since there are 4 spaces with the same spaceType but only 1 will get translated right now
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuator since there are 4 spaces with the same spaceType but only 1 will get translated right now
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());
    EXPECT_EQ("Printer", actuatedNames.front());
    EXPECT_EQ("Space 1 Electric Equipment 1", actuatedNames.back());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorActuator_API_EMS) {
  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names

  //Model model;
  Model model = exampleModel();
  auto oems = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  oems.setActuatorAvailabilityDictionaryReporting("Verbose");

  std::vector<Lights> lights = model.getConcreteModelObjects<Lights>();
  std::vector<ElectricEquipment> electricEquipment = model.getConcreteModelObjects<ElectricEquipment>();
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  std::vector<ThermalZone> thermalZone = model.getConcreteModelObjects<ThermalZone>();

  auto thermalZone2 = thermalZone[0].clone(model).cast<ThermalZone>();
  auto thermalZone3 = thermalZone[0].clone(model).cast<ThermalZone>();
  auto thermalZone4 = thermalZone[0].clone(model).cast<ThermalZone>();
  //this will create 4 zones in a zonelist
  spaces[0].setThermalZone(thermalZone[0]);  // Space 1
  spaces[1].setThermalZone(thermalZone2);    // Space 2
  spaces[2].setThermalZone(thermalZone3);    // Space 3
  spaces[3].setThermalZone(thermalZone4);    // Space 4
  //This is the EDD file for the spaceload actuators for this model so far
  /*
EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 PEOPLE 1, People, Number of People, [each]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 PEOPLE 1, People, Number of People, [each]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 PEOPLE 1, People, Number of People, [each]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 PEOPLE 1, People, Number of People, [each]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 LIGHTS 1, Lights, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 LIGHTS 1, Lights, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 LIGHTS 1, Lights, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 LIGHTS 1, Lights, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, PRINTER, ElectricEquipment, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
*/

  // NOTE: here I'm setting one actuator to a Zone and one to a Space
  //actuator settings
  std::string lightsComponentType = "Lights";
  std::string lightsControlType = "Electricity Rate";
  //create actuator zone2
  EnergyManagementSystemActuator lightsActuator2(lights[0], lightsComponentType, lightsControlType, spaces[2].thermalZone().get());
  //EXPECT_EQ(lightsControlType, lightsActuator2.actuatedComponentControlType());
  //EXPECT_EQ(lightsComponentType, lightsActuator2.actuatedComponentType());
  //EXPECT_EQ(lights, lightsActuator2.actuatedComponent().get());
  EXPECT_EQ(spaces[2].thermalZone()->handle(), lightsActuator2.thermalZone().get().handle());

  //create actuator zone3
  EnergyManagementSystemActuator lightsActuator3(lights[0], lightsComponentType, lightsControlType, spaces[3]);
  //EXPECT_EQ(lightsControlType, lightsActuator3.actuatedComponentControlType());
  //EXPECT_EQ(lightsComponentType, lightsActuator3.actuatedComponentType());
  //EXPECT_EQ(lights, lightsActuator3.actuatedComponent().get());
  EXPECT_EQ(spaces[3].handle(), lightsActuator3.space().get().handle());

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace workspace = ft.translateModel(model);

    // We get only a warning, since we can recover from it easily
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ(
      "Actuator 'Energy_Management_System_Actuator_2' references a SpaceLoad 'Lights 1' and specified that it wanted to be attached to the Space "
      "'Space 4' but you have turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Thermal Zone 4'.",
      ft.warnings().front().logMessage());

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuators
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());

    EXPECT_EQ("Thermal Zone 3 Lights 1", actuatedNames.at(0));
    EXPECT_EQ("Thermal Zone 4 Lights 1", actuatedNames.at(1));

    //model.save(toPath("./EMS_actuator_exampleModel2_electric.osm"), true);
    //workspace.save(toPath("./EMS_actuator_exampleModel2_electric.idf"), true);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);
    Workspace workspace = ft.translateModel(model);

    // We get only a warning, since we can recover from it easily
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(1, ft.warnings().size());
    EXPECT_EQ("Actuator 'Energy_Management_System_Actuator_1' references a SpaceLoad 'Lights 1' and specified that it wanted to be attached to the "
              "Thermal Zone 'Thermal Zone 3' but you have turned on ForwardTranslation's Space Feature. Falling back to using the attached Space "
              "'Space 3' since there is only 1.",
              ft.warnings().front().logMessage());

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuators
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());

    EXPECT_EQ("Space 3 Lights 1", actuatedNames.at(0));
    EXPECT_EQ("Space 4 Lights 1", actuatedNames.at(1));
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorActuator_API3_EMS) {
  //SAME AS PREVIOUS TEST BUT DELETE THE SPACE BEFORE FT
  //use spacetype with multiple spaces
  //this is the issue with spaceloads if there are multiple spaces using a spaceload defined in a spaceType
  //the zonelist is created from the spaceType name, and the zones in the list are the space.thermalzone names

  //Model model;
  Model model = exampleModel();
  auto oems = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  oems.setActuatorAvailabilityDictionaryReporting("Verbose");

  std::vector<Lights> lights = model.getConcreteModelObjects<Lights>();
  std::vector<ElectricEquipment> electricEquipment = model.getConcreteModelObjects<ElectricEquipment>();
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  std::vector<ThermalZone> thermalZone = model.getConcreteModelObjects<ThermalZone>();

  auto thermalZone2 = thermalZone[0].clone(model).cast<ThermalZone>();
  auto thermalZone3 = thermalZone[0].clone(model).cast<ThermalZone>();
  auto thermalZone4 = thermalZone[0].clone(model).cast<ThermalZone>();
  //this will create 4 zones in a zonelist
  spaces[0].setThermalZone(thermalZone[0]);
  spaces[1].setThermalZone(thermalZone2);
  spaces[2].setThermalZone(thermalZone3);
  spaces[3].setThermalZone(thermalZone4);
  //This is the EDD file for the spaceload actuators for this model so far
  /*
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 PEOPLE 1, People, Number of People, [each]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 PEOPLE 1, People, Number of People, [each]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 PEOPLE 1, People, Number of People, [each]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 PEOPLE 1, People, Number of People, [each]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 LIGHTS 1, Lights, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 LIGHTS 1, Lights, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 LIGHTS 1, Lights, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 LIGHTS 1, Lights, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, PRINTER, ElectricEquipment, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 1 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 2 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 3 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
  EnergyManagementSystem : Actuator Available, THERMAL ZONE 4 ELECTRIC EQUIPMENT 1, ElectricEquipment, Electricity Rate, [W]
  */

  //actuator settings
  std::string lightsComponentType = "Lights";
  std::string lightsControlType = "Electricity Rate";
  //create actuator zone2
  EnergyManagementSystemActuator lightsActuator2(lights[0], lightsComponentType, lightsControlType, thermalZone2);
  //EXPECT_EQ(lightsControlType, lightsActuator2.actuatedComponentControlType());
  //EXPECT_EQ(lightsComponentType, lightsActuator2.actuatedComponentType());
  //EXPECT_EQ(lights, lightsActuator2.actuatedComponent().get());
  EXPECT_EQ(spaces[1].thermalZone()->handle(), lightsActuator2.thermalZone()->handle());

  //create actuator zone3
  EnergyManagementSystemActuator lightsActuator3(lights[0], lightsComponentType, lightsControlType, thermalZone3);
  //EXPECT_EQ(lightsControlType, lightsActuator3.actuatedComponentControlType());
  //EXPECT_EQ(lightsComponentType, lightsActuator3.actuatedComponentType());
  //EXPECT_EQ(lights, lightsActuator3.actuatedComponent().get());
  EXPECT_EQ(spaces[2].thermalZone()->handle(), lightsActuator3.thermalZone()->handle());

  //this will create 4 zones in a zonelist
  spaces[0].setThermalZone(thermalZone[0]);
  spaces[1].setThermalZone(thermalZone[0]);
  spaces[2].setThermalZone(thermalZone[0]);
  spaces[3].setThermalZone(thermalZone[0]);

  thermalZone2.remove();
  thermalZone3.remove();
  thermalZone4.remove();

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace workspace = ft.translateModel(model);

    // We get only a warning, since we can recover from it easily
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(2, ft.warnings().size());
    EXPECT_TRUE(checkLogMessagesContain(
      ft.warnings(),
      {
        "Actuator 'Energy_Management_System_Actuator_1' references a SpaceLoad 'Lights 1' attached to the SpaceType 'Space Type 1' but you have "
        "turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Thermal Zone 1' since there is only 1.",
        //
        "Actuator 'Energy_Management_System_Actuator_2' references a SpaceLoad 'Lights 1' attached to the SpaceType 'Space Type 1' but you have "
        "turned off ForwardTranslation's Space Feature. Falling back to using the attached ThermalZone 'Thermal Zone 1' since there is only 1.",
      }));

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuators
    //ACTUATORS WILL STILL GET TRANSLATED WITH BLANK ZONENAME FIELD
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());

    // TODO: hum we end up with two actuators on the same zone, this is problematic... We did issue helpful warnings, and let E+ catch the issue.
    // Can't always protect the user against themselves
    EXPECT_EQ("Thermal Zone 1 Lights 1", actuatedNames.at(0));
    EXPECT_EQ("Thermal Zone 1 Lights 1", actuatedNames.at(1));

    //model.save(toPath("./EMS_actuator_exampleModel2_electric.osm"), true);
    //workspace.save(toPath("./EMS_actuator_exampleModel2_electric.idf"), true);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);
    Workspace workspace = ft.translateModel(model);

    // We get only a warning, since we can recover from it easily
    EXPECT_EQ(2, ft.errors().size());
    EXPECT_TRUE(checkLogMessagesContain(
      ft.errors(),
      {
        "Actuator 'Energy_Management_System_Actuator_1' references a SpaceLoad 'Lights 1' attached to the SpaceType 'Space Type 1' but you have "
        "turned "
        "on ForwardTranslation's Space Feature. The Space Type has multiple spaces attached. Falling back to using the first Space 'Space 1'.",
        //
        "Actuator 'Energy_Management_System_Actuator_2' references a SpaceLoad 'Lights 1' attached to the SpaceType 'Space Type 1' but you have "
        "turned "
        "on ForwardTranslation's Space Feature. The Space Type has multiple spaces attached. Falling back to using the first Space 'Space 1'.",
      }));
    EXPECT_TRUE(checkLogMessagesSizeWithExclusions(
      0, ft.warnings(),
      {"Object of type 'OS:ThermalZone' and named 'Thermal Zone 1' has DaylightingControl Objects assigned. The interior "
       "walls between Spaces will be merged. Make sure these are correctly Matched!"}));

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    //expect 2 actuators
    //ACTUATORS WILL STILL GET TRANSLATED WITH BLANK ZONENAME FIELD
    ASSERT_EQ(2, objects.size());

    std::vector<std::string> actuatedNames;
    std::transform(objects.cbegin(), objects.cend(), std::back_inserter(actuatedNames),
                   [](const auto& wo) { return wo.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).get(); });
    std::sort(actuatedNames.begin(), actuatedNames.end());

    // TODO: hum we end up with two actuators on the same zone, this is problematic... We did issue helpful warnings, and let E+ catch the issue.
    // Can't always protect the user against themselves
    EXPECT_EQ("Space 1 Lights 1", actuatedNames.at(0));
    EXPECT_EQ("Space 1 Lights 1", actuatedNames.at(1));
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslatorProgramWeirdFormatting_EMS) {

  // This is a dummy IDF snippet
  // Its goal is to test the various weird formatting pattern that would still work with E+
  std::string emsSnippet("  EnergyManagementSystem:ProgramCallingManager,\n"
                         "    ProgCaller,              !- Name\n"
                         "    EndOfZoneTimestepBeforeZoneReporting,  !- EnergyPlus Model Calling Point\n"
                         "    DummyProg;               !- Program Name 1\n"
                         "\n"
                         "  EnergyManagementSystem:GlobalVariable,\n"
                         "    Var1;                     !- Erl Variable 1 Name\n"
                         "\n"
                         "  EnergyManagementSystem:GlobalVariable,\n"
                         "    Var2;                     !- Erl Variable 1 Name\n"
                         "\n"
                         "  EnergyManagementSystem:Subroutine,\n"
                         "    DummySubRoutine,\n"
                         "    Set Var2 = 10,\n"
                         "    SET Var2 =Var2*10^4;\n"
                         "\n"
                         "  EnergyManagementSystem:Program,\n"
                         "    DummyProg,                 !- Name\n"
                         "    IF (Hour >= 6) &&(Hour <22),\n"
                         "      SET  Var1= 1 *0.1* Hour,\n"
                         "    ELSE,\n"
                         "        SET Var1 = 2-0.1*Hour,\n"
                         "    ENDIF,\n"
                         "    RUN DummySubRoutine,\n"
                         "    IF (Var1) >=2,\n"
                         "        SeT Var2 = Var2,\n"
                         "        Set Var2 =Var2* 2,\n"
                         "    ELSE,\n"
                         "        set Var2 = Var1,\n"
                         "    ENDIF;\n");

  std::istringstream is(emsSnippet);
  OptionalIdfFile idfFile = IdfFile::load(is, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);

  EXPECT_EQ(2u, model.getConcreteModelObjects<EnergyManagementSystemGlobalVariable>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<EnergyManagementSystemProgramCallingManager>().size());

  /**
   * Test the EMS Program
   * */
  std::vector<EnergyManagementSystemProgram> emsProgs = model.getConcreteModelObjects<EnergyManagementSystemProgram>();
  ASSERT_EQ(1u, emsProgs.size());
  EnergyManagementSystemProgram emsProg = emsProgs[0];
  EXPECT_EQ(12u, emsProg.lines().size());
  std::vector<ModelObject> refObjs = emsProg.referencedObjects();
  // There should be 10 references in total, 4 for Var1 and 5 for Var2, 1 for DummySubRoutine, so only two unique refs (Var1 and Var2)
  EXPECT_EQ(10u, refObjs.size());

  int n1(0);
  int n2(0);
  int n3(0);
  int n0(0);

  for (const ModelObject& refObj : refObjs) {
    std::string oname = refObj.name().get();
    if (oname == "Var1") {
      ++n1;
    } else if (oname == "Var2") {
      ++n2;
    } else if (oname == "DummySubRoutine") {
      ++n3;
    } else {
      ++n0;
    }
  }
  // 4 Var1
  EXPECT_EQ(4, n1);
  // 6 Var2
  EXPECT_EQ(5, n2);
  // 1 DummySubRoutine
  EXPECT_EQ(1, n3);
  // Nothing Else
  EXPECT_EQ(0, n0);

  /**
   * Test the subroutine
   * */
  std::vector<EnergyManagementSystemSubroutine> emsSubs = model.getConcreteModelObjects<EnergyManagementSystemSubroutine>();
  ASSERT_EQ(1u, emsSubs.size());
  EnergyManagementSystemSubroutine emsSub = emsSubs[0];
  EXPECT_EQ(2u, emsSub.lines().size());
  refObjs = emsSub.referencedObjects();
  // There should be 3 references in total, 3 Var2
  EXPECT_EQ(3u, refObjs.size());

  n2 = 0;
  n0 = 0;

  for (const ModelObject& refObj : refObjs) {
    std::string oname = refObj.name().get();
    if (oname == "Var2") {
      ++n2;
    } else {
      ++n0;
    }
  }
  // 3 Var2
  EXPECT_EQ(3, n2);
  // Nothing Else
  EXPECT_EQ(0, n0);

  // Save for Debug
  // inWorkspace.save(toPath("./EMS_WeirdFormat.idf"), true);
  // model.save(toPath("./EMS_WeirdFormat.osm"), true);
}
