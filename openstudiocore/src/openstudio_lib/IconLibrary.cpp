/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "IconLibrary.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using std::map;

namespace openstudio {

  IconLibrary* IconLibrary::s_instance = nullptr;

  IconLibrary& IconLibrary::Instance()
  {
    if (!s_instance)
    {
      s_instance = new IconLibrary();
    }

    return *s_instance;
  }

  const QPixmap* IconLibrary::findIcon(unsigned int v) const
  {
    auto i = m_icons.find(v);
    if (i != m_icons.end())
      return (*i).second;

    return nullptr;

  }

  const QPixmap* IconLibrary::findMiniIcon(unsigned int v) const
  {
    auto i = m_miniIcons.find(v);
    if (i != m_miniIcons.end())
      return (*i).second;

    return nullptr;

  }

IconLibrary::IconLibrary()
{
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).value()] = new QPixmap(":images/OAMixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryCoolOnly).value()] = new QPixmap(":images/DXCoolingCoil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir).value()] = new QPixmap(":images/heat_pump3.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).value()] = new QPixmap(":images/heat_pump3.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem).value()] = new QPixmap(":images/unitary_system.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).value()] = new QPixmap(":images/unitary_system.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_ReturnPlenum).value()] = new QPixmap(":images/mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_SupplyPlenum).value()] = new QPixmap(":images/splitter.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_ZoneMixer).value()] = new QPixmap(":images/mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_ZoneSplitter).value()] = new QPixmap(":images/splitter.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV).value()] = new QPixmap(":images/airterminal_dualduct_vav.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume).value()] = new QPixmap(":images/airterminal_dualduct_constantvolume.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir).value()] = new QPixmap(":images/airterminal_dualduct_vav_outdoorair.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam).value()] = new QPixmap(":images/chilled_beam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat).value()] = new QPixmap(":images/cav_reheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction).value()] = new QPixmap(":images/single_ducts_constant_vol_4pipe.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam).value()] = new QPixmap(":images/airterminal_fourpipebeam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat).value()] = new QPixmap(":images/parallel_fan_terminal.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat).value()] = new QPixmap(":images/series_fan_terminal.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled).value()] = new QPixmap(":images/direct-air.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat).value()] = new QPixmap(":images/vav_noreheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat).value()] = new QPixmap(":images/vav-reheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer).value()] = new QPixmap(":images/air_terminal_inlet_mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat).value()] = new QPixmap(":images/vav_noreheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat).value()] = new QPixmap(":images/vav-reheat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_HotWater).value()] = new QPixmap(":images/boiler.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_Steam).value()] = new QPixmap(":images/boiler_steam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CentralHeatPumpSystem).value()] = new QPixmap(":images/centralheatpumpsystem.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Electric_EIR).value()] = new QPixmap(":images/chiller_air.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Absorption_Indirect).value()] = new QPixmap(":images/chiller_absorption_indirect.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Absorption).value()] = new QPixmap(":images/chiller_absorption_direct.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed).value()] = new QPixmap(":images/dxcoolingcoil_singlespeed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed).value()] = new QPixmap(":images/dxcoolingcoil_2speed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed).value()] = new QPixmap(":images/dx_cooling_multispeed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed).value()] = new QPixmap(":images/cool_coil_dx_vari_speed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/coilcoolinglowtemprad_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/coilcoolinglowtemprad_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/wahpDXCC.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit).value()] = new QPixmap(":images/Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_Water).value()] = new QPixmap(":images/cool_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed).value()] = new QPixmap(":images/coil_ht_dx_singlespeed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed).value()] = new QPixmap(":images/ht_coil_dx_vari.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Electric).value()] = new QPixmap(":images/electric_furnace.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas).value()] = new QPixmap(":images/furnace.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage).value()] = new QPixmap(":images/furnace_multi_stage.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water).value()] = new QPixmap(":images/heat_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/coilheatinglowtemprad_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/coilheatinglowtemprad_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/wahpDXHC.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit).value()] = new QPixmap(":images/Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_SingleSpeed).value()] = new QPixmap(":images/cooling_tower.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_TwoSpeed).value()] = new QPixmap(":images/cooling_tower_2speed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_VariableSpeed).value()] = new QPixmap(":images/cooling_tower_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Connector_Mixer).value()] = new QPixmap(":images/mixer.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Connector_Splitter).value()] = new QPixmap(":images/splitter.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictCooling).value()] = new QPixmap(":images/districtcooling.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_DistrictHeating).value()] = new QPixmap(":images/districtheating.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Duct).value()] = new QPixmap(":images/duct.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial).value()] = new QPixmap(":images/directEvap.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial).value()] = new QPixmap(":images/indirectEvap.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ConstantVolume).value()] = new QPixmap(":images/fan_constant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_OnOff).value()] = new QPixmap(":images/fan_on_off.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_VariableVolume).value()] = new QPixmap(":images/fan_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ZoneExhaust).value()] = new QPixmap(":images/fan_zoneexhaust.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_FluidCooler_SingleSpeed).value()] = new QPixmap(":images/fluid_cooler_single.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_FluidCooler_TwoSpeed).value()] = new QPixmap(":images/fluid_cooler_two.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical).value()] = new QPixmap(":images/ground_heat_exchanger_vertical.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench).value()] = new QPixmap(":images/ground_heat_exchanger_horizontal.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeaderedPumps_ConstantSpeed).value()] = new QPixmap(":images/headered_pumps_constant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeaderedPumps_VariableSpeed).value()] = new QPixmap(":images/headered_pumps_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent).value()] = new QPixmap(":images/heat_transfer_outdoorair.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid).value()] = new QPixmap(":images/fluid_hx.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling).value()] = new QPixmap(":images/heatpump_watertowater_equationfit_cooling.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating).value()] = new QPixmap(":images/heatpump_watertowater_equationfit_heating.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Humidifier_Steam_Electric).value()] = new QPixmap(":images/electric_humidifier.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed).value()] = new QPixmap(":images/evap_fluid_cooler.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed).value()] = new QPixmap(":images/evap_fluid_cooler_two_speed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger).value()] = new QPixmap(":images/generator_fuelcell_exhaustgastowaterheatexchanger.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery).value()] = new QPixmap(":images/generator_microturbine_heatrecovery.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_LoadProfile_Plant).value()] = new QPixmap(":images/plant_profile.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Adiabatic).value()] = new QPixmap(":images/pipe.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Indoor).value()] = new QPixmap(":images/pipe_indoor.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Outdoor).value()] = new QPixmap(":images/pipe_outdoor.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_PlantComponent_TemperatureSource).value()] = new QPixmap(":images/plant_temp_source.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_PlantComponent_UserDefined).value()] = new QPixmap(":images/user_defined.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_ConstantSpeed).value()] = new QPixmap(":images/pump_constant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_VariableSpeed).value()] = new QPixmap(":images/pump_variable.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_AirCooled).value()] = new QPixmap(":images/air_cooled.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_Cascade).value()] = new QPixmap(":images/condenser_cascade.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled).value()] = new QPixmap(":images/evap_cooled.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_WaterCooled).value()] = new QPixmap(":images/water_cooled.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal).value()] = new QPixmap(":images/solarcollector_flatplate_photovoltaicthermal.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water).value()] = new QPixmap(":images/solarcollector_flatplate_water.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage).value()] = new QPixmap(":images/solarcollector_integralstorage.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_TemperingValve).value()] = new QPixmap(":images/tempering_valve.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalZone).value()] = new QPixmap(":images/zone.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed).value()] = new QPixmap(":images/thermal_storage_ice.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified).value()] = new QPixmap(":images/thermal_storage_strat.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_HeatPump).value()] = new QPixmap(":images/water_heater.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Mixed).value()] = new QPixmap(":images/water_heater_mixed.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Stratified).value()] = new QPixmap(":images/water_heater_stratified.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Connections).value()] = new QPixmap(":images/water_connection.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment).value()] = new QPixmap(":images/sink.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric).value()] = new QPixmap("images/baseboard_rad_convect_electric.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water).value()] = new QPixmap("images/baseboard_rad_convect_water.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric).value()] = new QPixmap(":images/baseboard_electric.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water).value()] = new QPixmap(":images/baseboard_water.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Dehumidifier_DX).value()] = new QPixmap(":images/dehumidifier_dx.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator).value()] = new QPixmap(":images/energy_recov_vent.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard).value()] = new QPixmap(":images/coilheatingwater_baseboard.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant).value()] = new QPixmap(":images/Coil_Heating_Water_Baseboard_Radiant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_CooledBeam).value()] = new QPixmap(":images/coilcoolingchilledbeam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_FourPipeBeam).value()] = new QPixmap(":images/coilcoolingfourpipebeam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_FourPipeBeam).value()] = new QPixmap(":images/coilheatingfourpipebeam.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode).value()] = new QPixmap(":images/dxcoolingcoil_2stage_humidity.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted).value()] = new QPixmap(":images/coil_system_coolingwater_heat_exchanger.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted).value()] = new QPixmap(":images/coilsystem_cooling_dx.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil).value()] = new QPixmap(":images/four_pipe_fan_coil.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant).value()] = new QPixmap(":images/hightempradiant.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/lowtempradiant_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/lowtempradiant_varflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/lowtempradiant_constflow.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner).value()] = new QPixmap(":images/system_type_1.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump).value()] = new QPixmap(":images/system_type_2.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).value()] = new QPixmap(":images/vrf_unit.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump).value()] = new QPixmap(":images/watertoairHP.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater).value()] = new QPixmap(":images/heat_coil-uht.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitVentilator).value()] = new QPixmap(":images/unit_ventilator.png");


  // TODO: Update if create new icons... placeholder for now
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_Scheduled).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOn).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOff).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOn).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOff).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOn).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOff).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_DifferentialThermostat).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_OptimumStart).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_NightCycle).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_NightVentilation).value()] = new QPixmap(":images/hvac-icon.png");
  m_icons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HybridVentilation).value()] = new QPixmap(":images/hvac-icon.png");

  // mini icons
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirConditioner_VariableRefrigerantFlow).value()] = new QPixmap(":images/mini_icons/mini_vrf_outdoor.png");
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
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Duct).value()] = new QPixmap(":images/mini_icons/mini_duct.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ElectricEquipment).value()] = new QPixmap(":images/mini_icons/electric_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ElectricEquipment_Definition).value()] = new QPixmap(":images/mini_icons/electric_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Facility).value()] = new QPixmap(":images/mini_icons/facility.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_FluidCooler_SingleSpeed).value()] = new QPixmap(":images/mini_icons/mini_fluid_cooler_single.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_FluidCooler_TwoSpeed).value()] = new QPixmap(":images/mini_icons/mini_fluid_cooler_two.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GasEquipment).value()] = new QPixmap(":images/mini_icons/gas_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GasEquipment_Definition).value()] = new QPixmap(":images/mini_icons/gas_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger).value()] = new QPixmap(":images/mini_icons/generator_fuelcell_exhaustgastowaterheatexchanger.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery).value()] = new QPixmap(":images/mini_icons/generator_microturbine_heatrecovery.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench).value()] = new QPixmap(":images/mini_icons/mini_ground_heat_exchanger_horizontal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical).value()] = new QPixmap(":images/mini_icons/ground_heat_exchanger_vertical.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeaderedPumps_ConstantSpeed).value()] = new QPixmap(":images/mini_icons/mini_headered_pumps_constant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeaderedPumps_VariableSpeed).value()] = new QPixmap(":images/mini_icons/mini_headered_pumps_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent).value()] = new QPixmap(":images/mini_icons/heat_transfer_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid).value()] = new QPixmap(":images/mini_icons/fluid_hx_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Humidifier_Steam_Electric).value()] = new QPixmap(":images/mini_icons/mini_electric_humidifier.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_IlluminanceMap).value()] = new QPixmap(":images/mini_icons/illuminance_map.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceInfiltration_DesignFlowRate).value()] = new QPixmap(":images/mini_icons/infiltration.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea).value()] = new QPixmap(":images/mini_icons/mini_infiltration_leak.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_TemperingValve).value()] = new QPixmap(":images/mini_icons/mini_tempering_valve.png");
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
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Indoor).value()] = new QPixmap(":images/mini_icons/mini_pipe_indoor.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Outdoor).value()] = new QPixmap(":images/mini_icons/mini_pipe_outdoor.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_PlantComponent_TemperatureSource).value()] = new QPixmap(":images/mini_icons/mini_plant_temp_source.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_PlantComponent_UserDefined).value()] = new QPixmap(":images/mini_icons/user_defined.png");  
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Compressor).value()] = new QPixmap(":images/mini_icons/mini_compressor.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_AirCooled).value()] = new QPixmap(":images/mini_icons/mini_condensor.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Case).value()] = new QPixmap(":images/mini_icons/mini_display_case.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Subcooler_Mechanical).value()] = new QPixmap(":images/mini_icons/mini_mechanical-sub-cooler.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction).value()] = new QPixmap(":images/mini_icons/mini_slhx.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_System).value()] = new QPixmap(":images/mini_icons/mini_refrigeration_system.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_WalkIn).value()] = new QPixmap(":images/mini_icons/mini_walkin_case.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_AirCooled).value()] = new QPixmap(":images/mini_icons/mini_air_cooled.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_Cascade).value()] = new QPixmap(":images/mini_icons/mini_condenser_cascade.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled).value()] = new QPixmap(":images/mini_icons/mini_evap_cooled.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Refrigeration_Condenser_WaterCooled).value()] = new QPixmap(":images/mini_icons/mini_water_cooled.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Compact).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Constant).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_FixedInterval).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_Ruleset).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Schedule_VariableInterval).value()] = new QPixmap(":images/mini_icons/schedule.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ShadingSurface).value()] = new QPixmap(":images/mini_icons/shading_surface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ShadingSurfaceGroup).value()] = new QPixmap(":images/mini_icons/shading_surface_group.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Site).value()] = new QPixmap(":images/mini_icons/site.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal).value()] = new QPixmap(":images/mini_icons/solarcollector_flatplate_photovoltaicthermal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water).value()] = new QPixmap(":images/mini_icons/solarcollector_flatplate_water.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage).value()] = new QPixmap(":images/mini_icons/solarcollector_integralstorage.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Space).value()] = new QPixmap(":images/mini_icons/space.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SpaceType).value()] = new QPixmap(":images/mini_icons/space_type.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SteamEquipment).value()] = new QPixmap(":images/mini_icons/steam_equipment.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SteamEquipment_Definition).value()] = new QPixmap(":images/mini_icons/steam_equipment_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SubSurface).value()] = new QPixmap(":images/mini_icons/subsurface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Surface).value()] = new QPixmap(":images/mini_icons/surface.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed).value()] = new QPixmap(":images/mini_icons/mini_thermal_storage_ice.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified).value()] = new QPixmap(":images/mini_icons/mini_thermal_storage_strat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ThermalZone).value()] = new QPixmap(":images/mini_icons/thermal_zone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_HeatPump).value()] = new QPixmap(":images/mini_icons/water_heater.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Mixed).value()] = new QPixmap(":images/mini_icons/water_heater_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterHeater_Stratified).value()] = new QPixmap(":images/mini_icons/water_heater_stratified.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Connections).value()] = new QPixmap(":images/mini_icons/water_connection.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment).value()] = new QPixmap(":images/mini_icons/sinkmini_icon_definition.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WaterUse_Equipment_Definition).value()] = new QPixmap(":images/mini_icons/sink.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_Blind).value()] = new QPixmap(":images/mini_icons/mini_window_blinds.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice).value()] = new QPixmap(":images/mini_icons/mini_window_daylightredirectiondevice.png");
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
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).value()] = new QPixmap(":images/mini_icons/heat_pump3.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem).value()] = new QPixmap(":images/mini_icons/mini_unitary_system.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass).value()] = new QPixmap(":images/mini_icons/mini_unitary_system.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV).value()] = new QPixmap(":images/mini_icons/airterminal_dualduct_vav.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume).value()] = new QPixmap(":images/mini_icons/airterminal_dualduct_constantvolume.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir).value()] = new QPixmap(":images/mini_icons/airterminal_dualduct_vav_outdoorair.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam).value()] = new QPixmap(":images/mini_icons/chilled_beam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction).value()] = new QPixmap(":images/mini_icons/four_pipe_induction_terminal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam).value()] = new QPixmap(":images/mini_icons/airterminal_fourpipebeam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat).value()] = new QPixmap(":images/mini_icons/cav_reheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat).value()] = new QPixmap(":images/mini_icons/parallel_fan_terminal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat).value()] = new QPixmap(":images/mini_icons/series_fan_terminal.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled).value()] = new QPixmap(":images/mini_icons/direct-air.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat).value()] = new QPixmap(":images/mini_icons/vav_noreheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat).value()] = new QPixmap(":images/mini_icons/vav-reheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat).value()] = new QPixmap(":images/mini_icons/vav_noreheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat).value()] = new QPixmap(":images/mini_icons/vav-reheat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer).value()] = new QPixmap(":images/mini_icons/mini_air_terminal_inlet_mixer.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_HotWater).value()] = new QPixmap(":images/mini_icons/boiler_steam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Boiler_Steam).value()] = new QPixmap(":images/mini_icons/boiler.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CentralHeatPumpSystem).value()] = new QPixmap(":images/mini_icons/centralheatpumpsystem.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Electric_EIR).value()] = new QPixmap(":images/mini_icons/chiller_air.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Absorption_Indirect).value()] = new QPixmap(":images/mini_icons/mini_chiller_absorption_indirect.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Chiller_Absorption).value()] = new QPixmap(":images/mini_icons/mini_chiller_absorption_direct.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_CooledBeam).value()] = new QPixmap(":images/mini_icons/coilcoolingchilledbeam_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_FourPipeBeam).value()] = new QPixmap(":images/mini_icons/coilcoolingfourpipebeam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_FourPipeBeam).value()] = new QPixmap(":images/mini_icons/coilheatingfourpipebeam.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed).value()] = new QPixmap(":images/mini_icons/mini_dxcoolingcoil_singlespeed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed).value()] = new QPixmap(":images/mini_icons/mini-dxcoolingcoil_2speed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode).value()] = new QPixmap(":images/mini_icons/mini_dxcoolingcoil_2stage_humidity.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed).value()] = new QPixmap(":images/mini_icons/mini_dx_cooling_coil_multispeed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed).value()] = new QPixmap(":images/mini_icons/mini_cool_coil_dx_vari_speed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/coilcoolinglowtemprad_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/coilcoolinglowtemprad_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_Water).value()] = new QPixmap(":images/mini_icons/cool_coil.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/mini_icons/wahpDXCC_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit).value()] = new QPixmap(":images/mini_icons/Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed).value()] = new QPixmap(":images/mini_icons/coil_ht_dx_singlespeed_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed).value()] = new QPixmap(":images/mini_icons/mini_ht_coil_dx_vari.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Electric).value()] = new QPixmap(":images/mini_icons/electric_furnace_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas).value()] = new QPixmap(":images/mini_icons/furnace.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage).value()] = new QPixmap(":images/mini_icons/mini_furnace_multi_stage.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/coilheatinglowtemprad_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/coilheatinglowtemprad_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water).value()] = new QPixmap(":images/mini_icons/heat_coil.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit).value()] = new QPixmap(":images/mini_icons/wahpDXHC_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit).value()] = new QPixmap(":images/mini_icons/Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted).value()] = new QPixmap(":images/mini_icons/mini_coil_system_water.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted).value()] = new QPixmap(":images/mini_icons/mini_coilsystem_cooling_DX.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_SingleSpeed).value()] = new QPixmap(":images/mini_icons/mini_cooling_tower.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_TwoSpeed).value()] = new QPixmap(":images/mini_icons/mini_cooling_tower_2speed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_CoolingTower_VariableSpeed).value()] = new QPixmap(":images/mini_icons/mini_cooling_tower_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial).value()] = new QPixmap(":images/mini_icons/directEvap.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial).value()] = new QPixmap(":images/mini_icons/indirectEvap.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ConstantVolume).value()] = new QPixmap(":images/mini_icons/fan_constant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_OnOff).value()] = new QPixmap(":images/mini_icons/fan_on_off_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_VariableVolume).value()] = new QPixmap(":images/mini_icons/fan_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Fan_ZoneExhaust).value()] = new QPixmap(":images/mini_icons/fan_zoneexhaust.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed).value()] = new QPixmap(":images/mini_icons/evap_fluid_cooler.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed).value()] = new QPixmap(":images/mini_icons/evap_fluid_cooler_two_speed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling).value()] = new QPixmap(":images/mini_icons/heatpump_watertowater_equationfit_cooling.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating).value()] = new QPixmap(":images/mini_icons/heatpump_watertowater_equationfit_heating.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_LoadProfile_Plant).value()] = new QPixmap(":images/mini_icons/mini_plant_profile.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pipe_Adiabatic).value()] = new QPixmap(":images/mini_icons/pipe.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_ConstantSpeed).value()] = new QPixmap(":images/mini_icons/pump_constant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Pump_VariableSpeed).value()] = new QPixmap(":images/mini_icons/pump_variable.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_Coldest).value()] = new QPixmap(":images/mini_icons/setpoint_coldest.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_FollowGroundTemperature).value()] = new QPixmap(":images/mini_icons/setpoint_follow_ground_temp.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature).value()] = new QPixmap(":images/mini_icons/setpoint_follow_outdoorair.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_FollowSystemNodeTemperature).value()] = new QPixmap(":images/mini_icons/setpoint_follow_system_node.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MixedAir).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_Cooling_Average).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_cooling.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_Heating_Average).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_heating.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_humidity_max.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_humidity_min.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_MaximumHumidity_Average).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_maxhumidity_avg.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average).value()] = new QPixmap(":images/mini_icons/setpoint_multizone_minhumidity_avg.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_OutdoorAirPretreat).value()] = new QPixmap(":images/mini_icons/setpoint_pretreat.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_OutdoorAirReset).value()] = new QPixmap(":images/mini_icons/setpoint_outdoorair.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_Scheduled).value()] = new QPixmap(":images/mini_icons/setpoint_scheduled.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_Scheduled_DualSetpoint).value()] = new QPixmap(":images/mini_icons/setpoint_dual.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone_humidity_max.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone_humidity_min.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling).value()] = new QPixmap(":images/mini_icons/setpoint_onestage_cooling.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageHeating).value()] = new QPixmap(":images/mini_icons/setpoint_onestage_heating.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Reheat).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Cooling).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_SingleZone_Heating).value()] = new QPixmap(":images/mini_icons/setpoint_singlezone.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_Warmest).value()] = new QPixmap(":images/mini_icons/setpoint_warmest.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_SetpointManager_WarmestTemperatureFlow).value()] = new QPixmap(":images/mini_icons/setpoint_warmest_tempflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).value()] = new QPixmap(":images/mini_icons/mini_vrf_unit.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric).value()] = new QPixmap(":images/mini_icons/mini_baseboard_rad_convect_electric.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water).value()] = new QPixmap(":images/mini_icons/mini_baseboard_rad_convect_water.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric).value()] = new QPixmap(":images/mini_icons/baseboard_electric.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water).value()] = new QPixmap(":images/mini_icons/baseboard_water.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_Dehumidifier_DX).value()] = new QPixmap(":images/mini_icons/mini_dehumidifier_dx.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator).value()] = new QPixmap(":images/mini_icons/mini_energy_recov_vent.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard).value()] = new QPixmap(":images/mini_icons/coilheatingwater_baseboard_mini.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant).value()] = new QPixmap(":images/mini_icons/Coil_Heating_Water_Baseboard_Radiant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil).value()] = new QPixmap(":images/mini_single_ducts_constant_vol_4pipe.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant).value()] = new QPixmap(":images/mini_icons/hightempradiant.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric).value()] = new QPixmap(":images/mini_icons/zonehvac_low_temperature_radiant_electric.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow).value()] = new QPixmap(":images/mini_icons/lowtempradiant_varflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow).value()] = new QPixmap(":images/mini_icons/lowtempradiant_constflow.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump).value()] = new QPixmap(":images/mini_icons/watertoairHP.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner).value()] = new QPixmap(":images/mini_icons/system_type_1.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump).value()] = new QPixmap(":images/mini_icons/system_type_2.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater).value()] = new QPixmap(":images/mini_icons/heat_coil-uht.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_ZoneHVAC_UnitVentilator).value()] = new QPixmap(":images/mini_icons/mini_unit_ventilator.png");

  // TODO: Update if create new icons... placeholder for now
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_Scheduled).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOn).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOff).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOn).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOff).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOn).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOff).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_DifferentialThermostat).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_OptimumStart).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_NightCycle).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_NightVentilation).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");
  m_miniIcons[openstudio::IddObjectType(openstudio::IddObjectType::OS_AvailabilityManager_HybridVentilation).value()] = new QPixmap(":images/mini_icons/setpoint_mixed.png");

}

}
