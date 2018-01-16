/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
/*
#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../ParentObject.hpp"
#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Zone.hpp"
#include "../Zone_Impl.hpp"
#include "../Surface.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../Lights.hpp"
#include "../LightShelf.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

//#include <utilities/idd/IddFieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace openstudio;

// this test examines the model unchanged
TEST_F(ModelFixture, ParentChildObject_Daylighting_School_1)
{
  // read in EnergyPlus files
  Workspace inWorkspace(daylightingIdfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  SimulationControl simulation = model.getUniqueModelObject<SimulationControl>();

  // simulation is highest level
  EXPECT_FALSE(simulation.parent());

  // simulation has some children
  EXPECT_FALSE(simulation.children().empty());

  Site site = model.getUniqueModelObject<Site>();

  // site is highest level
  EXPECT_FALSE(site.parent());

  // site has children
  EXPECT_FALSE(site.children().empty());

  Facility facility = model.getUniqueModelObject<Facility>();

  // facility is highest level
  EXPECT_FALSE(facility.parent());

  // facility child is building
  Building building = model.getUniqueModelObject<Building>();
  ASSERT_EQ(static_cast<unsigned>(1), facility.children().size());
  EXPECT_EQ(building.handle(), facility.children()[0].handle());
  ASSERT_TRUE(building.parent());
  EXPECT_EQ(building.parent()->handle(), facility.handle());

  // find building shades
  ShadingSurfaceGroupVector buildingShades = building.shadingSurfaceGroups();
  ASSERT_EQ(static_cast<unsigned>(1), buildingShades.size());
  ASSERT_TRUE(buildingShades[0].parent());
  EXPECT_EQ(buildingShades[0].parent()->handle(), building.handle());

  // check that building children includes buildingShades
  bool found = false;
  for (ModelObject obj : building.children()){
    OptionalParentObject parent = obj.optionalCast<ParentObject>();
    if (parent){
      if (parent->handle() == buildingShades[0].handle()){
        found = true;
      }
    }
  }

  // one building shade in it's own group
  ASSERT_EQ(static_cast<unsigned>(1), buildingShades.size());
  ASSERT_EQ(static_cast<unsigned>(1), buildingShades[0].shadingSurfaces().size());
  ASSERT_TRUE(buildingShades[0].shadingSurfaces()[0].name());
  EXPECT_EQ("Adjacent Classroom", buildingShades[0].shadingSurfaces()[0].name().get());

  // one zone model
  ASSERT_EQ(static_cast<unsigned>(1), building.zones().size());
  Zone zone = building.zones()[0];
  ASSERT_TRUE(zone.parent());
  EXPECT_EQ(zone.parent()->handle(), building.handle());

  // check surfaces
  EXPECT_EQ(static_cast<unsigned>(7), zone.surfaces().size()); // does not includes surface in light shelf

  // check interior surface partition groups
  ASSERT_EQ(static_cast<unsigned>(1), zone.interiorPartitionSurfaceGroups().size()); // one for light shelf
  ASSERT_EQ(static_cast<unsigned>(1), zone.interiorPartitionSurfaceGroups()[0].interiorPartitionSurfaces().size()); // one for light shelf
  ASSERT_TRUE(zone.interiorPartitionSurfaceGroups()[0].zone());
  EXPECT_EQ(zone.handle(), zone.interiorPartitionSurfaceGroups()[0].zone()->handle());

  // check exterior shading groups
  ASSERT_EQ(static_cast<unsigned>(1), zone.shadingSurfaceGroups().size()); // one for the outside only light shelf
  ASSERT_TRUE(zone.shadingSurfaceGroups()[0].zone());
  EXPECT_EQ(zone.handle(), zone.shadingSurfaceGroups()[0].zone()->handle());
}

// change the parent
TEST_F(ModelFixture, ParentChildObject_SetParent)
{
  // read in EnergyPlus files
  Workspace inWorkspace(daylightingIdfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // one zone model
  ZoneVector zones = model.getModelObjects<Zone>();
  ASSERT_EQ(static_cast<unsigned>(1), zones.size());

  // get lights
  LightsVector lights = zones[0].lights();
  ASSERT_EQ(static_cast<unsigned>(1), lights.size());
  ASSERT_TRUE(lights[0].zone());
  EXPECT_EQ(zones[0].handle(), lights[0].zone()->handle());
  ASSERT_TRUE(lights[0].parent());
  EXPECT_EQ(zones[0].handle(), lights[0].parent()->handle());

  // new zone
  Zone newZone(model);
  ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<Zone>().size());
  EXPECT_EQ(static_cast<unsigned>(0), newZone.lights().size());

  // reparent the lights
  lights[0].setParent(newZone);
  EXPECT_EQ(static_cast<unsigned>(0), zones[0].lights().size());
  EXPECT_EQ(static_cast<unsigned>(1), newZone.lights().size());
  ASSERT_TRUE(lights[0].zone());
  EXPECT_EQ(newZone.handle(), lights[0].zone()->handle());
  ASSERT_TRUE(lights[0].parent());
  EXPECT_EQ(newZone.handle(), lights[0].parent()->handle());

}

TEST_F(ModelFixture, ParentChildObject_AllowableChildTypes)
{
  // read in EnergyPlus files
  Workspace inWorkspace(daylightingIdfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // one zone model
  ZoneVector zones = model.getModelObjects<Zone>();
  ASSERT_EQ(static_cast<unsigned>(1), zones.size());

  // parent child object from the zone
  ParentObject zone(zones[0]);
  std::vector<IddObjectType> allowableChildTypes = zone.allowableChildTypes();
  EXPECT_TRUE(!allowableChildTypes.empty());

  // loop over and add an object of each type
  for (IddObjectType iddObjectType : allowableChildTypes){

    // note that if any of the following steps failed we could call remove on the object and pretend it never happened

    // get the idd object name for populating a context menu
    OptionalIddObject iddObject = model.iddFile().getObject(iddObjectType);
    ASSERT_TRUE(iddObject);
    EXPECT_TRUE(!iddObject->name().empty());

    // make idf object from idd type
    IdfObject idfObject(iddObjectType);

    // add to workspace
    OptionalWorkspaceObject w = model.addObject(idfObject);
    ASSERT_TRUE(w);

    // get model object
    openstudio::Handle h = w->handle();
    OptionalModelObject modelObject = model.getModelObject<ModelObject>(h);
    ASSERT_TRUE(modelObject);

    // set parent on this object
    EXPECT_TRUE(modelObject->setParent(zone));

    // check that parent is correct
    OptionalParentObject parent = modelObject->parent();
    ASSERT_TRUE(parent);
    EXPECT_EQ(zone.handle(), parent->handle());
  }

  // parent child object from the building
  ParentObject building = model.getUniqueModelObject<Building>();
  allowableChildTypes = building.allowableChildTypes();
  EXPECT_TRUE(!allowableChildTypes.empty());

  // loop over and add an object of each type
  for (IddObjectType iddObjectType : allowableChildTypes){

    // note that if any of the following steps failed we could call remove on the object and pretend it never happened

    // get the idd object name for populating a context menu
    OptionalIddObject iddObject = model.iddFile().getObject(iddObjectType);
    ASSERT_TRUE(iddObject);
    EXPECT_TRUE(!iddObject->name().empty());

    // make idf object from idd type
    IdfObject idfObject(iddObjectType);

    // add to workspace
    OptionalWorkspaceObject w = model.addObject(idfObject);
    ASSERT_TRUE(w);

    // get model object
    openstudio::Handle h = w->handle();
    OptionalModelObject modelObject = model.getModelObject<ModelObject>(h);
    ASSERT_TRUE(modelObject);

    // convert child to parent child object
    OptionalParentObject child = modelObject->optionalCast<ParentObject>();
    ASSERT_TRUE(child);

    // set parent on this object
    EXPECT_TRUE(child->setParent(building));

    // check that parent is correct
    OptionalParentObject parent = child->parent();
    ASSERT_TRUE(parent);
    EXPECT_EQ(building.handle(), parent->handle());
  }

}
*/
