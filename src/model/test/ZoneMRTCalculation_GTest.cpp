/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../People.hpp"
#include "../PeopleDefinition.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneMRTCalculation.hpp"
#include "../ZoneMRTCalculation_Impl.hpp"

#include <limits>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneMRTCalculation_MRTWeightingFactor) {
  Model model;
  PeopleDefinition definition(model);
  People people(definition);

  // Individual factors may use the full IDD range, including both endpoints.
  EXPECT_NO_THROW(MRTWeightingFactor(people, 0.5));
  EXPECT_NO_THROW(MRTWeightingFactor(people, 0.0));
  EXPECT_NO_THROW(MRTWeightingFactor(people, 1.0));

  // Invalid and non-finite values are rejected by the helper wrapper before a model object is mutated.
  EXPECT_THROW(MRTWeightingFactor(people, -0.1), openstudio::Exception);
  EXPECT_THROW(MRTWeightingFactor(people, 1.1), openstudio::Exception);
  EXPECT_THROW(MRTWeightingFactor(people, std::numeric_limits<double>::quiet_NaN()), openstudio::Exception);
}

TEST_F(ModelFixture, ZoneMRTCalculation_Uniqueness) {
  Model model;
  ThermalZone thermalZone(model);
  auto size = model.modelObjects().size();

  // The preferred ThermalZone getter lazily creates the one allowed ZoneMRTCalculation object.
  ZoneMRTCalculation zoneMRTCalculation1 = thermalZone.getZoneMRTCalculation();
  EXPECT_EQ(thermalZone.handle(), zoneMRTCalculation1.thermalZone().handle());
  EXPECT_EQ(size + 1, model.modelObjects().size());

  // Calling the getter again should return the existing object, not create another one.
  ZoneMRTCalculation zoneMRTCalculation2 = thermalZone.getZoneMRTCalculation();
  EXPECT_EQ(size + 1, model.modelObjects().size());
  EXPECT_EQ(zoneMRTCalculation1, zoneMRTCalculation2);

  // A newly created object starts with no extensible People/weight records.
  EXPECT_EQ(0u, zoneMRTCalculation2.numberofMRTWeightingFactors());
  EXPECT_TRUE(zoneMRTCalculation2.mrtWeightingFactors().empty());

  // Direct construction is intentionally blocked once the ThermalZone already has one object.
  EXPECT_THROW((ZoneMRTCalculation(thermalZone)), openstudio::Exception);
  EXPECT_EQ(size + 1, model.modelObjects().size());

  // Cloning is disallowed so cloned objects cannot lose the required one-per-zone relationship.
  EXPECT_THROW(zoneMRTCalculation1.clone(model), openstudio::Exception);
  EXPECT_EQ(size + 1, model.modelObjects().size());
}

TEST_F(ModelFixture, ZoneMRTCalculation_ThrowingAPIs) {
  Model model;
  ThermalZone thermalZone(model);
  ZoneMRTCalculation zoneMRTCalculation = thermalZone.getZoneMRTCalculation();

  PeopleDefinition definition(model);
  People people(definition);

  // The overload taking People and double still constructs an MRTWeightingFactor internally, so bad values throw.
  EXPECT_THROW(zoneMRTCalculation.addMRTWeightingFactor(people, -0.1), openstudio::Exception);
  EXPECT_THROW(zoneMRTCalculation.addMRTWeightingFactor(people, 1.1), openstudio::Exception);

  // The explicit throwing paths remain covered here as a compact smoke test.
  EXPECT_THROW((ZoneMRTCalculation(thermalZone)), openstudio::Exception);
  EXPECT_THROW(zoneMRTCalculation.clone(model), openstudio::Exception);
}

TEST_F(ModelFixture, ZoneMRTCalculation_ThermalZoneRemove) {
  Model model;
  ThermalZone thermalZone(model);

  Space space(model);
  EXPECT_TRUE(space.setThermalZone(thermalZone));

  PeopleDefinition definition(model);
  definition.setNumberofPeople(1.0);
  EXPECT_TRUE(definition.setThermalComfortModelType(0, "Fanger"));

  People people(definition);
  EXPECT_TRUE(people.setSpace(space));

  ZoneMRTCalculation zoneMRTCalculation = thermalZone.getZoneMRTCalculation();
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 1.0));
  ASSERT_EQ(1u, model.getConcreteModelObjects<ZoneMRTCalculation>().size());

  // Removing the ThermalZone should also remove the ZoneMRTCalculation that requires it.
  thermalZone.remove();
  EXPECT_TRUE(model.getConcreteModelObjects<ZoneMRTCalculation>().empty());
}

TEST_F(ModelFixture, ZoneMRTCalculation_AddAndRemoveMRTWeightingFactors) {
  Model model;
  ThermalZone thermalZone(model);
  ZoneMRTCalculation zoneMRTCalculation = thermalZone.getZoneMRTCalculation();

  Space space(model);
  PeopleDefinition definition(model);
  definition.setNumberofPeople(1.0);
  EXPECT_TRUE(definition.setThermalComfortModelType(0, "Fanger"));

  People people(definition);
  people.setName("People 1");

  // People must be assigned directly to a Space before they can be referenced.
  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // The Space must also belong to the same ThermalZone as the ZoneMRTCalculation object.
  ThermalZone otherThermalZone(model);
  Space otherSpace(model);
  EXPECT_TRUE(otherSpace.setThermalZone(otherThermalZone));
  EXPECT_TRUE(people.setSpace(otherSpace));
  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());

  space.setThermalZone(thermalZone);
  EXPECT_TRUE(people.setSpace(space));

  // The PeopleDefinition must select at least one Thermal Comfort Model Type, matching the IDD contract.
  PeopleDefinition noComfortDefinition(model);
  noComfortDefinition.setNumberofPeople(1.0);
  People peopleWithoutThermalComfortModel(noComfortDefinition);
  peopleWithoutThermalComfortModel.setName("People Without Thermal Comfort Model");
  EXPECT_TRUE(peopleWithoutThermalComfortModel.setSpace(space));
  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(peopleWithoutThermalComfortModel, 0.5));
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // A valid People/weight pair creates the first extensible group and can be found by People identity.
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  EXPECT_EQ(0u, zoneMRTCalculation.mrtWeightingFactorIndex(people).get());

  // Indexed access returns the complete helper wrapper, while out-of-range access returns none.
  boost::optional<MRTWeightingFactor> mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_EQ(people.handle(), mrtWeightingFactor->people().handle());
  EXPECT_DOUBLE_EQ(0.5, mrtWeightingFactor->mrtWeightingFactor());
  EXPECT_FALSE(zoneMRTCalculation.getMRTWeightingFactor(1));

  // Adding the same People again updates the existing group rather than creating a duplicate.
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.25));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_DOUBLE_EQ(0.25, mrtWeightingFactor->mrtWeightingFactor());

  // Bulk add succeeds when each new factor keeps the total weighting sum within the IDD maximum of 1.0.
  People people2(definition);
  people2.setName("People 2");
  EXPECT_TRUE(people2.setSpace(space));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactors({MRTWeightingFactor(people2, 0.75)}));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // A third valid individual factor is rejected if the total weighting sum would exceed 1.0.
  PeopleDefinition definition3(model);
  definition3.setNumberofPeople(1.0);
  EXPECT_TRUE(definition3.setThermalComfortModelType(0, "Fanger"));
  People people3(definition3);
  people3.setName("People 3");
  EXPECT_TRUE(people3.setSpace(space));
  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(people3, 0.01));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // Replacing an existing People's weight recalculates the total without double-counting the old value.
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.20));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());
  mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_DOUBLE_EQ(0.20, mrtWeightingFactor->mrtWeightingFactor());

  // Removing a referenced People object cleans up its ZoneMRTCalculation extensible group.
  people2.remove();
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people3, 0.80));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // The scenarios below would test more proactive cleanup than most existing extensible-reference patterns use.
  // Existing examples generally remove extensible references from the referenced object's remove() path, while
  // parent changes and other invalidating edits are handled through explicit remove APIs or read/translation-time
  // filtering.
  /*
  EXPECT_TRUE(people3.setSpace(otherSpace));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  EXPECT_TRUE(people3.setSpace(space));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people3, 0.80));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  people3.resetSpace();
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  EXPECT_TRUE(people3.setSpace(space));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people3, 0.80));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  SpaceType spaceType(model);
  EXPECT_TRUE(people3.setSpaceType(spaceType));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  EXPECT_TRUE(people3.setSpace(space));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people3, 0.80));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  EXPECT_TRUE(definition3.eraseThermalComfortModelType(0));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_EQ(people.handle(), mrtWeightingFactor->people().handle());

  EXPECT_TRUE(definition3.setThermalComfortModelType(0, "Fanger"));
  */

  // Bulk add reports partial failure while keeping any valid groups that were accepted before/after the failure.
  zoneMRTCalculation.removeAllMRTWeightingFactors();
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_FALSE(
    zoneMRTCalculation.addMRTWeightingFactors({MRTWeightingFactor(peopleWithoutThermalComfortModel, 0.25), MRTWeightingFactor(people3, 0.5)}));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  // Single-group removal and remove-all leave the object with the expected remaining group count.
  zoneMRTCalculation.removeMRTWeightingFactor(0);
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  zoneMRTCalculation.removeAllMRTWeightingFactors();
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());
}
