#include <benchmark/benchmark.h>

#include "../IddObject.hpp"
#include "../../core/ASCIIStrings.hpp"

#include <sstream>
#include <string>

using namespace openstudio;

constexpr auto bigObject = R"(ZoneHVAC:TerminalUnit:VariableRefrigerantFlow,
        \memo A terminal unit with variable refrigerant flow (VRF) DX cooling and heating coils
        \memo (air-to-air heat pump). The VRF terminal units are served by an
        \memo AirConditioner:VariableRefrigerantFlow or
        \memo AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:* system.
        \memo Terminal units can be configured as zone, air loop or outside air system equipment.
        \min-fields 19
  A1 ,  \field Zone Terminal Unit Name
        \required-field
        \type alpha
        \reference ZoneTerminalUnitNames
        \reference DOAToZonalUnit
        \reference ZoneEquipmentNames
        \reference-class-name validBranchEquipmentTypes
        \reference validBranchEquipmentNames
        \reference-class-name validOASysEquipmentTypes
        \reference validOASysEquipmentNames
  A2 ,  \field Terminal Unit Availability Schedule
        \type object-list
        \object-list ScheduleNames
        \note The unit is available the entire simulation if this field is left blank
        \note Schedule values of 0 denote the unit is off.
  A3 ,  \field Terminal Unit Air Inlet Node Name
        \required-field
        \type node
        \note the inlet node to the terminal unit
  A4 ,  \field Terminal Unit Air Outlet Node Name
        \required-field
        \type node
        \note the outlet node of the terminal unit
  N1 ,  \field Cooling Supply Air Flow Rate
        \type real
        \units m3/s
        \minimum> 0.0
        \autosizable
  N2 ,  \field No Cooling Supply Air Flow Rate
        \type real
        \units m3/s
        \minimum 0.0
        \autosizable
  N3 ,  \field Heating Supply Air Flow Rate
        \type real
        \units m3/s
        \minimum> 0.0
        \autosizable
  N4 ,  \field No Heating Supply Air Flow Rate
        \type real
        \units m3/s
        \minimum 0.0
        \autosizable
  N5 ,  \field Cooling Outdoor Air Flow Rate
        \type real
        \units m3/s
        \minimum 0.0
        \autosizable
        \note This field is used only when an oudoor air mixer is included.
        \note This field is set to zero flow when the VRF terminal unit is connected to
        \note central dedicated outdoor air through air terminal single duct mixer object.
        \note When this VRF terminal is used as air loop equipment the autosized flow
        \note rate will be set to 0 when an outdoor air system is connected to this air loop,
        \note otherwise the outdoor air flow rate will equal the maximum outdoor air flow rate.
  N6 ,  \field Heating Outdoor Air Flow Rate
        \type real
        \units m3/s
        \minimum 0.0
        \autosizable
        \note This field is used only when an oudoor air mixer is included.
        \note This field is set to zero flow when the VRF terminal unit is connected to
        \note central dedicated outdoor air through air terminal single duct mixer object.
        \note When this VRF terminal is used as air loop equipment the autosized flow
        \note rate will be set to 0 when an outdoor air system is connected to this air loop,
        \note otherwise the outdoor air flow rate will equal the maximum outdoor air flow rate.
  N7 ,  \field No Load Outdoor Air Flow Rate
        \type real
        \units m3/s
        \minimum 0.0
        \autosizable
        \note This field is used only when an oudoor air mixer is included.
        \note This field is set to zero flow when the VRF terminal unit is connected to
        \note central dedicated outdoor air through air terminal single duct mixer object.
        \note When this VRF terminal is used as air loop equipment the autosized flow
        \note rate will be set to 0 when an outdoor air system is connected to this air loop,
        \note otherwise the outdoor air flow rate will equal the maximum outdoor air flow rate.
  A5 ,  \field Supply Air Fan Operating Mode Schedule Name
        \type object-list
        \object-list ScheduleNames
        \note Required for zone equipment. Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
        \note Also leave blank if terminal unit is used on main AirloopHVAC branch and terminal unit has no fan.
  A6 ,  \field Supply Air Fan Placement
        \type choice
        \key BlowThrough
        \key DrawThrough
        \default BlowThrough
        \note Select fan placement as either blow through or draw through.
        \note Required for zone equipment. This field is ignored if the VRF terminal unit is used
        \note in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
        \note This field is also ignored if VRF terminal unit is used on main AirloopHVAC branch
        \note and terminal unit has no fan.
  A7 ,  \field Supply Air Fan Object Type
        \type choice
        \key Fan:SystemModel
        \key Fan:OnOff
        \key Fan:ConstantVolume
        \key Fan:VariableVolume
        \default Fan:ConstantVolume
        \note Supply Air Fan Object Type must be Fan:SystemModel, Fan:OnOff, or Fan:ConstantVolume
        \note if AirConditioner:VariableRefrigerantFlow is used to model VRF outdoor unit
        \note Supply Air Fan Object Type must be Fan:SystemModel or Fan:VariableVolume if
        \note AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl or
        \note AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:HR
        \note is used to model VRF outdoor unit
        \note Required for zone equipment. Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
        \note Also leave blank if terminal unit is used on main AirloopHVAC branch and terminal unit has no fan.
  A8 ,  \field Supply Air Fan Object Name
        \type object-list
        \object-list FansCVandOnOffandVAV
        \object-list FansSystemModel
  A9 ,  \field Outside Air Mixer Object Type
        \type choice
        \key OutdoorAir:Mixer
        \note Currently only one type OutdoorAir:Mixer object is available.
        \note If this field is blank, and outside air mixer is not used.
        \note This field should be left blank if the VRF terminal unit is connected to
        \note central dedicated outdoor air through an AirTerminal:SingleDuct:Mixer object.
        \note This field may also be left blank when the VRF terminal is used in the air
        \note loop or outdoor air system.
  A10,  \field Outside Air Mixer Object Name
        \type object-list
        \object-list OutdoorAirMixers
        \note If this field is blank, the OutdoorAir:Mixer is not used.
        \note This optional field specifies the name of the OutdoorAir:Mixer object.
        \note When used, this name needs to match name of the OutdoorAir:Mixer object.
        \note This field should be left blank if the VRF terminal unit is connected to
        \note central dedicated outdoor air through an AirTerminal:SingleDuct:Mixer object.
        \note This field may also be left blank when the VRF terminal is used in the air
        \note loop or outdoor air system.
  A11,  \field Cooling Coil Object Type
        \type choice
        \key Coil:Cooling:DX:VariableRefrigerantFlow
        \key Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl
        \note Cooling Coil Type must be Coil:Cooling:DX:VariableRefrigerantFlow
        \note if AirConditioner:VariableRefrigerantFlow is used
        \note to model VRF outdoor unit
        \note Cooling Coil Type must be
        \note Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl
        \note if AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl or
        \note if AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:HR
        \note is used to model VRF outdoor unit
        \note This field may be left blank if heating-only mode is used
  A12,  \field Cooling Coil Object Name
        \type object-list
        \object-list CoolingCoilsDXVarRefrigFlow
        \object-list CoolingCoilsDXVarRefrigFlowFluidTemperatureControl
        \note Cooling Coil Type must be Coil:Cooling:DX:VariableRefrigerantFlow
        \note This field may be left blank if heating-only mode is used
  A13,  \field Heating Coil Object Type
        \type choice
        \key Coil:Heating:DX:VariableRefrigerantFlow
        \key Coil:Heating:DX:VariableRefrigerantFlow:FluidTemperatureControl
        \note Heating Coil Type must be Coil:Heating:DX:VariableRefrigerantFlow
        \note if AirConditioner:VariableRefrigerantFlow is used
        \note to model VRF outdoor unit
        \note Heating Coil Type must be
        \note Coil:Heating:DX:VariableRefrigerantFlow:FluidTemperatureControl
        \note if AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl or
        \note if AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:HR
        \note is used to model VRF outdoor unit
        \note This field may be left blank if cooling-only mode is used
  A14,  \field Heating Coil Object Name
        \type object-list
        \object-list HeatingCoilsDXVarRefrigFlow
        \object-list HeatingCoilsDXVarRefrigFlowFluidTemperatureControl
        \note Heating Coil Type must be Coil:Heating:DX:VariableRefrigerantFlow
        \note This field may be left blank if cooling-only mode is used
  N8 ,  \field Zone Terminal Unit On Parasitic Electric Energy Use
        \type real
        \units W
        \minimum 0
        \default 0
  N9 ,  \field Zone Terminal Unit Off Parasitic Electric Energy Use
        \type real
        \units W
        \minimum 0
        \default 0
  N10, \field Rated Heating Capacity Sizing Ratio
       \type real
       \units W/W
       \minimum 1.0
       \default 1.0
       \note If this terminal unit's heating coil is autosized, the heating capacity is sized
       \note to be equal to the cooling capacity multiplied by this sizing ratio.
       \note This input applies to the terminal unit heating coil and overrides the sizing
       \note ratio entered in the AirConditioner:VariableRefrigerantFlow object.
  A15, \field Availability Manager List Name
       \note Enter the name of an AvailabilityManagerAssignmentList object.
       \type object-list
       \object-list SystemAvailabilityManagerLists
  A16, \field Design Specification ZoneHVAC Sizing Object Name
       \note Enter the name of a DesignSpecificationZoneHVACSizing object.
       \type object-list
       \object-list DesignSpecificationZoneHVACSizingName
  A17, \field Supplemental Heating Coil Object Type
       \type choice
       \key Coil:Heating:Fuel
       \key Coil:Heating:Electric
       \key Coil:Heating:Water
       \key Coil:Heating:Steam
       \note works with gas, electric, hot water and steam heating coil.
  A18, \field Supplemental Heating Coil Name
       \type object-list
       \object-list HeatingCoilName
       \note Needs to match in the supplemental heating coil object.
  N11, \field Maximum Supply Air Temperature from Supplemental Heater
       \note required-field disabled as it has a default
       \type real
       \units C
       \autosizable
       \default autosize
       \note Supply air temperature from the supplemental heater will not exceed this value.
  N12, \field Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation
       \type real
       \maximum 21.0
       \default 21.0
       \units C
       \note Supplemental heater will not operate when outdoor temperature exceeds this value.
  A19; \field Controlling Zone or Thermostat Location
       \type object-list
       \object-list ZoneNames
       \note Used only for AirloopHVAC equipment on a main branch and defines zone name where thermostat is located.
       \note Not required for zone equipment. Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
       \note Required when terminal unit is used on main AirloopHVAC branch and coils are not set point controlled.
       \note When terminal unit is used in air loop and is load controlled, this zone's thermostat will control operation.
  )";

constexpr auto smallObject = R"(ZoneList,
       \memo Defines a list of thermal zones which can be referenced as a group. The ZoneList name
       \memo may be used elsewhere in the input to apply a parameter to all zones in the list.
       \memo ZoneLists can be used effectively with the following objects: People, Lights,
       \memo ElectricEquipment, GasEquipment, HotWaterEquipment, ZoneInfiltration:DesignFlowRate,
       \memo ZoneVentilation:DesignFlowRate, Sizing:Zone, ZoneControl:Thermostat, and others.
       \min-fields 2
       \extensible:1 - repeat last field, remembering to remove ; from "inner" fields.
  A1 , \field Name
       \note Name of the Zone List
       \required-field
       \type alpha
       \reference ZoneListNames
       \reference ZoneAndZoneListNames
  A2 ; \field Zone 1 Name
       \begin-extensible
       \required-field
       \type object-list
       \object-list ZoneNames)";

static void BM_ParseIddObject(benchmark::State& state, const std::string& object) {

  std::stringstream ss(object);

  std::string groupName = "Zone HVAC Forced Air Units";

  std::string line;
  std::string objectName;
  getline(ss, line);
  if (auto pos = line.find_first_of(",;"); pos != std::string::npos) {
    objectName = line.substr(0, pos);  //
    openstudio::ascii_trim(objectName);
  }
  std::string text(line);
  while (getline(ss, line)) {
    openstudio::ascii_trim(line);
    text += line;
  }

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto idfObject = IddObject::load(objectName, groupName, text);
  }
};

BENCHMARK_CAPTURE(BM_ParseIddObject, SmallObject, std::string(smallObject));
BENCHMARK_CAPTURE(BM_ParseIddObject, BigObject, std::string(bigObject));
