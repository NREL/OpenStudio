/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
  // construct
  Model model;
  AirWallMaterial airWallMaterial (model);

  // thickness--should always be 0.0, say "settable", but set always returns false
  ASSERT_TRUE(airWallMaterial.getAttribute("thickness"));
  Attribute airWallThickness = airWallMaterial.getAttribute("thickness").get();
  ASSERT_TRUE(airWallThickness.valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.0,airWallThickness.valueAsDouble());

  EXPECT_FALSE(airWallMaterial.setAttribute("thickness", 0.01));

  ASSERT_TRUE(airWallMaterial.getAttribute("thickness"));
  airWallThickness = airWallMaterial.getAttribute("thickness").get();
  ASSERT_TRUE(airWallThickness.valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.0,airWallThickness.valueAsDouble());
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
  // construct
  Model model;
  InfraredTransparentMaterial infraredTransparentMaterial (model);

  // thickness--should always be 0.0, say "settable", but set always returns false
  ASSERT_TRUE(infraredTransparentMaterial.getAttribute("thickness"));
  Attribute infraredTransparentMaterialThickness =
      infraredTransparentMaterial.getAttribute("thickness").get();
  ASSERT_TRUE(infraredTransparentMaterialThickness.valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.0,infraredTransparentMaterialThickness.valueAsDouble());

  EXPECT_FALSE(infraredTransparentMaterial.setAttribute("thickness", 0.01));

  ASSERT_TRUE(infraredTransparentMaterial.getAttribute("thickness"));
  infraredTransparentMaterialThickness =
      infraredTransparentMaterial.getAttribute("thickness").get();
  ASSERT_TRUE(infraredTransparentMaterialThickness.valueType() == AttributeValueType::Double);
  EXPECT_DOUBLE_EQ(0.0,infraredTransparentMaterialThickness.valueAsDouble());

}
