/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IddFixture.hpp"
#include "../IddObject.hpp"
#include <utilities/idd/IddFactory.hxx>
#include "../IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../ExtensibleIndex.hpp"
#include "../../core/Containers.hpp"

#include <sstream>
#include <string>

using namespace openstudio;

TEST_F(IddFixture, IddObject_URL) {
  OptionalIddObject object1 = IddFactory::instance().getObject(IddObjectType::Schedule_File);
  ASSERT_TRUE(object1);
  EXPECT_TRUE(object1->hasURL());
  UnsignedVector vec = object1->urlFields();
  EXPECT_TRUE(vec.size() == 1);
  EXPECT_TRUE(vec[0] == 2);
}

TEST_F(IddFixture, IddObject_EqualityOperators) {
  // IddObjectProperties
  // == because same
  OptionalIddObject object1 = IddFactory::instance().getObject(IddObjectType::WindowMaterial_Blind);
  ASSERT_TRUE(object1);
  EXPECT_TRUE(object1->properties() == object1->properties());
  // != by comparison
  OptionalIddObject object2 = IddFactory::instance().getObject(IddObjectType::SurfaceProperty_ConvectionCoefficients);
  ASSERT_TRUE(object2);
  EXPECT_TRUE(object1->properties() != object2->properties());
  // deep ==
  // make explicit copy
  IddObjectProperties standaloneProperties(object1->properties());
  EXPECT_TRUE(standaloneProperties == object1->properties());
  // deep != (really here to check that is an actual, changable copy)
  ++(standaloneProperties.minFields);
  EXPECT_FALSE(standaloneProperties == object1->properties());

  // IddObject
  std::stringstream ss;
  object1->print(ss);
  OptionalIddObject object1Copy = IddObject::load(object1->name(), object1->group(), ss.str(), object1->type());
  ASSERT_TRUE(object1Copy);
  EXPECT_TRUE(*object1 == *object1Copy);
  EXPECT_TRUE(*object1 != *object2);
}

TEST_F(IddFixture, IddObject_ExtensibleIndex) {
  OptionalIddObject oio = IddFactory::instance().getObject(IddObjectType::BuildingSurface_Detailed);
  ASSERT_TRUE(oio);
  IddObject iddObj = *oio;

  ExtensibleIndex extInd(0, 0);
  unsigned ind = iddObj.index(extInd);
  EXPECT_EQ(static_cast<unsigned>(11), ind);

  extInd.field = 2;
  ind = iddObj.index(extInd);
  EXPECT_EQ(static_cast<unsigned>(13), ind);

  extInd.group = 5;
  ind = iddObj.index(extInd);
  EXPECT_EQ(static_cast<unsigned>(28), ind);

  ind = 20;
  extInd = iddObj.extensibleIndex(ind);
  EXPECT_EQ(static_cast<unsigned>(3), extInd.group);
  EXPECT_EQ(static_cast<unsigned>(0), extInd.field);

  ind = 27;
  extInd = iddObj.extensibleIndex(ind);
  EXPECT_EQ(static_cast<unsigned>(5), extInd.group);
  EXPECT_EQ(static_cast<unsigned>(1), extInd.field);
}

TEST_F(IddFixture, IddObjectVector_GetTypes) {
  IddObjectVector objects = IddFactory::instance().objects();
  IddObjectTypeVector typeVector = getIddObjectTypeVector(objects);
  EXPECT_EQ(objects.size(), typeVector.size());
  IddObjectVector roundtripObjects;
  for (const IddObjectType& type : typeVector) {
    roundtripObjects.push_back(IddFactory::instance().getObject(type).get());
  }
  EXPECT_TRUE(roundtripObjects == objects);
  roundtripObjects.insert(roundtripObjects.end(), objects.begin(), objects.end());
  typeVector = getIddObjectTypeVector(roundtripObjects);
  EXPECT_EQ(2 * objects.size(), typeVector.size());
  IddObjectTypeSet typeSet = getIddObjectTypeSet(roundtripObjects);
  EXPECT_EQ(objects.size(), typeSet.size());
}

TEST_F(IddFixture, IddObject_InsertHandleField) {
  // Test on EnergyPlus IDD object since no handles there
  // Use object with mix of A's and N's
  IddObjectType type(IddObjectType::HeatBalanceAlgorithm);
  IddObject temp = IddFactory::instance().getObject(type).get();
  std::stringstream ss;
  temp.print(ss);
  IddObject object = IddObject::load("HeatBalanceAlgorithm", "Simulation Control", ss.str()).get();
  object.insertHandleField();
  EXPECT_EQ("A1", object.getField(0).get().fieldId());
  EXPECT_EQ("A2", object.getField(1).get().fieldId());
  EXPECT_EQ("N1", object.getField(2).get().fieldId());
}

TEST_F(IddFixture, IddObject_nameField) {
  IddObjectType type(IddObjectType::NodeList);
  IddObject nodeList = IddFactory::instance().getObject(type).get();

  ASSERT_TRUE(nodeList.hasNameField());
  auto i = nodeList.nameFieldIndex();
  ASSERT_TRUE(i);
  ASSERT_EQ(0u, i.get());
}

// E+ no longer allows non-unique names, so we need to check that we enforce a strict naming policy
// To do so, we we attribute a \reference to all objects that aren't part of a reference group
// cf: https://github.com/NREL/OpenStudio/issues/3079
TEST_F(IddFixture, IddObject_uniqueNames) {

  std::vector<IddFileType> fts = {IddFileType(IddFileType::OpenStudio), IddFileType(IddFileType::EnergyPlus)};

  for (const auto& ft : fts) {
    std::vector<IddObject> objects = IddFactory::instance().getObjects(ft);
    for (const auto& obj : objects) {
      if (obj.hasNameField()) {
        std::stringstream ss;
        for (const auto& s : obj.references()) {
          ss << s << ",";
        }
        EXPECT_GE(obj.references().size(), 1) << "Failed for " << obj.name() << ", it has the following references: " << ss.str();
      }
    }
  }
}
