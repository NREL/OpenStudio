/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Model.hpp"

#include "../../model/AvailabilityManagerNightCycle.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerNightCycle) {

  Model m;

  AvailabilityManagerNightCycle avm(m);

  EXPECT_TRUE(avm.setCyclingRunTime(1800));
  EXPECT_EQ(1800, avm.cyclingRunTime());
  EXPECT_TRUE(avm.setCyclingRunTimeControlType("Thermostat"));
  EXPECT_EQ("Thermostat", avm.cyclingRunTimeControlType());

  // Assign it to a loop
  AirLoopHVAC a(m);
  // TODO: This is going to be deprecated once #2844 gets merged in, replace it
  // EXPECT_TRUE(a.addAvailabilityManager(avm));
  EXPECT_TRUE(a.setAvailabilityManager(avm));

  // Assign Thermal Zones to the AirLoopHVAC
  ThermalZone z1(m);
  ThermalZone z2(m);
  ThermalZone z3(m);
  ThermalZone z4(m);

  EXPECT_TRUE(a.addBranchForZone(z1));
  EXPECT_TRUE(a.addBranchForZone(z2));
  EXPECT_TRUE(a.addBranchForZone(z3));
  EXPECT_TRUE(a.addBranchForZone(z4));

  // Cooling Zones is the only required, we don't set it
  EXPECT_TRUE(avm.setControlType("CycleOnAnyCoolingZone"));

  std::vector<ThermalZone> controlThermalZones = {z1};
  EXPECT_TRUE(avm.setControlThermalZones(controlThermalZones));

  // std::vector<ThermalZone> coolingControlThermalZones = {z1, z2};
  // EXPECT_TRUE(avm.setCoolingControlThermalZones(coolingControlThermalZones));

  std::vector<ThermalZone> heatingControlThermalZones = {z1, z2};
  EXPECT_TRUE(avm.setHeatingControlThermalZones(heatingControlThermalZones));

  std::vector<ThermalZone> heatingZoneFansOnlyThermalZones = {z1, z2, z3};
  EXPECT_TRUE(avm.setHeatingZoneFansOnlyThermalZones(heatingZoneFansOnlyThermalZones));


  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Should have 3 zonelists only (control zone is one zone = just name and no zonelist)
  EXPECT_EQ(3u, w.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManager_NightCycle));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // Cycling Run Time
  ASSERT_EQ(avm.cyclingRunTimeControlType(), idf_avm.getString(AvailabilityManager_NightCycleFields::CyclingRunTimeControlType).get());
  ASSERT_EQ(avm.cyclingRunTime(), idf_avm.getDouble(AvailabilityManager_NightCycleFields::CyclingRunTime).get());


  // Check the Zone Lists

  // Control Zones: single zone means no ZoneList, just the name of the zone here
  ASSERT_TRUE(idf_avm.getString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName));
  EXPECT_EQ(z1.name().get(), idf_avm.getString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName).get());

  // Heating Control Zones
  boost::optional<WorkspaceObject> idf_heatingControlThermalZones = idf_avm.getTarget(AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName);
  ASSERT_TRUE(idf_heatingControlThermalZones);
  EXPECT_EQ(heatingControlThermalZones.size(), idf_heatingControlThermalZones->extensibleGroups().size());

  // Heating Zone Fans Only Zones
  boost::optional<WorkspaceObject> idf_heatingZoneFansOnlyThermalZones = idf_avm.getTarget(AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName);
  ASSERT_TRUE(idf_heatingZoneFansOnlyThermalZones);
  EXPECT_EQ(heatingZoneFansOnlyThermalZones.size(), idf_heatingZoneFansOnlyThermalZones->extensibleGroups().size());


  // Cooling, here it should have been defaulted to all zones served by the AirLoopHVAC
  boost::optional<WorkspaceObject> idf_coolingControlThermalZones = idf_avm.getTarget(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName);
  ASSERT_TRUE(idf_coolingControlThermalZones);
  EXPECT_EQ(a.thermalZones().size(), idf_coolingControlThermalZones->extensibleGroups().size());



  // Second Check

  // If we set to CycleOnAny, the cooling zone list should be present because no defaulting needed
  EXPECT_TRUE(avm.setControlType("CycleOnAny"));

  // Re translate
  w = ft.translateModel(m);

  // Should have 2 zonelists only (control zone is one zone = just name and no zonelist, and no cooling one)
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObjectVector idfObjs2(w.getObjectsByType(IddObjectType::AvailabilityManager_NightCycle));
  EXPECT_EQ(1u, idfObjs2.size());
  WorkspaceObject idf_avm2(idfObjs2[0]);

  boost::optional<WorkspaceObject> idf_coolingControlThermalZones2 = idf_avm2.getTarget(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName);
  EXPECT_FALSE(idf_coolingControlThermalZones2);

}


