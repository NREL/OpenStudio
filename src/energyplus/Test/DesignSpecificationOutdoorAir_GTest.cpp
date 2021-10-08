/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/data/Matrix.hpp"

#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

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

  // TODO: set number of people. Add people to spacetype too?
  EXPECT_TRUE(space2.setNumberOfPeople(numberOfPeople));

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

  // DSOA Characteristics
  // =====================
  //
  // |     DSOA       | Outdoor Air |   Per Person  | Per Floor Area | Absolute |   ACH  | Calculated   |
  // |     Name       |   Method    | (m3/s-person) |   (m3/s-m2)    |  (m3/s)  | (1/hr) | Total (m3/s) |
  // |----------------|-------------|---------------|----------------|----------|--------|--------------|
  // | dsoaSpace1     | Sum         | 0.015         | 0.004          |          |        | 0.55         |
  // | dsoaSpace2     | Max         |               |                | 0.65     | 2.0    | 0.65         |
  // | dsoaOffice     | Max         |               | 0.006          | 0.40     |        | 0.60         |
  // | dsoaBuilding   | Sum         |               |                | 0.10     | 0.5    | 0.15         |

  // Which DSOA applies to which spaces?
  // ====================================
  //
  // | Name   | Space Type                   | Space-Specic DSOA | DSOA         |
  // |--------|------------------------------|-------------------|--------------|
  // | Space1 | OfficeSpaceType              | dsoaSpace1        | dsoaSpace1   |
  // | Space2 | OfficeSpaceType              |                   | dsoaOffice   |
  // | Space3 | Inherited: buildingSpaceType | dsoaSpace3        | dsoaSpace3   |
  // | Space4 | Inherited: buildingSpaceType |                   | dsoaBuilding |

  struct DsoaInfo
  {
    DesignSpecificationOutdoorAir& dsoa;
    bool methodIsMax = false;
    double perPerson = 0.0;
    double perFloorArea = 0.0;
    double absolute = 0.0;
    double ach = 0.0;
    double outdoorAirForPeople;
    double outdoorAirForFloorArea;
    double outdoorAirRate;
    double outdoorAirForVolume;
    double outdoorAirTotal;

    DsoaInfo(DesignSpecificationOutdoorAir& t_dsoa, bool t_methodIsMax, double t_perPerson, double t_perFloorArea, double t_absolute, double t_ach)
      : dsoa(t_dsoa), methodIsMax(t_methodIsMax), perPerson(t_perPerson), perFloorArea(t_perFloorArea), absolute(t_absolute), ach(t_ach) {

      outdoorAirForPeople = 10.0 * perPerson;
      outdoorAirForFloorArea = 100.0 * perFloorArea;
      outdoorAirRate = absolute;
      outdoorAirForVolume = 3.6 * 10.0 * 10.0 * ach;
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
}
