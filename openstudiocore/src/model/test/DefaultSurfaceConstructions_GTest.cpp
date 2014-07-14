/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../Material.hpp"
#include "../Material_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultSurfaceConstructions)
{
  Model model;

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultSurfaceConstructions.floorConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.floorConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.floorConstruction()->handle());
  defaultSurfaceConstructions.resetFloorConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.floorConstruction());

  EXPECT_FALSE(defaultSurfaceConstructions.wallConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.wallConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.wallConstruction()->handle());
  defaultSurfaceConstructions.resetWallConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.wallConstruction());

  EXPECT_FALSE(defaultSurfaceConstructions.roofCeilingConstruction());
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));
  ASSERT_TRUE(defaultSurfaceConstructions.roofCeilingConstruction());
  EXPECT_EQ(construction.handle(), defaultSurfaceConstructions.roofCeilingConstruction()->handle());
  defaultSurfaceConstructions.resetRoofCeilingConstruction();
  EXPECT_FALSE(defaultSurfaceConstructions.roofCeilingConstruction());

}

TEST_F(ModelFixture, DefaultSurfaceConstructions_Clone) {
  Model model;
  Model exampleModel = model::exampleModel();
  
  unsigned numConstructions(0);
  unsigned numMaterials(0);
  for (const DefaultSurfaceConstructions& object : exampleModel.getModelObjects<DefaultSurfaceConstructions>()) {
    object.clone(model);
    for (const ResourceObject& construction : object.resources()) {
      EXPECT_TRUE(construction.optionalCast<ConstructionBase>());
      ++numConstructions;
      numMaterials += construction.resources().size();
    }
  }
  EXPECT_EQ(exampleModel.getModelObjects<DefaultSurfaceConstructions>().size(),model.getModelObjects<DefaultSurfaceConstructions>().size());
  EXPECT_EQ(numConstructions,model.getModelObjects<ConstructionBase>().size());
  EXPECT_LE(model.getModelObjects<Material>().size(),numMaterials); // some materials are reused

  model.save(toPath("./model.osm"),true);
}
