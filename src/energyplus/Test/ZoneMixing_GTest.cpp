/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/People.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <utility>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneMixing_DifferentZones) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  constexpr double spaceFloorArea = width * width;
  constexpr double spaceVolume = spaceFloorArea * height;
  constexpr double numPeoplePerSpace = 10.0;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┼────────┤
  //   │        │        │        │        │
  //   │ Zone 1 │ Zone 1 │ Zone 2 │ Zone 2 │
  //   │ Space 1│ Space 2│ Space 3│ Space 4│
  //   │        │        │        │        │
  //   └────────┴────────┴────────┴────────┴──► x
  //  0        10       20        30       40

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space3.setXOrigin(width * 2);
  auto space4 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space4.setXOrigin(width * 3);

  ThermalZone z1(m);
  space1.setThermalZone(z1);
  space2.setThermalZone(z1);

  ThermalZone z2(m);
  space3.setThermalZone(z2);
  space4.setThermalZone(z2);

  // Add 10 People in each space
  SpaceType buildingSpaceType(m);
  buildingSpaceType.setName("buildingSpaceType");
  PeopleDefinition pd(m);
  pd.setNumberofPeople(numPeoplePerSpace);
  People p(pd);
  EXPECT_TRUE(p.setSpaceType(buildingSpaceType));
  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));

  ZoneMixing zmAbs(space3);
  zmAbs.setName("zmAbs");
  EXPECT_TRUE(zmAbs.setSourceSpace(space2));
  constexpr double m3sAbs = 0.6;
  EXPECT_TRUE(zmAbs.setDesignFlowRate(m3sAbs));

  ZoneMixing zmFloor(space2);
  zmFloor.setName("zmFloor");
  EXPECT_TRUE(zmFloor.setSourceSpace(space4));
  constexpr double m3sPerFlow = 0.001;
  constexpr double equivm3s_Floor = m3sPerFlow * spaceFloorArea;
  EXPECT_TRUE(zmFloor.setFlowRateperFloorArea(m3sPerFlow));

  ZoneMixing zmPerson(space4);
  zmPerson.setName("zmPerson");
  EXPECT_TRUE(zmPerson.setSourceSpace(space1));
  constexpr double m3sPerPerson = 0.03;
  constexpr double equivm3s_Person = m3sPerPerson * numPeoplePerSpace;
  EXPECT_TRUE(zmPerson.setFlowRateperPerson(m3sPerPerson));

  ZoneMixing zmACH(space1);
  zmACH.setName("zmACH");
  EXPECT_TRUE(zmACH.setSourceSpace(space3));
  constexpr double ach = 0.5;
  constexpr double equivm3s_ACH = ach * spaceVolume / 3600.0;
  EXPECT_TRUE(zmACH.setAirChangesperHour(ach));

  ZoneMixing zmZone(z1);
  zmZone.setName("zmZone");
  EXPECT_TRUE(zmZone.setSourceZone(z2));
  EXPECT_TRUE(zmZone.setFlowRateperFloorArea(0.008));

  // |    Zone Mixing   | Receiving |  Source | Flow/Space | Per Floor Area | Per Person |   ACH )  || Calculated Total   |
  // |       Name       |  Entity   |  Entity |   (m3/s)   |    (m3/s-m2)   |  (m3/s.p)  |  (1/hr)  || For 1 Space (m3/s) |
  // |------------------|-----------|---------|------------|----------------|------------|----------||--------------------|
  // | zmAbs            | space3    | space2  | 0.6        |                |            |          ||      0.6           |
  // | zmFloor          | space2    | space4  |            | 0.001          |            |          ||      0.1           |
  // | zmPerson         | space4    | space1  |            |                | 0.03       |          ||      0.3           |
  // | zmACH            | space1    | space3  |            |                |            | 0.5      ||      0.05          |
  // | zmZone           | **z1**    | **z2**  |            | 0.008          |            | 0.5      ||      0.05          |

  EXPECT_EQ(0.6, m3sAbs);
  EXPECT_EQ(0.1, equivm3s_Floor);
  EXPECT_EQ(0.3, equivm3s_Person);
  EXPECT_EQ(0.05, equivm3s_ACH);

  EXPECT_TRUE(zmAbs.setDeltaTemperature(0.5));

  ScheduleConstant mixingSch(m);
  mixingSch.setName("mixingSch");
  EXPECT_TRUE(zmZone.setSchedule(mixingSch));

  ScheduleConstant deltaTempSch(m);
  deltaTempSch.setName("deltaTempSch");
  EXPECT_TRUE(zmZone.setDeltaTemperatureSchedule(deltaTempSch));

  ScheduleConstant minimumReceivingTempSch(m);
  minimumReceivingTempSch.setName("minimumReceivingTempSch");
  EXPECT_TRUE(zmZone.setMinimumReceivingTemperatureSchedule(minimumReceivingTempSch));

  ScheduleConstant minimumSourceTempSch(m);
  minimumSourceTempSch.setName("minimumSourceTempSch");
  EXPECT_TRUE(zmZone.setMinimumSourceTemperatureSchedule(minimumSourceTempSch));

  ScheduleConstant minimumOutdoorTempSch(m);
  minimumOutdoorTempSch.setName("minimumOutdoorTempSch");
  EXPECT_TRUE(zmZone.setMinimumOutdoorTemperatureSchedule(minimumOutdoorTempSch));

  ScheduleConstant maximumReceivingTempSch(m);
  maximumReceivingTempSch.setName("maximumReceivingTempSch");
  EXPECT_TRUE(zmZone.setMaximumReceivingTemperatureSchedule(maximumReceivingTempSch));

  ScheduleConstant maximumSourceTempSch(m);
  maximumSourceTempSch.setName("maximumSourceTempSch");
  EXPECT_TRUE(zmZone.setMaximumSourceTemperatureSchedule(maximumSourceTempSch));

  ScheduleConstant maximumOutdoorTempSch(m);
  maximumOutdoorTempSch.setName("maximumOutdoorTempSch");
  EXPECT_TRUE(zmZone.setMaximumOutdoorTemperatureSchedule(maximumOutdoorTempSch));

  auto alwaysOnContinuousSchedule = m.alwaysOnContinuousSchedule();

  ForwardTranslator ft;
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(2, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());

    auto zms = w.getObjectsByType(IddObjectType::ZoneMixing);
    ASSERT_EQ(5, zms.size());

    for (const auto& zm : zms) {
      auto name = zm.nameString();
      auto receiving_ = zm.getTarget(ZoneMixingFields::ZoneorSpaceName);
      ASSERT_TRUE(receiving_);
      auto source_ = zm.getTarget(ZoneMixingFields::SourceZoneorSpaceName);
      ASSERT_TRUE(source_);
      auto sch_ = zm.getTarget(ZoneMixingFields::ScheduleName);
      ASSERT_TRUE(sch_);

      if (name == zmZone.nameString()) {
        EXPECT_EQ(receiving_->iddObject().type(), IddObjectType::Zone);
        EXPECT_EQ(source_->iddObject().type(), IddObjectType::Zone);
        EXPECT_EQ(z1.nameString(), receiving_->nameString());
        EXPECT_EQ(mixingSch.nameString(), sch_->nameString());
        EXPECT_EQ("Flow/Area", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DesignFlowRate));
        EXPECT_EQ(0.008, zm.getDouble(ZoneMixingFields::FlowRateperFloorArea).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z2.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
        EXPECT_EQ("deltaTempSch", zm.getString(ZoneMixingFields::DeltaTemperatureScheduleName).get());
        EXPECT_EQ("minimumReceivingTempSch", zm.getString(ZoneMixingFields::MinimumReceivingTemperatureScheduleName).get());
        EXPECT_EQ("maximumReceivingTempSch", zm.getString(ZoneMixingFields::MaximumReceivingTemperatureScheduleName).get());
        EXPECT_EQ("minimumSourceTempSch", zm.getString(ZoneMixingFields::MinimumSourceTemperatureScheduleName).get());
        EXPECT_EQ("maximumSourceTempSch", zm.getString(ZoneMixingFields::MaximumSourceTemperatureScheduleName).get());
        EXPECT_EQ("minimumOutdoorTempSch", zm.getString(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName).get());
        EXPECT_EQ("maximumOutdoorTempSch", zm.getString(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName).get());
        continue;
      }

      EXPECT_EQ(receiving_->iddObject().type(), IddObjectType::Space);
      EXPECT_EQ(source_->iddObject().type(), IddObjectType::Space);
      EXPECT_EQ(alwaysOnContinuousSchedule.nameString(), sch_->nameString());

      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumReceivingTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumReceivingTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumSourceTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumSourceTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName));
      if (name == zmAbs.nameString()) {
        EXPECT_EQ(space3.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_EQ(m3sAbs, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(space2.nameString(), source_->nameString());
        EXPECT_EQ(0.5, zm.getDouble(ZoneMixingFields::DeltaTemperature).get());
      } else if (name == zmFloor.nameString()) {
        EXPECT_EQ(space2.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Area", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DesignFlowRate));
        EXPECT_EQ(0.001, zm.getDouble(ZoneMixingFields::FlowRateperFloorArea).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(space4.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      } else if (name == zmPerson.nameString()) {
        EXPECT_EQ(space4.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Person", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DesignFlowRate));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_EQ(0.03, zm.getDouble(ZoneMixingFields::FlowRateperPerson).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(space1.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      } else if (name == zmACH.nameString()) {
        EXPECT_EQ(space1.nameString(), receiving_->nameString());
        EXPECT_EQ("AirChanges/Hour", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DesignFlowRate));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_EQ(0.5, zm.getDouble(ZoneMixingFields::AirChangesperHour).get());
        EXPECT_EQ(space3.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      }
    }
  }

  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(2, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());

    auto zms = w.getObjectsByType(IddObjectType::ZoneMixing);
    ASSERT_EQ(5, zms.size());

    for (const auto& zm : zms) {
      auto name = zm.nameString();
      auto receiving_ = zm.getTarget(ZoneMixingFields::ZoneorSpaceName);
      ASSERT_TRUE(receiving_);
      auto source_ = zm.getTarget(ZoneMixingFields::SourceZoneorSpaceName);
      ASSERT_TRUE(source_);
      auto sch_ = zm.getTarget(ZoneMixingFields::ScheduleName);
      ASSERT_TRUE(sch_);

      EXPECT_EQ(receiving_->iddObject().type(), IddObjectType::Zone);
      EXPECT_EQ(source_->iddObject().type(), IddObjectType::Zone);

      if (name == zmZone.nameString()) {
        EXPECT_EQ(z1.nameString(), receiving_->nameString());
        EXPECT_EQ(mixingSch.nameString(), sch_->nameString());
        EXPECT_EQ("Flow/Area", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DesignFlowRate));
        EXPECT_EQ(0.008, zm.getDouble(ZoneMixingFields::FlowRateperFloorArea).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z2.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
        EXPECT_EQ("deltaTempSch", zm.getString(ZoneMixingFields::DeltaTemperatureScheduleName).get());
        EXPECT_EQ("minimumReceivingTempSch", zm.getString(ZoneMixingFields::MinimumReceivingTemperatureScheduleName).get());
        EXPECT_EQ("maximumReceivingTempSch", zm.getString(ZoneMixingFields::MaximumReceivingTemperatureScheduleName).get());
        EXPECT_EQ("minimumSourceTempSch", zm.getString(ZoneMixingFields::MinimumSourceTemperatureScheduleName).get());
        EXPECT_EQ("maximumSourceTempSch", zm.getString(ZoneMixingFields::MaximumSourceTemperatureScheduleName).get());
        EXPECT_EQ("minimumOutdoorTempSch", zm.getString(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName).get());
        EXPECT_EQ("maximumOutdoorTempSch", zm.getString(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName).get());
        continue;
      }

      EXPECT_EQ(alwaysOnContinuousSchedule.nameString(), sch_->nameString());
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumReceivingTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumReceivingTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumSourceTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumSourceTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName));
      EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName));
      if (name == zmAbs.nameString()) {
        EXPECT_EQ(z2.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_EQ(m3sAbs, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z1.nameString(), source_->nameString());
        EXPECT_EQ(0.5, zm.getDouble(ZoneMixingFields::DeltaTemperature).get());
      } else if (name == zmFloor.nameString()) {
        EXPECT_EQ(z1.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_EQ(equivm3s_Floor, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z2.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      } else if (name == zmPerson.nameString()) {
        EXPECT_EQ(z2.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_EQ(equivm3s_Person, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z1.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      } else if (name == zmACH.nameString()) {
        EXPECT_EQ(z1.nameString(), receiving_->nameString());
        EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
        EXPECT_EQ(equivm3s_ACH, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
        EXPECT_EQ(z2.nameString(), source_->nameString());
        EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneMixing_SameZone) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼
  //   │        │        │
  //   │ Zone 1 │ Zone 1 │
  //   │ Space 1│ Space 2│
  //   │        │        │        │        │
  //   └────────┴────────┴────────┴────────┴──► x
  //  0        10       20        30       40

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);

  ThermalZone z1(m);
  space1.setThermalZone(z1);
  space2.setThermalZone(z1);

  ZoneMixing zmAbs(space1);
  zmAbs.setName("zmAbs");
  EXPECT_TRUE(zmAbs.setSourceSpace(space2));
  constexpr double m3sAbs = 0.6;
  EXPECT_TRUE(zmAbs.setDesignFlowRate(m3sAbs));

  // |    Zone Mixing   | Receiving |  Source | Flow/Space | Per Floor Area | Per Person |   ACH )  || Calculated Total   |
  // |       Name       |  Entity   |  Entity |   (m3/s)   |    (m3/s-m2)   |  (m3/s.p)  |  (1/hr)  || For 1 Space (m3/s) |
  // |------------------|-----------|---------|------------|----------------|------------|----------||--------------------|
  // | zmAbs            | space1    | space2  | 0.6        |                |            |          ||      0.6           |

  auto alwaysOnContinuousSchedule = m.alwaysOnContinuousSchedule();

  ForwardTranslator ft;
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::Space).size());

    auto zms = w.getObjectsByType(IddObjectType::ZoneMixing);
    ASSERT_EQ(1, zms.size());
    auto& zm = zms.front();

    auto name = zm.nameString();
    auto receiving_ = zm.getTarget(ZoneMixingFields::ZoneorSpaceName);
    ASSERT_TRUE(receiving_);
    auto source_ = zm.getTarget(ZoneMixingFields::SourceZoneorSpaceName);
    ASSERT_TRUE(source_);
    auto sch_ = zm.getTarget(ZoneMixingFields::ScheduleName);
    ASSERT_TRUE(sch_);

    EXPECT_EQ(receiving_->iddObject().type(), IddObjectType::Space);
    EXPECT_EQ(source_->iddObject().type(), IddObjectType::Space);

    EXPECT_EQ(name, zmAbs.nameString());
    EXPECT_EQ(alwaysOnContinuousSchedule.nameString(), sch_->nameString());
    EXPECT_EQ(space1.nameString(), receiving_->nameString());
    EXPECT_EQ("Flow/Zone", zm.getString(ZoneMixingFields::DesignFlowRateCalculationMethod).get());
    EXPECT_EQ(m3sAbs, zm.getDouble(ZoneMixingFields::DesignFlowRate).get());
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperFloorArea));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::FlowRateperPerson));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::AirChangesperHour));
    EXPECT_EQ(space2.nameString(), source_->nameString());
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperature));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::DeltaTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumReceivingTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumReceivingTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumSourceTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumSourceTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName));
    EXPECT_TRUE(zm.isEmpty(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName));
  }

  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());

    auto zms = w.getObjectsByType(IddObjectType::ZoneMixing);
    ASSERT_EQ(0, zms.size());

    // This is problematic, we actually try to FT that object 4 TIMES
    // EXPECT_EQ(1, ft.warnings().size());
    EXPECT_GE(ft.warnings().size(), 1);
    std::string expectedWarning =
      "Object of type 'OS:ZoneMixing' and named 'zmAbs' doesn't have a Source Zone or Space, it will not be translated. As you were using "
      "Space-Level ZoneMixing, and you are not translating to Spaces, it's possible it was pointing to two spaces inside the same zone";
    auto warnings = ft.warnings();
    EXPECT_NE(warnings.cend(),
              std::find_if(warnings.cbegin(), warnings.cend(), [&expectedWarning](auto& l) { return l.logMessage() == expectedWarning; }));
  }
}
