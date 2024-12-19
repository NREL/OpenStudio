/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ForwardTranslator.hpp"
#include "UserModel.hpp"

#include "../utilities/time/Date.hpp"

#include "../model/Model.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/BoilerHotWater_Impl.hpp"
#include "../model/BoilerSteam.hpp"
#include "../model/BoilerSteam_Impl.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/ChillerElectricEIR_Impl.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../model/CoilCoolingDXTwoSpeed.hpp"
#include "../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilHeatingDXSingleSpeed.hpp"
#include "../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingElectric_Impl.hpp"
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilHeatingGas_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/ConnectorMixer.hpp"
#include "../model/ConnectorMixer_Impl.hpp"
#include "../model/ConnectorSplitter.hpp"
#include "../model/ConnectorSplitter_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/ExteriorLights.hpp"
#include "../model/ExteriorLights_Impl.hpp"
#include "../model/ExteriorLightsDefinition.hpp"
#include "../model/ExteriorLightsDefinition_Impl.hpp"
#include "../model/Gas.hpp"
#include "../model/Gas_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/GasMixture.hpp"
#include "../model/GasMixture_Impl.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include "../model/OpaqueMaterial.hpp"
#include "../model/OpaqueMaterial_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/PipeAdiabatic.hpp"
#include "../model/PipeAdiabatic_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/RefractionExtinctionGlazing.hpp"
#include "../model/RefractionExtinctionGlazing_Impl.hpp"
#include "../model/RoofVegetation.hpp"
#include "../model/RoofVegetation_Impl.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/StandardGlazing.hpp"
#include "../model/StandardGlazing_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermochromicGlazing.hpp"
#include "../model/ThermochromicGlazing_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/WaterHeaterMixed.hpp"
#include "../model/WaterHeaterMixed_Impl.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../model/ZoneHVACUnitHeater.hpp"
#include "../model/ZoneHVACUnitHeater_Impl.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"

#include <math.h>

namespace openstudio {
namespace isomodel {

  ForwardTranslator::ForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.isomodel()\\.ForwardTranslator()"));
  }

  UserModel ForwardTranslator::translateModel(const openstudio::model::Model& t_model) {
    m_logSink.resetStringStream();

    /// \todo this should probably be a clone in case the uniqueModelObject calls manipulate the model?
    openstudio::model::Model model = t_model;

    LOG(Debug, "...Setting() Defaults");
    LOG(Debug, "*****************************");
    LOG(Debug, "       Setting Defaults");
    LOG(Debug, "*****************************");

    // set the flags below to true or false to try to get info from OSM file or simply use defaults below
    bool OSM_extract_occupancy = true;      // extract days and hours of occupancy
    bool OSM_extract_temp_setpoint = true;  // extract temperature setpoints
    bool OSM_extract_dhw = true;            // extract dhw info
    bool OSM_extract_vent_rate = true;      // extract the ventilation rate
    bool OSM_extract_infil_rate = true;     // extract infiltration rate info
    bool OSM_extract_HVAC = true;           // extract HVAC info like cooling COP, heating efficiencies, fuel type.
    /// \todo unused
    //bool OSM_extract_glazing_info = true;   // extract glazing info (U & SHGC)
    //bool OSM_extract_wallroof_info = true;  // extract the wall and roof U and other info

    // NOTE  YOU *MUST* SET HVAC SYSTEM TYPE and IPLV/COP ratio below

    // set fan and pump control factors;  (1.0 = no energy saving features)
    // start the strings with a floating point as that's what is stripped off to set the falue
    // fan control  1 = no control, 0.75 = inlet blade adjuct, 0.65= variable speed  see NEN 2916 7.3.3.4
    double fan_flow_control_factor_default = 1.0;  // set default fan flow control factor to 1.0 (no energy savings)
    // pump control 0 = no pump, 0.5 = auto pump controls for more 50% of pumps, 1.0 = all other cases.   See NEN 2914 9.4.3
    double heating_pump_control_factor_default = 1.0;  // set default heating pump control factor to 1.0 (no control or vfd)
    double cooling_pump_control_factor_default = 1.0;  // set default cooling pump control factor to 1.0 (no control or vfd)

    // until we can extract HVAC inputs and guess at HVAC type, set these defaults
    // give the HVAC type number first and a descriptive string after
    int hvac_type_default = 24;            // set default HVAC type to VAV
    double cooling_IPLVToCop_ratio = 1.0;  // set default system partial load ratio
    int ventilation_type_default = 1;      //set ventilation type (1 = mech only, 2 = natural, 3 = mixed)
    int bem_type_default = 1;              // set bem_type=1 (1 = none or minimal BEM, 2 = modern BEM, 3 = advanced BEM w/ FDD)

    // HVAC settings that are hard to extract but might be changed
    double exhaust_recirculation_fraction = 0.0;  // set fraction of air recirculated  to 0.0 (i.e. total outside air)
    double heat_recovery_fraction = 0.0;          // set ventilation heat recovery fraction to 0 since OS doesn't support heat recovery yet

    //these are items that aren't modeled in OS or are hard to extract so set default values
    int occupancy_sensors_default = 1;  // occupancy sensors:  false = none or minimal true > if 60% of floor space has it
    int const_illum_ctrl_default = 1;   //constant illumination control: false = none, true

    // this can be changed within OS, but it is often not sent.
    double specific_fan_power = 1 / 0.7733;  // set default specific fan power (L/W) to match EnergyPlus default

    // set default SCF and SDF for all windows.
    double solar_control_factor = 1.0;
    double shading_device_factor = 1.0;

    // set fall through defaults in case extraction fails or we decide not to extract

    // default occupancy info if we can't extract
    int occupancy_day_start = 1;   // set default starting day of occupancy as monday (Su=0, Mon=1 Sa=6)
    int occupancy_day_end = 5;     // set the default ending day of occupancy
    int occupancy_hour_start = 8;  // set the default starting hour for occupancy for an occupied day
    int occupancy_hour_end = 18;   // set the default ending hour for occupancy for an unoccupied day

    // temperature setpoint defaults
    double heating_setpoint_occ_default = 21;    // set the default temperature setpoint for heating during occupied times
    double heating_setpoint_unocc_default = 15;  // set the default temperature setpoint for heating during unccupied times
    double cooling_setpoint_occ_default = 25;    // set the default temp setpoint for cooling during occupied times
    double cooling_setpoint_unocc_default = 28;  // set the default temp setpoint for cooling during unoccupied times

    // HVAC defaults
    double cooling_COP_default = 3.2;
    double heating_system_efficiency_default = 0.8;
    int heating_fuel_type_default = 2;  // set default fuel type to 2 = gas

    // Ventilation defaults
    double vent_rate_per_person_default = 10;  // set default ventilation rate (10 L/s/persoin ~ 20 cfm/person is good for offices)

    // DHW defaults
    //set dhw distribution type 0, close to taps, 1 far from taps, 2 = circulation/other/ unknown)
    int dhw_dist_type_default = 2;
    double dhw_rate_per_person_default = 3.8;  // set default dhw rate at 3.8 L/person/day from ASHRAE HVAC Apps chapter 50
    int dhw_fuel_type_default = 2;             // set default DHW fuel to gas (1 = electric, 2 = gas)
    double dhw_system_efficiency_default = 0.8;

    // Infiltration defaults
    double infil_rate_default = 7.0;  // set default infiltration rate to 7 m3/m2/hr@ 75 Pa by default to match normal EnergyPlus default values

    // set envelope defaults U in W/m2/K
    double glazing_U_default = 3.0;     // default is simple IGU
    double glazing_SHGC_default = 0.7;  // default is clear IGU

    // set default inside and outside air film R values
    double outside_air_film_R = 0.04;  // set the outside air film coefficient R value in m2*K/W
    double inside_air_film_R = 0.13;   // set the inside air film coefficient R value in m2*K/W

    // convert strings to integers and floats
    int bem_type = bem_type_default;
    int hvac_type = hvac_type_default;
    int ventilation_type = ventilation_type_default;

    double fan_flow_control_factor = fan_flow_control_factor_default;
    double heating_pump_control_factor = heating_pump_control_factor_default;
    double cooling_pump_control_factor = cooling_pump_control_factor_default;
    int dhw_dist_type = dhw_dist_type_default;

    int occupancy_sensors = occupancy_sensors_default;
    int const_illum_ctrl = const_illum_ctrl_default;

    // set SCF (external solar control) and SDF (shading) these for all directions N, NE, E, SE, S, SW, W, NW
    std::vector<double> window_SCF(8, solar_control_factor);   // set default window solar control factor = 1.0 (no overhang solar control)
    std::vector<double> window_SDF(8, shading_device_factor);  // set default window shading device factor = 1.0 (no shades)

    LOG(Debug, "*****************************");
    LOG(Debug, "       Setting Defaults");
    LOG(Debug, "*****************************");

    LOG(Debug, "Occupancy Sensors set to " << occupancy_sensors_default);
    LOG(Debug, "Constant Illumination Control set to " << const_illum_ctrl_default);

    LOG(Debug, "HVAC system type set to " << hvac_type_default);
    LOG(Debug, "HVAC set IPLV/COP,  mean partial load fraction,  to " << cooling_IPLVToCop_ratio);
    LOG(Debug, "Heat recovery set to " << heat_recovery_fraction);
    LOG(Debug, "Exhaust air recirculation fraction set to " << exhaust_recirculation_fraction);
    LOG(Debug, "Ventilation type set to " << ventilation_type_default);
    LOG(Debug, "BAS/BEM set to " << bem_type_default);

    LOG(Debug, "Specific fan power set to " << specific_fan_power << " (L/s/W)");
    LOG(Debug, "DHW distribution system set " << dhw_dist_type_default);
    LOG(Debug, "Fan Flow Control Factor set " << fan_flow_control_factor_default);
    LOG(Debug, "Heating Pump Control Factor set " << heating_pump_control_factor_default);
    LOG(Debug, "Cooling pump Control Factor set " << cooling_pump_control_factor_default);
    LOG(Debug, "Setting window  SCF=" << solar_control_factor << ", SDF=" << shading_device_factor);

    LOG(Debug, "*****************************");
    LOG(Debug, "Setting Fall Through Defaults");
    LOG(Debug, "*****************************");

    LOG(Debug, "Occupancy Start Day = " << occupancy_day_start << " with Sun = 0, Sat = 6");
    LOG(Debug, "Occupancy End Day = " << occupancy_day_end << " with Sun = 0, Sat = 6");
    LOG(Debug, "Occupancy Start Hour = " << occupancy_hour_start << " ");
    LOG(Debug, "Occupancy End Hour = " << occupancy_hour_end);

    LOG(Debug, "Heating Temp Set Point During Occupied Times = " << heating_setpoint_occ_default << " C");
    LOG(Debug, "Heating Temp Set Point During Unccupied Times = " << heating_setpoint_unocc_default << " C");
    LOG(Debug, "Cooling Temp Set Point During Occupied Times = " << cooling_setpoint_occ_default << " C");
    LOG(Debug, "Cooling Temp Set Point During Unccupied Times = " << cooling_setpoint_unocc_default << " C");

    LOG(Debug, "Default Cooling Cop = " << cooling_COP_default << " W/W");
    LOG(Debug, "Default Heating System Efficiency = " << heating_system_efficiency_default);
    LOG(Debug, "Default Heating Fuel Type = " << heating_fuel_type_default);

    LOG(Debug, "Default outside air film resistance = " << outside_air_film_R << " W/m2/K");
    LOG(Debug, "Default inside air film resistance = " << inside_air_film_R << " W/m2/K");

    LOG(Debug, "...Parsing() Model");

    LOG(Debug, "*****************************");
    LOG(Debug, "  Parsing Model");
    LOG(Debug, "*****************************");

    // get the weatherfile if one is defined in the OSM and convert to a string;
    // wfu = model.getWeatherFile().url().to_s();
    // if wfu.empty();
    // LOG(Debug, "Warning: A Weather file !defined in the OSM");
    // weather_file_path="none";
    // } else {

    // strip off the file: part of the URL, only save the stuff after file:;
    // weather_file_path=wfu.split(':').last();
    // LOG(Debug, "Weather file " << weather_file_path << " found");
    // }

    // get some important objects from the building model;
    openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();
    openstudio::model::Facility facility = model.getUniqueModelObject<openstudio::model::Facility>();
    std::vector<openstudio::model::Surface> surfaces = model.getConcreteModelObjects<openstudio::model::Surface>();
    std::vector<openstudio::model::SubSurface> sub_surfaces = model.getConcreteModelObjects<openstudio::model::SubSurface>();
    std::vector<openstudio::model::Space> spaces = model.getConcreteModelObjects<openstudio::model::Space>();
    std::vector<openstudio::model::ThermalZone> thermal_zones = model.getConcreteModelObjects<openstudio::model::ThermalZone>();
    std::vector<openstudio::model::SpaceType> space_types = model.getConcreteModelObjects<openstudio::model::SpaceType>();
    std::vector<openstudio::model::Construction> constructions = model.getConcreteModelObjects<openstudio::model::Construction>();
    std::vector<openstudio::model::InternalMassDefinition> internal_masses =
      model.getConcreteModelObjects<openstudio::model::InternalMassDefinition>();
    std::vector<openstudio::model::AirLoopHVAC> air_loops = model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>();
    std::vector<openstudio::model::PlantLoop> plant_loops = model.getConcreteModelObjects<openstudio::model::PlantLoop>();

    LOG(Debug, "Found " << surfaces.size() << " surfaces");
    LOG(Debug, "Found " << sub_surfaces.size() << " sub surfaces");
    LOG(Debug, "Found " << spaces.size() << " spaces");
    LOG(Debug, "Found " << thermal_zones.size() << " thermal zones");
    LOG(Debug, "Found " << space_types.size() << " space types");
    LOG(Debug, "Found " << constructions.size() << " constructions");
    LOG(Debug, "Found " << internal_masses.size() << " Internal Mass Definitions");
    LOG(Debug, "Found " << air_loops.size() << " Air Loops");
    LOG(Debug, "Found " << plant_loops.size() << " Plant Loops");

    // get the terrain class from the site object or assign default if none given;
    std::string terrain;
    openstudio::model::Site site = model.getUniqueModelObject<openstudio::model::Site>();
    if (!site.terrain().empty()) {
      terrain = site.terrain();
      LOG(Debug, "Terrain Type " << terrain << " in OSM");
    } else {
      LOG(Debug, "Site Terrain Not Defined in OpenStudio Model - Set to Urban Terrain By Default");
      terrain = "Urban";
    }

    double terrain_class = 0;
    // when we have ocean or country, there are no blocks so terrain_class=1.0;
    if (terrain == "Ocean" || terrain == "Country") {
      terrain_class = 1.0;
    } else if (terrain == "Suburbs") {
      terrain_class = 0.9;
    } else {
      // when city or urban set to 0.8;
      terrain_class = 0.8;
    }

    LOG(Debug, "Terrain class value set to " << terrain_class);

    int number_of_people = (int)building.numberOfPeople();
    // get the total nominal population;
    LOG(Debug, "Nominal Number of people = " << number_of_people);

    LOG(Debug, "*****************************");
    LOG(Debug, "  Analyzing Construction");
    LOG(Debug, "*****************************");

    std::vector<std::string> const_names;
    std::map<std::string, double> const_solarAbsorptance;
    std::map<std::string, double> const_thermalAbsorptance;
    std::map<std::string, double> const_heatCapacity;
    std::map<std::string, double> const_U;
    std::map<std::string, double> const_SHGC;

    for (const openstudio::model::Construction& construction : constructions) {
      std::string name = construction.name().get();
      std::vector<openstudio::model::Material> layers = construction.layers();
      const_names.push_back(name);

      // assign 0.0 as default values in case we can!assign it by searching through the construction;
      const_U[name] = 0.0;
      const_heatCapacity[name] = 0.0;
      const_thermalAbsorptance[name] = 0.0;
      const_solarAbsorptance[name] = 0.0;
      const_SHGC[name] = 0.0;

      // first find the opaque constructions - these could be exterior walls and roofs;
      if (construction.isOpaque()) {

        // get the 1st layer of construction (outside layer), convert to opaque material and get the thermal & solar Absorptance;
        // store them in an array indexed by the name of the construction;

        if (layers[0].optionalCast<openstudio::model::OpaqueMaterial>()) {
          const_thermalAbsorptance[name] = layers[0].optionalCast<openstudio::model::OpaqueMaterial>().get().thermalAbsorptance();
        }
        if (layers[0].optionalCast<openstudio::model::OpaqueMaterial>()) {
          const_solarAbsorptance[name] = layers[0].optionalCast<openstudio::model::OpaqueMaterial>().get().solarAbsorptance();
        }

        //set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls;
        double const_R = outside_air_film_R + inside_air_film_R;
        double const_heat_capacity = 0.0;
        for (const openstudio::model::Material& layer : layers) {
          double layer_R = 0;
          double layer_heat_capacity = 0;

          if (layer.optionalCast<openstudio::model::StandardOpaqueMaterial>()) {
            // do this stuff if the material can convert to StandardOpaqueMaterial;
            openstudio::model::StandardOpaqueMaterial material = layer.optionalCast<openstudio::model::StandardOpaqueMaterial>().get();
            // get the R value of the layer;
            layer_R = material.thermalResistance();
            if (material.heatCapacity()) {
              // get the heat capacity directly;
              layer_heat_capacity = material.heatCapacity().get();
            } else {
              // if heat capacity is !defined, calculated it as thickness*density*specific heat;
              layer_heat_capacity = material.specificHeat() * material.density() * material.thickness();
            }
          } else if (layer.optionalCast<openstudio::model::MasslessOpaqueMaterial>()) {
            // do this stuff if the material can convert to a MasslessOpaqueMaterial;
            openstudio::model::MasslessOpaqueMaterial material = layer.optionalCast<openstudio::model::MasslessOpaqueMaterial>().get();
            // get the R value of the layer;
            layer_R = material.thermalResistance();
            // heat capacity is !defined for this material;
            layer_heat_capacity = 0.0;
          } else if (layer.optionalCast<openstudio::model::RoofVegetation>()) {
            openstudio::model::RoofVegetation material = layer.optionalCast<openstudio::model::RoofVegetation>().get();
            layer_R = material.thermalResistance();
            layer_heat_capacity = material.specificHeat() * material.density() * material.thickness();
          } else {
            layer_R = 0.0;
            layer_heat_capacity = 0.0;
          }
          const_R += layer_R;
          const_heat_capacity += layer_heat_capacity;
        }
        const_heatCapacity[name] = const_heat_capacity;
        const_U[name] = 1.0 / const_R;

        // next find the fenestration/window constructions find U value, heat capcity and SHGC of assembly;
        // this is somewhat flaky and nowhere near as good as extracting from an EnergyPlus file after running OS/EP, but it will have to do for now
      } else if (construction.isFenestration()) {
        // assume windows have no heat capacity;
        const_heatCapacity[name] = 0.0;
        // assume windows have an effective thermal emissivity of 0;
        const_thermalAbsorptance[name] = 0.0;
        // assume windows have an effective solar absorption of 0;
        const_solarAbsorptance[name] = 0.0;

        //set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls;
        double const_R = outside_air_film_R + inside_air_film_R;

        // first check if the construction is a  single layer;
        //simple glazing, if so pull off U factor and SHGC directly, set heat capacity to 0;
        if (layers.size() == 1) {

          openstudio::model::Material layer = layers[0];
          if (layer.optionalCast<openstudio::model::SimpleGlazing>()) {
            // check to see if the layer is simple glazing;
            //convert to a simple glazing material;
            openstudio::model::SimpleGlazing material = layer.optionalCast<openstudio::model::SimpleGlazing>().get();
            // U factor is directly defined;
            const_U[name] = material.uFactor();
            const_SHGC[name] = material.solarHeatGainCoefficient();
          } else if (layer.optionalCast<openstudio::model::StandardGlazing>()) {
            // check to see if the layer is standard glazing;
            // convert the layer to a standard glazing;
            openstudio::model::StandardGlazing material = layer.optionalCast<openstudio::model::StandardGlazing>().get();
            // U value = thermal conductance + film resistance;
            const_U[name] = 1.0 / (1.0 / material.thermalConductance() + const_R);
            // SHGC = solar transmittance - ignore frame;
            const_SHGC[name] = material.solarTransmittance();
          } else if (layer.optionalCast<openstudio::model::ThermochromicGlazing>()) {
            LOG(Debug, "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3");
            openstudio::model::ThermochromicGlazing material = layer.optionalCast<openstudio::model::ThermochromicGlazing>().get();
            // \todo this material type doesn't have a thermalConductance
            //const_U[name] = 1.0/(1.0/ material.thermalConductance() + const_R);
            const_SHGC[name] = 0.3;
          } else if (layer.optionalCast<openstudio::model::RefractionExtinctionGlazing>()) {
            LOG(Debug, "Refraction Extinction Glazing Not Converted - only estimating U value, others set to clear glass values");
            openstudio::model::RefractionExtinctionGlazing material = layer.optionalCast<openstudio::model::RefractionExtinctionGlazing>().get();
            const_U[name] = 1.0 / (1.0 / material.thermalConductance() + const_R);
            const_SHGC[name] = 0.8;
          }
        } else if (layers.size() == 3) {
          // double glaze window.   assume glass - gas - glass layers;
          // assume that a layer is !simple glazing;
          if (layers[0].optionalCast<openstudio::model::StandardGlazing>()) {  // check to see if layer 1 is standard glazing;
            // get layer 1 properties;
            // convert the 1st layer to a standard glazing;
            openstudio::model::StandardGlazing material1 = layers[0].optionalCast<openstudio::model::StandardGlazing>().get();
            double norm_ST1 = material1.solarTransmittanceatNormalIncidence().get();
            double uValue1 = material1.thermalConductance();
            double norm_back_SR1 = material1.backSideSolarReflectanceatNormalIncidence().get();

            /// \todo these are set but not used
            double norm_ST2 = 0;
            double uValue2 = 0;

            // Now get layer 2 properties;
            if (layers[1].optionalCast<openstudio::model::StandardGlazing>()) {  // check to see if layer 2 is standard glazing;
              // convert the 2nd layer to a gas mixture;
              openstudio::model::StandardGlazing material2 = layers[1].optionalCast<openstudio::model::StandardGlazing>().get();
              norm_ST2 = material2.solarTransmittanceatNormalIncidence().get();
              uValue2 = material2.thermalConductance();
            } else if (layers[1].optionalCast<openstudio::model::AirGap>()) {  // check to see if layer 1 is an air gap;
              // convert the 2nd layer to an air gap;
              openstudio::model::AirGap material2 = layers[1].optionalCast<openstudio::model::AirGap>().get();
              norm_ST2 = 1.0;
              uValue2 = 1.0 / material2.thermalResistance();
            } else if (layers[1].optionalCast<openstudio::model::Gas>()) {  // check to see if layer 1 is a gas;
              // convert the 2nd layer to a simple gas layer;
              openstudio::model::Gas material2 = layers[1].optionalCast<openstudio::model::Gas>().get();
              norm_ST2 = 1.0;
              // get U value at 290 K;
              uValue2 = material2.getThermalConductance(290);
            } else if (layers[1].optionalCast<openstudio::model::GasMixture>()) {  // check to see if layer 1 is a gas;
              // convert the 2nd layer to a simple gas layer;
              openstudio::model::GasMixture material2 = layers[1].optionalCast<openstudio::model::GasMixture>().get();
              norm_ST2 = 1.0;
              // get U value at 290 K;
              uValue2 = material2.getThermalConductance(290);
            } else {
              // we can't figure out what it is so assume properties of simple 12 mm air gap w/ U~6.0 from ASHRAE 2009;
              norm_ST2 = 1.0;
              uValue2 = 6.0;
            }

            double norm_ST3 = 0;
            double norm_front_SR3 = 0;
            double uValue3 = 0;

            // now get layer 3 properties;
            if (layers[2].optionalCast<openstudio::model::StandardGlazing>()) {
              // check to see if layer 2 is standard glazing;
              openstudio::model::StandardGlazing material3 = layers[2].optionalCast<openstudio::model::StandardGlazing>().get();
              // convert the 2nd layer to a gas mixture;
              norm_ST3 = material3.solarTransmittanceatNormalIncidence().get();
              uValue3 = material3.thermalConductance();
              norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence().get();
            } else {
              // If it isn't standard glazing we don't know what it might be so assume properties of simple 3 mm clear glazing;
              norm_ST3 = 0.87;
              uValue3 = 6.0;
              /// \note this value is impossible to get here, there is no material3 in this context, so I'm commenting it out
              // norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence().get();
            }
            // compute SHGC of a 3 layer window as ST1 * ST3 / (1 - R1 *R3);
            const_SHGC[name] = norm_ST1 * norm_ST3 / (1.0 - norm_back_SR1 * norm_front_SR3) * norm_ST2;
            // for U value = 1/ (sum R values for each layer + film coefficients);
            const_U[name] = 1.0 / (1.0 / uValue1 + 1.0 / uValue2 + 1.0 / uValue3 + const_R);

          } else if (layers[0].optionalCast<openstudio::model::ThermochromicGlazing>()) {
            LOG(Debug, "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3");
            openstudio::model::ThermochromicGlazing material = layers[0].optionalCast<openstudio::model::ThermochromicGlazing>().get();
            // \todo this material type doesn't have a thermalConductance
            // const_U[name] = 1.0/(1.0/ material.thermalConductance() + const_R);
            const_SHGC[name] = 0.3;
          } else if (!layers[0].optionalCast<openstudio::model::RefractionExtinctionGlazing>()) {
            LOG(Debug, "Refraction Extinction Glazing Not Converted - only estimating U value, others set to 0.7");
            openstudio::model::RefractionExtinctionGlazing material = layers[0].optionalCast<openstudio::model::RefractionExtinctionGlazing>().get();
            const_U[name] = 1.0 / (1.0 / material.thermalConductance() + const_R);
            const_SHGC[name] = 0.7;
          }
        } else {
          LOG(Debug, "Only 1 and 3 layer windows are supported at this time, values set to a standard clear, double glaze window");
          const_U[name] = glazing_U_default;
          const_SHGC[name] = glazing_SHGC_default;
        }
      }
    }

    for (const std::string& name : const_names) {
      LOG(Debug, "Construction: " << name);
      LOG(Debug, "Ufactor = " << const_U[name] << " W/K/m2, Heat Capacity = " << const_heatCapacity[name] << " J/K/m2");
      LOG(Debug, "Solar Absorption = " << const_solarAbsorptance[name] << ", Thermal Absorption = " << const_thermalAbsorptance[name]
                                       << ", SHGC = " << const_SHGC[name]);
    }

    // parse the schedules to find the average schedule value for occupied and unoccupied times;
    LOG(Debug, "...Parsing Schedules");

    std::vector<openstudio::model::ScheduleRuleset> schedule_rulesets = model.getConcreteModelObjects<openstudio::model::ScheduleRuleset>();
    LOG(Debug, "Found " << schedule_rulesets.size() << " schedules in the OSM");

    // get the year description from the model and set the calendar year to 2007
    // we use 2006 because Jan 1 is a sunday and it makes day 1 = sunday
    openstudio::model::YearDescription yd = model.getUniqueModelObject<openstudio::model::YearDescription>();
    yd.setCalendarYear(2006);

    std::vector<openstudio::Time> t(24);

    for (size_t i = 0; i <= 23; ++i) {
      t[i] = openstudio::Time(i / 24.0);
    }

    if (OSM_extract_occupancy) {
      // create a hash with all the schedule_ruleset since we can't access dayschedules directly from
      // the schedule stored in the space_type, we need to find the associated schedule_ruleset

      std::map<std::string, openstudio::model::ScheduleRuleset> sched_rulesets;

      for (const openstudio::model::ScheduleRuleset& ruleset : schedule_rulesets) {
        std::string name = ruleset.name().get();
        sched_rulesets.insert(std::make_pair(name, ruleset));
      }

      std::vector<std::vector<double>> occupancy_schedules_ave(7, std::vector<double>(24, 0));
      [[maybe_unused]] int nscheds = 0;
      double space_area_total = 0.0;

      // get an area weighted average over all the space types.

      for (const openstudio::model::SpaceType& space_type : space_types) {
        double space_area = space_type.floorArea();
        space_area_total += space_area;

        for (const openstudio::model::People& people : space_type.people()) {
          // look for an occupancy schedule

          if (people.numberofPeopleSchedule()) {
            // get the schedule.   Unfortuntately we need to find the associated sched ruleset to get the day schedule
            openstudio::model::Schedule sched = people.numberofPeopleSchedule().get();

            openstudio::model::ScheduleRuleset ruleset = sched_rulesets.find(sched.name().get())->second;

            for (size_t week = 0; week <= 51; ++week) {
              openstudio::Date startDate = yd.makeDate(week * 7 + 1);
              openstudio::Date endDate = yd.makeDate(week * 7 + 8);

              nscheds += 1;
              space_area_total += space_area;

              std::vector<openstudio::model::ScheduleDay> dayschedule =
                ruleset.getDaySchedules(startDate, endDate);  // get the day schedules for the week

              for (size_t day = 0; day <= 6; ++day) {
                for (size_t hour = 0; hour <= 23; ++hour) {
                  double value = dayschedule[day].getValue(t[hour]);
                  occupancy_schedules_ave[day][hour] += value * space_area;  // add in area weighted value
                }
              }
            }
          }
        }
      }

      // now get the area weighted average values for the schedules by dividing by the number of schedules averaged
      // also get the total daily fractional occupancy (i.e. equivalent hours of full time occupancy)

      double threshold = 7.0;
      int num_occ_days = 0;

      std::vector<double> occ_frac_sum(7, 0);
      std::vector<int> day_occ(7, 0);

      for (size_t day = 0; day <= 6; ++day) {
        for (size_t hour = 0; hour <= 23; ++hour) {
          occupancy_schedules_ave[day][hour] = occupancy_schedules_ave[day][hour] / space_area_total;
          occ_frac_sum[day] += occupancy_schedules_ave[day][hour];
        }

        if (occ_frac_sum[day] > threshold) {
          day_occ[day] = 1;
          num_occ_days += 1;
        }
      }

      // now find the first and last occupied day of the week assuming a continuous stream
      int first_occ_day =
        (int)std::distance(day_occ.begin(), std::find(day_occ.begin(), day_occ.end(), 1));  //find the first instance of 1 in the day_occ array
      int last_occ_day =
        6
        - (int)std::distance(day_occ.rbegin(),
                             std::find(day_occ.rbegin(), day_occ.rend(),
                                       1));  // find the last instance of 1 by reversing array to find how far from the end the last occupied day is

      // now, for the days considered occupied, find an "average" occupancy schedule to estimate the best starting and stopping times

      // find an "average" schedule during the occupied days (i.e. do not include unoccupied days in this average)

      std::vector<double> avg_occupied_schedule(24, 0);
      double avg_occ_sum = 0.0;

      for (size_t hour = 0; hour <= 23; ++hour) {
        avg_occupied_schedule[hour] = 0.0;

        for (int day = first_occ_day; day <= last_occ_day; ++day) {
          avg_occupied_schedule[hour] += occupancy_schedules_ave[day][hour];
        }

        if (num_occ_days > 0) {

          avg_occupied_schedule[hour] = avg_occupied_schedule[hour] / num_occ_days;
        }
        avg_occ_sum += avg_occupied_schedule[hour];

        //printf("%1.1f, ",avg_occupied_schedule[hour])
      }
      //print "\n"

      // now estimate the average number of fully occupied hours per day by rounding down the sum to the nearest number of hours
      int num_occ_hours = (int)std::floor(avg_occ_sum);

      // now we find the best start time for the occupancy by matching a sliding window of full occupancy to the average occupancy
      // we maximize the sum of the product of actual occupancy and the sliding window to get the best hours of operation
      // find the start time by maximizing the sum of the product of the start-to-end sliding window and the average hourly occupancy

      double max_sum = 0.0;
      int max_index = 0;

      for (int start_hour = 0; start_hour <= 23 - num_occ_hours; ++start_hour) {
        double temp_sum = 0.0;

        for (int i = start_hour; i <= start_hour + num_occ_hours; ++i) {
          temp_sum += avg_occupied_schedule[i];
        }

        if (temp_sum > max_sum) {

          max_sum = temp_sum;

          max_index = start_hour;
        }
      }

      occupancy_day_start = first_occ_day;  // set starting day for occupancy
      occupancy_day_end = last_occ_day;     // set the ending day for occupancy
      occupancy_hour_start = max_index;     // set starting hour for occupied day
      occupancy_hour_end =
        max_index + num_occ_hours + 1;  // set ending hour for an occupied day.   Add one because we want occupancy to drop at that time
    }

    LOG(Debug, "*******************************");
    LOG(Debug, " Calculating Schedule Averages");
    LOG(Debug, "*******************************");

    std::vector<std::string> sched_names;
    std::map<std::string, double> occ_aves;
    std::map<std::string, double> unocc_aves;

    openstudio::Date startDate = yd.makeDate(1);
    openstudio::Date endDate = yd.makeDate(365);

    // compute the number of occupied days in a year for water use calculations

    double number_days_occupied_per_year = 52 * (occupancy_day_end - occupancy_day_start + 1);
    double occupied_hours = number_days_occupied_per_year * (occupancy_hour_end - occupancy_hour_start + 1);
    double unoccupied_hours = 8760 - occupied_hours;

    // gets all the schedule for each day of the year in one array

    for (const openstudio::model::ScheduleRuleset& schedule : schedule_rulesets) {
      double occupied_sum = 0;
      double unoccupied_sum = 0;

      // get the day of the week of the starting day of the schedule and subtract 1 from it because we increment before we compare
      int day_of_week = startDate.dayOfWeek().value() - 1;

      std::vector<openstudio::model::ScheduleDay> daySchedules = schedule.getDaySchedules(startDate, endDate);

      // loop over the schedule for each day of the year
      for (const openstudio::model::ScheduleDay& daySchedule : daySchedules) {
        day_of_week += 1;  // increment the day of week counter wrapping around at 7

        if (day_of_week > 6) {
          day_of_week = 0;
        }

        // loop over each hour of the day
        for (int hour = 0; hour <= 23; ++hour) {
          double value = daySchedule.getValue(t[hour]);  // get the schedule value at this hour

          // check if the day of the week is an occupied day or not
          if ((day_of_week >= occupancy_day_start) && (day_of_week <= occupancy_day_end)) {

            // check if the hour is also an occupied hour
            // if so, add value to occupied sum and increment occupied counter
            if ((hour >= occupancy_hour_start) && (hour < occupancy_hour_end)) {
              occupied_sum += value;
            } else {  // if hour not occupied, add value to unoccupied and increment unoccupied counter
              unoccupied_sum += value;
            }
          } else {  // if day is not occupied, add the hour value to unoccupied
            unoccupied_sum += value;
          }
        }
      }

      sched_names.push_back(schedule.name().get());
      occ_aves[schedule.name().get()] = occupied_sum / occupied_hours;
      unocc_aves[schedule.name().get()] = unoccupied_sum / unoccupied_hours;

      LOG(Debug, "Schedule " << schedule.name().get() << " has occupied ave = " << occ_aves[schedule.name().get()]
                             << " and unoccupied ave = " << unocc_aves[schedule.name().get()]);
    }

    // at this point we have an array with the schedule names and two arrays with the averages;
    // that are indexed by the schedule names.;

    // define what fraction of the year are occupied and unoccupied;
    double frac_year_occ = occupied_hours / 8760.0;
    double frac_year_unocc = unoccupied_hours / 8760.0;

    LOG(Debug, "Fraction of year occupied = " << frac_year_occ << " and Fraction of year unoccupied = " << frac_year_unocc);

    LOG(Debug, "...Calculating() Electric and Gas Loads");
    LOG(Debug, "*****************************");
    LOG(Debug, "  Calculating Electric and Gas Loads");
    LOG(Debug, "*****************************");

    // get the light, electrical, and gas power densities and people densities;
    // overall nominal lighting power density;
    double nominal_lpd = building.lightingPowerPerFloorArea();
    // overall nominal electric equipment power density W/m2;
    double nominal_epd = building.electricEquipmentPowerPerFloorArea();
    // overall nominal gas equipment power density W/m2;
    double nominal_gpd = building.gasEquipmentPowerPerFloorArea();

    // loop over the lighting, equipment, and occupancy schedules to get the occupied and unoccupied values;
    // and compute the area weighted averages;
    // set this to a tiny number to avoid divide by zero but to also avoid skewing the area;
    double light_total_area = 0;
    double light_occ_total = 0;
    double light_unocc_total = 0;
    double elec_total_area = 0;
    double elec_occ_total = 0;
    double elec_unocc_total = 0;
    double gas_total_area = 0;
    double gas_occ_total = 0;
    double gas_unocc_total = 0;
    double people_total_area = 0;
    double people_occ_total = 0;
    double people_unocc_total = 0;
    double activity_total_area = 0;
    double activity_occ_total = 0;
    double activity_unocc_total = 0;

    //std::vector<double> light_areas=[];
    //light_scheds=[];

    for (const openstudio::model::SpaceType& space_type : space_types) {
      // work with the lighting schedule - luminaires are !supported at this time;
      double space_area = space_type.floorArea();
      for (const openstudio::model::Lights& light : space_type.lights()) {
        if (light.schedule()) {
          // look for a lighting schedule;
          // get the schedule;
          openstudio::model::Schedule sched = light.schedule().get();
          light_total_area += space_area;
          light_occ_total += occ_aves[sched.name().get()] * space_area;
          light_unocc_total += unocc_aves[sched.name().get()] * space_area;
        }
      }

      for (const openstudio::model::ElectricEquipment& electric : space_type.electricEquipment()) {
        // look for an electric equipment schedule;
        if (electric.schedule()) {
          openstudio::model::Schedule sched = electric.schedule().get();
          elec_total_area += space_area;
          elec_occ_total += occ_aves[sched.name().get()] * space_area;
          elec_unocc_total += unocc_aves[sched.name().get()] * space_area;
        }
      }

      for (const openstudio::model::GasEquipment& gas : space_type.gasEquipment()) {
        // look for a gas equipment schedule;
        if (gas.schedule()) {
          openstudio::model::Schedule sched = gas.schedule().get();
          gas_total_area += space_area;
          gas_occ_total += occ_aves[sched.name().get()] * space_area;
          gas_unocc_total += unocc_aves[sched.name().get()] * space_area;
        }
      }

      for (const openstudio::model::People& people : space_type.people()) {
        // look for an occupancy schedule;
        if (people.numberofPeopleSchedule()) {
          openstudio::model::Schedule sched = people.numberofPeopleSchedule().get();
          people_total_area += space_area;
          people_occ_total += occ_aves[sched.name().get()] * space_area;
          people_unocc_total += unocc_aves[sched.name().get()] * space_area;
        }

        // look for an occupant activity level schedule;
        if (people.activityLevelSchedule()) {
          openstudio::model::Schedule sched = people.activityLevelSchedule().get();
          activity_total_area += space_area;
          activity_occ_total += occ_aves[sched.name().get()] * space_area;
          activity_unocc_total += unocc_aves[sched.name().get()] * space_area;
        }
      }
    }

    // compute the fractional multipliers from the schedule data;
    double lpd_occ_mult = 0;
    double lpd_unocc_mult = 0;
    double epd_occ_mult = 0;
    double epd_unocc_mult = 0;
    double gpd_occ_mult = 0;
    double gpd_unocc_mult = 0;
    double people_occ_mult = 0;
    double people_unocc_mult = 0;

    if (light_total_area > 0) {
      lpd_occ_mult = light_occ_total / light_total_area;
      lpd_unocc_mult = light_unocc_total / light_total_area;
    } else {
      lpd_occ_mult = 1;
      lpd_unocc_mult = 1;
    }
    if (elec_total_area > 0) {
      epd_occ_mult = elec_occ_total / elec_total_area;
      epd_unocc_mult = elec_unocc_total / elec_total_area;
    } else {
      epd_occ_mult = 1;
      epd_unocc_mult = 1;
    }
    if (gas_total_area > 0) {
      gpd_occ_mult = gas_occ_total / gas_total_area;
      gpd_unocc_mult = gas_unocc_total / gas_total_area;
    } else {
      gpd_occ_mult = 1;
      gpd_unocc_mult = 1;
    }
    if (people_total_area > 0) {
      people_occ_mult = people_occ_total / people_total_area;
      people_unocc_mult = people_unocc_total / people_total_area;
    } else {
      people_occ_mult = 1;
      people_unocc_mult = 1;
    }

    // compute the activity level (heat gain per person) from schedule data;
    double activity_level_occ = activity_occ_total / activity_total_area;
    double activity_level_unocc = activity_unocc_total / activity_total_area;

    // compute the occupied and unoccupied lighting and equipment power densities;
    double lpd_unoccupied = nominal_lpd * lpd_unocc_mult;

    // when computing occupied, subtract the occupied because the isomodel calc adds unoccupied to occupied to account for parasitic lighting;
    // that people often forget;
    double lpd_occupied = nominal_lpd * lpd_occ_mult - lpd_unoccupied;
    double epd_occupied = nominal_epd * epd_occ_mult;
    double epd_unoccupied = nominal_epd * epd_unocc_mult;
    double gpd_occupied = nominal_gpd * gpd_occ_mult;
    double gpd_unoccupied = nominal_gpd * gpd_unocc_mult;

    LOG(Debug, "Lighting Power Density: Occupied= " << lpd_occupied << " W/m2, Unoccupied = " << lpd_unoccupied << " W/m2");
    LOG(Debug, "Electric Equipment Power Density: Occupied= " << epd_occupied << " W/m2, Unoccupied = " << epd_unoccupied << " W/m2");
    LOG(Debug, "Gas Equipment Power Density: Occupied= " << gpd_occupied << " W/m2, Unoccupied = " << gpd_unoccupied << " W/m2\n");

    // compute the nominal area per person info;
    double nominal_app = 1.0 / building.peoplePerFloorArea();
    // nominal area per person m^2/person;

    // set the heat gain per person to be the average occupied value;
    double heat_gain_per_person_occ = activity_level_occ;
    double heat_gain_per_person_unocc = activity_level_unocc;

    // get the area per person scaled by occupancy multiplier;
    double area_per_person_occupied = nominal_app / people_occ_mult;
    double area_per_person_unoccupied = nominal_app / people_unocc_mult;

    LOG(Debug,
        "Occupant Heat Gain: Occupied= " << heat_gain_per_person_occ << " W/person, Unoccupied = " << heat_gain_per_person_unocc << " W/person");
    LOG(Debug,
        "Area per person: Occupied= " << area_per_person_occupied << " m2/person, Unoccupied = " << area_per_person_unoccupied << " m2/person");

    // compute exterior lighting power;
    double exterior_lighting_power = 0.0;
    if (!facility.exteriorLights().empty()) {
      // get the vector of exterior lights;
      std::vector<openstudio::model::ExteriorLights> ext_lights = facility.exteriorLights();
      // loop over the lights;
      for (const openstudio::model::ExteriorLights& exlight : ext_lights) {
        double ext_light_mult = exlight.multiplier();
        double ext_light_base = exlight.exteriorLightsDefinition().designLevel();
        exterior_lighting_power += ext_light_base * ext_light_mult;
      }
      LOG(Debug, "Found " << ext_lights.size() << " Exterior Lights");
    }
    LOG(Debug, "Exterior Lighting Power = " << exterior_lighting_power << " W");

    // check for luminaire definitions and print a warning if they are defined;
    std::vector<openstudio::model::Luminaire> luminaires = model.getConcreteModelObjects<openstudio::model::Luminaire>();
    if (!luminaires.empty()) {
      LOG(Debug, "Warning: Luminaires defined in OSM but not currently translated into ISO");
    }

    LOG(Debug, "...Calculating() Thermostats");
    LOG(Debug, "*****************************");
    LOG(Debug, "  Calculating Thermostats");
    LOG(Debug, "*****************************");

    double cooling_setpoint_occ = 0;
    double cooling_setpoint_unocc = 0;
    double heating_setpoint_occ = 0;
    double heating_setpoint_unocc = 0;

    if (OSM_extract_temp_setpoint) {
      // set the occupied and unoccupied temperature setpoint by finding average setpoint over the occupied and unoccupied times

      // set the occupied and unoccupied temperature setpoint by finding average setpoint over the occupied and unoccupied times;
      //std::vector<openstudio::model::ThermostatSetpointDualSetpoint> thermostats = model.getConcreteModelObjects<openstudio::model::ThermostatSetpointDualSetpoint>();

      double total_zone_area = 0;
      double heat_setpoint_occ_total = 0;
      double heat_setpoint_unocc_total = 0;
      double cool_setpoint_occ_total = 0;
      double cool_setpoint_unocc_total = 0;

      // loop through the zones and find the thermostats rather than use the thermostat vector we can get from the model;
      // we want to do a zone area weighted average in case the different zones have different;
      //loop through the zones;
      for (const openstudio::model::ThermalZone& zone : thermal_zones) {
        double zone_area = zone.floorArea() * zone.multiplier();
        total_zone_area += zone_area;

        if (zone.thermostatSetpointDualSetpoint()) {
          openstudio::model::ThermostatSetpointDualSetpoint thermostat = zone.thermostatSetpointDualSetpoint().get();
          if (thermostat.coolingSetpointTemperatureSchedule()) {
            openstudio::model::Schedule cool_sched = thermostat.coolingSetpointTemperatureSchedule().get();
            cool_setpoint_occ_total += occ_aves[cool_sched.name().get()] * zone_area;
            cool_setpoint_unocc_total += unocc_aves[cool_sched.name().get()] * zone_area;
          } else {
            // if we have no schedule, use the default values for thiz zone;
            cool_setpoint_occ_total += cooling_setpoint_occ_default * zone_area;
            cool_setpoint_unocc_total += cooling_setpoint_unocc_default * zone_area;
          }
          if (thermostat.heatingSetpointTemperatureSchedule()) {
            openstudio::model::Schedule heat_sched = thermostat.heatingSetpointTemperatureSchedule().get();
            heat_setpoint_occ_total += occ_aves[heat_sched.name().get()] * zone_area;
            heat_setpoint_unocc_total += unocc_aves[heat_sched.name().get()] * zone_area;
          } else {
            heat_setpoint_occ_total += heating_setpoint_occ_default * zone_area;
            heat_setpoint_unocc_total += heating_setpoint_unocc_default * zone_area;
          }
        } else {
          cool_setpoint_occ_total += cooling_setpoint_occ_default * zone_area;
          heat_setpoint_occ_total += heating_setpoint_occ_default * zone_area;
          cool_setpoint_unocc_total += cooling_setpoint_unocc_default * zone_area;
          heat_setpoint_unocc_total += heating_setpoint_unocc_default * zone_area;
        }
      }
      cooling_setpoint_occ = cool_setpoint_occ_total / total_zone_area;
      cooling_setpoint_unocc = cool_setpoint_unocc_total / total_zone_area;
      heating_setpoint_occ = heat_setpoint_occ_total / total_zone_area;
      heating_setpoint_unocc = heat_setpoint_unocc_total / total_zone_area;
    } else {
      cooling_setpoint_occ = cooling_setpoint_occ_default;
      cooling_setpoint_unocc = cooling_setpoint_unocc_default;
      heating_setpoint_occ = heating_setpoint_occ_default;
      heating_setpoint_unocc = heating_setpoint_unocc_default;
    }

    LOG(Debug, "Cooling Setpoint Occupied = " << cooling_setpoint_occ << " C, Cooling Setpoint Unoccupied = " << cooling_setpoint_unocc << " C");
    LOG(Debug, "Heating Setpoint Occupied = " << heating_setpoint_occ << " C, Heating Setpoint Unoccupied = " << heating_setpoint_unocc << " C");

    // try to find if >60% of floor area has daylight control;
    // loop through spaces, get area and check for any daylighting controls;
    double daylight_area = 0.0;
    double total_area = 0;

    for (const openstudio::model::Space& s : spaces) {
      // add up the total floor area;
      total_area += s.floorArea() * s.multiplier();
      std::vector<openstudio::model::DaylightingControl> daylight_ctrls = s.daylightingControls();
      // check for a daylighting control of any kind, assume it's turned on;
      if (daylight_ctrls.size() != 0) {
        // add up the total daylighting control area;
        daylight_area += s.floorArea() * s.multiplier();
      }
    }
    double frac_daylight = 0;

    if (total_area > 0) {
      frac_daylight = daylight_area / total_area;
    } else {
      frac_daylight = 0;
    }

    LOG(Debug, "Calculated " << frac_daylight << " of total area has daylighting controls");
    int daylight_sensors = 0;
    if (frac_daylight > 0.6) {
      // turn on daylighting controls in ISO model;
      daylight_sensors = 2;
    } else {
      // turn off daylighting controls in ISO model;
      daylight_sensors = 1;
    }

    LOG(Debug, "...Parsing() Geometry");

    LOG(Debug, "*****************************");
    LOG(Debug, "    Parsing Geometry");
    LOG(Debug, "*****************************");

    // get the overall floor area from the building object;
    // first see if conditioned floor area is set and use that if it is, otherwise use regular floor area;
    double floor_area = 0;
    if (building.conditionedFloorArea()) {
      floor_area = building.conditionedFloorArea().get();
    } else {
      floor_area = building.floorArea();
    }
    LOG(Debug, "Floor Area = " << floor_area << " m2");

    // Compute the building height as the difference from max and min vertex height;
    double maxz = -1000.0;
    double minz = 1000.0;
    for (const openstudio::model::Space& sp : spaces) {
      // loop through space surfaces to find max z value;
      for (const openstudio::model::Surface& s : sp.surfaces()) {
        std::vector<double> z_points;
        for (const openstudio::Point3d& vertex : s.vertices()) {
          z_points.push_back(vertex.z());
        }

        /// \todo shouldn't there be some test to make sure there was > 0 vertices?

        // find the min z for the surface and add the space origin offset;
        double minsz = *std::min_element(z_points.begin(), z_points.end()) + sp.zOrigin();
        // find the max z for the surface and add the space origin offset;
        double maxsz = *std::max_element(z_points.begin(), z_points.end()) + sp.zOrigin();
        if (maxsz > maxz) maxz = maxsz;
        if (minsz < minz) minz = minsz;
      }
    }

    double building_height = maxz - minz;

    LOG(Debug, "Building Height  = " << building_height << " m");

    // find the areas of wall, roofs, and windows;
    // create arrays for the walls and windows;
    static const std::string direction_names[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"};
    static const double dirLimit[] = {0, 45, 90, 135, 180, 225, 270, 315, 360};
    // generate an 8x1 array filled with zeros;
    std::vector<double> wall_areas(8, 0.0);
    std::vector<double> window_areas(8, 0.0);
    std::vector<double> wall_U(8, 0.0);
    std::vector<double> window_U(8, 0.0);
    std::vector<double> wall_solar_absorption(8, 0.0);
    std::vector<double> wall_thermal_emissivity(8, 0.0);
    std::vector<double> window_SHGC(8, 0.0);

    double wall_HC_sum = 0.0;
    std::vector<double> wall_U_area(8, 0.0);
    std::vector<double> wall_U_sum(8, 0.0);
    std::vector<double> wall_abs_sum(8, 0.0);
    std::vector<double> wall_emiss_sum(8, 0.0);
    std::vector<double> wall_abs_area(8, 0.0);

    double window_HC_sum = 0.0;
    std::vector<double> window_U_sum(8, 0.0);
    std::vector<double> window_U_area(8, 0.0);
    std::vector<double> window_SHGC_sum(8, 0.0);

    // extract outside walls and windows, and add up areas and add up weighted surface properties;
    LOG(Debug, "Found " << building.exteriorWalls().size() << " exterior wall surfaces");
    for (const openstudio::model::Surface& s : building.exteriorWalls()) {

      double absorption = const_solarAbsorptance[s.construction().get().name().get()];
      double emissivity = const_thermalAbsorptance[s.construction().get().name().get()];
      double heatcapacity = const_heatCapacity[s.construction().get().name().get()];

      // get the surface azimuth and convert from radians to degrees;
      double azimuth = s.azimuth() * 180 / 3.141592654;
      // rotate the coordinate system 22.5 degrees so N is 0 to 45 instead of -22.5 to 22.5;
      double az = azimuth + 22.5;
      if (az > 360) {
        // remap anything between 360 and 382.5 back to 0 to 22.5;
        az = az - 360;
      }
      for (int i = 0; i <= 7; ++i) {
        if ((az > dirLimit[i]) && (az <= dirLimit[i + 1])) {

          // add in the wall area without window;
          wall_areas[i] += s.netArea();
          wall_abs_sum[i] += absorption * s.netArea();
          wall_emiss_sum[i] += emissivity * s.netArea();
          wall_HC_sum += heatcapacity * s.netArea();

          double wallU = const_U[s.construction().get().name().get()];
          // if the U value is !NaN add in the U value and area for weighting;
          // keep a separate area sum for wall U in case there are some segments with undefined U values
          // we don't want them to be averaged in
#ifdef _MSC_VER
          if (!_isnan(wallU)) {
#else
          if (!std::isnan(wallU)) {
#endif
            wall_U_area[i] += s.netArea();
            wall_U_sum[i] += wallU * s.netArea();
          }
          std::vector<openstudio::model::SubSurface> subsurface = s.subSurfaces();
          for (const openstudio::model::SubSurface& ss : subsurface) {
            double windowU = const_U[ss.construction().get().name().get()];
            double windowSHGC = const_SHGC[ss.construction().get().name().get()];
#ifdef _MSC_VER
            if (!_isnan(windowU)) {
#else
            if (!std::isnan(windowU)) {
#endif
              window_U_area[i] += ss.surface().get().netArea();
              window_U_sum[i] += windowU * ss.surface().get().netArea();
              window_SHGC_sum[i] += windowSHGC * ss.surface().get().netArea();
            }
          }
          // get just the window area;
          window_areas[i] += s.grossArea() - s.netArea();

          // add in lines to  compute SCF, and SDF in each direction;
        }
      }
    }

    //  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC;
    //  If areas are zero then all values are set to zero
    for (int i = 0; i <= 7; ++i) {
      if (wall_areas[i] > 0.0) {
        wall_solar_absorption[i] = wall_abs_sum[i] / wall_areas[i];
        wall_thermal_emissivity[i] = wall_emiss_sum[i] / wall_areas[i];

      } else {
        wall_solar_absorption[i] = 0;
        wall_thermal_emissivity[i] = 0;
      }
      if (wall_U_area[i] > 0.0) {  // if any wall surfaces have a U value defined, use the weighted average of U values
        wall_U[i] = wall_U_sum[i] / wall_U_area[i];
      } else {
        wall_U[i] = 0;
      }
      if (window_areas[i] > 0.0) {
        window_U[i] = window_U_sum[i] / window_areas[i];
        window_SHGC[i] = window_SHGC_sum[i] / window_areas[i];
      } else {
        window_U[i] = 0;
        window_SHGC[i] = 0;
        window_SCF[i] = 0;

        /// \todo window_SDF seems to always be 0
        window_SDF[i] = 0;
      }
    }

    double total_wall_area = 0.0;
    for (double a : wall_areas) {
      total_wall_area += a;
    }

    double total_window_area = 0.0;
    for (double a : window_areas) {
      total_window_area += a;
    }

    LOG(Debug, "Total Wall Area = " << total_wall_area << " m2");
    LOG(Debug, "Total Window Area = " << total_window_area << " m2");

    // set the loop sums to 0 before the loop
    double roof_area = 0.0;
    double skylight_area = 0.0;

    double roof_abs_sum = 0.0;
    double roof_emiss_sum = 0.0;
    double roof_U_sum = 0.0;
    double roof_HC_sum = 0.0;
    double skylight_U_sum = 0.0;
    double skylight_SHGC_sum = 0.0;
    int roof_count = 0;

    // the building.roofs() variable does !seem to return the roof surfaces in a vector as it supposed to;
    // so we search through all surfaces and find the outside roof surfaces ourselves;
    // calculate area and average solar absorption and thermal emissivity;
    for (const openstudio::model::Surface& s : surfaces) {
      // skip surface if !labeled roof;
      if (s.surfaceType() != "RoofCeiling") {
        continue;
      }
      // skip surface if !outside;
      if (s.outsideBoundaryCondition() != "Outdoors") {
        continue;
      }
      roof_count += 1;
      double absorption = const_solarAbsorptance[s.construction().get().name().get()];
      double emissivity = const_thermalAbsorptance[s.construction().get().name().get()];
      double heatcapacity = const_heatCapacity[s.construction().get().name().get()];
      double roofU = const_U[s.construction().get().name().get()];

      roof_abs_sum += absorption * s.netArea();
      roof_emiss_sum += emissivity * s.netArea();
      roof_HC_sum += heatcapacity * s.netArea();
      roof_U_sum += roofU * s.netArea();

      // get the roof area without skylights;
      roof_area += s.netArea();
      // get the skylight area;
      skylight_area += s.grossArea() - s.netArea();

      std::vector<openstudio::model::SubSurface> subsurface = s.subSurfaces();
      for (const openstudio::model::SubSurface& ss : subsurface) {
        skylight_U_sum += const_U[ss.construction().get().name().get()] * ss.surface().get().netArea();
        skylight_SHGC_sum += const_SHGC[ss.construction().get().name().get()] * ss.surface().get().netArea();
      }
    }
    LOG(Debug, "Found " << roof_count << " roof surfaces");

    double roof_solar_absorption = 0;
    double roof_thermal_emissivity = 0;
    double roof_U = 0;

    if (roof_area > 0.0) {
      // compute roof absorption as area weighted average;
      roof_solar_absorption = roof_abs_sum / roof_area;
      // compute roof emissivity as an area weighted average;
      roof_thermal_emissivity = roof_emiss_sum / roof_area;
      // compute roof U as area weighted average;
      roof_U = roof_U_sum / roof_area;
    } else {
      roof_solar_absorption = 0;
      roof_thermal_emissivity = 0;
      roof_U = 0;
    }

    double skylight_U = 0;
    double skylight_SHGC = 0;

    if (skylight_area > 0.0) {
      skylight_U = skylight_U_sum / skylight_area;
      skylight_SHGC = skylight_SHGC_sum / skylight_area;
    } else {
      skylight_U = 0;
      skylight_SHGC = 0;
    }

    for (int i = 0; i <= 7; ++i) {
      LOG(Debug, "Wall" << direction_names[i] << ": Area=" << wall_areas[i] << " m2, U=" << wall_U[i]
                        << " W/m2/K, Absorption=" << wall_solar_absorption[i] << ", Emissivity=" << wall_thermal_emissivity[i]);
    }
    for (int i = 0; i <= 7; ++i) {
      LOG(Debug, "Window" << direction_names[i] << ": Area=" << window_areas[i] << " m2, U=" << window_U[i] << " W/m2/K, SHGC=" << window_SHGC[i]
                          << ", SCF=" << window_SCF[i] << ", SDF=" << window_SDF[i]);
    }

    LOG(Debug, "Roof: Area=" << roof_area << " m2, U=" << roof_U << " W/m2/K, Absorption=" << roof_solar_absorption
                             << ", Emissivity=" << roof_thermal_emissivity);
    LOG(Debug, "Skylight: Area=" << skylight_area << " m2, U=" << skylight_U << " W/m2/K, SHGC=" << skylight_SHGC);

    // compute exterior heat capacity;
    double exterior_heat_capacity = (roof_HC_sum + wall_HC_sum + window_HC_sum) / (total_wall_area + roof_area + total_window_area);
    LOG(Debug, "Exterior Heat Capacity = " << exterior_heat_capacity << " J/K/m2 based on surface area");

    // add up the heat capacity of the defined interior surfaces including any internal mass elements;
    // compute as normalized to floor area and !surface area;
    double interior_HC_sum = 0.0;
    for (const openstudio::model::Surface& s : surfaces) {
      // skip surface if outside;
      if (s.outsideBoundaryCondition() == "Outdoors") {
        continue;
      }
      interior_HC_sum += const_heatCapacity[s.construction().get().name().get()] * s.netArea();
    }

    // get and add in the heat capacity of any defined internal masses;
    double internal_mass_HC_sum = 0.0;
    for (const openstudio::model::InternalMassDefinition& mass : internal_masses) {
      internal_mass_HC_sum += mass.surfaceArea().get() * const_heatCapacity[mass.construction().get().name().get()];
    }
    double interior_heat_capacity = (interior_HC_sum + internal_mass_HC_sum) / building.floorArea();
    LOG(Debug, "Interior  Heat Capacity = " << interior_heat_capacity << " J/K/m2 based on floor area");

    LOG(Debug, "...Calculating() Infiltration");
    LOG(Debug, "*****************************");
    LOG(Debug, "   Calculating Infiltration");
    LOG(Debug, "*****************************");

    double infiltration_rate = 0;

    // compute infiltration;

    if (OSM_extract_infil_rate) {
      //first check to see if there effective leakage areas defined and if !those, then design flow rates;
      if (!model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationEffectiveLeakageArea>().empty()) {
        //std::vector<openstudio::model::SpaceInfiltrationEffectiveLeakageArea> infiltration = model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationEffectiveLeakageArea>();

        /// \todo no reason for this to be in a loop
        //      for (const openstudio::model::SpaceInfiltrationEffectiveLeakageArea &infil : infiltration) {
        // set default average envelope air leakage (infiltration) as 7 m3/h/m2 which is about the EnergyPlus defaults;
        LOG(Debug, "EffectiveLeakageArea !Implemented Yet, Infiltration Rate Set to 7.0 m3/m2/h @ 75 Pa");
        infiltration_rate = infil_rate_default;
        //      }

      } else if (!model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationDesignFlowRate>().empty()) {
        std::vector<openstudio::model::SpaceInfiltrationDesignFlowRate> infiltration =
          model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationDesignFlowRate>();
        LOG(Debug, "Found " << infiltration.size() << " SpaceInfiltrationDesignFlowRate objects");

        /// \todo this is not used
        //double infil_frac_sum = 0.0;
        double infil_rate_sum = 0.0;
        int count = 0;
        for (const openstudio::model::SpaceInfiltrationDesignFlowRate& infil : infiltration) {
          count += 1;
          std::string rate_type = infil.designFlowRateCalculationMethod();
          double infil_rate_temp = 0;

          if (infil.space()) {
            LOG(Debug, "Space defined for Infiltration object #" << count);
            if (rate_type == "Flow/Space") {
              // add in the design flow rate per space * number of spaces;
              infil_rate_temp = infil.designFlowRate().get();
              LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "Flow/ExteriorArea" || rate_type == "Flow/ExteriorWallArea") {
              // add in the flow/exterior area  * the exterior area;
              infil_rate_temp = infil.flowperExteriorSurfaceArea().get() * infil.space()->exteriorArea();
              LOG(Debug, "Infiltration Object #" << count << " is Flow/ExteriorArea, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "Flow/Area") {
              infil_rate_temp = infil.flowperSpaceFloorArea().get() * infil.space()->floorArea();
              LOG(Debug, "Infiltration Object #" << count << " is Flow/Area, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "AirChanges/Hour") {
              infil_rate_temp = 0;
              LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, Not implemented yet so nothing added");
            } else {
              infil_rate_temp = 0;
              LOG(Debug, "Infiltration Ojbect #" << count << " has no calc method defined, so nothing added");
            }
          } else if (infil.spaceType()) {
            LOG(Debug, "No space defined in Infiltration object #" << count << ", using spacetype object instead");
            openstudio::model::SpaceType st = infil.spaceType().get();
            double st_volume = 0.0;
            double st_exterior_area = 0.0;
            /// \todo this is not used
            //int st_num = st.spaces().size();
            for (const openstudio::model::Space& s : st.spaces()) {
              st_volume += s.volume();
              st_exterior_area += s.exteriorArea();
            }
            LOG(Debug, "Found " << st.spaces().size() << " spaces in spacetype for Infiltration object " << count);
            LOG(Debug, "Infiltration design flow rate found of type " << rate_type);

            if (rate_type == "Flow/Space") {
              // add in the design flow rate per space * number of spaces;
              infil_rate_temp = infil.designFlowRate().get() * st.spaces().size();
              LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "Flow/ExteriorArea" || rate_type == "Flow/ExteriorWallArea") {
              // add in the flow/exterior area  * the exterior area;
              infil_rate_temp = infil.flowperExteriorSurfaceArea().get() * st_exterior_area;
              LOG(Debug, "Infiltration Object #" << count << " is Flow/ExteriorArea, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "Flow/Area") {
              // add in the flow/floor area * floor area;
              infil_rate_temp = infil.flowperSpaceFloorArea().get() * st.floorArea();
              LOG(Debug, "Infiltration Object #" << count << " is Flow/Area, " << infil_rate_temp << " m3/s added");
            } else if (rate_type == "AirChanges/Hour") {
              infil_rate_temp = infil.flowperSpaceFloorArea().get() * st.floorArea();
              LOG(Debug, "Infiltration Object #" << count << " is AirChanges/Hour, " << infil_rate_temp << " m3/s added");
            } else {
              infil_rate_temp = infil.airChangesperHour().get() * st_volume;
              LOG(Debug, "Infiltration Ojbect #" << count << " has no calc method defined, so nothing added");
            }
          } else {
            LOG(Debug, "No space or spacetype defined for Infiltration object #" << count << ", skipping analysis");
          }

          double infil_frac = 0;

          // now look for the schedule for the infil object and add up the fraction;
          if (infil.schedule()) {
            std::string sched = infil.schedule().get().name().get();
            // if a schedule exists, get the average yearly fraction by a time weighted average of occupied and unoccupied averages;
            infil_frac = occ_aves[sched] * frac_year_occ + unocc_aves[sched] * frac_year_unocc;
            LOG(Debug, "Infiltration schedule found for Infiltration object #" << count << ", scaling by " << infil_frac);
          } else {
            LOG(Debug, "No Infiltration schedule found for Infiltration object #" << count << ", assuming always on");
            infil_frac = 1.0;
          }
          infil_rate_sum += infil_rate_temp * infil_frac;
        }
        LOG(Debug, "Total Infiltration = " << infil_rate_sum << " m3/s at 4 Pa (natural pressure difference)");

        // get avg infiltration rate and convert to from m3/m2/min @ 4 Pa to m3/m2/h @ 75 Pa with a .67 pressure exponent;
        // assume constant infilration rate is based on a nominal 4 Pa pressure difference;
        infiltration_rate = infil_rate_sum / (total_wall_area + roof_area) * 3600 * ::pow((75 / 4), 0.67);
      } else {
        LOG(Debug, "No Infiltration Design Flow Rates Found");
        infiltration_rate = infil_rate_default;
      }
    } else {
      LOG(Debug, "No Infiltration Extraction from OSM File");
      infiltration_rate = infil_rate_default;
    }

    LOG(Debug, "Infiltration rate set to " << infiltration_rate << " m3/h/m2 @ 75 Pa");

    LOG(Debug, "...Parsing() HVAC Info");
    LOG(Debug, "*****************************");
    LOG(Debug, "   Parsing HVAC Info");
    LOG(Debug, "*****************************");

    LOG(Debug, "*****************************");
    LOG(Debug, "  Analyzing HVAC");
    LOG(Debug, "*****************************");
    LOG(Debug, "...Analyzing() HVAC");

    double cooling_COP = 0;
    double heating_system_efficiency = 0;
    int heating_fuel_type = 0;

    size_t num_ideal_air_loads = 0;
    for (const openstudio::model::ThermalZone& z : thermal_zones) {
      if (z.useIdealAirLoads()) ++num_ideal_air_loads;
    }

    bool uses_ideal_air_loads = false;

    if (num_ideal_air_loads > 0 && num_ideal_air_loads != thermal_zones.size()) {
      throw std::runtime_error("cannot translate model, only some zones use ideal air loads");
    } else if (num_ideal_air_loads == thermal_zones.size()) {
      uses_ideal_air_loads = true;
    }

    if (OSM_extract_HVAC && !uses_ideal_air_loads) {
      std::vector<openstudio::model::ModelObject> hvac_component_array;
      std::vector<double> hvac_component_area_array;

      std::vector<openstudio::model::PlantLoop> plant_loops_array;
      std::vector<double> plant_loops_area_array;

      // look through the air loops and extract the components;
      // for components that can connect to a plant water loop (namely things with water heating and cooling coils);
      for (openstudio::model::AirLoopHVAC& air_loop : air_loops) {
        std::vector<openstudio::model::ModelObject> supply_components = air_loop.supplyComponents();

        double zone_area = 0.0;
        // get the total area of all the zones connected through the loop;
        for (const openstudio::model::ThermalZone& z : air_loop.thermalZones()) {
          zone_area += z.floorArea();
        }

        // loop through the components and store them all in a single array;
        // if;
        for (const openstudio::model::ModelObject& component : supply_components) {
          // skip if the component is a node;
          if (component.optionalCast<openstudio::model::Node>()) continue;
          // skip of component is a mixer connector;
          if (component.optionalCast<openstudio::model::ConnectorMixer>()) continue;
          // skip if component is a splitter;
          if (component.optionalCast<openstudio::model::ConnectorSplitter>()) continue;
          // skip if component is a pipe;
          if (component.optionalCast<openstudio::model::PipeAdiabatic>()) continue;

          // if the component is a water cooling coil, do !save, but instead save the attached plant loop;
          if ((component
                 .optionalCast<openstudio::model::CoilCoolingWater>())) {  // if the component is a cooling coil, don't save the attached plant loop;
            plant_loops_array.push_back(component.optionalCast<openstudio::model::CoilCoolingWater>().get().plantLoop().get());
            plant_loops_area_array.push_back(zone_area);
            // if the component is a water cooling coil, do !save but instead save the attached plant loop;
          } else if (component.optionalCast<openstudio::model::CoilHeatingWater>()) {
            // if the component is a heating coil, don't save and search the attached plant loop;
            //std::vector<openstudio::model::ModelObject> plant_loop_components = component.optionalCast<openstudio::model::CoilHeatingWater>().get().plantLoop().get().supplyComponents();
            // get the cooling coil plant loop;
            plant_loops_array.push_back(component.optionalCast<openstudio::model::CoilHeatingWater>().get().plantLoop().get());
            plant_loops_area_array.push_back(zone_area);
          } else {
            // otherwise this component is !hooked to a water loop so save it for later parsing;
            hvac_component_array.push_back(component);
            hvac_component_area_array.push_back(zone_area);
          }
        }
      }

      for (size_t i = 0; i < plant_loops_array.size(); ++i) {
        std::vector<openstudio::model::ModelObject> supply_components = plant_loops_array[i].supplyComponents();
        double zone_area = plant_loops_area_array[i];

        for (const openstudio::model::ModelObject& component : supply_components) {
          // skip if the component is a node;
          if (component.optionalCast<openstudio::model::Node>()) continue;
          // skip of component is a mixer connector;
          if (component.optionalCast<openstudio::model::ConnectorMixer>()) continue;
          // skip if component is a splitter;
          if (component.optionalCast<openstudio::model::ConnectorSplitter>()) continue;
          // skip if component is a pipe;
          if (component.optionalCast<openstudio::model::PipeAdiabatic>()) continue;

          hvac_component_array.push_back(component);
          hvac_component_area_array.push_back(zone_area);
        }
      }

      // now add in any HVAC components !part of an air loop (PTAC, PTHP, unit heaters, etc);
      for (openstudio::model::ThermalZone& zone : thermal_zones) {
        for (const openstudio::model::ModelObject& component : zone.equipment()) {
          hvac_component_array.push_back(component);
          hvac_component_area_array.push_back(zone.floorArea());
        }
      }

      std::vector<openstudio::model::HVACComponent> cooling_coil_array;
      std::vector<double> cooling_coil_area_array;
      std::vector<openstudio::model::HVACComponent> heating_coil_array;
      std::vector<double> heating_coil_area_array;
      for (size_t i = 0; i < hvac_component_array.size(); ++i) {
        openstudio::model::ModelObject component = hvac_component_array[i];
        double area = hvac_component_area_array[i];

        // first check for PTAC;
        if (component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalAirConditioner>()) {
          openstudio::model::ZoneHVACPackagedTerminalAirConditioner ptac =
            component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalAirConditioner>().get();
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalAirConditioner>().get().coolingCoil());
          cooling_coil_area_array.push_back(area);
          heating_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalAirConditioner>().get().heatingCoil());
          heating_coil_area_array.push_back(area);
          // next check for PTHP;
        } else if (component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalHeatPump>()) {
          openstudio::model::ZoneHVACPackagedTerminalHeatPump pthp =
            component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalHeatPump>().get();
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalHeatPump>().get().coolingCoil());
          cooling_coil_area_array.push_back(area);
          heating_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACPackagedTerminalHeatPump>().get().heatingCoil());
          heating_coil_area_array.push_back(area);
          // next check for water to air heat pump;
        } else if (component.optionalCast<openstudio::model::ZoneHVACWaterToAirHeatPump>()) {
          openstudio::model::ZoneHVACWaterToAirHeatPump wahp = component.optionalCast<openstudio::model::ZoneHVACWaterToAirHeatPump>().get();
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACWaterToAirHeatPump>().get().coolingCoil());
          cooling_coil_area_array.push_back(area);
          heating_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACWaterToAirHeatPump>().get().heatingCoil());
          heating_coil_area_array.push_back(area);
          // next check for a component Heater;
        } else if (component.optionalCast<openstudio::model::ZoneHVACUnitHeater>()) {
          heating_coil_array.push_back(component.optionalCast<openstudio::model::ZoneHVACUnitHeater>().get().heatingCoil());
          heating_coil_area_array.push_back(area);
          //check for unitary air-to-air heat pump;
        } else if (component.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>()) {
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>().get().coolingCoil());
          cooling_coil_area_array.push_back(area);
          heating_coil_array.push_back(component.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>().get().heatingCoil());
          heating_coil_area_array.push_back(area);
          // next check for the cooling and heating coils themselves and save them to the lists;
        } else if (component.optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>()) {
          // check for single speed DX coil;
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          cooling_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::CoilCoolingDXTwoSpeed>()) {
          // check for a two speed DX coil;
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          cooling_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::CoilCoolingWater>()) {
          // check for plain cooling water coil;
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          cooling_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::ChillerElectricEIR>()) {
          // check for electric chiller;
          cooling_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          cooling_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::CoilHeatingWater>()) {
          // check for plain heating water coil;
          heating_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          heating_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::CoilHeatingElectric>()) {
          heating_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          heating_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::CoilHeatingGas>()) {
          heating_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          heating_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::BoilerHotWater>()) {
          heating_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          heating_coil_area_array.push_back(area);
        } else if (component.optionalCast<openstudio::model::BoilerSteam>()) {
          heating_coil_array.push_back(component.optionalCast<openstudio::model::HVACComponent>().get());
          heating_coil_area_array.push_back(area);
        }
      }

      LOG(Debug, "Found " << cooling_coil_array.size() << " cooling elements");
      LOG(Debug, "Found " << heating_coil_array.size() << " heating elements");

      // go through the cooling coil array list and add up the COP*zone area and zone area to compute zonearea weighted COP;
      double cop_sum = 0.0;
      double cop_area_sum = 0.0;

      for (size_t i = 0; i < cooling_coil_array.size(); ++i) {
        openstudio::model::HVACComponent coil = cooling_coil_array[i];
        double area = cooling_coil_area_array[i];
        if (coil.optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>()) {
          // check for single speed DX coil;
          cop_sum += coil.optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>().get().ratedCOP() * area;
          cop_area_sum += area;
        } else if (coil.optionalCast<openstudio::model::CoilCoolingDXTwoSpeed>()) {
          // check for two speed DX coil;
          cop_sum += coil.optionalCast<openstudio::model::CoilCoolingDXTwoSpeed>().get().ratedHighSpeedCOP() * area;
          cop_area_sum += area;
        } else if (coil.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>()) {
          // check for heat pump;
          cop_sum += coil.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>()
                       .get()
                       .coolingCoil()
                       .optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>()
                       .get()
                       .ratedCOP()
                     * area;
          cop_area_sum += area;
        } else if (coil.optionalCast<openstudio::model::ChillerElectricEIR>()) {
          cop_sum += coil.optionalCast<openstudio::model::ChillerElectricEIR>().get().referenceCOP() * area;

          cop_area_sum += area;
        }
      }

      // if the COP sum is 0 but the cooling coil array is non-zero, check the plant loop for a chiller;
      if ((cop_sum == 0.0) && (cooling_coil_array.size() > 0)) {
        LOG(Debug, "checking plant loop for chiller");

        for (openstudio::model::PlantLoop& plant_loop : plant_loops) {
          std::vector<openstudio::model::ModelObject> supply_components = plant_loop.supplyComponents();
          // loop through the components and get the efficiency;
          double area = building.floorArea();
          for (const openstudio::model::ModelObject& component : supply_components) {
            // if we are here then we don't have a specific area assigned to the cooling unit so use the full building floor area;
            if (component.optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>()) {
              // check for single speed DX coil;
              cop_sum += component.optionalCast<openstudio::model::CoilCoolingDXSingleSpeed>().get().ratedCOP();
              cop_area_sum += building.floorArea();
            } else if (component.optionalCast<openstudio::model::CoilCoolingDXTwoSpeed>()) {
              // check for two speed DX coil;
              cop_sum += component.optionalCast<openstudio::model::CoilCoolingDXTwoSpeed>().get().ratedHighSpeedCOP();
              cop_area_sum += area;
            } else if (component.optionalCast<openstudio::model::ChillerElectricEIR>()) {
              // check for a chiller;
              /// \todo coil must be wrong here, there's no coil in scope, changing to component
              cop_sum += component.optionalCast<openstudio::model::ChillerElectricEIR>().get().referenceCOP() * area;
              cop_area_sum += area;
            }
          }
        }
      }

      if (cop_area_sum > 0.0) {
        cooling_COP = cop_sum / cop_area_sum;
        LOG(Debug, "Area Weighted Average Cooling COP = " << cooling_COP);
      } else {
        cooling_COP = 1E+12;  // set cooling COP to huge number so cooling energy is zero if cooling was found
        LOG(Debug, "No Cooling Equipment Found, set COP = " << cooling_COP);
      }

      // go through the heating coil array list and add up the COP*zone area for gas and electic heating coils;
      double elec_sum = 0.0;
      double elec_area_sum = 0.0;
      double gas_sum = 0.0;
      double gas_area_sum = 0.0;

      for (size_t i = 0; i < heating_coil_array.size(); ++i) {
        openstudio::model::HVACComponent coil = heating_coil_array[i];
        double area = heating_coil_area_array[i];

        // check for gas heating coil;
        if (coil.optionalCast<openstudio::model::CoilHeatingGas>()) {
          gas_sum += coil.optionalCast<openstudio::model::CoilHeatingGas>().get().gasBurnerEfficiency() * area;
          gas_area_sum += area;
          // check for electric heating coil;
        } else if (coil.optionalCast<openstudio::model::CoilHeatingElectric>()) {
          elec_sum += coil.optionalCast<openstudio::model::CoilHeatingElectric>().get().efficiency() * area;
          elec_area_sum += area;
          // check for single speed DX heating coil;
        } else if (coil.optionalCast<openstudio::model::CoilHeatingDXSingleSpeed>()) {
          elec_sum += coil.optionalCast<openstudio::model::CoilHeatingDXSingleSpeed>().get().ratedCOP() * area;
          elec_area_sum += area;
          // check for unitary heat pump, extract coil and get;
        } else if (coil.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>()) {
          elec_sum += coil.optionalCast<openstudio::model::AirLoopHVACUnitaryHeatPumpAirToAir>()
                        .get()
                        .heatingCoil()
                        .optionalCast<openstudio::model::CoilHeatingDXSingleSpeed>()
                        .get()
                        .ratedCOP()
                      * area;
          elec_area_sum += area;
        } else if (coil.optionalCast<openstudio::model::BoilerHotWater>()) {
          openstudio::model::BoilerHotWater boiler = coil.optionalCast<openstudio::model::BoilerHotWater>().get();
          if (boiler.fuelType() == "NaturalGas") {
            gas_sum += boiler.nominalThermalEfficiency() * area;
            gas_area_sum += area;
          } else {
            elec_sum += boiler.nominalThermalEfficiency() * area;
            elec_area_sum += area;
          }
        }
        //  else if (!coil)
      }

      // if there seems to be no heating energy used but there are heating elements detected, check the plant loop for a boiler;
      if (((gas_sum + elec_sum) == 0.0) && (heating_coil_array.size() > 0)) {
        for (openstudio::model::PlantLoop& plant_loop : plant_loops) {
          std::vector<openstudio::model::ModelObject> supply_components = plant_loop.supplyComponents();
          // loop through the components and get the efficiency;
          double area = building.floorArea();
          for (const openstudio::model::ModelObject& component : supply_components) {
            // check for a hot water boiler;
            if (component.optionalCast<openstudio::model::BoilerHotWater>()) {
              openstudio::model::BoilerHotWater boiler = component.optionalCast<openstudio::model::BoilerHotWater>().get();
              if (boiler.fuelType() == "NaturalGas") {
                gas_sum += boiler.nominalThermalEfficiency() * area;
                gas_area_sum += area;
              } else {
                elec_sum += boiler.nominalThermalEfficiency() * area;
                elec_area_sum += area;
              }
            }
          }
        }
      }

      if (gas_sum > elec_sum) {
        heating_fuel_type = 2;
        // set heating fuel type to gas if heating_gas_area is larger than electric area;
        LOG(Debug, "Fuel Type Determined to be Mostly or All Gas, HVAC Fuel Type = 2");
      } else {
        LOG(Debug, "Fuel Type Determined to be Mostly or All Electric, HVAC Fuel Type = 1");
        heating_fuel_type = 1;
      }

      if ((gas_sum + elec_sum) > 0.0) {
        heating_system_efficiency = (elec_sum + gas_sum) / (elec_area_sum + gas_area_sum);
        LOG(Debug, "Average Heating Efficiency = " << heating_system_efficiency);
      } else {
        LOG(Debug, "No heating elements found - using HVAC defaults");
        heating_system_efficiency = heating_system_efficiency_default;
        heating_fuel_type = heating_fuel_type_default;
      }
    } else if (uses_ideal_air_loads) {
      LOG(Warn, "Using ideal airloads, which should use district heating and cooling, but we don't have that option yet");
      cooling_COP = 1.0;
      heating_system_efficiency = 1.0;
      heating_fuel_type = 2;  // gas
                              //      hvac_type = 0; // perfect efficiencies
    } else {
      cooling_COP = cooling_COP_default;
      heating_system_efficiency = heating_system_efficiency_default;
      heating_fuel_type = heating_fuel_type_default;
    }

    // set up the HVAC loss tables;
    // hvac type     0    1    2    3    4    5    6    7    8    9;
    //          10    11    12    13    14    15    16    17    18    19;
    //          20    21    22    23    24    25    26    27    28    29;
    //          30    31    32    33    34    35    36    37    38    39;
    double hvac_waste_table[] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.03, 0.00, 0.00, 0.04, 0.00, 0.03, 0.00, 0.00, 0.08, 0.03, 0.00, 0.00, 0.03, 0.00,
                                 0.00, 0.04, 0.00, 0.03, 0.00, 0.00, 0.04, 0.00, 0.00, 0.08, 0.04, 0.00, 0.00, 0.04, 0.00, 0.04, 0.00, 0.00};

    double hvac_hot_table[] = {0.00, 0.08, 0.25, 0.00, 0.36, 0.08, 0.08, 0.25, 0.00, 0.36, 0.08, 0.08, 0.25, 0.00, 0.08, 0.08, 0.25, 0.08, 0.08,
                               0.25, 0.00, 0.36, 0.08, 0.08, 0.25, 0.08, 0.00, 0.00, 0.00, 0.08, 0.00, 0.00, 0.08, 0.00, 0.08, 0.00, 0.00};

    double hvac_cold_table[] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.01, 0.00, 0.00, 0.00, 0.00, 0.01, 0.00, 0.00, 0.01, 0.01, 0.00, 0.00, 0.01, 0.00,
                                0.00, 0.00, 0.00, 0.01, 0.00, 0.00, 0.01, 0.00, 0.00, 0.01, 0.01, 0.00, 0.00, 0.01, 0.00, 0.01, 0.00, 0.00};

    //extract losses from loss tables based on hvac type;
    double hvac_waste_factor = hvac_waste_table[hvac_type];
    double hvac_heating_loss_factor = hvac_hot_table[hvac_type];
    double hvac_cooling_loss_factor = hvac_cold_table[hvac_type];

    LOG(Debug, "#HVAC type was " << hvac_type);
    LOG(Debug, "HVAC Waste Factor = " << hvac_waste_factor);
    LOG(Debug, "HVAC Heating Loss Factor = " << hvac_heating_loss_factor);
    LOG(Debug, "HVAC Cooling Loss Factor = " << hvac_cooling_loss_factor);

    // print out efficiencies;
    LOG(Debug, "Cooling System COP = " << cooling_COP);
    LOG(Debug, "Cooling System IPLV/COP Ratio =" << cooling_IPLVToCop_ratio);
    // heating energy, 1=electric, 0=gas);
    LOG(Debug, "Heating Fuel Type = " << heating_fuel_type);
    LOG(Debug, "Heating System Efficiency = " << heating_system_efficiency);

    // calculate fresh air ventilation rates;
    double freshair_flow_rate = 0.0;
    if (OSM_extract_vent_rate) {
      for (const openstudio::model::Space& s : spaces) {
        double space_air_rate = 0;
        if (s.designSpecificationOutdoorAir()) {
          openstudio::model::DesignSpecificationOutdoorAir outdoor_air = s.designSpecificationOutdoorAir().get();

          // these methods will return 0.0 if nothing is defined;
          // get ACH and convert to L/s;
          double air_ach = outdoor_air.outdoorAirFlowAirChangesperHour() * s.volume() / 3.6;
          // get air flow rate in L/s;
          double air_rate = outdoor_air.outdoorAirFlowRate() * 1000;
          // get in m3/s/person and convert to L/s;
          double air_per_person = outdoor_air.outdoorAirFlowperPerson() * s.numberOfPeople() * 1000;
          // get in m3/s/m2 and convert to L/s;
          double air_per_area = outdoor_air.outdoorAirFlowperFloorArea() * s.floorArea() * 1000;

          double sched_mult = 0;
          if (outdoor_air.outdoorAirFlowRateFractionSchedule()) {
            std::string sched = outdoor_air.outdoorAirFlowRateFractionSchedule().get().name().get();
            sched_mult = occ_aves[sched] * frac_year_occ + unocc_aves[sched] * frac_year_unocc;
          } else {
            // if no schedule is defined set multiplier to 1.0;
            sched_mult = 1.0;
          }

          if (outdoor_air.outdoorAirMethod() == "Maximum") {
            std::vector<double> vals;
            vals.push_back(air_per_person);
            vals.push_back(air_per_area);
            vals.push_back(air_rate);
            vals.push_back(air_ach);
            space_air_rate = (*std::max_element(vals.begin(), vals.end())) * s.multiplier() * sched_mult;
          } else {
            // if maximum is !selected, assume sum is selected;
            space_air_rate = (air_per_person + air_per_area + air_rate + air_ach) * s.multiplier() * sched_mult;
          }

        } else {
          // if no outdoor air rate is defined for space, default to 20 cfm/nominal person and convert to L/S from cfm;
          LOG(Debug, " No outdoor air rate define for space " << s.name() << " , default of 20cm/nominal person used");
          space_air_rate = 20 * s.numberOfPeople() / 2.12;
        }

        freshair_flow_rate += space_air_rate;
      }
    } else {
      freshair_flow_rate = number_of_people * vent_rate_per_person_default;
    }

    double supply_exhaust_rate = freshair_flow_rate;
    // set exhaust = supply rate as default (i.e(). no pressurization);
    LOG(Debug, "Total ventilation fresh air flow rate = " << freshair_flow_rate << "L/s");
    LOG(Debug, "No exhaust rate extraction - set air exhaust rate to ventilation rate");

    double dhw_system_efficiency = 0;
    double dhw_demand = 0.0;
    int dhw_fuel_type = 0;

    if (OSM_extract_dhw) {
      // Try to find DHW water components and get the water use;

      // search plant loops for water heaters;
      std::vector<double> dhw_gas_array;
      std::vector<double> dhw_elec_array;
      for (openstudio::model::PlantLoop& plant_loop : plant_loops) {
        std::vector<openstudio::model::ModelObject> supply_components = plant_loop.supplyComponents();
        // loop through the components and find the components that are water heaters;
        for (const openstudio::model::ModelObject& component : supply_components) {
          // enter here if the item is a water heater;
          if (component.optionalCast<openstudio::model::WaterHeaterMixed>()) {
            // convert component to water heater type;
            openstudio::model::WaterHeaterMixed heater = component.optionalCast<openstudio::model::WaterHeaterMixed>().get();
            // check to see if the heater is gas or electric;
            if (heater.heaterFuelType() == "NaturalGas") {
              // store the efficiency;
              dhw_gas_array.push_back(heater.heaterThermalEfficiency().get());
            } else {
              // store the efficiency;
              dhw_elec_array.push_back(heater.heaterThermalEfficiency().get());
            }
          }
        }
      }

      // add up all the uses from the individual components;
      double dhw_gas_sum = 0.0;
      for (double gas : dhw_gas_array) {
        dhw_gas_sum += gas;
      }
      double dhw_elec_sum = 0.0;
      for (double elec : dhw_elec_array) {
        dhw_elec_sum += elec;
      }

      // find the average efficiency of water heaters;
      size_t n_dhw_units = dhw_gas_array.size() + dhw_elec_array.size();

      // if the number of water heaters is > 0;
      if (n_dhw_units > 0) {
        dhw_system_efficiency = (dhw_gas_sum + dhw_elec_sum) / (dhw_gas_array.size() + dhw_elec_array.size());
      } else {
        dhw_system_efficiency = 1.0;
      }

      // if there are more gas DHW water heaters, fuel type is gas;
      if (dhw_gas_sum > dhw_elec_sum) {
        dhw_fuel_type = 2;
      } else {
        dhw_fuel_type = 1;
      }

      // extract the water usage by starting with the water use connections and then finding equipment;
      std::vector<openstudio::model::WaterUseConnections> water_connects = model.getConcreteModelObjects<openstudio::model::WaterUseConnections>();
      for (const openstudio::model::WaterUseConnections& connect : water_connects) {
        std::vector<openstudio::model::WaterUseEquipment> equipments = connect.waterUseEquipment();
        for (const openstudio::model::WaterUseEquipment& equip : equipments) {
          std::string frac_sched;
          double mult;
          if (equip.flowRateFractionSchedule()) {
            // get multiplier by doing a weighted average of occupied and unoccupied times;
            std::string frac_sched = equip.flowRateFractionSchedule().get().name().get();
            mult = (occ_aves[frac_sched] * frac_year_occ) + (unocc_aves[frac_sched] * frac_year_unocc);
          } else {
            mult = 1.0;
          }
          // get peak value and convert from m3/s to m3/yr;
          double peak = equip.waterUseEquipmentDefinition().peakFlowRate() * 3600 * 8760;
          LOG(Debug, "DHW: " << frac_sched << ", peak use = " << peak << " m3/yr, mult = " << mult);
          dhw_demand += peak * mult;
        }
      }

      if ((dhw_gas_sum + dhw_elec_sum) == 0.0) {
        // if the gas + electric sum = 0 then there were no DHW heaters, so set demand to 0 no matter what;
        dhw_demand = 0.0;
      }
    } else {
      dhw_fuel_type = dhw_fuel_type_default;
      dhw_system_efficiency = dhw_system_efficiency_default;
      // demand is # of people * # days * dhw rate/person/day in L/person/day /1000 to get m3/year
      dhw_demand = number_of_people * dhw_rate_per_person_default * number_days_occupied_per_year / 1000;
    }

    // use DHW distribution type to set DHW distribution efficiency;
    // create lookup table;
    double dhw_dist_eff_table[] = {1.0, 0.8, 0.6};
    double dhw_distribution_efficiency = dhw_dist_eff_table[dhw_dist_type];

    // print out the DHW fuel type, system efficiency, distribution efficiency;
    LOG(Debug, "DHW: Fuel Type = " << dhw_fuel_type << ", System Efficiency = " << dhw_system_efficiency
                                   << " Distribution Efficiency = " << dhw_distribution_efficiency << ", Demand = " << dhw_demand << " m3/yr");

    // create output file and start writing out;

    // get the current system clock time;
    openstudio::Time time;
    //    File.open(osm2iso_output_file, 'w') do |file|;

    //    file.LOG(Debug, "#" << osm2iso_output_file << " generated by osm2iso.rb() on " << input_file << " on " << time.ctime());

    openstudio::isomodel::UserModel userModel = openstudio::isomodel::UserModel();

    //userModel.setWeatherFilePath(weather_file_path);

    //    file.LOG(Debug, "# Terrain class urban/city = 0.8, suburban/some shielding = 0.9, country/open = 1.0");
    userModel.setTerrainClass(terrain_class);
    //    file.LOG(Debug, "# building height is in m,  floor area is in m^2, people density is m2/person");
    userModel.setBuildingHeight(building_height);
    userModel.setFloorArea(floor_area);
    userModel.setPeopleDensityOccupied(area_per_person_occupied);
    userModel.setPeopleDensityUnoccupied(area_per_person_unoccupied);

    userModel.setBuildingOccupancyFrom(occupancy_day_start);
    userModel.setBuildingOccupancyTo(occupancy_day_end);
    userModel.setEquivFullLoadOccupancyFrom(occupancy_hour_start);
    userModel.setEquivFullLoadOccupancyTo(occupancy_hour_end);

    //    file.LOG(Debug, "# LPD, EPD, GPD all in W/m2, ext light power in W, an heat gain is W/person");
    userModel.setLightingPowerIntensityOccupied(lpd_occupied);
    userModel.setLightingPowerIntensityUnoccupied(lpd_unoccupied);
    userModel.setElecPowerAppliancesOccupied(epd_occupied);
    userModel.setElecPowerAppliancesUnoccupied(epd_unoccupied);
    userModel.setGasPowerAppliancesOccupied(gpd_occupied);
    userModel.setGasPowerAppliancesUnoccupied(gpd_unoccupied);
    userModel.setExteriorLightingPower(exterior_lighting_power);
    userModel.setHeatGainPerPerson(activity_level_occ);
    // when we implement occupied and unoccupied heat gain per person, we comment above and uncomment below;
    //userModel.setHeatGainPerPersonOccupied(activity_level_occ);
    //userModel.setHeatGainPerPersonUnoccupied(activity_level_unocc);

    //    file.LOG(Debug, "# daylight sensors, occupancy sensors, illum control are set to 1 if there is no control.   See iso 15193 Annex F/G for values");
    userModel.setDaylightSensorSystem(daylight_sensors);
    userModel.setLightingOccupancySensorSystem(occupancy_sensors);
    userModel.setConstantIlluminationControl(const_illum_ctrl);

    //    file.LOG(Debug, "# Temp set points in degrees C");
    userModel.setHeatingOccupiedSetpoint(heating_setpoint_occ);
    userModel.setHeatingUnoccupiedSetpoint(heating_setpoint_unocc);
    userModel.setCoolingOccupiedSetpoint(cooling_setpoint_occ);
    userModel.setCoolingUnoccupiedSetpoint(cooling_setpoint_unocc);

    // file.puts "\#HVAC type was set to #{hvac_type_default}"
    //    file.LOG(Debug, "# HVAC waste, heating and cooling loss factors set by HVAC type from EN 15243");
    userModel.setHvacWasteFactor(hvac_waste_factor);
    userModel.setHvacHeatingLossFactor(hvac_heating_loss_factor);
    userModel.setHvacCoolingLossFactor(hvac_cooling_loss_factor);

    //    file.LOG(Debug, "# COP is W/W, coolingSystemIPLV is the ratio of IPLV/COP");
    userModel.setCoolingSystemCOP(cooling_COP);
    userModel.setCoolingSystemIPLVToCOPRatio(cooling_IPLVToCop_ratio);

    //    file.LOG(Debug, "#energycarrier is 1 if electric, 2 if gas");
    userModel.setHeatingEnergyCarrier(heating_fuel_type);
    // heating energy, 1=electric, 0=gas);
    userModel.setHeatingSystemEfficiency(heating_system_efficiency);

    //    file.LOG(Debug, "# vent type is 1 if mech, 2 if natural, 3 if mixed");
    userModel.setVentilationType(ventilation_type);
    //    file.LOG(Debug, "# ventilation flow rates in L/s");
    userModel.setFreshAirFlowRate(freshair_flow_rate);
    userModel.setSupplyExhaustRate(supply_exhaust_rate);

    //    file.LOG(Debug, "# heatRecover = efficiency of heat recovery (0 for none), exhaustAirRecir = fraction of supply air recirculated");
    userModel.setHeatRecovery(heat_recovery_fraction);
    // heat recovery efficiency, 0 for no heat recovery);
    userModel.setExhaustAirRecirculation(exhaust_recirculation_fraction);
    // fraction of supply air that is recirculated);

    //    file.LOG(Debug, "# DHW demand in m3/yr use 10 m3/yr/person as a default for offices");
    userModel.setDhwDemand(dhw_demand);
    //);
    // when we implement occupied and unoccupied dhw demand, we comment above and uncomment below;
    //userModel.setDhwDemandOccupied(dhw_demand_occ);
    //userModel.setDhwDemandUnoccupied(dhw_demand_unocc);

    //    file.LOG(Debug, "# dhwDistribution efficiency all taps w/i 3m = 1, taps more than 3m = 0.8, circulation or unknown = 0.6, see NEN 2916 12.6, ");
    userModel.setDhwEfficiency(dhw_system_efficiency);
    userModel.setDhwDistributionEfficiency(dhw_distribution_efficiency);
    userModel.setDhwEnergyCarrier(dhw_fuel_type);

    //    file.LOG(Debug, "# Surface Heat capacity in J/K/m2");
    userModel.setInteriorHeatCapacity(interior_heat_capacity);
    userModel.setExteriorHeatCapacity(exterior_heat_capacity);

    //    file.LOG(Debug, "# BEM type 1= none, 2 = simple, 3=advanced with FDD");
    userModel.setBemType(bem_type);

    //    file.LOG(Debug, "# specific fan power in L/s/W,  flow control factor is energy reduction from fan control measures");
    userModel.setSpecificFanPower(specific_fan_power);

    //    file.LOG(Debug, "# fan flow control  1= no control, 0.75 = inlet blade adjuct, 0.65= variable speed  see NEN 2916 7.3.3.4  ");
    userModel.setFanFlowControlFactor(fan_flow_control_factor);

    //    file.LOG(Debug, "# infiltration in m3/m2/hr @ 75 Pa based on surface area");
    userModel.setInfiltration(infiltration_rate);
    // leakage per unit surface area in m^3/m^2/hr);
    // when we implement occupied and unoccupied infiltration, we comment above and uncomment below;
    //userModel.setInfiltrationOccupied(infiltration_rate_occ);
    //userModel.setInfiltrationUnoccupied(infiltration_rate_unocc);

    //    file.LOG(Debug, "# pump control 0= no pump, 0.5 = auto pump controls for more 50% of pumps, 1.0 = all other cases.   See NEN 2914 9.4.3");
    userModel.setHeatingPumpControl(heating_pump_control_factor);
    userModel.setCoolingPumpControl(cooling_pump_control_factor);

    // write out roof and skylight info;
    //    file.LOG(Debug, "# Areas in m2, U values in W/m2/K");
    //    file.LOG(Debug, "# SCF = solar control factor (external control), SDF = shading device factor = internal shading");
    userModel.setRoofArea(roof_area);
    userModel.setRoofUValue(roof_U);
    userModel.setRoofSolarAbsorption(roof_solar_absorption);
    userModel.setRoofThermalEmissivity(roof_thermal_emissivity);

    userModel.setSkylightArea(skylight_area);
    userModel.setSkylightUvalue(skylight_U);
    userModel.setSkylightSHGC(skylight_SHGC);

    //static const std::string direction_names[] = {"N","NE","E","SE","S","SW","W","NW","N"};

    userModel.setWallAreaN(wall_areas[0]);
    userModel.setWallUvalueN(wall_U[0]);
    userModel.setWallSolarAbsorptionN(wall_solar_absorption[0]);
    userModel.setWallThermalEmissivityN(wall_thermal_emissivity[0]);
    userModel.setWindowAreaN(window_areas[0]);
    userModel.setWindowUvalueN(window_U[0]);
    userModel.setWindowSHGCN(window_SHGC[0]);
    userModel.setWindowSCFN(window_SDF[0]);
    userModel.setWindowSDFN(window_SDF[0]);

    userModel.setWallAreaNE(wall_areas[1]);
    userModel.setWallUvalueNE(wall_U[1]);
    userModel.setWallSolarAbsorptionNE(wall_solar_absorption[1]);
    userModel.setWallThermalEmissivityNE(wall_thermal_emissivity[1]);
    userModel.setWindowAreaNE(window_areas[1]);
    userModel.setWindowUvalueNE(window_U[1]);
    userModel.setWindowSHGCNE(window_SHGC[1]);
    userModel.setWindowSCFNE(window_SDF[1]);
    userModel.setWindowSDFNE(window_SDF[1]);

    userModel.setWallAreaE(wall_areas[2]);
    userModel.setWallUvalueE(wall_U[2]);
    userModel.setWallSolarAbsorptionE(wall_solar_absorption[2]);
    userModel.setWallThermalEmissivityE(wall_thermal_emissivity[2]);
    userModel.setWindowAreaE(window_areas[2]);
    userModel.setWindowUvalueE(window_U[2]);
    userModel.setWindowSHGCE(window_SHGC[2]);
    userModel.setWindowSCFE(window_SDF[2]);
    userModel.setWindowSDFE(window_SDF[2]);

    userModel.setWallAreaSE(wall_areas[3]);
    userModel.setWallUvalueSE(wall_U[3]);
    userModel.setWallSolarAbsorptionSE(wall_solar_absorption[3]);
    userModel.setWallThermalEmissivitySE(wall_thermal_emissivity[3]);
    userModel.setWindowAreaSE(window_areas[3]);
    userModel.setWindowUvalueSE(window_U[3]);
    userModel.setWindowSHGCSE(window_SHGC[3]);
    userModel.setWindowSCFSE(window_SDF[3]);
    userModel.setWindowSDFSE(window_SDF[3]);

    userModel.setWallAreaS(wall_areas[4]);
    userModel.setWallUvalueS(wall_U[4]);
    userModel.setWallSolarAbsorptionS(wall_solar_absorption[4]);
    userModel.setWallThermalEmissivityS(wall_thermal_emissivity[4]);
    userModel.setWindowAreaS(window_areas[4]);
    userModel.setWindowUvalueS(window_U[4]);
    userModel.setWindowSHGCS(window_SHGC[4]);
    userModel.setWindowSCFS(window_SDF[4]);
    userModel.setWindowSDFS(window_SDF[4]);

    userModel.setWallAreaSW(wall_areas[5]);
    userModel.setWallUvalueSW(wall_U[5]);
    userModel.setWallSolarAbsorptionSW(wall_solar_absorption[5]);
    userModel.setWallThermalEmissivitySW(wall_thermal_emissivity[5]);
    userModel.setWindowAreaSW(window_areas[5]);
    userModel.setWindowUvalueSW(window_U[5]);
    userModel.setWindowSHGCSW(window_SHGC[5]);
    userModel.setWindowSCFSW(window_SDF[5]);
    userModel.setWindowSDFSW(window_SDF[5]);

    userModel.setWallAreaW(wall_areas[6]);
    userModel.setWallUvalueW(wall_U[6]);
    userModel.setWallSolarAbsorptionW(wall_solar_absorption[6]);
    userModel.setWallThermalEmissivityW(wall_thermal_emissivity[6]);
    userModel.setWindowAreaW(window_areas[6]);
    userModel.setWindowUvalueW(window_U[6]);
    userModel.setWindowSHGCW(window_SHGC[6]);
    userModel.setWindowSCFW(window_SDF[6]);
    userModel.setWindowSDFW(window_SDF[6]);

    userModel.setWallAreaNW(wall_areas[7]);
    userModel.setWallUvalueNW(wall_U[7]);
    userModel.setWallSolarAbsorptionNW(wall_solar_absorption[7]);
    userModel.setWallThermalEmissivityNW(wall_thermal_emissivity[7]);
    userModel.setWindowAreaNW(window_areas[7]);
    userModel.setWindowUvalueNW(window_U[7]);
    userModel.setWindowSHGCNW(window_SHGC[7]);
    userModel.setWindowSCFNW(window_SDF[7]);
    userModel.setWindowSDFNW(window_SDF[7]);

    LOG(Debug, "Done Converting!");
    return userModel;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

}  // namespace isomodel
// isomodel
}  // namespace openstudio
// openstudio
