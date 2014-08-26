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

  # define what happens when the measure is run
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)
    
    # use the built-in error checking 
    if not runner.validateUserArguments(arguments(workspace), user_arguments)
      return false
    end

    # assign the user inputs to variables
    json_work_items = runner.getStringArgumentValue("json_work_items", user_arguments)
    json_work_items = JSON.parse(json_work_items)
    json_work_items.each do |json_work_item|
      begin
        work_item = OpenStudio::Runmanager::WorkItem.fromJSON(json_work_item)
        puts work_item.type
        puts work_item.params
        puts work_item.files
        
        measure_file = work_item.files.getLastByFilename("measure.rb")
        measure_file.fullPath
        
        work_item.params.each do |p|
          puts p
        end
      rescue Exception => e
        puts e
      end
    end
    
    return true
 
  end 

end 

# register the measure to be used by the application
ReportRequest.new.registerWithApplication