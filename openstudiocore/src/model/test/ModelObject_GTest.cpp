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
  // Removed due to removal of attributes
  // Model model;

  // OptionalWorkspaceObject oObject = model.addObject(IdfObject(IddObjectType::OS_Version));
  // ASSERT_TRUE(oObject);
  // ModelObject version = oObject->cast<ModelObject>();
  // StringVector versionAttributeNames = version.attributeNames();
  // ASSERT_EQ(static_cast<unsigned>(3),versionAttributeNames.size());
  // EXPECT_EQ("iddObjectType",versionAttributeNames[0]);
  // EXPECT_EQ("handle",versionAttributeNames[1]);
  // EXPECT_EQ("name",versionAttributeNames[2]);

  // EXPECT_FALSE(version.getAttribute("N a m e"));
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
