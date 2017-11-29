require 'rubygems'
require 'json'
require 'erb'
require 'date'

require_relative 'resources/va3c'

#start the measure
class ViewData < OpenStudio::Ruleset::ReportingUserScript
  
  #define the name that a user will see
  def name
    return "ViewData"
  end
  
  # human readable description
  def description
    return "Visualize energy simulation data plotted on an OpenStudio model in a web based viewer"
  end

  # human readable description of modeling approach
  def modeler_description
    return "Converts the OpenStudio model to vA3C JSON format and renders using Three.js, simulation data is applied to surfaces of the model"
  end
  
  def energyPlusOutputRequests(runner, user_arguments)
    super(runner, user_arguments)

    result = OpenStudio::IdfObjectVector.new

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(), user_arguments)
      return result
    end
    
    reporting_frequency = runner.getStringArgumentValue('reporting_frequency',user_arguments)
    
    variable1_name = runner.getStringArgumentValue('variable1_name',user_arguments)
    result << OpenStudio::IdfObject.load("Output:Variable,*,#{variable1_name},#{reporting_frequency};").get

    variable2_name = runner.getStringArgumentValue('variable2_name',user_arguments)
    result << OpenStudio::IdfObject.load("Output:Variable,*,#{variable2_name},#{reporting_frequency};").get

    variable3_name = runner.getStringArgumentValue('variable3_name',user_arguments)
    result << OpenStudio::IdfObject.load("Output:Variable,*,#{variable3_name},#{reporting_frequency};").get
    
    return result
  end
  
  #define the arguments that the user will input
  def arguments()
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
    chs = OpenStudio::StringVector.new
    chs << 'Last OSM'
    chs << 'Last IDF'
    file_source = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('file_source', chs, true)
    file_source.setDisplayName('Model Source')
    file_source.setDefaultValue('Last OSM')
    args << file_source
    
    chs = OpenStudio::StringVector.new
    chs << 'Timestep'
    chs << 'Hourly'
    reporting_frequency = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('reporting_frequency', chs, true)
    reporting_frequency.setDisplayName('Reporting Frequency')
    reporting_frequency.setDefaultValue('Hourly')
    args << reporting_frequency
    
    variable1_name = OpenStudio::Ruleset::OSArgument::makeStringArgument('variable1_name', true)
    variable1_name.setDisplayName('Variable 1 Name')
    variable1_name.setDefaultValue('Surface Outside Face Temperature')
    args << variable1_name
    
    variable2_name = OpenStudio::Ruleset::OSArgument::makeStringArgument('variable2_name', true)
    variable2_name.setDisplayName('Variable 2 Name')
    variable2_name.setDefaultValue('Surface Inside Face Temperature')
    args << variable2_name
    
    variable3_name = OpenStudio::Ruleset::OSArgument::makeStringArgument('variable3_name', true)
    variable3_name.setDisplayName('Variable 3 Name')
    variable3_name.setDefaultValue('Zone Mean Radiant Temperature')
    args << variable3_name
    
    return args
  end 
  
  def datetimes_to_array(dateTimes)
    result = []
    
    dateTimes.each do |d| 
      date = d.date
      time = d.time
      result << DateTime.new(date.year, date.monthOfYear.value, date.dayOfMonth, time.hours, time.minutes, time.seconds).strftime('%s').to_i

    end
    return result
  end
  
  def vector_to_array(vector)
    result = []
    (0...vector.size).each do |i| 
      result << vector[i]
    end
    return result
  end
  
  def format_array(vector)
    result = []
    vector.each do |x| 
      result << ("%.6g" % x).to_f
    end
    return result
  end
  
  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    #use the built-in error checking 
    if not runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    file_source = runner.getStringArgumentValue('file_source',user_arguments)
    from_idf = (file_source == 'Last IDF')
    reporting_frequency = runner.getStringArgumentValue('reporting_frequency',user_arguments)
    variable1_name = runner.getStringArgumentValue('variable1_name',user_arguments)
    variable2_name = runner.getStringArgumentValue('variable2_name',user_arguments)
    variable3_name = runner.getStringArgumentValue('variable3_name',user_arguments)
    
    # 'Timestep' is the key in the input file, 'Zone Timestep' is the key in the SqlFile
    if reporting_frequency == "Timestep"
      reporting_frequency = "Zone Timestep"
    end
    
    variable_names = []
    
    if /Zone/.match(variable1_name) || /Surface/.match(variable1_name) 
      variable_names << variable1_name
    else
      if !variable1_name.empty?
        runner.registerWarning("Variable '#{variable1_name}' is not a zone or surface variable, skipping")
      end
    end
    
    if variable_names.include?(variable2_name)
      runner.registerWarning("Variable '#{variable2_name}' already requested, skipping")
    elsif /Zone/.match(variable2_name) || /Surface/.match(variable2_name) 
      variable_names << variable2_name
    else
      if !variable2_name.empty?
        runner.registerWarning("Variable '#{variable2_name}' is not a zone or surface variable, skipping")
      end
    end
    
    if variable_names.include?(variable3_name)
      runner.registerWarning("Variable '#{variable3_name}' already requested, skipping")
    elsif /Zone/.match(variable3_name) || /Surface/.match(variable3_name) 
      variable_names << variable3_name
    else
      if !variable3_name.empty?
        runner.registerWarning("Variable '#{variable3_name}' is not a zone or surface variable, skipping")
      end
    end
    
    model = nil
    if from_idf
      # get the last workspace
      workspace = runner.lastEnergyPlusWorkspace
      if workspace.empty?
        runner.registerError("Cannot find last workspace.")
        return false
      end
      workspace = workspace.get
      rt = OpenStudio::EnergyPlus::ReverseTranslator.new
      model = rt.translateWorkspace(workspace)
      runner.registerInfo("Loaded model with '#{model.getSpaces.size}' spaces")
    else
      # get the last model
      model = runner.lastOpenStudioModel
      if model.empty?
        runner.registerError("Cannot find last model.")
        return false
      end
      model = model.get
      runner.registerInfo("Loaded model with '#{model.getSpaces.size}' spaces") 
    end
    
    # get the last sql file
    sqlFile = runner.lastEnergyPlusSqlFile
    if sqlFile.empty?
      runner.registerError("Cannot find last sql file.")
      return false
    end
    sqlFile = sqlFile.get
    model.setSqlFile(sqlFile)
    
    # find the environment period
    env_period = nil
    sqlFile.availableEnvPeriods.each do |p|
      if 'WeatherRunPeriod'.to_EnvironmentType == sqlFile.environmentType(p).get
        env_period = p
        break
      end
    end
    
    if !env_period
      runner.registerError("No WeatherRunPeriods found in results")
      return false
    end
    runner.registerInfo("Gathering results for run period '#{env_period}'")

    start_time = Time.now
    #puts "printing variables"
    # print all variables for reference
    sqlFile.availableVariableNames(env_period, reporting_frequency).each do |variable|
      runner.registerInfo("Available variable name '#{variable}'")
    end
    #puts "done printing variables, elapsed time #{Time.now-start_time}"
    
    #start_time = Time.now
    #puts "computing surface_data"
    # list surface and thermal zone name for each surface
    # surface_data is a temporary variable, it is not written to JSON
    surface_data = []
    model.getPlanarSurfaces.each do |surface|
      surface_name = surface.name.to_s.upcase
      thermal_zone_name = nil
      if (space = surface.space) && !space.empty?
        if from_idf
          # if we translated from IDF, the space name will be the E+ zone name
          thermal_zone_name = space.get.name.to_s.upcase
        else
          if (thermal_zone = space.get.thermalZone) && !thermal_zone.empty?
            thermal_zone_name = thermal_zone.get.name.to_s.upcase
          end
        end
      end

      surface_data << {:surface_name => surface_name, :thermal_zone_name => thermal_zone_name, :variables => []}
    end
    #puts "done computing surface_data, elapsed time #{Time.now-start_time}"
   
    # same across all variables for given timestep
    times = nil
    hoursPerInterval = nil
    intervalsPerHour = nil
    intervalsPerDay = nil
    numDays = nil
    
    # changes for each variable
    start_time = Time.now
    meta_variables = []
    variables = []
    variable_names.each do |variable_name|
      units = nil
      values = []
      data_range = [Float::MAX, Float::MIN] # data max, data min
      
      #puts "getting keys for variable, #{variable_name}"
      keys = sqlFile.availableKeyValues(env_period, reporting_frequency, variable_name)
      #puts "done getting #{keys.size} keys, elapsed time #{Time.now-start_time}"
      
      if keys.empty?
        runner.registerWarning("No data available for variable '#{variable_name}', skipping")
        next
      end
      
      keys.each do |key|
        runner.registerInfo("Available key '#{key}' for variable name '#{variable_name}'")
        
        #puts "getting timeseries for key, #{key}"
        ts = sqlFile.timeSeries(env_period, reporting_frequency, variable_name, key).get
        units = ts.units 
        #puts "done getting timeseries, elapsed time #{Time.now-start_time}"
        
        if times.nil?
          times = datetimes_to_array(ts.dateTimes)
          if !ts.intervalLength.empty?
            hoursPerInterval = ts.intervalLength.get.totalHours
            intervalsPerHour = 1.0 / hoursPerInterval.to_f
            intervalsPerDay = 1.0 / ts.intervalLength.get.totalDays
            numDays = times.size * ts.intervalLength.get.totalDays
          end
        end

        this_values = vector_to_array(ts.values)

        min = this_values.min
        max = this_values.max
        if (min < data_range[0])
          data_range[0] = min
        end
        if (max > data_range[1])
          data_range[1] = max
        end
        
        valueIndex = values.length
        values << format_array(this_values)

        if i = surface_data.index{|s| s[:surface_name] == key}
          surface_data[i][:variables] << {:name => variable_name, :valueIndex => valueIndex, :keyName=>key}
        else  
          surface_data.each do |s|
            if s[:thermal_zone_name] == key
              s[:variables] << {:name => variable_name, :valueIndex => valueIndex, :keyName=>key}
            end
          end
        end
        
        #puts "finished with key #{key}, elapsed time #{Time.now-start_time}"
      end
      
      meta_variables << {:name=>variable_name, :intervalsPerHour=>intervalsPerHour, :intervalsPerDay=>intervalsPerDay, 
                         :hoursPerInterval=>hoursPerInterval, :numDays=>numDays, :units=>units,
                         :valueMin=>data_range[0], :valueMax=>data_range[1]}
                                     
      variables << {:name=>variable_name, :intervalsPerHour=>intervalsPerHour, :intervalsPerDay=>intervalsPerDay, 
                    :hoursPerInterval=>hoursPerInterval, :numDays=>numDays, :units=>units, 
                    :valueMin=>data_range[0], :valueMax=>data_range[1], :timeIndex=>0, :values=>values}
      
      #puts "finished with variable #{variable_name}, elapsed time #{Time.now-start_time}"
    end
    
    # convert the model to vA3C JSON format
    start_time = Time.now
    #puts "converting model to vA3C"
    json = VA3C.convert_model(model)
    #puts "finished converting model, elapsed time #{Time.now-start_time}"
    
    json['metadata'][:variables] = meta_variables
    json[:times] = [times]
    json[:variables] = variables
    
    json['object'][:children].each do |child|
      name = child[:userData][:name].upcase
      
      surface = surface_data.find{|x| x[:surface_name] == name}
      
      valueIndex = nil
      keyName = nil
      if surface
        child[:userData][:variables] = surface[:variables]
      end
      
    end

    # write json file
    #start_time = Time.now
    #puts "writing JSON"
    json_out_path = "./report.json"
    File.open(json_out_path, 'w') do |file|
      file << JSON::generate(json, {:object_nl=>"\n", :array_nl=>"", :indent=>"  "})
      #file << JSON::generate(json, {:object_nl=>"", :array_nl=>"", :indent=>""})
      # make sure data is written to the disk one way or the other      
      begin
        file.fsync
      rescue
        file.flush
      end
    end
    #puts "finished writing JSON, elapsed time #{Time.now-start_time}"
    
    # read in template
    #start_time = Time.now
    #puts "writing html"
    html_in_path = "#{File.dirname(__FILE__)}/resources/report.html.in"
    if File.exist?(html_in_path)
        html_in_path = html_in_path
    else
        html_in_path = "#{File.dirname(__FILE__)}/report.html.in"
    end
    html_in = ""
    File.open(html_in_path, 'r') do |file|
      html_in = file.read
    end
    
    # configure template with variable values
    os_data = JSON::generate(json, {:object_nl=>"", :array_nl=>"", :indent=>""})
    title = "View Data"
    renderer = ERB.new(html_in)
    html_out = renderer.result(binding)

    # write html file
    html_out_path = "./report.html"
    File.open(html_out_path, 'w') do |file|
      file << html_out
      
      # make sure data is written to the disk one way or the other      
      begin
        file.fsync
      rescue
        file.flush
      end
    end
    #puts "finished writing html, elapsed time #{Time.now-start_time}"
    
    #closing the sql file
    #sqlFile.close()

    #reporting final condition
    #runner.registerFinalCondition("Model written.")
    
    return true
 
  end #end the run method

end #end the measure

#this allows the measure to be use by the application
ViewData.new.registerWithApplication