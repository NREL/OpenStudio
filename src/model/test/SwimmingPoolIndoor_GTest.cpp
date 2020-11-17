/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../SwimmingPoolIndoor.hpp"
#include "../SwimmingPoolIndoor_Impl.hpp"

#include "../Surface.hpp"
#include "../Space.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleRuleset.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SwimmingPoolIndoor_GettersSetters) {
  Model m;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, m);
  ASSERT_TRUE(space1);
  auto surfaces = space1->surfaces();
  EXPECT_EQ(6u, surfaces.size());

  auto wallSurfaceIt = std::find_if(std::begin(surfaces), std::end(surfaces), [](const auto& surface) { return surface.surfaceType() == "Wall"; });
  ASSERT_NE(wallSurfaceIt, std::end(surfaces));
  Surface wallSurface = *wallSurfaceIt;

  auto floorSurfaceIt = std::find_if(std::begin(surfaces), std::end(surfaces), [](const auto& surface) { return surface.surfaceType() == "Floor"; });
  ASSERT_NE(floorSurfaceIt, std::end(surfaces));
  Surface floorSurface = *floorSurfaceIt;

  ASSERT_ANY_THROW(SwimmingPoolIndoor(m, wallSurface));

  SwimmingPoolIndoor swimmingPoolIndoor(m, floorSurface);

  swimmingPoolIndoor.setName("My SwimmingPoolIndoor");

  // Surface Name: Required Object
  EXPECT_FALSE(swimmingPoolIndoor.setSurface(wallSurface));
  EXPECT_EQ(floorSurface, swimmingPoolIndoor.surface());

  floorPrint.clear();
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(20, 10, 0));
  floorPrint.push_back(Point3d(20, 0, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, m);
  ASSERT_TRUE(space2);
  auto surfaces2 = space2->surfaces();
  auto floorSurfaceIt2 = std::find_if(std::begin(surfaces), std::end(surfaces), [](const auto& surface) { return surface.surfaceType() == "Floor"; });
  ASSERT_NE(floorSurfaceIt2, std::end(surfaces2));
  Surface floorSurface2 = *floorSurfaceIt2;
  EXPECT_TRUE(swimmingPoolIndoor.setSurface(floorSurface2));
  EXPECT_EQ(floorSurface2, swimmingPoolIndoor.surface());

  // Average Depth: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setAverageDepth(3));
  EXPECT_EQ(3, swimmingPoolIndoor.averageDepth());

  // Activity Factor Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setActivityFactorSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.activityFactorSchedule());
  }

  // Make-up Water Supply Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setMakeupWaterSupplySchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.makeupWaterSupplySchedule());
  }

  // Cover Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setCoverSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.coverSchedule());
  }

  // Cover Evaporation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverEvaporationFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverEvaporationFactor());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverEvaporationFactor(-10.0));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverEvaporationFactor());

  // Cover Convection Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverConvectionFactor(0.6));
  EXPECT_EQ(0.6, swimmingPoolIndoor.coverConvectionFactor());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverConvectionFactor(-10.0));
  EXPECT_EQ(0.6, swimmingPoolIndoor.coverConvectionFactor());

  // Cover Short-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverShortWavelengthRadiationFactor(0.7));
  EXPECT_EQ(0.7, swimmingPoolIndoor.coverShortWavelengthRadiationFactor());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverShortWavelengthRadiationFactor(-10.0));
  EXPECT_EQ(0.7, swimmingPoolIndoor.coverShortWavelengthRadiationFactor());

  // Cover Long-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverLongWavelengthRadiationFactor(0.8));
  EXPECT_EQ(0.8, swimmingPoolIndoor.coverLongWavelengthRadiationFactor());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverLongWavelengthRadiationFactor(-10.0));
  EXPECT_EQ(0.8, swimmingPoolIndoor.coverLongWavelengthRadiationFactor());

  // Pool Heating System Maximum Water Flow Rate: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolHeatingSystemMaximumWaterFlowRate(0.1));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolHeatingSystemMaximumWaterFlowRate());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setPoolHeatingSystemMaximumWaterFlowRate(-10.0));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolHeatingSystemMaximumWaterFlowRate());

  // Pool Miscellaneous Equipment Power: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolMiscellaneousEquipmentPower(2000.0));
  EXPECT_EQ(2000., swimmingPoolIndoor.poolMiscellaneousEquipmentPower());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setPoolMiscellaneousEquipmentPower(-10.0));
  EXPECT_EQ(2000., swimmingPoolIndoor.poolMiscellaneousEquipmentPower());

  // Setpoint Temperature Schedule: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setSetpointTemperatureSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.setpointTemperatureSchedule());
  }

  // Maximum Number of People: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setMaximumNumberofPeople(12.5));
  EXPECT_EQ(12.5, swimmingPoolIndoor.maximumNumberofPeople());
  // Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setMaximumNumberofPeople(-10.0));
  EXPECT_EQ(12.5, swimmingPoolIndoor.maximumNumberofPeople());

  // People Schedule: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setPeopleSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.peopleSchedule());
  }

  // People Heat Gain Schedule: Required Object
  {
    ScheduleConstant sch(m);
    EXPECT_TRUE(swimmingPoolIndoor.setPeopleHeatGainSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.peopleHeatGainSchedule());
  }
}

TEST_F(ModelFixture, SwimmingPoolIndoor_addToNode) {

  Model m;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, m);
  ASSERT_TRUE(space1);
  auto surfaces = space1->surfaces();
  EXPECT_EQ(6u, surfaces.size());

  auto floorSurfaceIt = std::find_if(std::begin(surfaces), std::end(surfaces), [](const auto& surface) { return surface.surfaceType() == "Floor"; });
  ASSERT_NE(floorSurfaceIt, std::end(surfaces));
  Surface floorSurface = *floorSurfaceIt;

  SwimmingPoolIndoor swimmingPoolIndoor(m, floorSurface);
  EXPECT_FALSE(swimmingPoolIndoor.inletModelObject());
  EXPECT_FALSE(swimmingPoolIndoor.outletModelObject());
  EXPECT_FALSE(swimmingPoolIndoor.poolWaterInletNode());
  EXPECT_FALSE(swimmingPoolIndoor.poolWaterOutletNode());

  // Not accepted on any AirLoopHVAC
  AirLoopHVAC airLoop = AirLoopHVAC(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(swimmingPoolIndoor.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(swimmingPoolIndoor.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);

  // This should de settable to the demand side only
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(swimmingPoolIndoor.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(swimmingPoolIndoor.poolWaterInletNode());
  EXPECT_FALSE(swimmingPoolIndoor.poolWaterOutletNode());

  // This should be settable to the demand side
  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(swimmingPoolIndoor.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());
  EXPECT_TRUE(swimmingPoolIndoor.poolWaterInletNode());
  EXPECT_TRUE(swimmingPoolIndoor.poolWaterOutletNode());

  SwimmingPoolIndoor swimmingPoolIndoorClone = swimmingPoolIndoor.clone(m).cast<SwimmingPoolIndoor>();
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  EXPECT_TRUE(swimmingPoolIndoor.poolWaterInletNode());
  EXPECT_TRUE(swimmingPoolIndoor.poolWaterOutletNode());
  EXPECT_FALSE(swimmingPoolIndoorClone.poolWaterInletNode());
  EXPECT_FALSE(swimmingPoolIndoorClone.poolWaterOutletNode());

  EXPECT_TRUE(swimmingPoolIndoorClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());

  EXPECT_TRUE(swimmingPoolIndoor.poolWaterInletNode());
  EXPECT_TRUE(swimmingPoolIndoor.poolWaterOutletNode());
  EXPECT_TRUE(swimmingPoolIndoorClone.poolWaterInletNode());
  EXPECT_TRUE(swimmingPoolIndoorClone.poolWaterOutletNode());
}
