# see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

require 'json'

# start the measure
class ReportRequest < OpenStudio::Ruleset::WorkspaceUserScript
  
  # human readable name
  def name
    return "Report Request"
  end

  # human readable description
  def description
    return "Requests EnergyPlus output reports based on downstream reporting measures."
  end
  
  # human readable description of modeling approach
  def modeler_description
    return ""
  end
  
  # define the arguments that the user will input
  def arguments(workspace)
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
    json_work_items = OpenStudio::Ruleset::OSArgument::makeStringArgument("json_work_items", true)
    args << json_work_items
    
    return args
  end
  
  # examines object and determines whether or not to add it to the workspace
  def add_object(runner, workspace, idf_object)
  
  end

  # define what happens when the measure is run
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)
    
    # use the built-in error checking 
    if not runner.validateUserArguments(arguments(workspace), user_arguments)
      return false
    end

    # assign the user inputs to variables
    json_work_items = runner.getStringArgumentValue("json_work_items", user_arguments)
    
    File.open('example.json', 'w') do |f|
      f << json_work_items
    end
    
    json_work_items = JSON.parse(json_work_items)
    
    runner.registerInfo("Examining report requests from #{json_work_items.size} reporting measures")
    
    error = false
    num_added = 0
    json_work_items.each do |json_work_item|
      begin
        #work_item = OpenStudio::Runmanager::WorkItem.fromJSON(json_work_item.to_s)
        #measure_file = work_item.files.getLastByFilename("measure.rb")
        
        measure_file = json_work_item["script"]
        if measure_file.nil?
          error = true
          runner.registerError("Measure does not specify a script file")
          next
        elsif not File.exists?(measure_file.to_s)
          error = true
          runner.registerError("Measure file '#{measure_file}' does not exist")
          next
        else
          runner.registerInfo("Loading measure file '#{measure_file}'")
        end
        
        user_arguments = OpenStudio::Ruleset::OSArgumentMap.new

        # Check list of objects in memory before loading the script
        currentObjects = Hash.new
        ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj| currentObjects[obj] = true }
        ObjectSpace.garbage_collect
        
        load measure_file.to_s # need load in case have seen this script before
        userScript = nil
        type = String.new
        ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) do |obj|
          if not currentObjects[obj]
            if obj.is_a? OpenStudio::Ruleset::ReportingUserScript
              userScript = obj
              type = "report"
            end
          end
        end
        
        if userScript.nil?
          error = true
          runner.registerInfo("Measure at '#{measure_file}' does not appear to be a reporting measures")
          next
        end

        idf_objects = userScript.energyPlusOutputRequests(runner, user_arguments)
        
        idf_objects.each do |idf_object|
          num_added += add_object(runner, workspace, idf_object)
        end
        
      rescue Exception => e
        error = true
        runner.registerError("#{e.message}\n#{e.backtrace}")
      end
    end
    
    if error
      return false
    end
    
    runner.registerFinalCondition("Added #{num_added} output requests for reporting measures")

    return true
 
  end 

end 

# register the measure to be used by the application
ReportRequest.new.registerWithApplication