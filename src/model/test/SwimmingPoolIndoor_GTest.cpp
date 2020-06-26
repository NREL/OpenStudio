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

// TODO: Check the following class names against object getters and setters.
#include "../model/FloorSurface.hpp"
#include "../model/FloorSurface_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../model/Connection.hpp"
#include "../model/Connection_Impl.hpp"

#include "../model/Connection.hpp"
#include "../model/Connection_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SwimmingPoolIndoor_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  SwimmingPoolIndoor swimmingPoolIndoor(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // SwimmingPoolIndoor swimmingPoolIndoor = m.getUniqueModelObject<SwimmingPoolIndoor>();

  swimmingPoolIndoor.setName("My SwimmingPoolIndoor");

  // Surface Name: Required Object
  FloorSurface obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setSurface(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.surface());

  // Average Depth: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setAverageDepth(3));
  EXPECT_EQ(3, swimmingPoolIndoor.averageDepth());

  // Activity Factor Schedule Name: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setActivityFactorSchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.activityFactorSchedule());

  // Make-up Water Supply Schedule Name: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setMakeupWaterSupplySchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.makeupWaterSupplySchedule());

  // Cover Schedule Name: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setCoverSchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.coverSchedule());

  // Cover Evaporation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverEvaporationFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverEvaporationFactor());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverEvaporationFactor(-10.0));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverEvaporationFactor());

  // Cover Convection Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverConvectionFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverConvectionFactor());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverConvectionFactor(-10.0));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverConvectionFactor());

  // Cover Short-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverShortWavelengthRadiationFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverShortWavelengthRadiationFactor());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverShortWavelengthRadiationFactor(-10.0));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverShortWavelengthRadiationFactor());

  // Cover Long-Wavelength Radiation Factor: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setCoverLongWavelengthRadiationFactor(0.5));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverLongWavelengthRadiationFactor());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setCoverLongWavelengthRadiationFactor(-10.0));
  EXPECT_EQ(0.5, swimmingPoolIndoor.coverLongWavelengthRadiationFactor());

  // Pool Water Inlet Node: Required Object
  Connection obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setPoolWaterInletNode(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.poolWaterInletNode());

  // Pool Water Outlet Node: Required Object
  Connection obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setPoolWaterOutletNode(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.poolWaterOutletNode());

  // Pool Heating System Maximum Water Flow Rate: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolHeatingSystemMaximumWaterFlowRate(0.1));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolHeatingSystemMaximumWaterFlowRate());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setPoolHeatingSystemMaximumWaterFlowRate(-10.0));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolHeatingSystemMaximumWaterFlowRate());

  // Pool Miscellaneous Equipment Power: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setPoolMiscellaneousEquipmentPower(0.1));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolMiscellaneousEquipmentPower());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setPoolMiscellaneousEquipmentPower(-10.0));
  EXPECT_EQ(0.1, swimmingPoolIndoor.poolMiscellaneousEquipmentPower());

  // Setpoint Temperature Schedule: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setSetpointTemperatureSchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.setpointTemperatureSchedule());

  // Maximum Number of People: Required Double
  EXPECT_TRUE(swimmingPoolIndoor.setMaximumNumberofPeople(0.1));
  EXPECT_EQ(0.1, swimmingPoolIndoor.maximumNumberofPeople());
   Bad Value
  EXPECT_FALSE(swimmingPoolIndoor.setMaximumNumberofPeople(-10.0));
  EXPECT_EQ(0.1, swimmingPoolIndoor.maximumNumberofPeople());

  // People Schedule: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setPeopleSchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.peopleSchedule());

  // People Heat Gain Schedule: Required Object
  Schedule obj(m);
  EXPECT_TRUE(swimmingPoolIndoor.setPeopleHeatGainSchedule(obj));
EXPECT_EQ(obj, swimmingPoolIndoor.peopleHeatGainSchedule());

