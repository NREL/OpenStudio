
#make some units that we'll use
$GJ_unit = OpenStudio::createUnit("GJ", "Celsius".to_UnitSystem).get
$m2_unit = OpenStudio::createUnit("m^2", "Celsius".to_UnitSystem).get
$kBtu_unit = OpenStudio::createUnit("kBtu", "BTU".to_UnitSystem).get
$ft2_unit = OpenStudio::createUnit("ft^2", "BTU".to_UnitSystem).get

#define some timesteps that we'll use over and over   
$zone_time_step = "Zone Timestep"
$hourly_time_step = "Hourly"
$hvac_time_step = "HVAC System Timestep"

#some generic abbreviations for flag naming
$eda = "Xcel EDA"
$gen_mod = "General Modeling" 
