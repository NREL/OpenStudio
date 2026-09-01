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
#include "../ThermalZone.hpp"
#include "../ZoneMRTCalculation.hpp"
#include "../ZoneMRTCalculation_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneMRTCalculation_MRTWeightingFactor) {
  Model model;
  PeopleDefinition definition(model);
  People people(definition);

  EXPECT_NO_THROW(MRTWeightingFactor(people, 0.5));
  EXPECT_NO_THROW(MRTWeightingFactor(people, 0.0));
  EXPECT_NO_THROW(MRTWeightingFactor(people, 1.0));
  EXPECT_THROW(MRTWeightingFactor(people, -0.1), openstudio::Exception);
  EXPECT_THROW(MRTWeightingFactor(people, 1.1), openstudio::Exception);
}

TEST_F(ModelFixture, ZoneMRTCalculation_Uniqueness) {
  Model model;
  ThermalZone thermalZone(model);
  auto size = model.modelObjects().size();

  ZoneMRTCalculation zoneMRTCalculation1 = thermalZone.getZoneMRTCalculation();
  EXPECT_EQ(thermalZone.handle(), zoneMRTCalculation1.thermalZone().handle());
  EXPECT_EQ(size + 1, model.modelObjects().size());

  ZoneMRTCalculation zoneMRTCalculation2 = thermalZone.getZoneMRTCalculation();
  EXPECT_EQ(size + 1, model.modelObjects().size());
  EXPECT_EQ(zoneMRTCalculation1, zoneMRTCalculation2);

  EXPECT_EQ(0u, zoneMRTCalculation2.numberofMRTWeightingFactors());
  EXPECT_TRUE(zoneMRTCalculation2.mrtWeightingFactors().empty());

  EXPECT_THROW((ZoneMRTCalculation(thermalZone)), openstudio::Exception);
  EXPECT_EQ(size + 1, model.modelObjects().size());

  EXPECT_THROW(zoneMRTCalculation1.clone(model), openstudio::Exception);
  EXPECT_EQ(size + 1, model.modelObjects().size());
}

TEST_F(ModelFixture, ZoneMRTCalculation_ThrowingAPIs) {
  Model model;
  ThermalZone thermalZone(model);
  ZoneMRTCalculation zoneMRTCalculation = thermalZone.getZoneMRTCalculation();

  PeopleDefinition definition(model);
  People people(definition);

  EXPECT_THROW(zoneMRTCalculation.addMRTWeightingFactor(people, -0.1), openstudio::Exception);
  EXPECT_THROW(zoneMRTCalculation.addMRTWeightingFactor(people, 1.1), openstudio::Exception);
  EXPECT_THROW((ZoneMRTCalculation(thermalZone)), openstudio::Exception);
  EXPECT_THROW(zoneMRTCalculation.clone(model), openstudio::Exception);
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

  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());

  ThermalZone otherThermalZone(model);
  Space otherSpace(model);
  EXPECT_TRUE(otherSpace.setThermalZone(otherThermalZone));
  EXPECT_TRUE(people.setSpace(otherSpace));
  EXPECT_FALSE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());

  space.setThermalZone(thermalZone);
  EXPECT_TRUE(people.setSpace(space));

  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.5));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  boost::optional<MRTWeightingFactor> mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_EQ(people.handle(), mrtWeightingFactor->people().handle());
  EXPECT_DOUBLE_EQ(0.5, mrtWeightingFactor->mrtWeightingFactor());
  EXPECT_FALSE(zoneMRTCalculation.getMRTWeightingFactor(1));

  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people, 0.25));
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  mrtWeightingFactor = zoneMRTCalculation.getMRTWeightingFactor(0);
  ASSERT_TRUE(mrtWeightingFactor);
  EXPECT_DOUBLE_EQ(0.25, mrtWeightingFactor->mrtWeightingFactor());

  People people2(definition);
  people2.setName("People 2");
  EXPECT_TRUE(people2.setSpace(space));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactors({MRTWeightingFactor(people2, 0.75)}));
  EXPECT_EQ(2u, zoneMRTCalculation.numberofMRTWeightingFactors());

  zoneMRTCalculation.removeMRTWeightingFactor(0);
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());

  zoneMRTCalculation.removeAllMRTWeightingFactors();
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());
}