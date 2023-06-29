/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputConstructions.hpp"
#include "../../model/OutputConstructions_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_Constructions_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputConstructions) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  auto outputConstructions = m.getUniqueModelObject<OutputConstructions>();

  {
    EXPECT_TRUE(outputConstructions.setReportConstructions(false));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Constructions);
    EXPECT_EQ(0u, idfObjs.size());
  }

  {
    EXPECT_TRUE(outputConstructions.setReportMaterials(true));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Constructions);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_constructions(idfObjs[0]);

    EXPECT_EQ("Materials", idf_constructions.getString(Output_ConstructionsFields::DetailsType1).get());
    EXPECT_TRUE(idf_constructions.isEmpty(Output_ConstructionsFields::DetailsType2));
  }

  {
    EXPECT_TRUE(outputConstructions.setReportConstructions(true));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Constructions);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_constructions(idfObjs[0]);

    EXPECT_EQ("Constructions", idf_constructions.getString(Output_ConstructionsFields::DetailsType1).get());
    EXPECT_EQ("Materials", idf_constructions.getString(Output_ConstructionsFields::DetailsType2).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputConstructions) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputConstructions>());
  }

  OptionalWorkspaceObject _i_outputConstructions = w.addObject(IdfObject(IddObjectType::Output_Constructions));
  ASSERT_TRUE(_i_outputConstructions);

  // There but no keys, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputConstructions>());
  }

  _i_outputConstructions->setString(Output_ConstructionsFields::DetailsType1, "Materials");

  {
    Model m = rt.translateWorkspace(w);
    ASSERT_TRUE(m.getOptionalUniqueModelObject<OutputConstructions>());
    auto outputConstructions = m.getUniqueModelObject<OutputConstructions>();
    EXPECT_FALSE(outputConstructions.reportConstructions());
    EXPECT_TRUE(outputConstructions.reportMaterials());
  }

  _i_outputConstructions->setString(Output_ConstructionsFields::DetailsType2, "Constructions");
  {
    Model m = rt.translateWorkspace(w);
    ASSERT_TRUE(m.getOptionalUniqueModelObject<OutputConstructions>());
    auto outputConstructions = m.getUniqueModelObject<OutputConstructions>();
    EXPECT_TRUE(outputConstructions.reportConstructions());
    EXPECT_TRUE(outputConstructions.reportMaterials());
  }
}
