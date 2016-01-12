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
#include "../Model.hpp"
#include "../ModelObject.hpp"
#include "../ModelObject_Impl.hpp"

#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../StandardsInformationConstruction.hpp"
#include "../StandardsInformationConstruction_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/core/Containers.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ModelObject_Attributes)
{
  Model model;

  OptionalWorkspaceObject oObject = model.addObject(IdfObject(IddObjectType::OS_Version));
  ASSERT_TRUE(oObject);
  ModelObject version = oObject->cast<ModelObject>();
  StringVector versionAttributeNames = version.attributeNames();
  ASSERT_EQ(static_cast<unsigned>(3),versionAttributeNames.size());
  EXPECT_EQ("iddObjectType",versionAttributeNames[0]);
  EXPECT_EQ("handle",versionAttributeNames[1]);
  EXPECT_EQ("name",versionAttributeNames[2]);

  EXPECT_FALSE(version.getAttribute("N a m e"));
}

TEST_F(ModelFixture, ModelObject_Clone_SameModel) {
  // Make model object with resource that has children
  Model original;
  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, original);
  Construction construction(original);
  surface.setConstruction(construction);
  construction.standardsInformation(); // creates object
  StandardOpaqueMaterial material(original);
  construction.setLayers(MaterialVector(1u,material));
  EXPECT_EQ(4u,original.numObjects());

  // Clone into same model -- new object with different name. resources reused.
  Surface newSurface = surface.clone(original).cast<Surface>();
  EXPECT_FALSE(newSurface == surface);
  EXPECT_EQ(5u,original.numObjects());
  EXPECT_NE(surface.name().get(),newSurface.name().get());
  ASSERT_TRUE(surface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_TRUE(surface.construction().get() == newSurface.construction().get());
  EXPECT_TRUE(newSurface.construction().get().cast<LayeredConstruction>().numLayers() == 1u);
}

TEST_F(ModelFixture, ModelObject_Clone_DifferentModel) {
  // Make model object with resource that has children
  Model original;
  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, original);
  Construction construction(original);
  surface.setConstruction(construction);
  construction.standardsInformation(); // creates object
  StandardOpaqueMaterial material(original);
  construction.setLayers(MaterialVector(1u,material));
  EXPECT_EQ(4u,original.numObjects());

  // Clone into new model -- everything added
  Model newModel;
  EXPECT_EQ(0u,newModel.numObjects());
  Surface newSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_EQ(4u,newModel.numObjects());
  EXPECT_TRUE(newModel.isMember(newSurface.handle()));

  // Clone into that model again -- object added, resource and children reused
  Surface anotherNewSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_FALSE(anotherNewSurface == newSurface);
  EXPECT_EQ(5u,newModel.numObjects());
  ASSERT_TRUE(anotherNewSurface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_TRUE(anotherNewSurface.construction().get() == newSurface.construction().get());
  EXPECT_TRUE(anotherNewSurface.construction().get().standardsInformation() == newSurface.construction().get().standardsInformation());
  EXPECT_TRUE(anotherNewSurface.construction().get().cast<LayeredConstruction>().layers() == newSurface.construction().get().cast<LayeredConstruction>().layers());

  // Change the data in the resource's child
  StandardsInformationConstructionVector stdsInfos = newModel.getModelObjects<StandardsInformationConstruction>();
  EXPECT_EQ(1u,stdsInfos.size());
  stdsInfos[0].setIntendedSurfaceType(StandardsInformationConstruction::intendedSurfaceTypeValues()[0]);

  // Clone into that model again -- everything added, except resource of resource
  anotherNewSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_FALSE(anotherNewSurface == newSurface);
  EXPECT_EQ(8u,newModel.numObjects());
  ASSERT_TRUE(anotherNewSurface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_FALSE(anotherNewSurface.construction().get() == newSurface.construction().get());
  EXPECT_FALSE(anotherNewSurface.construction().get().standardsInformation() == newSurface.construction().get().standardsInformation());
  EXPECT_TRUE(anotherNewSurface.construction().get().cast<LayeredConstruction>().layers() == newSurface.construction().get().cast<LayeredConstruction>().layers());
}
