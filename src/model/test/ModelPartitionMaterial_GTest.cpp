/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../InfraredTransparentMaterial.hpp"
#include "../InfraredTransparentMaterial_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

/*
TEST_F(ModelFixture, ModelPartitionMaterial_Daylighting_School_1)
{
  // translate energyplus simulation to building model
  Workspace inWorkspace(daylightingIdfFile);
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // get model partition materials, sort by type, and log usage
  ModelPartitionMaterialVector modelPartitionMaterials = model.getModelObjects<ModelPartitionMaterial>();
  EXPECT_TRUE(modelPartitionMaterials.empty());
}
*/

TEST_F(ModelFixture, ModelPartitionMaterial_InfraredTransparentMaterial_Constructors) {
  // construct from scratch
  Model model;
  InfraredTransparentMaterial infraredTransparentMaterial(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  InfraredTransparentMaterialVector infraredTransparentMaterials = model.getConcreteModelObjects<InfraredTransparentMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), infraredTransparentMaterials.size());
  EXPECT_TRUE(infraredTransparentMaterial == infraredTransparentMaterials[0]);
  EXPECT_TRUE(infraredTransparentMaterial.iddObject().type() == IddObjectType::OS_Material_InfraredTransparent);

  // construct by clone
  auto modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  infraredTransparentMaterials = modelClone.getConcreteModelObjects<InfraredTransparentMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), infraredTransparentMaterials.size());
  infraredTransparentMaterial = infraredTransparentMaterials[0];
  EXPECT_FALSE(infraredTransparentMaterial.model() == model);
  EXPECT_TRUE(infraredTransparentMaterial.iddObject().type() == IddObjectType::OS_Material_InfraredTransparent);
}
