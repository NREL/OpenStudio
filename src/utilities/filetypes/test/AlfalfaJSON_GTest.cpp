#include <gtest/gtest.h>
#include <vector>

#include "../AlfalfaJSON.hpp"
#include "utilities/alfalfa/AlfalfaPoint.hpp"
#include "model/Model.hpp"
#include "model/OutputMeter.hpp"
#include "utilities/data/DataEnums.hpp"
#include "utilities/idf/IdfObject.hpp"

#include <json/json.h>

using namespace openstudio;
using namespace openstudio::alfalfa;
using namespace openstudio::model;

TEST(AlfalfaJSON, basic_export) {
  AlfalfaJSON alfalfa;
  AlfalfaPoint constant = alfalfa.exposeConstant(17, "hello").get();

  // Check that point has output of type "Constant"
  ASSERT_TRUE(constant.getOutput().is_initialized());
  ASSERT_EQ(constant.getOutput().get().type, "Constant");
  // Check that point does not have an input
  ASSERT_FALSE(constant.getInput().is_initialized());

  AlfalfaPoint meter = alfalfa.exposeMeter("Electricity:Facility").get();

  // Check that point has output of type "Meter"
  ASSERT_TRUE(meter.getOutput().is_initialized());
  ASSERT_EQ(meter.getOutput().get().type, "Meter");
  // Check that point does not have an input
  ASSERT_FALSE(meter.getInput().is_initialized());

  AlfalfaPoint actuator = alfalfa.exposeActuator("thing1", "thing2", "thing3").get();

  // Check that point has output of type "Actuator"
  ASSERT_TRUE(actuator.getOutput().is_initialized());
  ASSERT_EQ(actuator.getOutput().get().type, "Actuator");
  // Check that point has input of type "Actuator"
  ASSERT_TRUE(actuator.getInput().is_initialized());
  ASSERT_EQ(actuator.getInput().get().type, "Actuator");

  AlfalfaPoint output_variable = alfalfa.exposeOutputVariable("key", "name").get();

  // Check that point has output of type "OutputVariable"
  ASSERT_TRUE(output_variable.getOutput().is_initialized());
  ASSERT_EQ(output_variable.getOutput().get().type, "OutputVariable");
  // Check that point does not have an input
  ASSERT_FALSE(output_variable.getInput().is_initialized());

  AlfalfaPoint global_variable = alfalfa.exposeGlobalVariable("name").get();

  // Check that point has output of type "GlobalVariable"
  ASSERT_TRUE(global_variable.getOutput().is_initialized());
  ASSERT_EQ(global_variable.getOutput().get().type, "GlobalVariable");
  // Check that point has input of type "GlobalVariable"
  ASSERT_TRUE(global_variable.getInput().is_initialized());
  ASSERT_EQ(global_variable.getInput().get().type, "GlobalVariable");

  std::vector<AlfalfaPoint> points = alfalfa.getPoints();

  ASSERT_EQ(points.size(), 5);

  constant.setUnits("mW");

  ASSERT_EQ("mW", points.at(0).getUnits().get());

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
  root_json["id"] = "";

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

TEST(AlfalfaJSON, expose_meter) {
  AlfalfaJSON alfalfa;
  Model model;

  OutputMeter meter_obj_osm(model);
  meter_obj_osm.setFuelType(FuelType::Electricity);
  meter_obj_osm.setInstallLocationType(InstallLocationType::Facility);

  IdfObject meter_obj_idf = IdfObject::load("Output:Meter,Electricity:Facility;").get();

  AlfalfaPoint meter_osm = alfalfa.exposeMeter(meter_obj_osm).get();
  AlfalfaPoint meter_str = alfalfa.exposeMeter("Electricity:Facility").get();
  AlfalfaPoint meter_idf = alfalfa.exposeMeter(meter_obj_idf).get();

  ASSERT_EQ(meter_osm.toJSON(), meter_str.toJSON());
  ASSERT_EQ(meter_osm.toJSON(), meter_idf.toJSON());

  meter_obj_osm.setDisplayName("Facility Meter");

  meter_osm = alfalfa.exposeMeter(meter_obj_osm).get();
  meter_str = alfalfa.exposeMeter("Electricity:Facility", "Facility Meter").get();
  ASSERT_EQ(meter_osm.toJSON(), meter_str.toJSON());
  ASSERT_EQ(meter_osm.toJSON(), alfalfa.exposePoint(meter_obj_osm).get());
}
