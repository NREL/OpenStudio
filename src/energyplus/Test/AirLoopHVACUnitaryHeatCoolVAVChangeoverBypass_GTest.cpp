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

#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>
#include <algorithm>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/**
 * Tests only the minimumRuntimeBeforeOperatingModeChange and the plenumorMixer of the ForwardTranslator which I exposed after the fact
 **/
TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_plenumorMixer) {
  ForwardTranslator ft;

  Model m;
  FanConstantVolume fan(m);
  CoilHeatingElectric heatingCoil(m);
  CoilCoolingDXSingleSpeed coolingCoil(m);

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);

  EXPECT_TRUE(unitary.setMinimumRuntimeBeforeOperatingModeChange(0.25));
  EXPECT_EQ(0.25, unitary.minimumRuntimeBeforeOperatingModeChange());

  AirLoopHVAC a(m);
  ThermalZone z(m);
  ThermalZone plenumZone(m);
  EXPECT_TRUE(a.addBranchForZone(z));
  EXPECT_TRUE(z.setReturnPlenum(plenumZone));
  AirLoopHVACZoneMixer mixer = a.zoneMixer();

  auto modelObjects = a.demandComponents(z, mixer);
  auto plenums = subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects);
  ASSERT_EQ(1u, plenums.size());
  AirLoopHVACReturnPlenum plenum = plenums[0];

  Node supplyOutletNode = a.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  // Nested lambda helper to check if we find a specific node name in the extensible fields (=Inlet nodes) of the AirLoopHVAC:ZoneMixer or
  // AirLoopHVAC:ReturnPlenum fields (they have the same extensible groups structure)
  auto checkIfFoundInExtensibleGroups = [](const WorkspaceObject& idf_object, const std::string& plenumNodeName, unsigned extFieldIndex) {
    std::vector<IdfExtensibleGroup> groups = idf_object.extensibleGroups();
    auto it = std::find_if(groups.begin(), groups.end(), [&plenumNodeName, extFieldIndex](IdfExtensibleGroup eg) {
      if (boost::optional<std::string> _inletNodeName = eg.getString(extFieldIndex)) {
        if (openstudio::istringEqual(plenumNodeName, _inletNodeName.get())) {
          return true;
        }
      }
      return false;
    });
    return it != groups.end();
  };

  // Not connected
  {
    EXPECT_EQ(1u, mixer.inletModelObjects().size());
    EXPECT_EQ(1u, plenum.inletModelObjects().size());
    EXPECT_FALSE(unitary.plenumorMixer());

    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> idf_unitarys = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass);
    std::vector<WorkspaceObject> idf_zonemixers = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ZoneMixer);
    std::vector<WorkspaceObject> idf_returnplenums = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ReturnPlenum);
    EXPECT_EQ(1u, idf_unitarys.size());
    EXPECT_EQ(1u, idf_zonemixers.size());
    EXPECT_EQ(1u, idf_returnplenums.size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC).size());

    WorkspaceObject idf_unitary = idf_unitarys[0];
    WorkspaceObject idf_zonemixer = idf_zonemixers[0];
    WorkspaceObject idf_returnplenum = idf_returnplenums[0];

    // Check the FT of the new numeric field too
    EXPECT_EQ(0.25, idf_unitary.getDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumRuntimeBeforeOperatingModeChange).get());

    // Both objects should only have one inlet node
    EXPECT_EQ(1u, idf_zonemixer.numExtensibleGroups());
    EXPECT_EQ(1u, idf_returnplenum.numExtensibleGroups());

    boost::optional<std::string> _plenumNodeName =
      idf_unitary.getString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PlenumorMixerInletNodeName, false, true);
    EXPECT_FALSE(_plenumNodeName);
  }

  // Connected to AirLoopHVAC:ZoneMixer
  {
    EXPECT_TRUE(unitary.setPlenumorMixer(mixer));
    EXPECT_EQ(2u, mixer.inletModelObjects().size());
    EXPECT_EQ(1u, plenum.inletModelObjects().size());
    ASSERT_TRUE(unitary.plenumorMixer());
    EXPECT_EQ(mixer.handle(), unitary.plenumorMixer()->handle());

    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> idf_unitarys = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass);
    std::vector<WorkspaceObject> idf_zonemixers = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ZoneMixer);
    std::vector<WorkspaceObject> idf_returnplenums = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ReturnPlenum);
    EXPECT_EQ(1u, idf_unitarys.size());
    EXPECT_EQ(1u, idf_zonemixers.size());
    EXPECT_EQ(1u, idf_returnplenums.size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC).size());

    WorkspaceObject idf_unitary = idf_unitarys[0];
    WorkspaceObject idf_zonemixer = idf_zonemixers[0];
    WorkspaceObject idf_returnplenum = idf_returnplenums[0];

    // The ZoneMixer should now have two inlet nodes, and we can find plenum node in its extensible fields
    EXPECT_EQ(2u, idf_zonemixer.numExtensibleGroups());
    EXPECT_EQ(1u, idf_returnplenum.numExtensibleGroups());

    boost::optional<std::string> _plenumNodeName =
      idf_unitary.getString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PlenumorMixerInletNodeName, false, true);
    ASSERT_TRUE(_plenumNodeName);

    EXPECT_TRUE(checkIfFoundInExtensibleGroups(idf_zonemixer, _plenumNodeName.get(), AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName))
      << "Expected to find Plenum Node Name='" << _plenumNodeName.get() << "' in the AirLoopHVAC:ZoneMixer extensible fields";

    EXPECT_FALSE(checkIfFoundInExtensibleGroups(idf_returnplenum, _plenumNodeName.get(), AirLoopHVAC_ReturnPlenumExtensibleFields::InletNodeName))
      << "Did not expect to find Plenum Node Name='" << _plenumNodeName.get() << "' in the AirLoopHVAC:ReturnPlenum extensible fields";
  }

  // Connected to AirLoopHVAC:ReturnPlenum
  {
    EXPECT_TRUE(unitary.setPlenumorMixer(plenum));
    EXPECT_EQ(1u, mixer.inletModelObjects().size());
    EXPECT_EQ(2u, plenum.inletModelObjects().size());
    ASSERT_TRUE(unitary.plenumorMixer());
    EXPECT_EQ(plenum, unitary.plenumorMixer().get());

    Workspace workspace = ft.translateModel(m);

    std::vector<WorkspaceObject> idf_unitarys = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass);
    std::vector<WorkspaceObject> idf_zonemixers = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ZoneMixer);
    std::vector<WorkspaceObject> idf_returnplenums = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_ReturnPlenum);
    EXPECT_EQ(1u, idf_unitarys.size());
    EXPECT_EQ(1u, idf_zonemixers.size());
    EXPECT_EQ(1u, idf_returnplenums.size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC).size());

    WorkspaceObject idf_unitary = idf_unitarys[0];
    WorkspaceObject idf_zonemixer = idf_zonemixers[0];
    WorkspaceObject idf_returnplenum = idf_returnplenums[0];

    EXPECT_EQ(1u, idf_zonemixer.numExtensibleGroups());
    EXPECT_EQ(2u, idf_returnplenum.numExtensibleGroups());

    boost::optional<std::string> _plenumNodeName =
      idf_unitary.getString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PlenumorMixerInletNodeName, false, true);
    ASSERT_TRUE(_plenumNodeName);

    EXPECT_FALSE(checkIfFoundInExtensibleGroups(idf_zonemixer, _plenumNodeName.get(), AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName))
      << "Expected to find Plenum Node Name='" << _plenumNodeName.get() << "' in the AirLoopHVAC:ZoneMixer extensible fields";

    EXPECT_TRUE(checkIfFoundInExtensibleGroups(idf_returnplenum, _plenumNodeName.get(), AirLoopHVAC_ReturnPlenumExtensibleFields::InletNodeName))
      << "Did not expect to find Plenum Node Name='" << _plenumNodeName.get() << "' in the AirLoopHVAC:ReturnPlenum extensible fields";
  }
}
