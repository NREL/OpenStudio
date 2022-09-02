/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
