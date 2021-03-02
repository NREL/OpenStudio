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
// #include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"

#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingWater.hpp"

#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>

#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
// Not yet supported in OS
// #include <utilities/idd/Coil_Heating_Steam_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_NodeConnections) {

  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  ThermalZone z(m);
  EXPECT_TRUE(vrf.addToThermalZone(z));

  ForwardTranslator ft;

  // VRF defaults to DrawThrough, so components are ordered like
  // InletNode ----- (Mixer) ---- CoolingCoil ----- HeatingCoil ----- Fan ---- (Supplemental Heating Coil) -------- OutletNode

  // No Supplemental HC
  {
    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Electric);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(0u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];

    // Check Node Connections
    // --- CC --- HC --- Fan
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingElectric
  {
    CoilHeatingElectric supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Electric);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingGas (translates to Coil_Heating_Fuel)
  {
    CoilHeatingGas supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Fuel);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_FuelFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingWater
  {
    CoilHeatingWater supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Water);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_WaterFields::AirInletNodeName).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_SupplementalHeating) {

  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  ThermalZone z(m);
  EXPECT_TRUE(vrf.addToThermalZone(z));

  ForwardTranslator ft;

  {
    vrf.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(19.5);

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    EXPECT_EQ(1u, idf_vrfs.size());
    WorkspaceObject idf_vrf = idf_vrfs[0];

    EXPECT_TRUE(openstudio::istringEqual(
      "Autosize", idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater).get()));
    EXPECT_EQ(
      19.5,
      idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation).get());
  }

  {
    vrf.setMaximumSupplyAirTemperaturefromSupplementalHeater(35.2);
    vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(15.5);

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    EXPECT_EQ(1u, idf_vrfs.size());
    WorkspaceObject idf_vrf = idf_vrfs[0];

    EXPECT_EQ(35.2, idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater).get());
    EXPECT_EQ(
      15.5,
      idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation).get());
  }
}
