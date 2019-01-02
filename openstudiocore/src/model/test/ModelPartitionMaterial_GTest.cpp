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

#include "../AirWallMaterial.hpp"
#include "../AirWallMaterial_Impl.hpp"
#include "../InfraredTransparentMaterial.hpp"
#include "../InfraredTransparentMaterial_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"

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
TEST_F(ModelFixture, ModelPartitionMaterial_AirWallMaterial_Constructors)
{
  // construct from scratch
  Model model;
  AirWallMaterial airWallMaterial(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  AirWallMaterialVector airWallMaterials = model.getModelObjects<AirWallMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),airWallMaterials.size());
  EXPECT_TRUE(airWallMaterial == airWallMaterials[0]);
  EXPECT_TRUE(airWallMaterial.iddObject().type() == IddObjectType::OS_Material_AirWall);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  airWallMaterials = modelClone.getModelObjects<AirWallMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),airWallMaterials.size());
  airWallMaterial = airWallMaterials[0];
  EXPECT_FALSE(airWallMaterial.model() == model);
  EXPECT_TRUE(airWallMaterial.iddObject().type() == IddObjectType::OS_Material_AirWall);
}

TEST_F(ModelFixture, ModelPartitionMaterial_AirWallMaterial_Attributes)
{
  // Removed due to removal of attributes

  // construct
  // Model model;
  // AirWallMaterial airWallMaterial (model);

  // thickness--should always be 0.0, say "settable", but set always returns false
  // ASSERT_TRUE(airWallMaterial.getAttribute("thickness"));
  // Attribute airWallThickness = airWallMaterial.getAttribute("thickness").get();
  // ASSERT_TRUE(airWallThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,airWallThickness.valueAsDouble());

  // EXPECT_FALSE(airWallMaterial.setAttribute("thickness", 0.01));

  // ASSERT_TRUE(airWallMaterial.getAttribute("thickness"));
  // airWallThickness = airWallMaterial.getAttribute("thickness").get();
  // ASSERT_TRUE(airWallThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,airWallThickness.valueAsDouble());
}

TEST_F(ModelFixture, ModelPartitionMaterial_InfraredTransparentMaterial_Constructors)
{
    // construct from scratch
  Model model;
  InfraredTransparentMaterial infraredTransparentMaterial(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.objects().size());
  InfraredTransparentMaterialVector infraredTransparentMaterials =
      model.getModelObjects<InfraredTransparentMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),infraredTransparentMaterials.size());
  EXPECT_TRUE(infraredTransparentMaterial == infraredTransparentMaterials[0]);
  EXPECT_TRUE(infraredTransparentMaterial.iddObject().type() ==
              IddObjectType::OS_Material_InfraredTransparent);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),modelClone.objects().size());
  infraredTransparentMaterials = modelClone.getModelObjects<InfraredTransparentMaterial>();
  ASSERT_EQ(static_cast<unsigned>(1),infraredTransparentMaterials.size());
  infraredTransparentMaterial = infraredTransparentMaterials[0];
  EXPECT_FALSE(infraredTransparentMaterial.model() == model);
  EXPECT_TRUE(infraredTransparentMaterial.iddObject().type() ==
              IddObjectType::OS_Material_InfraredTransparent);
}

TEST_F(ModelFixture, ModelPartitionMaterial_InfraredTransparentMaterial_Attributes)
{
  // Removed due to removal of attributes

  // construct
  // Model model;
  // InfraredTransparentMaterial infraredTransparentMaterial (model);

  // thickness--should always be 0.0, say "settable", but set always returns false
  // ASSERT_TRUE(infraredTransparentMaterial.getAttribute("thickness"));
  // Attribute infraredTransparentMaterialThickness =
  //     infraredTransparentMaterial.getAttribute("thickness").get();
  // ASSERT_TRUE(infraredTransparentMaterialThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,infraredTransparentMaterialThickness.valueAsDouble());

  // EXPECT_FALSE(infraredTransparentMaterial.setAttribute("thickness", 0.01));

  // ASSERT_TRUE(infraredTransparentMaterial.getAttribute("thickness"));
  // infraredTransparentMaterialThickness =
  //     infraredTransparentMaterial.getAttribute("thickness").get();
  // ASSERT_TRUE(infraredTransparentMaterialThickness.valueType() == AttributeValueType::Double);
  // EXPECT_DOUBLE_EQ(0.0,infraredTransparentMaterialThickness.valueAsDouble());

}
