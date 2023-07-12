/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../Lights.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
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

TEST_F(ModelFixture, OutputVariable_ThermalZone) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  for (const ThermalZone& zone : building.thermalZones()) {

    // all possible variables
    std::vector<std::string> variableNames = zone.outputVariableNames();
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Electricity Rate") != variableNames.end());
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Radiant Heating Energy") != variableNames.end());

    // variables actually found
    OutputVariableVector variables = zone.outputVariables();
    EXPECT_TRUE(variables.empty());
  }

  // add Zone Lights Electricity Rate to both zones
  OutputVariable lightsElectricPower("Zone Lights Electricity Rate", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electricity Rate", lightsElectricPower.variableName());
  //Check BCVTB
  EXPECT_FALSE(lightsElectricPower.exportToBCVTB());
  EXPECT_TRUE(lightsElectricPower.isExportToBCVTBDefaulted());
  lightsElectricPower.setExportToBCVTB(true);
  EXPECT_TRUE(lightsElectricPower.exportToBCVTB());
  lightsElectricPower.resetExportToBCVTB();
  EXPECT_FALSE(lightsElectricPower.exportToBCVTB());

  // add Zone Lights Radiant Heating Energy to only zone1
  OutputVariable lightsRadiantHeatGain("Zone Lights Radiant Heating Energy", model);
  EXPECT_TRUE(lightsRadiantHeatGain.setKeyValue(zone1.name().get()));
  EXPECT_EQ(zone1.name().get(), lightsRadiantHeatGain.keyValue());
  EXPECT_EQ("Zone Lights Radiant Heating Energy", lightsRadiantHeatGain.variableName());

  ASSERT_EQ(2u, zone1.outputVariables().size());
  if (lightsElectricPower.handle() == zone1.outputVariables()[0].handle()) {
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[1].handle());
  } else {
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[1].handle());
  }

  ASSERT_EQ(1u, zone2.outputVariables().size());
  EXPECT_EQ(lightsElectricPower.handle(), zone2.outputVariables()[0].handle());
}

TEST_F(ModelFixture, MapOfAllOutputVariables) {
  Model model = exampleModel();

  // map of variable name to output variable
  std::map<std::string, boost::optional<OutputVariable>> outputVariableMap;

  // get list of all variable names
  for (const auto& modelObject : model.getModelObjects<ModelObject>()) {
    for (const std::string& variableName : modelObject.outputVariableNames()) {
      outputVariableMap[variableName] = boost::none;
    }
  }

  // add all variables to map, allow only one variable per variable name in this application
  for (auto& outputVariable : model.getConcreteModelObjects<OutputVariable>()) {
    if (outputVariableMap[outputVariable.variableName()]) {
      // already have output variable for this name, then remove this object
      outputVariable.remove();
    } else {

      // make sure that key value is set to '*'
      outputVariable.setKeyValue("*");

      outputVariableMap[outputVariable.variableName()] = outputVariable;
    }
  }

  // now make an output variable for each variable name
  for (const auto& [varName, outVar_] : outputVariableMap) {
    if (!outVar_) {
      OutputVariable outputVariable(varName, model);
      outputVariable.setReportingFrequency("Hourly");
      outputVariableMap[varName] = outputVariable;
    }

    boost::optional<OutputVariable> outputVariable = outputVariableMap[varName];
    ASSERT_TRUE(outputVariable);
  }
}

TEST_F(ModelFixture, BadVariableName) {
  Model model;

  OutputVariable goodVar("Good Name", model);
  EXPECT_EQ("Good Name", goodVar.variableName());

  OutputVariable badVar("Bad, !Name", model);
  EXPECT_EQ("Bad, !Name", badVar.variableName());
}
