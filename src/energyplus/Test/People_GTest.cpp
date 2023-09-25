/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/People_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_People) {
  Model m;

  ThermalZone zone(m);

  Space space(m);
  space.setThermalZone(zone);

  PeopleDefinition pd(m);
  EXPECT_TRUE(pd.setSpaceFloorAreaperPerson(10.0));
  EXPECT_TRUE(pd.setThermalComfortModelType(0, "Fanger"));
  EXPECT_TRUE(pd.setThermalComfortModelType(1, "Pierce"));
  EXPECT_TRUE(pd.setFractionRadiant(0.35));
  EXPECT_TRUE(pd.setSensibleHeatFraction(0.2));
  EXPECT_TRUE(pd.setCarbonDioxideGenerationRate(3.6e-8));
  EXPECT_TRUE(pd.setEnableASHRAE55ComfortWarnings(true));
  EXPECT_TRUE(pd.setMeanRadiantTemperatureCalculationType("SurfaceWeighted"));

  People p(pd);
  EXPECT_TRUE(p.setSpace(space));

  {
    ScheduleConstant numPeopleSch(m);
    numPeopleSch.setName("NumberofPeopleSchedule");
    numPeopleSch.setValue(1);
    EXPECT_TRUE(p.setNumberofPeopleSchedule(numPeopleSch));
  }
  {
    ScheduleConstant activitySch(m);
    activitySch.setName("ActivitySchedule");
    activitySch.setValue(131.8);
    EXPECT_TRUE(p.setActivityLevelSchedule(activitySch));
  }
  {

    ScheduleConstant workEffSch(m);
    workEffSch.setName("WorkEfficiencySchedule");
    workEffSch.setValue(0.0);
    EXPECT_TRUE(p.setWorkEfficiencySchedule(workEffSch));
  }
  {
    ScheduleConstant cloSch(m);
    cloSch.setName("ClothingInsulationSchedule");
    cloSch.setValue(1.0);
    EXPECT_TRUE(p.setClothingInsulationSchedule(cloSch));
  }

  {
    ScheduleConstant airSch(m);
    airSch.setName("AirVelocitySchedule");
    airSch.setValue(0.137);
    EXPECT_TRUE(p.setAirVelocitySchedule(airSch));
  }

  EXPECT_TRUE(p.setMultiplier(2));
  {
    ScheduleConstant ankleSch(m);
    ankleSch.setName("AnkleLevelAirVelocitySchedule");
    ankleSch.setValue(0.127);
    EXPECT_TRUE(p.setAnkleLevelAirVelocitySchedule(ankleSch));
  }

  EXPECT_TRUE(p.setColdStressTemperatureThreshold(15.0));
  EXPECT_TRUE(p.setHeatStressTemperatureThreshold(31.0));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  std::vector<WorkspaceObject> peopleObjects = workspace.getObjectsByType(IddObjectType::People);
  ASSERT_EQ(1u, peopleObjects.size());

  auto& peopleObject = peopleObjects.front();

  EXPECT_EQ("Space 1", peopleObject.getString(PeopleFields::ZoneorZoneListorSpaceorSpaceListName).get());
  EXPECT_EQ("NumberofPeopleSchedule", peopleObject.getString(PeopleFields::NumberofPeopleScheduleName).get());
  EXPECT_EQ("Area/Person", peopleObject.getString(PeopleFields::NumberofPeopleCalculationMethod).get());
  EXPECT_TRUE(peopleObject.isEmpty(PeopleFields::NumberofPeople));
  EXPECT_TRUE(peopleObject.isEmpty(PeopleFields::PeopleperFloorArea));
  EXPECT_EQ(20.0, peopleObject.getDouble(PeopleFields::FloorAreaperPerson).get());
  EXPECT_EQ(0.35, peopleObject.getDouble(PeopleFields::FractionRadiant).get());
  EXPECT_EQ(0.2, peopleObject.getDouble(PeopleFields::SensibleHeatFraction).get());
  EXPECT_EQ("ActivitySchedule", peopleObject.getString(PeopleFields::ActivityLevelScheduleName).get());
  EXPECT_EQ(3.6e-08, peopleObject.getDouble(PeopleFields::CarbonDioxideGenerationRate).get());
  EXPECT_EQ("Yes", peopleObject.getString(PeopleFields::EnableASHRAE55ComfortWarnings).get());
  EXPECT_EQ("SurfaceWeighted", peopleObject.getString(PeopleFields::MeanRadiantTemperatureCalculationType).get());
  EXPECT_TRUE(peopleObject.isEmpty(PeopleFields::SurfaceName_AngleFactorListName));
  EXPECT_EQ("WorkEfficiencySchedule", peopleObject.getString(PeopleFields::WorkEfficiencyScheduleName).get());

  EXPECT_TRUE(peopleObject.isEmpty(PeopleFields::ClothingInsulationCalculationMethod));
  EXPECT_EQ("ClothingInsulationSchedule", peopleObject.getString(PeopleFields::ClothingInsulationCalculationMethod, true).get());
  EXPECT_TRUE(peopleObject.isEmpty(PeopleFields::ClothingInsulationCalculationMethodScheduleName));

  EXPECT_EQ("ClothingInsulationSchedule", peopleObject.getString(PeopleFields::ClothingInsulationScheduleName).get());
  EXPECT_EQ("AirVelocitySchedule", peopleObject.getString(PeopleFields::AirVelocityScheduleName).get());
  EXPECT_EQ("Fanger", peopleObject.getString(PeopleFields::ThermalComfortModel1Type).get());
  EXPECT_EQ("Pierce", peopleObject.getString(PeopleFields::ThermalComfortModel2Type).get());

  EXPECT_EQ("AnkleLevelAirVelocitySchedule", peopleObject.getString(PeopleFields::AnkleLevelAirVelocityScheduleName).get());
  EXPECT_EQ(15.0, peopleObject.getDouble(PeopleFields::ColdStressTemperatureThreshold).get());
  EXPECT_EQ(31.0, peopleObject.getDouble(PeopleFields::HeatStressTemperatureThreshold).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_People) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  OptionalWorkspaceObject _i_zone = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone);
  _i_zone->setName("Space1");

  OptionalWorkspaceObject _i_people = w.addObject(IdfObject(IddObjectType::People));
  ASSERT_TRUE(_i_people);

  EXPECT_TRUE(_i_people->setPointer(PeopleFields::ZoneorZoneListorSpaceorSpaceListName, _i_zone->handle()));

  auto assignSchedule = [&_i_people, &w](unsigned index, const std::string& scheduleName) {
    OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
    ASSERT_TRUE(_i_sch);

    _i_sch->setName(scheduleName);
    EXPECT_TRUE(_i_people->setPointer(index, _i_sch->handle()));
  };

  assignSchedule(PeopleFields::NumberofPeopleScheduleName, "NumberofPeopleSchedule");
  EXPECT_TRUE(_i_people->setString(PeopleFields::NumberofPeopleCalculationMethod, "Area/Person"));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::NumberofPeople, 0.0));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::PeopleperFloorArea, 0.0));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::FloorAreaperPerson, 10.0));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::FractionRadiant, 0.35));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::SensibleHeatFraction, 0.2));

  assignSchedule(PeopleFields::ActivityLevelScheduleName, "ActivitySchedule");
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::CarbonDioxideGenerationRate, 3.6e-08));
  EXPECT_TRUE(_i_people->setString(PeopleFields::EnableASHRAE55ComfortWarnings, "Yes"));
  EXPECT_TRUE(_i_people->setString(PeopleFields::MeanRadiantTemperatureCalculationType, "SurfaceWeighted"));
  EXPECT_TRUE(_i_people->setString(PeopleFields::SurfaceName_AngleFactorListName, ""));
  assignSchedule(PeopleFields::WorkEfficiencyScheduleName, "WorkEfficiencySchedule");
  EXPECT_TRUE(_i_people->setString(PeopleFields::ClothingInsulationCalculationMethod, ""));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ClothingInsulationCalculationMethodScheduleName, ""));
  assignSchedule(PeopleFields::ClothingInsulationScheduleName, "ClothingInsulationSchedule");
  assignSchedule(PeopleFields::AirVelocityScheduleName, "AirVelocitySchedule");
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel1Type, "Fanger"));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel2Type, "Pierce"));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel3Type, ""));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel4Type, ""));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel5Type, ""));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel6Type, ""));
  EXPECT_TRUE(_i_people->setString(PeopleFields::ThermalComfortModel7Type, ""));
  assignSchedule(PeopleFields::AnkleLevelAirVelocityScheduleName, "AnkleLevelAirVelocitySchedule");
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::ColdStressTemperatureThreshold, 15.0));
  EXPECT_TRUE(_i_people->setDouble(PeopleFields::HeatStressTemperatureThreshold, 31.0));

  Model m = rt.translateWorkspace(w);

  ASSERT_EQ(1, m.getConcreteModelObjects<People>().size());
  ASSERT_EQ(1, m.getConcreteModelObjects<PeopleDefinition>().size());

  auto p = m.getConcreteModelObjects<People>()[0];

  EXPECT_EQ("People 1", p.nameString());
  ASSERT_TRUE(p.space());
  ASSERT_TRUE(p.numberofPeopleSchedule());
  EXPECT_EQ("NumberofPeopleSchedule", p.numberofPeopleSchedule()->nameString());
  ASSERT_TRUE(p.activityLevelSchedule());
  EXPECT_EQ("ActivitySchedule", p.activityLevelSchedule()->nameString());
  ASSERT_TRUE(p.workEfficiencySchedule());
  EXPECT_EQ("WorkEfficiencySchedule", p.workEfficiencySchedule()->nameString());
  ASSERT_TRUE(p.clothingInsulationSchedule());
  EXPECT_EQ("ClothingInsulationSchedule", p.clothingInsulationSchedule()->nameString());
  ASSERT_TRUE(p.airVelocitySchedule());
  EXPECT_EQ("AirVelocitySchedule", p.airVelocitySchedule()->nameString());

  // E+ object doesn't have one
  EXPECT_EQ(1.0, p.multiplier());
  ASSERT_TRUE(p.ankleLevelAirVelocitySchedule());
  EXPECT_EQ("AnkleLevelAirVelocitySchedule", p.ankleLevelAirVelocitySchedule()->nameString());
  EXPECT_EQ(15.0, p.coldStressTemperatureThreshold());
  EXPECT_EQ(31.0, p.heatStressTemperatureThreshold());

  auto pd = p.definition().cast<PeopleDefinition>();
  EXPECT_EQ("Area/Person", pd.numberofPeopleCalculationMethod());
  EXPECT_FALSE(pd.numberofPeople());
  EXPECT_FALSE(pd.peopleperSpaceFloorArea());
  ASSERT_TRUE(pd.spaceFloorAreaperPerson());
  EXPECT_EQ(10.0, pd.spaceFloorAreaperPerson().get());
  EXPECT_EQ(0.35, pd.fractionRadiant());
  ASSERT_TRUE(pd.sensibleHeatFraction());
  EXPECT_EQ(0.2, pd.sensibleHeatFraction().get());
  EXPECT_EQ(3.6e-08, pd.carbonDioxideGenerationRate());
  EXPECT_TRUE(pd.enableASHRAE55ComfortWarnings());
  EXPECT_EQ("SurfaceWeighted", pd.meanRadiantTemperatureCalculationType());

  ASSERT_EQ(2, pd.numThermalComfortModelTypes());
  ASSERT_EQ(2, pd.numExtensibleGroups());
  EXPECT_EQ("Fanger", pd.getThermalComfortModelType(0).get());
  EXPECT_EQ("Pierce", pd.getThermalComfortModelType(1).get());
}
