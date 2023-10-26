/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../AirGap.hpp"
#include "../AirGap_Impl.hpp"
#include "../MasslessOpaqueMaterial.hpp"
#include "../MasslessOpaqueMaterial_Impl.hpp"
#include "../RoofVegetation.hpp"
#include "../RoofVegetation_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../StandardsInformationMaterial.hpp"
#include "../StandardsInformationMaterial_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include <utilities/idd/OS_Material_RoofVegetation_FieldEnums.hxx>
#include <utilities/idd/OS_Material_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;
/*
TEST_F(ModelFixture, OpaqueMaterial_Daylighting_School_1)
{
  // translate energyplus simulation to building model
  Workspace inWorkspace(daylightingIdfFile);
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // get opaque materials, sort by type, and log usage
  OpaqueMaterialVector opaqueMaterials = model.getModelObjects<OpaqueMaterial>();
  ASSERT_FALSE(opaqueMaterials.empty());
  IdfObjectTypeLess comparator;
  std::sort(opaqueMaterials.begin(),opaqueMaterials.end(),comparator);
  LOG(Info,"List of OpaqueMaterials in Daylighting School model, each "
      << "followed by their directUseCount(), nonResourceObjectUseCount().");
  for (const OpaqueMaterial& material : opaqueMaterials) {
    unsigned absCount = material.directUseCount();
    unsigned mainCount = material.nonResourceObjectUseCount();
    std::string name = material.name().get();
    LOG(Info,material.iddObject().type() << ", '" << name << "': "
        << absCount << ", " << mainCount);
  }
}
*/
TEST_F(ModelFixture, OpaqueMaterial_AirGap_Constructors) {
  // construct from scratch
  Model model;
  AirGap airGap(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  AirGapVector airGaps = model.getConcreteModelObjects<AirGap>();
  ASSERT_EQ(static_cast<unsigned>(1), airGaps.size());
  EXPECT_TRUE(airGap == airGaps[0]);
  EXPECT_TRUE(airGap.iddObject().type() == IddObjectType::OS_Material_AirGap);

  // construct by clone
  auto modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  airGaps = modelClone.getConcreteModelObjects<AirGap>();
  ASSERT_EQ(static_cast<unsigned>(1), airGaps.size());
  airGap = airGaps[0];
  EXPECT_FALSE(airGap.model() == model);
  EXPECT_TRUE(airGap.iddObject().type() == IddObjectType::OS_Material_AirGap);
}

TEST_F(ModelFixture, OpaqueMaterial_MasslessOpaqueMaterial_Constructors) {
  // construct from scratch
  Model model;
  MasslessOpaqueMaterial masslessMaterial(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  MasslessOpaqueMaterialVector masslessMaterials = model.getConcreteModelObjects<MasslessOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), masslessMaterials.size());
  EXPECT_TRUE(masslessMaterial == masslessMaterials[0]);
  EXPECT_TRUE(masslessMaterial.iddObject().type() == IddObjectType::OS_Material_NoMass);

  // construct by clone
  auto modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  masslessMaterials = modelClone.getConcreteModelObjects<MasslessOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), masslessMaterials.size());
  masslessMaterial = masslessMaterials[0];
  EXPECT_FALSE(masslessMaterial.model() == model);
  EXPECT_TRUE(masslessMaterial.iddObject().type() == IddObjectType::OS_Material_NoMass);
}

TEST_F(ModelFixture, OpaqueMaterial_RoofVegetation_Constructors) {
  // construct from scratch
  Model model;
  RoofVegetation greenRoof(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  RoofVegetationVector greenRoofs = model.getConcreteModelObjects<RoofVegetation>();
  ASSERT_EQ(static_cast<unsigned>(1), greenRoofs.size());
  EXPECT_TRUE(greenRoof == greenRoofs[0]);
  EXPECT_TRUE(greenRoof.iddObject().type() == IddObjectType::OS_Material_RoofVegetation);

  // construct by clone
  auto modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  greenRoofs = modelClone.getConcreteModelObjects<RoofVegetation>();
  ASSERT_EQ(static_cast<unsigned>(1), greenRoofs.size());
  greenRoof = greenRoofs[0];
  EXPECT_FALSE(greenRoof.model() == model);
  EXPECT_TRUE(greenRoof.iddObject().type() == IddObjectType::OS_Material_RoofVegetation);
}

TEST_F(ModelFixture, OpaqueMaterial_StandardOpaqueMaterial_Constructors) {
  // construct from scratch
  Model model;
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  std::vector<StandardOpaqueMaterial> materials = model.getConcreteModelObjects<StandardOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), materials.size());
  EXPECT_TRUE(material == materials[0]);
  EXPECT_TRUE(material.iddObject().type() == IddObjectType::OS_Material);

  // construct by clone
  auto modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  materials = modelClone.getConcreteModelObjects<StandardOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1), materials.size());
  material = materials[0];
  EXPECT_FALSE(material.model() == model);
  EXPECT_TRUE(material.iddObject().type() == IddObjectType::OS_Material);
}

TEST_F(ModelFixture, OpaqueMaterial_StandardsInformationMaterial) {
  // construct
  Model model;

  StandardOpaqueMaterial material(model);
  EXPECT_EQ(0, model.getConcreteModelObjects<StandardsInformationMaterial>().size());

  StandardsInformationMaterial tmp = material.standardsInformation();
  EXPECT_EQ(1u, model.getConcreteModelObjects<StandardsInformationMaterial>().size());

  StandardsInformationMaterial info = material.standardsInformation();
  EXPECT_EQ(1u, model.getConcreteModelObjects<StandardsInformationMaterial>().size());
  EXPECT_EQ(toString(info.handle()), toString(tmp.handle()));

  std::vector<std::string> test;

  EXPECT_FALSE(info.materialStandard());
  EXPECT_FALSE(info.suggestedMaterialStandards().empty());
  EXPECT_FALSE(info.materialStandardSource());
  EXPECT_TRUE(info.suggestedMaterialStandardSources().empty());

  test = info.suggestedMaterialStandards();
  EXPECT_NE(test.end(), std::find(test.begin(), test.end(), "CEC Title24-2013"));
  info.setMaterialStandard("CEC Title24-2013");
  ASSERT_TRUE(info.materialStandard());
  EXPECT_EQ("CEC Title24-2013", info.materialStandard().get());

  //EXPECT_FALSE(info.suggestedMaterialStandardSources().emtpy());

  EXPECT_FALSE(info.standardsCategory());
  EXPECT_FALSE(info.suggestedStandardsCategories().empty());
  EXPECT_FALSE(info.standardsIdentifier());
  EXPECT_TRUE(info.suggestedStandardsIdentifiers().empty());

  test = info.suggestedStandardsCategories();
  EXPECT_NE(test.end(), std::find(test.begin(), test.end(), "Bldg Board and Siding"));
  info.setStandardsCategory("Bldg Board and Siding");
  ASSERT_TRUE(info.standardsCategory());
  EXPECT_EQ("Bldg Board and Siding", info.standardsCategory().get());

  EXPECT_FALSE(info.suggestedStandardsIdentifiers().empty());

  material.remove();
  EXPECT_EQ(0, model.getConcreteModelObjects<StandardsInformationMaterial>().size());
}
