/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../StandardsInformationConstruction.hpp"
#include "../StandardsInformationConstruction_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/core/Containers.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ModelObject_Clone_SameModel) {
  // Make model object with resource that has children
  Model original;
  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, original);
  Construction construction(original);
  surface.setConstruction(construction);
  construction.standardsInformation();  // creates object
  StandardOpaqueMaterial material(original);
  construction.setLayers(MaterialVector(1u, material));
  EXPECT_EQ(4u, original.numObjects());

  // Clone into same model -- new object with different name. resources reused.
  auto newSurface = surface.clone(original).cast<Surface>();
  EXPECT_FALSE(newSurface == surface);
  EXPECT_EQ(5u, original.numObjects());
  EXPECT_NE(surface.name().get(), newSurface.name().get());
  ASSERT_TRUE(surface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_TRUE(surface.construction().get() == newSurface.construction().get());
  EXPECT_TRUE(newSurface.construction().get().cast<LayeredConstruction>().numLayers() == 1u);
}

TEST_F(ModelFixture, ModelObject_Clone_DifferentModel) {
  // Make model object with resource that has children
  Model original;
  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, original);
  Construction construction(original);
  surface.setConstruction(construction);
  construction.standardsInformation();  // creates object
  StandardOpaqueMaterial material(original);
  construction.setLayers(MaterialVector(1u, material));
  EXPECT_EQ(4u, original.numObjects());

  // Clone into new model -- everything added
  Model newModel;
  EXPECT_EQ(0u, newModel.numObjects());
  auto newSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_EQ(4u, newModel.numObjects());
  EXPECT_TRUE(newModel.isMember(newSurface.handle()));

  // Clone into that model again -- object added, resource and children reused
  auto anotherNewSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_FALSE(anotherNewSurface == newSurface);
  EXPECT_EQ(5u, newModel.numObjects());
  ASSERT_TRUE(anotherNewSurface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_TRUE(anotherNewSurface.construction().get() == newSurface.construction().get());
  EXPECT_TRUE(anotherNewSurface.construction().get().standardsInformation() == newSurface.construction().get().standardsInformation());
  EXPECT_TRUE(anotherNewSurface.construction().get().cast<LayeredConstruction>().layers()
              == newSurface.construction().get().cast<LayeredConstruction>().layers());

  // Change the data in the resource's child
  StandardsInformationConstructionVector stdsInfos = newModel.getConcreteModelObjects<StandardsInformationConstruction>();
  EXPECT_EQ(1u, stdsInfos.size());
  stdsInfos[0].setIntendedSurfaceType(StandardsInformationConstruction::intendedSurfaceTypeValues()[0]);

  // Clone into that model again -- everything added, except resource of resource
  anotherNewSurface = surface.clone(newModel).cast<Surface>();
  EXPECT_FALSE(anotherNewSurface == newSurface);
  EXPECT_EQ(8u, newModel.numObjects());
  ASSERT_TRUE(anotherNewSurface.construction());
  ASSERT_TRUE(newSurface.construction());
  EXPECT_FALSE(anotherNewSurface.construction().get() == newSurface.construction().get());
  EXPECT_FALSE(anotherNewSurface.construction().get().standardsInformation() == newSurface.construction().get().standardsInformation());
  EXPECT_TRUE(anotherNewSurface.construction().get().cast<LayeredConstruction>().layers()
              == newSurface.construction().get().cast<LayeredConstruction>().layers());
}

TEST_F(ModelFixture, ModelObject_SetString) {
  Model model;
  Space space1(model);
  Space space2(model);
  unsigned nameIndex = 1;

  EXPECT_TRUE(space1.setString(nameIndex, "Space 1"));
  ASSERT_TRUE(space1.getString(nameIndex));
  EXPECT_EQ("Space 1", space1.getString(nameIndex).get());

  // Set string will return true but the string will not be "Space 1"
  EXPECT_TRUE(space2.setString(nameIndex, "Space 1"));
  ASSERT_TRUE(space2.getString(nameIndex));
  EXPECT_NE("Space 1", space2.getString(nameIndex).get());
  EXPECT_EQ("Space 2", space2.getString(nameIndex).get());

  EXPECT_FALSE(space2.setString(nameIndex, ""));
  ASSERT_TRUE(space2.getString(nameIndex));
  EXPECT_NE("", space2.getString(nameIndex).get());
  EXPECT_EQ("Space 2", space2.getString(nameIndex).get());
}

TEST_F(ModelFixture, ModelObject_SpecialMembers) {

  static_assert(!std::is_trivial<ModelObject>{});
  static_assert(!std::is_pod<ModelObject>{});

  // checks if a type has a default constructor
  static_assert(!std::is_default_constructible<ModelObject>{});
  static_assert(!std::is_trivially_default_constructible<ModelObject>{});
  static_assert(!std::is_nothrow_default_constructible<ModelObject>{});

  // checks if a type has a copy constructor
  static_assert(std::is_copy_constructible<ModelObject>{});
  static_assert(!std::is_trivially_copy_constructible<ModelObject>{});
  static_assert(!std::is_nothrow_copy_constructible<ModelObject>{});

  // checks if a type can be constructed from an rvalue reference
  // Note: Types without a move constructor, but with a copy constructor that accepts const T& arguments, satisfy std::is_move_constructible.
  // Move constructors are usually noexcept, since otherwise they are unusable in any code that provides strong exception guarantee.
  static_assert(std::is_move_constructible<ModelObject>{});
  static_assert(!std::is_trivially_move_constructible<std::shared_ptr<openstudio::model::detail::ModelObject_Impl>>{});
  static_assert(!std::is_trivially_move_constructible<ModelObject>{});
  static_assert(std::is_nothrow_move_constructible<ModelObject>{});

  // checks if a type has a copy assignment operator
  static_assert(std::is_copy_assignable<ModelObject>{});
  static_assert(!std::is_trivially_copy_assignable<ModelObject>{});
  static_assert(!std::is_nothrow_copy_assignable<ModelObject>{});  // We didn't specify noexcept on the user defined one

  // checks if a type has a move assignment operator
  static_assert(std::is_move_assignable<ModelObject>{});
  static_assert(!std::is_trivially_move_assignable<std::shared_ptr<openstudio::model::detail::ModelObject_Impl>>{});
  static_assert(!std::is_trivially_move_assignable<ModelObject>{});
  static_assert(std::is_nothrow_move_assignable<ModelObject>{});

  // checks if a type has a non-deleted destructor
  static_assert(std::is_destructible<ModelObject>{});
  static_assert(!std::is_trivially_destructible<std::shared_ptr<openstudio::model::detail::ModelObject_Impl>>{});
  static_assert(!std::is_trivially_destructible<ModelObject>{});
  static_assert(std::is_nothrow_destructible<ModelObject>{});

  // checks if a type has a virtual destructor
  static_assert(std::has_virtual_destructor<ModelObject>{});

  // checks if objects of a type can be swapped with objects of same or different type
  static_assert(std::is_swappable<ModelObject>{});
  static_assert(std::is_nothrow_swappable<ModelObject>{});
}
