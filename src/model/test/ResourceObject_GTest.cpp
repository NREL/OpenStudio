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
#include "../ResourceObject.hpp"
#include "../ResourceObject_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../StandardsInformationConstruction.hpp"
#include "../StandardsInformationConstruction_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;
using namespace openstudio;

/*
TEST_F(ModelFixture, ResourceObject_Daylighting_School_1)
{
  // read in EnergyPlus files
  Workspace inWorkspace(daylightingIdfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // get resource objects, log usage
  ResourceObjectVector resources = model.getModelObjects<ResourceObject>();
  ASSERT_FALSE(resources.empty());
  LOG(Info,"List of ResourceObjects in Daylighting School model, each "
      << "followed by their directUseCount(), nonResourceObjectUseCount().");
  for (const ResourceObject& resource : resources) {
    unsigned absCount = resource.directUseCount();
    unsigned mainCount = resource.nonResourceObjectUseCount();
    OptionalString oName = resource.name();
    if (oName) {
      LOG(Info,resource.iddObject().type() << ", '" << *oName << "': "
          << absCount << ", " << mainCount);
    }
    else {
      LOG(Info,resource.iddObject().type() << ": " << absCount << ", "
          << mainCount);
    }
  }

  // purge unused resources, test usage
  IdfObjectVector removedObjects = model.purgeUnusedResourceObjects();
  ResourceObjectVector reducedResources = model.getModelObjects<ResourceObject>();
  ASSERT_TRUE(reducedResources.size() <= resources.size());
  EXPECT_TRUE(removedObjects.size() >= resources.size() - reducedResources.size());
  for (const ResourceObject& resource : reducedResources) {
    unsigned absCount = resource.directUseCount();
    unsigned mainCount = resource.nonResourceObjectUseCount();
    EXPECT_TRUE(absCount > 0);
    EXPECT_TRUE(mainCount > 0);
  }
}
*/

TEST_F(ModelFixture,ResourceObject_Clone) {
  Model original;
  Construction construction(original);
  construction.standardsInformation();
  StandardOpaqueMaterial material(original);
  bool ok = construction.setLayers(MaterialVector(1u,material));
  EXPECT_TRUE(ok);
  EXPECT_EQ(3u,original.numObjects()) << original;
  EXPECT_EQ(1u,construction.numLayers()) << construction;

  // clone into original
  Construction newConstruction = construction.clone(original).cast<Construction>();
  // self cloned
  EXPECT_FALSE(newConstruction == construction);
  EXPECT_TRUE(newConstruction.model() == construction.model());
  // child cloned
  EXPECT_EQ(2u,original.getModelObjects<StandardsInformationConstruction>().size()) << original;
  EXPECT_FALSE(newConstruction.standardsInformation() == construction.standardsInformation());
  EXPECT_EQ(2u,original.getModelObjects<StandardsInformationConstruction>().size()) << original;
  // resource not cloned
  EXPECT_EQ(1u,newConstruction.numLayers());
  EXPECT_TRUE(newConstruction.layers() == construction.layers());
  EXPECT_EQ(5u,original.numObjects()) << original;

  // clone into new model
  Model newModel;
  newConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_FALSE(newConstruction.model() == construction.model());
  EXPECT_EQ(3u,newModel.numObjects()) << newModel;
  EXPECT_EQ("Material 1",newConstruction.layers()[0].name().get());
  // clone again -- object and child added again, resource reused
  Construction anotherNewConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_FALSE(anotherNewConstruction == newConstruction);
  EXPECT_EQ(5u,newModel.numObjects()) << newModel;
  EXPECT_EQ("Material 1",anotherNewConstruction.layers()[0].name().get());
  // change resource data
  newConstruction.layers()[0].setName("Material with Changed Data");
  // clone again -- all added again
  anotherNewConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_EQ(8u,newModel.numObjects()) << newModel;
  EXPECT_EQ("Material with Changed Data",newConstruction.layers()[0].name().get());
  EXPECT_EQ("Material 1",anotherNewConstruction.layers()[0].name().get());
}
