/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ModelMerger.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

unsigned setWWR(Space& space, double wwr){
  unsigned windowsAdded = 0;
  for (Surface& surface : space.surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      boost::optional<SubSurface> subSurface = surface.setWindowToWallRatio(wwr);
      if (subSurface) {
        windowsAdded += 1;
      }
    }
  }
  return windowsAdded;
}

TEST_F(ModelFixture, ModelMerger_Initial) {

  Model model1;
  Model model2;
  std::map<UUID, UUID> handleMapping;

  // first model is empty

  // second model has spaces
  
  // object#_model#
  std::vector<Point3d> floorprint1_2;
  floorprint1_2.push_back(Point3d(0, 10, 0));
  floorprint1_2.push_back(Point3d(10, 10, 0));
  floorprint1_2.push_back(Point3d(10, 0, 0));
  floorprint1_2.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_2;
  floorprint2_2.push_back(Point3d(0, 5, 0));
  floorprint2_2.push_back(Point3d(5, 5, 0));
  floorprint2_2.push_back(Point3d(5, 0, 0));
  floorprint2_2.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1_2 = Space::fromFloorPrint(floorprint1_2, 3, model2);
  ASSERT_TRUE(space1_2);
  space1_2->setName("Space 1 - Model 2");
  EXPECT_EQ(6u, space1_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_2, 0.3));

  boost::optional<Space> space2_2 = Space::fromFloorPrint(floorprint2_2, 3, model2);
  ASSERT_TRUE(space2_2);
  space2_2->setName("Space 2 - Model 2");
  EXPECT_EQ(6u, space2_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_2, 0.3));

  ThermalZone zone1_2(model2);
  zone1_2.setName("Zone 1 - Model 2");
  space1_2->setThermalZone(zone1_2);

  ThermalZone zone2_2(model2);
  zone2_2.setName("Zone 2 - Model 2");
  space2_2->setThermalZone(zone2_2);

  // pre merge tests
  EXPECT_EQ(0u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  // do merge
  ModelMerger merger;
  merger.mergeModels(model1, model2, handleMapping);

  // post merge tests
  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  boost::optional<Space> testSpace = model1.getConcreteModelObjectByName<Space>("Space 1 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 1 - Model 2", testSpace->thermalZone()->nameString());

  testSpace = model1.getConcreteModelObjectByName<Space>("Space 2 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 2 - Model 2", testSpace->thermalZone()->nameString());
}

TEST_F(ModelFixture, ModelMerger_Merge) {

  Model model1;
  Model model2;
  std::map<UUID, UUID> handleMapping;

  // first model is empty

  // second model has spaces
  
  // object#_model#
  std::vector<Point3d> floorprint1_1;
  floorprint1_1.push_back(Point3d(0, 10, 0));
  floorprint1_1.push_back(Point3d(10, 10, 0));
  floorprint1_1.push_back(Point3d(10, 0, 0));
  floorprint1_1.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_1;
  floorprint2_1.push_back(Point3d(0, 5, 0));
  floorprint2_1.push_back(Point3d(5, 5, 0));
  floorprint2_1.push_back(Point3d(5, 0, 0));
  floorprint2_1.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint1_2;
  floorprint1_2.push_back(Point3d(0, 10, 0));
  floorprint1_2.push_back(Point3d(10, 10, 0));
  floorprint1_2.push_back(Point3d(10, 0, 0));
  floorprint1_2.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_2;
  floorprint2_2.push_back(Point3d(0, 5, 0));
  floorprint2_2.push_back(Point3d(5, 5, 0));
  floorprint2_2.push_back(Point3d(5, 0, 0));
  floorprint2_2.push_back(Point3d(0, 0, 0));

  // set up model 1
  boost::optional<Space> space1_1 = Space::fromFloorPrint(floorprint1_1, 3, model1);
  ASSERT_TRUE(space1_1);
  space1_1->setName("Space 1 - Model 1");
  EXPECT_EQ(6u, space1_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_1, 0.3));

  boost::optional<Space> space2_1 = Space::fromFloorPrint(floorprint2_1, 3, model1);
  ASSERT_TRUE(space2_1);
  space2_1->setName("Space 2 - Model 1");
  EXPECT_EQ(6u, space2_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_1, 0.3));

  ThermalZone zone1_1(model1);
  zone1_1.setName("Zone 1 - Model 1");
  space1_1->setThermalZone(zone1_1);

  ThermalZone zone2_1(model1);
  zone2_1.setName("Zone 2 - Model 1");
  space2_1->setThermalZone(zone2_1);

  // set up model 2
  boost::optional<Space> space1_2 = Space::fromFloorPrint(floorprint1_2, 3, model2);
  ASSERT_TRUE(space1_2);
  space1_2->setName("Space 1 - Model 2");
  EXPECT_EQ(6u, space1_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_2, 0.3));

  boost::optional<Space> space2_2 = Space::fromFloorPrint(floorprint2_2, 3, model2);
  ASSERT_TRUE(space2_2);
  space2_2->setName("Space 2 - Model 2");
  EXPECT_EQ(6u, space2_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_2, 0.3));

  ThermalZone zone1_2(model2);
  zone1_2.setName("Zone 1 - Model 2");
  space1_2->setThermalZone(zone1_2);

  ThermalZone zone2_2(model2);
  zone2_2.setName("Zone 2 - Model 2");
  space2_2->setThermalZone(zone2_2);

  // set up handle map
  UUID space1_1_handle = space1_1->handle();
  UUID space2_1_handle = space2_1->handle();
  UUID space1_2_handle = space1_2->handle();
  UUID space2_2_handle = space2_2->handle();
  UUID zone1_1_handle = zone1_1.handle();
  UUID zone2_1_handle = zone2_1.handle();
  UUID zone1_2_handle = zone1_2.handle();
  UUID zone2_2_handle = zone2_2.handle();

  handleMapping[space1_1_handle] = space1_2_handle;
  handleMapping[space2_1_handle] = space2_2_handle;
  handleMapping[zone1_1_handle] = zone1_2_handle;
  handleMapping[zone2_1_handle] = zone2_2_handle;

  // pre merge tests
  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  // do merge
  ModelMerger merger;
  merger.mergeModels(model1, model2, handleMapping);

  // post merge tests

  // handles would be null if deleted
  EXPECT_EQ(space1_1_handle, space1_1->handle());
  EXPECT_EQ(space2_1_handle, space2_1->handle());
  EXPECT_EQ(space1_2_handle, space1_2->handle());
  EXPECT_EQ(space2_2_handle, space2_2->handle());
  EXPECT_EQ(zone1_1_handle, zone1_1.handle());
  EXPECT_EQ(zone2_1_handle, zone2_1.handle());
  EXPECT_EQ(zone1_2_handle, zone1_2.handle());
  EXPECT_EQ(zone2_2_handle, zone2_2.handle());

  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  boost::optional<Space> testSpace = model1.getConcreteModelObjectByName<Space>("Space 1 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 1 - Model 2", testSpace->thermalZone()->nameString());

  testSpace = model1.getConcreteModelObjectByName<Space>("Space 2 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 2 - Model 2", testSpace->thermalZone()->nameString());
}


TEST_F(ModelFixture, ModelMerger_Clobber) {

  Model model1;
  Model model2;
  std::map<UUID, UUID> handleMapping;

  // first model is empty

  // second model has spaces
  
  // object#_model#
  std::vector<Point3d> floorprint1_1;
  floorprint1_1.push_back(Point3d(0, 10, 0));
  floorprint1_1.push_back(Point3d(10, 10, 0));
  floorprint1_1.push_back(Point3d(10, 0, 0));
  floorprint1_1.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_1;
  floorprint2_1.push_back(Point3d(0, 5, 0));
  floorprint2_1.push_back(Point3d(5, 5, 0));
  floorprint2_1.push_back(Point3d(5, 0, 0));
  floorprint2_1.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint1_2;
  floorprint1_2.push_back(Point3d(0, 10, 0));
  floorprint1_2.push_back(Point3d(10, 10, 0));
  floorprint1_2.push_back(Point3d(10, 0, 0));
  floorprint1_2.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_2;
  floorprint2_2.push_back(Point3d(0, 5, 0));
  floorprint2_2.push_back(Point3d(5, 5, 0));
  floorprint2_2.push_back(Point3d(5, 0, 0));
  floorprint2_2.push_back(Point3d(0, 0, 0));

  // set up model 1
  boost::optional<Space> space1_1 = Space::fromFloorPrint(floorprint1_1, 3, model1);
  ASSERT_TRUE(space1_1);
  space1_1->setName("Space 1 - Model 1");
  EXPECT_EQ(6u, space1_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_1, 0.3));

  boost::optional<Space> space2_1 = Space::fromFloorPrint(floorprint2_1, 3, model1);
  ASSERT_TRUE(space2_1);
  space2_1->setName("Space 2 - Model 1");
  EXPECT_EQ(6u, space2_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_1, 0.3));

  ThermalZone zone1_1(model1);
  zone1_1.setName("Zone 1 - Model 1");
  space1_1->setThermalZone(zone1_1);

  ThermalZone zone2_1(model1);
  zone2_1.setName("Zone 2 - Model 1");
  space2_1->setThermalZone(zone2_1);

  // set up model 2
  boost::optional<Space> space1_2 = Space::fromFloorPrint(floorprint1_2, 3, model2);
  ASSERT_TRUE(space1_2);
  space1_2->setName("Space 1 - Model 2");
  EXPECT_EQ(6u, space1_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_2, 0.3));

  boost::optional<Space> space2_2 = Space::fromFloorPrint(floorprint2_2, 3, model2);
  ASSERT_TRUE(space2_2);
  space2_2->setName("Space 2 - Model 2");
  EXPECT_EQ(6u, space2_2->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_2, 0.3));

  ThermalZone zone1_2(model2);
  zone1_2.setName("Zone 1 - Model 2");
  space1_2->setThermalZone(zone1_2);

  ThermalZone zone2_2(model2);
  zone2_2.setName("Zone 2 - Model 2");
  space2_2->setThermalZone(zone2_2);

  // set up handle map
  UUID space1_1_handle = space1_1->handle();
  UUID space2_1_handle = space2_1->handle();
  UUID space1_2_handle = space1_2->handle();
  UUID space2_2_handle = space2_2->handle();
  UUID zone1_1_handle = zone1_1.handle();
  UUID zone2_1_handle = zone2_1.handle();
  UUID zone1_2_handle = zone1_2.handle();
  UUID zone2_2_handle = zone2_2.handle();

  // DLM: clober cause no handle mapping provided
  //handleMapping[space1_1_handle] = space1_2_handle;
  //handleMapping[space2_1_handle] = space2_2_handle;
  //handleMapping[zone1_1_handle] = zone1_2_handle;
  //handleMapping[zone2_1_handle] = zone2_2_handle;

  // pre merge tests
  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  // do merge
  ModelMerger merger;
  merger.mergeModels(model1, model2, handleMapping);

  // post merge tests

  // handles in model 1 are null since deleted
  EXPECT_TRUE(space1_1->handle().isNull());
  EXPECT_TRUE(space2_1->handle().isNull());
  EXPECT_EQ(space1_2_handle, space1_2->handle());
  EXPECT_EQ(space2_2_handle, space2_2->handle());
  EXPECT_TRUE(zone1_1.handle().isNull());
  EXPECT_TRUE(zone2_1.handle().isNull());
  EXPECT_EQ(zone1_2_handle, zone1_2.handle());
  EXPECT_EQ(zone2_2_handle, zone2_2.handle());

  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(2u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model2.getConcreteModelObjects<ThermalZone>().size());

  boost::optional<Space> testSpace = model1.getConcreteModelObjectByName<Space>("Space 1 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 1 - Model 2", testSpace->thermalZone()->nameString());

  testSpace = model1.getConcreteModelObjectByName<Space>("Space 2 - Model 2");
  ASSERT_TRUE(testSpace);
  EXPECT_EQ(6u, testSpace->surfaces().size());
  for (const auto& surface : testSpace->surfaces()){
    if (istringEqual(surface.surfaceType(), "Wall")){
      EXPECT_EQ(1u, surface.subSurfaces().size());
    } else{
      EXPECT_EQ(0u, surface.subSurfaces().size());
    }
  }
  ASSERT_TRUE(testSpace->thermalZone());
  EXPECT_EQ("Zone 2 - Model 2", testSpace->thermalZone()->nameString());
}

TEST_F(ModelFixture, ModelMerger_Remove) {

  Model model1;
  Model model2;
  std::map<UUID, UUID> handleMapping;

  // first model has spaces

  // second model is empty
  
  // object#_model#
  std::vector<Point3d> floorprint1_1;
  floorprint1_1.push_back(Point3d(0, 10, 0));
  floorprint1_1.push_back(Point3d(10, 10, 0));
  floorprint1_1.push_back(Point3d(10, 0, 0));
  floorprint1_1.push_back(Point3d(0, 0, 0));

  std::vector<Point3d> floorprint2_1;
  floorprint2_1.push_back(Point3d(0, 5, 0));
  floorprint2_1.push_back(Point3d(5, 5, 0));
  floorprint2_1.push_back(Point3d(5, 0, 0));
  floorprint2_1.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1_1 = Space::fromFloorPrint(floorprint1_1, 3, model1);
  ASSERT_TRUE(space1_1);
  space1_1->setName("Space 1 - Model 1");
  EXPECT_EQ(6u, space1_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space1_1, 0.3));

  boost::optional<Space> space2_1 = Space::fromFloorPrint(floorprint2_1, 3, model1);
  ASSERT_TRUE(space2_1);
  space2_1->setName("Space 2 - Model 1");
  EXPECT_EQ(6u, space2_1->surfaces().size());
  EXPECT_EQ(4u, setWWR(*space2_1, 0.3));

  ThermalZone zone1_1(model1);
  zone1_1.setName("Zone 1 - Model 1");
  space1_1->setThermalZone(zone1_1);

  ThermalZone zone2_1(model1);
  zone2_1.setName("Zone 2 - Model 1");
  space2_1->setThermalZone(zone2_1);

  // pre merge tests
  EXPECT_EQ(2u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(12u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(8u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(2u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(0u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<ThermalZone>().size());

  // do merge
  ModelMerger merger;
  merger.mergeModels(model1, model2, handleMapping);

  // post merge tests
  EXPECT_EQ(0u, model1.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(0u, model1.getConcreteModelObjects<ThermalZone>().size());

  EXPECT_EQ(0u, model2.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<SubSurface>().size());
  EXPECT_EQ(0u, model2.getConcreteModelObjects<ThermalZone>().size());
}