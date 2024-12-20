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

#include "../../model/ZoneHVACEvaporativeCoolerUnit.hpp"
#include "../../model/ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/SystemAvailabilityManagerLists.hpp"
#include "../../model/SystemAvailabilityManagerLists_Impl.hpp"
#include "../../model/Fans.hpp"
#include "../../model/Fans_Impl.hpp"
#include "../../model/EvapCooler.hpp"
#include "../../model/EvapCooler_Impl.hpp"
#include "../../model/EvapCooler.hpp"
#include "../../model/EvapCooler_Impl.hpp"
#include "../../model/DesignSpecificationZoneHVACSizingName.hpp"
#include "../../model/DesignSpecificationZoneHVACSizingName_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/ZoneHVAC_EvaporativeCoolerUnit_FieldEnums.hxx>
using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACEvaporativeCoolerUnit) {

  ForwardTranslator ft;

  Model m;
  // TODO: Check regular Ctor arguments
  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit = m.getUniqueModelObject<ZoneHVACEvaporativeCoolerUnit>();

  zoneHVACEvaporativeCoolerUnit.setName("My ZoneHVACEvaporativeCoolerUnit");
  boost::optional<Schedule> availabilitySchedule(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilitySchedule(availabilitySchedule));
  boost::optional<SystemAvailabilityManagerLists> availabilityManagerList(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilityManagerList(availabilityManagerList));
  Node outdoorAirInletNodeName(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setOutdoorAirInletNodeName(outdoorAirInletNodeName));
  Node coolerOutletNodeName(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolerOutletNodeName(coolerOutletNodeName));
  Node zoneReliefAirNodeName(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setZoneReliefAirNodeName(zoneReliefAirNodeName));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSupplyAirFanObjectType("Fan:SystemModel"));
  Fans supplyAirFan(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSupplyAirFan(supplyAirFan));
  // Autosize
  // zoneHVACEvaporativeCoolerUnit.autosizeDesignSupplyAirFlowRate();
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setDesignSupplyAirFlowRate(0.9));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFanPlacement("BlowThrough"));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolerUnitControlMethod("ZoneTemperatureDeadbandOnOffCycling"));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setThrottlingRangeTemperatureDifference(1.2));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolingLoadControlThresholdHeatTransferRate(1.3));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFirstEvaporativeCoolerObjectType("EvaporativeCooler:Direct:CelDekPad"));
  EvapCooler firstEvaporativeCoolerObject(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFirstEvaporativeCoolerObject(firstEvaporativeCoolerObject));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSecondEvaporativeCoolerObjectType("EvaporativeCooler:Direct:CelDekPad"));
  boost::optional<EvapCooler> secondEvaporativeCooler(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSecondEvaporativeCooler(secondEvaporativeCooler));
  boost::optional<DesignSpecificationZoneHVACSizingName> designSpecificationZoneHVACSizingObject(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setDesignSpecificationZoneHVACSizingObject(designSpecificationZoneHVACSizingObject));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(95.0));

  // TODO: you're responsible for creating all other objects needed so this object actually gets ForwardTranslated

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::ZoneHVAC_EvaporativeCoolerUnit);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ(availabilitySchedule.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName).get());
  EXPECT_EQ(availabilityManagerList.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName).get());
  EXPECT_EQ(outdoorAirInletNodeName.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName).get());
  EXPECT_EQ(coolerOutletNodeName.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName).get());
  EXPECT_EQ(zoneReliefAirNodeName.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName).get());
  EXPECT_EQ("Fan:SystemModel", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanObjectType).get());
  EXPECT_EQ(supplyAirFan.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName).get());
  // EXPECT_EQ("Autosize", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate).get());  EXPECT_EQ(0.9, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate).get());  EXPECT_EQ("BlowThrough", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement).get());  EXPECT_EQ("ZoneTemperatureDeadbandOnOffCycling", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod).get());  EXPECT_EQ(1.2, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ThrottlingRangeTemperatureDifference).get());  EXPECT_EQ(1.3, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate).get());  EXPECT_EQ("EvaporativeCooler:Direct:CelDekPad", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectType).get()); EXPECT_EQ(firstEvaporativeCoolerObject.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectName).get());
  EXPECT_EQ("EvaporativeCooler:Direct:CelDekPad", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerObjectType).get());
  EXPECT_EQ(secondEvaporativeCooler.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName).get());
  EXPECT_EQ(designSpecificationZoneHVACSizingObject.nameString(),
            idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizingObjectName).get());
  EXPECT_EQ(95.0, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity).get());
}
