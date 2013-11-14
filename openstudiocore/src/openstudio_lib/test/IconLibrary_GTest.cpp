/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include <openstudio_lib/test/OpenStudioLibFixture.hpp>

#include <openstudio_lib/IconLibrary.hpp>

#include <utilities/idd/IddFactory.hxx>

#include <QPixmap>

#include <boost/foreach.hpp>

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

  BOOST_FOREACH(IddObjectType iddObjectType, iddObjectTypes){
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

  BOOST_FOREACH(IddObjectType iddObjectType, iddObjectTypes){
    const QPixmap* pixmap = IconLibrary::Instance().findMiniIcon(iddObjectType.value());
    EXPECT_TRUE(pixmap) << "Could not find mini-icon for IddObjectType = '" << iddObjectType.valueName() << "'";
  }

}
