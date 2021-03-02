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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SwimmingPoolIndoor.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/SwimmingPool_Indoor_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SwimmingPoolIndoor) {

  ForwardTranslator ft;

  // Create a model
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

  // Space needs to be in a ThermalZone or it's not translated
  ThermalZone z(m);
  EXPECT_TRUE(space1->setThermalZone(z));

  auto floorSurfaceIt = std::find_if(std::begin(surfaces), std::end(surfaces), [](const auto& surface) { return surface.surfaceType() == "Floor"; });
  ASSERT_NE(floorSurfaceIt, std::end(surfaces));
  Surface floorSurface = *floorSurfaceIt;

  SwimmingPoolIndoor swimmingPoolIndoor(m, floorSurface);

  // Average Depth: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setAverageDepth(3));
  EXPECT_EQ(3, swimmingPoolIndoor.averageDepth());

  // Activity Factor Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("Activity Factor Schedule");
    EXPECT_TRUE(swimmingPoolIndoor.setActivityFactorSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.activityFactorSchedule());
  }

  // Make-up Water Supply Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("Make-up Water Supply Schedule");
    EXPECT_TRUE(swimmingPoolIndoor.setMakeupWaterSupplySchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.makeupWaterSupplySchedule());
  }

  // Cover Schedule Name: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("Cover Schedule");
    EXPECT_TRUE(swimmingPoolIndoor.setCoverSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.coverSchedule());
  }

  // Cover Evaporation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverEvaporationFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverEvaporationFactor());

  // Cover Convection Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverConvectionFactor(0.6));
  EXPECT_EQ(0.6, swimmingPoolIndoor.coverConvectionFactor());

  // Cover Short-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverShortWavelengthRadiationFactor(0.7));
  EXPECT_EQ(0.7, swimmingPoolIndoor.coverShortWavelengthRadiationFactor());

  // Cover Long-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverLongWavelengthRadiationFactor(0.8));
  EXPECT_EQ(0.8, swimmingPoolIndoor.coverLongWavelengthRadiationFactor());

  // Pool Heating System Maximum Water Flow Rate: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolHeatingSystemMaximumWaterFlowRate(0.1));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolHeatingSystemMaximumWaterFlowRate());

  // Pool Miscellaneous Equipment Power: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolMiscellaneousEquipmentPower(2000.0));
  EXPECT_EQ(2000., swimmingPoolIndoor.poolMiscellaneousEquipmentPower());

  // Setpoint Temperature Schedule: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("Setpoint Temperature");
    EXPECT_TRUE(swimmingPoolIndoor.setSetpointTemperatureSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.setpointTemperatureSchedule());
  }

  // Maximum Number of People: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setMaximumNumberofPeople(12.5));
  EXPECT_EQ(12.5, swimmingPoolIndoor.maximumNumberofPeople());

  // People Schedule: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("People Schedule");
    EXPECT_TRUE(swimmingPoolIndoor.setPeopleSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.peopleSchedule());
  }

  // People Heat Gain Schedule: Required Object
  {
    ScheduleConstant sch(m);
    sch.setName("People Heat Gain Schedule");
    EXPECT_TRUE(swimmingPoolIndoor.setPeopleHeatGainSchedule(sch));
    EXPECT_EQ(sch, swimmingPoolIndoor.peopleHeatGainSchedule());
  }

  // Not on a PlantLoop: not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SwimmingPool_Indoor);
    ASSERT_EQ(0u, idfObjs.size());
  }

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(swimmingPoolIndoor));
  ASSERT_TRUE(swimmingPoolIndoor.inletModelObject());
  swimmingPoolIndoor.inletModelObject()->setName("Pool Inlet Node");
  ASSERT_TRUE(swimmingPoolIndoor.outletModelObject());
  swimmingPoolIndoor.outletModelObject()->setName("Pool Outlet Node");

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SwimmingPool_Indoor);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_pool(idfObjs[0]);

    EXPECT_EQ(swimmingPoolIndoor.nameString(), idf_pool.getString(SwimmingPool_IndoorFields::Name).get());

    EXPECT_EQ(3.0, idf_pool.getDouble(SwimmingPool_IndoorFields::AverageDepth).get());
    EXPECT_EQ(0.5, idf_pool.getDouble(SwimmingPool_IndoorFields::CoverEvaporationFactor).get());
    EXPECT_EQ(0.6, idf_pool.getDouble(SwimmingPool_IndoorFields::CoverConvectionFactor).get());
    EXPECT_EQ(0.7, idf_pool.getDouble(SwimmingPool_IndoorFields::CoverShortWavelengthRadiationFactor).get());
    EXPECT_EQ(0.8, idf_pool.getDouble(SwimmingPool_IndoorFields::CoverLongWavelengthRadiationFactor).get());
    EXPECT_EQ(0.1, idf_pool.getDouble(SwimmingPool_IndoorFields::PoolHeatingSystemMaximumWaterFlowRate).get());
    EXPECT_EQ(2000.0, idf_pool.getDouble(SwimmingPool_IndoorFields::PoolMiscellaneousEquipmentPower).get());
    EXPECT_EQ(12.5, idf_pool.getDouble(SwimmingPool_IndoorFields::MaximumNumberofPeople).get());

    EXPECT_EQ("Activity Factor Schedule", idf_pool.getString(SwimmingPool_IndoorFields::ActivityFactorScheduleName).get());
    EXPECT_EQ("Make-up Water Supply Schedule", idf_pool.getString(SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName).get());
    EXPECT_EQ("Cover Schedule", idf_pool.getString(SwimmingPool_IndoorFields::CoverScheduleName).get());
    EXPECT_EQ("Setpoint Temperature", idf_pool.getString(SwimmingPool_IndoorFields::SetpointTemperatureSchedule).get());
    EXPECT_EQ("People Schedule", idf_pool.getString(SwimmingPool_IndoorFields::PeopleSchedule).get());
    EXPECT_EQ("People Heat Gain Schedule", idf_pool.getString(SwimmingPool_IndoorFields::PeopleHeatGainSchedule).get());

    EXPECT_EQ("Pool Inlet Node", idf_pool.getString(SwimmingPool_IndoorFields::PoolWaterInletNode).get());
    EXPECT_EQ("Pool Outlet Node", idf_pool.getString(SwimmingPool_IndoorFields::PoolWaterOutletNode).get());
  }
}

//TEST_F(EnergyPlusFixture, ReverseTranslator_SwimmingPoolIndoor) {

//ReverseTranslator rt;

//Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

//// Not there, Model shouldn't have it either
//Model m = rt.translateWorkspace(w);
//EXPECT_FALSE(m.getOptionalUniqueModelObject<SwimmingPoolIndoor>());

//OptionalWorkspaceObject _i_pool = w.addObject(IdfObject(IddObjectType::SwimmingPool_Indoor));
//ASSERT_TRUE(_i_pool);

//EXPECT_TRUE(_i_pool->setString(SwimmingPool_IndoorFields::SwimmingPoolIndoor, boolToString(json)));
//EXPECT_TRUE(_i_pool->setString(SwimmingPool_IndoorFields::OutputCBOR, boolToString(cbor)));
//EXPECT_TRUE(_i_pool->setString(SwimmingPool_IndoorFields::OutputMessagePack, boolToString(msgpack)));

//Model m = rt.translateWorkspace(w);

//// Get the unique object
//auto pools = m.getConcreteModelObjects<SwimmingPoolIndoor>();
//ASSERT_EQ(1u, pools.size());

//}
