/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneMixing.hpp"
#include "../ZoneMixing_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../PeopleDefinition.hpp"
#include "../People.hpp"
#include "../SpaceType.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneMixing) {
  Model model;
  EXPECT_NE(model.alwaysOnContinuousSchedule().handle(), model.alwaysOnDiscreteSchedule().handle());

  ThermalZone zone1(model);
  ThermalZone zone2(model);
  ZoneMixing mixing(zone1);
  EXPECT_EQ(zone1.handle(), mixing.zoneOrSpace().handle());
  EXPECT_EQ(model.alwaysOnContinuousSchedule().handle(), mixing.schedule().handle());
  EXPECT_FALSE(mixing.sourceZone());

  EXPECT_EQ(1u, zone1.zoneMixing().size());
  EXPECT_EQ(1u, zone1.supplyZoneMixing().size());
  EXPECT_EQ(0, zone1.exhaustZoneMixing().size());
  EXPECT_EQ(0, zone2.zoneMixing().size());
  EXPECT_EQ(0, zone2.supplyZoneMixing().size());
  EXPECT_EQ(0, zone2.exhaustZoneMixing().size());

  EXPECT_EQ("Flow/Zone", mixing.designFlowRateCalculationMethod());
  ASSERT_TRUE(mixing.designFlowRate());
  EXPECT_EQ(0.0, mixing.designFlowRate().get());
  EXPECT_FALSE(mixing.flowRateperFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setDesignFlowRate(100.0));
  EXPECT_EQ("Flow/Zone", mixing.designFlowRateCalculationMethod());
  ASSERT_TRUE(mixing.designFlowRate());
  EXPECT_EQ(100.0, mixing.designFlowRate().get());
  EXPECT_FALSE(mixing.flowRateperFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setFlowRateperFloorArea(100.0));
  EXPECT_EQ("Flow/Area", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  ASSERT_TRUE(mixing.flowRateperFloorArea());
  EXPECT_EQ(100.0, mixing.flowRateperFloorArea().get());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setFlowRateperPerson(100.0));
  EXPECT_EQ("Flow/Person", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  EXPECT_FALSE(mixing.flowRateperFloorArea());
  ASSERT_TRUE(mixing.flowRateperPerson());
  EXPECT_EQ(100.0, mixing.flowRateperPerson().get());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setAirChangesperHour(100.0));
  EXPECT_EQ("AirChanges/Hour", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  EXPECT_FALSE(mixing.flowRateperFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  ASSERT_TRUE(mixing.airChangesperHour());
  EXPECT_EQ(100.0, mixing.airChangesperHour().get());

  mixing.setSourceZone(zone2);
  ASSERT_TRUE(mixing.sourceZone());
  EXPECT_EQ(zone2.handle(), mixing.sourceZone().get().handle());

  EXPECT_EQ(1u, zone1.zoneMixing().size());
  EXPECT_EQ(1u, zone1.supplyZoneMixing().size());
  EXPECT_EQ(0, zone1.exhaustZoneMixing().size());
  EXPECT_EQ(1u, zone2.zoneMixing().size());
  EXPECT_EQ(0, zone2.supplyZoneMixing().size());
  EXPECT_EQ(1u, zone2.exhaustZoneMixing().size());

  EXPECT_FALSE(mixing.deltaTemperature());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumReceivingTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumReceivingTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumSourceTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumSourceTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumOutdoorTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumOutdoorTemperatureSchedule());

  ScheduleRuleset fractionalSchedule(model);
  ScheduleTypeLimits fractionalLimits(model);
  EXPECT_TRUE(fractionalLimits.setUnitType("Dimensionless"));
  fractionalLimits.setLowerLimitValue(0.0);  // needs to match schedule type registry
  fractionalLimits.setUpperLimitValue(1.0);  // needs to match schedule type registry
  EXPECT_TRUE(fractionalSchedule.setScheduleTypeLimits(fractionalLimits));

  ScheduleRuleset temperatureSchedule(model);
  ScheduleTypeLimits temperatureLimits(model);
  EXPECT_TRUE(temperatureLimits.setUnitType("Temperature"));
  EXPECT_TRUE(temperatureSchedule.setScheduleTypeLimits(temperatureLimits));

  ScheduleRuleset deltaTemperatureSchedule(model);
  ScheduleTypeLimits deltaTemperatureLimits(model);
  EXPECT_TRUE(deltaTemperatureLimits.setUnitType("DeltaTemperature"));
  EXPECT_TRUE(deltaTemperatureSchedule.setScheduleTypeLimits(deltaTemperatureLimits));

  EXPECT_FALSE(mixing.setSchedule(temperatureSchedule));
  EXPECT_FALSE(mixing.setSchedule(deltaTemperatureSchedule));
  EXPECT_TRUE(mixing.setSchedule(fractionalSchedule));
  EXPECT_EQ(fractionalSchedule.handle(), mixing.schedule().handle());

  mixing.setDeltaTemperature(10.0);
  ASSERT_TRUE(mixing.deltaTemperature());
  EXPECT_EQ(10.0, mixing.deltaTemperature().get());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());

  EXPECT_FALSE(mixing.setDeltaTemperatureSchedule(fractionalSchedule));
  EXPECT_FALSE(mixing.setDeltaTemperatureSchedule(temperatureSchedule));
  EXPECT_TRUE(mixing.setDeltaTemperatureSchedule(deltaTemperatureSchedule));
  ASSERT_TRUE(mixing.deltaTemperatureSchedule());
  EXPECT_EQ(deltaTemperatureSchedule.handle(), mixing.deltaTemperatureSchedule().get().handle());

  mixing.setDeltaTemperature(-10.0);
  ASSERT_TRUE(mixing.deltaTemperature());
  EXPECT_EQ(-10.0, mixing.deltaTemperature().get());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());

  EXPECT_FALSE(mixing.maximumReceivingTemperatureSchedule());
  EXPECT_FALSE(mixing.setMaximumReceivingTemperatureSchedule(fractionalSchedule));
  EXPECT_FALSE(mixing.setMaximumReceivingTemperatureSchedule(deltaTemperatureSchedule));
  EXPECT_TRUE(mixing.setMaximumReceivingTemperatureSchedule(temperatureSchedule));
  ASSERT_TRUE(mixing.maximumReceivingTemperatureSchedule());
  EXPECT_EQ(temperatureSchedule.handle(), mixing.maximumReceivingTemperatureSchedule().get().handle());
}

TEST_F(ModelFixture, ZoneMixing_SameZone) {
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    EXPECT_TRUE(mixing.setSourceZone(zone2));
  }
  {
    Model model;
    ThermalZone zone1(model);
    ZoneMixing mixing(zone1);
    EXPECT_FALSE(mixing.setSourceZone(zone1));
  }
}

TEST_F(ModelFixture, ZoneMixing_ZoneRemove) {
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    mixing.setSourceZone(zone2);

    EXPECT_EQ(1u, model.getConcreteModelObjects<ZoneMixing>().size());
    zone1.remove();
    EXPECT_EQ(0u, model.getConcreteModelObjects<ZoneMixing>().size());
  }
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    mixing.setSourceZone(zone2);

    EXPECT_EQ(1u, model.getConcreteModelObjects<ZoneMixing>().size());
    zone2.remove();
    EXPECT_EQ(0u, model.getConcreteModelObjects<ZoneMixing>().size());
  }
}

TEST_F(ModelFixture, ZoneMixing_CantMixSpacesAndZones) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  // constexpr double spaceFloorArea = width * width;
  // constexpr double spaceVolume = spaceFloorArea * height;
  // constexpr double oneWallArea = width * height;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┤
  //   │        │        │        │
  //   │ Zone 1 │ Zone 1 │ Zone 2 │
  //   │ Space 1│ Space 2│ Space 3│
  //   │        │        │        │
  //   └────────┴────────┴────────┴─────► x
  //  0        10       20        30

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space3.setXOrigin(width * 2);

  ThermalZone z1(m);
  space1.setThermalZone(z1);
  space2.setThermalZone(z1);

  ThermalZone z2(m);
  space3.setThermalZone(z2);

  {
    ZoneMixing zm(space1);
    // Can't mix Spaces and Zones
    EXPECT_FALSE(zm.setSourceZone(z1));
    EXPECT_FALSE(zm.sourceZoneOrSpace());
    EXPECT_FALSE(zm.sourceZone());
    EXPECT_FALSE(zm.sourceSpace());

    EXPECT_TRUE(zm.setSourceSpace(space3));
    ASSERT_TRUE(zm.sourceZoneOrSpace());
    EXPECT_EQ(space3, zm.sourceZoneOrSpace().get());
    ASSERT_TRUE(zm.sourceSpace());
    EXPECT_EQ(space3, zm.sourceSpace().get());
    EXPECT_FALSE(zm.sourceZone());
  }

  {
    ZoneMixing zm(z1);
    // Can't mix Spaces and Zones
    EXPECT_FALSE(zm.setSourceSpace(space1));
    EXPECT_FALSE(zm.sourceZoneOrSpace());
    EXPECT_FALSE(zm.sourceZone());
    EXPECT_FALSE(zm.sourceSpace());

    EXPECT_TRUE(zm.setSourceZone(z2));
    ASSERT_TRUE(zm.sourceZoneOrSpace());
    EXPECT_EQ(z2, zm.sourceZoneOrSpace().get());
    ASSERT_TRUE(zm.sourceZone());
    EXPECT_EQ(z2, zm.sourceZone().get());
    EXPECT_FALSE(zm.sourceSpace());
  }
}

TEST_F(ModelFixture, ZoneMixing_CombineSpacesDifferentZones) {

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
  EXPECT_TRUE(zmAbs.setSourceSpace(space2));
  constexpr double m3sAbs = 0.6;
  EXPECT_TRUE(zmAbs.setDesignFlowRate(m3sAbs));

  ZoneMixing zmFloor(space2);
  EXPECT_TRUE(zmFloor.setSourceSpace(space4));
  constexpr double m3sPerFlow = 0.001;
  constexpr double equivm3s_Floor = m3sPerFlow * spaceFloorArea;
  EXPECT_TRUE(zmFloor.setFlowRateperFloorArea(m3sPerFlow));

  ZoneMixing zmPerson(space4);
  EXPECT_TRUE(zmPerson.setSourceSpace(space1));
  constexpr double m3sPerPerson = 0.03;
  constexpr double equivm3s_Person = m3sPerPerson * numPeoplePerSpace;
  EXPECT_TRUE(zmPerson.setFlowRateperPerson(m3sPerPerson));

  ZoneMixing zmACH(space1);
  EXPECT_TRUE(zmACH.setSourceSpace(space3));
  constexpr double ach = 0.5;
  constexpr double equivm3s_ACH = ach * spaceVolume / 3600.0;
  EXPECT_TRUE(zmACH.setAirChangesperHour(ach));

  // |    Zone Mixing   | Receiving | Source  | Flow/Space | Per Floor Area | Per Person |   ACH )  || Calculated Total   |
  // |       Name       |   Space   |  Space  |   (m3/s)   |    (m3/s-m2)   |  (m3/s.p)  |  (1/hr)  || For 1 Space (m3/s) |
  // |------------------|-----------|---------|------------|----------------|------------|----------||--------------------|
  // | zmAbs            | space3    | space2  | 0.6        |                |            |          ||      0.6           |
  // | zmFloor          | space2    | space4  |            | 0.001          |            |          ||      0.1           |
  // | zmPerson         | space4    | space1  |            |                | 0.03       |          ||      0.3           |
  // | zmACH            | space1    | space3  |            |                |            | 0.5      ||      0.05          |

  EXPECT_EQ(0.6, m3sAbs);
  EXPECT_EQ(0.1, equivm3s_Floor);
  EXPECT_EQ(0.3, equivm3s_Person);
  EXPECT_EQ(0.05, equivm3s_ACH);

  // Combine Zone 1's space
  // Only receiving spaces trigger a hardsize, so zmFloor and zmACH should be hardsized
  auto space12_ = z1.combineSpaces();
  ASSERT_TRUE(space12_);

  EXPECT_EQ(3, m.getConcreteModelObjects<Space>().size());

  ASSERT_TRUE(zmAbs.designFlowRate());
  EXPECT_EQ(m3sAbs, zmAbs.designFlowRate().get());
  EXPECT_FALSE(zmAbs.flowRateperFloorArea());
  EXPECT_FALSE(zmAbs.flowRateperPerson());
  EXPECT_FALSE(zmAbs.airChangesperHour());
  ASSERT_TRUE(zmAbs.space());
  EXPECT_EQ(space3, zmAbs.space().get());
  EXPECT_FALSE(zmAbs.zone());
  ASSERT_TRUE(zmAbs.sourceSpace());
  EXPECT_EQ(space12_.get(), zmAbs.sourceSpace().get());
  EXPECT_FALSE(zmAbs.sourceZone());

  ASSERT_TRUE(zmFloor.designFlowRate());
  EXPECT_EQ(equivm3s_Floor, zmFloor.designFlowRate().get());
  EXPECT_FALSE(zmFloor.flowRateperFloorArea());
  EXPECT_FALSE(zmFloor.flowRateperPerson());
  EXPECT_FALSE(zmFloor.airChangesperHour());
  ASSERT_TRUE(zmFloor.space());
  EXPECT_EQ(space12_.get(), zmFloor.space().get());
  EXPECT_FALSE(zmFloor.zone());
  ASSERT_TRUE(zmFloor.sourceSpace());
  EXPECT_EQ(space4, zmFloor.sourceSpace().get());
  EXPECT_FALSE(zmFloor.sourceZone());

  EXPECT_FALSE(zmPerson.designFlowRate());
  EXPECT_FALSE(zmPerson.flowRateperFloorArea());
  ASSERT_TRUE(zmPerson.flowRateperPerson());
  EXPECT_EQ(m3sPerPerson, zmPerson.flowRateperPerson().get());
  EXPECT_FALSE(zmPerson.airChangesperHour());
  ASSERT_TRUE(zmPerson.space());
  EXPECT_EQ(space4, zmPerson.space().get());
  EXPECT_FALSE(zmPerson.zone());
  ASSERT_TRUE(zmPerson.sourceSpace());
  EXPECT_EQ(space12_.get(), zmPerson.sourceSpace().get());
  EXPECT_FALSE(zmPerson.sourceZone());

  ASSERT_TRUE(zmACH.designFlowRate());
  EXPECT_EQ(equivm3s_ACH, zmACH.designFlowRate().get());
  EXPECT_FALSE(zmACH.flowRateperFloorArea());
  EXPECT_FALSE(zmACH.flowRateperPerson());
  EXPECT_FALSE(zmACH.airChangesperHour());
  ASSERT_TRUE(zmACH.space());
  EXPECT_EQ(space12_.get(), zmACH.space().get());
  EXPECT_FALSE(zmACH.zone());
  ASSERT_TRUE(zmACH.sourceSpace());
  EXPECT_EQ(space3, zmACH.sourceSpace().get());
  EXPECT_FALSE(zmACH.sourceZone());

  auto space34_ = z2.combineSpaces();
  ASSERT_TRUE(space34_);

  EXPECT_EQ(2, m.getConcreteModelObjects<Space>().size());

  ASSERT_TRUE(zmAbs.designFlowRate());
  EXPECT_EQ(m3sAbs, zmAbs.designFlowRate().get());
  EXPECT_FALSE(zmAbs.flowRateperFloorArea());
  EXPECT_FALSE(zmAbs.flowRateperPerson());
  EXPECT_FALSE(zmAbs.airChangesperHour());
  ASSERT_TRUE(zmAbs.space());
  EXPECT_EQ(space34_.get(), zmAbs.space().get());
  EXPECT_FALSE(zmAbs.zone());
  ASSERT_TRUE(zmAbs.sourceSpace());
  EXPECT_EQ(space12_.get(), zmAbs.sourceSpace().get());
  EXPECT_FALSE(zmAbs.sourceZone());

  ASSERT_TRUE(zmFloor.designFlowRate());
  EXPECT_EQ(equivm3s_Floor, zmFloor.designFlowRate().get());
  EXPECT_FALSE(zmFloor.flowRateperFloorArea());
  EXPECT_FALSE(zmFloor.flowRateperPerson());
  EXPECT_FALSE(zmFloor.airChangesperHour());
  ASSERT_TRUE(zmFloor.space());
  EXPECT_EQ(space12_.get(), zmFloor.space().get());
  EXPECT_FALSE(zmFloor.zone());
  ASSERT_TRUE(zmFloor.sourceSpace());
  EXPECT_EQ(space34_.get(), zmFloor.sourceSpace().get());
  EXPECT_FALSE(zmFloor.sourceZone());

  ASSERT_TRUE(zmPerson.designFlowRate());
  EXPECT_EQ(equivm3s_Person, zmPerson.designFlowRate().get());
  EXPECT_FALSE(zmPerson.flowRateperFloorArea());
  EXPECT_FALSE(zmPerson.flowRateperPerson());
  EXPECT_FALSE(zmPerson.airChangesperHour());
  ASSERT_TRUE(zmPerson.space());
  EXPECT_EQ(space34_.get(), zmPerson.space().get());
  EXPECT_FALSE(zmPerson.zone());
  ASSERT_TRUE(zmPerson.sourceSpace());
  EXPECT_EQ(space12_.get(), zmPerson.sourceSpace().get());
  EXPECT_FALSE(zmPerson.sourceZone());

  ASSERT_TRUE(zmACH.designFlowRate());
  EXPECT_EQ(equivm3s_ACH, zmACH.designFlowRate().get());
  EXPECT_FALSE(zmACH.flowRateperFloorArea());
  EXPECT_FALSE(zmACH.flowRateperPerson());
  EXPECT_FALSE(zmACH.airChangesperHour());
  ASSERT_TRUE(zmACH.space());
  EXPECT_EQ(space12_.get(), zmACH.space().get());
  EXPECT_FALSE(zmACH.zone());
  ASSERT_TRUE(zmACH.sourceSpace());
  EXPECT_EQ(space34_.get(), zmACH.sourceSpace().get());
  EXPECT_FALSE(zmACH.sourceZone());
}
