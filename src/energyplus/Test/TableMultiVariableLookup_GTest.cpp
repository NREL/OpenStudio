/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/TableMultiVariableLookup.hpp"
#include "../../model/TableMultiVariableLookup_Impl.hpp"

#include <utilities/idd/Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariableList_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_TableMultiVariableLookup) {
  {
    Model m;
    TableMultiVariableLookup table(m, 1);

    ASSERT_TRUE(table.addPoint(70, 0.1));
    ASSERT_TRUE(table.addPoint(72, 0.3));
    ASSERT_TRUE(table.addPoint(74, 0.5));
    ASSERT_TRUE(table.addPoint(76, 0.7));
    ASSERT_TRUE(table.addPoint(78, 0.9));

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
    EXPECT_FALSE(idfTable.getString(Table_LookupFields::NormalizationMethod, false, true));  // Don't return default, return unitialized empty
    EXPECT_FALSE(idfTable.getDouble(Table_LookupFields::NormalizationDivisor));

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
    ASSERT_TRUE(
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
    EXPECT_EQ(
      independentVariableObject.nameString(),
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
  }

  {
    Model m;

    // Table with 3 independent variables
    TableMultiVariableLookup table(m, 3);

    // from the E+ IO Reference for Table:Lookup
    EXPECT_TRUE(table.addPoint(1, 1, 1, 111));  // 0
    EXPECT_TRUE(table.addPoint(1, 1, 2, 112));  // 1
    EXPECT_TRUE(table.addPoint(1, 1, 3, 113));  // 2
    EXPECT_TRUE(table.addPoint(1, 1, 4, 114));  // 3
    EXPECT_TRUE(table.addPoint(1, 2, 1, 121));  // 4
    EXPECT_TRUE(table.addPoint(1, 2, 2, 122));  // 5
    EXPECT_TRUE(table.addPoint(1, 2, 3, 123));  // 6
    EXPECT_TRUE(table.addPoint(1, 2, 4, 124));  // 7

    EXPECT_TRUE(table.addPoint(2, 1, 1, 211));  // 8
    EXPECT_TRUE(table.addPoint(2, 1, 2, 212));  // 9
    EXPECT_TRUE(table.addPoint(2, 1, 3, 213));  // 10
    EXPECT_TRUE(table.addPoint(2, 1, 4, 214));  // 11
    EXPECT_TRUE(table.addPoint(2, 2, 1, 221));  // 12
    EXPECT_TRUE(table.addPoint(2, 2, 2, 222));  // 13
    EXPECT_TRUE(table.addPoint(2, 2, 3, 223));  // 14
    EXPECT_TRUE(table.addPoint(2, 2, 4, 224));  // 15

    EXPECT_TRUE(table.addPoint(3, 1, 1, 311));  // 16
    EXPECT_TRUE(table.addPoint(3, 1, 2, 312));  // 17
    EXPECT_TRUE(table.addPoint(3, 1, 3, 313));  // 18
    EXPECT_TRUE(table.addPoint(3, 1, 4, 314));  // 19
    EXPECT_TRUE(table.addPoint(3, 2, 1, 321));  // 20
    EXPECT_TRUE(table.addPoint(3, 2, 2, 322));  // 21
    EXPECT_TRUE(table.addPoint(3, 2, 3, 323));  // 22
    EXPECT_TRUE(table.addPoint(3, 2, 4, 324));  // 23

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
    ASSERT_TRUE(
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
    EXPECT_EQ(
      independentVariableObjects[0].nameString(),
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
    ASSERT_TRUE(
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
    EXPECT_EQ(
      independentVariableObjects[1].nameString(),
      independentVariableListObject.getExtensibleGroup(1).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
    ASSERT_TRUE(
      independentVariableListObject.getExtensibleGroup(0).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName));
    EXPECT_EQ(
      independentVariableObjects[2].nameString(),
      independentVariableListObject.getExtensibleGroup(2).getString(Table_IndependentVariableListExtensibleFields::IndependentVariableName).get());
  }

  // One with a normalization reference
  {
    Model m;
    TableMultiVariableLookup table(m, 1);
    table.setNormalizationReference(0.9);
    ASSERT_TRUE(table.addPoint(70, 0.1));
    ASSERT_TRUE(table.addPoint(72, 0.3));
    ASSERT_TRUE(table.addPoint(74, 0.5));
    ASSERT_TRUE(table.addPoint(76, 0.7));
    ASSERT_TRUE(table.addPoint(78, 0.9));

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

    // Normallization reference should be filled in TableLookup
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
