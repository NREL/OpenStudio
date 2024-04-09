# To regenerate OpenSutdio/src/workflow/RunPreProcessMonthlyReports.hpp
require 'openstudio'

include OpenStudio::Model

excluded_EUTs = ["HeatingCoils", "CoolingCoils", "Boilers", "Baseboard", "HeatRecoveryForCooling", "HeatRecoveryForHeating"].map{|s| OpenStudio::EndUseType.new(s)}
included_EUTs = OpenStudio::EndUseType.getValues.map{|i| OpenStudio::EndUseType.new(i)}.reject{|x| excluded_EUTs.include?(x)}
included_EUTs.map(&:valueDescription)

# excluded_FTs= ["Solar", "Geothermal"].map{|s| OpenStudio::FuelType.new(s)} # These are rejected by OutputMeter
# included_FTs = OpenStudio::FuelType.getValues.map{|i| OpenStudio::FuelType.new(i)}.reject{|x| excluded_FTs.include?(x)}.map(&:valueDescription)
included_FTs = ["Electricity", "NaturalGas", "DistrictHeatingWater", "DistrictCooling"].map{|s| OpenStudio::FuelType.new(s)}

c_monthlyReports = ""

specificEndUseType = OpenStudio::OptionalString.new()
installLocationType = OpenStudio::OptionalInstallLocationType.new()
specificInstallLocation = OpenStudio::OptionalString.new()
ft_excludes_euts = {
  OpenStudio::FuelType.new("NaturalGas").value => ["InteriorLights", "ExteriorLights", "Fans", "Pumps", "HeatRejection", "Humidifier", "HeatRecovery", "Refrigeration"].map{|s| OpenStudio::EndUseType.new(s)},
  OpenStudio::FuelType.new("DistrictHeatingWater").value => ["Refrigeration"].map{|s| OpenStudio::EndUseType.new(s)},
  OpenStudio::FuelType.new("DistrictCooling").value => ["Refrigeration"].map{|s| OpenStudio::EndUseType.new(s)},
}

w = OpenStudio::Workspace.new("None".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
included_FTs.each do |ft|
  i = w.addObject(OpenStudio::IdfObject.new("Output:Table:Monthly".to_IddObjectType)).get

  table_name = OpenStudio::EndUseFuelType.new(ft.valueName).valueDescription
  i.setName("Building Energy Performance - #{table_name}")
  i.setInt(1, 2) # Digits after decimal
  this_included_EUTs = included_EUTs
  if ft_excludes_euts.include?(ft.value)
    this_included_EUTs = this_included_EUTs.reject{|x| ft_excludes_euts[ft.value].include?(x)}
  end
  this_included_EUTs.each do |eut|
    ig = i.pushExtensibleGroup
    name = OutputMeter::getName(specificEndUseType, OpenStudio::OptionalEndUseType.new(eut), OpenStudio::OptionalFuelType.new(ft), installLocationType, specificInstallLocation)
    ig.setString(0, name)
    ig.setString(1, "SumOrAverage")
  end

  c_monthlyReports << "\n" << '  R"idf(' << "\n" << i.to_s.chomp << ')idf",'
end

facilityLoc = OpenStudio::OptionalInstallLocationType.new(OpenStudio::InstallLocationType.new("Facility"))
included_FTs.each do |ft|
  i = w.addObject(OpenStudio::IdfObject.new("Output:Table:Monthly".to_IddObjectType)).get

  table_name = OpenStudio::EndUseFuelType.new(ft.valueName).valueDescription
  i.setName("Building Energy Performance - #{table_name} Peak Demand")
  i.setInt(1, 2) # Digits after decimal
  ig = i.pushExtensibleGroup
  facility_meter_name = OutputMeter::getName(specificEndUseType, OpenStudio::OptionalEndUseType.new(), OpenStudio::OptionalFuelType.new(ft), facilityLoc, specificInstallLocation)

  ig.setString(0, facility_meter_name)
  ig.setString(1, "Maximum")

  this_included_EUTs = included_EUTs
  if ft_excludes_euts.include?(ft.value)
    this_included_EUTs = this_included_EUTs.reject{|x| ft_excludes_euts[ft.value].include?(x)}
  end
  this_included_EUTs.each do |eut|
    ig = i.pushExtensibleGroup
    name = OutputMeter::getName(specificEndUseType, OpenStudio::OptionalEndUseType.new(eut), OpenStudio::OptionalFuelType.new(ft), installLocationType, specificInstallLocation)
    ig.setString(0, name)
    ig.setString(1, "ValueWhenMaximumOrMinimum")
  end

  c_monthlyReports << "\n" << '  R"idf(' << "\n" << i.to_s.chomp << ')idf",'
end

n_tables = included_FTs.size * 2
cpp_code = """/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP
#define WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP

#include <array>
#include <string_view>

// NOTE: see OpenStudio/developer/ruby/GeneratePreProcessMonthlyReports.rb

"""
cpp_code << "static constexpr std::array<std::string_view, #{n_tables}> c_monthlyReports = {"
cpp_code << c_monthlyReports.chop << "};"

calib_meterfileonlys = [
  ["NaturalGas", "Daily"],
  ["Electricity", "Timestep"],
  ["Electricity", "Daily"],
]

n_metersForced = calib_meterfileonlys.size + included_FTs.size
cpp_code << "\n\nstatic constexpr std::array<std::string_view, #{n_metersForced}> c_metersForced{\n"
cpp_code << "  // These are needed for the calibration report\n"

calib_meterfileonlys.each do |ft_name, freq|
  ft = OpenStudio::FuelType.new(ft_name)
  facility_meter_name = OutputMeter::getName(specificEndUseType, OpenStudio::OptionalEndUseType.new(), OpenStudio::OptionalFuelType.new(ft), facilityLoc, specificInstallLocation)
  cpp_code << '  "Output:Meter:MeterFileOnly,' << facility_meter_name << "," << freq << ';",' << "\n"
end

cpp_code << "\n  // Always add in the timestep facility meters\n"

included_FTs.each do |ft|
  facility_meter_name = OutputMeter::getName(specificEndUseType, OpenStudio::OptionalEndUseType.new(), OpenStudio::OptionalFuelType.new(ft), facilityLoc, specificInstallLocation)
  cpp_code << '  "Output:Meter,' << facility_meter_name << ',Timestep;",' << "\n"
end
cpp_code << "};\n\n"
cpp_code << "#endif  // WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP\n"

ROOT_DIR = File.absolute_path(File.join(File.dirname(__FILE__), "../../"))
PREPROCESS_FILE = File.join(ROOT_DIR, "src/workflow/RunPreProcessMonthlyReports.hpp")
raise "File #{PREPROCESS_FILE} does not exist" unless File.exist?(PREPROCESS_FILE)
# puts cpp_code
File.write(PREPROCESS_FILE, cpp_code)

# Save to workflow-gem and also paste the text into workflow/util/energyplus.rb
# w.save('/path/to/OpenStudio-workflow-gem/lib/openstudio/workflow/jobs/resources/monthly_report.idf', true)
