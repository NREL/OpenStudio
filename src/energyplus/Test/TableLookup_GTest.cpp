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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/TableLookup.hpp"
#include "../../model/TableLookup_Impl.hpp"
#include "../../model/TableIndependentVariable.hpp"
#include "../../model/TableIndependentVariable_Impl.hpp"

#include <utilities/idd/Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariableList_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_TableLookup_Basic) {
  Model m;
  TableLookup tableLookup(m);
  tableLookup.addOutputValue(0.1);
  tableLookup.addOutputValue(0.3);
  tableLookup.addOutputValue(0.5);
  tableLookup.addOutputValue(0.7);
  tableLookup.addOutputValue(0.9);

  TableIndependentVariable independentVariable(m);
  independentVariable.setInterpolationMethod("Cubic");
  independentVariable.setExtrapolationMethod("Linear");
  independentVariable.setMinimumValue(70);
  independentVariable.setMaximumValue(78);
  independentVariable.addValue(70);
  independentVariable.addValue(72);
  independentVariable.addValue(74);
  independentVariable.addValue(76);
  independentVariable.addValue(78);
  tableLookup.addIndependentVariable(independentVariable);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_Lookup);
  ASSERT_EQ(1u, tableObjects.size());

  WorkspaceObject idfTable = tableObjects.front();

  ASSERT_EQ(idfTable.getString(Table_LookupFields::OutputUnitType).get(), "Dimensionless");
  ASSERT_EQ(5u, idfTable.numExtensibleGroups());
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.1);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.3);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.5);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.7);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.9);
  ASSERT_EQ(idfTable.getString(Table_LookupFields::NormalizationMethod).get(), "None");
  ASSERT_EQ(idfTable.getDouble(Table_LookupFields::NormalizationDivisor).get(), 1.0);

  std::vector<WorkspaceObject> independentVariableObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariable);
  ASSERT_EQ(1u, independentVariableObjects.size());

  WorkspaceObject independentVariableObject = independentVariableObjects.front();
  ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::InterpolationMethod));
  EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::InterpolationMethod).get(), "Cubic");
  ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::ExtrapolationMethod));
  EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::ExtrapolationMethod).get(), "Linear");
  ASSERT_TRUE(independentVariableObject.getDouble(Table_IndependentVariableFields::MinimumValue));
  EXPECT_DOUBLE_EQ(independentVariableObject.getDouble(Table_IndependentVariableFields::MinimumValue).get(), 70.0);
  ASSERT_TRUE(independentVariableObject.getDouble(Table_IndependentVariableFields::MaximumValue));
  EXPECT_DOUBLE_EQ(independentVariableObject.getDouble(Table_IndependentVariableFields::MaximumValue).get(), 78.0);
  EXPECT_FALSE(independentVariableObject.getDouble(Table_IndependentVariableFields::NormalizationReferenceValue));
  ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::UnitType));
  EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::UnitType).get(), "Dimensionless");
  ASSERT_EQ(5u, independentVariableObject.numExtensibleGroups());

  std::vector<WorkspaceObject> independentVariableListObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariableList);
  ASSERT_EQ(1u, independentVariableListObjects.size());

  WorkspaceObject independentVariableListObject = independentVariableListObjects.front();
  ASSERT_EQ(1u, independentVariableListObject.numExtensibleGroups());
  ASSERT_TRUE(independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
  EXPECT_EQ(
    independentVariableObject.nameString(),
    independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_1TableLookup_3independentVariables) {

  Model m;

  // Table with 3 independent variables
  TableLookup tableLookup(m);
  tableLookup.addOutputValue(111);
  tableLookup.addOutputValue(112);
  tableLookup.addOutputValue(113);
  tableLookup.addOutputValue(114);
  tableLookup.addOutputValue(121);
  tableLookup.addOutputValue(122);
  tableLookup.addOutputValue(123);
  tableLookup.addOutputValue(124);
  tableLookup.addOutputValue(211);
  tableLookup.addOutputValue(212);
  tableLookup.addOutputValue(213);
  tableLookup.addOutputValue(214);
  tableLookup.addOutputValue(221);
  tableLookup.addOutputValue(222);
  tableLookup.addOutputValue(223);
  tableLookup.addOutputValue(224);
  tableLookup.addOutputValue(311);
  tableLookup.addOutputValue(312);
  tableLookup.addOutputValue(313);
  tableLookup.addOutputValue(314);
  tableLookup.addOutputValue(321);
  tableLookup.addOutputValue(322);
  tableLookup.addOutputValue(323);
  tableLookup.addOutputValue(324);

  TableIndependentVariable independentVariable1(m);
  independentVariable1.setInterpolationMethod("Cubic");
  independentVariable1.setExtrapolationMethod("Linear");
  independentVariable1.setMinimumValue(1);
  independentVariable1.setMaximumValue(3);
  independentVariable1.addValue(1);
  independentVariable1.addValue(2);
  independentVariable1.addValue(3);
  tableLookup.addIndependentVariable(independentVariable1);

  TableIndependentVariable independentVariable2(m);
  independentVariable2.setInterpolationMethod("Cubic");
  independentVariable2.setExtrapolationMethod("Linear");
  independentVariable2.setMinimumValue(1);
  independentVariable2.setMaximumValue(2);
  independentVariable2.addValue(1);
  independentVariable2.addValue(2);
  tableLookup.addIndependentVariable(independentVariable2);

  TableIndependentVariable independentVariable3(m);
  independentVariable3.setInterpolationMethod("Cubic");
  independentVariable3.setExtrapolationMethod("Linear");
  independentVariable3.setMinimumValue(1);
  independentVariable3.setMaximumValue(4);
  independentVariable3.addValue(1);
  independentVariable3.addValue(2);
  independentVariable3.addValue(3);
  independentVariable3.addValue(4);
  tableLookup.addIndependentVariable(independentVariable3);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_Lookup);
  ASSERT_EQ(1u, tableObjects.size());

  WorkspaceObject idfTable = tableObjects.front();

  ASSERT_EQ(idfTable.getString(Table_LookupFields::OutputUnitType).get(), "Dimensionless");
  ASSERT_EQ(24u, idfTable.numExtensibleGroups());
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 111);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 112);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 113);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 114);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 121);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(5).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 122);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(6).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 123);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(7).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 124);

  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(8).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 211);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(9).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 212);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(10).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 213);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(11).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 214);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(12).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 221);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(13).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 222);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(14).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 223);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(15).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 224);

  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(16).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 311);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(17).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 312);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(18).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 313);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(19).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 314);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(20).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 321);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(21).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 322);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(22).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 323);
  ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(23).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 324);

  std::vector<WorkspaceObject> independentVariableObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariable);
  std::sort(independentVariableObjects.begin(), independentVariableObjects.end(), IdfObjectNameLess());
  ASSERT_EQ(3u, independentVariableObjects.size());

  ASSERT_TRUE(independentVariableObjects[0].getString(Table_IndependentVariableFields::InterpolationMethod));
  EXPECT_EQ(independentVariableObjects[0].getString(Table_IndependentVariableFields::InterpolationMethod).get(), "Cubic");
  ASSERT_TRUE(independentVariableObjects[0].getString(Table_IndependentVariableFields::ExtrapolationMethod));
  EXPECT_EQ(independentVariableObjects[0].getString(Table_IndependentVariableFields::ExtrapolationMethod).get(), "Linear");
  ASSERT_TRUE(independentVariableObjects[0].getDouble(Table_IndependentVariableFields::MinimumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[0].getDouble(Table_IndependentVariableFields::MinimumValue).get(), 1.0);
  ASSERT_TRUE(independentVariableObjects[0].getDouble(Table_IndependentVariableFields::MaximumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[0].getDouble(Table_IndependentVariableFields::MaximumValue).get(), 3.0);
  EXPECT_FALSE(independentVariableObjects[0].getDouble(Table_IndependentVariableFields::NormalizationReferenceValue));
  ASSERT_TRUE(independentVariableObjects[0].getString(Table_IndependentVariableFields::UnitType));
  EXPECT_EQ(independentVariableObjects[0].getString(Table_IndependentVariableFields::UnitType).get(), "Dimensionless");
  ASSERT_EQ(3u, independentVariableObjects[0].numExtensibleGroups());

  ASSERT_TRUE(independentVariableObjects[1].getString(Table_IndependentVariableFields::InterpolationMethod));
  EXPECT_EQ(independentVariableObjects[1].getString(Table_IndependentVariableFields::InterpolationMethod).get(), "Cubic");
  ASSERT_TRUE(independentVariableObjects[1].getString(Table_IndependentVariableFields::ExtrapolationMethod));
  EXPECT_EQ(independentVariableObjects[1].getString(Table_IndependentVariableFields::ExtrapolationMethod).get(), "Linear");
  ASSERT_TRUE(independentVariableObjects[1].getDouble(Table_IndependentVariableFields::MinimumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[1].getDouble(Table_IndependentVariableFields::MinimumValue).get(), 1.0);
  ASSERT_TRUE(independentVariableObjects[1].getDouble(Table_IndependentVariableFields::MaximumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[1].getDouble(Table_IndependentVariableFields::MaximumValue).get(), 2.0);
  EXPECT_FALSE(independentVariableObjects[1].getDouble(Table_IndependentVariableFields::NormalizationReferenceValue));
  ASSERT_TRUE(independentVariableObjects[1].getString(Table_IndependentVariableFields::UnitType));
  EXPECT_EQ(independentVariableObjects[1].getString(Table_IndependentVariableFields::UnitType).get(), "Dimensionless");
  ASSERT_EQ(2u, independentVariableObjects[1].numExtensibleGroups());

  ASSERT_TRUE(independentVariableObjects[2].getString(Table_IndependentVariableFields::InterpolationMethod));
  EXPECT_EQ(independentVariableObjects[2].getString(Table_IndependentVariableFields::InterpolationMethod).get(), "Cubic");
  ASSERT_TRUE(independentVariableObjects[2].getString(Table_IndependentVariableFields::ExtrapolationMethod));
  EXPECT_EQ(independentVariableObjects[2].getString(Table_IndependentVariableFields::ExtrapolationMethod).get(), "Linear");
  ASSERT_TRUE(independentVariableObjects[2].getDouble(Table_IndependentVariableFields::MinimumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[2].getDouble(Table_IndependentVariableFields::MinimumValue).get(), 1.0);
  ASSERT_TRUE(independentVariableObjects[2].getDouble(Table_IndependentVariableFields::MaximumValue));
  EXPECT_DOUBLE_EQ(independentVariableObjects[2].getDouble(Table_IndependentVariableFields::MaximumValue).get(), 4.0);
  EXPECT_FALSE(independentVariableObjects[2].getDouble(Table_IndependentVariableFields::NormalizationReferenceValue));
  ASSERT_TRUE(independentVariableObjects[2].getString(Table_IndependentVariableFields::UnitType));
  EXPECT_EQ(independentVariableObjects[2].getString(Table_IndependentVariableFields::UnitType).get(), "Dimensionless");
  ASSERT_EQ(4u, independentVariableObjects[2].numExtensibleGroups());

  std::vector<WorkspaceObject> independentVariableListObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariableList);
  ASSERT_EQ(1u, independentVariableListObjects.size());

  WorkspaceObject independentVariableListObject = independentVariableListObjects.front();
  ASSERT_EQ(3u, independentVariableListObject.numExtensibleGroups());
  ASSERT_TRUE(independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
  EXPECT_EQ(
    independentVariableObjects[0].nameString(),
    independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
  ASSERT_TRUE(independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
  EXPECT_EQ(
    independentVariableObjects[1].nameString(),
    independentVariableListObject.getExtensibleGroup(1).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
  ASSERT_TRUE(independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
  EXPECT_EQ(
    independentVariableObjects[2].nameString(),
    independentVariableListObject.getExtensibleGroup(2).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_TableLookup_NormalizationReference) {

  // One with a normalization reference
  {
    Model m;
    TableLookup tableLookup(m);
    tableLookup.setNormalizationMethod("DivisorOnly");
    tableLookup.setNormalizationDivisor(0.9);
    tableLookup.addOutputValue(0.1);
    tableLookup.addOutputValue(0.3);
    tableLookup.addOutputValue(0.5);
    tableLookup.addOutputValue(0.7);
    tableLookup.addOutputValue(0.9);

    TableIndependentVariable independentVariable(m);
    independentVariable.setInterpolationMethod("Cubic");
    independentVariable.setExtrapolationMethod("Linear");
    independentVariable.setMinimumValue(70);
    independentVariable.setMaximumValue(78);
    independentVariable.addValue(70);
    independentVariable.addValue(72);
    independentVariable.addValue(74);
    independentVariable.addValue(76);
    independentVariable.addValue(78);
    tableLookup.addIndependentVariable(independentVariable);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> tableObjects = workspace.getObjectsByType(IddObjectType::Table_Lookup);
    ASSERT_EQ(1u, tableObjects.size());

    WorkspaceObject idfTable = tableObjects.front();

    ASSERT_EQ(idfTable.getString(Table_LookupFields::OutputUnitType).get(), "Dimensionless");
    ASSERT_EQ(5u, idfTable.numExtensibleGroups());
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(0).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.1);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(1).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.3);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(2).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.5);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(3).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.7);
    ASSERT_DOUBLE_EQ(idfTable.getExtensibleGroup(4).getDouble(Table_LookupExtensibleFields::OutputValue).get(), 0.9);

    // Normalization reference should be filled in TableLookup
    ASSERT_TRUE(idfTable.getString(Table_LookupFields::NormalizationMethod));
    EXPECT_EQ(idfTable.getString(Table_LookupFields::NormalizationMethod).get(), "DivisorOnly");
    ASSERT_TRUE(idfTable.getDouble(Table_LookupFields::NormalizationDivisor));
    EXPECT_EQ(idfTable.getDouble(Table_LookupFields::NormalizationDivisor).get(), 0.9);

    std::vector<WorkspaceObject> independentVariableObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariable);
    ASSERT_EQ(1u, independentVariableObjects.size());

    WorkspaceObject independentVariableObject = independentVariableObjects.front();
    ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::InterpolationMethod));
    EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::InterpolationMethod).get(), "Cubic");
    ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::ExtrapolationMethod));
    EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::ExtrapolationMethod).get(), "Linear");
    ASSERT_TRUE(independentVariableObject.getDouble(Table_IndependentVariableFields::MinimumValue));
    EXPECT_DOUBLE_EQ(independentVariableObject.getDouble(Table_IndependentVariableFields::MinimumValue).get(), 70.0);
    ASSERT_TRUE(independentVariableObject.getDouble(Table_IndependentVariableFields::MaximumValue));
    EXPECT_DOUBLE_EQ(independentVariableObject.getDouble(Table_IndependentVariableFields::MaximumValue).get(), 78.0);

    // It's the TableLookup object that gets the normalization reference
    EXPECT_FALSE(independentVariableObject.getDouble(Table_IndependentVariableFields::NormalizationReferenceValue));

    ASSERT_TRUE(independentVariableObject.getString(Table_IndependentVariableFields::UnitType));
    EXPECT_EQ(independentVariableObject.getString(Table_IndependentVariableFields::UnitType).get(), "Dimensionless");
    ASSERT_EQ(5u, independentVariableObject.numExtensibleGroups());

    std::vector<WorkspaceObject> independentVariableListObjects = workspace.getObjectsByType(IddObjectType::Table_IndependentVariableList);
    ASSERT_EQ(1u, independentVariableListObjects.size());

    WorkspaceObject independentVariableListObject = independentVariableListObjects.front();
    ASSERT_EQ(1u, independentVariableListObject.numExtensibleGroups());
    ASSERT_TRUE(
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
    EXPECT_EQ(
      independentVariableObject.nameString(),
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
  }
}
