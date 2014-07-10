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
#   Take OpenStudio Model which has been run using DaylightSim and create lighting
#   schedules for each zone.
#
# == Usage
#
#  ruby MakeSchedules.rb ARGV[0] (ARGV[1]) 
#
#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Path to EnergyPlus SQLite output.
#
# == Examples
#
#   ruby MakeSchedules.rb 'C:\path\to\model.osm' 'C:\path\to\eplusout.sql'
#
######################################################################

require 'openstudio'
require 'csv'
require 'optparse'
require 'ostruct'

if ARGV.length < 2
  puts "Usage: ruby MakeSchedules.rb 'C:\\path\\to\\model.osm' 'C:\\path\\to\\eplusout.sql'"
  exit -1
end

modelPath = OpenStudio::Path.new(ARGV[0])
modelPath = OpenStudio::system_complete(modelPath)
sqlPath = OpenStudio::Path.new(ARGV[1])

class Optparse

  #
  # Return a structure describing the options.
  #
  def self.parse(args)
    # The options specified on the command line will be collected in *options*.
    # We set default values here.
    options = OpenStruct.new
    options.verbose = false

    opts = OptionParser.new do |opts|
      opts.banner = "Usage: ruby MakeSchedules.rb 'C:\\path\\to\\model.osm' 'C:\\path\\to\\eplusout.sql'"

      opts.separator ""
      opts.separator " Options:"

      # Optionally keep original osm input file (and schedule)
      options.keep = false
      opts.on( '-k', '--keep', "Keep original osm file and schedule" ) do
        options.keep = true
      end      
      
      # Use workplane average for setpoint instead of sensor point
      options.setpointInput = false
      opts.on( '-a', '--average', "use workplane average illuminance for setpoint input") do |i|
        options.setpointInput = true
      end      

      # Goldwasser-style stdout
      options.verbose = false
      opts.on( '-v', '--verbose', "Verbose mode" ) do
        options.verbose = true
      end      
 
      # No argument, shows at tail.  This will print an options summary.
      opts.on_tail("-h", "--help", "Show this message") do
        puts opts
        exit
      end

    end

    opts.parse!(args)
    options
  end  # parse()

end  # class Optparse

options = Optparse.parse(ARGV)

if options.verbose == true
  puts "Dimming setpoint is #{options.setpoint} lux"
end

# load the model
model = OpenStudio::Model::Model::load(OpenStudio::Path.new(ARGV[0]))
if model.empty?
  puts "Model cannot be read"
  return false
end
model = model.get

# load the sql file
sqlFile = OpenStudio::SqlFile.new(sqlPath)
if not sqlFile.connectionOpen
  puts "SqlFile #{sqlPath} connection is not open"
  return false
end

# set the sql file
model.setSqlFile(sqlFile)
if model.sqlFile.empty?
  puts "Model's SqlFile is not initialized"
  return false
end

# model paths
modelPath = OpenStudio::Path.new(ARGV[0]) 
modelPath = OpenStudio::system_complete(modelPath)  
workPath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem) / OpenStudio::Path.new("model") /OpenStudio::Path.new("radiance")

# only run period in pre process job
environmentName = "Run Period 1"



# loop through each thermal zone
model.getThermalZones.each do |thermalZone|

  spaces = thermalZone.spaces

  if spaces.empty?
    puts "ThermalZone '#{thermalZone.name}' has no spaces, skipping."
    next
  end
  
  # get people schedule for zone
  # TODO: require people for occupancy controls
  peopleTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone People Occupant Count", thermalZone.name.get.upcase)
  
  if peopleTimeseries.empty?
    puts "Cannot find timeseries 'Zone People Occupant Count' for ThermalZone '#{thermalZone.name}'."
  end
  
  # get lights schedule for zone
  lightsTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone Lights Electric Power", thermalZone.name.get.upcase)
  
  if lightsTimeseries.empty?
    newname = thermalZone.name.get.sub(/^OS:/, '');
    puts "Cannot find timeseries 'Zone Lights Electric Power' for ThermalZone '#{thermalZone.name}', skipping."
    next
  end
  
  lightsTimeseries = lightsTimeseries.get
   
  # get illuminance map
  illuminanceMap = thermalZone.illuminanceMap

  if illuminanceMap.empty?
    puts "Cannot find IlluminanceMap for ThermalZone '#{thermalZone.name}', skipping."
    next
  end

  illuminanceMap = illuminanceMap.get

  # get the space
  space = illuminanceMap.space
  
  if space.empty?
    puts "Cannot find Space for IlluminanceMap '#{illuminanceMap.name}' in ThermalZone '#{thermalZone.name}', skipping."
    next
  end
  
  space = space.get
  
  space_name = space.name.get.gsub(' ', '_').gsub(':', '_')

  radSqlPath = workPath / OpenStudio::Path.new("/output/radout.sql")

  # load the illuminance map
  # assume this will be reported in 1 hour timesteps starting on 1/1
  averageIlluminances = []
  radSqlFile = OpenStudio::SqlFile.new(radSqlPath)

  if options.verbose == true
    puts "Loading radiance data file from " + radSqlPath.to_s
  end

  if options.setpointInput == true
    # we have to calculate the average ourselves
    reportIndices = radSqlFile.illuminanceMapHourlyReportIndices(space_name)

    reportIndices.each do |index|

      map = radSqlFile.illuminanceMap(index)
      #  averageIlluminances << OpenStudio::mean(map)
      sum = 0
      illuminances = Array.new

      # we have to normalize the values to 0 if there's any that are less than 0
      map.size1().times do |x|
        map.size2().times do |y|

          illuminance = map[x, y]

          if illuminance < 0
            if options.verbose == true
              puts "Warning illuminance #{illuminance} less than zero, will be reset to 0"
            end
            illuminance = 0
          end

          illuminances << illuminance
          sum += illuminance
        end
      end
      if options.verbose == true
        puts "Average illuminance " + (sum / illuminances.size.to_f).to_s
      end
      averageIlluminances << sum/illuminances.size.to_f
    end
  else 
    # use the daylight sensor input
    spacename = space.name.get.gsub(' ', '_').gsub(':', '_')
    envPeriods = radSqlFile.availableEnvPeriods

    if envPeriods.size == 0
      puts "No available environment periods in radiance sql file, skipping"
      next
    end

    if options.verbose == true
      puts "Using environment period " + envPeriods[0]
    end
    daylightSensor = radSqlFile.timeSeries(envPeriods[0], "Hourly", "Daylight Sensor Illuminance", space_name)
    
    if daylightSensor.empty?
      puts "Daylight sensor data could not be loaded, skipping"
      next
    end

    values = daylightSensor.get.values

    values.length.times do |i|
      val = values[i];
      if options.verbose == true
        puts "Daylight sensor illuminance " + val.to_s
      end

      if val < 0
        if options.verbose == true
          puts "Warning illuminance #{val} less than zero, will be reset to 0"
        end
        val = 0
      end
      averageIlluminances << val
    end
   end

  daylightSetpoint = 0.0

  primaryDaylightingControl = thermalZone.primaryDaylightingControl
  if not primaryDaylightingControl.empty?
    daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
  end

    secondaryDaylightingControl = thermalZone.secondaryDaylightingControl
    if not secondaryDaylightingControl.empty?
      if daylightSetpoint == 0.0
        daylightSetpoint = secondaryDaylightingControl.get.illuminanceSetpoint
      else
        "Ignoring secondary daylighting control in ThermalZone '#{thermalZone.name}'"
    end
  end
  
  if daylightSetpoint == 0.0
    space.daylightingControls.each do |i|
      daylightSetpoint = i.illuminanceSetpoint
      if daylightSetpoint != 0.0
        break
      end
    end
  end
  
  if daylightSetpoint == 0.0
    puts "Illuminance setpoint is not defined in Space '#{space.name}' or in ThermalZone '#{thermalZone.name}', skipping."
    next
  end
  
  puts "ThermalZone '#{thermalZone.name}' illuminance setpoint is: #{daylightSetpoint} lux"
  
  originalLightsValues = lightsTimeseries.values
  lightsValues = OpenStudio::Vector.new(averageIlluminances.size)
  averageIlluminances.each_index do |i|
    dimmingResponse = [(daylightSetpoint-averageIlluminances[i])/daylightSetpoint, 0].max
    #puts "#{daylightSetpoint}, #{averageIlluminances[i]}, #{dimmingResponse}"
    lightsValues[i] = dimmingResponse*originalLightsValues[i]

#    is_occupied = false
#    if not peopleTimeseries.empty?
#      is_occupied = peopleTimeseries.get.values[i] > 0
#    end
#    
#    daValues[i] = is_occupied*(averageIlluminances[i] > daylightSetpoint)
  end
  
  # get max lighting power
  lightingLevel = OpenStudio::maximum(lightsValues)
  
  if lightingLevel <= 0.0
    puts "ThermalZone '#{thermalZone.name}' lighting level is less than or equal to 0, skipping"
    next
  end
  
  puts "ThermalZone '#{thermalZone.name}' lighting level is: #{lightingLevel} W"
  
  # normalize lights values
  averageIlluminances.each_index do |i|
    lightsValues[i] = lightsValues[i]/lightingLevel
  end
  
  startDate = OpenStudio::Date.new(OpenStudio::MonthOfYear.new(1), 1)
  interval = OpenStudio::Time.new(0,1,0)
  timeseries = OpenStudio::TimeSeries.new(startDate, interval, lightsValues, "W")

  schedule = OpenStudio::Model::ScheduleInterval::fromTimeSeries(timeseries, model)
  
  if schedule.empty?
    puts "Could not create modified lighting schedule for ThermalZone '#{thermalZone.name}', skipping"
    next
  end
  
  schedule = schedule.get
  
  schedule.setName(thermalZone.name.get + " Lights Schedule")
  
  # remove all lights in this zone
  spaces.each do |space|
    space.hardApplySpaceType(true)
    space.lights.each do |light|
      light.remove
    end
    space.luminaires.each do |luminaire|
      luminaire.remove
    end
  end
  
  # add a new lights object to first space in this zone and set schedule
  lightsDefinition = OpenStudio::Model::LightsDefinition.new(model)
  lightsDefinition.setLightingLevel(lightingLevel)
  
  lights = OpenStudio::Model::Lights.new(lightsDefinition)
  lights.setSchedule(schedule)
  lights.setSpace(spaces[0])
  
  # remove illuminance map and daylighting controls from thermal zone to prevent double counting
  thermalZone.resetPrimaryDaylightingControl
  thermalZone.resetSecondaryDaylightingControl
  thermalZone.resetIlluminanceMap
end
  
#write new model   
if options.keep
  # write to working directory
  outPath = workPath / OpenStudio::Path.new("out.osm")
  model.save(outPath, true)
else
  # overwrite existing file
  outDir = File.dirname(modelPath.to_s)
  outName = File.basename(modelPath.to_s, '.osm')
  outPath = OpenStudio::Path.new(outDir) / OpenStudio::Path.new(outName + ".osm")
  model.save(outPath, true)
end
