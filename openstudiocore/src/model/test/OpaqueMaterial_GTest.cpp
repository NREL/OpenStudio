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

#include "../../utilities/data/Attribute.hpp"
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
TEST_F(ModelFixture, OpaqueMaterial_AirGap_Constructors)
{
  // construct from scratch
  Model model;
  AirGap airGap(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  AirGapVector airGaps = model.getModelObjects<AirGap>();
  ASSERT_EQ(static_cast<unsigned>(1),airGaps.size());
  EXPECT_TRUE(airGap == airGaps[0]);
  EXPECT_TRUE(airGap.iddObject().type() == IddObjectType::OS_Material_AirGap);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  airGaps = modelClone.getModelObjects<AirGap>();
  ASSERT_EQ(static_cast<unsigned>(1),airGaps.size());
  airGap = airGaps[0];
  EXPECT_FALSE(airGap.model() == model);
  EXPECT_TRUE(airGap.iddObject().type() == IddObjectType::OS_Material_AirGap);
}

TEST_F(ModelFixture, OpaqueMaterial_AirGap_Attributes)
{
  // construct
  Model model;
  AirGap airGap(model);

  // Removed due to removal of attributes
  // thickness always returns 0.0, and setter always returns false
  // ASSERT_TRUE(airGap.getAttribute("thickness"));
  // Attribute airGapThickness = airGap.getAttribute("thickness").get();
  // EXPECT_TRUE(airGapThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,airGapThickness.valueAsDouble());

  // EXPECT_FALSE(airGap.setAttribute("thickness", 0.01));

  // ASSERT_TRUE(airGap.getAttribute("thickness"));
  // airGapThickness = airGap.getAttribute("thickness").get();
  // EXPECT_TRUE(airGapThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,airGapThickness.valueAsDouble());
}

TEST_F(ModelFixture, OpaqueMaterial_MasslessOpaqueMaterial_Constructors)
{
  // construct from scratch
  Model model;
  MasslessOpaqueMaterial masslessMaterial(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  MasslessOpaqueMaterialVector masslessMaterials = model.getModelObjects<MasslessOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),masslessMaterials.size());
  EXPECT_TRUE(masslessMaterial == masslessMaterials[0]);
  EXPECT_TRUE(masslessMaterial.iddObject().type() == IddObjectType::OS_Material_NoMass);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  masslessMaterials = modelClone.getModelObjects<MasslessOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),masslessMaterials.size());
  masslessMaterial = masslessMaterials[0];
  EXPECT_FALSE(masslessMaterial.model() == model);
  EXPECT_TRUE(masslessMaterial.iddObject().type() == IddObjectType::OS_Material_NoMass);
}

TEST_F(ModelFixture, OpaqueMaterial_MasslessOpaqueMaterial_Attributes)
{
  // construct
  Model model;
  MasslessOpaqueMaterial masslessMaterial(model);

  // Removed due to removal of attributes
  // thickness always returns 0.0, and setter always returns false
  // ASSERT_TRUE(masslessMaterial.getAttribute("thickness"));
  // Attribute masslessMaterialThickness = masslessMaterial.getAttribute("thickness").get();
  // EXPECT_TRUE(masslessMaterialThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,masslessMaterialThickness.valueAsDouble());

  // EXPECT_FALSE(masslessMaterial.setAttribute("thickness", 0.01));

  // ASSERT_TRUE(masslessMaterial.getAttribute("thickness"));
  // masslessMaterialThickness = masslessMaterial.getAttribute("thickness").get();
  // EXPECT_TRUE(masslessMaterialThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,masslessMaterialThickness.valueAsDouble());
}

TEST_F(ModelFixture, OpaqueMaterial_RoofVegetation_Constructors)
{
  // construct from scratch
  Model model;
  RoofVegetation greenRoof(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  RoofVegetationVector greenRoofs = model.getModelObjects<RoofVegetation>();
  ASSERT_EQ(static_cast<unsigned>(1),greenRoofs.size());
  EXPECT_TRUE(greenRoof == greenRoofs[0]);
  EXPECT_TRUE(greenRoof.iddObject().type() == IddObjectType::OS_Material_RoofVegetation);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  greenRoofs = modelClone.getModelObjects<RoofVegetation>();
  ASSERT_EQ(static_cast<unsigned>(1),greenRoofs.size());
  greenRoof = greenRoofs[0];
  EXPECT_FALSE(greenRoof.model() == model);
  EXPECT_TRUE(greenRoof.iddObject().type() == IddObjectType::OS_Material_RoofVegetation);
}

TEST_F(ModelFixture, OpaqueMaterial_RoofVegetation_Attributes)
{
  // construct
  Model model;
  RoofVegetation roofVegetation(model);

  // Removed due to removal of attributes
  // thickness attribute equivalent to thickness field
  // ASSERT_TRUE(roofVegetation.getAttribute("thickness"));
  // Attribute roofVegetationThickness = roofVegetation.getAttribute("thickness").get();
  // EXPECT_TRUE(roofVegetationThickness.valueType() == AttributeValueType::Double);

  // // not set yet--will return default value
  // OptionalDouble oDefault = roofVegetation.iddObject().getField(OS_Material_RoofVegetationFields::Thickness).get().properties().numericDefault;
  // ASSERT_TRUE(oDefault);
  // EXPECT_DOUBLE_EQ(*oDefault,roofVegetationThickness.valueAsDouble());
  // EXPECT_FALSE(roofVegetation.getDouble(OS_Material_RoofVegetationFields::Thickness));
  // EXPECT_TRUE(roofVegetation.getDouble(OS_Material_RoofVegetationFields::Thickness,true));

  // // should be settable
  // EXPECT_TRUE(roofVegetation.setAttribute("thickness", 0.051));
  // ASSERT_TRUE(roofVegetation.getDouble(OS_Material_RoofVegetationFields::Thickness));
  // EXPECT_DOUBLE_EQ(0.051,roofVegetation.getDouble(OS_Material_RoofVegetationFields::Thickness).get());
}

TEST_F(ModelFixture, OpaqueMaterial_StandardOpaqueMaterial_Constructors)
{
  // construct from scratch
  Model model;
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  std::vector<StandardOpaqueMaterial> materials = model.getModelObjects<StandardOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),materials.size());
  EXPECT_TRUE(material == materials[0]);
  EXPECT_TRUE(material.iddObject().type() == IddObjectType::OS_Material);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  materials = modelClone.getModelObjects<StandardOpaqueMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),materials.size());
  material = materials[0];
  EXPECT_FALSE(material.model() == model);
  EXPECT_TRUE(material.iddObject().type() == IddObjectType::OS_Material);
}

TEST_F(ModelFixture, OpaqueMaterial_StandardsInformationMaterial)
{
  // construct
  Model model;

  StandardOpaqueMaterial material(model);
  EXPECT_EQ(0, model.getModelObjects<StandardsInformationMaterial>().size());

  StandardsInformationMaterial tmp = material.standardsInformation();
  EXPECT_EQ(1u, model.getModelObjects<StandardsInformationMaterial>().size());

  StandardsInformationMaterial info = material.standardsInformation();
  EXPECT_EQ(1u, model.getModelObjects<StandardsInformationMaterial>().size());
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
  EXPECT_EQ(0, model.getModelObjects<StandardsInformationMaterial>().size());
}
