/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingWater.hpp"
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
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Cooling_DX_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
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

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_CoilTypes) {
  // Test for #4330 - AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass does not accept a Coil:Cooling:DX:VariableSpeed
  // Test for #4329 - Air nodes not set for Coil:Cooling:DX:TwoStageWithHumidityControlMode on AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass

  ForwardTranslator ft;

  struct NodeNames
  {
    std::string oaMixerOutlet;
    std::string coolingCoilInlet;
    std::string coolingCoilOutlet;
    std::string heatingCoilInlet;
    std::string heatingCoilOutlet;
    std::string fanInlet;
    std::string fanOutlet;
  };

  auto getObjects = [](const Workspace& w) -> std::tuple<WorkspaceObject, WorkspaceObject, WorkspaceObject, WorkspaceObject> {
    WorkspaceObjectVector idf_unitarys(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass));
    EXPECT_EQ(1u, idf_unitarys.size());
    WorkspaceObject idf_unitary(idf_unitarys[0]);

    auto idf_heatingCoil_ = idf_unitary.getTarget(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilName);

    auto idf_coolingCoil_ = idf_unitary.getTarget(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoilName);

    auto idf_oamixer_ = idf_unitary.getTarget(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirMixerName);

    auto idf_fan_ = idf_unitary.getTarget(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanName);

    return {idf_oamixer_.get(), idf_coolingCoil_.get(), idf_heatingCoil_.get(), idf_fan_.get()};
  };

  auto validateNodeMatch = [](const NodeNames& n, const std::string& testName = "", bool drawThrough = true) {
    EXPECT_FALSE(n.oaMixerOutlet.empty());
    EXPECT_FALSE(n.coolingCoilInlet.empty());
    EXPECT_FALSE(n.coolingCoilOutlet.empty());
    EXPECT_FALSE(n.heatingCoilInlet.empty());
    EXPECT_FALSE(n.heatingCoilOutlet.empty());
    EXPECT_FALSE(n.fanInlet.empty());
    if (drawThrough) {
      EXPECT_EQ(n.oaMixerOutlet, n.coolingCoilInlet) << testName << " [" << drawThrough << "]: OA Mixer Outlet doesn't match Cooling Coil Inlet";
      EXPECT_EQ(n.coolingCoilOutlet, n.heatingCoilInlet) << testName << " [" << drawThrough << "]: Cooling Outlet doesn't match Heating Coil Inlet";
      EXPECT_EQ(n.heatingCoilOutlet, n.fanInlet) << testName << " [" << drawThrough << "]: Heating Outlet doesn't match Fan Inlet";
    } else {
      EXPECT_EQ(n.oaMixerOutlet, n.fanInlet) << testName << " [" << drawThrough << "]: OA Mixer Outlet doesn't match Fan Inlet";
      EXPECT_EQ(n.fanOutlet, n.coolingCoilInlet) << testName << " [" << drawThrough << "]: Fan Outlet doesn't match Cooling Coil Inlet";
      EXPECT_EQ(n.coolingCoilOutlet, n.heatingCoilInlet) << testName << " [" << drawThrough << "]: Cooling Outlet doesn't match Heating Coil Inlet";
    }
  };

  {
    constexpr auto test_name = "CoilHeatingDXSingleSpeed, CoilCoolingDXSingleSpeed";
    for (bool drawThrough : {true, false}) {
      Model m;
      CoilHeatingDXSingleSpeed heatingCoil(m);
      CoilCoolingDXSingleSpeed coolingCoil(m);
      FanConstantVolume fan(m);
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);
      unitary.setName("UnitarySystem");
      EXPECT_TRUE(unitary.setSupplyAirFanPlacement(drawThrough ? "DrawThrough" : "BlowThrough"));

      AirLoopHVAC a(m);
      Node supplyOutletNode = a.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      Workspace w = ft.translateModel(m);

      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_SingleSpeed).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed).size());

      auto getNodeNames = [&getObjects](const auto& w) {
        auto [idf_oamixer, idf_coolingCoil, idf_heatingCoil, idf_fan] = getObjects(w);

        EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_DX_SingleSpeed);
        EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_DX_SingleSpeed);

        NodeNames n;
        n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
        n.coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get();
        n.coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get();
        n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName).get();
        n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName).get();
        n.fanInlet = idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get();
        n.fanOutlet = idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get();

        return n;
      };

      validateNodeMatch(getNodeNames(w), test_name, drawThrough);
    }
  }

  {
    constexpr auto test_name = "CoilHeatingDXVariableSpeed, CoilCoolingDXVariableSpeed";
    for (bool drawThrough : {true, false}) {
      ForwardTranslator ft;
      Model m;
      CoilHeatingDXVariableSpeed heatingCoil(m);
      CoilCoolingDXVariableSpeed coolingCoil(m);
      FanConstantVolume fan(m);
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);
      unitary.setName("UnitarySystem");
      EXPECT_TRUE(unitary.setSupplyAirFanPlacement(drawThrough ? "DrawThrough" : "BlowThrough"));

      AirLoopHVAC a(m);
      Node supplyOutletNode = a.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      Workspace w = ft.translateModel(m);

      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed).size());

      auto getNodeNames = [&getObjects](const auto& w) {
        auto [idf_oamixer, idf_coolingCoil, idf_heatingCoil, idf_fan] = getObjects(w);

        EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);
        EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);

        NodeNames n;
        n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
        n.coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName).get();
        n.coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName).get();
        n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName).get();
        n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName).get();
        n.fanInlet = idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get();
        n.fanOutlet = idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get();

        return n;
      };

      validateNodeMatch(getNodeNames(w), test_name, drawThrough);
    }
  }

  {
    constexpr auto test_name = "CoilHeatingElectric, CoilCoolingDXTwoStageWithHumidityControlMode";
    for (bool drawThrough : {true, false}) {
      ForwardTranslator ft;
      Model m;
      CoilHeatingElectric heatingCoil(m);
      CoilCoolingDXTwoStageWithHumidityControlMode coolingCoil(m);
      FanConstantVolume fan(m);
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);
      unitary.setName("UnitarySystem");
      EXPECT_TRUE(unitary.setSupplyAirFanPlacement(drawThrough ? "DrawThrough" : "BlowThrough"));

      AirLoopHVAC a(m);
      Node supplyOutletNode = a.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      Workspace w = ft.translateModel(m);

      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Electric).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode).size());

      auto getNodeNames = [&getObjects](const auto& w) {
        auto [idf_oamixer, idf_coolingCoil, idf_heatingCoil, idf_fan] = getObjects(w);

        EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode);
        EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_Electric);

        NodeNames n;
        n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
        n.coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName).get();
        n.coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName).get();
        n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_ElectricFields::AirInletNodeName).get();
        n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_ElectricFields::AirOutletNodeName).get();
        n.fanInlet = idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get();
        n.fanOutlet = idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get();

        return n;
      };

      validateNodeMatch(getNodeNames(w), test_name, drawThrough);
    }
  }

  {
    constexpr auto test_name = "CoilHeatingWater, CoilCoolingDXSingleSpeed";
    for (bool drawThrough : {true, false}) {
      ForwardTranslator ft;
      Model m;
      CoilHeatingWater heatingCoil(m);
      CoilCoolingDXSingleSpeed coolingCoil(m);
      FanConstantVolume fan(m);
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);
      unitary.setName("UnitarySystem");
      EXPECT_TRUE(unitary.setSupplyAirFanPlacement(drawThrough ? "DrawThrough" : "BlowThrough"));

      AirLoopHVAC a(m);
      Node supplyOutletNode = a.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      Workspace w = ft.translateModel(m);

      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Water).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed).size());

      auto getNodeNames = [&getObjects](const auto& w) {
        auto [idf_oamixer, idf_coolingCoil, idf_heatingCoil, idf_fan] = getObjects(w);

        EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_DX_SingleSpeed);
        EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_Water);

        NodeNames n;
        n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
        n.coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get();
        n.coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get();
        n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_WaterFields::AirInletNodeName).get();
        n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_WaterFields::AirOutletNodeName).get();
        n.fanInlet = idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get();
        n.fanOutlet = idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get();

        return n;
      };

      validateNodeMatch(getNodeNames(w), test_name, drawThrough);
    }
  }

  {
    constexpr auto test_name = "CoilHeatingGas, CoilSystemCoolingDXHeatExchangerAssisted";
    for (bool drawThrough : {true, false}) {
      ForwardTranslator ft;
      Model m;
      CoilHeatingGas heatingCoil(m);
      CoilSystemCoolingDXHeatExchangerAssisted coolingCoil(m);
      FanConstantVolume fan(m);
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);
      unitary.setName("UnitarySystem");
      EXPECT_TRUE(unitary.setSupplyAirFanPlacement(drawThrough ? "DrawThrough" : "BlowThrough"));

      AirLoopHVAC a(m);
      Node supplyOutletNode = a.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      Workspace w = ft.translateModel(m);

      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Fuel).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent).size());
      EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed).size());

      auto getNodeNames = [&getObjects](const auto& w) {
        auto [idf_oamixer, idf_coilSystem, idf_heatingCoil, idf_fan] = getObjects(w);

        EXPECT_EQ(idf_coilSystem.iddObject().type(), IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted);

        // this is fun, we have a CoilSystem that wraps a HX and a Cooling Coil... We'll do some manual node check here
        auto idf_coolingCoil = idf_coilSystem.getTarget(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoilName).get();
        EXPECT_EQ(idf_coolingCoil.iddObject().type(), IddObjectType::Coil_Cooling_DX_SingleSpeed);
        EXPECT_EQ(idf_heatingCoil.iddObject().type(), IddObjectType::Coil_Heating_Fuel);

        auto idf_HX = idf_coilSystem.getTarget(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchangerName).get();
        EXPECT_EQ(idf_HX.iddObject().type(), IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent);

        // Connected to the branch / other components
        std::string hxSupplyInlet = idf_HX.getString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName).get();
        std::string hxExhaustOutlet = idf_HX.getString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName).get();

        // To/From Cooling coil
        std::string hxSupplyOutlet = idf_HX.getString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName).get();
        std::string hxExhaustInlet = idf_HX.getString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName).get();
        std::string coolingCoilInlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get();
        std::string coolingCoilOutlet = idf_coolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get();
        EXPECT_EQ(hxSupplyOutlet, coolingCoilInlet);
        EXPECT_EQ(hxExhaustInlet, coolingCoilOutlet);

        NodeNames n;
        n.oaMixerOutlet = idf_oamixer.getString(OutdoorAir_MixerFields::MixedAirNodeName).get();
        // This are atually the HX Suppy Inlet / Exhaust Outlet
        n.coolingCoilInlet = hxSupplyInlet;
        n.coolingCoilOutlet = hxExhaustOutlet;
        n.heatingCoilInlet = idf_heatingCoil.getString(Coil_Heating_FuelFields::AirInletNodeName).get();
        n.heatingCoilOutlet = idf_heatingCoil.getString(Coil_Heating_FuelFields::AirOutletNodeName).get();
        n.fanInlet = idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get();
        n.fanOutlet = idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get();

        return n;
      };

      validateNodeMatch(getNodeNames(w), test_name, drawThrough);
    }
  }
}
