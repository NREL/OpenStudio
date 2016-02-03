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
#include "RulesetFixture.hpp"

#include "../OSArgument.hpp"
#include "../OSRunner.hpp"
#include "../OSResult.hpp"
#include "../ModelUserScript.hpp"
#include "../UserScript.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Finder.hpp"

#include "../../utilities/units/QuantityConverter.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;

class TestOSRunner : public OSRunner {
 public:

  virtual bool inSelection(const openstudio::model::ModelObject& modelObject) const override {
    return false;
  }

};

// derive some test classes
class TestModelUserScript1 : public ModelUserScript {
 public:

  virtual std::string name() const override {
    return "TestModelUserScript1";
  }

  // remove all spaces and add a new one
  virtual bool run(Model& model,
                   OSRunner& runner,
                   const std::map<std::string, OSArgument>& user_arguments) const override
  {
    ModelUserScript::run(model,runner,user_arguments);

    // no arguments, so do not bother validating them

    std::stringstream ss;

    // remove old spaces
    int count(0);
    for (openstudio::model::Space space : model.getModelObjects<openstudio::model::Space>()) {
      space.remove();
      ++count;
    }
    ss << "Initial model had " << count << " spaces.";
    runner.registerInitialCondition(ss.str()); ss.str("");

    // add a new one
    openstudio::model::Space space(model);

    ss << "Removed the " << count << " original spaces, and added one new one named '" << space.name().get() << "'.";
    runner.registerFinalCondition(ss.str());

    // success
    return true;
  }

};

TEST_F(RulesetFixture, UserScript_TestModelUserScript1) {
  TestModelUserScript1 script;
  EXPECT_EQ("TestModelUserScript1", script.name());

  TestOSRunner runner;
  std::map<std::string, OSArgument> user_arguments;

  // test with empty model
  openstudio::model::Model model1;
  EXPECT_EQ(0u, model1.getModelObjects<openstudio::model::Space>().size());
  EXPECT_EQ(0u, script.arguments(model1).size());
  script.run(model1, runner, user_arguments);
  EXPECT_EQ(1u, model1.getModelObjects<openstudio::model::Space>().size());
  OSResult result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  ASSERT_TRUE(result.initialCondition());
  EXPECT_TRUE(result.initialCondition()->logLevel() == Info);
  EXPECT_EQ("openstudio.ruleset." + script.name(),result.initialCondition()->logChannel());
  EXPECT_EQ("Initial model had 0 spaces.",result.initialCondition()->logMessage());
  ASSERT_TRUE(result.finalCondition());
  EXPECT_TRUE(result.finalCondition()->logLevel() == Info);
  EXPECT_EQ("openstudio.ruleset." + script.name(),result.finalCondition()->logChannel());
  EXPECT_EQ("Removed the 0 original spaces, and added one new one named 'Space 1'.",result.finalCondition()->logMessage());

  // test with populated model
  openstudio::model::Model model2;
  openstudio::model::Space space1(model2);
  openstudio::model::Space space2(model2);
  EXPECT_EQ(2u, model2.getModelObjects<openstudio::model::Space>().size());
  EXPECT_EQ(0u, script.arguments(model2).size());
  script.run(model2, runner, user_arguments);
  EXPECT_EQ(1u, model2.getModelObjects<openstudio::model::Space>().size());
  result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  ASSERT_TRUE(result.initialCondition());
  EXPECT_TRUE(result.initialCondition()->logLevel() == Info);
  EXPECT_EQ("openstudio.ruleset." + script.name(),result.initialCondition()->logChannel());
  EXPECT_EQ("Initial model had 2 spaces.",result.initialCondition()->logMessage());
  ASSERT_TRUE(result.finalCondition());
  EXPECT_TRUE(result.finalCondition()->logLevel() == Info);
  EXPECT_EQ("openstudio.ruleset." + script.name(),result.finalCondition()->logChannel());
  EXPECT_EQ("Removed the 2 original spaces, and added one new one named 'Space 1'.",result.finalCondition()->logMessage());
}

class TestModelUserScript2 : public ModelUserScript {
 public:
  virtual std::string name() const override {
    return "TestModelUserScript2";
  }

  virtual std::vector<OSArgument> arguments(const Model& model) const override
  {
    std::vector<OSArgument> result;

    OSArgument arg = makeChoiceArgumentOfWorkspaceObjects(
                         "lights_definition",
                         IddObjectType::OS_Lights_Definition,
                         model);
    result.push_back(arg);

    arg = OSArgument::makeDoubleArgument("multiplier",false);
    arg.setDefaultValue(0.8);
    result.push_back(arg);

    return result;
  }

  // remove all spaces and add a new one
  virtual bool run(Model& model,
                   OSRunner& runner,
                   const std::map<std::string, OSArgument>& user_arguments) const override
  {
    ModelUserScript::run(model,runner,user_arguments); // initializes runner

    // calls runner.registerAttribute for 'lights_definition' and 'multiplier'
    if (!runner.validateUserArguments(arguments(model),user_arguments)) {
      return false;
    }

    // lights_definition argument value will be object handle
    Handle h = toUUID(runner.getStringArgumentValue("lights_definition",user_arguments));

    OptionalWorkspaceObject wo = model.getObject(h);
    if (!wo) {
      std::stringstream ss;
      ss << "Object " << toString(h) << " not found in model.";
      runner.registerError(ss.str());
      return false;
    }

    OptionalLightsDefinition lightsDef = wo->optionalCast<LightsDefinition>();
    if (!lightsDef) {
      std::stringstream ss;
      ss << wo->briefDescription() << " is not a LightsDefinition.";
      runner.registerError(ss.str());
      return false;
    }
    // save name of lights definition
    runner.registerValue("lights_definition_name",lightsDef->name().get());

    if (!(lightsDef->designLevelCalculationMethod() == "Watts/Area")) {
      std::stringstream ss;
      ss << "This measure only applies to lights definitions that are in units of Watts/Area. ";
      ss << lightsDef->briefDescription() << " is in units of ";
      ss << lightsDef->designLevelCalculationMethod() << ".";
      runner.registerAsNotApplicable(ss.str());
      return true;
    }

    double multiplier = runner.getDoubleArgumentValue("multiplier",user_arguments);

    if (multiplier < 0.0) {
      std::stringstream ss;
      ss << "The lighting power density multiplier must be greater than or equal to 0. ";
      ss << "Instead, it is " << toString(multiplier) << ".";
      runner.registerError(ss.str());
      return false;
    }

    double originalValue = lightsDef->wattsperSpaceFloorArea().get();
    double newValue = multiplier * originalValue;

    lightsDef->setWattsperSpaceFloorArea(newValue);

    // register effects of this measure

    // human-readable
    std::stringstream ss;
    ss << "The lighting power density of " << lightsDef->briefDescription();
    ss << ", which is used by " << lightsDef->quantity() << " instances covering ";
    ss << lightsDef->floorArea() << " m^2 of floor area, was " << originalValue << ".";
    runner.registerInitialCondition(ss.str()); ss.str("");
    ss << "The lighting power density of " << lightsDef->briefDescription();
    ss << " has been changed to " << newValue << ".";
    runner.registerFinalCondition(ss.str()); ss.str("");

    // machine-readable
    runner.registerValue("lpd_in","Input Lighting Power Density",originalValue,"W/m^2");
    runner.registerValue("lpd_out","Output Lighting Power Density",newValue,"W/m^2");
    runner.registerValue("lights_definition_num_instances",lightsDef->quantity());
    runner.registerValue("lights_definition_floor_area",
                         "Floor Area using this Lights Definition (SI)",
                         lightsDef->floorArea(),
                         "m^2");
    runner.registerValue("lights_definition_floor_area_ip",
                         "Floor Area using this Lights Definition (IP)",
                         convert(lightsDef->floorArea(),"m^2","ft^2").get(),
                         "ft^2");

    return true;
  }
};

TEST_F(RulesetFixture, UserScript_TestModelUserScript2) {
  TestModelUserScript2 script;
  EXPECT_EQ("TestModelUserScript2", script.name());

  Model model;

  // serialize ossrs
  openstudio::path fileDir = toPath("./OSResultOSSRs");
  boost::filesystem::create_directory(fileDir);

  // call with no arguments
  TestOSRunner runner;
  std::map<std::string, OSArgument> user_arguments;
  bool ok = script.run(model,runner,user_arguments);
  EXPECT_FALSE(ok);
  OSResult result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Fail);
  EXPECT_EQ(2u,result.errors().size()); // missing required and defaulted arguments
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_TRUE(result.attributes().empty());
  result.save(fileDir / toPath("TestModelUserScript2_1.ossr"),true);

  // call with required argument, but no lights definitions in model
  LightsDefinition lightsDef(model);
  OSArgumentVector definitions = script.arguments(model);
  user_arguments = runner.getUserInput(definitions);
  OSArgument arg = definitions[0];
  arg.setValue(toString(lightsDef.handle()));
  user_arguments["lights_definition"] = arg;
  lightsDef.remove();
  EXPECT_EQ(0u,model.numObjects());
  ok = script.run(model,runner,user_arguments);
  EXPECT_FALSE(ok);
  result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Fail);
  EXPECT_EQ(1u,result.errors().size()); // object not in model
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_EQ(2u,result.attributes().size()); // registers argument values
  result.save(fileDir / toPath("TestModelUserScript2_2.ossr"),true);
  // save attributes json for inspection
  saveJSON(result.attributes(),fileDir / toPath("TestModelUserScript2_2.json"),true);

  // call properly using default multiplier, but lights definition not Watts/Area
  lightsDef = LightsDefinition(model);
  lightsDef.setLightingLevel(700.0);
  definitions = script.arguments(model);
  user_arguments = runner.getUserInput(definitions);
  arg = definitions[0];
  arg.setValue(toString(lightsDef.handle()));
  user_arguments["lights_definition"] = arg;
  ok = script.run(model,runner,user_arguments);
  EXPECT_TRUE(ok);
  result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::NA);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(1u,result.info().size()); // Measure not applicable as called
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_EQ(3u,result.attributes().size()); // Registers lights definition name, then fails
  result.save(fileDir / toPath("TestModelUserScript2_3.ossr"),true);
  // save attributes json for inspection
  saveJSON(result.attributes(),fileDir / toPath("TestModelUserScript2_3.json"),true);

  // call properly using default multiplier
  lightsDef.setWattsperSpaceFloorArea(10.0);
  ok = script.run(model,runner,user_arguments);
  EXPECT_TRUE(ok);
  result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes
  EXPECT_EQ(8u,result.attributes().size());
  result.save(fileDir / toPath("TestModelUserScript2_4.ossr"),true);
  EXPECT_DOUBLE_EQ(8.0,lightsDef.wattsperSpaceFloorArea().get());
  // save attributes json for inspection
  saveJSON(result.attributes(),fileDir / toPath("TestModelUserScript2_4.json"),true);

  // call properly using different multiplier
  arg = definitions[1];
  arg.setValue(0.5);
  user_arguments["multiplier"] = arg;
  ok = script.run(model,runner,user_arguments);
  EXPECT_TRUE(ok);
  result = runner.result();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes
  EXPECT_EQ(8u,result.attributes().size());
  result.save(fileDir / toPath("TestModelUserScript2_5.ossr"),true);
  EXPECT_DOUBLE_EQ(4.0,lightsDef.wattsperSpaceFloorArea().get());
  // save attributes json for inspection
  saveJSON(result.attributes(),fileDir / toPath("TestModelUserScript2_5.json"),true);

  // check that can load ossrs
  OptionalOSResult temp = OSResult::load(fileDir / toPath("TestModelUserScript2_1.ossr"));
  ASSERT_TRUE(temp);
  result = temp.get();
  EXPECT_TRUE(result.value() == OSResultValue::Fail);
  EXPECT_EQ(2u,result.errors().size()); // missing required argument
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());

  temp = OSResult::load(fileDir / toPath("TestModelUserScript2_2.ossr"));
  ASSERT_TRUE(temp);
  result = temp.get();
  EXPECT_TRUE(result.value() == OSResultValue::Fail);
  EXPECT_EQ(1u,result.errors().size()); // object not in model
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());

  temp = OSResult::load(fileDir / toPath("TestModelUserScript2_3.ossr"));
  ASSERT_TRUE(temp);
  result = temp.get();
  EXPECT_TRUE(result.value() == OSResultValue::NA);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(1u,result.info().size()); // Measure not applicable as called
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());

  temp = OSResult::load(fileDir / toPath("TestModelUserScript2_4.ossr"));
  ASSERT_TRUE(temp);
  result = temp.get();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes

  temp = OSResult::load(fileDir / toPath("TestModelUserScript2_5.ossr"));
  ASSERT_TRUE(temp);
  result = temp.get();
  EXPECT_TRUE(result.value() == OSResultValue::Success);
  EXPECT_EQ(0u,result.errors().size());
  EXPECT_EQ(0u,result.warnings().size());
  EXPECT_EQ(0u,result.info().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes

  // check that can load attribute jsons
  std::vector<Attribute> loadedAttributes;
  NameFinder<Attribute> lightsDefinitionFinder("lights_definition",true);
  NameFinder<Attribute> multiplierFinder("multiplier",true);
  NameFinder<Attribute> lightsDefinitionNameFinder("lights_definition_name",true);
  NameFinder<Attribute> lpdInFinder("lpd_in",true);
  NameFinder<Attribute> lpdOutFinder("lpd_out",true);
  NameFinder<Attribute> lightsDefinitionNumInstancesFinder("lights_definition_num_instances",true);
  NameFinder<Attribute> lightsDefinitionFloorAreaFinder("lights_definition_floor_area",true);
  NameFinder<Attribute> lightsDefinitionFloorAreaIPFinder("lights_definition_floor_area_ip",true);
  AttributeVector::const_iterator it;

  // lights definition not in model - load attributes
  loadedAttributes = toVectorOfAttribute(fileDir / toPath("TestModelUserScript2_2.json"));
  EXPECT_EQ(2u,loadedAttributes.size());
  // lights_definition
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // multiplier
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),multiplierFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.8,it->valueAsDouble());
  EXPECT_FALSE(it->units());

  // run with bad lights definition type - load attributes
  loadedAttributes = toVectorOfAttribute(fileDir / toPath("TestModelUserScript2_3.json"));
  EXPECT_EQ(3u,loadedAttributes.size());
  // lights_definition
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // multiplier
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),multiplierFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.8,it->valueAsDouble());
  EXPECT_FALSE(it->units());
  // lights_definition_name
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionNameFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());

  // good run, default multiplier
  loadedAttributes = toVectorOfAttribute(fileDir / toPath("TestModelUserScript2_4.json"));
  EXPECT_EQ(8u,loadedAttributes.size());
  // lights_definition
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // multiplier
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),multiplierFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.8,it->valueAsDouble());
  EXPECT_FALSE(it->units());
  // lights_definition_name
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionNameFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // lpd_in
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lpdInFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(10.0,it->valueAsDouble());
  ASSERT_TRUE(it->units());
  EXPECT_EQ("W/m^2",it->units().get());
  // -- unit conversion example --
  OptionalDouble ipValue = convert(it->valueAsDouble(),it->units().get(),"W/ft^2");
  ASSERT_TRUE(ipValue);
  EXPECT_DOUBLE_EQ(0.9290304,*ipValue);
  // lpd_out
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lpdOutFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(8.0,it->valueAsDouble());
  ASSERT_TRUE(it->units());
  EXPECT_EQ("W/m^2",it->units().get());
  // lights_definition_num_instances
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionNumInstancesFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  //EXPECT_TRUE(it->valueType() == AttributeValueType::Integer); // JSON does not distinguish between int and float
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  // lights_definition_floor_area
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFloorAreaFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  ASSERT_TRUE(it->units());
  EXPECT_EQ("m^2",it->units().get());
  // lights_definition_floor_area_ip
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFloorAreaIPFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  ASSERT_TRUE(it->units());
  EXPECT_EQ("ft^2",it->units().get());

  // good run, different multiplier
  loadedAttributes = toVectorOfAttribute(fileDir / toPath("TestModelUserScript2_5.json"));
  EXPECT_EQ(8u,loadedAttributes.size());
  // lights_definition
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // multiplier
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),multiplierFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.5,it->valueAsDouble());
  EXPECT_FALSE(it->units());
  // lights_definition_name
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionNameFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::String);
  EXPECT_FALSE(it->valueAsString().empty());
  // lpd_in
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lpdInFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(8.0,it->valueAsDouble()); // uses previous example _out as _in
  ASSERT_TRUE(it->units());
  EXPECT_EQ("W/m^2",it->units().get());
  // -- unit conversion example --
  ipValue = convert(it->valueAsDouble(),it->units().get(),"W/ft^2");
  ASSERT_TRUE(ipValue);
  EXPECT_DOUBLE_EQ(0.74322432,*ipValue);
  // lpd_out
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lpdOutFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(4.0,it->valueAsDouble());
  ASSERT_TRUE(it->units());
  EXPECT_EQ("W/m^2",it->units().get());
  // lights_definition_num_instances
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionNumInstancesFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  //EXPECT_TRUE(it->valueType() == AttributeValueType::Integer); // JSON does not distinguish between int and float
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  // lights_definition_floor_area
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFloorAreaFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  ASSERT_TRUE(it->units());
  EXPECT_EQ("m^2",it->units().get());
  // lights_definition_floor_area_ip
  it = std::find_if(loadedAttributes.begin(),loadedAttributes.end(),lightsDefinitionFloorAreaIPFinder);
  ASSERT_FALSE(it == loadedAttributes.end());
  EXPECT_TRUE(it->valueType() == AttributeValueType::Double);
  ASSERT_TRUE(it->units());
  EXPECT_EQ("ft^2",it->units().get());
}
