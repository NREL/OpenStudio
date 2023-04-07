/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginOutputVariable.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Variables_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_OutputVariable_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginOutputVariable) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  PythonPluginVariable pyVar(m);
  pyVar.setName("PythonPluginVariable1");

  PythonPluginOutputVariable pyOutVar(pyVar);
  pyOutVar.setName("Chiller Electricity Consumption");
  EXPECT_TRUE(pyOutVar.setTypeofDatainVariable("Metered"));
  EXPECT_TRUE(pyOutVar.setUpdateFrequency("SystemTimestep"));
  EXPECT_TRUE(pyOutVar.setUnits("J"));
  EXPECT_TRUE(pyOutVar.setResourceType("Electricity"));
  EXPECT_TRUE(pyOutVar.setGroupType("HVAC"));
  EXPECT_TRUE(pyOutVar.setEndUseCategory("Cooling"));
  EXPECT_TRUE(pyOutVar.setEndUseSubcategory("Custom air cooled chiller"));

  Workspace w = ft.translateModel(m);
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::PythonPlugin_Variables).size());
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_OutputVariable);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObj = idfObjs.front();

  EXPECT_EQ(pyOutVar.nameString(), idfObj.nameString());

  EXPECT_EQ("PythonPluginVariable1", idfObj.getString(PythonPlugin_OutputVariableFields::PythonPluginVariableName).get());
  EXPECT_EQ("Metered", idfObj.getString(PythonPlugin_OutputVariableFields::TypeofDatainVariable).get());
  EXPECT_EQ("SystemTimestep", idfObj.getString(PythonPlugin_OutputVariableFields::UpdateFrequency).get());
  EXPECT_EQ("J", idfObj.getString(PythonPlugin_OutputVariableFields::Units).get());
  EXPECT_EQ("Electricity", idfObj.getString(PythonPlugin_OutputVariableFields::ResourceType).get());
  EXPECT_EQ("HVAC", idfObj.getString(PythonPlugin_OutputVariableFields::GroupType).get());
  EXPECT_EQ("Cooling", idfObj.getString(PythonPlugin_OutputVariableFields::EndUseCategory).get());
  EXPECT_EQ("Custom air cooled chiller", idfObj.getString(PythonPlugin_OutputVariableFields::EndUseSubcategory).get());
}
