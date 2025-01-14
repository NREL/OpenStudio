/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <fmt/core.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

#include "../AlfalfaJSON.hpp"
#include "../AlfalfaPoint.hpp"
#include "../AlfalfaConstant.hpp"
#include "../AlfalfaActuator.hpp"
#include "../AlfalfaGlobalVariable.hpp"
#include "../AlfalfaMeter.hpp"
#include "../AlfalfaOutputVariable.hpp"
#include "../../utilities/core/StringStreamLogSink.hpp"
#include "../../model/Model.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/EnergyManagementSystemOutputVariable.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../utilities/data/DataEnums.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idd/IddEnums.hpp"

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
#include <fmt/format.h>

using namespace openstudio;
using namespace openstudio::alfalfa;
using namespace openstudio::model;

TEST(AlfalfaJSON, basic_export) {
  AlfalfaJSON alfalfa;
  AlfalfaPoint constant = alfalfa.exposeConstant(17, "hello").get();

  // Check that point has output of type "Constant"
  ASSERT_TRUE(constant.output().is_initialized());
  EXPECT_EQ(constant.output().get().typeName(), "Constant");
  // Check that point does not have an input
  EXPECT_FALSE(constant.input().is_initialized());

  const AlfalfaPoint meter = alfalfa.exposeMeter("Electricity:Facility").get();
  std::cout << "here" << std::endl;

  // Check that point has output of type "Meter"
  ASSERT_TRUE(meter.output().is_initialized());
  EXPECT_EQ(meter.output().get().typeName(), "Meter");
  // Check that point does not have an input
  EXPECT_FALSE(meter.input().is_initialized());

  const AlfalfaPoint actuator = alfalfa.exposeActuator("thing1", "thing2", "thing3").get();

  // Check that point has output of type "Actuator"
  ASSERT_TRUE(actuator.output().is_initialized());
  EXPECT_EQ(actuator.output().get().typeName(), "Actuator");
  // Check that point has input of type "Actuator"
  ASSERT_TRUE(actuator.input().is_initialized());
  EXPECT_EQ(actuator.input().get().typeName(), "Actuator");

  const AlfalfaPoint output_variable = alfalfa.exposeOutputVariable("key", "name").get();

  // Check that point has output of type "OutputVariable"
  ASSERT_TRUE(output_variable.output().is_initialized());
  EXPECT_EQ(output_variable.output().get().typeName(), "OutputVariable");
  // Check that point does not have an input
  EXPECT_FALSE(output_variable.input().is_initialized());

  const AlfalfaPoint global_variable = alfalfa.exposeGlobalVariable("name").get();

  // Check that point has output of type "GlobalVariable"
  ASSERT_TRUE(global_variable.output().is_initialized());
  EXPECT_EQ(global_variable.output().get().typeName(), "GlobalVariable");
  // Check that point has input of type "GlobalVariable"
  ASSERT_TRUE(global_variable.input().is_initialized());
  EXPECT_EQ(global_variable.input().get().typeName(), "GlobalVariable");

  std::vector<AlfalfaPoint> points = alfalfa.points();

  ASSERT_EQ(points.size(), 5);

  constant.setUnits("mW");

  EXPECT_EQ("mW", points.at(0).units().get());

  EXPECT_EQ(constant, points.at(0));
  EXPECT_EQ(meter, points.at(1));
  EXPECT_EQ(actuator, points.at(2));
  EXPECT_EQ(output_variable, points.at(3));
  EXPECT_EQ(global_variable, points.at(4));
}

TEST(AlfalfaJSON, json_serialization) {
  AlfalfaJSON alfalfa;

  const std::string point_name = "example_name";

  Json::Value root_json;
  root_json["name"] = point_name;
  root_json["id"] = point_name;
  root_json["optional"] = true;

  // Test generation of JSON for constant components
  const double value = 10.0;
  const AlfalfaPoint constant = alfalfa.exposeConstant(value, point_name + "_constant").get();

  Json::Value constant_json = root_json;
  constant_json["name"] = point_name + "_constant";
  constant_json["id"] = constant_json["name"];
  constant_json["output"]["type"] = "Constant";
  constant_json["output"]["parameters"]["value"] = value;

  EXPECT_EQ(constant.toJSON(), constant_json);

  // Test generation of JSON for actuator components
  const std::string act_component_name = "example_component";
  const std::string act_component_type = "example_component_type";
  const std::string act_control_type = "example_control_type";

  const AlfalfaPoint actuator = alfalfa.exposeActuator(act_component_name, act_component_type, act_control_type, point_name + "_actuator").get();

  Json::Value actuator_json = root_json;
  actuator_json["name"] = point_name + "_actuator";
  actuator_json["id"] = actuator_json["name"];
  actuator_json["output"]["type"] = "Actuator";
  actuator_json["output"]["parameters"]["component_name"] = act_component_name;
  actuator_json["output"]["parameters"]["component_type"] = act_component_type;
  actuator_json["output"]["parameters"]["control_type"] = act_control_type;

  actuator_json["input"] = actuator_json["output"];

  EXPECT_EQ(actuator.toJSON(), actuator_json);

  // Test generation of JSON for global variable components
  const std::string global_variable_name = "example_global_variable_name";

  const AlfalfaPoint global_variable = alfalfa.exposeGlobalVariable(global_variable_name, point_name + "_global_variable").get();

  Json::Value global_variable_json = root_json;
  global_variable_json["name"] = point_name + "_global_variable";
  global_variable_json["id"] = global_variable_json["name"];
  global_variable_json["output"]["type"] = "GlobalVariable";
  global_variable_json["output"]["parameters"]["variable_name"] = global_variable_name;

  global_variable_json["input"] = global_variable_json["output"];

  EXPECT_EQ(global_variable.toJSON(), global_variable_json);

  // Test generation of JSON for meter components
  const std::string meter_name = "example_meter_name";

  const AlfalfaPoint meter = alfalfa.exposeMeter(meter_name, point_name + "_meter").get();

  Json::Value meter_json = root_json;
  meter_json["name"] = point_name + "_meter";
  meter_json["id"] = meter_json["name"];
  meter_json["output"]["type"] = "Meter";
  meter_json["output"]["parameters"]["meter_name"] = meter_name;

  EXPECT_EQ(meter.toJSON(), meter_json);

  // Test generation of JSON for output variable components
  const std::string output_variable_key = "example_output_variable_key";
  const std::string output_variable_name = "example_output_variable_name";

  const AlfalfaPoint output_variable = alfalfa.exposeOutputVariable(output_variable_key, output_variable_name, point_name + "_output_variable").get();

  Json::Value output_variable_json = root_json;
  output_variable_json["name"] = point_name + "_output_variable";
  output_variable_json["id"] = output_variable_json["name"];
  output_variable_json["output"]["type"] = "OutputVariable";
  output_variable_json["output"]["parameters"]["variable_key"] = output_variable_key;
  output_variable_json["output"]["parameters"]["variable_name"] = output_variable_name;

  EXPECT_EQ(output_variable.toJSON(), output_variable_json);

  // Test that the file written is the same as the one inside the AlfalfaJSON object
  const path out_file = tempDir() / "alfalfa.json";
  alfalfa.saveAs(out_file);

  Json::Value root;
  std::ifstream ifs(openstudio::toSystemFilename(out_file));

  const Json::CharReaderBuilder r_builder;
  std::string formatted_errors;
  const bool parsing_success = Json::parseFromStream(r_builder, ifs, &root, &formatted_errors);
  EXPECT_TRUE(parsing_success);
  EXPECT_EQ(alfalfa.toJSON(), root);
  for (Json::ArrayIndex i = 0; const auto& point : alfalfa.points()) {
    EXPECT_EQ(root[i++], point.toJSON());
  }
}

TEST(AlfalfaJSON, point_exceptions_logging) {
  const std::string DISPLAY_NAME_EMPTY_ERROR = "Display name must have non-zero length";
  const std::string ID_EMPTY_ERROR = "Id must have non-zero length";
  const std::string LOG_CHANNEL = "openstudio.AlfalfaPoint";
  StringStreamLogSink ss;
  ss.setLogLevel(Warn);

  AlfalfaPoint point("Point");
  ASSERT_EQ(0, ss.logMessages().size());
  point.id();
  ASSERT_EQ(0, ss.logMessages().size());

  //Test logging in constructor
  EXPECT_THROW(
    {
      try {
        const AlfalfaPoint invalid_point("");
      } catch (const std::runtime_error& error) {
        EXPECT_EQ(error.what(), DISPLAY_NAME_EMPTY_ERROR);
        throw;
      }
    },
    std::runtime_error);

  // Test exception handling in setId()
  EXPECT_THROW(
    {
      try {
        point.setId("");
      } catch (const std::runtime_error& error) {
        EXPECT_EQ(error.what(), ID_EMPTY_ERROR);
        throw;
      }
    },
    std::runtime_error);
  ASSERT_EQ(ss.logMessages().size(), 0);

  //Test Calls work when provided with legal input
  AlfalfaPoint valid_point("A Good Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that getting the ID does not warn if valid.
  boost::optional<std::string> valid_id = valid_point.id();
  ASSERT_TRUE(valid_id.is_initialized());
  ASSERT_EQ(valid_id.get(), "A_Good_Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that changing display name changes the ID if an ID has not been set.
  valid_point.setDisplayName("Another Good Point");
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_EQ(valid_point.id(), "Another_Good_Point");
  ASSERT_EQ(ss.logMessages().size(), 0);

  Json::Value valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that setting a valid ID is sticky
  const std::string new_id = "Another_Valid_Point(123)";
  valid_point.setId(new_id);
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_EQ(valid_point.id(), new_id);

  valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);

  // Test that once an ID is set, setting a new display name won't throw a warning.
  valid_point.setDisplayName("Valid Name, but Invalid Id $$$");
  ASSERT_EQ(ss.logMessages().size(), 0);
  ASSERT_EQ(valid_point.id(), new_id);

  valid_json = valid_point.toJSON();
  ASSERT_EQ(ss.logMessages().size(), 0);
}

class InputComponent : public AlfalfaComponentBase
{
 public:
  InputComponent() = default;
  AlfalfaComponentCapability capability() const override {
    return AlfalfaComponentCapability::Input;
  }

  AlfalfaComponentType type() const override {
    return AlfalfaComponentType::Constant;
  }

  std::unique_ptr<AlfalfaComponentBase> clone() const override {
    return std::make_unique<InputComponent>(*this);
  }

  std::string deriveName() const override {
    return "";
  }

  Json::Value toJSON() const override {
    Json::Value parameters;
    return parameters;
  }
};

TEST(AlfalfaJSON, point_component_exceptions) {
  AlfalfaPoint point("Point Name");

  const AlfalfaConstant constant(12.5);
  // No Components can not be used as outputs so I create a fictional component to test that guard
  const InputComponent input_only;

  EXPECT_FALSE(point.input().is_initialized());
  EXPECT_FALSE(point.output().is_initialized());

  EXPECT_THROW(
    {
      try {
        point.setInput(constant);
      } catch (std::runtime_error& error) {
        ASSERT_EQ(error.what(), fmt::format("Component of type: {} cannot be used as an input.", constant.typeName()));
        throw;
      }
    },
    std::runtime_error);

  EXPECT_THROW(
    {
      try {
        point.setOutput(input_only);
      } catch (std::runtime_error& error) {
        ASSERT_EQ(error.what(), fmt::format("Component of type: {} cannot be used as an output.", input_only.typeName()));
        throw;
      }
    },
    std::runtime_error);

  // Confirm that point components are still empty
  EXPECT_FALSE(point.input().is_initialized());
  EXPECT_FALSE(point.output().is_initialized());

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
  const Model model;

  const std::string display_name = "Electricity Facility Meter";
  const std::string meter_name = "Electricity:Facility";

  OutputMeter os_meter(model);
  IdfObject idf_meter(IddObjectType::Output_Meter);

  // Test error handling for malformed meter objects
  EXPECT_THROW({ new AlfalfaMeter(os_meter); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(os_meter).is_initialized());

  EXPECT_THROW({ new AlfalfaMeter(idf_meter); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(idf_meter).is_initialized());

  // Populate meters with values
  os_meter.setFuelType(FuelType::Electricity);
  os_meter.setInstallLocationType(InstallLocationType::Facility);
  os_meter.setDisplayName(display_name);

  idf_meter.setString(Output_MeterFields::KeyName, meter_name);

  // All Components should be the same
  const AlfalfaMeter str_component(meter_name);
  const AlfalfaMeter os_component(os_meter);
  const AlfalfaMeter idf_component(idf_meter);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All points should be the same

  const AlfalfaPoint str_point = alfalfa.exposeMeter(meter_name, display_name).get();
  const AlfalfaPoint os_point = alfalfa.exposeFromObject(os_meter).get();
  const AlfalfaPoint idf_point = alfalfa.exposeFromObject(idf_meter, display_name).get();

  EXPECT_EQ(os_point, str_point);
  EXPECT_EQ(os_point, idf_point);

  EXPECT_FALSE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);

  // Test Other Point Construction Methods
  const AlfalfaPoint component_point = alfalfa.exposeFromComponent(str_component, display_name).get();
  AlfalfaPoint custom_point(display_name);
  EXPECT_THROW({ custom_point.setInput(str_component); }, std::runtime_error);
  custom_point.setOutput(str_component);

  EXPECT_EQ(str_point, custom_point);
  EXPECT_EQ(str_point, component_point);
}

TEST(AlfalfaJSON, expose_output_variable) {
  AlfalfaJSON alfalfa;
  const Model model;

  const std::string variable_name = "my_var";
  const std::string variable_key = "EMS";
  const std::string display_name = "My Output Variable";

  OutputVariable os_output_variable(variable_name, model);
  EnergyManagementSystemOutputVariable os_ems_output_variable(model, "");
  IdfObject idf_output_variable = IdfObject(IddObjectType::Output_Variable);
  IdfObject idf_ems_output_variable = IdfObject(IddObjectType::EnergyManagementSystem_OutputVariable);

  // Test error handling for malformed output variable objects
  EXPECT_THROW({ new AlfalfaOutputVariable(os_output_variable); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(os_output_variable).is_initialized());

  EXPECT_THROW({ new AlfalfaOutputVariable(os_ems_output_variable); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(os_ems_output_variable).is_initialized());

  EXPECT_THROW({ new AlfalfaOutputVariable(idf_output_variable); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(idf_output_variable).is_initialized());

  EXPECT_THROW({ new AlfalfaOutputVariable(idf_ems_output_variable); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(idf_ems_output_variable).is_initialized());

  os_ems_output_variable = EnergyManagementSystemOutputVariable(model, variable_name);

  os_output_variable.setKeyValue(variable_key);
  os_output_variable.setDisplayName(display_name);

  os_ems_output_variable.setDisplayName(display_name);

  idf_output_variable.setString(Output_VariableFields::KeyValue, variable_key);
  idf_output_variable.setString(Output_VariableFields::VariableName, variable_name);

  idf_ems_output_variable.setString(EnergyManagementSystem_OutputVariableFields::EMSVariableName, variable_name);
  idf_ems_output_variable.setString(EnergyManagementSystem_OutputVariableFields::Name, display_name);

  // All components created should be equal
  const AlfalfaOutputVariable str_component(variable_key, variable_name);
  const AlfalfaOutputVariable os_component(os_output_variable);
  const AlfalfaOutputVariable os_ems_component(os_ems_output_variable);
  const AlfalfaOutputVariable idf_component(idf_output_variable);
  const AlfalfaOutputVariable idf_ems_component(idf_ems_output_variable);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, os_ems_component);
  EXPECT_EQ(str_component, idf_component);
  EXPECT_EQ(str_component, idf_ems_component);

  // All points created should be the same
  os_output_variable.setDisplayName(display_name);

  const AlfalfaPoint str_point = alfalfa.exposeOutputVariable(variable_key, variable_name, display_name).get();
  const AlfalfaPoint os_point = alfalfa.exposeFromObject(os_output_variable).get();
  const AlfalfaPoint os_ems_point = alfalfa.exposeFromObject(os_ems_output_variable).get();
  const AlfalfaPoint idf_point = alfalfa.exposeFromObject(idf_output_variable, display_name).get();
  const AlfalfaPoint idf_ems_point = alfalfa.exposeFromObject(idf_ems_output_variable).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point, os_ems_point);
  EXPECT_EQ(str_point, idf_point);
  EXPECT_EQ(str_point, idf_ems_point);

  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_FALSE(str_point.input().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);

  // Test Other Point Construction Methods
  const AlfalfaPoint component_point = alfalfa.exposeFromComponent(str_component, display_name).get();
  AlfalfaPoint custom_point(display_name);
  EXPECT_THROW({ custom_point.setInput(str_component); }, std::runtime_error);
  custom_point.setOutput(str_component);

  EXPECT_EQ(str_point, custom_point);
  EXPECT_EQ(str_point, component_point);
}

TEST(AlfalfaJSON, expose_global_variable) {
  AlfalfaJSON alfalfa;
  const Model model;

  const std::string variable_name = "Variable_Name";
  const std::string display_name = "Variable Name";

  EnergyManagementSystemGlobalVariable os_global_variable(model, variable_name);
  IdfObject idf_global_variable(IddObjectType::EnergyManagementSystem_GlobalVariable);

  // Test error handling for malformed global variable objects

  EXPECT_THROW({ new AlfalfaGlobalVariable(idf_global_variable); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(idf_global_variable).is_initialized());

  os_global_variable.setDisplayName(display_name);

  idf_global_variable.setString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, variable_name);

  // All Components created should be equal
  const AlfalfaGlobalVariable str_component(variable_name);
  const AlfalfaGlobalVariable os_component(os_global_variable);
  const AlfalfaGlobalVariable idf_component(idf_global_variable);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All points created should be equal
  const AlfalfaPoint str_point = alfalfa.exposeGlobalVariable(variable_name, display_name).get();
  const AlfalfaPoint os_point = alfalfa.exposeFromObject(os_global_variable).get();
  const AlfalfaPoint idf_point = alfalfa.exposeFromObject(idf_global_variable, display_name).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point.toJSON(), idf_point.toJSON());

  EXPECT_TRUE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);

  // Test Other Point Construction Methods
  const AlfalfaPoint component_point = alfalfa.exposeFromComponent(str_component, display_name).get();
  AlfalfaPoint custom_point(display_name);
  custom_point.setInput(str_component);
  custom_point.setOutput(str_component);

  EXPECT_EQ(str_point, custom_point);
  EXPECT_EQ(str_point, component_point);
}

TEST(AlfalfaJSON, expose_actuator) {
  AlfalfaJSON alfalfa;
  const Model model;

  const std::string component_name = "component_name";
  const std::string component_type = "component_type";
  const std::string control_type = "control_type";
  const std::string display_name = "My Actuator";

  ThermalZone actuated_zone(model);
  EnergyManagementSystemActuator os_actuator(actuated_zone, "", "");
  IdfObject idf_actuator(IddObjectType::EnergyManagementSystem_Actuator);

  // Test error handling for malformed actuator objects
  EXPECT_THROW({ new AlfalfaActuator(os_actuator); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(os_actuator).is_initialized());

  EXPECT_THROW({ new AlfalfaActuator(idf_actuator); }, std::runtime_error);
  EXPECT_FALSE(alfalfa.exposeFromObject(idf_actuator).is_initialized());

  actuated_zone.setName(component_name);
  os_actuator = EnergyManagementSystemActuator(actuated_zone, component_type, control_type);
  os_actuator.setDisplayName(display_name);

  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, component_name);
  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, component_type);
  idf_actuator.setString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, control_type);

  // All components should be the same
  const AlfalfaActuator str_component(component_name, component_type, control_type);
  const AlfalfaActuator os_component(os_actuator);
  const AlfalfaActuator idf_component(idf_actuator);

  EXPECT_EQ(str_component, os_component);
  EXPECT_EQ(str_component, idf_component);

  // All point should be the same
  const AlfalfaPoint str_point = alfalfa.exposeActuator(component_name, component_type, control_type, display_name).get();
  const AlfalfaPoint os_point = alfalfa.exposeFromObject(os_actuator).get();
  const AlfalfaPoint idf_point = alfalfa.exposeFromObject(idf_actuator, display_name).get();

  EXPECT_EQ(str_point, os_point);
  EXPECT_EQ(str_point, idf_point);

  EXPECT_TRUE(str_point.input().is_initialized());
  EXPECT_TRUE(str_point.output().is_initialized());
  EXPECT_EQ(str_point.displayName(), display_name);

  // Test Other Point Construction Methods
  const AlfalfaPoint component_point = alfalfa.exposeFromComponent(str_component, display_name).get();
  AlfalfaPoint custom_point(display_name);
  custom_point.setInput(str_component);
  custom_point.setOutput(str_component);

  EXPECT_EQ(str_point, custom_point);
  EXPECT_EQ(str_point, component_point);
}
