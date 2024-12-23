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

#include "../../model/ZoneHVACEvaporativeCoolerUnit.hpp"
#include "../../model/ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/FanComponentModel.hpp"
#include "../../model/FanComponentModel_Impl.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../../model/EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

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

  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);

  zoneHVACEvaporativeCoolerUnit.setName("My ZoneHVACEvaporativeCoolerUnit");
  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilitySchedule(availabilitySchedule));
  FanComponentModel supplyAirFan(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSupplyAirFan(supplyAirFan));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setDesignSupplyAirFlowRate(0.9));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFanPlacement("BlowThrough"));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolerUnitControlMethod("ZoneTemperatureDeadbandOnOffCycling"));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setThrottlingRangeTemperatureDifference(1.2));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolingLoadControlThresholdHeatTransferRate(1.3));
  EvaporativeCoolerDirectResearchSpecial firstEvaporativeCooler(m, availabilitySchedule);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFirstEvaporativeCooler(firstEvaporativeCooler));
  EvaporativeCoolerIndirectResearchSpecial secondEvaporativeCooler(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSecondEvaporativeCooler(secondEvaporativeCooler));
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(95.0));

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  zoneHVACEvaporativeCoolerUnit.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::ZoneHVAC_EvaporativeCoolerUnit);
  ASSERT_EQ(1u, idfObjs.size());
  const auto& idfObject = idfObjs.front();

  EXPECT_EQ(zoneHVACEvaporativeCoolerUnit.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::Name).get());
  EXPECT_EQ(availabilitySchedule.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName).get());
  EXPECT_EQ("", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName).get());
  EXPECT_EQ("", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName).get());
  EXPECT_EQ("", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName).get());
  EXPECT_EQ("", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName).get());
  EXPECT_EQ("Fan:ComponentModel", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanObjectType).get());
  EXPECT_EQ(supplyAirFan.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName).get());
  EXPECT_EQ(0.9, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate).get());
  EXPECT_EQ("BlowThrough", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement).get());
  EXPECT_EQ("ZoneTemperatureDeadbandOnOffCycling", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod).get());
  EXPECT_EQ(1.2, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ThrottlingRangeTemperatureDifference).get());
  EXPECT_EQ(1.3, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate).get());
  EXPECT_EQ("EvaporativeCooler:Direct:ResearchSpecial",
            idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectType).get());
  EXPECT_EQ(firstEvaporativeCooler.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectName).get());
  EXPECT_EQ("EvaporativeCooler:Indirect:ResearchSpecial",
            idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerObjectType).get());
  EXPECT_EQ(secondEvaporativeCooler.nameString(), idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName).get());
  EXPECT_EQ("", idfObject.getString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizingObjectName).get());
  EXPECT_EQ(95.0, idfObject.getDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity).get());
}
