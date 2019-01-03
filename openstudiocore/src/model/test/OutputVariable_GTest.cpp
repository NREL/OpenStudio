/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

TEST_F(ModelFixture, OutputVariable_ThermalZone)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  for (const ThermalZone& zone : building.thermalZones()) {

    // all possible variables
    std::vector<std::string> variableNames = zone.outputVariableNames();
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Electric Power") != variableNames.end());
    EXPECT_TRUE(std::find(variableNames.begin(), variableNames.end(), "Zone Lights Radiant Heating Energy") != variableNames.end());

    // variables actually found
    OutputVariableVector variables = zone.outputVariables();
    EXPECT_TRUE(variables.empty());
  }

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electric Power", lightsElectricPower.variableName());
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
  if (lightsElectricPower.handle() == zone1.outputVariables()[0].handle()){
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[1].handle());
  }else{
    EXPECT_EQ(lightsRadiantHeatGain.handle(), zone1.outputVariables()[0].handle());
    EXPECT_EQ(lightsElectricPower.handle(), zone1.outputVariables()[1].handle());
  }

  ASSERT_EQ(1u, zone2.outputVariables().size());
  EXPECT_EQ(lightsElectricPower.handle(), zone2.outputVariables()[0].handle());

}

TEST_F(ModelFixture, MapOfAllOutputVariables)
{
  Model model = exampleModel();

  // map of variable name to output variable
  std::map<std::string, boost::optional<OutputVariable> > outputVariableMap;

  // get list of all variable names
  for (const ModelObject& modelObject : model.getModelObjects<ModelObject>()){
    for (const std::string& variableName : modelObject.outputVariableNames()){
      outputVariableMap[variableName] = boost::none;
    }
  }

  // add all variables to map, allow only one variable per variable name in this application
  for (OutputVariable outputVariable : model.getModelObjects<OutputVariable>()){
    if (outputVariableMap[outputVariable.variableName()]){
      // already have output variable for this name, then remove this object
      outputVariable.remove();
    }else{

      // make sure that key value is set to '*'
      outputVariable.setKeyValue("*");

      outputVariableMap[outputVariable.variableName()] = outputVariable;
    }
  }

  // now make an output variable for each variable name
  typedef std::pair<std::string, boost::optional<OutputVariable> > MapType;
  for (MapType mapVal : outputVariableMap){
    if (!mapVal.second){
      OutputVariable outputVariable(mapVal.first, model);
      outputVariable.setReportingFrequency("Hourly");
      outputVariableMap[mapVal.first] = outputVariable;
    }

    boost::optional<OutputVariable> outputVariable = outputVariableMap[mapVal.first];
    ASSERT_TRUE(outputVariable);
  }
}


TEST_F(ModelFixture, BadVariableName)
{
  Model model;

  OutputVariable goodVar("Good Name", model);
  EXPECT_EQ("Good Name", goodVar.variableName());

  OutputVariable badVar("Bad, !Name", model);
  EXPECT_EQ("Bad, !Name", badVar.variableName());
}
