/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultSubSurfaceConstructions.hpp"
#include "../../model/Construction.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SubSurfaceConstructionAssignments_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultSubSurfaceConstructions) {

  ForwardTranslator ft;

  Model m;

  DefaultConstructionSet defaultConstructionSet(m);

  defaultConstructionSet.setName("Building Default Construction Set");
  Building building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setDefaultConstructionSet(defaultConstructionSet));


  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(m);
  defaultSubSurfaceConstructions.setName("Exterior SubSurface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(defaultSubSurfaceConstructions));


  Construction fixedWindowConstruction(m);
  fixedWindowConstruction.setName("Fixed Window Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setFixedWindowConstruction(fixedWindowConstruction));

  Construction operableWindowConstruction(m);
  operableWindowConstruction.setName("Operable Window Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setOperableWindowConstruction(operableWindowConstruction));

  Construction doorConstruction(m);
  doorConstruction.setName("Door Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setDoorConstruction(doorConstruction));

  Construction glassDoorConstruction(m);
  glassDoorConstruction.setName("Glass Door Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setGlassDoorConstruction(glassDoorConstruction));

  Construction overheadDoorConstruction(m);
  overheadDoorConstruction.setName("Overhead Door Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setOverheadDoorConstruction(overheadDoorConstruction));

  Construction skylightConstruction(m);
  skylightConstruction.setName("Skylight Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setSkylightConstruction(skylightConstruction));

  Construction tubularDaylightDomeConstruction(m);
  tubularDaylightDomeConstruction.setName("Tubular Daylight Dome Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setTubularDaylightDomeConstruction(tubularDaylightDomeConstruction));

  Construction tubularDaylightDiffuserConstruction(m);
  tubularDaylightDiffuserConstruction.setName("Tubular Daylight Diffuser Construction");
  EXPECT_TRUE(defaultSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(tubularDaylightDiffuserConstruction));

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::SubSurfaceConstructionAssignments);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ(defaultSubSurfaceConstructions.nameString(), idfObject.nameString());
  EXPECT_EQ(fixedWindowConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::FixedWindowConstructionName).get());
  EXPECT_EQ(operableWindowConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::OperableWindowConstructionName).get());
  EXPECT_EQ(doorConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::DoorConstructionName).get());
  EXPECT_EQ(glassDoorConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::GlassDoorConstructionName).get());
  EXPECT_EQ(overheadDoorConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::OverheadDoorConstructionName).get());
  EXPECT_EQ(skylightConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::SkylightConstructionName).get());
  EXPECT_EQ(tubularDaylightDomeConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::TubularDaylightDomeConstructionName).get());
  EXPECT_EQ(tubularDaylightDiffuserConstruction.nameString(), idfObject.getString(SubSurfaceConstructionAssignmentsFields::TubularDaylightDiffuserConstructionName).get());
}
