#include <openstudio_lib/IconLibrary.hpp>
#include <utilities/idd/IddEnums.hxx>

using std::map;

namespace openstudio {

IconLibrary* IconLibrary::s_instance=NULL;

IconLibrary& IconLibrary::Instance()
{
  if( !s_instance)
  {
    s_instance = new IconLibrary();
  }

  return *s_instance;
}

const QPixmap* IconLibrary::findIcon( unsigned int v) const
{
  map<unsigned int, QPixmap*>::const_iterator i = m_icons.find(v);
  if(i!=m_icons.end())
    return (*i).second;

  return NULL;

}

const QPixmap* IconLibrary::findMiniIcon( unsigned int v) const
{
  map<unsigned int, QPixmap*>::const_iterator i = m_miniIcons.find(v);
  if(i!=m_miniIcons.end())
    return (*i).second;

  return NULL;

}

IconLibrary::IconLibrary()
{
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).value()] = new QPixmap(":images/OAMixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryCoolOnly).value()] = new QPixmap(":images/DXCoolingCoil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir).value()] = new QPixmap(":images/heat_pump3.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_ZoneMixer).value()] = new QPixmap(":images/mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_ZoneSplitter).value()] = new QPixmap(":images/splitter.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat).value()] = new QPixmap(":images/parallel_fan_terminal.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled).value()] = new QPixmap(":images/direct-air.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat).value()] = new QPixmap(":images/vav-reheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_HotWater).value()] = new QPixmap(":images/boiler.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_Steam).value()] = new QPixmap(":images/boiler_steam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Electric_EIR).value()] = new QPixmap(":images/chiller_air.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed).value()] = new QPixmap(":images/dxcoolingcoil_singlespeed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed).value()] = new QPixmap(":images/dxcoolingcoil_2speed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/coilcoolinglowtemprad_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/coilcoolinglowtemprad_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/wahpDXCC.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_Water).value()] = new QPixmap(":images/cool_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed).value()] = new QPixmap(":images/coil_ht_dx_singlespeed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Electric).value()] = new QPixmap(":images/electric_furnace.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas).value()] = new QPixmap(":images/furnace.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water).value()] = new QPixmap(":images/heat_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/coilheatinglowtemprad_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/coilheatinglowtemprad_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/wahpDXHC.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_SingleSpeed).value()] = new QPixmap(":images/cooling_tower.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Connector_Mixer).value()] = new QPixmap(":images/mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Connector_Splitter).value()] = new QPixmap(":images/splitter.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictCooling).value()] = new QPixmap(":images/districtcooling.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictHeating).value()] = new QPixmap(":images/districtheating.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial).value()] = new QPixmap(":images/directEvap.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ConstantVolume).value()] = new QPixmap(":images/fan_constant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_OnOff).value()] = new QPixmap(":images/fan_on_off.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_VariableVolume).value()] = new QPixmap(":images/fan_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical).value()] = new QPixmap(":images/ground_heat_exchanger_vertical.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent).value()] = new QPixmap(":images/heat_transfer_outdoorair.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed).value()] = new QPixmap(":images/evap_fluid_cooler.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Adiabatic).value()] = new QPixmap(":images/pipe.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_ConstantSpeed).value()] = new QPixmap(":images/pump_constant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_VariableSpeed).value()] = new QPixmap(":images/pump_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalZone).value()] = new QPixmap(":images/zone.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Mixed).value()] = new QPixmap(":images/water_heater.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Connections).value()] = new QPixmap(":images/water_connection.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment).value()] = new QPixmap(":images/sink.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric).value()] = new QPixmap(":images/baseboard_electric.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water).value()] = new QPixmap(":images/baseboard_water.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard).value()] = new QPixmap(":images/coilheatingwater_baseboard.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil).value()] = new QPixmap(":images/four_pipe_fan_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric).value()] = new QPixmap(":images/zonehvac_low_temperature_radiant_electric.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/lowtempradiant_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/lowtempradiant_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner).value()] = new QPixmap(":images/system_type_1.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump).value()] = new QPixmap(":images/system_type_2.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump).value()] = new QPixmap(":images/watertoairHP.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater).value()] = new QPixmap(":images/heat_coil-uht.png");

  // mini icons
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Building).value()] = new QPixmap(":images/mini_icons/building.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_BuildingStory).value()] = new QPixmap(":images/mini_icons/building_story.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Construction).value()] = new QPixmap(":images/mini_icons/construction.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Construction_CfactorUndergroundWall).value()] = new QPixmap(":images/mini_icons/construction_undergnd.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Construction_FfactorGroundFloor).value()] = new QPixmap(":images/mini_icons/construction_gnd.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Construction_InternalSource).value()] = new QPixmap(":images/mini_icons/construct_inter_source.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Construction_WindowDataFile).value()] = new QPixmap(":images/mini_icons/min_window_data.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Daylighting_Control).value()] = new QPixmap(":images/mini_icons/daylighting_control.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DefaultConstructionSet).value()] = new QPixmap(":images/mini_icons/default_construction_set.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DefaultScheduleSet).value()] = new QPixmap(":images/mini_icons/default_schedule_set.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DefaultSubSurfaceConstructions).value()] = new QPixmap(":images/mini_icons/default_subsurface_constructions.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DefaultSurfaceConstructions).value()] = new QPixmap(":images/mini_icons/default_surface_constructions.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictCooling).value()] = new QPixmap(":images/mini_icons/districtcooling.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictHeating).value()] = new QPixmap(":images/mini_icons/districtheating.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ElectricEquipment).value()] = new QPixmap(":images/mini_icons/electric_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ElectricEquipment_Definition).value()] = new QPixmap(":images/mini_icons/electric_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Facility).value()] = new QPixmap(":images/mini_icons/facility.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GasEquipment).value()] = new QPixmap(":images/mini_icons/gas_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GasEquipment_Definition).value()] = new QPixmap(":images/mini_icons/gas_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical).value()] = new QPixmap(":images/mini_icons/ground_heat_exchanger_vertical.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent).value()] = new QPixmap(":images/mini_icons/heat_transfer_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_IlluminanceMap).value()] = new QPixmap(":images/mini_icons/illuminance_map.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceInfiltration_DesignFlowRate).value()] = new QPixmap(":images/mini_icons/infiltration.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea).value()] = new QPixmap(":images/mini_icons/mini_infiltration_leak.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_InteriorPartitionSurface).value()] = new QPixmap(":images/mini_icons/interior_partition_surface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_InteriorPartitionSurfaceGroup).value()] = new QPixmap(":images/mini_icons/interior_partition_surface_group.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_InternalMass).value()] = new QPixmap(":images/mini_icons/internal_mass.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_InternalMass_Definition).value()] = new QPixmap(":images/mini_icons/internal_mass_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Lights).value()] = new QPixmap(":images/mini_icons/lights.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Lights_Definition).value()] = new QPixmap(":images/mini_icons/lights_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Luminaire).value()] = new QPixmap(":images/mini_icons/luminaire.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Luminaire_Definition).value()] = new QPixmap(":images/mini_icons/luminaire_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Glare_Sensor).value()] = new QPixmap(":images/mini_icons/glare_sensor.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material).value()] = new QPixmap(":images/mini_icons/material.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material_AirGap).value()] = new QPixmap(":images/mini_icons/material_airgap.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material_AirWall).value()] = new QPixmap(":images/mini_icons/material_airwall.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material_InfraredTransparent).value()] = new QPixmap(":images/mini_icons/material_infrared_transparent.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material_NoMass).value()] = new QPixmap(":images/mini_icons/insulation.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Material_RoofVegetation).value()] = new QPixmap(":images/mini_icons/grassanddirt.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_OtherEquipment).value()] = new QPixmap(":images/mini_icons/other_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_OtherEquipment_Definition).value()] = new QPixmap(":images/mini_icons/other_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DesignSpecification_OutdoorAir).value()] = new QPixmap(":images/mini_icons/outdoor_air.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_People).value()] = new QPixmap(":images/mini_icons/people.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_People_Definition).value()] = new QPixmap(":images/mini_icons/people_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Compact).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Constant).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_FixedInterval).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Ruleset).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_VariableInterval).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ShadingSurface).value()] = new QPixmap(":images/mini_icons/shading_surface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ShadingSurfaceGroup).value()] = new QPixmap(":images/mini_icons/shading_surface_group.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Site).value()] = new QPixmap(":images/mini_icons/site.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Space).value()] = new QPixmap(":images/mini_icons/space.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceType).value()] = new QPixmap(":images/mini_icons/space_type.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SteamEquipment).value()] = new QPixmap(":images/mini_icons/steam_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SteamEquipment_Definition).value()] = new QPixmap(":images/mini_icons/steam_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SubSurface).value()] = new QPixmap(":images/mini_icons/subsurface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Surface).value()] = new QPixmap(":images/mini_icons/surface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalZone).value()] = new QPixmap(":images/mini_icons/thermal_zone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Mixed).value()] = new QPixmap(":images/mini_icons/water_heater.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Connections).value()] = new QPixmap(":images/mini_icons/water_connection.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment).value()] = new QPixmap(":images/mini_icons/sinkmini_icon_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment_Definition).value()] = new QPixmap(":images/mini_icons/sink.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Blind).value()] = new QPixmap(":images/mini_icons/mini_window_blinds.png"); 
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Gas).value()] = new QPixmap(":images/mini_icons/window_material_glazing.png"); //new QPixmap(":images/mini_icons/window_material_gas.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_GasMixture).value()] = new QPixmap(":images/mini_icons/window_material_glazing.png"); //new QPixmap(":images/mini_icons/window_material_gasmixture.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Glazing).value()] = new QPixmap(":images/mini_icons/window_material_glazing.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod).value()] = new QPixmap(":images/mini_icons/mini_window_refraction.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic).value()] = new QPixmap(":images/mini_icons/mini_window_thermoc.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Screen).value()] = new QPixmap(":images/mini_icons/mini_window_screen.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Shade).value()] = new QPixmap(":images/mini_icons/mini-window-shade.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_SimpleGlazingSystem).value()] = new QPixmap(":images/mini_icons/min_window_simple_glazing.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).value()] = new QPixmap(":images/mini_icons/OAMixer.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir).value()] = new QPixmap(":images/mini_icons/heat_pump3.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat).value()] = new QPixmap(":images/mini_icons/parallel_fan_terminal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled).value()] = new QPixmap(":images/mini_icons/direct-air.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat).value()] = new QPixmap(":images/mini_icons/vav-reheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_HotWater).value()] = new QPixmap(":images/mini_icons/boiler_steam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_Steam).value()] = new QPixmap(":images/mini_icons/boiler.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Electric_EIR).value()] = new QPixmap(":images/mini_icons/chiller_air.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed).value()] = new QPixmap(":images/mini_icons/mini_dxcoolingcoil_singlespeed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed).value()] = new QPixmap(":images/mini_icons/mini-dxcoolingcoil_2speed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/coilcoolinglowtemprad_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/coilcoolinglowtemprad_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_Water).value()] = new QPixmap(":images/mini_icons/cool_coil.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/mini_icons/wahpDXCC_mini.png");  
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed).value()] = new QPixmap(":images/mini_icons/coil_ht_dx_singlespeed_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Electric).value()] = new QPixmap(":images/mini_icons/electric_furnace_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas).value()] = new QPixmap(":images/mini_icons/furnace.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/coilheatinglowtemprad_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/coilheatinglowtemprad_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water).value()] = new QPixmap(":images/mini_icons/heat_coil.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/mini_icons/wahpDXHC_mini.png");  
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_SingleSpeed).value()] = new QPixmap(":images/mini_icons/cooling_tower.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial).value()] = new QPixmap(":images/mini_icons/directEvap.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ConstantVolume).value()] = new QPixmap(":images/mini_icons/fan_constant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_OnOff).value()] = new QPixmap(":images/mini_icons/fan_on_off_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_VariableVolume).value()] = new QPixmap(":images/mini_icons/fan_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed).value()] = new QPixmap(":images/mini_icons/evap_fluid_cooler.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Adiabatic).value()] = new QPixmap(":images/mini_icons/pipe.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_ConstantSpeed).value()] = new QPixmap(":images/mini_icons/pump_constant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_VariableSpeed).value()] = new QPixmap(":images/mini_icons/pump_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature).value()] = new QPixmap(":images/mini_icons/setpoint_follow_outdoorair.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MixedAir).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_OutdoorAirReset).value()] = new QPixmap(":images/mini_icons/setpoint_outdoorair.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_Scheduled).value()] = new QPixmap(":images/mini_icons/setpoint_scheduled.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Reheat).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric).value()] = new QPixmap(":images/mini_icons/baseboard_electric.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water).value()] = new QPixmap(":images/mini_icons/baseboard_water.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard).value()] = new QPixmap(":images/mini_icons/coilheatingwater_baseboard.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil).value()] = new QPixmap(":images/mini_icons/four_pipe_fan_coil.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric).value()] = new QPixmap(":images/mini_icons/zonehvac_low_temperature_radiant_electric.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/lowtempradiant_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/lowtempradiant_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump).value()] = new QPixmap(":images/mini_icons/watertoairHP.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner).value()] = new QPixmap(":images/mini_icons/system_type_1.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump).value()] = new QPixmap(":images/mini_icons/system_type_2.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater).value()] = new QPixmap(":images/mini_icons/heat_coil-uht.png");}

}

