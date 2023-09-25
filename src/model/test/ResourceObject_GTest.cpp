/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

TEST_F(ModelFixture, ResourceObject_Clone) {
  Model original;
  Construction construction(original);
  construction.standardsInformation();
  StandardOpaqueMaterial material(original);
  bool ok = construction.setLayers(MaterialVector(1u, material));
  EXPECT_TRUE(ok);
  EXPECT_EQ(3u, original.numObjects()) << original;
  EXPECT_EQ(1u, construction.numLayers()) << construction;

  // clone into original
  auto newConstruction = construction.clone(original).cast<Construction>();
  // self cloned
  EXPECT_FALSE(newConstruction == construction);
  EXPECT_TRUE(newConstruction.model() == construction.model());
  // child cloned
  EXPECT_EQ(2u, original.getConcreteModelObjects<StandardsInformationConstruction>().size()) << original;
  EXPECT_FALSE(newConstruction.standardsInformation() == construction.standardsInformation());
  EXPECT_EQ(2u, original.getConcreteModelObjects<StandardsInformationConstruction>().size()) << original;
  // resource not cloned
  EXPECT_EQ(1u, newConstruction.numLayers());
  EXPECT_TRUE(newConstruction.layers() == construction.layers());
  EXPECT_EQ(5u, original.numObjects()) << original;

  // clone into new model
  Model newModel;
  newConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_FALSE(newConstruction.model() == construction.model());
  EXPECT_EQ(3u, newModel.numObjects()) << newModel;
  EXPECT_EQ("Material 1", newConstruction.layers()[0].name().get());
  // clone again -- object and child added again, resource reused
  auto anotherNewConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_FALSE(anotherNewConstruction == newConstruction);
  EXPECT_EQ(5u, newModel.numObjects()) << newModel;
  EXPECT_EQ("Material 1", anotherNewConstruction.layers()[0].name().get());
  // change resource data
  newConstruction.layers()[0].setName("Material with Changed Data");
  // clone again -- all added again
  anotherNewConstruction = construction.clone(newModel).cast<Construction>();
  EXPECT_EQ(8u, newModel.numObjects()) << newModel;
  EXPECT_EQ("Material with Changed Data", newConstruction.layers()[0].name().get());
  EXPECT_EQ("Material 1", anotherNewConstruction.layers()[0].name().get());
}
