/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "OpenStudioLibFixture.hpp"

#include "../IconLibrary.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <QPixmap>

using namespace openstudio;

TEST_F(OpenStudioLibFixture, IconLibrary_Icon)
{
  std::vector<IddObjectType> iddObjectTypes;
  iddObjectTypes.push_back(IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem);
  iddObjectTypes.push_back(IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir);
  iddObjectTypes.push_back(IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled);
  iddObjectTypes.push_back(IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat);
  iddObjectTypes.push_back(IddObjectType::OS_Boiler_HotWater);
  //iddObjectTypes.push_back(IddObjectType::OS_Building);
  //iddObjectTypes.push_back(IddObjectType::OS_BuildingStory);
  iddObjectTypes.push_back(IddObjectType::OS_Chiller_Electric_EIR);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Cooling_Water);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Electric);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Gas);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Water);
  //iddObjectTypes.push_back(IddObjectType::OS_Construction);
  //iddObjectTypes.push_back(IddObjectType::OS_Construction_CfactorUndergroundWall);
  //iddObjectTypes.push_back(IddObjectType::OS_Construction_FfactorGroundFloor);
  //iddObjectTypes.push_back(IddObjectType::OS_Construction_InternalSource);
  //iddObjectTypes.push_back(IddObjectType::OS_Construction_WindowDataFile);
  //iddObjectTypes.push_back(IddObjectType::OS_Daylighting_Control);
  //iddObjectTypes.push_back(IddObjectType::OS_DefaultConstructionSet);
  //iddObjectTypes.push_back(IddObjectType::OS_DefaultScheduleSet);
  //iddObjectTypes.push_back(IddObjectType::OS_DesignSpecification_OutdoorAir);
  //iddObjectTypes.push_back(IddObjectType::OS_ElectricEquipment);
  //iddObjectTypes.push_back(IddObjectType::OS_ElectricEquipment_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  //iddObjectTypes.push_back(IddObjectType::OS_Facility);
  iddObjectTypes.push_back(IddObjectType::OS_Fan_ConstantVolume);
  iddObjectTypes.push_back(IddObjectType::OS_Fan_VariableVolume);
  //iddObjectTypes.push_back(IddObjectType::OS_GasEquipment);
  //iddObjectTypes.push_back(IddObjectType::OS_GasEquipment_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_IlluminanceMap);
  //iddObjectTypes.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  //iddObjectTypes.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  //iddObjectTypes.push_back(IddObjectType::OS_InternalMass);
  //iddObjectTypes.push_back(IddObjectType::OS_InternalMass_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_Lights);
  //iddObjectTypes.push_back(IddObjectType::OS_Lights_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_Luminaire);
  //iddObjectTypes.push_back(IddObjectType::OS_Luminaire_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_Material);
  //iddObjectTypes.push_back(IddObjectType::OS_Material_AirGap);
  //iddObjectTypes.push_back(IddObjectType::OS_Material_AirWall);
  //iddObjectTypes.push_back(IddObjectType::OS_Material_InfraredTransparent);
  //iddObjectTypes.push_back(IddObjectType::OS_Material_NoMass);
  //iddObjectTypes.push_back(IddObjectType::OS_Material_RoofVegetation);
  //iddObjectTypes.push_back(IddObjectType::OS_OtherEquipment);
  //iddObjectTypes.push_back(IddObjectType::OS_OtherEquipment_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_People);
  //iddObjectTypes.push_back(IddObjectType::OS_People_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_Pump_VariableSpeed);
  //iddObjectTypes.push_back(IddObjectType::OS_Schedule_Compact);
  //iddObjectTypes.push_back(IddObjectType::OS_Schedule_Constant);
  //iddObjectTypes.push_back(IddObjectType::OS_Schedule_FixedInterval);
  //iddObjectTypes.push_back(IddObjectType::OS_Schedule_Ruleset);
  //iddObjectTypes.push_back(IddObjectType::OS_Schedule_VariableInterval);
  //iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_MixedAir);
  //iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_Scheduled);
  //iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_SingleZone_Reheat);
  //iddObjectTypes.push_back(IddObjectType::OS_ShadingSurfaceGroup);
  //iddObjectTypes.push_back(IddObjectType::OS_ShadingSurface);
  //iddObjectTypes.push_back(IddObjectType::OS_Space);
  //iddObjectTypes.push_back(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
  //iddObjectTypes.push_back(IddObjectType::OS_SpaceType);
  //iddObjectTypes.push_back(IddObjectType::OS_SteamEquipment);
  //iddObjectTypes.push_back(IddObjectType::OS_SteamEquipment_Definition);
  //iddObjectTypes.push_back(IddObjectType::OS_SubSurface);
  //iddObjectTypes.push_back(IddObjectType::OS_Surface);
  iddObjectTypes.push_back(IddObjectType::OS_ThermalZone);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Blind);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Gas);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GasMixture);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Screen);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Shade);
  //iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
  iddObjectTypes.push_back(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner);
  iddObjectTypes.push_back(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump);

  for (IddObjectType iddObjectType : iddObjectTypes){
    const QPixmap* pixmap = IconLibrary::Instance().findIcon(iddObjectType.value());
    EXPECT_TRUE(pixmap) << "Could not find icon for IddObjectType = '" << iddObjectType.valueName() << "'";
  }
}

TEST_F(OpenStudioLibFixture, IconLibrary_MiniIcon)
{
  std::vector<IddObjectType> iddObjectTypes;
  iddObjectTypes.push_back(IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem);
  iddObjectTypes.push_back(IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir);
  iddObjectTypes.push_back(IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled);
  iddObjectTypes.push_back(IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat);
  iddObjectTypes.push_back(IddObjectType::OS_Boiler_HotWater);
  iddObjectTypes.push_back(IddObjectType::OS_Building);
  iddObjectTypes.push_back(IddObjectType::OS_BuildingStory);
  iddObjectTypes.push_back(IddObjectType::OS_Chiller_Electric_EIR);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Cooling_Water);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Electric);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Gas);
  iddObjectTypes.push_back(IddObjectType::OS_Coil_Heating_Water);
  iddObjectTypes.push_back(IddObjectType::OS_Construction);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_CfactorUndergroundWall);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_FfactorGroundFloor);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_InternalSource);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_WindowDataFile);
  iddObjectTypes.push_back(IddObjectType::OS_Daylighting_Control);
  iddObjectTypes.push_back(IddObjectType::OS_DefaultConstructionSet);
  iddObjectTypes.push_back(IddObjectType::OS_DefaultScheduleSet);
  iddObjectTypes.push_back(IddObjectType::OS_DesignSpecification_OutdoorAir);
  iddObjectTypes.push_back(IddObjectType::OS_ElectricEquipment);
  iddObjectTypes.push_back(IddObjectType::OS_ElectricEquipment_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  iddObjectTypes.push_back(IddObjectType::OS_Facility);
  iddObjectTypes.push_back(IddObjectType::OS_Fan_ConstantVolume);
  iddObjectTypes.push_back(IddObjectType::OS_Fan_VariableVolume);
  iddObjectTypes.push_back(IddObjectType::OS_GasEquipment);
  iddObjectTypes.push_back(IddObjectType::OS_GasEquipment_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_IlluminanceMap);
  iddObjectTypes.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  iddObjectTypes.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  iddObjectTypes.push_back(IddObjectType::OS_InternalMass);
  iddObjectTypes.push_back(IddObjectType::OS_InternalMass_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_Lights);
  iddObjectTypes.push_back(IddObjectType::OS_Lights_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_Luminaire);
  iddObjectTypes.push_back(IddObjectType::OS_Luminaire_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_Material);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirGap);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirWall);
  iddObjectTypes.push_back(IddObjectType::OS_Material_InfraredTransparent);
  iddObjectTypes.push_back(IddObjectType::OS_Material_NoMass);
  iddObjectTypes.push_back(IddObjectType::OS_Material_RoofVegetation);
  iddObjectTypes.push_back(IddObjectType::OS_OtherEquipment);
  iddObjectTypes.push_back(IddObjectType::OS_OtherEquipment_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_People);
  iddObjectTypes.push_back(IddObjectType::OS_People_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_Pump_VariableSpeed);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Compact);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Constant);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_FixedInterval);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Ruleset);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_VariableInterval);
  iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_MixedAir);
  iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_Scheduled);
  iddObjectTypes.push_back(IddObjectType::OS_SetpointManager_SingleZone_Reheat);
  iddObjectTypes.push_back(IddObjectType::OS_ShadingSurfaceGroup);
  iddObjectTypes.push_back(IddObjectType::OS_ShadingSurface);
  iddObjectTypes.push_back(IddObjectType::OS_Space);
  iddObjectTypes.push_back(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
  iddObjectTypes.push_back(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
  iddObjectTypes.push_back(IddObjectType::OS_SpaceType);
  iddObjectTypes.push_back(IddObjectType::OS_SteamEquipment);
  iddObjectTypes.push_back(IddObjectType::OS_SteamEquipment_Definition);
  iddObjectTypes.push_back(IddObjectType::OS_SubSurface);
  iddObjectTypes.push_back(IddObjectType::OS_Surface);
  iddObjectTypes.push_back(IddObjectType::OS_ThermalZone);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Blind);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Gas);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GasMixture);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Screen);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Shade);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
  iddObjectTypes.push_back(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner);
  iddObjectTypes.push_back(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump);

  for (IddObjectType iddObjectType : iddObjectTypes){
    const QPixmap* pixmap = IconLibrary::Instance().findMiniIcon(iddObjectType.value());
    EXPECT_TRUE(pixmap) << "Could not find mini-icon for IddObjectType = '" << iddObjectType.valueName() << "'";
  }

}
