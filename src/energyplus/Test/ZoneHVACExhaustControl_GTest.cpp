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

#include "../../model/ZoneHVACExhaustControl.hpp"
#include "../../model/ZoneHVACExhaustControl_Impl.hpp"
// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Zone.hpp"
#include "../../model/Zone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/ZoneHVAC_ExhaustControl_FieldEnums.hxx>
using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACExhaustControl) {

  ForwardTranslator ft;

  Model m;
  // TODO: Check regular Ctor arguments
  ZoneHVACExhaustControl zoneHVACExhaustControl(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // ZoneHVACExhaustControl zoneHVACExhaustControl = m.getUniqueModelObject<ZoneHVACExhaustControl>();

  zoneHVACExhaustControl.setName("My ZoneHVACExhaustControl");
  boost::optional<Schedule> availabilitySchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setAvailabilitySchedule(availabilitySchedule));
  Zone zone(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setZone(zone));
  Node inletNodeName(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setInletNodeName(inletNodeName));
  Node outletNodeName(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setOutletNodeName(outletNodeName));
  // Autosize
  // zoneHVACExhaustControl.autosizeDesignExhaustFlowRate();
  EXPECT_TRUE(zoneHVACExhaustControl.setDesignExhaustFlowRate(0.6));
  EXPECT_TRUE(zoneHVACExhaustControl.setFlowControlType("Scheduled"));
  boost::optional<Schedule> exhaustFlowFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setExhaustFlowFractionSchedule(exhaustFlowFractionSchedule));
  Node supplyNodeorNodeListName(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setSupplyNodeorNodeListName(supplyNodeorNodeListName));
  boost::optional<Schedule> minimumZoneTemperatureLimitSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setMinimumZoneTemperatureLimitSchedule(minimumZoneTemperatureLimitSchedule));
  boost::optional<Schedule> minimumExhaustFlowFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setMinimumExhaustFlowFractionSchedule(minimumExhaustFlowFractionSchedule));
  boost::optional<Schedule> balancedExhaustFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setBalancedExhaustFractionSchedule(balancedExhaustFractionSchedule));


  // TODO: you're responsible for creating all other objects needed so this object actually gets ForwardTranslated

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::ZoneHVAC_ExhaustControl);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
 EXPECT_EQ(availabilitySchedule.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::AvailabilityScheduleName).get());
 EXPECT_EQ(zone.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::ZoneName).get());
 EXPECT_EQ(inletNodeName.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::InletNodeName).get());
 EXPECT_EQ(outletNodeName.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::OutletNodeName).get());
  // EXPECT_EQ("Autosize", idfObject.getString(ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate).get());  EXPECT_EQ(0.6, idfObject.getDouble(ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate).get());  EXPECT_EQ("Scheduled", idfObject.getString(ZoneHVAC_ExhaustControlFields::FlowControlType).get()); EXPECT_EQ(exhaustFlowFractionSchedule.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName).get());
 EXPECT_EQ(supplyNodeorNodeListName.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::SupplyNodeorNodeListName).get());
 EXPECT_EQ(minimumZoneTemperatureLimitSchedule.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName).get());
 EXPECT_EQ(minimumExhaustFlowFractionSchedule.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName).get());
 EXPECT_EQ(balancedExhaustFractionSchedule.nameString(), idfObject.getString(ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName).get());

}
