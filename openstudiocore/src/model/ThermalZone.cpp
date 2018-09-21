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

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVACSupplyPlenum.hpp"
#include "AirLoopHVACSupplyPlenum_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "ZoneHVACEquipmentList.hpp"
#include "ZoneHVACEquipmentList_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "SizingZone.hpp"
#include "SizingZone_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceLoad.hpp"
#include "SpaceLoad_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "DaylightingControl.hpp"
#include "DaylightingControl_Impl.hpp"
#include "IlluminanceMap.hpp"
#include "IlluminanceMap_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Thermostat.hpp"
#include "Thermostat_Impl.hpp"
#include "ThermostatSetpointDualSetpoint.hpp"
#include "ThermostatSetpointDualSetpoint_Impl.hpp"
#include "ZoneControlContaminantController.hpp"
#include "ZoneControlContaminantController_Impl.hpp"
#include "ZoneControlHumidistat.hpp"
#include "ZoneControlHumidistat_Impl.hpp"
#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "LifeCycleCost_Impl.hpp"
#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "SetpointManagerSingleZoneCooling.hpp"
#include "SetpointManagerSingleZoneCooling_Impl.hpp"
#include "SetpointManagerSingleZoneHeating.hpp"
#include "SetpointManagerSingleZoneHeating_Impl.hpp"
#include "ZoneMixing.hpp"
#include "ZoneMixing_Impl.hpp"
#include "AirflowNetworkZone.hpp"
#include "AirflowNetworkZone_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

#include "../utilities/units/Unit.hpp"
#include "../utilities/units/QuantityConverter.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

#include "../utilities/sql/SqlFile.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalZone_Impl::ThermalZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalZone::iddObjectType());
  }


  ThermalZone_Impl::ThermalZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalZone::iddObjectType());
  }

  ThermalZone_Impl::ThermalZone_Impl(const ThermalZone_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> ThermalZone_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->model().building());
  }

  std::vector<ModelObject> ThermalZone_Impl::children() const
  {
    std::vector<ModelObject> result;

    // Sizing Zone object
    SizingZone sizingZone = this->sizingZone();
    result.push_back(sizingZone);

    // DLM: temporarily add supplyZoneMixing as children so we can see them in IG
    // remove once we have gridview for these
    for (const auto& mixing : supplyZoneMixing()){
      result.push_back(mixing);
    }

    boost::optional<AirflowNetworkZone> afnz = airflowNetworkZone();
    if (afnz) {
      result.push_back(afnz.get());
    }

    return result;
  }

  bool ThermalZone_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Building>()){
      result = (this->model() == newParent.model());
    }
    return result;
  }

  std::vector<IddObjectType> ThermalZone_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    // DLM: this does not seem to agree with implementation of children()
    result.push_back(IddObjectType::OS_ThermostatSetpoint_DualSetpoint);
    result.push_back(IddObjectType::OS_ZoneControl_Thermostat_StagedDualSetpoint);
    return result;
  }

  const std::vector<std::string>& ThermalZone_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{

      /*
       * Zone Thermal Output
       */

      // Outdoor
      "Zone Outdoor Air Drybulb Temperature",
      "Zone Outdoor Air Wetbulb Temperature",
      "Zone Outdoor Air Wind Speed",

      // Internal
      "Zone Total Internal Radiant Heating Energy",
      "Zone Total Internal Radiant Heating Rate",
      "Zone Total Internal Visible Radiation Heating Energy",
      "Zone Total Internal Visible Radiation Heating Rate",
      "Zone Total Internal Convective Heating Energy",
      "Zone Total Internal Convective Heating Rate",
      "Zone Total Internal Latent Gain Energy",
      "Zone Total Internal Latent Gain Rate",
      "Zone Total Internal Total Heating Energy",
      "Zone Total Internal Total Heating Rate",

      // Temperatures and RH
      "Zone Mean Air Temperature", // Zone level
      "Zone Air Temperature", // HVAC level
      "Zone Mean Air Dewpoint Temperature",
      "Zone Mean Radiant Temperature",
      "Zone Operative Temperature",
      "Zone Air Humidity Ratio",
      "Zone Air Relative Humidity",

      // Heat Balance
      "Zone Air Heat Balance Internal Convective Heat Gain Rate",
      "Zone Air Heat Balance Surface Convection Rate",
      "Zone Air Heat Balance Interzone Air Transfer Rate",
      "Zone Air Heat Balance Outdoor Air Transfer Rate",
      "Zone Air Heat Balance System Air Transfer Rate",
      "Zone Air Heat Balance System Convective Heat Gain Rate",
      "Zone Air Heat Balance Air Energy Storage Rate",
      "Zone Air Heat Balance Deviation Rate",

      // Air System
      "Zone Air System Sensible Heating Energy",
      "Zone Air System Sensible Cooling Energy",
      "Zone Air System Sensible Heating Rate",
      "Zone Air System Sensible Cooling Rate",

      /*
       * Zone Outputs from LOADS: Equipment, People, Lights
       */

      // TODO:: All of this section should be dynamic (if no ElectricEquipment, don't propose the output...)

      // Electric Equipment
      "Zone Electric Equipment Electric Power",
      "Zone Electric Equipment Electric Energy",
      "Zone Electric Equipment Radiant Heating Energy",
      "Zone Electric Equipment Radiant Heating Rate",
      "Zone Electric Equipment Convective Heating Energy",
      "Zone Electric Equipment Convective Heating Rate",
      "Zone Electric Equipment Latent Gain Energy",
      "Zone Electric Equipment Latent Gain Rate",
      "Zone Electric Equipment Lost Heat Energy",
      "Zone Electric Equipment Lost Heat Rate",
      "Zone Electric Equipment Total Heating Energy",
      "Zone Electric Equipment Total Heating Rate",

      // Gas Equipment
      "Zone Gas Equipment Gas Rate",
      "Zone Gas Equipment Gas Energy",
      "Zone Gas Equipment Radiant Heating Energy",
      "Zone Gas Equipment Radiant Heating Rate",
      "Zone Gas Equipment Convective Heating Energy",
      "Zone Gas Equipment Convective Heating Rate",
      "Zone Gas Equipment Latent Gain Energy",
      "Zone Gas Equipment Latent Gain Rate",
      "Zone Gas Equipment Lost Heat Energy",
      "Zone Gas Equipment Lost Heat Rate",
      "Zone Gas Equipment Total Heating Energy",
      "Zone Gas Equipment Total Heating Rate",

      // HotWater Equipment
      "Zone Hot Water Equipment District Heating Rate",
      "Zone Hot Water Equipment District Heating Energy",
      "Zone Hot Water Equipment Radiant Heating Energy",
      "Zone Hot Water Equipment Radiant Heating Rate",
      "Zone Hot Water Equipment Convective Heating Energy",
      "Zone Hot Water Equipment Convective Heating Rate",
      "Zone Hot Water Equipment Latent Gain Energy",
      "Zone Hot Water Equipment Latent Gain Rate",
      "Zone Hot Water Equipment Lost Heat Energy",
      "Zone Hot Water Equipment Lost Heat Rate",
      "Zone Hot Water Equipment Total Heating Energy",
      "Zone Hot Water Equipment Total Heating Rate",

      // Lights
      // TODO: if zone.spaces.select{|s| s.lights.size > 0}.size > 0
      "Zone Lights Electric Power",
      "Zone Lights Radiant Heating Energy",
      "Zone Lights Radiant Heating Rate",
      "Zone Lights Visible Radiation Heating Energy",
      "Zone Lights Visible Radiation Heating Rate",
      "Zone Lights Convective Heating Energy",
      "Zone Lights Convective Heating Rate",
      "Zone Lights Return Air Heating Energy",
      "Zone Lights Return Air Heating Rate",
      "Zone Lights Total Heating Energy",
      "Zone Lights Total Heating Rate",
      "Zone Lights Electric Energy",

      // OtherEquipment
      "Zone Other Equipment Radiant Heating Energy",
      "Zone Other Equipment Radiant Heating Rate",
      "Zone Other Equipment Convective Heating Energy",
      "Zone Other Equipment Convective Heating Rate",
      "Zone Other Equipment Latent Gain Energy",
      "Zone Other Equipment Latent Gain Rate",
      "Zone Other Equipment Lost Heat Energy",
      "Zone Other Equipment Lost Heat Rate",
      "Zone Other Equipment Total Heating Energy",
      "Zone Other Equipment Total Heating Rate",

      // People
      "Zone People Occupant Count",
      "Zone People Radiant Heating Energy",
      "Zone People Radiant Heating Rate",
      "Zone People Convective Heating Energy",
      "Zone People Convective Heating Rate",
      "Zone People Sensible Heating Energy",
      "Zone People Sensible Heating Rate",
      "Zone People Latent Gain Energy",
      "Zone People Latent Gain Rate",
      "Zone People Total Heating Energy",
      "Zone People Total Heating Rate",

      // TODO: check if has thermal comfort enabled
      // Ruby pseudo code...
      //zone.spaces.each do |s|
        //next if s.people.size == 0
        //s.people.each do |p|
          //pd = p.peopleDefinition
          //next if pd.numThermalComfortModelTypes == 0
          //for i in (0..pd.numThermalComfortModelTypes-1)
            //t = pd.getThermalComfortModelType(i)
            //if t in ["Fanger", "Pierce", "KSU", "AdaptiveASH55", "AdaptiveCEN15251"]
              //puts t
            //end
          //end
        //end
      //end
      // JM: Not sure which model outputs these, maybe all
      "Zone Thermal Comfort Mean Radiant Temperature",
      "Zone Thermal Comfort Operative Temperature",
      "Zone Thermal Comfort Clothing Surface Temperature",
      // if Fanger
      "Zone Thermal Comfort Fanger Model PMV",
      "Zone Thermal Comfort Fanger Model PPD",
      // if Pierce
      "Zone Thermal Comfort Pierce Model Effective Temperature PMV",
      "Zone Thermal Comfort Pierce Model Standard Effective Temperature PMV",
      "Zone Thermal Comfort Pierce Model Discomfort Index",
      "Zone Thermal Comfort Pierce Model Thermal Sensation Index",
      // if KSU
      "Zone Thermal Comfort KSU Model Thermal Sensation Index",
      // if AdaptiveASH55
      "Zone Thermal Comfort ASHRAE 55 Adaptive Model 80%% Acceptability Status",
      "Zone Thermal Comfort ASHRAE 55 Adaptive Model 90%% Acceptability Status",
      "Zone Thermal Comfort ASHRAE 55 Adaptive Model Running Average Outdoor Air Temperature",
      "Zone Thermal Comfort ASHRAE 55 Adaptive Model Temperature",
      // if AdaptiveCEN15251
      "Zone Thermal Comfort CEN 15251 Adaptive Model Category I Status",
      "Zone Thermal Comfort CEN 15251 Adaptive Model Category II Status",
      "Zone Thermal Comfort CEN 15251 Adaptive Model Category III Status",
      "Zone Thermal Comfort CEN 15251 Adaptive Model Running Average Outdoor Air Temperature",
      "Zone Thermal Comfort CEN 15251 Adaptive Model Temperature",



      // Steam Equipment
      "Zone Steam Equipment District Heating Rate",
      "Zone Steam Equipment District Heating Energy",
      "Zone Steam Equipment Radiant Heating Energy",
      "Zone Steam Equipment Radiant Heating Rate",
      "Zone Steam Equipment Convective Heating Energy",
      "Zone Steam Equipment Convective Heating Rate",
      "Zone Steam Equipment Latent Gain Energy",
      "Zone Steam Equipment Latent Gain Rate",
      "Zone Steam Equipment Lost Heat Energy",
      "Zone Steam Equipment Lost Heat Rate",
      "Zone Steam Equipment Total Heating Energy",
      "Zone Steam Equipment Total Heating Rate",

      /*
       * Zone Outputs from Daylighting Controls
       */

      "Daylighting Lighting Power Multiplier",

      // DLM: this is a static list so no dynamic
      //if (primaryDaylightingControl()){
        "Daylighting Reference Point 1 Illuminance",
        "Daylighting Reference Point 1 Glare Index",
        "Daylighting Reference Point 1 Glare Index Setpoint Exceeded Time",
        "Daylighting Reference Point 1 Daylight Illuminance Setpoint Exceeded Time",
      //}

      //if (secondaryDaylightingControl()){
        "Daylighting Reference Point 2 Illuminance",
        "Daylighting Reference Point 2 Glare Index",
        "Daylighting Reference Point 2 Glare Index Setpoint Exceeded Time",
        "Daylighting Reference Point 2 Daylight Illuminance Setpoint Exceeded Time",
      //}


      /*
       * Zone Outputs from Surfaces
       */


      // Opaque
      // Output variables applicable to opaque heat transfer surfaces (FLOOR, WALL, ROOF, DOOR).
      // if Output:Diagnostics is used
      //"Zone Opaque Surface Inside Face Conduction",
      //"Zone Opaque Surface Inside Faces Total Conduction Heat Gain Rate",
      //"Zone Opaque Surface Inside Faces Total Conduction Heat Loss Rate",
      //"Zone Opaque Surface Inside Faces Total Conduction Heat Gain Energy",
      //"Zone Opaque Surface Inside Faces Total Conduction Heat Loss Energy",
      //"Zone Opaque Surface Outside Face Conduction",
      //"Zone Opaque Surface Outside Face Conduction Gain",
      //"Zone Opaque Surface Outside Face Conduction Loss",

      // Window
      "Zone Windows Total Transmitted Solar Radiation Rate",
      "Zone Windows Total Transmitted Solar Radiation Energy",

      "Zone Windows Total Heat Gain Rate",
      "Zone Windows Total Heat Gain Energy",
      "Zone Windows Total Heat Loss Rate",
      "Zone Windows Total Heat Loss Energy",

      "Zone Exterior Windows Total Transmitted Beam Solar Radiation Rate",
      "Zone Exterior Windows Total Transmitted Beam Solar Radiation Energy",
      "Zone Exterior Windows Total Transmitted Diffuse Solar Radiation Rate",
      "Zone Exterior Windows Total Transmitted Diffuse Solar Radiation Energy",

      "Zone Interior Windows Total Transmitted Beam Solar Radiation Rate",
      "Zone Interior Windows Total Transmitted Beam Solar Radiation Energy",
      "Zone Interior Windows Total Transmitted Diffuse Solar Radiation Rate",
      "Zone Interior Windows Total Transmitted Diffuse Solar Radiation Energy",

      /*
       * Zone Outputs from other
       */

      // Infiltration
      // If infiltration is used
      "Zone Infiltration Sensible Heat Loss Energy",
      "Zone Infiltration Sensible Heat Gain Energy",
      "Zone Infiltration Latent Heat Loss Energy",
      "Zone Infiltration Latent Heat Gain Energy",
      "Zone Infiltration Total Heat Loss Energy",
      "Zone Infiltration Total Heat Gain Energy",
      "Zone Infiltration Current Density Volume Flow Rate",
      "Zone Infiltration Standard Density Volume Flow Rate",
      "Zone Infiltration Current Density Volume",
      "Zone Infiltration Standard Density Volume",
      "Zone Infiltration Mass",
      "Zone Infiltration Mass Flow Rate",
      "Zone Infiltration Air Change Rate",


      // ZoneVentilation:DesignFlowRate
      "Zone Ventilation Sensible Heat Loss Energy",
      "Zone Ventilation Sensible Heat Gain Energy",
      "Zone Ventilation Latent Heat Loss Energy",
      "Zone Ventilation Latent Heat Gain Energy",
      "Zone Ventilation Total Heat Loss Energy",
      "Zone Ventilation Total Heat Gain Energy",
      "Zone Ventilation Current Density Volume Flow Rate",
      "Zone Ventilation Standard Density Volume Flow Rate",
      "Zone Ventilation Current Density Volume",
      "Zone Ventilation Standard Density Volume",
      "Zone Ventilation Mass",
      "Zone Ventilation Mass Flow Rate",
      "Zone Ventilation Air Change Rate",
      "Zone Ventilation Fan Electric Energy",
      "Zone Ventilation Air Inlet Temperature",



      "Zone Predicted Sensible Load to Setpoint Heat Transfer Rate",
      "Zone Predicted Sensible Load to Heating Setpoint Heat Transfer Rate",
      "Zone Predicted Sensible Load to Cooling Setpoint Heat Transfer Rate",
      "Zone Predicted Moisture Load Moisture Transfer Rate",
      "Zone Predicted Moisture Load to Humidifying Setpoint Moisture Transfer Rate",
      "Zone Predicted Moisture Load to Dehumidifying Setpoint Moisture Transfer Rate",

      "Zone Thermostat Control Type",
      "Zone Thermostat Heating Setpoint Temperature",
      "Zone Thermostat Cooling Setpoint Temperature",

      "Zone Mechanical Ventilation No Load Heat Removal Energy",
      "Zone Mechanical Ventilation Cooling Load Increase Energy",
      "Zone Mechanical Ventilation Cooling Load Increase Energy Due to Overheating Energy",
      "Zone Mechanical Ventilation Cooling Load Decrease Energy",
      "Zone Mechanical Ventilation No Load Heat Addition Energy",
      "Zone Mechanical Ventilation No Load Heat Removal Energy",
      "Zone Mechanical Ventilation Cooling Load Increase Energy",
      "Zone Mechanical Ventilation Cooling Load Increase Energy Due to Overheating Energy",
      "Zone Mechanical Ventilation Cooling Load Decrease Energy",
      "Zone Mechanical Ventilation No Load Heat Addition Energy",
      "Zone Mechanical Ventilation Heating Load Increase Energy",
      "Zone Mechanical Ventilation Heating Load Increase Energy Due to Overcooling Energy",
      "Zone Mechanical Ventilation Heating Load Decrease Energy",
      "Zone Mechanical Ventilation Mass Flow Rate",
      "Zone Mechanical Ventilation Mass",
      "Zone Mechanical Ventilation Standard Density Volume Flow Rate",
      "Zone Mechanical Ventilation Standard Density Volume",
      "Zone Mechanical Ventilation Current Density Volume Flow Rate",
      "Zone Mechanical Ventilation Current Density Volume",
      "Zone Mechanical Ventilation Air Changes per Hour",


      // ZoneContaminantAirBalance
      // The following output variables are available when Carbon Dioxide Concentration = Yes.
      "Zone Air CO2 Internal Gain Volume Flow Rate",
      "Zone Air CO2 Concentration",
      // The following output variable is available when Generic Contaminant Concentration = Yes.
      "Zone Generic Air Contaminant Generation Volume Flow Rate",
      "Zone Air Generic Air Contaminant Concentration",

      // Refrigeration:AirChiller
      // TODO: implement test to see if AirChiller.
      "Refrigeration Zone Air Chiller Sensible Cooling Rate",
      "Refrigeration Zone Air Chiller Sensible Cooling Energy",
      "Refrigeration Zone Air Chiller Heating Rate",
      "Refrigeration Zone Air Chiller Heating Energy",
      "Refrigeration Zone Air Chiller Latent Cooling Rate",
      "Refrigeration Zone Air Chiller Latent Cooling Energy",
      "Refrigeration Zone Air Chiller Total Cooling Rate",
      "Refrigeration Zone Air Chiller Total Cooling Energy",
      "Refrigeration Zone Air Chiller Water Removed Mass Flow Rate",

      // RefrigerationWalkin
      // TODO: test
      "Refrigeration Walk In Zone Sensible Cooling Rate",
      "Refrigeration Walk In Zone Sensible Cooling Energy",
      "Refrigeration Walk In Zone Sensible Heating Rate",
      "Refrigeration Walk In Zone Sensible Heating Energy",
      "Refrigeration Walk In Zone Latent Rate",
      "Refrigeration Walk In Zone Latent Energy"



    };
    return result;
  }

  IddObjectType ThermalZone_Impl::iddObjectType() const {
    return ThermalZone::iddObjectType();
  }

  std::vector<HVACComponent> ThermalZone_Impl::edges(const boost::optional<HVACComponent> & prev)
  {
    std::vector<HVACComponent> edges;
    auto returncomps = subsetCastVector<HVACComponent>(returnPortList().modelObjects());
    for ( auto & comp : returncomps ) {
      edges.push_back(comp);
    }
    return edges;
  }

  int ThermalZone_Impl::multiplier() const {
    boost::optional<int> value = getInt(OS_ThermalZoneFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalZone_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_ThermalZoneFields::Multiplier);
  }

  boost::optional<double> ThermalZone_Impl::ceilingHeight() const {
    return getDouble(OS_ThermalZoneFields::CeilingHeight,true);
  }

  OSOptionalQuantity ThermalZone_Impl::getCeilingHeight(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_ThermalZoneFields::CeilingHeight,true,returnIP);
    return value;
  }

  bool ThermalZone_Impl::isCeilingHeightDefaulted() const {
    return isEmpty(OS_ThermalZoneFields::CeilingHeight);
  }

  bool ThermalZone_Impl::isCeilingHeightAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalZoneFields::CeilingHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  boost::optional<double> ThermalZone_Impl::volume() const {
    return getDouble(OS_ThermalZoneFields::Volume,true);
  }

  OSOptionalQuantity ThermalZone_Impl::getVolume(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_ThermalZoneFields::Volume,true,returnIP);
    return value;
  }

  bool ThermalZone_Impl::isVolumeDefaulted() const {
    return isEmpty(OS_ThermalZoneFields::Volume);
  }

  bool ThermalZone_Impl::isVolumeAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalZoneFields::Volume, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  boost::optional<std::string> ThermalZone_Impl::zoneInsideConvectionAlgorithm() const {
    return getString(OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm,true);
  }

  boost::optional<std::string> ThermalZone_Impl::zoneOutsideConvectionAlgorithm() const {
    return getString(OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm,true);
  }

  std::string ThermalZone_Impl::zoneConditioningEquipmentListName() const {
    boost::optional<std::string> value = getString(OS_ThermalZoneFields::ZoneConditioningEquipmentListName,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalZone_Impl::fractionofZoneControlledbyPrimaryDaylightingControl() const {
    boost::optional<double> value = getDouble(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ThermalZone_Impl::getFractionofZoneControlledbyPrimaryDaylightingControl(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  bool ThermalZone_Impl::isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const {
    return isEmpty(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl);
  }

  double ThermalZone_Impl::fractionofZoneControlledbySecondaryDaylightingControl() const {
    boost::optional<double> value = getDouble(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ThermalZone_Impl::getFractionofZoneControlledbySecondaryDaylightingControl(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  bool ThermalZone_Impl::isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const {
    return isEmpty(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl);
  }

  bool ThermalZone_Impl::setMultiplier(int multiplier) {
    bool result = setInt(OS_ThermalZoneFields::Multiplier, multiplier);
    return result;
  }

  void ThermalZone_Impl::resetMultiplier() {
    bool result = setString(OS_ThermalZoneFields::Multiplier, "");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setCeilingHeight(boost::optional<double> ceilingHeight) {
    bool result = false;
    if (ceilingHeight) {
      result = setDouble(OS_ThermalZoneFields::CeilingHeight, ceilingHeight.get());
    } else {
      result = setString(OS_ThermalZoneFields::CeilingHeight, "");
    }
    OS_ASSERT(result);
    return result;
  }

  bool ThermalZone_Impl::setCeilingHeight(double ceilingHeight) {
    bool result = setDouble(OS_ThermalZoneFields::CeilingHeight, ceilingHeight);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalZone_Impl::setCeilingHeight(const OSOptionalQuantity& ceilingHeight) {
    bool result;
    if (ceilingHeight.isSet()) {
      result = setQuantity(OS_ThermalZoneFields::CeilingHeight,ceilingHeight.get());
    } else {
      result = setString(OS_ThermalZoneFields::CeilingHeight, "");
    }
    return result;
  }

  void ThermalZone_Impl::resetCeilingHeight() {
    bool result = setString(OS_ThermalZoneFields::CeilingHeight, "");
    OS_ASSERT(result);
  }

  void ThermalZone_Impl::autocalculateCeilingHeight() {
    bool result = setString(OS_ThermalZoneFields::CeilingHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setVolume(boost::optional<double> volume) {
    bool result = false;
    if (volume) {
      result = setDouble(OS_ThermalZoneFields::Volume, volume.get());
    } else {
      result = setString(OS_ThermalZoneFields::Volume, "");
    }
    OS_ASSERT(result);
    return result;
  }

  bool ThermalZone_Impl::setVolume(double volume) {
    bool result = setDouble(OS_ThermalZoneFields::Volume, volume);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalZone_Impl::setVolume(const OSOptionalQuantity& volume) {
    bool result;
    if (volume.isSet()) {
      result = setQuantity(OS_ThermalZoneFields::Volume,volume.get());
    } else {
      result = setString(OS_ThermalZoneFields::Volume, "");
    }
    return result;
  }

  void ThermalZone_Impl::resetVolume() {
    bool result = setString(OS_ThermalZoneFields::Volume, "");
    OS_ASSERT(result);
  }

  void ThermalZone_Impl::autocalculateVolume() {
    bool result = setString(OS_ThermalZoneFields::Volume, "autocalculate");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm) {
    bool result = false;
    if (zoneInsideConvectionAlgorithm) {
      result = setString(OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm, zoneInsideConvectionAlgorithm.get());
    } else {
      result = setString(OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm, "");
    }
    return result;
  }

  bool ThermalZone_Impl::setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm) {
    bool result = setString(OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm, zoneInsideConvectionAlgorithm);
    return result;
  }

  void ThermalZone_Impl::resetZoneInsideConvectionAlgorithm() {
    bool result = setString(OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm, "");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm) {
    bool result = false;
    if (zoneOutsideConvectionAlgorithm) {
      result = setString(OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm, zoneOutsideConvectionAlgorithm.get());
    } else {
      result = setString(OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm, "");
    }
    return result;
  }

  bool ThermalZone_Impl::setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm) {
    bool result = setString(OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm, zoneOutsideConvectionAlgorithm);
    return result;
  }

  void ThermalZone_Impl::resetZoneOutsideConvectionAlgorithm() {
    bool result = setString(OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm, "");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName) {
    bool result = setString(OS_ThermalZoneFields::ZoneConditioningEquipmentListName, zoneConditioningEquipmentListName);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalZone_Impl::setFractionofZoneControlledbyPrimaryDaylightingControl(double fractionofZoneControlledbyPrimaryDaylightingControl) {

    double secondaryFrac = fractionofZoneControlledbySecondaryDaylightingControl();
    if ( (fractionofZoneControlledbyPrimaryDaylightingControl + secondaryFrac) > 1.0) {
      LOG(Error, "Fraction of Zone Controlled by Secondary Daylight Control is " << secondaryFrac
          << " and you supplied a Primary Fraction of " << fractionofZoneControlledbyPrimaryDaylightingControl
          << " which would result in a sum greater than 1.0");
      return false;
    }
    bool result = setDouble(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl, fractionofZoneControlledbyPrimaryDaylightingControl);
    return result;
  }

  bool ThermalZone_Impl::setFractionofZoneControlledbyPrimaryDaylightingControl(const Quantity& fractionofZoneControlledbyPrimaryDaylightingControl) {
    // Get double
    OptionalDouble value = getDoubleFromQuantity(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl,
                                                 fractionofZoneControlledbyPrimaryDaylightingControl);
    if (!value) {
      return false;
    } else {
      // Call the method with the double, where the sanity check is
      return setFractionofZoneControlledbyPrimaryDaylightingControl(value.get());
    }
  }

  void ThermalZone_Impl::resetFractionofZoneControlledbyPrimaryDaylightingControl() {
    bool result = setString(OS_ThermalZoneFields::FractionofZoneControlledbyPrimaryDaylightingControl, "");
    OS_ASSERT(result);
  }

  bool ThermalZone_Impl::setFractionofZoneControlledbySecondaryDaylightingControl(double fractionofZoneControlledbySecondaryDaylightingControl) {
    double primaryFrac = fractionofZoneControlledbyPrimaryDaylightingControl();
    if ( (fractionofZoneControlledbySecondaryDaylightingControl + primaryFrac) > 1.0) {
      LOG(Error, "Fraction of Zone Controlled by Primary Daylight Control is " << primaryFrac
          << " and you supplied a Secondary Fraction of " << fractionofZoneControlledbySecondaryDaylightingControl
          << " which would result in a sum greater than 1.0");
      return false;
    }
    bool result = setDouble(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl, fractionofZoneControlledbySecondaryDaylightingControl);
    return result;
  }

  bool ThermalZone_Impl::setFractionofZoneControlledbySecondaryDaylightingControl(const Quantity& fractionofZoneControlledbySecondaryDaylightingControl) {
    // Get double
    OptionalDouble value = getDoubleFromQuantity(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl,
                                                 fractionofZoneControlledbySecondaryDaylightingControl);
    if (!value) {
      return false;
    } else {
      // Call the method with the double, where the sanity check is
      return setFractionofZoneControlledbySecondaryDaylightingControl(value.get());
    }
  }

  void ThermalZone_Impl::resetFractionofZoneControlledbySecondaryDaylightingControl() {
    bool result = setString(OS_ThermalZoneFields::FractionofZoneControlledbySecondaryDaylightingControl, "");
    OS_ASSERT(result);
  }

  PortList ThermalZone_Impl::returnPortList() const
  {
    boost::optional<PortList> pl = getObject<ModelObject>().getModelObjectTarget<PortList>(OS_ThermalZoneFields::ZoneReturnAirPortList);
    OS_ASSERT(pl);
    return pl.get();
  }

  unsigned ThermalZone_Impl::returnAirPort()
  {
    return returnPortList().port(0);
  }

  unsigned ThermalZone_Impl::zoneAirPort()
  {
    return OS_ThermalZoneFields::ZoneAirNodeName;
  }

  OptionalModelObject ThermalZone_Impl::returnAirModelObject()
  {
    return returnPortList().connectedObject(this->returnAirPort());
  }

  std::vector<ModelObject> ThermalZone_Impl::returnAirModelObjects() const
  {
    return returnPortList().modelObjects();
  }

  Node ThermalZone_Impl::zoneAirNode()
  {
    return this->connectedObject(this->zoneAirPort())->cast<Node>();
  }

  boost::optional<DaylightingControl> ThermalZone_Impl::primaryDaylightingControl() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DaylightingControl>(OS_ThermalZoneFields::PrimaryDaylightingControlName);
  }

  bool ThermalZone_Impl::setPrimaryDaylightingControl(const DaylightingControl& daylightingControl)
  {
    return setDaylightingControlsAndIlluminanceMaps(daylightingControl, this->secondaryDaylightingControl(), this->illuminanceMap());
  }

  void ThermalZone_Impl::resetPrimaryDaylightingControl()
  {
    bool test = setString(OS_ThermalZoneFields::PrimaryDaylightingControlName, "");
    OS_ASSERT(test);
    resetSecondaryDaylightingControl();
  }

  boost::optional<DaylightingControl> ThermalZone_Impl::secondaryDaylightingControl() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DaylightingControl>(OS_ThermalZoneFields::SecondaryDaylightingControlName);
  }

  bool ThermalZone_Impl::setSecondaryDaylightingControl(const DaylightingControl& daylightingControl)
  {
    return setDaylightingControlsAndIlluminanceMaps(this->primaryDaylightingControl(), daylightingControl, this->illuminanceMap());
  }

  void ThermalZone_Impl::resetSecondaryDaylightingControl()
  {
    bool test = setString(OS_ThermalZoneFields::SecondaryDaylightingControlName, "");
    OS_ASSERT(test);
  }

  boost::optional<IlluminanceMap> ThermalZone_Impl::illuminanceMap() const
  {
    return getObject<ModelObject>().getModelObjectTarget<IlluminanceMap>(OS_ThermalZoneFields::IlluminanceMapName);
  }

  bool ThermalZone_Impl::setIlluminanceMap(const IlluminanceMap& illuminanceMap)
  {
    return setDaylightingControlsAndIlluminanceMaps(this->primaryDaylightingControl(), this->secondaryDaylightingControl(), illuminanceMap);
  }

  void ThermalZone_Impl::resetIlluminanceMap()
  {
    bool test = setString(OS_ThermalZoneFields::IlluminanceMapName, "");
    OS_ASSERT(test);
  }

  bool ThermalZone_Impl::setDaylightingControlsAndIlluminanceMaps(const boost::optional<DaylightingControl>& primaryDaylightingControl,
                                                                  const boost::optional<DaylightingControl>& secondaryDaylightingControl,
                                                                  const boost::optional<IlluminanceMap>& illuminanceMap)
  {
    resetPrimaryDaylightingControl();
    resetSecondaryDaylightingControl();
    resetIlluminanceMap();

    bool result = true;
    if (primaryDaylightingControl){
      result = setPointer(OS_ThermalZoneFields::PrimaryDaylightingControlName, primaryDaylightingControl->handle());
    }

    if (secondaryDaylightingControl){
      if (isEmpty(OS_ThermalZoneFields::PrimaryDaylightingControlName)){
        result = false;
      }else{
        result = result && setPointer(OS_ThermalZoneFields::SecondaryDaylightingControlName, secondaryDaylightingControl->handle());
      }
    }

    if (illuminanceMap){
      boost::optional<Space> space = illuminanceMap->space();
      if (space && (space->thermalZone()) && (space->thermalZone()->handle() == this->handle())){
        result = result && setPointer(OS_ThermalZoneFields::IlluminanceMapName, illuminanceMap->handle());
      }
    }

    return result;
  }

  void ThermalZone_Impl::checkDaylightingControlsAndIlluminanceMaps()
  {
    setDaylightingControlsAndIlluminanceMaps(this->primaryDaylightingControl(), this->secondaryDaylightingControl(), this->illuminanceMap());
  }

  boost::optional<RenderingColor> ThermalZone_Impl::renderingColor() const
  {
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_ThermalZoneFields::GroupRenderingName);
  }

  bool ThermalZone_Impl::setRenderingColor(const RenderingColor& renderingColor)
  {
    return setPointer(OS_ThermalZoneFields::GroupRenderingName, renderingColor.handle());
  }

  void ThermalZone_Impl::resetRenderingColor()
  {
    bool test = setString(OS_ThermalZoneFields::GroupRenderingName, "");
    OS_ASSERT(test);
  }

  std::vector<Space> ThermalZone_Impl::spaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<Space>(
      Space::iddObjectType());
  }

  double ThermalZone_Impl::floorArea() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.floorArea();
    }
    return result;
  }

  double ThermalZone_Impl::exteriorSurfaceArea() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.exteriorArea();
    }
    return result;
  }

  double ThermalZone_Impl::exteriorWallArea() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.exteriorWallArea();
    }
    return result;
  }

  double ThermalZone_Impl::airVolume() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.volume();
    }
    return result;
  }

  double ThermalZone_Impl::numberOfPeople() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.numberOfPeople();
    }
    return result;
  }

  double ThermalZone_Impl::peoplePerFloorArea() const {
    double area = floorArea();
    double np = numberOfPeople();
    if (equal(area,0.0)) {
      if (equal(np,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].peoplePerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return np / area;
  }

  double ThermalZone_Impl::floorAreaPerPerson() const {
    double area = floorArea();
    double np = numberOfPeople();
    if (equal(np,0.0)) {
      if (spaces().size() == 1u) {
        return spaces()[0].floorAreaPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return area / np;
  }

  double ThermalZone_Impl::lightingPower() const {
    double result(0.0);
    for (const Space& space : spaces()){
      result += space.lightingPower();
    }
    return result;
  }

  double ThermalZone_Impl::lightingPowerPerFloorArea() const {
    double area = floorArea();
    double lp = lightingPower();
    if (equal(area,0.0)) {
      if (equal(lp,0.0)) {
        return 0.0;
      }
      else if (spaces().size() == 1u) {
        return spaces()[0].lightingPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return lp / area;
  }

  double ThermalZone_Impl::lightingPowerPerPerson() const {
    double np = numberOfPeople();
    double lp = lightingPower();
    if (equal(np,0.0)) {
      if (equal(lp,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].lightingPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return lp / np;
  }

  double ThermalZone_Impl::electricEquipmentPower() const {
    double result(0.0);
    for (const Space& space : spaces()){
      result += space.electricEquipmentPower();
    }
    return result;
  }

  double ThermalZone_Impl::electricEquipmentPowerPerFloorArea() const {
    double area = floorArea();
    double ep = electricEquipmentPower();
    if (equal(area,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].electricEquipmentPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / area;
  }

  double ThermalZone_Impl::electricEquipmentPowerPerPerson() const {
    double np = numberOfPeople();
    double ep = electricEquipmentPower();
    if (equal(np,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].electricEquipmentPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / np;
  }

  double ThermalZone_Impl::gasEquipmentPower() const {
    double result(0.0);
    for (const Space& space : spaces()){
      result += space.gasEquipmentPower();
    }
    return result;
  }

  double ThermalZone_Impl::gasEquipmentPowerPerFloorArea() const {
    double area = floorArea();
    double ep = gasEquipmentPower();
    if (equal(area,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].gasEquipmentPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / area;
  }

  double ThermalZone_Impl::gasEquipmentPowerPerPerson() const {
    double np = numberOfPeople();
    double ep = gasEquipmentPower();
    if (equal(np,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].gasEquipmentPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / np;
  }

  double ThermalZone_Impl::infiltrationDesignFlowRate() const {
    double result(0.0);
    for (const Space& space : spaces()) {
      result += space.infiltrationDesignFlowRate();
    }
    return result;
  }

  double ThermalZone_Impl::infiltrationDesignFlowPerSpaceFloorArea() const {
    double area = floorArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerSpaceFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double ThermalZone_Impl::infiltrationDesignFlowPerExteriorSurfaceArea() const {
    double area = exteriorSurfaceArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerExteriorSurfaceArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double ThermalZone_Impl::infiltrationDesignFlowPerExteriorWallArea() const {
    double area = exteriorWallArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerExteriorWallArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double ThermalZone_Impl::infiltrationDesignAirChangesPerHour() const {
    double volume = airVolume();
    double idfr = infiltrationDesignFlowRate();
    if (equal(volume,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignAirChangesPerHour();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return convert(idfr/volume,"1/s","1/h").get();
  }

  boost::optional<std::string> ThermalZone_Impl::isConditioned() const {
    boost::optional<std::string> result;

    OptionalSqlFile mySqlFile = model().sqlFile();

    // TODO: this should not require sql file

    if (mySqlFile) {
      // now use sql query to check if conditioned
      std::string zoneName = boost::to_upper_copy(name(true).get());
      result = mySqlFile->execAndReturnFirstString("SELECT Value from tabulardatawithstrings where (reportname = 'InputVerificationandResultsSummary') and (ReportForString = 'Entire Facility') and (TableName = 'Zone Summary'  ) and (ColumnName ='Conditioned (Y/N)') and (RowName = '" + zoneName + "')");
      if (!result){
        LOG(Error, "Query for " << briefDescription() << " isConditioned failed.");
      }
    }

    return result;
  }

  bool ThermalZone_Impl::isRemovable() const
  {
    return true;
  }

  boost::optional<ThermostatSetpointDualSetpoint> ThermalZone_Impl::thermostatSetpointDualSetpoint() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ThermostatSetpointDualSetpoint>(OS_ThermalZoneFields::ThermostatName);
  }

  bool ThermalZone_Impl::setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint & thermostat)
  {
    return setThermostat(thermostat);
  }

  void ThermalZone_Impl::resetThermostatSetpointDualSetpoint()
  {
    // This will reset other thermostat types, but I think that is ok
    resetThermostat();
  }

  boost::optional<Thermostat> ThermalZone_Impl::thermostat() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Thermostat>(OS_ThermalZoneFields::ThermostatName);
  }

  bool ThermalZone_Impl::setThermostat(const Thermostat & thermostat)
  {
    auto t_model = model();

    if( t_model != thermostat.model() ) {
      return false;
    }

    if( auto currentZone = thermostat.thermalZone() ) {
      if( currentZone->handle() == handle() ) {
        // or should it be false?
        // I think this is similar to what you would see in
        // Lights::setSpace() under similar conditions
        return true;
      } else {
        auto thermostatClone = thermostat.clone(t_model).cast<Thermostat>();
        resetThermostat();
        return setPointer(OS_ThermalZoneFields::ThermostatName, thermostatClone.handle());
      }
    } else {
      resetThermostat();
      return setPointer(OS_ThermalZoneFields::ThermostatName, thermostat.handle());
    }
  }

  void ThermalZone_Impl::resetThermostat()
  {
    if( boost::optional<Thermostat> thermostat = this->thermostat() ) {
      thermostat->remove();
    }

    setString(OS_ThermalZoneFields::ThermostatName, "");
  }

  boost::optional<ZoneControlHumidistat> ThermalZone_Impl::zoneControlHumidistat() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ZoneControlHumidistat>(OS_ThermalZoneFields::HumidistatName);
  }

  bool ThermalZone_Impl::setZoneControlHumidistat(const ZoneControlHumidistat & humidistat)
  {
    auto t_model = model();

    if( t_model != humidistat.model() ) {
      return false;
    }

    if( auto currentZone = humidistat.controlledZone() ) {
      if( currentZone->handle() == handle() ) {
        return true;
      } else {
        auto humidistatClone = humidistat.clone(t_model).cast<ZoneControlHumidistat>();
        resetZoneControlHumidistat();
        return setPointer(OS_ThermalZoneFields::HumidistatName, humidistatClone.handle());
      }
    } else {
      resetZoneControlHumidistat();
      return setPointer(OS_ThermalZoneFields::HumidistatName, humidistat.handle());
    }
  }

  void ThermalZone_Impl::resetZoneControlHumidistat()
  {
    if( boost::optional<ZoneControlHumidistat> humidistat = this->zoneControlHumidistat() ) {
      humidistat->remove();
    }

    setString(OS_ThermalZoneFields::HumidistatName, "");
  }

  /// Combines all spaces referencing this zone into a single space referencing this zone.
  /// If this zone has no spaces referencing it, then an uninitialized optional space is returned.
  /// If this zone has one space referencing it, then that space is returned.
  /// If this zone is referenced by more than one space, then geometry from all spaces is added to a single zone.
  /// The space origin is at the minimum x, y, z coordinate of all space origins, direction of relative north is preserved if same for all spaces.
  /// If all spaces reference the same building story then that is preserved, otherwise it is cleared.
  /// If all spaces reference the same space type then that is preserved, otherwise space loads from the space type are applied to the new space directly.
  /// Direct child space loads are converted to absolute levels.
  /// Constructions and schedules are hard applied to all child surfaces and loads.
  /// Surfaces referencing other surfaces within the space are converted to interior partitions.
  boost::optional<Space> ThermalZone_Impl::combineSpaces()
  {
    std::vector<Space> spaces = this->spaces();

    if (spaces.empty()){
      return boost::none;
    }else if (spaces.size() == 1){
      return spaces[0];
    }

    // sort by space name
    std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());

    // if these variables are set, then they are not defaulted and are common to all spaces
    boost::optional<BuildingStory> buildingStory = spaces[0].buildingStory();
    boost::optional<SpaceType> spaceType = spaces[0].spaceType();
    boost::optional<double> directionofRelativeNorth;
    if (!spaces[0].isDirectionofRelativeNorthDefaulted()){
      directionofRelativeNorth = spaces[0].directionofRelativeNorth();
    }
    double xOrigin = spaces[0].xOrigin();
    double yOrigin = spaces[0].yOrigin();
    double zOrigin = spaces[0].zOrigin();
    double sumFloorArea = 0.0;
    double sumNumberOfPeople = 0.0;
    double sumVolume = 0.0;
    double totalFloorArea = 0.0; // only area included in total floor area
    bool needToSetFloorArea = false;
    bool anyNotPartofTotalFloorArea = false;
    bool partofTotalFloorArea = spaces[0].partofTotalFloorArea();

    // variables for merging outdoor area
    boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = spaces[0].designSpecificationOutdoorAir();
    bool allDesignSpecificationOutdoorAirDefaulted = spaces[0].isDesignSpecificationOutdoorAirDefaulted();
    bool anyDesignSpecificationOutdoorAirSchedules = false;
    bool anyMaxOutdoorAirMethod = false;
    bool anySumOutdoorAirMethod = false;
    double sumOutdoorAirForPeople = 0.0;
    double sumOutdoorAirForFloorArea = 0.0;
    double sumOutdoorAirRate = 0.0;
    double sumOutdoorAirForVolume = 0.0;

    // Quick check to see what kind of ventilation methods are used
    for (Space space : spaces){
      if (boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = space.designSpecificationOutdoorAir()) {
        if (istringEqual("Maximum", designSpecificationOutdoorAir->outdoorAirMethod())){
          anyMaxOutdoorAirMethod = true;
        } else if(istringEqual("Sum", designSpecificationOutdoorAir->outdoorAirMethod())) {
          anySumOutdoorAirMethod = true;
        }
      }
    }

    // find common variables for the new space
    for (Space space : spaces){

      // if all spaces are on the same building story use that, otherwise clear it
      if (space.buildingStory()){
        if (buildingStory && (buildingStory->handle() == space.buildingStory()->handle())){
          // no-op
        }else{
          buildingStory.reset();
        }
      }else{
        buildingStory.reset();
      }

      // if all spaces have the same space type use that, otherwise clear it
      if (space.spaceType()){
        if (spaceType && (spaceType->handle() == space.spaceType()->handle())){
          // no-op
        }else{
          spaceType.reset();
        }
      }else{
        spaceType.reset();
      }

      // if all spaces have same directionOfRelativeNorth use that, otherwise clear it
      if (!space.isDirectionofRelativeNorthDefaulted()){
        if (directionofRelativeNorth && (*directionofRelativeNorth == space.directionofRelativeNorth())){
          // no-op
        }else{
          directionofRelativeNorth.reset();
        }
      }

      // pick the lower left corner if specified
      xOrigin = std::min(xOrigin, space.xOrigin());
      yOrigin = std::min(yOrigin, space.yOrigin());
      zOrigin = std::min(zOrigin, space.zOrigin());

      double floorArea = space.floorArea();
      sumFloorArea += floorArea;

      double numberOfPeople = space.numberOfPeople();
      sumNumberOfPeople += numberOfPeople;

      double volume = space.volume();
      sumVolume += volume;

      // check if we have to explicitly set floor area and hard size loads
      for (const Surface& surface : space.surfaces()) {
        if (istringEqual(surface.surfaceType(), "Floor")){

          // air wall floors do not count in floor area
          if (surface.isAirWall()){
            needToSetFloorArea = true;
            break;
          }

          auto adjacentSurface = surface.adjacentSurface();
          if (adjacentSurface){
            auto adjacentSpace = adjacentSurface->space();
            if (adjacentSpace){
              auto adjacentThermalZone = adjacentSpace->thermalZone();
              if (adjacentThermalZone){
                if (adjacentThermalZone->handle() == this->handle())
                {
                  // this surface is completely inside the zone, need to set floor area since this surface will be removed
                  needToSetFloorArea = true;
                  break;
                }
              }
            }
          }
        }
      }

      // space floor area is counted if any space is part of floor area
      if (space.partofTotalFloorArea()){
        partofTotalFloorArea = true;
        totalFloorArea += floorArea;

        if (anyNotPartofTotalFloorArea){
          needToSetFloorArea = true;
        }

      }else{
        anyNotPartofTotalFloorArea = true;

        if(partofTotalFloorArea){
          needToSetFloorArea = true;
        }
      }

      // if all spaces have the same outdoor air specification use that, otherwise clear it
      boost::optional<DesignSpecificationOutdoorAir> thisDesignSpecificationOutdoorAir = space.designSpecificationOutdoorAir();
      if (thisDesignSpecificationOutdoorAir){

        if (designSpecificationOutdoorAir && (designSpecificationOutdoorAir->handle() == thisDesignSpecificationOutdoorAir->handle())){
          // no-op
        }else{
          designSpecificationOutdoorAir.reset();
        }

        if (!space.isDesignSpecificationOutdoorAirDefaulted()){
          allDesignSpecificationOutdoorAirDefaulted = false;
        }

        if (thisDesignSpecificationOutdoorAir->outdoorAirFlowRateFractionSchedule()){
          anyDesignSpecificationOutdoorAirSchedules = true;
        }

        // compute outdoor air rates in case we need them
        double outdoorAirForPeople = numberOfPeople*thisDesignSpecificationOutdoorAir->outdoorAirFlowperPerson();
        double outdoorAirForFloorArea = floorArea*thisDesignSpecificationOutdoorAir->outdoorAirFlowperFloorArea();
        double outdoorAirRate = thisDesignSpecificationOutdoorAir->outdoorAirFlowRate();
        double outdoorAirForVolume = volume*thisDesignSpecificationOutdoorAir->outdoorAirFlowAirChangesperHour();

        // First check if this space uses the Maximum method and other spaces do not
        if (istringEqual("Maximum", thisDesignSpecificationOutdoorAir->outdoorAirMethod()) && anySumOutdoorAirMethod ){
          sumOutdoorAirRate += std::max(outdoorAirForPeople,
                                        std::max(outdoorAirForFloorArea,
                                        std::max(outdoorAirRate,
                                        outdoorAirForVolume)));
        }else{
          sumOutdoorAirForPeople += outdoorAirForPeople;
          sumOutdoorAirForFloorArea += outdoorAirForFloorArea;
          sumOutdoorAirRate += outdoorAirRate;
          sumOutdoorAirForVolume += outdoorAirForVolume;
        }

      }else{
        designSpecificationOutdoorAir.reset();
        allDesignSpecificationOutdoorAirDefaulted = false;
      }

    }

    // if all spaces share a common space type, ensure that there are no absolute loads
    if (spaceType){
      for (const auto& child : spaceType->children()){
        if (child.optionalCast<SpaceLoad>()){
          if (child.cast<SpaceLoad>().isAbsolute()){
            LOG(Warn, "SpaceType '" << spaceType->name() << "' contains absolute loads, cannot be shared by combined spaces.")
            spaceType.reset();
            break;
          }
        }
      }
    }

    // set E+ floor area here if needed, this is only used for reporting total building area
    // loads are hard sized according to OpenStudio space floor area
    if (needToSetFloorArea){

      // do not allow per area loads in the space type since we are overriding floor area
      spaceType.reset();

      // don't override if user provided zone floor area
      if (isEmpty(OS_ThermalZoneFields::FloorArea)){
        LOG(Info, "ThermalZone '" << this->name().get() << "' has spaces with mis-matched 'Part of Total Floor Area' flags. "
               << "Setting set the flag to 'Yes', but hard-coding the total floor area to only take into account the spaces "
               << "that are part of total Floor Area");
        this->setDouble(OS_ThermalZoneFields::FloorArea, totalFloorArea);
      } else {
        LOG(Info, "ThermalZone " << this->name().get() << " has a user-specified Floor Area, using this number");
      }
    }

    // make the new space
    Model model = this->model();
    Space newSpace(model);
    ThermalZone thermalZone = this->getObject<ThermalZone>();
    newSpace.setThermalZone(thermalZone);
    newSpace.setXOrigin(xOrigin);
    newSpace.setYOrigin(yOrigin);
    newSpace.setZOrigin(zOrigin);
    newSpace.setPartofTotalFloorArea(partofTotalFloorArea);

    if (directionofRelativeNorth){
      newSpace.setDirectionofRelativeNorth(*directionofRelativeNorth);
    }

    if (buildingStory){
      newSpace.setBuildingStory(*buildingStory);
    }

    if (spaceType){
      newSpace.setSpaceType(*spaceType);
    }else{
      // create a new space type
      SpaceType newSpaceType(model);

      // set space type to prevent picking up building level space type
      newSpace.setSpaceType(newSpaceType);
    }

    // new space transformation
    Transformation newTransformation = newSpace.transformation();

    // set common variables for the new space
    for (Space space : spaces){

      // shift the geometry
      space.changeTransformation(newTransformation);

      // apply the space type
      if (!spaceType){
        space.hardApplySpaceType(true);
      }
      space.hardApplyConstructions();

      // get the children
      std::vector<ModelObject> children = space.children();

      // first hard size any space loads, do this before removing surfaces as
      // hard sizing may require space geometry
      for (ModelObject child : children){
        if (child.optionalCast<SpaceLoad>()){
          child.cast<SpaceLoad>().hardSize();
          child.cast<SpaceLoad>().hardApplySchedules();
        }
      }

      // now move costs over to the new space
      for (LifeCycleCost cost : space.lifeCycleCosts()){
        // new costs are in absolute units as space area is changing in the merge
        LifeCycleCost newCost(newSpace);
        newCost.setName(cost.name().get());
        newCost.setCategory(cost.category());
        newCost.setCost(cost.totalCost());
        newCost.setCostUnits("CostPerEach");
        if (!cost.isYearsFromStartDefaulted()){
          newCost.setYearsFromStart(cost.yearsFromStart());
        }
        if (!cost.isMonthsFromStartDefaulted()){
          newCost.setMonthsFromStart(cost.monthsFromStart());
        }
        if (!cost.isRepeatPeriodYearsDefaulted()){
          newCost.setRepeatPeriodYears(cost.repeatPeriodYears());
        }
        if (!cost.isRepeatPeriodMonthsDefaulted()){
          newCost.setRepeatPeriodMonths(cost.repeatPeriodMonths());
        }
      }

      // now move everything over to the new space
      for (ModelObject child : children){
        child.setParent(newSpace);
      }

      // remove the old space
      space.remove();
    }

    // merge surfaces
    boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup;
    std::set<Surface> mergedSurfaces;

    // sort by surface name
    std::vector<Surface> surfaces = newSpace.surfaces();
    std::sort(surfaces.begin(), surfaces.end(), WorkspaceObjectNameLess());

    for (Surface surface : surfaces){

      auto it = mergedSurfaces.find(surface);
      if (it != mergedSurfaces.end()){
        continue;
      }

      boost::optional<Surface> adjacentSurface = surface.adjacentSurface();
      if (adjacentSurface){
        boost::optional<Space> adjacentSpace = adjacentSurface->space();
        if (adjacentSpace && (newSpace.handle() == adjacentSpace->handle())){

          // handling both the surface and the adjacentSurface
          mergedSurfaces.insert(surface);
          mergedSurfaces.insert(*adjacentSurface);

          // don't make interior partitions for interior air walls
          bool isAirWall = surface.isAirWall();
          bool isAdjacentAirWall = adjacentSurface->isAirWall();

          if (isAirWall && isAdjacentAirWall){
            continue;
          } else if (isAirWall){
            LOG(Warn, "Interior surface '" << surface.name() << "' is an air wall but adjacent surface '" << adjacentSurface->name() << "' is not, ignoring internal mass.")
              continue;
          } else if (isAdjacentAirWall){
            LOG(Warn, "Interior surface '" << adjacentSurface->name() << "' is an air wall but adjacent surface '" << surface.name() << "' is not, ignoring internal mass.")
            continue;
          }

          if (!interiorPartitionSurfaceGroup){
            interiorPartitionSurfaceGroup = InteriorPartitionSurfaceGroup(model);
            interiorPartitionSurfaceGroup->setSpace(newSpace);
          }

          // DLM: is there a better way to pick which vertices to keep based on outward normal?
          InteriorPartitionSurface interiorPartitionSurface(surface.vertices(), model);
          interiorPartitionSurface.setName("Merged " + surface.name().get() + " - " + adjacentSurface->name().get());
          interiorPartitionSurface.setInteriorPartitionSurfaceGroup(*interiorPartitionSurfaceGroup);

          boost::optional<ConstructionBase> construction = surface.construction();
          if (construction){
            interiorPartitionSurface.setConstruction(*construction);
          }
        }
      }
    }

    for (Surface mergedSurface : mergedSurfaces){
      mergedSurface.remove();
    }

    // if there is a common designSpecificationOutdoorAir
    if (designSpecificationOutdoorAir){

      // allow new space to inherit from space type
      bool useSpaceTypeOutdoorAir = false;
      if (allDesignSpecificationOutdoorAirDefaulted){
        if (spaceType){
          if (spaceType->designSpecificationOutdoorAir()){
            if (designSpecificationOutdoorAir->handle() == spaceType->designSpecificationOutdoorAir()->handle()){
              useSpaceTypeOutdoorAir = true;
            }
          }
        }
      }

      // set common designSpecificationOutdoorAir for the new space
      if (!useSpaceTypeOutdoorAir){
        newSpace.setDesignSpecificationOutdoorAir(*designSpecificationOutdoorAir);
      }

    }else{

      double outdoorAirForPeople = 0.0;
      if (sumOutdoorAirForPeople > 0 && sumNumberOfPeople > 0){
        outdoorAirForPeople = sumOutdoorAirForPeople / sumNumberOfPeople;
      }

      double outdoorAirForFloorArea = 0.0;
      if (sumOutdoorAirForFloorArea > 0 && sumFloorArea > 0){
        outdoorAirForFloorArea = sumOutdoorAirForFloorArea / sumFloorArea;
      }

      double outdoorAirForVolume = 0.0;
      if (sumOutdoorAirForVolume > 0 && sumVolume > 0){
        outdoorAirForVolume = sumOutdoorAirForVolume / sumVolume;
      }

      // make a new designSpecificationOutdoorAir
      designSpecificationOutdoorAir = DesignSpecificationOutdoorAir(model);
      if( anySumOutdoorAirMethod && anyMaxOutdoorAirMethod ) {
        designSpecificationOutdoorAir->setOutdoorAirMethod("Sum");
      }else if( anyMaxOutdoorAirMethod ) {
        designSpecificationOutdoorAir->setOutdoorAirMethod("Maximum");
      }else{
        designSpecificationOutdoorAir->setOutdoorAirMethod("Sum");
      }
      designSpecificationOutdoorAir->setOutdoorAirFlowperPerson(outdoorAirForPeople);
      designSpecificationOutdoorAir->setOutdoorAirFlowperFloorArea(outdoorAirForFloorArea);
      designSpecificationOutdoorAir->setOutdoorAirFlowRate(sumOutdoorAirRate);
      designSpecificationOutdoorAir->setOutdoorAirFlowAirChangesperHour(outdoorAirForVolume);

      if (anyDesignSpecificationOutdoorAirSchedules){
        LOG(Warn, "DesignSpecificationOutdoorAir objects merged for ThermalZone '" << this->name() << "', could not preserve outdoor air flow rate fraction schedules");
      }

      newSpace.setDesignSpecificationOutdoorAir(*designSpecificationOutdoorAir);
    }

    return newSpace;
  }

  std::vector<IdfObject> ThermalZone_Impl::remove()
  {
    auto m = model();

    auto thermalZone = getObject<ThermalZone>();
    for ( auto & airloop : airLoopHVACs() ) {
      airloop.removeBranchForZone(thermalZone);
    }

    auto comps = this->equipment();
    for ( auto & comp : comps ) {
      comp.remove();
    }

    //detach it from the zone air node
    auto airNode = zoneAirNode();
    airNode.disconnect();
    airNode.remove();

    // remove port lists
    returnPortList().remove();
    inletPortList().remove();
    exhaustPortList().remove();

    // remove ZoneHVACEquipmentList
    zoneHVACEquipmentList().remove();

    // remove ZoneMixing objects
    for (auto mixing : this->zoneMixing()) {
      mixing.remove();
    }

    return HVACComponent_Impl::remove();
  }

  void ThermalZone_Impl::disconnect()
  {
    ModelObject mo = this->getObject<ModelObject>();
    Model _model = this->model();

    auto pl = inletPortList();
    auto plPorts = pl.airLoopHVACPorts();
    for ( const auto & port : plPorts ) {
      _model.disconnect(pl, port);
    }

    pl = returnPortList();
    plPorts = pl.airLoopHVACPorts();
    for ( const auto & port : plPorts ) {
      _model.disconnect(pl, port);
    }
  }

  bool ThermalZone_Impl::useIdealAirLoads() const
  {
    boost::optional<std::string> value = getString(OS_ThermalZoneFields::UseIdealAirLoads);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ThermalZone_Impl::setUseIdealAirLoads(bool useIdealAirLoads)
  {
    bool result = true;

    if (useIdealAirLoads)
    {
      result &= setString(OS_ThermalZoneFields::UseIdealAirLoads, "Yes");

      std::vector<ModelObject> comps = this->equipment();

      for( auto & comp : comps )
      {
        comp.remove();
      }

      if( boost::optional<AirLoopHVAC> airLoop = this->airLoopHVAC() )
      {
        ThermalZone thisObject = this->getObject<ThermalZone>();

        result &= airLoop->removeBranchForZone(thisObject);
      }

    }
    else
    {
      result &= setString(OS_ThermalZoneFields::UseIdealAirLoads, "No");
    }
    return result;
  }

  openstudio::OSOptionalQuantity ThermalZone_Impl::ceilingHeight_SI() const {
    return getCeilingHeight(false);
  }

  openstudio::OSOptionalQuantity ThermalZone_Impl::ceilingHeight_IP() const {
    return getCeilingHeight(true);
  }

  openstudio::OSOptionalQuantity ThermalZone_Impl::volume_SI() const {
    return getVolume(false);
  }

  openstudio::OSOptionalQuantity ThermalZone_Impl::volume_IP() const {
    return getVolume(true);
  }

  openstudio::Quantity ThermalZone_Impl::fractionofZoneControlledbyPrimaryDaylightingControl_SI() const {
    return getFractionofZoneControlledbyPrimaryDaylightingControl(false);
  }

  openstudio::Quantity ThermalZone_Impl::fractionofZoneControlledbyPrimaryDaylightingControl_IP() const {
    return getFractionofZoneControlledbyPrimaryDaylightingControl(true);
  }

  openstudio::Quantity ThermalZone_Impl::fractionofZoneControlledbySecondaryDaylightingControl_SI() const {
    return getFractionofZoneControlledbySecondaryDaylightingControl(false);
  }

  openstudio::Quantity ThermalZone_Impl::fractionofZoneControlledbySecondaryDaylightingControl_IP() const {
    return getFractionofZoneControlledbySecondaryDaylightingControl(true);
  }


  boost::optional<ModelObject> ThermalZone_Impl::thermostatSetpointDualSetpointAsModelObject() const {
    OptionalModelObject result;
    OptionalThermostatSetpointDualSetpoint intermediate = thermostatSetpointDualSetpoint();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ThermalZone_Impl::zoneControlHumidistatAsModelObject() const {
    OptionalModelObject result;
    OptionalZoneControlHumidistat intermediate = zoneControlHumidistat();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ThermalZone_Impl::primaryDaylightingControlAsModelObject() const {
    OptionalModelObject result;
    OptionalDaylightingControl intermediate = primaryDaylightingControl();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ThermalZone_Impl::secondaryDaylightingControlAsModelObject() const {
    OptionalModelObject result;
    OptionalDaylightingControl intermediate = secondaryDaylightingControl();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ThermalZone_Impl::illuminanceMapAsModelObject() const {
    OptionalModelObject result;
    OptionalIlluminanceMap intermediate = illuminanceMap();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ThermalZone_Impl::renderingColorAsModelObject() const {
    OptionalModelObject result;
    OptionalRenderingColor intermediate = renderingColor();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> ThermalZone_Impl::equipmentAsModelObjects() {
    ModelObjectVector result = castVector<ModelObject>(equipment());
    return result;
  }

  std::vector<ModelObject> ThermalZone_Impl::spacesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(spaces());
    return result;
  }

  bool ThermalZone_Impl::setThermostatSetpointDualSetpointAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermostatSetpointDualSetpoint intermediate = modelObject->optionalCast<ThermostatSetpointDualSetpoint>();
      if (intermediate) {
        return setThermostatSetpointDualSetpoint(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetThermostatSetpointDualSetpoint();
    }
    return true;
  }

  bool ThermalZone_Impl::setZoneControlHumidistatAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalZoneControlHumidistat intermediate = modelObject->optionalCast<ZoneControlHumidistat>();
      if (intermediate) {
        return setZoneControlHumidistat(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetZoneControlHumidistat();
    }
    return true;
  }

  bool ThermalZone_Impl::setPrimaryDaylightingControlAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDaylightingControl intermediate = modelObject->optionalCast<DaylightingControl>();
      if (intermediate) {
        return setPrimaryDaylightingControl(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetPrimaryDaylightingControl();
    }
    return true;
  }

  bool ThermalZone_Impl::setSecondaryDaylightingControlAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDaylightingControl intermediate = modelObject->optionalCast<DaylightingControl>();
      if (intermediate) {
        return setSecondaryDaylightingControl(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSecondaryDaylightingControl();
    }
    return true;
  }

  bool ThermalZone_Impl::setIlluminanceMapAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalIlluminanceMap intermediate = modelObject->optionalCast<IlluminanceMap>();
      if (intermediate) {
        return setIlluminanceMap(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetIlluminanceMap();
    }
    return true;
  }

  bool ThermalZone_Impl::setRenderingColorAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalRenderingColor intermediate = modelObject->optionalCast<RenderingColor>();
      if (intermediate) {
        return setRenderingColor(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetRenderingColor();
    }
    return true;
  }

  SizingZone ThermalZone_Impl::sizingZone() const
  {
    boost::optional<SizingZone> sizingZone;

    std::vector<SizingZone> sizingObjects;

    //sizingObjects = model().getConcreteModelObjects<SizingZone>();

    sizingObjects = getObject<ModelObject>().getModelObjectSources<SizingZone>(SizingZone::iddObjectType());

    for( const auto & sizingObject : sizingObjects )
    {
      if( sizingObject.thermalZone().handle() == this->handle() )
      {
        sizingZone = sizingObject;
      }
    }

    if( sizingZone )
    {
      return sizingZone.get();
    }
    else
    {
      LOG_AND_THROW("ThermalZone missing Sizing:Zone object");
    }
  }

  bool ThermalZone_Impl::addToNodeImpl(Node & node)
  {
    Model _model = model();

    ThermalZone thisObject = getObject<ThermalZone>();

    if( node.model() != _model ) {
      return false;
    }

    if( isPlenum() ) {
      return false;
    }

    auto airLoop = node.airLoopHVAC();

    if( airLoop ) {
      auto loops = airLoopHVACs();
      for ( const auto & loop : loops ) {
        if( loop.handle() == airLoop->handle() ) {
          return false;
        }
      }

      auto inletObj = node.inletModelObject();
      auto outletObj = node.outletModelObject();

      if( inletObj && outletObj ) {
        if( (! inletObj->optionalCast<ThermalZone>()) && outletObj->optionalCast<Mixer>() ) {
          Node newNode(_model);
          auto thisobj = getObject<ThermalZone>();
          auto _inletPortList = inletPortList();
          auto _returnPortList = returnPortList();

          unsigned oldMixerPort  = node.connectedObjectPort( node.outletPort() ).get();

          _model.connect( node, node.outletPort(),
                          _inletPortList, _inletPortList.nextPort() );

          _model.connect( _returnPortList, _returnPortList.nextPort(),
                          newNode, newNode.inletPort() );

          _model.connect( newNode, newNode.outletPort(),
                          outletObj.get(), oldMixerPort );

          // Add the terminal to equipment list
          if( (! inletObj->optionalCast<Splitter>()) && (! inletObj->optionalCast<Node>()) )
          {
            addEquipment(inletObj.get());

            if( boost::optional<AirTerminalSingleDuctParallelPIUReheat> terminal = inletObj->optionalCast<AirTerminalSingleDuctParallelPIUReheat>() )
            {
              Node secondaryInletNode(_model);

              PortList t_exhaustPortList = exhaustPortList();

              _model.connect( t_exhaustPortList,
                              t_exhaustPortList.nextPort(),
                              secondaryInletNode,
                              secondaryInletNode.inletPort() );

              _model.connect( secondaryInletNode,
                              secondaryInletNode.outletPort(),
                              terminal.get(),
                              terminal->secondaryAirInletPort() );
            }
          }

          setUseIdealAirLoads(false);

          // If there is a single zone reheat spm, see if the control zone is set.
          // If not set, then set it to this zone.

          std::vector<ModelObject> supplyNodes = airLoop->supplyComponents(Node::iddObjectType());

          for( const auto & supplyNode : supplyNodes )
          {
            {
              std::vector<SetpointManagerSingleZoneReheat> setpointManagers = subsetCastVector<SetpointManagerSingleZoneReheat>(supplyNode.cast<Node>().setpointManagers());
              if( ! setpointManagers.empty() ) {
                SetpointManagerSingleZoneReheat spm = setpointManagers.front();
                if( ! spm.controlZone() )
                {
                  spm.setControlZone(thisobj);
                }
              }
            }

            {
              std::vector<SetpointManagerSingleZoneCooling> setpointManagers = subsetCastVector<SetpointManagerSingleZoneCooling>(supplyNode.cast<Node>().setpointManagers());
              if( ! setpointManagers.empty() ) {
                SetpointManagerSingleZoneCooling spm = setpointManagers.front();
                if( ! spm.controlZone() )
                {
                  spm.setControlZone(thisobj);
                }
              }
            }

            {
              std::vector<SetpointManagerSingleZoneHeating> setpointManagers = subsetCastVector<SetpointManagerSingleZoneHeating>(supplyNode.cast<Node>().setpointManagers());
              if( ! setpointManagers.empty() ) {
                SetpointManagerSingleZoneHeating spm = setpointManagers.front();
                if( ! spm.controlZone() )
                {
                  spm.setControlZone(thisobj);
                }
              }
            }
          }

          return true;
        }
      }
    }

    return false;
  }

  bool ThermalZone_Impl::addToNode(Node & node)
  {
    auto loops = airLoopHVACs();
    auto result = addToNodeImpl(node);
    auto zone = getObject<model::ThermalZone>();

    if ( result ) {
      for ( auto & loop : loops ) {
        loop.removeBranchForZone(zone);
      }
    }

    return result;
  }

  bool ThermalZone_Impl::multiAddToNode(Node & node)
  {
    return addToNodeImpl(node);
  }

  PortList ThermalZone_Impl::inletPortList() const
  {
    boost::optional<PortList> pl = getObject<ModelObject>().getModelObjectTarget<PortList>(OS_ThermalZoneFields::ZoneAirInletPortList);

    OS_ASSERT(pl);

    return pl.get();
  }

  PortList ThermalZone_Impl::exhaustPortList() const
  {
    boost::optional<PortList> pl = getObject<ModelObject>().getModelObjectTarget<PortList>(OS_ThermalZoneFields::ZoneAirExhaustPortList);

    OS_ASSERT(pl);

    return pl.get();
  }

  ZoneHVACEquipmentList ThermalZone_Impl::zoneHVACEquipmentList() const
  {
    boost::optional<ZoneHVACEquipmentList> result;

    std::vector<ZoneHVACEquipmentList> list = getObject<ModelObject>().getModelObjectSources<ZoneHVACEquipmentList>(ZoneHVACEquipmentList::iddObjectType());

    //std::vector<ZoneHVACEquipmentList> list = model().getConcreteModelObjects<ZoneHVACEquipmentList>();

    for( const auto & elem : list )
    {
      if( elem.thermalZone().handle() == handle() )
      {
        result = elem;
      }
    }

    if (result)
    {
      return result.get();
    } else
    {
      LOG_AND_THROW("ThermalZone missing ZoneHVAC:EquipmentList object");
    }
  }

  bool ThermalZone_Impl::addEquipment(const ModelObject & equipment)
  {
    return zoneHVACEquipmentList().addEquipment(equipment);
  }

  bool ThermalZone_Impl::removeEquipment(const ModelObject & equipment)
  {
    return zoneHVACEquipmentList().removeEquipment(equipment);
  }

  bool ThermalZone_Impl::setCoolingPriority(const ModelObject & equipment, unsigned priority)
  {
    return zoneHVACEquipmentList().setCoolingPriority(equipment,priority);
  }

  bool ThermalZone_Impl::setHeatingPriority(const ModelObject & equipment, unsigned priority)
  {
    return zoneHVACEquipmentList().setHeatingPriority(equipment,priority);
  }

  std::vector<ModelObject> ThermalZone_Impl::equipment() const
  {
    return zoneHVACEquipmentList().equipment();
  }

  std::string ThermalZone_Impl::loadDistributionScheme()
  {
    return zoneHVACEquipmentList().loadDistributionScheme();
  }

  bool ThermalZone_Impl::setLoadDistributionScheme(std::string scheme)
  {
    return zoneHVACEquipmentList().setLoadDistributionScheme(scheme);
  }

  std::vector<ModelObject> ThermalZone_Impl::equipmentInHeatingOrder()
  {
    return zoneHVACEquipmentList().equipmentInHeatingOrder();
  }

  std::vector<ModelObject> ThermalZone_Impl::equipmentInCoolingOrder()
  {
    return zoneHVACEquipmentList().equipmentInCoolingOrder();
  }

  ModelObject ThermalZone_Impl::clone(Model model) const
  {
    ThermalZone tz = HVACComponent_Impl::clone(model).cast<ThermalZone>();
    // We need this because "connect" is first going to try to disconnect from anything
    // currently attached.  At this point tz is left pointing (through a connection) to the old zone air node,
    // (because of ModelObject::clone behavior) so connecting to the new node will remove the connection joining
    // the original zone and the original node.
    tz.setString(OS_ThermalZoneFields::ZoneAirNodeName,"");

    tz.setString(OS_ThermalZoneFields::ThermostatName,"");
    tz.setString(OS_ThermalZoneFields::HumidistatName,"");

    Node node(model);
    model.connect(tz,tz.zoneAirPort(),node,node.inletPort());

    PortList inletPortList(tz);
    tz.setPointer(OS_ThermalZoneFields::ZoneAirInletPortList,inletPortList.handle());

    PortList exhaustPortList(tz);
    tz.setPointer(OS_ThermalZoneFields::ZoneAirExhaustPortList,exhaustPortList.handle());

    PortList outletPortList(tz);
    tz.setPointer(OS_ThermalZoneFields::ZoneReturnAirPortList,outletPortList.handle());

    auto sizingZoneClone = sizingZone().clone(model).cast<SizingZone>();
    sizingZoneClone.getImpl<detail::SizingZone_Impl>()->setThermalZone(tz);

    ZoneHVACEquipmentList equipmentList(tz);

    if( auto t_thermostat = thermostat() ) {
      auto thermostatClone = t_thermostat->clone(model).cast<Thermostat>();
      tz.setThermostat(thermostatClone);
    }

    if( auto t_humidistat = zoneControlHumidistat() ) {
      auto humidistatClone = t_humidistat->clone(model).cast<ZoneControlHumidistat>();
      tz.setZoneControlHumidistat(humidistatClone);
    }

    if( auto t_controller = zoneControlContaminantController() ) {
      auto controllerClone = t_controller->clone(model).cast<ZoneControlContaminantController>();
      tz.setZoneControlContaminantController(controllerClone);
    }

    // DLM: do not clone zone mixing objects

    if (auto t_afnzone = airflowNetworkZone()) {
      auto afnzoneClone = t_afnzone->clone(model).cast<AirflowNetworkZone>();
      afnzoneClone.setThermalZone(tz);
    }

    return tz;
  }

  boost::optional<AirLoopHVACSupplyPlenum> ThermalZone_Impl::airLoopHVACSupplyPlenum() const
  {
    boost::optional<AirLoopHVACSupplyPlenum> result;

    std::vector<AirLoopHVACSupplyPlenum> plenums = model().getConcreteModelObjects<AirLoopHVACSupplyPlenum>();

    for(const auto & plenum : plenums)
    {
      if( boost::optional<ThermalZone> tz = plenum.thermalZone() )
      {
        if( tz->handle() == handle() )
        {
          return plenum;
        }
      }
    }

    return result;
  }

  boost::optional<AirLoopHVACReturnPlenum> ThermalZone_Impl::airLoopHVACReturnPlenum() const
  {
    boost::optional<AirLoopHVACReturnPlenum> result;

    std::vector<AirLoopHVACReturnPlenum> plenums = model().getConcreteModelObjects<AirLoopHVACReturnPlenum>();

    for(const auto & plenum : plenums)
    {
      if( boost::optional<ThermalZone> tz = plenum.thermalZone() )
      {
        if( tz->handle() == handle() )
        {
          return plenum;
        }
      }
    }

    return result;
  }

  bool ThermalZone_Impl::isPlenum() const
  {
    bool result = false;
    if( airLoopHVACReturnPlenum() || airLoopHVACSupplyPlenum() )
    {
      result = true;
    }
    return result;
  }

  bool ThermalZone_Impl::canBePlenum() const
  {
    bool result = true;

    if( airLoopHVAC() || (! equipment().empty()) ) {
      result = false;
    }

    return result;
  }

  bool ThermalZone_Impl::setSupplyPlenum(const ThermalZone & plenumZone, unsigned branchIndex)
  {
    bool result = true;

    if( ! plenumZone.canBePlenum() )
    {
      result = false;
    }

    boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();
    if( ! t_airLoopHVAC )
    {
      result = false;
    }

    boost::optional<AirLoopHVACSupplyPlenum> plenum;

    if( result )
    {
      plenum = plenumZone.getImpl<ThermalZone_Impl>()->airLoopHVACSupplyPlenum();
      boost::optional<AirLoopHVAC> plenumAirLoop;

      if( plenum )
      {
        plenumAirLoop = plenum->airLoopHVAC();
      }

      if( plenumAirLoop )
      {
        if( plenumAirLoop.get() != t_airLoopHVAC.get() )
        {
          result = false;
        }
      }
    }

    Model t_model = model();

    if( result )
    {
      if( ! plenum )
      {
        plenum = AirLoopHVACSupplyPlenum(t_model);
        plenum->setThermalZone(plenumZone);
      }
    }

    boost::optional<AirLoopHVACZoneSplitter> zoneSplitter;

    if( result ) {
      auto zoneSplitters = t_airLoopHVAC->zoneSplitters();
      if( branchIndex < zoneSplitters.size() ) {
        zoneSplitter = zoneSplitters[branchIndex];
      }
    }

    if( ! zoneSplitter ) result = false;

    if( result )
    {
      removeSupplyPlenum();

      model::ModelObject mo = t_airLoopHVAC->demandComponents(zoneSplitter.get(),getObject<ThermalZone>(),Node::iddObjectType()).front();
      Node node = mo.cast<Node>();

      OS_ASSERT(plenum);
      result = plenum->addToNode(node);
    }

    return result;
  }

  bool ThermalZone_Impl::setSupplyPlenum(const ThermalZone & plenumZone)
  {
    return setSupplyPlenum(plenumZone,0u);
  }

  void ThermalZone_Impl::removeSupplyPlenum(const AirLoopHVAC & airloop, unsigned branchIndex)
  {
    Model m = model();

    auto zoneSplitters = airloop.zoneSplitters();
    if( branchIndex < zoneSplitters.size() ) {
      auto zoneSplitter = zoneSplitters[branchIndex];

      auto modelObjects = airloop.demandComponents(zoneSplitter,getObject<ThermalZone>());
      auto plenums = subsetCastVector<AirLoopHVACSupplyPlenum>(modelObjects);
      boost::optional<AirLoopHVACSupplyPlenum> plenum;

      if( ! plenums.empty() ) {
        auto plenum = plenums.front();

        if ( plenum.outletModelObjects().size() == 1u ) {
          plenum.remove();
        } else {
          auto it = std::find(modelObjects.begin(),modelObjects.end(),plenum);
          ModelObject plenumOutletModelObject = *(it + 1);
          unsigned branchIndex = plenum.branchIndexForOutletModelObject(plenumOutletModelObject);
          unsigned port = plenum.connectedObjectPort(plenum.outletPort(branchIndex)).get();
          plenum.removePortForBranch(branchIndex);
          m.connect(zoneSplitter,zoneSplitter.nextOutletPort(),plenumOutletModelObject,port);
        }
      }
    }
  }

  void ThermalZone_Impl::removeSupplyPlenum(unsigned branchIndex)
  {
    auto airloop = airLoopHVAC();
    if ( airloop ) {
      removeSupplyPlenum(airloop.get(), branchIndex);
    }
  }

  void ThermalZone_Impl::removeSupplyPlenum()
  {
    removeSupplyPlenum(0u);
  }

  void ThermalZone_Impl::removeSupplyPlenum(const AirLoopHVAC & airloop) {
    removeSupplyPlenum(airloop, 0u);
  }

  bool ThermalZone_Impl::setReturnPlenum(const ThermalZone & plenumZone, AirLoopHVAC & airLoop)
  {
    bool result = true;

    if( ! plenumZone.canBePlenum() ) {
      result = false;
    }

    boost::optional<AirLoopHVACReturnPlenum> plenum;

    if( result ) {
      plenum = plenumZone.getImpl<ThermalZone_Impl>()->airLoopHVACReturnPlenum();
      boost::optional<AirLoopHVAC> plenumAirLoop;

      if( plenum ) {
        plenumAirLoop = plenum->airLoopHVAC();
      }

      if( plenumAirLoop ) {
        if( plenumAirLoop.get() != airLoop ) {
          result = false;
        }
      }
    }

    Model t_model = model();

    if( result ) {
      if( ! plenum ) {
        plenum = AirLoopHVACReturnPlenum(t_model);
        plenum->setThermalZone(plenumZone);
      }
    }

    if( result ) {
      removeReturnPlenum(airLoop);

      auto node = airLoop.demandComponents(getObject<ThermalZone>(),airLoop.zoneMixer(),Node::iddObjectType()).front().cast<Node>();

      OS_ASSERT(plenum);
      result = plenum->addToNode(node);
    }

    return result;
  }

  bool ThermalZone_Impl::setReturnPlenum(const ThermalZone & plenumZone)
  {
    auto loop = airLoopHVAC();

    if ( loop ) {
      return setReturnPlenum(plenumZone, loop.get());
    } else {
      return false;
    }
  }


  void ThermalZone_Impl::removeReturnPlenum(AirLoopHVAC & airLoop)
  {
    Model t_model = model();

    auto zoneMixer = airLoop.zoneMixer();
    auto modelObjects = airLoop.demandComponents(getObject<ThermalZone>(),zoneMixer);
    auto plenums = subsetCastVector<AirLoopHVACReturnPlenum>(modelObjects);
    boost::optional<AirLoopHVACReturnPlenum> plenum;

    if( ! plenums.empty() ) {
      plenum = plenums.front();
    }

    if( plenum ) {
      if( plenum->inletModelObjects().size() == 1u ) {
        plenum->remove();
      } else {
        auto it = std::find(modelObjects.begin(),modelObjects.end(),plenum.get());
        ModelObject plenumInletModelObject = *(it - 1);
        unsigned branchIndex = plenum->branchIndexForInletModelObject(plenumInletModelObject);
        unsigned port = plenum->connectedObjectPort(plenum->inletPort(branchIndex)).get();
        plenum->removePortForBranch(branchIndex);
        t_model.connect(plenumInletModelObject,port,zoneMixer,zoneMixer.nextInletPort());
      }
    }
  }

  void ThermalZone_Impl::removeReturnPlenum()
  {
    auto loop = airLoopHVAC();
    if ( loop ) {
      removeReturnPlenum(loop.get());
    }
  }

  std::vector<ZoneMixing> ThermalZone_Impl::zoneMixing() const
  {
    return getObject<ModelObject>().getModelObjectSources<ZoneMixing>();
  }

  std::vector<ZoneMixing> ThermalZone_Impl::supplyZoneMixing() const
  {
    std::vector<ZoneMixing> result = this->zoneMixing();

    Handle handle = this->handle();
    auto new_end = std::remove_if(result.begin(), result.end(),
                                  [&](const ZoneMixing& mixing){ return (mixing.zone().handle() != handle); });

    result.erase(new_end, result.end());
    return result;
  }

  std::vector<ZoneMixing> ThermalZone_Impl::exhaustZoneMixing() const
  {
    std::vector<ZoneMixing> result = this->zoneMixing();

    Handle handle = this->handle();
    auto new_end = std::remove_if(result.begin(), result.end(),
                                  [&](const ZoneMixing& mixing){ return (!mixing.sourceZone() || (mixing.sourceZone()->handle() != handle)); });

    result.erase(new_end, result.end());
    return result;
  }

  std::vector<AirLoopHVAC> ThermalZone_Impl::airLoopHVACs() const
  {
    std::vector<AirLoopHVAC> result;

    auto pl = inletPortList();
    auto nodes = subsetCastVector<model::HVACComponent>(pl.airLoopHVACModelObjects());
    for ( auto & node : nodes ) {
      auto loop = node.airLoopHVAC();
      if ( loop ) {
        result.push_back(loop.get());
      }
    }

    return result;
  }

  boost::optional<HVACComponent> ThermalZone_Impl::airLoopHVACTerminal() const
  {
    if( auto mo = inletPortList().airLoopHVACModelObject() ) {
      if( auto node = mo->optionalCast<Node>() ) {
        if( auto nodeInlet = node->inletModelObject() ) {
          if( ! nodeInlet->optionalCast<Splitter>() ) {
            return nodeInlet->optionalCast<HVACComponent>();
          }
        }
      }
    }

    return boost::none;
  }

  std::vector<HVACComponent> ThermalZone_Impl::airLoopHVACTerminals() const
  {
    std::vector<HVACComponent> result;

    auto mos = inletPortList().airLoopHVACModelObjects();
    for ( const auto & mo : mos ) {
      if ( auto node = mo.optionalCast<Node>() ) {
        if ( auto nodeInlet = node->inletModelObject() ) {
          if ( ! nodeInlet->optionalCast<Splitter>() ) {
            auto hvaccomp = nodeInlet->optionalCast<HVACComponent>();
            if ( hvaccomp ) {
              result.push_back(hvaccomp.get());
            }
          }
        }
      }
    }

    return result;
  }

  boost::optional<ZoneControlContaminantController> ThermalZone_Impl::zoneControlContaminantController() const
  {
    auto h = handle();

    auto controllers = model().getConcreteModelObjects<ZoneControlContaminantController>();
    for( const auto & controller : controllers ) {
      if( auto zone = controller.getImpl<detail::ZoneControlContaminantController_Impl>()->controlledZone() ) {
        if( zone->handle() == h ) {
          return controller;
        }
      }
    }

    return boost::none;
  }

  bool ThermalZone_Impl::setZoneControlContaminantController(const ZoneControlContaminantController & contaminantController)
  {
    auto t_model = model();

    if( t_model != contaminantController.model() ) {
      return false;
    }

    if( auto currentZone = contaminantController.controlledZone() ) {
      if( currentZone->handle() == handle() ) {
        return true;
      } else {
        auto controllerClone = contaminantController.clone(t_model).cast<ZoneControlContaminantController>();
        auto tz = getObject<ThermalZone>();
        return controllerClone.getImpl<detail::ZoneControlContaminantController_Impl>()->setControlledZone(tz);
      }
    } else {
      resetZoneControlContaminantController();
      auto tz = getObject<ThermalZone>();
      return contaminantController.getImpl<detail::ZoneControlContaminantController_Impl>()->setControlledZone(tz);
    }
  }

  void ThermalZone_Impl::resetZoneControlContaminantController()
  {
    if( auto controller = zoneControlContaminantController() ) {
      controller->remove();
    }
  }

  std::vector<EMSActuatorNames> ThermalZone_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{{"Zone Temperature Control", "Heating Setpoint"},
                                            {"Zone Temperature Control", "Cooling Setpoint"},
                                            {"Zone Humidity Control", "Relative Humidity Humidifying Setpoint"},
                                            {"Zone Humidity Control", "Relative Humidity Dehumidifying Setpoint"},
                                            {"Zone Comfort Control", "Heating Setpoint"},
                                            {"Zone Comfort Control", "Cooling Setpoint"},
                                            {"Zone", "Outdoor Air Drybulb Temperature"},
                                            {"Zone", "Outdoor Air Wetbulb Temperature"},
                                            {"Zone", "Outdoor Air Wind Speed"},
                                            {"Zone", "Outdoor Air Wind Direction"}};
    return actuators;
  }

  std::vector<std::string> ThermalZone_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types{"Zone Floor Area",
                                   "Zone Air Volume",
                                   "Zone Multiplier",
                                   "Zone List Multiplier"};
    return types;
  }

  AirflowNetworkZone ThermalZone_Impl::getAirflowNetworkZone()
  {
    boost::optional<AirflowNetworkZone> opt = airflowNetworkZone();
    if (opt) {
      return opt.get();
    }
    return AirflowNetworkZone(model(), handle());
  }

  boost::optional<AirflowNetworkZone> ThermalZone_Impl::airflowNetworkZone() const
  {
    std::vector<AirflowNetworkZone> myAFNZones = getObject<ModelObject>().getModelObjectSources<AirflowNetworkZone>(AirflowNetworkZone::iddObjectType());
    auto count = myAFNZones.size();
    if (count == 1) {
      return myAFNZones[0];
    } else if (count > 1) {
      LOG(Warn, briefDescription() << " has more than one AirflowNetwork Zone attached, returning first.");
      return myAFNZones[0];
    }
    return boost::none;
  }

} // detail

ThermalZone::ThermalZone(const Model& model)
  : HVACComponent(ThermalZone::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalZone_Impl>());

  Node node(model);
  model.connect(*this,this->zoneAirPort(),node,node.inletPort());

  PortList inletPortList(*this);
  setPointer(OS_ThermalZoneFields::ZoneAirInletPortList,inletPortList.handle());

  PortList exhaustPortList(*this);
  setPointer(OS_ThermalZoneFields::ZoneAirExhaustPortList,exhaustPortList.handle());

  PortList returnPortList(*this);
  setPointer(OS_ThermalZoneFields::ZoneReturnAirPortList,returnPortList.handle());

  SizingZone sizingZone(model,*this);

  ZoneHVACEquipmentList equipmentList(*this);

  setUseIdealAirLoads(false);
}

IddObjectType ThermalZone::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ThermalZone);
  return result;
}

std::vector<std::string> ThermalZone::validZoneInsideConvectionAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalZoneFields::ZoneInsideConvectionAlgorithm);
}

std::vector<std::string> ThermalZone::validZoneOutsideConvectionAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalZoneFields::ZoneOutsideConvectionAlgorithm);
}

int ThermalZone::multiplier() const {
  return getImpl<detail::ThermalZone_Impl>()->multiplier();
}

bool ThermalZone::isMultiplierDefaulted() const {
  return getImpl<detail::ThermalZone_Impl>()->isMultiplierDefaulted();
}

boost::optional<double> ThermalZone::ceilingHeight() const {
  return getImpl<detail::ThermalZone_Impl>()->ceilingHeight();
}

OSOptionalQuantity ThermalZone::getCeilingHeight(bool returnIP) const {
  return getImpl<detail::ThermalZone_Impl>()->getCeilingHeight(returnIP);
}

bool ThermalZone::isCeilingHeightDefaulted() const {
  return getImpl<detail::ThermalZone_Impl>()->isCeilingHeightDefaulted();
}

bool ThermalZone::isCeilingHeightAutocalculated() const {
  return getImpl<detail::ThermalZone_Impl>()->isCeilingHeightAutocalculated();
}

boost::optional<double> ThermalZone::volume() const {
  return getImpl<detail::ThermalZone_Impl>()->volume();
}

OSOptionalQuantity ThermalZone::getVolume(bool returnIP) const {
  return getImpl<detail::ThermalZone_Impl>()->getVolume(returnIP);
}

bool ThermalZone::isVolumeDefaulted() const {
  return getImpl<detail::ThermalZone_Impl>()->isVolumeDefaulted();
}

bool ThermalZone::isVolumeAutocalculated() const {
  return getImpl<detail::ThermalZone_Impl>()->isVolumeAutocalculated();
}

boost::optional<std::string> ThermalZone::zoneInsideConvectionAlgorithm() const {
  return getImpl<detail::ThermalZone_Impl>()->zoneInsideConvectionAlgorithm();
}

boost::optional<std::string> ThermalZone::zoneOutsideConvectionAlgorithm() const {
  return getImpl<detail::ThermalZone_Impl>()->zoneOutsideConvectionAlgorithm();
}

std::string ThermalZone::zoneConditioningEquipmentListName() const {
  return getImpl<detail::ThermalZone_Impl>()->zoneConditioningEquipmentListName();
}

double ThermalZone::fractionofZoneControlledbyPrimaryDaylightingControl() const {
  return getImpl<detail::ThermalZone_Impl>()->fractionofZoneControlledbyPrimaryDaylightingControl();
}

Quantity ThermalZone::getFractionofZoneControlledbyPrimaryDaylightingControl(bool returnIP) const {
  return getImpl<detail::ThermalZone_Impl>()->getFractionofZoneControlledbyPrimaryDaylightingControl(returnIP);
}

bool ThermalZone::isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const {
  return getImpl<detail::ThermalZone_Impl>()->isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted();
}

double ThermalZone::fractionofZoneControlledbySecondaryDaylightingControl() const {
  return getImpl<detail::ThermalZone_Impl>()->fractionofZoneControlledbySecondaryDaylightingControl();
}

Quantity ThermalZone::getFractionofZoneControlledbySecondaryDaylightingControl(bool returnIP) const {
  return getImpl<detail::ThermalZone_Impl>()->getFractionofZoneControlledbySecondaryDaylightingControl(returnIP);
}

bool ThermalZone::isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const {
  return getImpl<detail::ThermalZone_Impl>()->isFractionofZoneControlledbySecondaryDaylightingControlDefaulted();
}

bool ThermalZone::setMultiplier(int multiplier) {
  return getImpl<detail::ThermalZone_Impl>()->setMultiplier(multiplier);
}

void ThermalZone::resetMultiplier() {
  getImpl<detail::ThermalZone_Impl>()->resetMultiplier();
}

bool ThermalZone::setCeilingHeight(boost::optional<double> ceilingHeight) {
  return getImpl<detail::ThermalZone_Impl>()->setCeilingHeight(ceilingHeight);
}

bool ThermalZone::setCeilingHeight(double ceilingHeight) {
  return getImpl<detail::ThermalZone_Impl>()->setCeilingHeight(ceilingHeight);
}

bool ThermalZone::setCeilingHeight(const Quantity& ceilingHeight) {
  return getImpl<detail::ThermalZone_Impl>()->setCeilingHeight(ceilingHeight);
}

void ThermalZone::resetCeilingHeight() {
  getImpl<detail::ThermalZone_Impl>()->resetCeilingHeight();
}

void ThermalZone::autocalculateCeilingHeight() {
  getImpl<detail::ThermalZone_Impl>()->autocalculateCeilingHeight();
}

bool ThermalZone::setVolume(boost::optional<double> volume) {
  return getImpl<detail::ThermalZone_Impl>()->setVolume(volume);
}

bool ThermalZone::setVolume(double volume) {
  return getImpl<detail::ThermalZone_Impl>()->setVolume(volume);
}

bool ThermalZone::setVolume(const Quantity& volume) {
  return getImpl<detail::ThermalZone_Impl>()->setVolume(volume);
}

void ThermalZone::resetVolume() {
  getImpl<detail::ThermalZone_Impl>()->resetVolume();
}

void ThermalZone::autocalculateVolume() {
  getImpl<detail::ThermalZone_Impl>()->autocalculateVolume();
}

bool ThermalZone::setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm) {
  return getImpl<detail::ThermalZone_Impl>()->setZoneInsideConvectionAlgorithm(zoneInsideConvectionAlgorithm);
}

bool ThermalZone::setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm) {
  return getImpl<detail::ThermalZone_Impl>()->setZoneInsideConvectionAlgorithm(zoneInsideConvectionAlgorithm);
}

void ThermalZone::resetZoneInsideConvectionAlgorithm() {
  getImpl<detail::ThermalZone_Impl>()->resetZoneInsideConvectionAlgorithm();
}

bool ThermalZone::setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm) {
  return getImpl<detail::ThermalZone_Impl>()->setZoneOutsideConvectionAlgorithm(zoneOutsideConvectionAlgorithm);
}

bool ThermalZone::setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm) {
  return getImpl<detail::ThermalZone_Impl>()->setZoneOutsideConvectionAlgorithm(zoneOutsideConvectionAlgorithm);
}

void ThermalZone::resetZoneOutsideConvectionAlgorithm() {
  getImpl<detail::ThermalZone_Impl>()->resetZoneOutsideConvectionAlgorithm();
}

bool ThermalZone::setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName) {
  return getImpl<detail::ThermalZone_Impl>()->setZoneConditioningEquipmentListName(zoneConditioningEquipmentListName);
}

bool ThermalZone::setFractionofZoneControlledbyPrimaryDaylightingControl(double fractionofZoneControlledbyPrimaryDaylightingControl) {
  return getImpl<detail::ThermalZone_Impl>()->setFractionofZoneControlledbyPrimaryDaylightingControl(fractionofZoneControlledbyPrimaryDaylightingControl);
}

bool ThermalZone::setFractionofZoneControlledbyPrimaryDaylightingControl(const Quantity& fractionofZoneControlledbyPrimaryDaylightingControl) {
  return getImpl<detail::ThermalZone_Impl>()->setFractionofZoneControlledbyPrimaryDaylightingControl(fractionofZoneControlledbyPrimaryDaylightingControl);
}

void ThermalZone::resetFractionofZoneControlledbyPrimaryDaylightingControl() {
  getImpl<detail::ThermalZone_Impl>()->resetFractionofZoneControlledbyPrimaryDaylightingControl();
}

bool ThermalZone::setFractionofZoneControlledbySecondaryDaylightingControl(double fractionofZoneControlledbySecondaryDaylightingControl) {
  return getImpl<detail::ThermalZone_Impl>()->setFractionofZoneControlledbySecondaryDaylightingControl(fractionofZoneControlledbySecondaryDaylightingControl);
}

bool ThermalZone::setFractionofZoneControlledbySecondaryDaylightingControl(const Quantity& fractionofZoneControlledbySecondaryDaylightingControl) {
  return getImpl<detail::ThermalZone_Impl>()->setFractionofZoneControlledbySecondaryDaylightingControl(fractionofZoneControlledbySecondaryDaylightingControl);
}

void ThermalZone::resetFractionofZoneControlledbySecondaryDaylightingControl() {
  getImpl<detail::ThermalZone_Impl>()->resetFractionofZoneControlledbySecondaryDaylightingControl();
}

unsigned ThermalZone::returnAirPort()
{
  return getImpl<detail::ThermalZone_Impl>()->returnAirPort();
}

unsigned ThermalZone::zoneAirPort()
{
  return getImpl<detail::ThermalZone_Impl>()->zoneAirPort();
}

OptionalModelObject ThermalZone::returnAirModelObject()
{
  return getImpl<detail::ThermalZone_Impl>()->returnAirModelObject();
}

std::vector<ModelObject> ThermalZone::returnAirModelObjects() const
{
  return getImpl<detail::ThermalZone_Impl>()->returnAirModelObjects();
}

Node ThermalZone::zoneAirNode()
{
  return getImpl<detail::ThermalZone_Impl>()->zoneAirNode();
}

boost::optional<DaylightingControl> ThermalZone::primaryDaylightingControl() const
{
  return getImpl<detail::ThermalZone_Impl>()->primaryDaylightingControl();
}

bool ThermalZone::setPrimaryDaylightingControl(const DaylightingControl& daylightingControl)
{
  return getImpl<detail::ThermalZone_Impl>()->setPrimaryDaylightingControl(daylightingControl);
}

void ThermalZone::resetPrimaryDaylightingControl()
{
  getImpl<detail::ThermalZone_Impl>()->resetPrimaryDaylightingControl();
}

boost::optional<DaylightingControl> ThermalZone::secondaryDaylightingControl() const
{
  return getImpl<detail::ThermalZone_Impl>()->secondaryDaylightingControl();
}

bool ThermalZone::setSecondaryDaylightingControl(const DaylightingControl& daylightingControl)
{
  return getImpl<detail::ThermalZone_Impl>()->setSecondaryDaylightingControl(daylightingControl);
}

void ThermalZone::resetSecondaryDaylightingControl()
{
  getImpl<detail::ThermalZone_Impl>()->resetSecondaryDaylightingControl();
}

boost::optional<IlluminanceMap> ThermalZone::illuminanceMap() const
{
  return getImpl<detail::ThermalZone_Impl>()->illuminanceMap();
}

bool ThermalZone::setIlluminanceMap(const IlluminanceMap& illuminanceMap)
{
  return getImpl<detail::ThermalZone_Impl>()->setIlluminanceMap(illuminanceMap);
}

void ThermalZone::resetIlluminanceMap()
{
  getImpl<detail::ThermalZone_Impl>()->resetIlluminanceMap();
}

void ThermalZone::checkDaylightingControlsAndIlluminanceMaps()
{
  getImpl<detail::ThermalZone_Impl>()->checkDaylightingControlsAndIlluminanceMaps();
}

boost::optional<RenderingColor> ThermalZone::renderingColor() const
{
  return getImpl<detail::ThermalZone_Impl>()->renderingColor();
}

bool ThermalZone::setRenderingColor(const RenderingColor& renderingColor)
{
  return getImpl<detail::ThermalZone_Impl>()->setRenderingColor(renderingColor);
}

void ThermalZone::resetRenderingColor()
{
  getImpl<detail::ThermalZone_Impl>()->resetRenderingColor();
}

std::vector<Space> ThermalZone::spaces() const {
  return getImpl<detail::ThermalZone_Impl>()->spaces();
}

double ThermalZone::floorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->floorArea();
}

double ThermalZone::exteriorSurfaceArea() const {
  return getImpl<detail::ThermalZone_Impl>()->exteriorSurfaceArea();
}

double ThermalZone::exteriorWallArea() const {
  return getImpl<detail::ThermalZone_Impl>()->exteriorWallArea();
}

double ThermalZone::airVolume() const {
  return getImpl<detail::ThermalZone_Impl>()->airVolume();
}

double ThermalZone::numberOfPeople() const {
  return getImpl<detail::ThermalZone_Impl>()->numberOfPeople();
}

double ThermalZone::peoplePerFloorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->peoplePerFloorArea();
}

double ThermalZone::floorAreaPerPerson() const {
  return getImpl<detail::ThermalZone_Impl>()->floorAreaPerPerson();
}

double ThermalZone::lightingPower() const {
  return getImpl<detail::ThermalZone_Impl>()->lightingPower();
}

double ThermalZone::lightingPowerPerFloorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->lightingPowerPerFloorArea();
}

double ThermalZone::lightingPowerPerPerson() const {
  return getImpl<detail::ThermalZone_Impl>()->lightingPowerPerPerson();
}

double ThermalZone::electricEquipmentPower() const {
  return getImpl<detail::ThermalZone_Impl>()->electricEquipmentPower();
}

double ThermalZone::electricEquipmentPowerPerFloorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->electricEquipmentPowerPerFloorArea();
}

double ThermalZone::electricEquipmentPowerPerPerson() const {
  return getImpl<detail::ThermalZone_Impl>()->electricEquipmentPowerPerPerson();
}

double ThermalZone::gasEquipmentPower() const {
  return getImpl<detail::ThermalZone_Impl>()->gasEquipmentPower();
}

double ThermalZone::gasEquipmentPowerPerFloorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->gasEquipmentPowerPerFloorArea();
}

double ThermalZone::gasEquipmentPowerPerPerson() const {
  return getImpl<detail::ThermalZone_Impl>()->gasEquipmentPowerPerPerson();
}

double ThermalZone::infiltrationDesignFlowRate() const {
  return getImpl<detail::ThermalZone_Impl>()->infiltrationDesignFlowRate();
}

double ThermalZone::infiltrationDesignFlowPerSpaceFloorArea() const {
  return getImpl<detail::ThermalZone_Impl>()->infiltrationDesignFlowPerSpaceFloorArea();
}

double ThermalZone::infiltrationDesignFlowPerExteriorSurfaceArea() const {
  return getImpl<detail::ThermalZone_Impl>()->infiltrationDesignFlowPerExteriorSurfaceArea();
}

double ThermalZone::infiltrationDesignFlowPerExteriorWallArea() const {
  return getImpl<detail::ThermalZone_Impl>()->infiltrationDesignFlowPerExteriorWallArea();
}

double ThermalZone::infiltrationDesignAirChangesPerHour() const {
  return getImpl<detail::ThermalZone_Impl>()->infiltrationDesignAirChangesPerHour();
}

boost::optional<std::string> ThermalZone::isConditioned() const {
  return getImpl<detail::ThermalZone_Impl>()->isConditioned();
}

boost::optional<ThermostatSetpointDualSetpoint> ThermalZone::thermostatSetpointDualSetpoint() const
{
  return getImpl<detail::ThermalZone_Impl>()->thermostatSetpointDualSetpoint();
}

bool ThermalZone::setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint & thermostat)
{
  return getImpl<detail::ThermalZone_Impl>()->setThermostatSetpointDualSetpoint(thermostat);
}

void ThermalZone::resetThermostatSetpointDualSetpoint()
{
  getImpl<detail::ThermalZone_Impl>()->resetThermostatSetpointDualSetpoint();
}

boost::optional<Thermostat> ThermalZone::thermostat() const
{
  return getImpl<detail::ThermalZone_Impl>()->thermostat();
}

bool ThermalZone::setThermostat(const Thermostat & thermostat)
{
  return getImpl<detail::ThermalZone_Impl>()->setThermostat(thermostat);
}

void ThermalZone::resetThermostat()
{
  getImpl<detail::ThermalZone_Impl>()->resetThermostat();
}

boost::optional<ZoneControlHumidistat> ThermalZone::zoneControlHumidistat() const
{
  return getImpl<detail::ThermalZone_Impl>()->zoneControlHumidistat();
}

bool ThermalZone::setZoneControlHumidistat(const ZoneControlHumidistat & humidistat)
{
  return getImpl<detail::ThermalZone_Impl>()->setZoneControlHumidistat(humidistat);
}

void ThermalZone::resetZoneControlHumidistat()
{
  getImpl<detail::ThermalZone_Impl>()->resetZoneControlHumidistat();
}

void ThermalZone::disconnect()
{
  getImpl<detail::ThermalZone_Impl>()->disconnect();
}

boost::optional<Space> ThermalZone::combineSpaces()
{
  return getImpl<detail::ThermalZone_Impl>()->combineSpaces();
}

bool ThermalZone::isRemovable() const
{
  return getImpl<detail::ThermalZone_Impl>()->isRemovable();
}

bool ThermalZone::useIdealAirLoads() const
{
  return getImpl<detail::ThermalZone_Impl>()->useIdealAirLoads();
}

bool ThermalZone::setUseIdealAirLoads(bool useIdealAirLoads)
{
  return getImpl<detail::ThermalZone_Impl>()->setUseIdealAirLoads(useIdealAirLoads);
}

SizingZone ThermalZone::sizingZone() const
{
  return getImpl<detail::ThermalZone_Impl>()->sizingZone();
}

bool ThermalZone::addToNode(Node & node)
{
  return getImpl<detail::ThermalZone_Impl>()->addToNode(node);
}

bool ThermalZone::multiAddToNode(Node & node)
{
  return getImpl<detail::ThermalZone_Impl>()->multiAddToNode(node);
}

PortList ThermalZone::returnPortList() const
{
  return getImpl<detail::ThermalZone_Impl>()->returnPortList();
}

PortList ThermalZone::inletPortList() const
{
  return getImpl<detail::ThermalZone_Impl>()->inletPortList();
}

PortList ThermalZone::exhaustPortList() const
{
  return getImpl<detail::ThermalZone_Impl>()->exhaustPortList();
}

bool ThermalZone::addEquipment(const ModelObject & equipment)
{
  return getImpl<detail::ThermalZone_Impl>()->addEquipment(equipment);
}

bool ThermalZone::setCoolingPriority(const ModelObject & equipment, unsigned priority)
{
  return getImpl<detail::ThermalZone_Impl>()->setCoolingPriority(equipment,priority);
}

bool ThermalZone::setHeatingPriority(const ModelObject & equipment, unsigned priority)
{
  return getImpl<detail::ThermalZone_Impl>()->setHeatingPriority(equipment,priority);
}

std::vector<ModelObject> ThermalZone::equipment() const
{
  return getImpl<detail::ThermalZone_Impl>()->equipment();
}

std::string ThermalZone::loadDistributionScheme()
{
  return getImpl<detail::ThermalZone_Impl>()->loadDistributionScheme();
}

bool ThermalZone::setLoadDistributionScheme(std::string scheme)
{
  return getImpl<detail::ThermalZone_Impl>()->setLoadDistributionScheme(scheme);
}

std::vector<ModelObject> ThermalZone::equipmentInHeatingOrder()
{
  return getImpl<detail::ThermalZone_Impl>()->equipmentInHeatingOrder();
}

std::vector<ModelObject> ThermalZone::equipmentInCoolingOrder()
{
  return getImpl<detail::ThermalZone_Impl>()->equipmentInCoolingOrder();
}

bool ThermalZone::removeEquipment(const ModelObject & equipment)
{
  return getImpl<detail::ThermalZone_Impl>()->removeEquipment(equipment);
}

bool ThermalZone::isPlenum() const
{
  return getImpl<detail::ThermalZone_Impl>()->isPlenum();
}

bool ThermalZone::canBePlenum() const
{
  return getImpl<detail::ThermalZone_Impl>()->canBePlenum();
}

bool ThermalZone::setSupplyPlenum(const ThermalZone & plenumZone)
{
  return getImpl<detail::ThermalZone_Impl>()->setSupplyPlenum(plenumZone);
}

bool ThermalZone::setSupplyPlenum(const ThermalZone & plenumZone, unsigned branchIndex)
{
  return getImpl<detail::ThermalZone_Impl>()->setSupplyPlenum(plenumZone,branchIndex);
}

void ThermalZone::removeSupplyPlenum()
{
  getImpl<detail::ThermalZone_Impl>()->removeSupplyPlenum();
}

void ThermalZone::removeSupplyPlenum(const AirLoopHVAC & airloop)
{
  getImpl<detail::ThermalZone_Impl>()->removeSupplyPlenum(airloop);
}

void ThermalZone::removeSupplyPlenum(const AirLoopHVAC & airloop, unsigned branchIndex)
{
  getImpl<detail::ThermalZone_Impl>()->removeSupplyPlenum(airloop, branchIndex);
}

void ThermalZone::removeSupplyPlenum(unsigned branchIndex)
{
  getImpl<detail::ThermalZone_Impl>()->removeSupplyPlenum(branchIndex);
}

bool ThermalZone::setReturnPlenum(const ThermalZone & plenumZone)
{
  return getImpl<detail::ThermalZone_Impl>()->setReturnPlenum(plenumZone);
}

bool ThermalZone::setReturnPlenum(const ThermalZone & plenumZone, AirLoopHVAC & airLoop)
{
  return getImpl<detail::ThermalZone_Impl>()->setReturnPlenum(plenumZone, airLoop);
}

void ThermalZone::removeReturnPlenum()
{
  getImpl<detail::ThermalZone_Impl>()->removeReturnPlenum();
}

void ThermalZone::removeReturnPlenum(AirLoopHVAC & airLoop)
{
  getImpl<detail::ThermalZone_Impl>()->removeReturnPlenum(airLoop);
}

std::vector<ZoneMixing> ThermalZone::zoneMixing() const
{
  return getImpl<detail::ThermalZone_Impl>()->zoneMixing();
}

std::vector<ZoneMixing> ThermalZone::supplyZoneMixing() const
{
  return getImpl<detail::ThermalZone_Impl>()->supplyZoneMixing();
}

std::vector<ZoneMixing> ThermalZone::exhaustZoneMixing() const
{
  return getImpl<detail::ThermalZone_Impl>()->exhaustZoneMixing();
}

boost::optional<HVACComponent> ThermalZone::airLoopHVACTerminal() const
{
  return getImpl<detail::ThermalZone_Impl>()->airLoopHVACTerminal();
}

std::vector<HVACComponent> ThermalZone::airLoopHVACTerminals() const
{
  return getImpl<detail::ThermalZone_Impl>()->airLoopHVACTerminals();
}

boost::optional<ZoneControlContaminantController> ThermalZone::zoneControlContaminantController() const
{
  return getImpl<detail::ThermalZone_Impl>()->zoneControlContaminantController();
}

bool ThermalZone::setZoneControlContaminantController(const ZoneControlContaminantController & contaminantController)
{
  return getImpl<detail::ThermalZone_Impl>()->setZoneControlContaminantController(contaminantController);
}

void ThermalZone::resetZoneControlContaminantController()
{
  getImpl<detail::ThermalZone_Impl>()->resetZoneControlContaminantController();
}

AirflowNetworkZone ThermalZone::getAirflowNetworkZone()
{
  return getImpl<detail::ThermalZone_Impl>()->getAirflowNetworkZone();
}

boost::optional<AirflowNetworkZone> ThermalZone::airflowNetworkZone() const
{
  return getImpl<detail::ThermalZone_Impl>()->airflowNetworkZone();
}

std::vector<AirLoopHVAC> ThermalZone::airLoopHVACs() const
{
  return getImpl<detail::ThermalZone_Impl>()->airLoopHVACs();
}

/// @cond
ThermalZone::ThermalZone(std::shared_ptr<detail::ThermalZone_Impl> impl)
  : HVACComponent(std::move(impl))
{}
/// @endcond


} // model
} // openstudio
