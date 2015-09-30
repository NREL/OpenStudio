require 'fileutils'

if not ARGV[0]
  raise "Need to specify output directory"
end

outDir = ARGV[0].gsub("\\", "/")
logDir = outDir + "/logs"
puts outDir
FileUtils::rm_rf(outDir) if File::exists?(outDir)
FileUtils::mkdir_p(outDir)
FileUtils::mkdir_p(logDir)

objects = []
# idd name, class name, baseclass name

objects << ["OS:Meter", "Meter", "ModelObject"]

objects << ["OS:Schedule:Constant", "ScheduleConstant", "Schedule"]
objects << ["OS:Schedule:Ruleset", "ScheduleRuleset", "Schedule"]
objects << ["OS:Schedule:Rule", "ScheduleRule", "ParentObject"]
objects << ["OS:ConvergenceLimits", "ConvergenceLimits", "ModelObject"]
objects << ["OS:HeatBalanceAlgorithm", "HeatBalanceAlgorithm", "ModelObject"]
objects << ["OS:RunPeriod", "RunPeriod", "ParentObject"]
objects << ["OS:RunPeriodControl:DaylightSavingTime", "DaylightSavingTime", "ModelObject"]
objects << ["OS:RunPeriodControl:SpecialDays", "RunPeriodControlSpecialDays", "ModelObject"]
objects << ["OS:ShadowCalculation", "ShadowCalculation", "ModelObject"]
objects << ["OS:SimulationControl", "SimulationControl", "ParentObject"]
objects << ["OS:Sizing:Parameters", "SizingParameters", "ModelObject"]
objects << ["OS:SurfaceConvectionAlgorithm:Inside", "InsideSurfaceConvectionAlgorithm", "ModelObject"]    
objects << ["OS:SurfaceConvectionAlgorithm:Outside", "OutsideSurfaceConvectionAlgorithm", "ModelObject"]    
objects << ["OS:Timestep", "Timestep", "ModelObject"]    
objects << ["OS:YearDescription", "YearDescription", "ModelObject"]
objects << ["OS:ZoneAirContaminantBalance", "ZoneAirContaminantBalance", "ModelObject"]    
objects << ["OS:ZoneAirHeatBalanceAlgorithm", "ZoneAirHeatBalanceAlgorithm", "ModelObject"]    
objects << ["OS:ZoneCapacitanceMultiplier:ResearchSpecial", "ZoneCapacitanceMultiplierResearchSpecial", "ModelObject"]    

objects << ["OS:Schedule:Day", "ScheduleDay", "ResourceObject"]
objects << ["OS:Schedule:Week", "ScheduleWeek", "ResourceObject"]
objects << ["OS:Schedule:Year", "ScheduleYear", "Schedule"]
objects << ["OS:Site", "Site", "ParentObject"]
objects << ["OS:Site:GroundTemperature:BuildingSurface", "SiteGroundTemperatureBuildingSurface", "ModelObject"]
objects << ["OS:Site:WaterMainsTemperature", "SiteWaterMainsTemperature", "ModelObject"]
objects << ["OS:WeatherFile", "WeatherFile", "ModelObject"]

objects << ["OS:BuildingStory", "BuildingStory", "ResourceObject"]
objects << ["OS:DefaultConstructionSet", "DefaultConstructionSet", "ResourceObject"]
objects << ["OS:DefaultScheduleSet", "DefaultScheduleSet", "ResourceObject"]
objects << ["OS:DefaultSubSurfaceConstructions", "DefaultSubSurfaceConstructions", "ResourceObject"]
objects << ["OS:DefaultSurfaceConstructions", "DefaultSurfaceConstructions", "ResourceObject"]
objects << ["OS:ElectricEquipment:Definition", "ElectricEquipmentDefinition", "SpaceLoadDefinition"]
objects << ["OS:GasEquipment:Definition", "GasEquipmentDefinition", "SpaceLoadDefinition"]
objects << ["OS:HotWaterEquipment:Definition", "HotWaterEquipmentDefinition", "SpaceLoadDefinition"]
objects << ["OS:InternalMass:Definition", "InternalMassDefinition", "SpaceLoadDefinition"]
objects << ["OS:Lights:Definition", "LightsDefinition", "SpaceLoadDefinition"]
objects << ["OS:Luminaire:Definition", "LuminaireDefinition", "SpaceLoadDefinition"]
objects << ["OS:People:Definition", "PeopleDefinition", "SpaceLoadDefinition"]
objects << ["OS:Rendering:Color", "RenderingColor", "ResourceObject"]
objects << ["OS:SpaceType", "SpaceType", "ResourceObject"]

objects << ["OS:Building", "Building", "ParentObject"]
objects << ["OS:Daylighting:Control", "DaylightingControl", "SpaceItem"]
objects << ["OS:DaylightingDevice:Shelf", "DaylightingDeviceShelf", "SpaceItem"]
objects << ["OS:ElectricEquipment", "ElectricEquipment", "SpaceLoadInstance"]
objects << ["OS:Facility", "Facility", "ParentObject"]
objects << ["OS:GasEquipment", "GasEquipment", "SpaceLoadInstance"]
objects << ["OS:HotWaterEquipment", "HotWaterEquipment", "SpaceLoadInstance"]
objects << ["OS:IlluminanceMap", "IlluminanceMap", "SpaceItem"]
objects << ["OS:InteriorPartitionSurfaceGroup", "InteriorPartitionSurfaceGroup", "PlanarSurfaceGroup"]
objects << ["OS:InteriorPartitionSurface", "InteriorPartitionSurface", "PlanarSurface"]
objects << ["OS:InternalMass", "InternalMass", "SpaceLoadInstance"]
objects << ["OS:LightingSimulationZone", "LightingSimulationZone", "ModelObject"]
objects << ["OS:Lights", "Lights", "SpaceLoadInstance"]
objects << ["OS:Luminaire", "Luminaire", "SpaceLoadInstance"]
objects << ["OS:People", "People", "SpaceLoadInstance"]
objects << ["OS:ShadingSurfaceGroup", "ShadingSurfaceGroup", "PlanarSurfaceGroup"]
objects << ["OS:ShadingSurface", "ShadingSurface", "PlanarSurface"]
objects << ["OS:Space", "Space", "PlanarSurfaceGroup"]
objects << ["OS:Surface", "Surface", "PlanarSurface"]
objects << ["OS:SubSurface", "SubSurface", "PlanarSurface"]
objects << ["OS:SpaceInfiltration:DesignFlowRate", "SpaceInfiltrationDesignFlowRate", "SpaceLoad"]
objects << ["OS:SpaceVentilation:DesignFlowRate", "SpaceVentilationDesignFlowRate", "SpaceLoad"]

objects << ["OS:LightingDesignDay", "LightingDesignDay", "ModelObject"]
objects << ["OS:LightingSimulationControl", "LightingSimulationControl", "ModelObject"]

objects << ["OS:ThermalZone", "ThermalZone", "HVACComponent"]

includePath = nil
if /mswin/.match(RUBY_PLATFORM)
  includePath = "../../build/OSCore-prefix/src/OSCore-build/ruby/Release"
else
  includePath = "../../build/OSCore-prefix/src/OSCore-build/ruby"
end

failed_objects = []
objects.each do |object|
  command = "ruby -I #{includePath} GenerateClass.rb -s 'model' -c '#{object[1]}' -b '#{object[2]}' -o #{outDir} -p -q -i '#{object[0].gsub(':','_')}' > #{outDir}/logs/#{object[0].gsub(':','_')}.log 2>&1 "
  puts command
  result = system(command)
  failed_objects << object if not result
end

failed_objects.each do |object|
  puts "Failed to make class for IddObject #{object[0]}"
end
