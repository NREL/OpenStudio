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
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/DesignDay.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/data/Matrix.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_OutdoorAir_SpaceList_FieldEnums.hxx>
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/People_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>

#include <resources.hxx>

#include <numeric>  // std::accumulate

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DesignSpecificationOutdoorAir) {

  Model m;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┼────────┤
  //   │        │        │        │        │
  //   │        │        │        │        │
  //   │ Space 1│ Space 2│ Space 3│ Space 4│
  //   │        │        │        │        │
  //   └────────┴────────┴────────┴────────┴──► x
  //  0        10       20        30       40

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  constexpr double spaceFloorArea = width * width;
  constexpr double spaceVolume = spaceFloorArea * height;
  constexpr double numberOfPeople = 10.0;

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  // 4 spaces in a zone
  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space3.setXOrigin(width * 2);
  auto space4 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space4.setXOrigin(width * 3);

  EXPECT_EQ(spaceFloorArea, space1.floorArea());
  EXPECT_EQ(spaceFloorArea, space2.floorArea());
  EXPECT_EQ(spaceFloorArea, space3.floorArea());
  EXPECT_EQ(spaceFloorArea, space4.floorArea());
  EXPECT_EQ(spaceVolume, space1.volume());
  EXPECT_EQ(spaceVolume, space2.volume());
  EXPECT_EQ(spaceVolume, space3.volume());
  EXPECT_EQ(spaceVolume, space4.volume());

  ThermalZone z(m);
  EXPECT_TRUE(space1.setThermalZone(z));
  EXPECT_TRUE(space2.setThermalZone(z));
  EXPECT_TRUE(space3.setThermalZone(z));
  EXPECT_TRUE(space4.setThermalZone(z));

  // Assign a default space type at building-level, with its own DSOA
  SpaceType buildingSpaceType(m);
  buildingSpaceType.setName("buildingSpaceType");

  DesignSpecificationOutdoorAir dsoaBuilding(m);
  dsoaBuilding.setName("dsoaBuilding");
  EXPECT_TRUE(buildingSpaceType.setDesignSpecificationOutdoorAir(dsoaBuilding));

  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));

  // Create an Office Space Type. Assign to Space 1 & 2 only (not 3 nor 4), it also has a DSOA
  SpaceType officeSpaceType(m);
  officeSpaceType.setName("officeSpaceType");
  DesignSpecificationOutdoorAir dsoaOffice(m);
  dsoaOffice.setName("dsoaOffice");
  EXPECT_TRUE(officeSpaceType.setDesignSpecificationOutdoorAir(dsoaOffice));
  EXPECT_TRUE(space1.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space2.setSpaceType(officeSpaceType));

  // Space 1 and 3 have a space-level DSOA
  DesignSpecificationOutdoorAir dsoaSpace1(m);
  dsoaSpace1.setName("dsoaSpace1");
  EXPECT_TRUE(space1.setDesignSpecificationOutdoorAir(dsoaSpace1));

  DesignSpecificationOutdoorAir dsoaSpace3(m);
  dsoaSpace3.setName("dsoaSpace3");
  EXPECT_TRUE(space3.setDesignSpecificationOutdoorAir(dsoaSpace3));

  PeopleDefinition officePd(m);
  officePd.setName("officeSpaceType People Definition");
  EXPECT_TRUE(officePd.setPeopleperSpaceFloorArea(10.0 / spaceFloorArea));
  People officeP(officePd);
  officeP.setName("officeSpaceType People");
  EXPECT_TRUE(officeP.setSpaceType(officeSpaceType));

  EXPECT_EQ(10.0, space1.numberOfPeople());
  EXPECT_EQ(10.0, space2.numberOfPeople());
  EXPECT_EQ(0.0, space3.numberOfPeople());
  EXPECT_EQ(0.0, space4.numberOfPeople());

  // Add a Space-specific People to space1
  PeopleDefinition space1Pd(m);
  space1Pd.setName("space1 People Definition");
  EXPECT_TRUE(space1Pd.setNumberofPeople(10.0));
  People space1P(space1Pd);
  space1P.setName("space1 People");
  EXPECT_TRUE(space1P.setSpace(space1));

  EXPECT_EQ(2 * numberOfPeople, space1.numberOfPeople());
  EXPECT_EQ(numberOfPeople, space2.numberOfPeople());
  EXPECT_EQ(0.0, space3.numberOfPeople());
  EXPECT_EQ(0.0, space4.numberOfPeople());
  EXPECT_EQ(30.0, building.numberOfPeople());
  EXPECT_EQ(3 * numberOfPeople, building.numberOfPeople());

  // DSOA Characteristics
  // =====================
  //
  // |     DSOA       | Outdoor Air |   Per Person  | Per Floor Area | Absolute |   ACH  | Calculated   |
  // |     Name       |   Method    | (m3/s-person) |   (m3/s-m2)    |  (m3/s)  | (1/hr) | Total (m3/s) |
  // |----------------|-------------|---------------|----------------|----------|--------|--------------|
  // | dsoaSpace1     | Sum         | 0.015         | 0.004          |          |        | 0.55         |
  // | dsoaSpace3     | Maximum     |               |                | 0.65     | (2.0)* | 0.65         |
  // | dsoaOffice     | Maximum     |               | 0.006          | (0.40)*  |        | 0.60         |
  // | dsoaBuilding   | Sum         |               |                | 0.10     | 0.5    | 0.15         |
  //
  // (val)* : this is ignored because method is Maximum and it amounts to a rate in m3/s that is lower than the maximum component

  // Which DSOA applies to which spaces?
  // ====================================
  //
  // | Name   | Space Type                   | Space-Specic DSOA | DSOA         |
  // |--------|------------------------------|-------------------|--------------|
  // | Space1 | OfficeSpaceType              | dsoaSpace1        | dsoaSpace1   |
  // | Space2 | OfficeSpaceType              |                   | dsoaOffice   |
  // | Space3 | Inherited: buildingSpaceType | dsoaSpace3        | dsoaSpace3   |
  // | Space4 | Inherited: buildingSpaceType |                   | dsoaBuilding |

  ASSERT_TRUE(space1.designSpecificationOutdoorAir());
  EXPECT_EQ(dsoaSpace1, space1.designSpecificationOutdoorAir().get());
  ASSERT_TRUE(space2.designSpecificationOutdoorAir());
  EXPECT_EQ(dsoaOffice, space2.designSpecificationOutdoorAir().get());
  ASSERT_TRUE(space3.designSpecificationOutdoorAir());
  EXPECT_EQ(dsoaSpace3, space3.designSpecificationOutdoorAir().get());
  ASSERT_TRUE(space4.designSpecificationOutdoorAir());
  EXPECT_EQ(dsoaBuilding, space4.designSpecificationOutdoorAir().get());

  struct DsoaInfo
  {
    DesignSpecificationOutdoorAir& dsoa;
    bool methodIsMax = false;
    double perPerson = 0.0;
    double perFloorArea = 0.0;
    double absolute = 0.0;
    double ach = 0.0;
    double outdoorAirForPeople = 0.0;
    double outdoorAirForFloorArea = 0.0;
    double outdoorAirRate = 0.0;
    double outdoorAirForVolume = 0.0;
    double outdoorAirTotal = 0.0;

    DsoaInfo(DesignSpecificationOutdoorAir& t_dsoa, bool t_methodIsMax, double t_perPerson, double t_perFloorArea, double t_absolute, double t_ach)
      : dsoa(t_dsoa), methodIsMax(t_methodIsMax), perPerson(t_perPerson), perFloorArea(t_perFloorArea), absolute(t_absolute), ach(t_ach) {

      this->setDsoaParams();
    }

    void setDsoaParams() {
      double numberOfPeople = 10.0;
      if (auto sources = dsoa.getModelObjectSources<Space>(); !sources.empty()) {
        OS_ASSERT(sources.size() == 1);
        if (sources[0].nameString() == "Space 1") {
          numberOfPeople = 20.0;
        }
      }
      outdoorAirForPeople = numberOfPeople * perPerson;

      outdoorAirForFloorArea = 100.0 * perFloorArea;
      outdoorAirRate = absolute;
      outdoorAirForVolume = 3.6 * 10.0 * 10.0 * ach / 3600.0;
      if (methodIsMax) {
        dsoa.setOutdoorAirMethod("Maximum");
        outdoorAirTotal += std::max(outdoorAirForPeople, std::max(outdoorAirForFloorArea, std::max(outdoorAirRate, outdoorAirForVolume)));
      } else {
        dsoa.setOutdoorAirMethod("Sum");
        outdoorAirTotal = outdoorAirForPeople + outdoorAirForFloorArea + outdoorAirRate + outdoorAirForVolume;
      }
      dsoa.setOutdoorAirFlowperPerson(perPerson);
      dsoa.setOutdoorAirFlowperFloorArea(perFloorArea);
      dsoa.setOutdoorAirFlowRate(absolute);
      dsoa.setOutdoorAirFlowAirChangesperHour(ach);
    }
  };

  std::vector<DsoaInfo> dsoaInfos{{dsoaSpace1, false, 0.015, 0.004, 0.0, 0.0},
                                  {dsoaSpace3, true, 0.0, 0.0, 0.65, 2.0},
                                  {dsoaOffice, true, 0.0, 0.006, 0.4, 0.0},
                                  {dsoaBuilding, false, 0.0, 0.0, 0.1, 0.5}};

  for (const auto& dsoaInfo : dsoaInfos) {
    if (dsoaInfo.methodIsMax) {
      EXPECT_EQ("Maximum", dsoaInfo.dsoa.outdoorAirMethod());
    } else {
      EXPECT_EQ("Sum", dsoaInfo.dsoa.outdoorAirMethod());
    }
    EXPECT_EQ(dsoaInfo.perPerson, dsoaInfo.dsoa.outdoorAirFlowperPerson());
    EXPECT_EQ(dsoaInfo.perFloorArea, dsoaInfo.dsoa.outdoorAirFlowperFloorArea());
    EXPECT_EQ(dsoaInfo.absolute, dsoaInfo.dsoa.outdoorAirFlowRate());
    EXPECT_EQ(dsoaInfo.ach, dsoaInfo.dsoa.outdoorAirFlowAirChangesperHour());
  }

  EXPECT_DOUBLE_EQ(0.7, dsoaInfos[0].outdoorAirTotal);
  EXPECT_DOUBLE_EQ(0.65, dsoaInfos[1].outdoorAirTotal);
  EXPECT_DOUBLE_EQ(0.6, dsoaInfos[2].outdoorAirTotal);
  EXPECT_DOUBLE_EQ(0.15, dsoaInfos[3].outdoorAirTotal);
  double expectedOutdoorAirTotal =
    std::accumulate(dsoaInfos.cbegin(), dsoaInfos.cend(), 0.0, [](double sum, const DsoaInfo& dsoaInfo) { return sum + dsoaInfo.outdoorAirTotal; });
  EXPECT_DOUBLE_EQ(2.1, expectedOutdoorAirTotal);

  // We neeed at least one equipment (or useIdealAirLoads) AND a DesignDay, for the Sizing:Zone (and DSOA:SpaceList) to be translated
  // Because we use IdealAirLoads, it also will create some ZoneVentilation:DesignFlowRate
  // (because we use the HVACTemplate:Zone:IdealLoadsAirSystem and not the full IDF object)
  EXPECT_TRUE(z.setUseIdealAirLoads(true));
  DesignDay d(m);

  ForwardTranslator ft;
  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Sizing_Zone).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir_SpaceList).size());

    auto peoples = w.getObjectsByType(IddObjectType::People);
    ASSERT_EQ(3, peoples.size());
    double totPeople = 0.0;
    for (const auto& people : peoples) {
      EXPECT_EQ("People", people.getString(PeopleFields::NumberofPeopleCalculationMethod).get());
      double nPeople = people.getDouble(PeopleFields::NumberofPeople).get();
      EXPECT_EQ(10.0, nPeople);
      totPeople += nPeople;
    }
    EXPECT_EQ(30.0, totPeople);

    auto dsoas = w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir);
    ASSERT_EQ(1, dsoas.size());
    auto dsoa = dsoas[0];

    // A mix of 'Maximum' and 'Sum' gives 'Sum'
    double perPerson = dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get();
    double perFloorArea = dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get();
    double absolute = dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone).get();
    double ach = dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get();
    EXPECT_EQ("Sum", dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());

    // We had 20 people in Space1, and it's the only one with the perPeople. Zone has 30 people. 0.015 * 20 / 30 = 0.01
    double expectedPerPerson = dsoaSpace1.outdoorAirFlowperPerson() * 20.0 / 30.0;
    EXPECT_EQ(0.01, expectedPerPerson);
    EXPECT_EQ(expectedPerPerson, perPerson);
    // For the rest is gets super tricky, since when you have a mix of Sum and Max,
    // the Max ones are calculated and used as "Outdoor Air Flow Rate" (absolute)
    // Let's try to check the total. Calculate each component in m3/s
    double outdoorAirForPeople = 3 * numberOfPeople * perPerson;
    double outdoorAirForFloorArea = 4 * spaceFloorArea * perFloorArea;
    double outdoorAirRate = absolute;
    double outdoorAirForVolume = 4 * height * width * width * ach / 3600.0;

    double outdoorAirTotal = outdoorAirForPeople + outdoorAirForFloorArea + outdoorAirRate + outdoorAirForVolume;
    EXPECT_DOUBLE_EQ(expectedOutdoorAirTotal, outdoorAirTotal);
    // Now we can check individual components
    EXPECT_EQ(0.01, perPerson);
    EXPECT_EQ(0.001, perFloorArea);
    EXPECT_EQ(1.35, absolute);
    EXPECT_EQ(0.125, ach);

    auto sizingZone = w.getObjectsByType(IddObjectType::Sizing_Zone)[0];
    ASSERT_TRUE(sizingZone.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName));
    EXPECT_EQ(dsoa, sizingZone.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName).get());

    auto zvs = w.getObjectsByType(IddObjectType::ZoneVentilation_DesignFlowRate);
    ASSERT_EQ(4, zvs.size());
    std::vector<std::tuple<std::string, unsigned int, double>> expectedZvs{
      {"AirChanges/Hour", ZoneVentilation_DesignFlowRateFields::AirChangesperHour, 0.125},
      {"Flow/Zone", ZoneVentilation_DesignFlowRateFields::DesignFlowRate, 1.35},
      {"Flow/Area", ZoneVentilation_DesignFlowRateFields::FlowRateperFloorArea, 0.001},
      {"Flow/Person", ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, 0.01}};
    auto checkZv = [&zvs, zoneName = z.nameString()](const std::string& method, unsigned int index, double value) -> void {
      auto it = std::find_if(zvs.cbegin(), zvs.cend(), [&method](const auto& zv) -> bool {
        EXPECT_TRUE(zv.getString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, false, true));
        return openstudio::istringEqual(method, zv.getString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod).get());
      });
      ASSERT_NE(zvs.cend(), it) << "Cannot find the ZoneVentilation:DesignFlowRate object with method '" << method << "'.";
      ASSERT_EQ(zoneName, it->getString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName).get());
      ASSERT_TRUE(it->getDouble(index));
      EXPECT_DOUBLE_EQ(value, it->getDouble(index).get()) << "Failed for " << method;
    };
    for (auto& [method, index, value] : expectedZvs) {
      checkZv(method, index, value);
    }
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1, w.getObjectsByType(IddObjectType::Sizing_Zone).size());
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::SpaceList).size());
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::ZoneVentilation_DesignFlowRate).size());

    auto space1_ = w.getObjectByTypeAndName(IddObjectType::Space, space1.nameString());
    ASSERT_TRUE(space1_);
    auto space2_ = w.getObjectByTypeAndName(IddObjectType::Space, space2.nameString());
    ASSERT_TRUE(space2_);
    auto space3_ = w.getObjectByTypeAndName(IddObjectType::Space, space3.nameString());
    ASSERT_TRUE(space3_);
    auto space4_ = w.getObjectByTypeAndName(IddObjectType::Space, space4.nameString());
    ASSERT_TRUE(space4_);

    auto officeSpaceType_ = w.getObjectByTypeAndName(IddObjectType::SpaceList, officeSpaceType.nameString());
    ASSERT_TRUE(officeSpaceType_);
    {
      ASSERT_EQ(2, officeSpaceType_->numExtensibleGroups());
      auto wegs = openstudio::castVector<WorkspaceExtensibleGroup>(officeSpaceType_->extensibleGroups());
      ASSERT_TRUE(std::all_of(wegs.begin(), wegs.end(), [](const auto& weg) { return weg.getTarget(SpaceListExtensibleFields::SpaceName); }));
      EXPECT_NE(wegs.cend(), std::find_if(wegs.cbegin(), wegs.cend(), [&space1_](const auto& weg) {
                  return weg.getTarget(SpaceListExtensibleFields::SpaceName).get() == space1_.get();
                }));
      EXPECT_NE(wegs.cend(), std::find_if(wegs.cbegin(), wegs.cend(), [&space2_](const auto& weg) {
                  return weg.getTarget(SpaceListExtensibleFields::SpaceName).get() == space2_.get();
                }));
    }

    auto buildingSpaceType_ = w.getObjectByTypeAndName(IddObjectType::SpaceList, buildingSpaceType.nameString());
    ASSERT_TRUE(buildingSpaceType_);
    {
      ASSERT_EQ(2, buildingSpaceType_->numExtensibleGroups());
      auto wegs = openstudio::castVector<WorkspaceExtensibleGroup>(buildingSpaceType_->extensibleGroups());
      ASSERT_TRUE(std::all_of(wegs.begin(), wegs.end(), [](const auto& weg) { return weg.getTarget(SpaceListExtensibleFields::SpaceName); }));
      EXPECT_NE(wegs.cend(), std::find_if(wegs.cbegin(), wegs.cend(), [&space3_](const auto& weg) {
                  return weg.getTarget(SpaceListExtensibleFields::SpaceName).get() == space3_.get();
                }));
      EXPECT_NE(wegs.cend(), std::find_if(wegs.cbegin(), wegs.cend(), [&space4_](const auto& weg) {
                  return weg.getTarget(SpaceListExtensibleFields::SpaceName).get() == space4_.get();
                }));
    }

    auto peoples = w.getObjectsByType(IddObjectType::People);
    ASSERT_EQ(2, peoples.size());

    auto peopleSpace1_ = w.getObjectByTypeAndName(IddObjectType::People, space1P.nameString());
    ASSERT_TRUE(peopleSpace1_);
    ASSERT_TRUE(peopleSpace1_->getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(space1_.get(), peopleSpace1_->getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName).get());
    EXPECT_EQ("People", peopleSpace1_->getString(PeopleFields::NumberofPeopleCalculationMethod).get());
    ASSERT_TRUE(peopleSpace1_->getDouble(PeopleFields::NumberofPeople));
    EXPECT_EQ(10.0, peopleSpace1_->getDouble(PeopleFields::NumberofPeople).get());

    auto peopleOffice_ = w.getObjectByTypeAndName(IddObjectType::People, officeP.nameString());
    ASSERT_TRUE(peopleOffice_);
    ASSERT_TRUE(peopleOffice_->getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(officeSpaceType_.get(), peopleOffice_->getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName).get());
    EXPECT_EQ("People/Area", peopleOffice_->getString(PeopleFields::NumberofPeopleCalculationMethod).get());
    ASSERT_TRUE(peopleOffice_->getDouble(PeopleFields::PeopleperFloorArea));
    EXPECT_EQ(0.1, peopleOffice_->getDouble(PeopleFields::PeopleperFloorArea).get());

    double totPeople = 10.0 + spaceFloorArea * 2 * 0.1;
    EXPECT_EQ(30.0, totPeople);

    auto dsoa_spaceLists = w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir_SpaceList);
    ASSERT_EQ(1, dsoa_spaceLists.size());
    auto& dsoa_spaceList = dsoa_spaceLists[0];
    ASSERT_EQ(4, dsoa_spaceList.numExtensibleGroups());

    auto dsoas = w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir);
    ASSERT_EQ(4, dsoas.size());
    auto dsoaSpace1_ = w.getObjectByTypeAndName(IddObjectType::DesignSpecification_OutdoorAir, dsoaSpace1.nameString());
    ASSERT_TRUE(dsoaSpace1_);
    auto dsoaOffice_ = w.getObjectByTypeAndName(IddObjectType::DesignSpecification_OutdoorAir, dsoaOffice.nameString());
    ASSERT_TRUE(dsoaOffice_);
    auto dsoaSpace3_ = w.getObjectByTypeAndName(IddObjectType::DesignSpecification_OutdoorAir, dsoaSpace3.nameString());
    ASSERT_TRUE(dsoaSpace3_);
    auto dsoaBuilding_ = w.getObjectByTypeAndName(IddObjectType::DesignSpecification_OutdoorAir, dsoaBuilding.nameString());
    ASSERT_TRUE(dsoaBuilding_);

    {
      auto eg = dsoa_spaceList.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName));
      EXPECT_EQ(space1_.get(), eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName).get());
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName));
      EXPECT_EQ(dsoaSpace1_.get(),
                eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName).get());

      auto dsoa = dsoaSpace1_.get();
      EXPECT_EQ(dsoaSpace1.outdoorAirMethod(), dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
      EXPECT_EQ(dsoaSpace1.outdoorAirFlowperPerson(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
      EXPECT_EQ(dsoaSpace1.outdoorAirFlowperFloorArea(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
      EXPECT_EQ(dsoaSpace1.outdoorAirFlowAirChangesperHour(),
                dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
    }
    {
      auto eg = dsoa_spaceList.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName));
      EXPECT_EQ(space2_.get(), eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName).get());
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName));
      EXPECT_EQ(dsoaOffice_.get(),
                eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName).get());

      auto dsoa = dsoaOffice_.get();
      EXPECT_EQ(dsoaOffice.outdoorAirMethod(), dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
      EXPECT_EQ(dsoaOffice.outdoorAirFlowperPerson(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
      EXPECT_EQ(dsoaOffice.outdoorAirFlowperFloorArea(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
      EXPECT_EQ(dsoaOffice.outdoorAirFlowAirChangesperHour(),
                dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
    }
    {
      auto eg = dsoa_spaceList.extensibleGroups()[2].cast<WorkspaceExtensibleGroup>();
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName));
      EXPECT_EQ(space3_.get(), eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName).get());
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName));
      EXPECT_EQ(dsoaSpace3_.get(),
                eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName).get());

      auto dsoa = dsoaSpace3_.get();
      EXPECT_EQ(dsoaSpace3.outdoorAirMethod(), dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
      EXPECT_EQ(dsoaSpace3.outdoorAirFlowperPerson(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
      EXPECT_EQ(dsoaSpace3.outdoorAirFlowperFloorArea(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
      EXPECT_EQ(dsoaSpace3.outdoorAirFlowAirChangesperHour(),
                dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
    }
    {
      auto eg = dsoa_spaceList.extensibleGroups()[3].cast<WorkspaceExtensibleGroup>();
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName));
      EXPECT_EQ(space4_.get(), eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceName).get());
      ASSERT_TRUE(eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName));
      EXPECT_EQ(dsoaBuilding_.get(),
                eg.getTarget(DesignSpecification_OutdoorAir_SpaceListExtensibleFields::SpaceDesignSpecificationOutdoorAirObjectName).get());

      auto dsoa = dsoaBuilding_.get();
      EXPECT_EQ(dsoaBuilding.outdoorAirMethod(), dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
      EXPECT_EQ(dsoaBuilding.outdoorAirFlowperPerson(), dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
      EXPECT_EQ(dsoaBuilding.outdoorAirFlowperFloorArea(),
                dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
      EXPECT_EQ(dsoaBuilding.outdoorAirFlowAirChangesperHour(),
                dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
    }

    auto sizingZone = w.getObjectsByType(IddObjectType::Sizing_Zone)[0];
    ASSERT_TRUE(sizingZone.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName));
    EXPECT_EQ(dsoa_spaceList, sizingZone.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName).get());

    auto zvs = w.getObjectsByType(IddObjectType::ZoneVentilation_DesignFlowRate);
    ASSERT_EQ(4, zvs.size());
    std::vector<std::tuple<std::string, unsigned int, double>> expectedZvs{
      {"AirChanges/Hour", ZoneVentilation_DesignFlowRateFields::AirChangesperHour, 0.125},  // 0.5 * 360 / (4*360) = 0.125
      {"Flow/Zone", ZoneVentilation_DesignFlowRateFields::DesignFlowRate, 0.75},            // 0.65 + 0.1
      {"Flow/Area", ZoneVentilation_DesignFlowRateFields::FlowRateperFloorArea, 0.0025},    // 0.004*100 + 0.006*100 / (4 * 100) = 0.0025
      {"Flow/Person", ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, 0.01}};      // 0.015 * 20 + 10*0.00/ 30 = 0.01
    auto checkZv = [&zvs, zoneName = z.nameString()](const std::string& method, unsigned int index, double value) -> void {
      auto it = std::find_if(zvs.cbegin(), zvs.cend(), [&method](const auto& zv) -> bool {
        EXPECT_TRUE(zv.getString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, false, true));
        return openstudio::istringEqual(method, zv.getString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod).get());
      });
      ASSERT_NE(zvs.cend(), it) << "Cannot find the ZoneVentilation:DesignFlowRate object with method '" << method << "'.";
      ASSERT_EQ(zoneName, it->getString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName).get());
      ASSERT_TRUE(it->getDouble(index));
      EXPECT_DOUBLE_EQ(value, it->getDouble(index).get()) << "Failed for " << method;
    };
    for (auto& [method, index, value] : expectedZvs) {
      checkZv(method, index, value);
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DesignSpecificationOutdoorAir_NoDSOA) {

  // Controller:MechnicalVentilation: Design Specification Outdoor Air Object Name <x>
  // > If this field is blank, the corresponding DesignSpecification:OutdoorAir object for the zone will come from
  // > the DesignSpecification:OutdoorAir object referenced by the Sizing:Zone object for the same zone.
  // > ***If no such zone match is found, default values from the IDD will be used for the DesignSpecification:OutdoorAir object
  // > which is 0.0094 m3/s-person.***

  Model m;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┤
  //   │        │        │
  //   │        │        │
  //   │ Space 1│ Space 2│
  //   │        │        │
  //   └────────┴────────┴──► x
  //  0        10       20

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  constexpr double spaceFloorArea = width * width;
  constexpr double spaceVolume = spaceFloorArea * height;

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  // 4 spaces in a zone
  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);

  EXPECT_EQ(spaceFloorArea, space1.floorArea());
  EXPECT_EQ(spaceFloorArea, space2.floorArea());
  EXPECT_EQ(spaceVolume, space1.volume());
  EXPECT_EQ(spaceVolume, space2.volume());

  ThermalZone z(m);
  EXPECT_TRUE(space1.setThermalZone(z));
  EXPECT_TRUE(space2.setThermalZone(z));

  // We neeed at least one equipment (or useIdealAirLoads) AND a DesignDay, for the Sizing:Zone (and DSOA:SpaceList) to be translated
  // Because we use IdealAirLoads, it also will create some ZoneVentilation:DesignFlowRate
  // (because we use the HVACTemplate:Zone:IdealLoadsAirSystem and not the full IDF object)
  EXPECT_TRUE(z.setUseIdealAirLoads(true));
  DesignDay d(m);

  ForwardTranslator ft;
  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    // In this case, ThermalZone::combineSpaces() already creates a DSOA with all zeroes
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Sizing_Zone).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir_SpaceList).size());

    auto dsoas = w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir);
    ASSERT_EQ(1, dsoas.size());
    auto dsoa = dsoas[0];

    EXPECT_EQ("Sum", dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    // In this case, we manually still create an empty DSOA
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Sizing_Zone).size());
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());  // There are zero spaces types here!
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir_SpaceList).size());

    auto dsoas = w.getObjectsByType(IddObjectType::DesignSpecification_OutdoorAir);
    ASSERT_EQ(1, dsoas.size());
    auto dsoa = dsoas[0];

    EXPECT_EQ("Sum", dsoa.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone).get());
    EXPECT_EQ(0.0, dsoa.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour).get());
  }
}
