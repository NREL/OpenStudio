/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "MeasureFixture.hpp"

#include "../OSArgument.hpp"
#include "../OSRunner.hpp"
#include "../OSMeasure.hpp"
#include "../ModelMeasure.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Finder.hpp"
#include "../../utilities/filetypes/WorkflowJSON.hpp"
#include "../../utilities/filetypes/WorkflowStep.hpp"

#include "../../utilities/units/QuantityConverter.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::measure;

class TestOSRunner : public OSRunner {
 public:
   TestOSRunner(const WorkflowJSON& workflow)
     : OSRunner(workflow)
   {}

  virtual bool inSelection(const openstudio::model::ModelObject& modelObject) const override {
    return false;
  }

};

// derive some test classes
class TestModelUserScript1 : public ModelMeasure {
 public:

  virtual std::string name() const override {
    return "TestModelUserScript1";
  }

  // remove all spaces and add a new one
  virtual bool run(Model& model,
                   OSRunner& runner,
                   const std::map<std::string, OSArgument>& user_arguments) const override
  {
    ModelMeasure::run(model,runner,user_arguments);

    if (!runner.validateUserArguments(arguments(model), user_arguments)){
      return false;
    }

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


TEST_F(MeasureFixture, UserScript_TestModelUserScript1) {
  TestModelUserScript1 script;
  EXPECT_EQ("TestModelUserScript1", script.name());

  std::vector<WorkflowStep> steps;
  steps.push_back(MeasureStep("dummy"));

  WorkflowJSON workflow;
  workflow.setWorkflowSteps(steps);

  TestOSRunner runner(workflow);
  std::map<std::string, OSArgument> user_arguments;

  // test with empty model
  openstudio::model::Model model1;
  EXPECT_EQ(0u, model1.getModelObjects<openstudio::model::Space>().size());
  EXPECT_EQ(0u, script.arguments(model1).size());
  script.run(model1, runner, user_arguments);
  EXPECT_EQ(1u, model1.getModelObjects<openstudio::model::Space>().size());
  WorkflowStepResult result = runner.result();
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Success);
  EXPECT_EQ(0u,result.stepErrors().size());
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  ASSERT_TRUE(result.initialCondition());
  EXPECT_EQ("Initial model had 0 spaces.",result.initialCondition()->logMessage());
  ASSERT_TRUE(result.finalCondition());
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
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Success);
  EXPECT_EQ(0u,result.stepErrors().size());
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  ASSERT_TRUE(result.initialCondition());
  EXPECT_EQ("Initial model had 2 spaces.",result.initialCondition()->logMessage());
  ASSERT_TRUE(result.finalCondition());
  EXPECT_EQ("Removed the 2 original spaces, and added one new one named 'Space 1'.",result.finalCondition()->logMessage());
}

class TestModelUserScript2 : public ModelMeasure {
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
    ModelMeasure::run(model,runner,user_arguments); // initializes runner

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

TEST_F(MeasureFixture, UserScript_TestModelUserScript2) {
  TestModelUserScript2 script;
  EXPECT_EQ("TestModelUserScript2", script.name());

  Model model;

  // serialize ossrs
  openstudio::path fileDir = toPath("./OSResultOSSRs");
  openstudio::filesystem::create_directory(fileDir);

  std::vector<WorkflowStep> steps;
  steps.push_back(MeasureStep("dummy"));

  WorkflowJSON workflow;
  workflow.setWorkflowSteps(steps);

  // call with no arguments
  TestOSRunner runner(workflow);
  std::map<std::string, OSArgument> user_arguments;
  bool ok = script.run(model,runner,user_arguments);
  EXPECT_FALSE(ok);
  WorkflowStepResult result = runner.result();
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Fail);
  EXPECT_EQ(2u,result.stepErrors().size()); // missing required and defaulted arguments
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_TRUE(result.stepValues().empty());

  // call with required argument, but no lights definitions in model
  runner.reset();
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
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Fail);
  EXPECT_EQ(1u,result.stepErrors().size()); // object not in model
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_EQ(2u,result.stepValues().size()); // registers argument values

  // call properly using default multiplier, but lights definition not Watts/Area
  runner.reset();
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
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::NA);
  EXPECT_EQ(0u,result.stepErrors().size());
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(1u,result.stepInfo().size()); // Measure not applicable as called
  EXPECT_FALSE(result.initialCondition());
  EXPECT_FALSE(result.finalCondition());
  EXPECT_EQ(3u,result.stepValues().size()); // Registers lights definition name, then fails

  // call properly using default multiplier
  runner.reset();
  lightsDef.setWattsperSpaceFloorArea(10.0);
  ok = script.run(model,runner,user_arguments);
  EXPECT_TRUE(ok);
  result = runner.result();
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Success);
  EXPECT_EQ(0u,result.stepErrors().size());
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes
  EXPECT_EQ(8u,result.stepValues().size());

  EXPECT_DOUBLE_EQ(8.0,lightsDef.wattsperSpaceFloorArea().get());

  // call properly using different multiplier
  runner.reset();
  arg = definitions[1];
  arg.setValue(0.5);
  user_arguments["multiplier"] = arg;
  ok = script.run(model,runner,user_arguments);
  EXPECT_TRUE(ok);
  result = runner.result();
  ASSERT_TRUE(result.stepResult());
  EXPECT_TRUE(result.stepResult()->value() == StepResult::Success);
  EXPECT_EQ(0u,result.stepErrors().size());
  EXPECT_EQ(0u,result.stepWarnings().size());
  EXPECT_EQ(0u,result.stepInfo().size());
  EXPECT_TRUE(result.initialCondition()); // describes original state
  EXPECT_TRUE(result.finalCondition());   // describes changes
  EXPECT_EQ(8u,result.stepValues().size());

  EXPECT_DOUBLE_EQ(4.0,lightsDef.wattsperSpaceFloorArea().get());

}

TEST_F(MeasureFixture, RegisterValueNames) {
  WorkflowJSON workflow;
  OSRunner runner(workflow);
  runner.registerValue("value", 0);
  runner.registerValue("ValueTwo", 1);
  runner.registerValue("VALUETHREE", 2);
  runner.registerValue("4ValueFour", 3);
  runner.registerValue("Value<Five>", 4);
  runner.registerValue("#V|a@l#u$e%F^i&v*e(V)a{l}u_e[F]i;v:e'V\"a,l<u.e>F\\i/v?e+V=", 5);
  runner.registerValue("Value&$@$Six", 6);
  runner.registerValue("Value____Seven", 7);
  std::vector<Attribute> attributes = runner.result().attributes();
  ASSERT_EQ(8u, attributes.size());
  EXPECT_EQ("value", attributes[0].name());
  EXPECT_EQ("value_two", attributes[1].name());
  EXPECT_EQ("valuethree", attributes[2].name());
  EXPECT_EQ("_4_value_four", attributes[3].name());
  EXPECT_EQ("value_five", attributes[4].name());
  EXPECT_EQ("v_a_l_u_e_f_i_v_e_v_a_l_u_e_f_i_v_e_v_a_l_u_e_f_i_v_e_v", attributes[5].name());
  EXPECT_EQ("value_six", attributes[6].name());
  EXPECT_EQ("value_seven", attributes[7].name());
}