#include <gtest/gtest.h>
#include <vector>

#include "../AlfalfaJSON.hpp"
#include "utilities/alfalfa/AlfalfaPoint.hpp"
#include "utilities/alfalfa/AlfalfaConstant.hpp"
#include "utilities/alfalfa/AlfalfaActuator.hpp"
#include "utilities/alfalfa/AlfalfaGlobalVariable.hpp"
#include "utilities/alfalfa/AlfalfaMeter.hpp"
#include "utilities/alfalfa/AlfalfaOutputVariable.hpp"
#include "utilities/core/StringStreamLogSink.hpp"
#include "model/Model.hpp"
#include "model/OutputMeter.hpp"
#include "model/OutputVariable.hpp"
#include "model/EnergyManagementSystemOutputVariable.hpp"
#include "model/ThermalZone.hpp"
#include "utilities/data/DataEnums.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <json/json.h>

using namespace openstudio;
using namespace openstudio::alfalfa;
using namespace openstudio::model;

TEST(AlfalfaJSON, basic_export) {
  AlfalfaJSON alfalfa;
  AlfalfaPoint constant = alfalfa.exposeConstant(17, "hello").get();

  // Check that point has output of type "Constant"
  ASSERT_TRUE(constant.output().is_initialized());
  ASSERT_EQ(constant.output().get().type, "Constant");
  // Check that point does not have an input
  ASSERT_FALSE(constant.input().is_initialized());

  AlfalfaPoint meter = alfalfa.exposeMeter("Electricity:Facility").get();

  // Check that point has output of type "Meter"
  ASSERT_TRUE(meter.output().is_initialized());
  ASSERT_EQ(meter.output().get().type, "Meter");
  // Check that point does not have an input
  ASSERT_FALSE(meter.input().is_initialized());

  AlfalfaPoint actuator = alfalfa.exposeActuator("thing1", "thing2", "thing3").get();

  // Check that point has output of type "Actuator"
  ASSERT_TRUE(actuator.output().is_initialized());
  ASSERT_EQ(actuator.output().get().type, "Actuator");
  // Check that point has input of type "Actuator"
  ASSERT_TRUE(actuator.input().is_initialized());
  ASSERT_EQ(actuator.input().get().type, "Actuator");

  AlfalfaPoint output_variable = alfalfa.exposeOutputVariable("key", "name").get();

  // Check that point has output of type "OutputVariable"
  ASSERT_TRUE(output_variable.output().is_initialized());
  ASSERT_EQ(output_variable.output().get().type, "OutputVariable");
  // Check that point does not have an input
  ASSERT_FALSE(output_variable.input().is_initialized());

  AlfalfaPoint global_variable = alfalfa.exposeGlobalVariable("name").get();

  // Check that point has output of type "GlobalVariable"
  ASSERT_TRUE(global_variable.output().is_initialized());
  ASSERT_EQ(global_variable.output().get().type, "GlobalVariable");
  // Check that point has input of type "GlobalVariable"
  ASSERT_TRUE(global_variable.input().is_initialized());
  ASSERT_EQ(global_variable.input().get().type, "GlobalVariable");

  std::vector<AlfalfaPoint> points = alfalfa.getPoints();

  ASSERT_EQ(points.size(), 5);

  constant.setUnits("mW");

  ASSERT_EQ("mW", points.at(0).units().get());

  ASSERT_EQ(constant, points.at(0));
  ASSERT_EQ(meter, points.at(1));
  ASSERT_EQ(actuator, points.at(2));
  ASSERT_EQ(output_variable, points.at(3));
  ASSERT_EQ(global_variable, points.at(4));
}

TEST(AlfalfaJSON, json_serialization) {
  AlfalfaJSON alfalfa;

  std::string point_name = "example_name";

  Json::Value root_json;
  root_json["name"] = point_name;
  root_json["id"] = point_name;
  root_json["optional"] = true;

  // Test generation of JSON for constant components
  float value = 10.0;
  AlfalfaPoint constant = alfalfa.exposeConstant(value, "example_name").get();

  Json::Value constant_json = root_json;
  constant_json["output"]["type"] = "Constant";
  constant_json["output"]["parameters"]["value"] = value;

  ASSERT_EQ(constant.toJSON(), constant_json);

  // Test generation of JSON for actuator components
  std::string act_component_name = "example_component";
  std::string act_component_type = "example_component_type";
  std::string act_control_type = "example_control_type";

  AlfalfaPoint actuator = alfalfa.exposeActuator(act_component_name, act_component_type, act_control_type, point_name).get();

  Json::Value actuator_json = root_json;
  actuator_json["output"]["type"] = "Actuator";
  actuator_json["output"]["parameters"]["component_name"] = act_component_name;
  actuator_json["output"]["parameters"]["component_type"] = act_component_type;
  actuator_json["output"]["parameters"]["control_type"] = act_control_type;

  actuator_json["input"] = actuator_json["output"];

  ASSERT_EQ(actuator.toJSON(), actuator_json);

  // Test generation of JSON for global variable components
  std::string global_variable_name = "example_global_variable_name";

  AlfalfaPoint global_variable = alfalfa.exposeGlobalVariable(global_variable_name, point_name).get();

  Json::Value global_variable_json = root_json;
  global_variable_json["output"]["type"] = "GlobalVariable";
  global_variable_json["output"]["parameters"]["variable_name"] = global_variable_name;

  global_variable_json["input"] = global_variable_json["output"];

  ASSERT_EQ(global_variable.toJSON(), global_variable_json);

  // Test generation of JSON for meter components
  std::string meter_name = "example_meter_name";

  AlfalfaPoint meter = alfalfa.exposeMeter(meter_name, point_name).get();

  Json::Value meter_json = root_json;
  meter_json["output"]["type"] = "Meter";
  meter_json["output"]["parameters"]["meter_name"] = meter_name;

  ASSERT_EQ(meter.toJSON(), meter_json);

  // Test generation of JSON for output variable components
  std::string output_variable_key = "example_output_variable_key";
  std::string output_variable_name = "example_output_variable_name";

  AlfalfaPoint output_variable = alfalfa.exposeOutputVariable(output_variable_key, output_variable_name, point_name).get();

  Json::Value output_variable_json = root_json;
  output_variable_json["output"]["type"] = "OutputVariable";
  output_variable_json["output"]["parameters"]["variable_key"] = output_variable_key;
  output_variable_json["output"]["parameters"]["variable_name"] = output_variable_name;

  ASSERT_EQ(output_variable.toJSON(), output_variable_json);
}

TEST(AlfalfaJSON, point_exceptions_logging) {
  const char* ID_VALID_CHARS_MSG = "IDs can only contain letters, numbers, and the following special characters _-[]():";
  const char* DISPLAY_NAME_VALID_CHARS_MSG = "Display name does not produce a valid point ID. Manually set a valid ID or export will fail.";
  const char* LOG_CHANNEL = "openstudio.AlfalfaPoint";
  StringStreamLogSink ss;
  ss.setLogLevel(Warn);

  AlfalfaPoint point("Point");
  ASSERT_EQ(0, ss.logMessages().size());
  point.id();
  ASSERT_EQ(0, ss.logMessages().size());

  //Test logging in constructor
  AlfalfaPoint invalid_point("Point$$$");
  ASSERT_EQ(1, ss.logMessages().size());
  LogMessage invalid_id_msg = ss.logMessages().at(0);
  EXPECT_STREQ(invalid_id_msg.logMessage().c_str(), DISPLAY_NAME_VALID_CHARS_MSG);
  EXPECT_EQ(invalid_id_msg.logLevel(), Warn);
  EXPECT_STREQ(invalid_id_msg.logChannel().c_str(), LOG_CHANNEL);
  ss.resetStringStream();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test logging when getting id()
  boost::optional<std::string> invalid_point_id = invalid_point.id();
  EXPECT_FALSE(invalid_point_id.is_initialized());
  ASSERT_EQ(ss.logMessages().size(), 1);
  invalid_id_msg = ss.logMessages().at(0);
  EXPECT_STREQ(invalid_id_msg.logMessage().c_str(), DISPLAY_NAME_VALID_CHARS_MSG);
  EXPECT_EQ(invalid_id_msg.logLevel(), Warn);
  EXPECT_STREQ(invalid_id_msg.logChannel().c_str(), LOG_CHANNEL);
  ss.resetStringStream();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test exception handling in setId()
  EXPECT_THROW(
    {
      try {
        invalid_point.setId("Point_123_$$$");
      } catch (const std::runtime_error& error) {
        EXPECT_STREQ(error.what(), ID_VALID_CHARS_MSG);
        throw;
      }
    },
    std::runtime_error);
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test exception handling in toJSON()
  EXPECT_THROW(
    {
      try {
        Json::Value root = invalid_point.toJSON();
      } catch (const std::runtime_error& error) {
        char message[200];
        sprintf(message, "Point requires a valid ID for export. %s", ID_VALID_CHARS_MSG);
        EXPECT_STREQ(error.what(), message);
        throw;
      }
    },
    std::runtime_error);
  ASSERT_EQ(ss.logMessages().size(), 1);
  invalid_id_msg = ss.logMessages().at(0);
  EXPECT_STREQ(invalid_id_msg.logMessage().c_str(), DISPLAY_NAME_VALID_CHARS_MSG);
  EXPECT_EQ(invalid_id_msg.logLevel(), Warn);
  EXPECT_STREQ(invalid_id_msg.logChannel().c_str(), LOG_CHANNEL);
  ss.resetStringStream();
  ASSERT_EQ(ss.logMessages().size(), 0);

  //Test Calls work when provided with legal input
  AlfalfaPoint valid_point("A Good Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that getting the ID does not warn if valid.
  boost::optional<std::string> valid_id = valid_point.id();
  ASSERT_TRUE(valid_id.is_initialized());
  ASSERT_STREQ(valid_id.get().c_str(), "A_Good_Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that changing display name changes the ID if an ID has not been set.
  valid_point.setDisplayName("Another Good Point");
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_TRUE(valid_point.id().is_initialized());
  ASSERT_STREQ(valid_point.id().get().c_str(), "Another_Good_Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  Json::Value valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that setting a valid ID is sticky
  std::string new_id = "Another_Valid_Point(123)";
  valid_point.setId(new_id);
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_TRUE(valid_point.id().is_initialized());
  ASSERT_STREQ(valid_point.id().get().c_str(), new_id.c_str());

  valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that once an ID is set, setting a new display name won't throw a warning.
  valid_point.setDisplayName("Valid Name, but Invalid Id $$$");
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_STREQ(valid_point.id().get().c_str(), new_id.c_str());

  valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);
}

TEST(AlfalfaJSON, point_component_exceptions) {
  AlfalfaPoint point("Point Name");

  AlfalfaConstant constant(12.5);
  // No Components can not be used as outputs so I create a fictional component to test that guard
  AlfalfaComponent input_only("InputOnly", Capability::Input);

  ASSERT_FALSE(point.input().is_initialized());
  ASSERT_FALSE(point.output().is_initialized());

  EXPECT_THROW(
    {
      try {
        point.setInput(constant);
      } catch (std::runtime_error error) {
        char expected_error[200];
        sprintf(expected_error, "Component of type: %s cannot be used as an input.", constant.type.c_str());
        ASSERT_STREQ(error.what(), expected_error);
        throw;
      }
    },
    std::runtime_error);

  EXPECT_THROW(
    {
      try {
        point.setOutput(input_only);
      } catch (std::runtime_error error) {
        char expected_error[200];
        sprintf(expected_error, "Component of type: %s cannot be used as an output.", input_only.type.c_str());
        ASSERT_STREQ(error.what(), expected_error);
        throw;
      }
    },
    std::runtime_error);

  // Confirm that point components are still empty
  ASSERT_FALSE(point.input().is_initialized());
  ASSERT_FALSE(point.output().is_initialized());

  // Populate correct IO
  point.setOutput(constant);
  point.setInput(input_only);

  // Confirm population worked correctly
  ASSERT_TRUE(point.input().is_initialized());
  ASSERT_TRUE(point.output().is_initialized());
  EXPECT_EQ(input_only, point.input().get());
  EXPECT_EQ(constant, point.output().get());
}

TEST(AlfalfaJSON, expose_meter) {
  AlfalfaJSON alfalfa;
  Model model;

  std::string display_name = "Electricity Facility Meter";
  std::string meter_name = "Electricity:Facility";

  OutputMeter os_meter(model);
  os_meter.setFuelType(FuelType::Electricity);
  os_meter.setInstallLocationType(InstallLocationType::Facility);
  os_meter.setDisplayName(display_name);

  IdfObject idf_meter(IddObjectType::Output_Meter);
  idf_meter.setString(Output_MeterFields::KeyName, meter_name);

  // All Components should be the same
  AlfalfaMeter str_component(meter_name);
  AlfalfaMeter os_component(os_meter);
  AlfalfaMeter idf_component(idf_meter);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All points should be the same

  AlfalfaPoint str_point = alfalfa.exposeMeter(meter_name, display_name).get();
  AlfalfaPoint os_point = alfalfa.exposePoint(os_meter).get();
  AlfalfaPoint idf_point = alfalfa.exposePoint(idf_meter, display_name).get();

  EXPECT_EQ(os_point, str_point);
  EXPECT_EQ(os_point, idf_point);

  EXPECT_FALSE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);
}

TEST(AlfalfaJSON, expose_output_variable) {
  AlfalfaJSON alfalfa;
  Model model;

  std::string variable_name = "my_var";
  std::string variable_key = "EMS";
  std::string display_name = "My Output Variable";
  OutputVariable os_output_variable(variable_name, model);
  os_output_variable.setKeyValue(variable_key);
  os_output_variable.setDisplayName(display_name);

  EnergyManagementSystemOutputVariable os_ems_output_variable(model, variable_name);
  os_ems_output_variable.setDisplayName(display_name);

  IdfObject idf_output_variable = IdfObject(IddObjectType::Output_Variable);
  idf_output_variable.setString(Output_VariableFields::KeyValue, variable_key);
  idf_output_variable.setString(Output_VariableFields::VariableName, variable_name);

  IdfObject idf_ems_output_variable = IdfObject(IddObjectType::EnergyManagementSystem_OutputVariable);
  idf_ems_output_variable.setString(EnergyManagementSystem_OutputVariableFields::EMSVariableName, variable_name);
  idf_ems_output_variable.setString(EnergyManagementSystem_OutputVariableFields::Name, display_name);

  // All components created should be equal
  AlfalfaOutputVariable str_component(variable_key, variable_name);
  AlfalfaOutputVariable os_component(os_output_variable);
  AlfalfaOutputVariable os_ems_component(os_ems_output_variable);
  AlfalfaOutputVariable idf_component(idf_output_variable);
  AlfalfaOutputVariable idf_ems_component(idf_ems_output_variable);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, os_ems_component);
  EXPECT_EQ(str_component, idf_component);
  EXPECT_EQ(str_component, idf_ems_component);

  // All points created should be the same
  os_output_variable.setDisplayName(display_name);

  AlfalfaPoint str_point = alfalfa.exposeOutputVariable(variable_key, variable_name, display_name).get();
  AlfalfaPoint os_point = alfalfa.exposePoint(os_output_variable).get();
  AlfalfaPoint os_ems_point = alfalfa.exposePoint(os_ems_output_variable).get();
  AlfalfaPoint idf_point = alfalfa.exposePoint(idf_output_variable, display_name).get();
  AlfalfaPoint idf_ems_point = alfalfa.exposePoint(idf_ems_output_variable).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point, os_ems_point);
  EXPECT_EQ(str_point, idf_point);
  EXPECT_EQ(str_point, idf_ems_point);

  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_FALSE(str_point.input().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);
}

TEST(AlfalfaJSON, expose_global_variable) {
  AlfalfaJSON alfalfa;
  Model model;

  std::string variable_name = "Variable_Name";
  std::string display_name = "Variable Name";

  EnergyManagementSystemGlobalVariable os_global_variable(model, variable_name);
  os_global_variable.setDisplayName(display_name);

  IdfObject idf_global_variable(IddObjectType::EnergyManagementSystem_GlobalVariable);
  idf_global_variable.setString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, variable_name);

  // All Components created should be equal
  AlfalfaGlobalVariable str_component(variable_name);
  AlfalfaGlobalVariable os_component(os_global_variable);
  AlfalfaGlobalVariable idf_component(idf_global_variable);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All points created should be equal
  AlfalfaPoint str_point = alfalfa.exposeGlobalVariable(variable_name, display_name).get();
  AlfalfaPoint os_point = alfalfa.exposePoint(os_global_variable).get();
  AlfalfaPoint idf_point = alfalfa.exposePoint(idf_global_variable, display_name).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point.toJSON(), idf_point.toJSON());

  EXPECT_TRUE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);
}

TEST(AlfalfaJSON, expose_actuator) {
  AlfalfaJSON alfalfa;
  Model model;

  std::string component_name = "component_name";
  std::string component_type = "component_type";
  std::string control_type = "control_type";
  std::string display_name = "My Actuator";

  ThermalZone actuated_zone(model);
  actuated_zone.setName(component_name);

  EnergyManagementSystemActuator os_actuator(actuated_zone, component_type, control_type);
  os_actuator.setDisplayName(display_name);

  IdfObject idf_actuator(IddObjectType::EnergyManagementSystem_Actuator);
  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, component_name);
  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, component_type);
  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, control_type);

  // All components should be the same
  AlfalfaActuator str_component(component_name, component_type, control_type);
  AlfalfaActuator os_component(os_actuator);
  AlfalfaActuator idf_component(idf_actuator);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All point should be the same
  AlfalfaPoint str_point = alfalfa.exposeActuator(component_name, component_type, control_type, display_name).get();
  AlfalfaPoint os_point = alfalfa.exposePoint(os_actuator).get();
  AlfalfaPoint idf_point = alfalfa.exposePoint(idf_actuator, display_name).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point, idf_point);

  EXPECT_TRUE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);
}