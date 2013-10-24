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

#include <isomodel/ForwardTranslator.hpp>
#include <isomodel/UserModel.hpp>

#include <model/Model.hpp>

namespace openstudio {
namespace isomodel {

  ForwardTranslator::ForwardTranslator()
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.isomodel\\.ForwardTranslator"));
  }

  ForwardTranslator::~ForwardTranslator()
  {
  }

  UserModel ForwardTranslator::translateModel(const openstudio::model::Model& model)
  {
    m_logSink.resetStringStream();

    return UserModel();
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const
  {
    std::vector<LogMessage> result;

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

}
// isomodel
}
// openstudio













// use this as a very small number to add to denominators to avoid divide by zero;
double epsilon = 0.00000000000001;


LOG(Debug, "...Setting Defaults");
LOG(Debug, "*****************************");
LOG(Debug, "       Setting Defaults");
LOG(Debug, "*****************************");
// these are items that aren't in the OSM file can't be extracted and must be set by user;
// set default starting day of occupancy as monday (Su=0, Mon=1 Sa=6);
int occupancy_day_start = 1;
// set the default ending day of occupancy;
int occupancy_day_end = 5;
// set the default starting hour for occupancy for an occupied day;
int occupancy_hour_start = 8;
// set the default ending hour for occupancy for an unoccupied day;
int occupancy_hour_end = 18;

LOG(Debug, "Occupancy Start Day = " << occupancy_day_start << " with Sun = 0, Sat = 6");
LOG(Debug, "Occupancy End Day = " << occupancy_day_end << " with Sun = 0, Sat = 6");
LOG(Debug, "Occupancy Start Hour = " << occupancy_hour_start << " ");
LOG(Debug, "Occupancy End Hour = " << occupancy_hour_end);

//these are items that aren't modeled in OS so set default values;
// occupancy sensors off in ISO since they can't be modeled directly in OS;
int occupancy_sensors = 1;
LOG(Debug, "Occupancy Sensors set 1 because they are !yet supported ");
// constant illumination control is off in ISO since it can't be modeled directly in OS;
int const_illum_ctrl = 1;
LOG(Debug, "Constant Illumination Control set to 1 because it is !supported"  );
//set ventilation type to mechanical only since OS doesn't handle natural ventilation yet;
ventilation_type = 1;
LOG(Debug, "Ventilation type set to 1 (mechanical only) because natural ventilation is !yet supported");
// set heat recovery fraction to 0 since OS doesn't support heat recovery yet;
heat_recovery_fraction = 0.0;
LOG(Debug, "Heat recovery set to 0.0 because it is !syet upported");
// set bem_type=1 because OS doesn't handle BEM yet;
bem_type = 1;
LOG(Debug, "BAS/BEM set to none because it is !yet supported");

//these are inLOG(Debug, that are really !the same as anything in OS so set defaults or get user inputs);
// set default specific fan power (L/W);
specific_fan_power = 1/0.7733;
LOG(Debug, "Specific fan power set to 1.3 L/W to match EnergyPlus default of 773.3 W/(m3/s)");

// set default fan flow control factor to 1.0 (no energy savings);
fan_flow_control_factor = 1.0;
LOG(Debug, "Fan Flow Control Factor set to 1.0 (no energy saving features)");

// set default heating pump control factor to 1.0 (no control or vfd);
heating_pump_control_factor = 1.0;
// set default cooling pump control factor to 1.0 (no control or vfd);
cooling_pump_control_factor = 1.0;
LOG(Debug, "Heating and Cooling Pump Control Factor set to 1.0 (no energy savings features)");

//these are inputs often ignored in OS models so use these as defaults);
//default DHW distribution_type is other/unknown (0, close to taps, 1 far from taps, 2 = circulation/other/ unknown;
dhw_dist_type = 2;

LOG(Debug, "DHW distribution system set to circulation or unknown");

// until we can extract HVAC inputs and guess at HVAC type, set these defaults);
// set default HVAC type to VAV;
hvac_type = 24;
LOG(Debug, "HVAC system type set VAV with water heating and water cooling");

cooling_IPLVToCop_ratio = 1.0;
// set default system partial load multiplier;

LOG(Debug, "HVAC set cooling mean partial load fraction to 1.0");

exhaust_recirculation_fraction = 0.0;
// set fraction of air recirculated  to 0.0 (i.e. total outside air);
// set default infiltration rate until we write code to extract it;
LOG(Debug, "Exhaust recirculation fraction set to 0.0");

LOG(Debug, "Shading Not Analyzed: Setting window  SCF=1, SDF=1");

// set SCF (external solar control) and SDF (shading) these for all directions N, NE, E, SE, S, SW, W, NW;
// set default window solar control factor = 1.0 (no overhang solar control);
window_SCF = Array.new(8,1.0);
// set default window shading device factor = 1.0 (no shades);
window_SDF = Array.new(8,1.0);


LOG(Debug, "...Parsing Model");
LOG(Debug, "*****************************");
LOG(Debug, "  Parsing Model");
LOG(Debug, "*****************************");

// get the weatherfile if one is defined in the OSM and convert to a string;
// wfu = model.getWeatherFile.url.to_s;
// if wfu.empty();
// LOG(Debug, "Warning: A Weather file !defined in the OSM");
// weather_file_path="none";
// } else {

// strip off the file: part of the URL, only save the stuff after file:;
// weather_file_path=wfu.split(':').last;
// LOG(Debug, "Weather file " << weather_file_path << " found");
// }

// get some important objects from the building model;
building=model.getBuilding;
facility=model.getFacility;
surfaces=model.getSurfaces;
LOG(Debug, "Found " << surfaces.size << " surfaces");
sub_surfaces = model.getSubSurfaces;
LOG(Debug, "Found " << sub_surfaces.size << " sub surfaces");
spaces = model.getSpaces;
LOG(Debug, "Found " << spaces.size << " spaces");
thermal_zones = model.getThermalZones;
LOG(Debug, "Found " << thermal_zones.size << " thermal zones");
space_types = model.getSpaceTypes;
LOG(Debug, "Found " << space_types.size << " space types");

constructions = model.getConstructions;
LOG(Debug, "Found " << constructions.size << " constructions");

internal_masses = model.getInternalMassDefinitions;
LOG(Debug, "Found " << internal_masses.size << " Internal Mass Definitions");

air_loops = model.getAirLoopHVACs;
LOG(Debug, "Found " << air_loops.size << " Air Loops");
plant_loops = model.getPlantLoops;
LOG(Debug, "Found " << plant_loops.size << " Plant Loops");


// get the terrain class from the site object or assign default if none given;
site = model.getSite;
if (!site.terrain.empty()) {
  terrain = site.terrain;
  LOG(Debug, "Terrain Type " << terrain << " in OSM");
} else {
  LOG(Debug, "Site Terrain Not Defined in OpenStudio Model - Set to Urban Terrain By Default");
  terrain = "Urban";
}

  // when we have ocean or country, there are no blocks so terrain_class=1.0;
if (terrain == "Ocean" || terrain == "Country") {
  terrain_class = 1.0;
} else if (terrain == "suburbs") {
  when "Suburbs";
  terrain_class = 0.9;
} else {
  // when city or urban set to 0.8;
  terrain_class = 0.8;
}

LOG(Debug, "Terrain class value set to " << terrain_class);

number_of_people=building.numberOfPeople;
// get the total nominal population;
logfile.printf("Nominal Number of people = %1.2f \n",number_of_people);

LOG(Debug, "*****************************");
LOG(Debug, "  Analyzing Construction");
LOG(Debug, "*****************************");

const_names = [];
const_solarAbsorptance={};
const_thermalAbsorptance={};
const_heatCapacity = {};
const_U = {};
const_SHGC= {};

BOOST_FOREACH(const &const, constructions) {
  name=const.name.to_s;
  layers=const.layers;
  const_names << name;

  // assign 0.0 as default values in case we can!assign it by searching through the construction;
  const_U[name]= 0.0;
  const_heatCapacity[name]= 0.0;
  const_thermalAbsorptance[name] = 0.0;
  const_solarAbsorptance[name] = 0.0;
  const_SHGC[name] = 0.0;

  // first find the opaque constructions - these could be exterior walls and roofs;
  if (const.isOpaque) {

    // get the 1st layer of construction (outside layer), convert to opaque material and get the thermal & solar Absorptance;
    // store them in an array indexed by the name of the construction;

    if (!layers[0].to_OpaqueMaterial.empty()) {
      const_thermalAbsorptance[name]= layers[0].to_OpaqueMaterial.get.thermalAbsorptance;
    }
    if (!layers[0].to_OpaqueMaterial.empty()) {
      const_solarAbsorptance[name]=layers[0].to_OpaqueMaterial.get.solarAbsorptance;
    }

    const_R = 0.04 + 0.12;
    //set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls;
    const_heat_capacity = 0.0;
    BOOST_FOREACH(const &layer,     layers) {
      if (!layer.to_StandardOpaqueMaterial.empty()) {
        // do this stuff if the material can convert to StandardOpaqueMaterial;
        material = layer.to_StandardOpaqueMaterial.get;
        // get the R value of the layer;
        layer_R = material.thermalResistance;
        if (!material.heatCapacity.empty()) {
          // get the heat capacity directly;
          layer_heat_capacity = material.heatCapacity;
        } else {
          // if heat capacity is !defined, calculated it as thickness*density*specific heat;
          layer_heat_capacity = material.specificHeat * material.density * material.thickness;
        }
      } else if (!layer.to_MasslessOpaqueMaterial.empty()) {
        // do this stuff if the material can convert to a MasslessOpaqueMaterial;
        material = layer.to_MasslessOpaqueMaterial.get;
        // get the R value of the layer;
        layer_R = material.thermalResistance;
        // heat capacity is !defined for this material so calculate it ias thickness*density*specific heat;
        layer_heat_capacity = material.specificHeat * material.density * material.thickness;
      } else if (!layer.to_RoofVegetation.empty()) {
        material = layer.to_RoofVegetation.get;
        layer_R = material.thermalResistance;
        layer_heat_capacity = material.specificHeat * material.density * material.thickness;
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
  } else if (const.isFenestration) {
    // assume windows have no heat capacity;
    const_heatCapacity[name]= 0.0;
    // assume windows have an effective thermal emissivity of 0;
    const_thermalAbsorptance[name]= 0.0;
    // assume windows have an effective solar absorption of 0;
    const_solarAbsorptance[name] = 0.0;

    //set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls;
    const_R = 0.04 + 0.12;

    // first check if the construction is a  single layer;
    //simple glazing, if so pull off U factor and SHGC directly, set heat capacity to 0;
    if (layers.size == 1) {

      layer=layers[0];
      if (!layer.to_SimpleGlazing.empty()) {
        // check to see if the layer is simple glazing;
        //convert to a simple glazing material;
        material = layer.to_SimpleGlazing.get;
        // U factor is directly defined;
        const_U[name]=material.uFactor.to_f;
        const_SHGC[name] = material.solarHeatGainCoefficient.to_f;
      } else if (!layer.to_StandardGlazing.empty()) {
        // check to see if the layer is standard glazing;
        // convert the layer to a standard glazing;
        material = layer.to_StandardGlazing.get;
        // U value = thermal conductance + film resistance;
        const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R);
        // SHGC = solar transmittance - ignore frame;
        const_SHGC[name] = material.solarTransmittance.to_f;
      } else if (!layer.to_ThermochromicGlazing.empty()) {
        LOG(Debug, "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3");
        material = layer.to_RefractionExtinctionGlazing;
        const_U[name] = 1.0/(1.0/ material.thermalConductance.to_f + const_R);
        const_SHGC[name] = 0.3;
      } else if (!layer.to_RefractionExtinctionGlazing) {
        LOG(Debug, "Refraction Extinction Glazing Not Converted - only estimating U value, others set to clear glass values");
        material = layer.to_RefractionExtinctionGlazing;
        const_U[name] = 1.0/(1.0/ material.thermalConductance.to_f + const_R);
        const_SHGC[name] = 0.8;
      }
    } else if (layers.size == 3) {
      // double glaze window.   assume glass - gas - glass layers;
      // assume that a layer is !simple glazing;
      if (!layers[0].to_StandardGlazing.empty()) { // check to see if layer 1 is standard glazing;
        // get layer 1 properties;
        // convert the 1st layer to a standard glazing;
        material1 = layers[0].to_StandardGlazing.get;
        norm_ST1 = material1.solarTransmittanceatNormalIncidence.to_f;
        uValue1 = material1.thermalConductance.to_f;
        norm_back_SR1 = material1.backSideSolarReflectanceatNormalIncidence.to_f;
        // Now get layer 2 properties;
        if (!layers[1].to_StandardGlazing.empty()) { // check to see if layer 2 is standard glazing;
          // convert the 2nd layer to a gas mixture;
          material2 = layers[1].to_StandardGlazing.get;
          norm_ST2 = material2.solarTransmittanceatNormalIncidence.to_f;
          uValue2 = material2.thermalConductance.to_f;
        } else if (!layers[1].to_AirGap.empty()) { // check to see if layer 1 is an air gap;
          // convert the 2nd layer to an air gap;
          material2 = layers[1].to_AirGap.get;
          norm_ST2 = 1.0;
          uValue2 = 1.0 / material2.thermalResistance.to_f;
        } else if (!layers[1].to_Gas.empty()) { // check to see if layer 1 is a gas;
          // convert the 2nd layer to a simple gas layer;
          material2 = layers[1].to_Gas.get;
          norm_ST2 = 1.0;
          // get U value at 290 K;
          uValue2 = material2.getThermalConductance(290).to_f;
        } else if (!layers[1].to_GasMixture.empty()) {          // check to see if layer 1 is a gas;
          // convert the 2nd layer to a simple gas layer;
          material2 = layers[1].to_GasMixture.get;
          norm_ST2 = 1.0;
          uValue2 = material2.getThermalConductance(290).to_f;
          // get U value at 290 K;
        } else {
          // we can't figure out what it is so assume properties of simple 12 mm air gap w/ U~6.0 from ASHRAE 2009;
          norm_ST2 = 1.0;
          uValue2 = 6.0;
        }
        // now get layer 3 properties;
        if (!layers[2].to_StandardGlazing.empty()) {
          // check to see if layer 2 is standard glazing;
          material3= layers[2].to_StandardGlazing.get;
          // convert the 2nd layer to a gas mixture;
          norm_ST3 = material3.solarTransmittanceatNormalIncidence.to_f;
          uValue3 = material3.thermalConductance.to_f;
          norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence.to_f;
        } else {
          // If it isn't standard glazing we don't know what it might be so assume properties of simple 3 mm clear glazing;
          norm_ST3 = 0.87;
          uValue3 = 6.0;
          norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence.to_f;
        }
        // compute SHGC of a 3 layer window as ST1 * ST3 / (1 - R1 *R3);
        const_SHGC[name] = norm_ST1 * norm_ST3 /(1.0 - norm_back_SR1*norm_front_SR3);
        // for U value = 1/ (sum R values for each layer + film coefficients);
        const_U[name] = 1.0/(1.0/uValue1 + 1.0/uValue2 + 1.0/uValue3 + const_R);

      } else if (!layers[0].to_ThermochromicGlazing.empty()) {
        LOG(Debug, "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3");
        material = layer.to_RefractionExtinctionGlazing;
        const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R);
        const_SHGC[name] = 0.3;
      } else if (!layers[0].to_RefractionExtinctionGlazing) {
        LOG(Debug, "Refraction Extinction Glazing Not Converted - only estimating U value, others set to clear glass values");
        material = layer.to_RefractionExtinctionGlazing;
        const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R);
        const_SHGC[name] = 0.7;
      }
    } else {
      LOG(Debug, "Only 1 and 3 layer windows are supported at this time, values set to a standard clear, double glaze window");
      const_U[name] = 3.5;
      const_SHGC[name] = 0.8;
    }
  }
}

BOOST_FOREACH(const &name, const_names) {
  LOG(Debug, name);
  logfile.printf("Ufactor = %0.2f W/K/m2, Heat Capacity = %0.0f J/K/m2, ", const_U[name].to_f, const_heatCapacity[name].to_f);
  logfile.printf("Solar Absorption = %0.2f, Thermal Absorption =%0.2f, SHGC = %0.2f \n",const_solarAbsorptance[name],const_thermalAbsorptance[name],const_SHGC[name]);
}

// parse the schedules to find the average schedule value for occupied and unoccupied times;
LOG(Debug, "...Parsing Schedules");

startDate = OpenStudio::Date.new("Jan".to_MonthOfYear,1);
endDate = OpenStudio::Date.new("Dec".to_MonthOfYear,31);

sched_names = [];
occ_aves={};
unocc_aves={};
occupied_hours=0.0;
unoccupied_hours = 0.0;

LOG(Debug, "*****************************");
LOG(Debug, "  Calculating Schedule Averages");
LOG(Debug, "*****************************");

schedule_rulesets = model.getScheduleRulesets;
LOG(Debug, "Found " << schedule_rulesets.size << " schedules in the OSM");

BOOST_FOREACH(const &schedule, schedule_rulesets) {
  occupied_sum=0;
  unoccupied_sum=0;
  occupied_hours=0;
  unoccupied_hours=0;

  // gets one day schedule for each day of the year;
  daySchedules = schedule.getDaySchedules(startDate, endDate);

  //get the day of the week of the starting day of the schedule and subtract 1 from it because we increment before we compare;
  day_of_week =  startDate.dayOfWeek.value - 1;

  // loop over the schedule for each day of the year;
  BOOST_FOREACH(const &daySchedule,   daySchedules) {

    day_of_week +=1;
    // increment the day of week counter wrapping around at 7;
    if (day_of_week > 6) {
      day_of_week =0;
    }

    // loop over each hour of the day;
    BOOST_FOREACH(const &t,     (0..23)) {
      // get the value at this hour;
      time = OpenStudio::Time.new(t / 24.0);
      value = daySchedule.getValue(time);

      // check if the day of the week is an occupied day or not;
      if ((day_of_week >= occupancy_day_start) && (day_of_week <= occupancy_day_end)) {
        // check if the hour is also an occupied hour;
        // if so, add value to occupied sum and increment occupied counter;
        if ((t >= occupancy_hour_start) && (t < occupancy_hour_end)) {
          occupied_sum+= value;
          occupied_hours += 1;
        } else {
          // if hour !occupied, add value to unoccupied and increment unoccupied counter;
          unoccupied_sum+= value;
          unoccupied_hours += 1;
        }
      } else {
        // if day is !occupied, add the hour value to unoccupied;
        unoccupied_sum+= value;
        unoccupied_hours += 1;
      }
    }
  }
  sched_names << schedule.name;

  occ_aves[schedule.name.to_s] = occupied_sum / occupied_hours;
  unocc_aves[schedule.name.to_s] = unoccupied_sum / unoccupied_hours;
  logfile.printf("Schedule %s has occupied ave = %3.2f and unoccupied ave = %3.2f\n",schedule.name.to_s, occ_aves[schedule.name.to_s], unocc_aves[schedule.name.to_s]);

}
// at this point we have an array with the schedule names and two arrays with the averages;
// that are indexed by the schedule names.;

// define what fraction of the year are occupied and unoccupied;
frac_year_occ = occupied_hours/8760.0;
frac_year_unocc = unoccupied_hours/8760.0;

logfile.printf("Fraction of year occupied = %1.3f and Fraction of year unoccupied = %1.3f\n", frac_year_occ, frac_year_unocc);

LOG(Debug, "...Calculating Electric and Gas Loads");
LOG(Debug, "*****************************");
LOG(Debug, "  Calculating Electric and Gas Loads");
LOG(Debug, "*****************************");

// get the light, electrical, and gas power densities and people densities;
// overall nominal lighting power density;
nominal_lpd= building.lightingPowerPerFloorArea;
// overall nominal electric equipment power density W/m2;
nominal_epd = building.electricEquipmentPowerPerFloorArea;
// overall nominal gas equipment power density W/m2;
nominal_gpd = building.gasEquipmentPowerPerFloorArea;

// loop over the lighting, equipment, and occupancy schedules to get the occupied and unoccupied values;
// and compute the area weighted averages;
// set this to a tiny number to avoid divide by zero but to also avoid skewing the area;
light_total_area = epsilon;
light_occ_total = 0;
light_unocc_total = 0;
elec_total_area = epsilon;
elec_occ_total = 0;
elec_unocc_total=0;
gas_total_area = epsilon;
gas_occ_total = 0;
gas_unocc_total=0;
people_total_area = epsilon;
people_occ_total = 0;
people_unocc_total = 0;
activity_total_area = epsilon;
activity_occ_total = 0;
activity_unocc_total = 0;

light_areas=[];
light_scheds=[];

BOOST_FOREACH(const &space_type, space_types) {
  // work with the lighting schedule - luminaires are !supported at this time;
  space_area = space_type.floorArea;
  BOOST_FOREACH(const &light,   space_type.lights) {
    if (!light.schedule.empty()) {
      // look for a lighting schedule;
      // get the schedule;
      sched = light.schedule.get;
      light_total_area += space_area;
      light_occ_total = occ_aves[sched.name.to_s] * space_area;
      light_unocc_total = unocc_aves[sched.name.to_s] * space_area;
    }
  }

  BOOST_FOREACH(const &electric,   space_type.electricEquipment) {
      // look for an electric equipment schedule;
    if (!electric.schedule.empty()) {
      sched = electric.schedule.get;
      elec_total_area += space_area;
      elec_occ_total = occ_aves[sched.name.to_s] * space_area;
      elec_unocc_total = unocc_aves[sched.name.to_s] * space_area;
    }
  }

  BOOST_FOREACH(const &gas,   space_type.gasEquipment) {
      // look for a gas equipment schedule;
    if (!gas.schedule.empty()) {
      sched = gas.schedule.get;
      gas_total_area += space_area;
      gas_occ_total = occ_aves[sched.name.to_s] * space_area;
      gas_unocc_total = unocc_aves[sched.name.to_s] * space_area;
    }
  }

  BOOST_FOREACH(const &people,   space_type.people) {
    // look for an occupancy schedule;
    if (!people.numberofPeopleSchedule.empty()) {
      sched = people.numberofPeopleSchedule.get;
      people_total_area += space_area;
      people_occ_total = occ_aves[sched.name.to_s] * space_area;
      people_unocc_total = unocc_aves[sched.name.to_s] * space_area;
    }

    // look for an occupant activity level schedule;
    if (!people.activityLevelSchedule.empty()) {
      sched = people.activityLevelSchedule.get;
      activity_total_area += space_area;
      activity_occ_total = occ_aves[sched.name.to_s] * space_area;
      activity_unocc_total = unocc_aves[sched.name.to_s] * space_area;
    }
  }

}

// compute the fractional multipliers from the schedule data;
lpd_occ_mult = light_occ_total/light_total_area;
lpd_unocc_mult = light_unocc_total/light_total_area;
epd_occ_mult = elec_occ_total/elec_total_area;
epd_unocc_mult = elec_unocc_total/elec_total_area;
gpd_occ_mult = gas_occ_total/gas_total_area;
gpd_unocc_mult = gas_unocc_total/gas_total_area;
people_occ_mult = people_occ_total/people_total_area;
people_unocc_mult = people_unocc_total/people_total_area;

// compute the activity level (heat gain per person) from schedule data;
activity_level_occ= activity_occ_total/activity_total_area;
activity_level_unocc= activity_unocc_total / activity_total_area;

// compute the occupied and unoccupied lighting and equipment power densities;

lpd_unoccupied=nominal_lpd*lpd_unocc_mult;
// when computing occupied, subtract the occupied because the isomodel calc adds unoccupied to occupied to account for parasitic lighting;
// that people often forget;
lpd_occupied=nominal_lpd*lpd_occ_mult - lpd_unoccupied;
epd_occupied=nominal_epd*epd_occ_mult;
epd_unoccupied=nominal_epd*epd_unocc_mult;
gpd_occupied=nominal_gpd*gpd_occ_mult;
gpd_unoccupied=nominal_gpd*gpd_unocc_mult;

logfile.printf("Lighting Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",lpd_occupied, lpd_unoccupied);
logfile.printf("Electric Equipment Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",epd_occupied, epd_unoccupied);
logfile.printf("Gas Equipment Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",gpd_occupied, gpd_unoccupied);

// compute the nominal area per person info;
nominal_app = 1.0/building.peoplePerFloorArea;
// nominal area per person m^2/person;

// set the heat gain per person to be the average occupied value;
heat_gain_per_person_occ = activity_level_occ;
heat_gain_per_person_unocc = activity_level_unocc;

// get the area per person scaled by occupancy multiplier;
area_per_person_occupied = nominal_app /people_occ_mult;
area_per_person_unoccupied = nominal_app/people_unocc_mult;

logfile.printf("Occupant Heat Gain: Occupied= %1.2f W/person, Unoccupied = %1.2f W/person\n",heat_gain_per_person_occ, heat_gain_per_person_unocc);
logfile.printf("Area per person: Occupied= %1.2f m2/person, Unoccupied = %1.2f m2/person\n",area_per_person_occupied, area_per_person_unoccupied);

// compute exterior lighting power;
exterior_lighting_power = 0.0;
if (!facility.exteriorLights.empty()) {
  // get the vector of exterior lights;
  ext_lights=facility.exteriorLights;
  // loop over the lights;
  BOOST_FOREACH(const &exlight,   ext_lights) {
    ext_light_mult = exlight.multiplier;
    ext_light_base = exlight.exteriorLightsDefinition.designLevel;
    exterior_lighting_power = ext_light_base * ext_light_mult;
  }
  LOG(Debug, "Found " << ext_lights.size << " Exterior Lights");
}
logfile.printf("Exterior Lighting Power = %1.2f W\n ",exterior_lighting_power);

// check for luminaire definitions and print a warning if they are defined;
luminaires = model.getLuminaires;
if (!luminaires.empty()) {
  LOG(Debug, "Warning: Luminaires defined in OSM but !currently translated into ISO");
}


LOG(Debug, "...Calculating Thermostats");
LOG(Debug, "*****************************");
LOG(Debug, "  Calculating Thermostats");
LOG(Debug, "*****************************");

// set the occupied and unoccupied temperature setpoint by finding average setpoint over the occupied and unoccupied times;
thermostats = model.getThermostatSetpointDualSetpoints;
cooling_setpoint_occ_=0;
cooling_setpoint_unocc=0;

// get the average heating and cooling thermostat values during occupied and unoccupied times;
// set some defaults in case  some thermostats aren't defined;
// set the default temperature setpoint for heating during occupied times;
heat_setpoint_occ_default = 21;
// set the default temperature setpoint for heating during unccupied times;
heat_setpoint_unocc_default = 15;
// set the default temp setpoint for cooling during occupied times;
cool_setpoint_occ_default = 25;
// set the default temp setpoint for cooling during unoccupied times;
cool_setpoint_unocc_default = 28;

total_zone_area = epsilon;
heat_setpoint_occ_total=0;
heat_setpoint_unocc_total=0;
cool_setpoint_occ_total = 0;
cool_setpoint_unocc_total = 0;

// loop through the zones and find the thermostats rather than use the thermostat vector we can get from the model;
// we want to do a zone area weighted average in case the different zones have different;
  //loop through the zones;
BOOST_FOREACH(const &zone, thermal_zones) {
  zone_area=zone.floorArea * zone.multiplier;
  total_zone_area += zone_area;

  if (!zone.thermostatSetpointDualSetpoint.empty()) {
    thermostat=zone.thermostatSetpointDualSetpoint.get;
    if (!thermostat.coolingSetpointTemperatureSchedule.empty()) {
      cool_sched = thermostat.coolingSetpointTemperatureSchedule.get;
      cool_setpoint_occ_total += occ_aves[cool_sched.name.to_s] * zone_area;
      cool_setpoint_unocc_total += unocc_aves[cool_sched.name.to_s] * zone_area;
    } else {
      // if we have no schedule, use the default values for thiz zone;
      cool_setpoint_occ_total += cool_setpoint_occ_default * zone_area;
      cool_setpoint_unocc_total += cool_setpoint_unocc_default * zone_area;
    }
    if (!thermostat.heatingSetpointTemperatureSchedule.empty()) {
      heat_sched = thermostat.heatingSetpointTemperatureSchedule.get;
      heat_setpoint_occ_total += occ_aves[heat_sched.name.to_s] * zone_area;
      heat_setpoint_unocc_total += unocc_aves[heat_sched.name.to_s] * zone_area;
    } else {
      heat_setpoint_occ_total += heat_setpoint_occ_default * zone_area;
      heat_setpoint_unocc_total += heat_setpoint_unocc_default * zone_area;
    }
  } else {
    cool_setpoint_occ_total += cool_setpoint_occ_default * zone_area;
    heat_setpoint_occ_total += heat_setpoint_occ_default * zone_area;
    cool_setpoint_unocc_total += cool_setpoint_unocc_default * zone_area;
    heat_setpoint_unocc_total += heat_setpoint_unocc_default * zone_area;
  }
}
cooling_setpoint_occ = cool_setpoint_occ_total/total_zone_area;
cooling_setpoint_unocc = cool_setpoint_unocc_total/total_zone_area;
heating_setpoint_occ = heat_setpoint_occ_total/total_zone_area;
heating_setpoint_unocc = heat_setpoint_unocc_total/total_zone_area;

logfile.printf("Cooling Setpoint Occupied = %1.2f C, Cooling Setpoint Unoccupied = %1.2f C\n",cooling_setpoint_occ, cooling_setpoint_unocc);
logfile.printf("Heating Setpoint Occupied = %1.2f C, Heating Setpoint Unoccupied = %1.2f C\n",heating_setpoint_occ, heating_setpoint_unocc);

// try to find if >60% of floor area has daylight control;
// loop through spaces, get area and check for any daylighting controls;
// start with a small total area to avoid divide by zero;
total_area=epsilon;
daylight_area = 0.0;
BOOST_FOREACH(const &s, spaces) {
  // add up the total floor area;
  total_area += s.floorArea * s.multiplier;
  daylight_ctrls = s.daylightingControls;
    // check for a daylighting control of any kind, assume it's turned on;
  if (daylight_ctrls.size != 0) {
    // add up the total daylighting control area;
    daylight_area += s.floorArea * s.multiplier;
  }
}
frac_daylight=daylight_area / total_area;

logfile.printf("Calculated %1.2f of total area has daylighting controls \n",frac_daylight);
if (frac_daylight > 0.6) {
  // turn on daylighting controls in ISO model;
  daylight_sensors=2;
} else {
  // turn off daylighting controls in ISO model;
  daylight_sensors=1;
}

LOG(Debug, "...Parsing Geometry");

LOG(Debug, "*****************************");
LOG(Debug, "    Parsing Geometry");
LOG(Debug, "*****************************");

// get the overall floor area from the building object;
// first see if conditioned floor area is set and use that if it is, otherwise use regular floor area;
if (!building.conditionedFloorArea.empty()) {
  floor_area = building.conditionedFloorarea;
} else {
  floor_area = building.floorArea;
}
LOG(Debug, "Floor Area = " << floor_area << " m2");

// Compute the building height as the difference from max and min vertex height;
maxz = -1000.0;
minz = 1000.0;
BOOST_FOREACH(const &sp, spaces) {
  // loop through space surfaces to find max z value;
  z_points=[];
  BOOST_FOREACH(const &s,   sp.surfaces) {
    BOOST_FOREACH(const &vertex,     s.vertices) {
      z_points << vertex.z;
      // get out all the z points for the surface and put in the array;
    }
    // find the min z for the surface and add the space origin offset;
    minsz = z_points.min + sp.zOrigin;
    // find the max z for the surface and add the space origin offset;
    maxsz = z_points.max + sp.zOrigin;
    if (maxsz > maxz) maxz = maxsz;
    if (minsz < minz) minz = minsz;
  }
}

building_height=maxz-minz;

LOG(Debug, "Building Height  = " << building_height << " m");

// find the areas of wall, roofs, and windows;
// create arrays for the walls and windows;
// set default wall U = 0.3, alpha = 0.5, epsilon=0.5 window U=3.0, window SHGC = 0.5;
direction_names= ["N","NE","E","SE","S","SW","W","NW","N"];
dirLimit = [0,45,90,135,180,225,270,315,360];
// generate an 8x1 array filled with zeros;
wall_areas=Array.new(8,0.0);
window_areas=Array.new(8,0.0);
window_areas=Array.new(8,0.0);
wall_U = Array.new(8,0.3);
window_U = Array.new(8,3.0);
wall_solar_absorption = Array.new(8,0.5);
wall_thermal_emissivity = Array.new(8,0.5);
window_SHGC = Array.new(8,0.4);

wall_HC_sum = 0.0;
wall_U_area = Array.new(8,0.0);
wall_U_sum = Array.new(8,0.0);
wall_abs_sum = Array.new(8,0.0);
wall_emiss_sum =Array.new(8,0.0);
wall_abs_area =Array.new(8,0.0);

window_HC_sum=0.0;
window_U_sum =Array.new(8,0.0);
window_U_area = Array.new(8,0.0);
window_SHGC_sum = Array.new(8,0.0);

// extract outside walls and windows, and add up areas and add up weighted surface properties;
LOG(Debug, "Found " << building.exteriorWalls.size << " exterior wall surfaces");
BOOST_FOREACH(const &s, building.exteriorWalls) {

  absorption = const_solarAbsorptance[s.construction.get.name.to_s];
  emissivity = const_thermalAbsorptance[s.construction.get.name.to_s];
  heatcapacity = const_heatCapacity[s.construction.get.name.to_s];

  // get the surface azimuth and convert from radians to degrees;
  azimuth = s.azimuth*180/3.141592654;
  // rotate the coordinate system 22.5 degrees so N is 0 to 45 instead of -22.5 to 22.5;
  az = azimuth + 22.5;
  if (az > 360 then) {
    // remap anything between 360 and 382.5 back to 0 to 22.5;
    az = az - 360;
  }
  for (int i = 0; i <= 7; ++i) {;
    if ((az > dirLimit[i]) && (az<= dirLimit[i+1])) {

      // add in the wall area without window;
      wall_areas[i] +=s.netArea;
      wall_abs_sum[i] += absorption * s.netArea;
      wall_emiss_sum[i] += emissivity * s.netArea;
      wall_HC_sum += heatcapacity *s.netArea;

      wallU = const_U[s.construction.get.name.to_s];
      // if the U value is !NaN add in the U value and area for weighting;
      if (!wallU.nan) {
        wall_U_area[i]+= s.netArea;
        wall_U_sum[i] += wallU * s.netArea;
      }
      subsurface=s.subSurfaces;
      BOOST_FOREACH(const &ss,       subsurface) {
        windowU = const_U[ss.construction.get.name.to_s];
        windowSHGC = const_SHGC[ss.construction.get.name.to_s];
        if (!windowU.nan) {
          window_U_area[i] += ss.surface.get.netArea;
          window_U_sum[i] += windowU * ss.surface.get.netArea;
          window_SHGC_sum[i] += windowSHGC * ss.surface.get.netArea;
        }
      }
      // get just the window area;
      window_areas[i] += s.grossArea-s.netArea;

      // add in lines to  compute SCF, and SDF in each direction;
    }
  }
}

//  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC;
for (int i = 0; i <= 7; ++i) {;
  if (!(wall_areas[i] == 0.0)) {
    wall_solar_absorption[i] = wall_abs_sum[i] / wall_areas[i];
    wall_thermal_emissivity[i]= wall_emiss_sum[i] / wall_areas[i];
  }
  if (!(wall_U_area[i] == 0.0)) {
    // if any wall surfaces have a U value defined, use the weighted average of U values;
    wall_U[i] = wall_U_sum[i]/wall_U_area[i];
  }
  if (!(window_areas[i] == 0.0)) {
    window_U[i] = window_U_sum[i]/window_areas[i];
    window_SHGC[i] = window_SHGC_sum[i] / window_areas[i];
  }
}

total_wall_area =0.0;
BOOST_FOREACH(const &a, wall_areas) {
  total_wall_area += a;
}

total_window_area=0.0;
BOOST_FOREACH(const &a, window_areas) {
  total_window_area += a;
}

logfile.printf("Total Wall Area = %1.2f m2\n",total_wall_area);
logfile.printf("Total Window Area = %1.2f  m2\n",total_window_area);

// set default roof U, absorption, emissivity and skylight U and SHGC;

// set default roof solar absorption coefficient to 0.9;
roof_solar_absorption = 0.9;
// set default roof thermal emissivity to 0.9;
roof_thermal_emissivity = 0.9;
// set default roof U to 0.2 W/m2/K;
roof_U = 0.2;
skylight_U = 3;
skylight_SHGC = 0.5;

roof_area=0.0;
skylight_area=0.0;

roof_abs_sum = 0.0;
roof_emiss_sum = 0.0;
roof_U_sum =0.0;
roof_HC_sum = 0.0;
skylight_U_sum =0.0;
skylight_SHGC_sum = 0.0;
roof_count=0;

// the building.roofs variable does !seem to return the roof surfaces in a vector as it supposed to;
// so we search through all surfaces and find the outside roof surfaces ourselves;
// calculate area and average solar absorption and thermal emissivity;
BOOST_FOREACH(const &s, surfaces) {
  // skip surface if !labeled roof;
  if (s.surfaceType != "RoofCeiling") { continue; }
  // skip surface if !outside;
  if (s.outsideBoundaryCondition != "Outdoors") { continue; }
  roof_count +=1;
  absorption = const_solarAbsorptance[s.construction.get.name.to_s];
  emissivity = const_thermalAbsorptance[s.construction.get.name.to_s];
  heatcapacity = const_heatCapacity[s.construction.get.name.to_s];
  roofU = const_U[s.construction.get.name.to_s];

  roof_abs_sum += absorption * s.netArea;
  roof_emiss_sum += emissivity * s.netArea;
  roof_HC_sum += heatcapacity * s.netArea;
  roof_U_sum += roofU * s.netArea;

  // get the roof area without skylights;
  roof_area += s.netArea;
  // get the skylight area;
  skylight_area += s.grossArea - s.netArea;

  subsurface=s.subSurfaces;
  BOOST_FOREACH(const &ss,   subsurface) {
    skylight_U_sum += const_U[ss.construction.get.name.to_s] * ss.surface.get.netArea;
    skylight_SHGC_sum+= const_SHGC[ss.construction.get.name.to_s] * ss.surface.get.netArea;
  }
}
LOG(Debug, "Found " << roof_count << " roof surfaces");

if (!(roof_area == 0.0)) {
  // compute roof absorption as area weighted average;
  roof_solar_absorption = roof_abs_sum / roof_area;
  // compute roof emissivity as an area weighted average;
  roof_thermal_emissivity=  roof_emiss_sum/ roof_area;
  // compute roof U as area weighted average;
  roof_U = roof_U_sum / roof_area;
}

if (!(skylight_area == 0.0)) {
  skylight_U = skylight_U_sum / skylight_area;
  skylight_SHGC = skylight_SHGC_sum /skylight_area;
}

for (int i = 0; i <= 7; ++i) {
  logfile.printf("Wall%s: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",
      direction_names[i],wall_areas[i],wall_U[i],wall_solar_absorption[i],wall_thermal_emissivity[i]);
}
for (int i = 0; i <= 7; ++i) {
  logfile.printf("Window%s: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f, SCF=%1.2f, SDF=%1.2f \n",
      direction_names[i],window_areas[i],window_U[i],window_SHGC[i],window_SCF[i],window_SDF[i]);
}


// BOOST_FOREACH(const &s, surfaces) {
// next if !s.surfaceType == "RoofCeiling";
// skip surface if !labeled roof;
// next if !s.outsideBoundaryCondition == "Outdoors";
// skip surface if !outside;
// roof_count +=1;
// absorption = const_solarAbsorptance[s.construction.get.name.to_s];
// emissivity = const_thermalAbsorptance[s.construction.get.name.to_s];
// heatcapacity = const_heatCapacity[s.construction.get.name.to_s];
// roofU = const_U[s.construction.get.name.to_s];

// roof_abs_sum += absorption * s.netArea;
// roof_emiss_sum += emissivity * s.netArea;
// roof_HC_sum += heatcapacity * s.netArea;
// roof_U_sum += roofU * s.netArea;

// roof_area += s.netArea;
// skylight_area += s.grossArea - s.netArea;

// subsurface=s.subSurfaces;
// BOOST_FOREACH(const &ss,   // subsurface) {
// skylight_U_sum += const_U[ss.construction.get.name.to_s] * ss.surface.get.netArea;
// skylight_SHGC_sum+= const_SHGC[ss.construction.get.name.to_s] * ss.surface.get.netArea;
// }

// }

logfile.printf("Roof: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",roof_area,roof_U,roof_solar_absorption,roof_thermal_emissivity);
logfile.printf("Skylight: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f \n",skylight_area,skylight_U,skylight_SHGC);

// compute exterior heat capacity;
exterior_heat_capacity = (roof_HC_sum+ wall_HC_sum + window_HC_sum)/(total_wall_area + roof_area+ total_window_area);
logfile.printf("Exterior Heat Capacity = %1.0f J/K/m2 based on surface area\n", exterior_heat_capacity);

// add up the heat capacity of the defined interior surfaces including any internale mass elements;
// compute as normalized to floor area and !surface area;
interior_HC_sum = 0.0;
BOOST_FOREACH(const &s, surfaces) {
  if (s.outsideBoundaryCondition == "Outdoors") { continue; }
  // skip surface if outside;
  interior_HC_sum += const_heatCapacity[s.construction.get.name.to_s] * s.netArea;
}

// get and add in the heat capacity of any defined internal masses;
internal_mass_HC_sum =0.0;
BOOST_FOREACH(const &mass, internal_masses) {
  internal_mass_HC_sum += mass.surfaceArea.to_f * const_heatCapacity[mass.construction.get.name.to_s];
}
interior_heat_capacity = (interior_HC_sum +internal_mass_HC_sum)/building.floorArea;
logfile.printf("Interior  Heat Capacity = %1.0f J/K/m2 based on floor area\n", interior_heat_capacity);

LOG(Debug, "...Calculating Infiltration");
LOG(Debug, "*****************************");
LOG(Debug, "   Calculating Infiltration");
LOG(Debug, "*****************************");

// compute infiltration;
//first check to see if there effective leakage areas defined and if !those, then design flow rates;
if (!model.getSpaceInfiltrationEffectiveLeakageAreas.empty()) {
  infiltration = model.getSpaceInfiltrationEffectiveLeakageAreas;
  BOOST_FOREACH(const &infil,   infiltration) {
    // set default average envelope air leakage (infiltration) as 7 m3/h/m2 which is about the EnergyPlus defaults;
    LOG(Debug, "EffectiveLeakageArea !Implemented Yet, Infiltration Rate Set to 7.0 m3/m2/h @ 75 Pa");
    infiltration_rate = 7.0;
  }
} else if (!model.getSpaceInfiltrationDesignFlowRates.empty()) {
  infiltration = model.getSpaceInfiltrationDesignFlowRates;
  LOG(Debug, "Found " << infiltration.size << " SpaceInfiltrationDesignFlowRate objects");

  infil_frac_sum = 0.0;
  infil_rate_sum = 0.0;
  count = 0;
  BOOST_FOREACH(const &infil,   infiltration) {
    count += 1;
    rate_type = infil.designFlowRateCalculationMethod;
    if (!infil.space.empty()) {
      LOG(Debug, "Space defined for Infiltration object #" << count);
      if (rate_type == "Flow/Space") {
        // add in the design flow rate per space * number of spaces;
        infil_rate_temp = infil.designFlowRate.to_s.to_f;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, " << infil_rate_temp << " m3/s added");
      } else if (rate_type == "Flow/ExteriorArea" || rate_type == "Flow/ExteriorWallArea") {
        // add in the flow/exterior area  * the exterior area;
        infil_rate_temp = infil.flowperExteriorSurfaceArea.to_s.to_f * infil.space.exteriorArea;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/ExteriorArea, " << infil_rate_temp << " m3/s added");
      } else if (rate_type == "Flow/Area") {
        infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * infil.space.floorArea;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/Area, " << infil_rate_temp << " m3/s added");
      } else if (rate_type == "AirChanges/Hour") {
        infil_rate_temp= 0;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, Not implemented yet so nothing added");
      } else {
        infil_rate_temp= 0;
        LOG(Debug, "Infiltration Ojbect #" << count << " has no calc method defined, so nothing added");
      }
    } else if (!infil.spaceType.empty()) {
      LOG(Debug, "No space defined in Infiltration object #" << count << ", using spacetype object instead");
      st=infil.spaceType.get;
      st_volume =0.0;
      st_exterior_area = 0.0;
      st_num = st.spaces.size;
      BOOST_FOREACH(const &s,       st.spaces) {
        st_volume += s.volume;
        st_exterior_area += s.exteriorArea;
      }
      LOG(Debug, "Found " << st.spaces.size << " spaces in spacetype for Infiltration object " << count);
      logfile. LOG(Debug, "Infiltration design flow rate found of type " << rate_type);

      if (rate_type == "Flow/Space") {
        // add in the design flow rate per space * number of spaces;
        infil_rate_temp= infil.designFlowRate.to_s.to_f * st.spaces.size;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/Space, " << infil_rate_temp << " m3/s added");
      } else if (rate_type == "Flow/ExteriorArea" || rate_type == "Flow/ExteriorWallArea") {
        // add in the flow/exterior area  * the exterior area;
        infil_rate_temp= infil.flowperExteriorSurfaceArea.to_s.to_f * st_exterior_area;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/ExteriorArea, " << infil_rate_temp << " m3/s added"        );
      } else if (rate_type == "Flow/Area") {
        // add in the flow/floor area * floor area;
        infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * st.floorArea;
        LOG(Debug, "Infiltration Object #" << count << " is Flow/Area, " << infil_rate_temp << " m3/s added"        );
      } else if (rate_type == "AirChanges/Hour") {
        infil_rate_temp= 0;
        infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * st.floorArea;
        LOG(Debug, "Infiltration Object #" << count << " is AirChanges/Hour, " << infil_rate_temp << " m3/s added");
      } else {
        infil_rate_temp = infil.airChangesperHour.to_s.to_f * st_volume;
        LOG(Debug, "Infiltration Ojbect #" << count << " has no calc method defined, so nothing added"        );
      }
    } else {
      LOG(Debug, "No space or spacetype defined for Infiltration object #" << count << ", skipping analysis");
    }

    // now look for the schedule for the infil object and add up the fraction;
    if (!infil.schedule.empty()) {
      sched = infil.schedule.get.name.to_s;
      // if a schedule exists, get the average yearly fraction by a time weighted average of occupied and unoccupied averages;
      infil_frac =  occ_aves[sched]*frac_year_occ + unocc_aves[sched]*frac_year_unocc;
      LOG(Debug, "Infiltration schedule found for Infiltration object #" << count << ", scaling by " << infil_frac);
    } else {
      LOG(Debug, "No Infiltration schedule found for Infiltration object #" << count << ", assuming always on");
      infil_frac = 1.0;
    }
    infil_rate_sum +=  infil_rate_temp * infil_frac;
  }
  logfile.printf("Total Infiltration = %1.3f m3/s at 4 Pa (natural pressure difference)\n",infil_rate_sum);

  // get avg infiltration rate and convert to from m3/m2/min @ 4 Pa to m3/m2/h @ 75 Pa with a .67 pressure exponent;
  // assume constant infilration rate is based on a nominal 4 Pa pressure difference;
  infiltration_rate = infil_rate_sum/(total_wall_area + roof_area)*3600*(75/4)**0.67;
} else {
  LOG(Debug, "No Infiltration Design Flow Rates Found, Infiltration Rate Set to 2.0 m3/m2/h @ 75 Pa");
  infiltration_rate = 2.0;
  // set default average envelope air leakage (infiltration) as 2 m3/h/m2 which is about the EnergyPlus default;
}

logfile.printf( "Infiltration rate set to %1.3f m3/h/m2 @ 75 Pa\n", infiltration_rate);

LOG(Debug, "...Parsing HVAC Info");
LOG(Debug, "*****************************");
LOG(Debug, "   Parsing HVAC Info");
LOG(Debug, "*****************************");

LOG(Debug, "*****************************");
LOG(Debug, "  Analyzing HVAC");
LOG(Debug, "*****************************");
LOG(Debug, "...Analyzing HVAC");

hvac_component_array = [];
hvac_component_area_array=[];

plant_loops_array=[];
plant_loops_area_array=[];

// look through the air loops and extract the components;
// for components that can connect to a plant water loop (namely things with water heating and cooling coils);
BOOST_FOREACH(const &air_loop, air_loops) {
  supply_components = air_loop.supplyComponents;

  zone_area = 0.0;
  // get the total area of all the zones connected through the loop;
  BOOST_FOREACH(const &z,   air_loop.thermalZones) {
    zone_area += z.floorArea;

  }

  // loop through the components and store them all in a single array;
  // if;
  BOOST_FOREACH(const &component,   supply_components) {
    // skip if the component is a node;
    if (!component.to_Node.empty()) continue;
    // skip of component is a mixer connector;
    if (!component.to_ConnectorMixer.empty()) continue;
    // skip if component is a splitter;
    if (!component.to_ConnectorSplitter.empty()) continue;
    // skip if component is a pipe;
    if (!component.to_PipeAdiabatic.empty()) continue;

    // if the component is a water cooling coil, do !save, but instead save the attached plant loop;
    if ((!component.to_CoilCoolingWater.empty())) {  // if the component is a cooling coil, don't save the attached plant loop;
      plant_loops_array << component.to_CoilCoolingWater.get.plantLoop;
      plant_loops_area_array << zone_area;
      // if the component is a water cooling coil, do !save but instead save the attached plant loop;
    } else if (!component.to_CoilHeatingWater.empty()) {
      // if the component is a heating coil, don't save and search the attached plant loop;
      plant_loop_components = component.to_CoilHeatingWater.get.plantLoop.get.supplyComponents;
      // get the cooling coil plant loop;
      plant_loops_array << component.to_CoilHeatingWater.get.plantLoop;
      plant_loops_area_array << zone_area;
    } else {
      // otherwise this component is !hooked to a water loop so save it for later parsing;
      hvac_component_array << component;
      hvac_component_area_array << zone_area;
    }
  }
}

BOOST_FOREACH(const &i, (0 .. (plant_loops_array.size - 1))) {
  supply_components = plant_loops_array[i].get.supplyComponents;
  zone_area = plant_loops_area_array[i];

  BOOST_FOREACH(const &component,   supply_components) {
    // skip if the component is a node;
    if (!component.to_Node.empty()) continue;
    // skip of component is a mixer connector;
    if (!component.to_ConnectorMixer.empty()) continue;
    // skip if component is a splitter;
    if (!component.to_ConnectorSplitter.empty()) continue;
    // skip if component is a pipe;
    if (!component.to_PipeAdiabatic.empty()) continue;

    hvac_component_array << component;
    hvac_component_area_array << zone_area;
  }
}


// now add in any HVAC components !part of an air loop (PTAC, PTHP, unit heaters, etc);
BOOST_FOREACH(const &zone, thermal_zones) {
  BOOST_FOREACH(const &component,   zone.equipment) {
    hvac_component_array << component;
    hvac_component_area_array << zone.floorArea;
  }
}


cooling_coil_array=[];
cooling_coil_area_array=[];
heating_coil_array=[];
heating_coil_area_array=[];
BOOST_FOREACH(const &i, (0 .. (hvac_component_array.size-1))) {
  component=hvac_component_array[i];
  area = hvac_component_area_array[i];

  // first check for PTAC;
  if (!component.to_ZoneHVACPackagedTerminalAirConditioner.empty()) {
    ptac=component.to_ZoneHVACPackagedTerminalAirConditioner.get;
    cooling_coil_array << component.to_ZoneHVACPackagedTerminalAirConditioner.get.coolingCoil;
    cooling_coil_area_array << area;
    heating_coil_array << component.to_ZoneHVACPackagedTerminalAirConditioner.get.heatingCoil;
    heating_coil_area_array << area;
    // next check for PTHP;
  } else if (!component.to_ZoneHVACPackagedTerminalHeatPump.empty()) {
    pthp=component.to_ZoneHVACPackagedTerminalHeatPump.get;
    cooling_coil_array << component.to_ZoneHVACPackagedTerminalHeatPump.get.coolingCoil;
    cooling_coil_area_array << area;
    heating_coil_array << component.to_ZoneHVACPackagedTerminalHeatPump.get.heatingCoil;
    heating_coil_area_array << area;
    // next check for water to air heat pump;
  } else if (!component.to_ZoneHVACWaterToAirHeatPump.empty()) {
    wahp=component.to_ZoneHVACWaterToAirHeatPump.get;
    cooling_coil_array << component.to_ZoneHVACWaterToAirHeatPump.get.coolingCoil;
    cooling_coil_area_array << area;
    heating_coil_array << component.to_ZoneHVACWaterToAirHeatPump.get.heatingCoil;
    heating_coil_area_array << area;
    // next check for a component Heater;
  } else if (!component.to_ZoneHVACUnitHeater.empty()) {
    heating_coil_array << component.to_ZoneHVACUnitHeater.get.heatingCoil;
    heating_coil_area_array << area;
    //check for unitary air-to-air heat pump;
  } else if (!component.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty()) {
    cooling_coil_array << component.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.coolingCoil;
    cooling_coil_area_array << area;
    heating_coil_array << component.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.heatingCoil;
    heating_coil_area_array << area;
    // next check for the cooling and heating coils themselves and save them to the lists;
  } else if (!component.to_CoilCoolingDXSingleSpeed.empty()) {
    // check for single speed DX coil;
    cooling_coil_array << component;
    cooling_coil_area_array << area;
  } else if (!component.to_CoilCoolingDXTwoSpeed.empty()) {
    // check for a two speed DX coil;
    cooling_coil_array << component;
    cooling_coil_area_array << area;
  } else if (!component.to_CoilCoolingWater.empty()) {
    // check for plain cooling water coil;
    cooling_coil_array << component;
    cooling_coil_area_array << area;
  } else if (!component.to_ChillerElectricEIR.empty()) {
    // check for electric chiller;
    cooling_coil_array << component;
    cooling_coil_area_array << area;
  } else if (!component.to_CoilHeatingWater.empty()) {
    // check for plain heating water coil;
    heating_coil_array << component;
    heating_coil_area_array << area;
  } else if (!component.to_CoilHeatingElectric.empty()) {
    heating_coil_array << component;
    heating_coil_area_array << area;
  } else if (!component.to_CoilHeatingGas.empty()) {
    heating_coil_array << component;
    heating_coil_area_array << area;
  } else if (!component.to_BoilerHotWater.empty()) {
    heating_coil_array << component;
    heating_coil_area_array << area;
  } else if (!component.to_BoilerSteam.empty()) {
    heating_coil_array << component;
    heating_coil_area_array << area;
  }
}


LOG(Debug, "Found " << cooling_coil_array.size << " cooling elements");
LOG(Debug, "Found " << heating_coil_array.size << " heating elements");

// go through the cooling coil array list and add up the COP*zone area and zone area to compute zonearea weighted COP;
cop_sum = 0.0;
cop_area_sum = 0.0;
cooling_coil_name_array=[];
BOOST_FOREACH(const &i, (0 .. (cooling_coil_array.size - 1))) {
  coil = cooling_coil_array[i];
  area = cooling_coil_area_array[i];
  if (!coil.to_CoilCoolingDXSingleSpeed.empty()) {
    // check for single speed DX coil;
    cop_sum += coil.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f*area;
    cop_area_sum += area;
  } else if (!coil.to_CoilCoolingDXTwoSpeed.empty()) {
    // check for two speed DX coil;
    cop_sum += coil.to_CoilCoolingDXTwoSpeed.get.ratedHighSpeedCOP.to_f*area;
    cop_area_sum += area;
  } else if (!coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty()) {
    // check for heat pump;
    cop_sum += coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.coolingCoil.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f*area;
    cop_area_sum += area;
  } else if (!coil.to_CoilCoolingWater.empty()) {
    // check for water cooling coils;
    cop_sum += coil.to_CoilCoolingWater.get*area;

    cop_area_sum += area;
  } else if (!coil.to_ChillerElectricEIR.empty()) {
    cop_sum += coil.to_ChillerElectricEIR.get.referenceCOP*area;

    cop_area_sum += area;

  }
}

// if the COP sum is 0 but the cooling coil array is non-zero, check the plant loop for a chiller;
if ((cop_sum == 0.0  ) && (cooling_coil_array.size > 0)) {
  LOG(Debug, "checking plant loop for chiller");

  BOOST_FOREACH(const &plant_loop,   plant_loops) {
    supply_components = plant_loop.supplyComponents;
    // loop through the components and get the efficiency;
    area=building.floorArea;
    BOOST_FOREACH(const &component,     supply_components) {
      // if we are here then we don't have a specific area assigned to the cooling unit so use the full building floor area;
      if (!component.to_CoilCoolingDXSingleSpeed.empty()) {
        // check for single speed DX coil;
        cop_array << component.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f;
        cop_areas << building.floorArea;
      } else if (!component.to_CoilCoolingDXTwoSpeed.empty()) {
        // check for two speed DX coil;
        cop_array <<  component.to_CoilCoolingDXTwoSpeed.get.ratedHighSpeedCOP.to_f;
        cop_areas << area;
      } else if (!component.to_to_ChillerElectricEIR.empty()) {
        // check for a chiller;
        cop_sum += coil.to_ChillerElectricEIR.get.referenceCOP*area;

        cop_area_sum += area;
      }
    }
  }
}


if (cop_area_sum > 0.0) {
  cooling_COP = cop_sum / cop_area_sum;
  LOG(Debug, "Area Weighted Average Cooling COP = " << cooling_COP);
} else {
  cooling_COP=0.0;
  LOG(Debug, "No Cooling Equipment Found, set COP = " << cooling_COP);
}


// go through the heating coil array list and add up the COP*zone area for gas and electic heating coils;
elec_sum = 0.0;
elec_area_sum = 0.0;
gas_sum = 0.0;
gas_area_sum = 0.0;
for (int i = 0; i < heating_coil_array.size(); ++i)
{
  coil = heating_coil_array[i];
  area = heating_coil_area_array[i];

    // check for gas heating coil;
  if (!coil.to_CoilHeatingGas.empty()) {
    gas_sum += coil.to_CoilHeatingGas.get.gasBurnerEfficiency.to_f*area;
    gas_area_sum += area;
    // check for electric heating coil;
  } else if (!coil.to_CoilHeatingElectric.empty()) {
    elec_sum += coil.to_CoilHeatingElectric.get.efficiency.to_f*area;
    elec_area_sum += area;
    // check for single speed DX heating coil;
  } else if (!coil.to_CoilHeatingDXSingleSpeed.empty()) {
    elec_sum += coil.to_CoilHeatingDXSingleSpeed.get.ratedCOP.to_f*area;
    elec_area_sum += area;
    // check for unitary heat pump, extract coil and get;
  } else if (!coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty()) {
    elec_sum += coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.heatingCoil.to_CoilHeatingDXSingleSpeed.get.ratedCOP.to_f*area;
    elec_area_sum += area;
  } else if (!coil.to_BoilerHotWater.empty()) {
    boiler =coil.to_BoilerHotWater.get;
    if (boiler.fuelType.to_s =="NaturalGas") {
      gas_sum +=  boiler.nominalThermalEfficiency.to_f*area;
      gas_area_sum +=  area;
    } else {
      elec_sum += boiler.nominalThermalEfficiency.to_f*area;
      elec_area_sum += area;
    }
  }
  // } else if (!coil) {
}


// if there seems to be no heating energy used but there are heating elements detected, check the plant loop for a boiler;
if (((gas_sum + elec_sum) == 0.0  ) && (heating_coil_array.size>0)) {
  BOOST_FOREACH(const &plant_loop,   plant_loops) {
    supply_components = plant_loop.supplyComponents;
    // loop through the components and get the efficiency;
    area=building.floorArea;
    BOOST_FOREACH(const &component,     supply_components) {
        // check for a hot water boiler;
      if (!component.to_BoilerHotWater.empty()) 
        boiler = component.to_BoilerHotWater.get;
      if (boiler.fuelType.to_s =="NaturalGas") {
        gas_sum +=  boiler.nominalThermalEfficiency.to_f*area;
        gas_area_sum += area;
      } else {
        elec_sum += boiler.nominalThermalEfficiency.to_f*area;
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
  heating_system_efficiency = (elec_sum + gas_sum)/(elec_area_sum + gas_area_sum);
  LOG(Debug, "Average Heating Efficiency = " << heating_system_efficiency);
} else {
  LOG(Debug, "No heating elements found - setting Average Heating Efficiency = 0.8 and Heating Fuel Type = 2");
  heating_system_efficiency = 0.8;
  heating_fuel_type = 2;
}




// set up the HVAC loss tables;
// hvac type     0    1    2    3    4    5    6    7    8    9;
//          10    11    12    13    14    15    16    17    18    19;
//          20    21    22    23    24    25    26    27    28    29;
//          30    31    32    33    34    35    36    37    38    39;
double hvac_waste_table[] ={  0.00,  0.00,   0.00,   0.00,   0.00,   0.03,   0.00,  0.00,   0.04,   0.00,
                 0.03,   0.00,  0.00,   0.08,   0.03,   0.00,   0.00,   0.03,   0.00,   0.00,
                 0.04,    0.00,   0.03,   0.00,   0.00,  0.04,   0.00,   0.00,   0.08,   0.04,
                 0.00,  0.00,  0.04,  0.00,  0.04,  0.00,  0.00};

double hvac_hot_table[] ={  0.00,  0.08,   0.25,   0.00,   0.36,   0.08,  0.08,   0.25,   0.00,   0.36,
               0.08,   0.08,  0.25,   0.00,   0.08,   0.08,   0.25,   0.08,   0.08,   0.25,
               0.00,    0.36,   0.08,   0.08,   0.25,  0.08,   0.00,   0.00,   0.00,   0.08,
               0.00,  0.00,  0.08,  0.00,  0.08,  0.00,  0.00};

double hvac_cold_table[] ={  0.00,   0.00,   0.00,   0.00,   0.00,   0.01,  0.00,   0.00,   0.00,   0.00,
                0.01,   0.00,  0.00,   0.01,   0.01,   0.00,   0.00,   0.01,   0.00,   0.00,
                0.00,    0.00,   0.01,   0.00,   0.00,  0.01,   0.00,   0.00,   0.01,   0.01,
                0.00,  0.00,  0.01,  0.00,  0.01,  0.00,  0.00};

//extract losses from loss tables based on hvac type;
double hvac_waste_factor=hvac_waste_table[hvac_type];
double hvac_heating_loss_factor = hvac_hot_table[hvac_type];
double hvac_cooling_loss_factor=hvac_cold_table[hvac_type];

LOG(Debug, "HVAC Waste Factor = " << hvac_waste_factor);
LOG(Debug, "HVAC Heating Loss Factor = " << hvac_heating_loss_factor);
LOG(Debug, "HVAC Cooling Loss Factor = " << hvac_cooling_loss_factor);

// print out efficiencies;
LOG(Debug, "Cooling System COP = " << cooling_COP);
LOG(Debug, "Cooling System IPLV/COP Ratio =" << cooling_IPLVToCop_ratio);
    // heating energy, 1=electric, 0=gas);
LOG(Debug, "Heating Fuel Type = " << heating_fuel_type;
LOG(Debug, "Heating System Efficiency = " << heating_system_efficiency);

// calculate fresh air ventilation rates;
double freshair_flow_rate = 0.0;
BOOST_FOREACH(const openstudio::model::Space &s, spaces) 
{
  if (!s.designSpecificationOutdoorAir.empty()) {
    outdoor_air = s.designSpecificationOutdoorAir.get;

    // these methods will return 0.0 if nothing is defined;
    // get ACH and convert to L/s;
    air_ach = outdoor_air.outdoorAirFlowAirChangesperHour* s.volume / 3.6;
    // get air flow rate in L/s;
    air_rate = outdoor_air.outdoorAirFlowRate * 1000;
    // get in m3/s/person and convert to L/s;
    air_per_person = outdoor_air.outdoorAirFlowperPerson * s.numberOfPeople * 1000;
    // get in m3/s/m2 and convert to L/s;
    air_per_area = outdoor_air.outdoorAirFlowperFloorArea * s.floorArea * 1000;

    if (!outdoor_air.outdoorAirFlowRateFractionSchedule.empty()) {
      sched = outdoor_air.outdoorAirFlowRateFractionSchedule.get.name.to_s;
      sched_mult=occ_aves[sched]*frac_year_occ + unocc_aves[sched]*frac_year_unocc;
    } else {
      // if no schedule is defined set multiplier to 1.0;
      sched_mult=1.0;
    }

    if (outdoor_air.outdoorAirMethod == "Maximum") {
      space_air_rate = [air_per_person, air_per_area, air_rate, air_ach].max * s.multiplier * sched_mult;
    } else {
      // if maximum is !selected, assume sum is selected;
      space_air_rate = (air_per_person + air_per_area + air_rate + air_ach) * s.multiplier * sched_mult;
    }

  } else {
    // if no outdoor air rate is defined for space, default to 20 cfm/nominal person and convert to L/S from cfm;
    LOG(Debug, " No outdoor air rate define for space " << s.name << " , default of 20cm/nominal person used");
    space_air_rate = 20 * s.numberOfPeople/2.12;
  }
  freshair_flow_rate += space_air_rate;
}

supply_exhaust_rate = freshair_flow_rate;
// set exhaust = supply rate as default (i.e. no pressurization);
logfile.printf("Total ventilation fresh air flow rate = %1.2f L/s\n", freshair_flow_rate);
LOG(Debug, "No exhaust rate extraction - set air exhaust rate to ventilation rate" );


// Try to find DHW water components and get the water use;

// search plant loops for water heaters;
dhw_gas_array=[];
dhw_elec_array=[];
BOOST_FOREACH(const &plant_loop, plant_loops) {
  supply_components = plant_loop.supplyComponents;
  // loop through the components and find the components that are water heaters;
  BOOST_FOREACH(const &component,   supply_components) {
      // enter here if the item is a water heater;
    if (!component.to_WaterHeaterMixed.empty()) {
      // convert component to water heater type;
      heater = component.to_WaterHeaterMixed.get;
        // check to see if the heater is gas or electric;
      if (heater.heaterFuelType.to_s =="NaturalGas") {
        // store the efficiency;
        dhw_gas_array << heater.heaterThermalEfficiency.to_f;
      } else {
        // store the efficiency;
        dhw_elec_array << heater.heaterThermalEfficiency.to_f;
      }
    }
  }
}

// add up all the uses from the individual components;
dhw_gas_sum=0.0;
BOOST_FOREACH(const &gas, dhw_gas_array) {
  dhw_gas_sum += gas;
}
dhw_elec_sum=0.0;
BOOST_FOREACH(const &elec, dhw_elec_array) {
  dhw_elec_sum += elec;
}

// find the average efficiency of water heaters;
n_dhw_units = dhw_gas_array.size + dhw_elec_array.size;
  // if the number of water heaters is > 0;
if (n_dhw_units > 0) {
  dhw_system_efficiency = (dhw_gas_sum + dhw_elec_sum)/(dhw_gas_array.size + dhw_elec_array.size);
} else {
  dhw_system_efficiency = 1.0;
}

  // if there are more gas DHW water heaters, fuel type is gas;
if (dhw_gas_sum > dhw_elec_sum) {
  dhw_fuel_type = 2;
} else {
  dhw_fuel_type = 1;
}

// use DHW distribution type to set DHW distribution efficiency;
// create lookup table;
dhw_dist_eff_table=[1.0,0.8,0.6];
dhw_distribution_efficiency = dhw_dist_eff_table[dhw_dist_type];

// extract the water usage by starting with the water use connections and then finding equipment;
water_connects = model.getWaterUseConnectionss;
dhw_demand = 0.0;
BOOST_FOREACH(const &connect, water_connects) {
  equipments = connect.waterUseEquipment;
  BOOST_FOREACH(const &equip,   equipments) {
    if (!equip.flowRateFractionSchedule.empty()) {
      // get multiplier by doing a weighted average of occupied and unoccupied times;
      frac_sched = equip.flowRateFractionSchedule.get.name.to_s;
      mult = occ_aves[frac_sched]*frac_year_occ + unocc_aves[frac_sched]*frac_year_unocc;
    } else {
      mult = 1.0;
    }
    // get peak value and convert from m3/s to m3/yr;
    peak = equip.waterUseEquipmentDefinition.peakFlowRate*3600*8760;
    logfile.printf("DHW: %s, peak use = %1.0f m3/yr, mult = %0.2f \n",frac_sched, peak, mult);
    dhw_demand += peak * mult;
  }
}

if ((dhw_gas_sum + dhw_elec_sum) ==0.0) {
  // if the gas + electric sum = 0 then there were no DHW heaters, so set demand to 0 no matter what;
  dhw_demand = 0.0;
}

// print out the DHW fuel type, system efficiency, distribution efficiency;
logfile.printf("DHW: Fuel Type = %i, System Efficiency = %0.2f Distribution Efficiency = %0.2f, Demand = %1.2f m3/yr \n",;
    dhw_fuel_type,  dhw_system_efficiency, dhw_distribution_efficiency,dhw_demand);

LOG(Debug, "Writing Output to " << osm2iso_output_file);
// create output file and start writing out;

time=Time.new;
// get the current system clock time;
File.open(osm2iso_output_file, 'w') do |file|;

file.LOG(Debug, "#" << osm2iso_output_file << " generated by osm2iso.rb on " << input_file << " on " << time.ctime);

file.LOG(Debug, "weatherFilePath = " << weather_file_path);
file.LOG(Debug, "# Terrain class urban/city = 0.8, suburban/some shielding = 0.9, country/open = 1.0");
file.LOG(Debug, "terrainClass = " << terrain_class);
file.LOG(Debug, "# building height is in m,  floor area is in m^2, people density is m2/person");
file.LOG(Debug, "buildingHeight = " << building_height);
file.LOG(Debug, "floorArea = " << floor_area);
file.LOG(Debug, "peopleDensityOccupied = " << area_per_person_occupied);
file.LOG(Debug, "peopleDensityUnoccupied = " << area_per_person_unoccupied);

file.LOG(Debug, "buildingOccupancyFrom = " << occupancy_day_start);
file.LOG(Debug, "buildingOccupancyTo = " << occupancy_day_end);
file.LOG(Debug, "equivFullLoadOccupancyFrom = " << occupancy_hour_start);
file.LOG(Debug, "equivFullLoadOccupancyTo = " << occupancy_hour_end);

file.LOG(Debug, "# LPD, EPD, GPD all in W/m2, ext light power in W, an heat gain is W/person");
file.LOG(Debug, "lightingPowerIntensityOccupied = " << lpd_occupied);
file.LOG(Debug, "lightingPowerIntensityUnoccupied = " << lpd_unoccupied);
file.LOG(Debug, "elecPowerAppliancesOccupied = " << epd_occupied);
file.LOG(Debug, "elecPowerAppliancesUnoccupied = " << epd_unoccupied);
file.LOG(Debug, "gasPowerAppliancesOccupied = " << gpd_occupied);
file.LOG(Debug, "gasPowerAppliancesUnoccupied = " << gpd_unoccupied);
file.LOG(Debug, "exteriorLightingPower = " << exterior_lighting_power);
file.LOG(Debug, "heatGainPerPerson=" << activity_level_occ);
// when we implement occupied and unoccupied heat gain per person, we comment above and uncomment below;
//file.LOG(Debug, "heatGainPerPersonOccupied=" << activity_level_occ);
//file.LOG(Debug, "heatGainPerPersonUnoccupied=" << activity_level_unocc);

file.LOG(Debug, "# Temp set points in degrees C");
file.LOG(Debug, "heatingOccupiedSetpoint = " << heating_setpoint_occ);
file.LOG(Debug, "heatingUnoccupiedSetpoint= " << heating_setpoint_unocc);
file.LOG(Debug, "coolingOccupiedSetpoint= " << cooling_setpoint_occ);
file.LOG(Debug, "coolingUnoccupiedSetpoint= " << cooling_setpoint_unocc);

file.LOG(Debug, "# HVAC waste, heating and cooling loss factors set by HVAC type from EN 15243");
file.LOG(Debug, "hvacWasteFactor = " << hvac_waste_factor  );
file.LOG(Debug, "hvacHeatingLossFactor = " << hvac_heating_loss_factor);
file.LOG(Debug, "hvacCoolingLossFactor = " << hvac_cooling_loss_factor);

file.LOG(Debug, "# daylight sensors, occupancy sensors, illum control are set to 1 if there is no control.   See iso 15193 Annex F/G for values");
file.LOG(Debug, "daylightSensorSystem = " << daylight_sensors);
file.LOG(Debug, "lightingOccupancySensorSystem = " << occupancy_sensors);
file.LOG(Debug, "constantIlluminationControl = " << const_illum_ctrl);

file.LOG(Debug, "# COP is W/W, coolingSystemIPLV is the ratio of IPLV/COP");
file.LOG(Debug, "coolingSystemCOP = " << cooling_COP);
file.LOG(Debug, "coolingSystemIPLVToCopRatio =" << cooling_IPLVToCop_ratio);

file.LOG(Debug, "#energycarrier is 1 if electric, 2 if gas");
file.LOG(Debug, "heatingEnergyCarrier = " << heating_fuel_type;
    // heating energy, 1=electric, 0=gas);
file.LOG(Debug, "heatingSystemEfficiency = " << heating_system_efficiency);

file.LOG(Debug, "# vent type is 1 if mech, 2 if natural, 3 if mixed");
file.LOG(Debug, "ventilationType = " << ventilation_type );
file.LOG(Debug, "# ventilation flow rates in L/s");
file.LOG(Debug, "freshAirFlowRate = " << freshair_flow_rate);
file.LOG(Debug, "supplyExhaustRate = " << supply_exhaust_rate);

file.LOG(Debug, "# heatRecover = efficiency of heat recovery (0 for none), exhaustAirRecir = fraction of supply air recirculated");
file.LOG(Debug, "heatRecovery = " << heat_recovery_fraction;
    // heat recovery efficiency, 0 for no heat recovery);
file.LOG(Debug, "exhaustAirRecirculation = " << exhaust_recirculation_fraction;
    // fraction of supply air that is recirculated);

file.LOG(Debug, "# DHW demand in m3/yr use 10 m3/yr/person as a default for offices");
file.LOG(Debug, "dhwDemand =  " << dhw_demand;
    //);
    // when we implement occupied and unoccupied infiltration, we comment above and uncomment below;
    //file.LOG(Debug, "dhwDemandOccupied=" << dhw_demand_occ);
    //file.LOG(Debug, "dhwDemandUnoccupied=" << dhw_demand_unocc);

file.LOG(Debug, "# dhwDistribution efficiency all taps w/i 3m = 1, taps more than 3m = 0.8, circulation or unknown = 0.6, see NEN 2916 12.6, ");
file.LOG(Debug, "dhwSystemEfficiency = " << dhw_system_efficiency);
file.LOG(Debug, "dhwDistributionEfficiency = " << dhw_distribution_efficiency);
file.LOG(Debug, "dhwEnergyCarrier = " << dhw_fuel_type  );

file.LOG(Debug, "# Surface Heat capacity in J/K/m2");
file.LOG(Debug, "interiorHeatCapacity = " << interior_heat_capacity);
file.LOG(Debug, "exteriorHeatCapacity = " << exterior_heat_capacity);

file.LOG(Debug, "# BEM type 1= none, 2 = simple, 3=advanced with FDD");
file.LOG(Debug, "bemType = " << bem_type);

file.LOG(Debug, "# specific fan power in L/s/W,  flow control factor is energy reduction from fan control measures");
file.LOG(Debug, "specificFanPower = " << specific_fan_power << " ");

file.LOG(Debug, "# fan flow control  1= no control, 0.75 = inlet blade adjuct, 0.65= variable speed  see NEN 2916 7.3.3.4  ");
file.LOG(Debug, "fanFlowcontrolFactor = " << fan_flow_control_factor);

file.LOG(Debug, "# infiltration in m3/m2/hr based on surface area");
file.LOG(Debug, "infiltration = " << infiltration_rate;
    // leakage per unit surface area in m^3/m^2/hr);
    // when we implement occupied and unoccupied infiltration, we comment above and uncomment below;
    //file.LOG(Debug, "infiltrationOccupied=" << infiltration_rate_occ);
    //file.LOG(Debug, "infiltrationUnoccupied=" << infiltration_rate_unocc);

file.LOG(Debug, "# pump control 0= no pump, 0.5 = auto pump controls for more 50% of pumps, 1.0 = all other cases.   See NEN 2914 9.4.3");
file.LOG(Debug, "heatingPumpControl=" << heating_pump_control_factor);
file.LOG(Debug, "coolingPumpControl=" << cooling_pump_control_factor);

// write out roof and skylight info;
file.LOG(Debug, "# Areas in m2, U values in W/m2/K");
file.LOG(Debug, "# SCF = solar control factor (external control), SDF = shading device factor = internal shading");
file.LOG(Debug, "roofArea = " << roof_area);
file.LOG(Debug, "roofUValue = " << roof_U);
file.LOG(Debug, "roofSolarAbsorption = " << roof_solar_absorption);
file.LOG(Debug, "roofThermalEmissivity= " << roof_thermal_emissivity);

file.LOG(Debug, "skylightArea = " << skylight_area);
file.LOG(Debug, "skylightUValue = " << skylight_U);
file.LOG(Debug, "skylightSHGC = " << skylight_SHGC);

// write out the wall and window info;
for (int i = 0; i <= 7; ++i) {
  file.LOG(Debug, "WallArea" << direction_names[i] << " = " << wall_areas[i]);
  file.LOG(Debug, "WallUvalue" << direction_names[i] << " = " << wall_U[i]);
  file.LOG(Debug, "WallSolarAbsorption" << direction_names[i] << " = " << wall_solar_absorption[i]);
  file.LOG(Debug, "WallThermalEmissivity" << direction_names[i] << " = " << wall_thermal_emissivity[i]);
}
for (int i = 0; i <= 7; ++i) {
  file.LOG(Debug, "WindowArea" << direction_names[i] << " = " << window_areas[i]);
  file.LOG(Debug, "WindowUvalue" << direction_names[i] << " = " << window_U[i]);
  file.LOG(Debug, "WindowSHGC" << direction_names[i] << " = " << window_SHGC[i]);
  file.LOG(Debug, "WindowSCF" << direction_names[i] << " = " << window_SCF[i]  );
  file.LOG(Debug, "WindowSDF" << direction_names[i] << " = " << window_SDF[i]      );
}
}
logfile.close;
LOG(Debug, "Done Converting!");
