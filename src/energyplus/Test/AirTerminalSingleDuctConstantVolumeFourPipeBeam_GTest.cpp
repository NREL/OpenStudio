/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"
#include "../../model/CoilCoolingFourPipeBeam.hpp"
#include "../../model/CoilCoolingFourPipeBeam_Impl.hpp"
#include "../../model/CoilHeatingFourPipeBeam.hpp"
#include "../../model/CoilHeatingFourPipeBeam_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Lights.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctConstantVolumeFourPipeBeam) {

  ForwardTranslator ft;

  // Create a one space, one zone model
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  LightsDefinition ld(m);
  Lights l(ld);
  l.setSpace(s);

  // Create an ATU, with no coils for now
  AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m);
  ScheduleConstant sch(m);
  EXPECT_TRUE(atu.setPrimaryAirAvailabilitySchedule(sch));

  // Connect it to an AirLoopHVAC
  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  // It is connected to an AirLoopHVAC, but no plantLoop, so ForwardTranslation should not translate it
  {
    Workspace w = ft.translateModel(m);

    // Check the ATU isn't present
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Add a Cooling Coil
  CoilCoolingFourPipeBeam cc(m);
  atu.setCoolingCoil(cc);
  // and a heating coil
  CoilHeatingFourPipeBeam hc(m);
  atu.setHeatingCoil(hc);

  // They must be connected to a PlantLoop too
  PlantLoop chw_p(m);
  chw_p.addDemandBranchForComponent(cc);

  PlantLoop hw_p(m);
  hw_p.addDemandBranchForComponent(hc);

  // Hardsize an autosize field
  atu.setDesignHotWaterVolumeFlowRate(0.005);

  // This time, it should have been translated
  {
    Workspace w = ft.translateModel(m);

    // Check the ATU
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_atu(idfObjs[0]);

    EXPECT_EQ(atu.name().get(), idf_atu.name().get());

    // Check that the primary air schedule was set
    EXPECT_EQ(atu.primaryAirAvailabilitySchedule().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName).get());
    EXPECT_EQ(sch.name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName).get());

    // Check the Node connections
    // Primary Air
    EXPECT_EQ(atu.primaryAirInletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName).get());
    EXPECT_EQ(atu.primaryAirOutletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName).get());

    // Chilled Water
    EXPECT_EQ(atu.chilledWaterInletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterInletNodeName).get());
    EXPECT_EQ(atu.chilledWaterOutletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ChilledWaterOutletNodeName).get());

    // Hot Water
    EXPECT_EQ(atu.hotWaterInletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterInletNodeName).get());
    EXPECT_EQ(atu.hotWaterOutletNode().get().name().get(),
              idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HotWaterOutletNodeName).get());

    // Check autosizable fields
    EXPECT_TRUE(openstudio::istringEqual(
      "autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate).get()));
    EXPECT_TRUE(openstudio::istringEqual(
      "autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate).get()));
    EXPECT_EQ(0.005, idf_atu.getDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate).get());
    EXPECT_TRUE(
      openstudio::istringEqual("autosize", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength).get()));

    // Check that the default value was assigned
    EXPECT_EQ(atu.ratedPrimaryAirFlowRateperBeamLength(),
              idf_atu.getDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength).get());

    // Check the ADU
    WorkspaceObjectVector idfObjs2 = w.getObjectsByType(IddObjectType::ZoneHVAC_AirDistributionUnit);
    EXPECT_EQ(1u, idfObjs2.size());
    WorkspaceObject idf_adu(idfObjs2[0]);

    // Check node match
    EXPECT_EQ(idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName).get(),
              idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName).get());

    EXPECT_EQ("AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam",
              idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType).get());
    EXPECT_EQ(idf_atu.name().get(), idf_adu.getString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName).get());
  }
}
