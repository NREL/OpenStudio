/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputSQLite.hpp"
#include "../../model/OutputSQLite_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputSQLite) {

  // Not there, but workspace should have it
  {
    Model m;
    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sqlite(idfObjs[0]);

    EXPECT_EQ("SimpleAndTabular", idf_sqlite.getString(Output_SQLiteFields::OptionType, false).get());
    EXPECT_TRUE(idf_sqlite.isEmpty(Output_SQLiteFields::UnitConversionforTabularData));
  }

  // Not there, m_forwardTranslatorOptions.excludeSQliteOutputReport() = True
  {
    Model m;
    ForwardTranslator ft;
    ft.setExcludeSQliteOutputReport(true);
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(0u, idfObjs.size());
  }

  // It's already in the model
  {
    Model m;
    ForwardTranslator ft;

    OutputSQLite outputSQLite = m.getUniqueModelObject<OutputSQLite>();

    EXPECT_TRUE(outputSQLite.setOptionType("Simple"));
    EXPECT_TRUE(outputSQLite.setUnitConversionforTabularData("None"));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sqlite(idfObjs[0]);

    EXPECT_EQ("Simple", idf_sqlite.getString(Output_SQLiteFields::OptionType, false).get());
    EXPECT_EQ("None", idf_sqlite.getString(Output_SQLiteFields::UnitConversionforTabularData, false).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputSQLite) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m;
  m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputSQLite>());

  OptionalWorkspaceObject _i_outputSQLite = w.addObject(IdfObject(IddObjectType::Output_SQLite));
  ASSERT_TRUE(_i_outputSQLite);

  EXPECT_TRUE(_i_outputSQLite->setString(Output_SQLiteFields::OptionType, "Simple"));
  EXPECT_TRUE(_i_outputSQLite->setString(Output_SQLiteFields::UnitConversionforTabularData, "InchPound"));

  m = rt.translateWorkspace(w);

  // Get the unique object
  OutputSQLite outputSQLite = m.getUniqueModelObject<OutputSQLite>();
  EXPECT_EQ("Simple", outputSQLite.optionType());
  EXPECT_EQ("InchPound", outputSQLite.unitConversionforTabularData());
}
