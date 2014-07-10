######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
#  All rights reserved.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis
#
#   Take translated EnergyPlus building model and run Radiance daylight simulations
#
# == Usage
#
#  ruby DaylightMetrics.rb ARGV[0] ARGV[1] ARGV[2]

#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Path to EnergyPlus SQLite output.
#            EnergyPlus simulation must be pre-run with the SQLite output including tabular data
#            and the EnvelopeSummary table must be requested.  The "Exterior
#            Horizontal Illuminance From Sky", and "Exterior Beam Normal
#            Illuminance" output variables must be requested at hourly reporting
#            frequency.
#
#  ARGV[2] - Path to Radiance SQLite output.
#
# == Examples
#
#   ruby DaylightMetrics.rb 'C:\path\to\model.osm' 'C:\path\to\eplusout.sql' 'C:\path\to\radout.sql'
#
######################################################################

require 'openstudio'
require 'fileutils'

$METHOD = 1

def writeTimeSeriesToSql(sqlfile, simDateTimes, illum, space_name, ts_name, ts_units)
  data = OpenStudio::Vector.new(illum.length)
  illum.length.times { |n|
    begin
      data[n] = illum[n].to_f;
    rescue Exception => e
      puts "Error inserting data: " + illum[n] + " inserting 0 instead";
      data[n] = 0;
    end
  }
  illumTS = OpenStudio::TimeSeries.new(simDateTimes, data, ts_units);
  sqlfile.insertTimeSeriesData(
    "Average", "Zone", "Zone", space_name, ts_name, OpenStudio::ReportingFrequency.new("Hourly"),
    OpenStudio::OptionalString.new(),
    ts_units, illumTS);
end

modelPath = OpenStudio::Path.new(ARGV[0])
modelPath = OpenStudio::system_complete(modelPath)

eplusoutPath = OpenStudio::Path.new(ARGV[1])
eplusoutPath = OpenStudio::system_complete(eplusoutPath)

radoutPath = OpenStudio::Path.new(ARGV[2])
radoutPath = OpenStudio::system_complete(radoutPath)

puts "modelPath=#{modelPath}"
puts "eplusoutPath=#{eplusoutPath}"
puts "radoutPath=#{radoutPath}"

# get model
idf = OpenStudio::IdfFile.load(modelPath)
model = OpenStudio::Model::Model.new(idf.get)

# load the sql files
eplusoutFile = OpenStudio::SqlFile.new(eplusoutPath)
if not eplusoutFile.connectionOpen
  puts "SqlFile #{eplusoutFile} connection is not open"
  return false
end

radoutFile = OpenStudio::SqlFile.new(radoutPath)
if not radoutFile.connectionOpen
  puts "SqlFile #{radoutFile} connection is not open"
  return false
end

# set the sql file for the model
model.setSqlFile(eplusoutFile)
if model.sqlFile.empty?
  puts "Model's SqlFile is not initialized"
  return false
end

# get exterior illuminance timeseries from E+ run
exteriorIlluminanceTimeseries = eplusoutFile.timeSeries("Run Period 1".upcase, "Hourly", "Site Exterior Horizontal Sky Illuminance")

# summary report string
summary_report = ""

building_average_space = []
building_average = ""

# loop through all the spaces
building = model.getBuilding
building.spaces.each do |space|

  space_name = space.name.get.gsub(' ', '_').gsub(':', '_')
  
  thermalZone = space.thermalZone
  next if thermalZone.empty?
  thermalZone = thermalZone.get
  
  map_name = "#{space_name} DAYLIGHT MAP"
  map_index = radoutFile.illuminanceMapIndex(map_name)
  next if map_index.empty?
  
  daylightSetpoint = nil
  primaryDaylightingControl = thermalZone.primaryDaylightingControl
  if primaryDaylightingControl.empty?
    puts "Cannot determine daylighting setpoint for ThermalZone #{thermalZone.name.get}, skipping"
    next
  else
    daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
  end
  
  puts "Calculating Daylight Metrics for Space #{space_name}"
  
  da_daylit = []
  da_occupied = []
  da_daylit_occupied = []
  cda_daylit = []
  cda_occupied = []
  cda_daylit_occupied = []
  udi_daylit = []
  udi_occupied = []
  udi_daylit_occupied = []
  
  
  # get people timeseries from E+ run for this zone
  peopleTimeseries = eplusoutFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone People Occupant Count", thermalZone.name.get.upcase)
  
  # loop over all timesteps, return type is std::vector< std::pair<int, DateTime> > 
  hourly_report_indices_dates = radoutFile.illuminanceMapHourlyReportIndicesDates(map_name)
  hourly_report_indices_dates.each do |hourly_report_index_date| 
    
    # initialize metrics to nil for timestep
    da_daylit << nil
    da_occupied << nil
    da_daylit_occupied << nil
    cda_daylit << nil
    cda_occupied << nil
    cda_daylit_occupied << nil
    udi_daylit << nil
    udi_occupied << nil
    udi_daylit_occupied << nil
  
    # extract timestep and index
    hourly_report_index = hourly_report_index_date.first
    hourly_report_date = hourly_report_index_date.second
    # puts "Calculating Daylight Metrics for Space #{space_name} at #{hourly_report_date}"
    
    # see if this is a daylit hour
    daylit_hour = false
    if not exteriorIlluminanceTimeseries.empty?
      val = exteriorIlluminanceTimeseries[0].value(hourly_report_date)
      if val > 0
        daylit_hour = true
        #puts "daylight!"
      end
    end
    
    # see if this is an occupied hour
    occupied_hour = false
    if not peopleTimeseries.empty?
      val = peopleTimeseries.get.value(hourly_report_date)
      if val > 0
        occupied_hour = true
      end
    end
  
    da = 0
    if $METHOD == 0
    
      # get map values
      map_values = radoutFile.illuminanceMap(hourly_report_index)
      
      # compute number of map points with illuminance greater than setpoint
      size1 = map_values.size1
      size2 = map_values.size2
      num = size1*size2
      num_da = 0
      for i in (0...size1)
        for j in (0...size2)
          map_value = map_values[i,j]
          if map_value >= daylightSetpoint
            num_da += 1
          end
        end
      end
      
      da = num_da.to_f / num.to_f
      
    elsif $METHOD == 1
    
      x = OpenStudio::DoubleVector.new
      y = OpenStudio::DoubleVector.new
      map_values = OpenStudio::DoubleVector.new
      
      radoutFile.illuminanceMap(hourly_report_index, x, y, map_values)
      
      # compute DA, conDA, and UDI
      num = map_values.size
      num_da = 0
      num_cda = 0
      num_udi = 0
      map_values.each do |map_value|
        if map_value >= daylightSetpoint
          num_da += 1
          num_cda += 1
        end
        if map_value > 0 and map_value < daylightSetpoint  
          num_cda += map_value / daylightSetpoint 
        end
        if map_value > 100 and map_value < 2000
          num_udi += 1
        end
      end
      
      da = num_da.to_f / num.to_f
      cda = num_cda.to_f / num.to_f
      udi = num_udi.to_f / num.to_f

    end

    # assign to timeseries
    if daylit_hour
      da_daylit[-1] = da
      cda_daylit[-1] = cda
      udi_daylit[-1] = udi
    end
    
    if occupied_hour
      da_occupied[-1] = da
      cda_occupied[-1] = cda
      udi_occupied[-1] = udi
    end
    
    if daylit_hour and occupied_hour
      da_daylit_occupied[-1] = da
      cda_daylit_occupied[-1] = cda
      udi_daylit_occupied[-1] = udi
    end

  end
  
  # compute annual metrics for space
  
  #Daylight Autonomy
  da_daylit_sum = 0
  da_daylit_num = 0
  da_daylit.each do |da|
    if not da.nil?
      da_daylit_sum += da
      da_daylit_num += 1
    end
  end
  annual_da_daylit = da_daylit_sum.to_f / da_daylit_num.to_f
  summary_report += "#{space_name}: DA(#{daylightSetpoint}) Daylit Hours, #{da_daylit_sum}, #{da_daylit_num}, #{annual_da_daylit}\n"
  
  da_occupied_sum = 0
  da_occupied_num = 0
  da_occupied.each do |da|
    if not da.nil?
      da_occupied_sum += da
      da_occupied_num += 1
    end
  end
  annual_da_occupied = da_occupied_sum.to_f / da_occupied_num.to_f
  summary_report += "#{space_name}: DA(#{daylightSetpoint}) Occupied Hours, #{da_occupied_sum}, #{da_occupied_num}, #{annual_da_occupied}\n"
  
  da_daylit_occupied_sum = 0
  da_daylit_occupied_num = 0
  da_daylit_occupied.each do |da|
    if not da.nil?
      da_daylit_occupied_sum += da
      da_daylit_occupied_num += 1
    end
  end
  annual_da_daylit_occupied = da_daylit_occupied_sum.to_f / da_daylit_occupied_num.to_f
  summary_report += "#{space_name}: DA(#{daylightSetpoint}) Daylit and Occupied Hours, #{da_daylit_occupied_sum}, #{da_daylit_occupied_num}, #{annual_da_daylit_occupied}\n"
  
  #Continuous Daylight Autonomy
  cda_daylit_sum = 0
  cda_daylit_num = 0
  cda_daylit.each do |cda|
    if not cda.nil?
      cda_daylit_sum += cda
      cda_daylit_num += 1
    end
  end
  annual_cda_daylit = cda_daylit_sum.to_f / cda_daylit_num.to_f
  summary_report += "#{space_name}: conDA(#{daylightSetpoint}) Daylit Hours, #{cda_daylit_sum}, #{cda_daylit_num}, #{annual_cda_daylit}\n"
  
  cda_occupied_sum = 0
  cda_occupied_num = 0
  cda_occupied.each do |cda|
    if not cda.nil?
      cda_occupied_sum += cda
      cda_occupied_num += 1
    end
  end
  annual_cda_occupied = cda_occupied_sum.to_f / cda_occupied_num.to_f
  summary_report += "#{space_name}: conDA(#{daylightSetpoint}) Occupied Hours, #{cda_occupied_sum}, #{cda_occupied_num}, #{annual_cda_occupied}\n"
  
  cda_daylit_occupied_sum = 0
  cda_daylit_occupied_num = 0
  cda_daylit_occupied.each do |cda|
    if not cda.nil?
      cda_daylit_occupied_sum += cda
      cda_daylit_occupied_num += 1
    end
  end
  annual_cda_daylit_occupied = cda_daylit_occupied_sum.to_f / cda_daylit_occupied_num.to_f
  summary_report += "#{space_name}: conDA(#{daylightSetpoint}) Daylit and Occupied Hours, #{cda_daylit_occupied_sum}, #{cda_daylit_occupied_num}, #{annual_cda_daylit_occupied}\n"

 # Useful Daylight Illuminance
  udi_daylit_sum = 0
  udi_daylit_num = 0
  udi_daylit.each do |udi|
    if not udi.nil?
      udi_daylit_sum += udi
      udi_daylit_num += 1
    end
  end
  annual_udi_daylit = udi_daylit_sum.to_f / udi_daylit_num.to_f
  summary_report += "#{space_name}: UDI Daylit Hours, #{udi_daylit_sum}, #{udi_daylit_num}, #{annual_udi_daylit}\n"
  
  udi_occupied_sum = 0
  udi_occupied_num = 0
  udi_occupied.each do |udi|
    if not udi.nil?
      udi_occupied_sum += udi
      udi_occupied_num += 1
    end
  end
  annual_udi_occupied = udi_occupied_sum.to_f / udi_occupied_num.to_f
  summary_report += "#{space_name}: UDI Occupied Hours, #{udi_occupied_sum}, #{udi_occupied_num}, #{annual_udi_occupied}\n"
  
  udi_daylit_occupied_sum = 0
  udi_daylit_occupied_num = 0
  udi_daylit_occupied.each do |udi|
    if not udi.nil?
      udi_daylit_occupied_sum += udi
      udi_daylit_occupied_num += 1
    end
  end
  annual_udi_daylit_occupied = udi_daylit_occupied_sum.to_f / cda_daylit_occupied_num.to_f
  summary_report += "#{space_name}: UDI Daylit and Occupied Hours, #{cda_daylit_occupied_sum}, #{cda_daylit_occupied_num}, #{annual_udi_daylit_occupied}\n"

  # now replace nil with 0 in each timeseries to radout.sql for plotting
  
  building_average_space << annual_da_daylit_occupied
  
end

# DLM: can we make some more metrics that are area weighted rather than just space weighted?
building_average_space_sum = 0.0
building_average_space.each {|e| building_average_space_sum += e}

# catch zero condition
if building_average_space_sum == 0.0
  building_average = 0.0
  puts "warning: Daylight Autonomy for building is zero, check daylighting control point(s) setup."
else
  building_average = building_average_space_sum / building_average_space.length
end

File.open('./DaylightingMetrics.csv', 'w') do |file|
  file.puts "Building average daylight autonomy,#{building_average}"
  file.puts summary_report
end
