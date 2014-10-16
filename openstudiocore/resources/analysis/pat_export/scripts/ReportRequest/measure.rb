# see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

require 'openstudio'
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
    
    measures_json = OpenStudio::Ruleset::OSArgument::makeStringArgument("measures_json", true)
    args << measures_json
    
    return args
  end
  
  # check to see if we have an exact match for this object already
  def check_for_object(runner, workspace, idf_object, idd_object_type)
    workspace.getObjectsByType(idd_object_type).each do |object|
      # all of these objects fields are data fields
      if idf_object.dataFieldsEqual(object)
        return true
      end
    end
    return false
  end
  
  # merge all summary reports that are not in the current workspace
  def merge_output_table_summary_reports(current_object, new_object)
  
    current_fields = []
    current_object.extensibleGroups.each do |current_extensible_group|
      current_fields << current_extensible_group.getString(0).to_s
    end
        
    fields_to_add = []
    new_object.extensibleGroups.each do |new_extensible_group|
      field = new_extensible_group.getString(0).to_s
      if !current_fields.include?(field)
        current_fields << field
        fields_to_add << field
      end
    end
    
    if !fields_to_add.empty?
      fields_to_add.each do |field|
        values = OpenStudio::StringVector.new
        values << field
        current_object.pushExtensibleGroup(values)
      end
      return true
    end
    
    return false
  end
  
  # examines object and determines whether or not to add it to the workspace
  def add_object(runner, workspace, idf_object)

    num_added = 0
    idd_object = idf_object.iddObject
   
    allowed_objects = []
    allowed_objects << "Output:Surfaces:List"
    allowed_objects << "Output:Surfaces:Drawing"
    allowed_objects << "Output:Schedules"
    allowed_objects << "Output:Constructions"
    allowed_objects << "Output:Table:TimeBins"
    allowed_objects << "Output:Table:Monthly"
    allowed_objects << "Output:Variable"
    allowed_objects << "Output:Meter"
    allowed_objects << "Output:Meter:MeterFileOnly"
    allowed_objects << "Output:Meter:Cumulative"
    allowed_objects << "Output:Meter:Cumulative:MeterFileOnly"
    allowed_objects << "Meter:Custom"
    allowed_objects << "Meter:CustomDecrement"
    
    if allowed_objects.include?(idd_object.name)
      if !check_for_object(runner, workspace, idf_object, idd_object.type)
        runner.registerInfo("Adding idf object #{idf_object.to_s.strip}")
        workspace.addObject(idf_object)
        num_added += 1
      else
        runner.registerInfo("Workspace already includes #{idf_object.to_s.strip}")
      end
    end
    
    allowed_unique_objects = []
    #allowed_unique_objects << "Output:EnergyManagementSystem" # TODO: have to merge
    #allowed_unique_objects << "OutputControl:SurfaceColorScheme" # TODO: have to merge
    allowed_unique_objects << "Output:Table:SummaryReports" # TODO: have to merge
    # OutputControl:Table:Style # not allowed
    # OutputControl:ReportingTolerances # not allowed
    # Output:SQLite # not allowed
   
    if allowed_unique_objects.include?(idf_object.iddObject.name)
      if idf_object.iddObject.name == "Output:Table:SummaryReports"
        summary_reports = workspace.getObjectsByType(idf_object.iddObject.type)
        if summary_reports.empty?
          runner.registerInfo("Adding idf object #{idf_object.to_s.strip}")
          workspace.addObject(idf_object)
          num_added += 1
        elsif merge_output_table_summary_reports(summary_reports[0], idf_object)
          runner.registerInfo("Merged idf object #{idf_object.to_s.strip}")     
        else
          runner.registerInfo("Workspace already includes #{idf_object.to_s.strip}")
        end
      end
    end
    
    return num_added
  end

  # define what happens when the measure is run
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)
    
    # use the built-in error checking 
    if not runner.validateUserArguments(arguments(workspace), user_arguments)
      return false
    end

    # assign the user inputs to variables
    measures_json = runner.getStringArgumentValue("measures_json", user_arguments)
    
    File.open('example.json', 'w') do |f|
      f << measures_json
    end
    
    measures_json = JSON.parse(measures_json)
    
    runner.registerInfo("Examining report requests from #{measures_json.size} measures")
    
    error = false
    num_added = 0
    measures_json.each do |measure_json|
      begin
     
        # look for the measure file
        measure_file = measure_json["measure"]   
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
 
        # recreate arguments for this measure
        arguments = OpenStudio::Ruleset::OSArgumentMap.new
        if arguments_json = measure_json["arguments"]
          runner.registerInfo("arguments_json.class = #{arguments_json.class}") 
          runner.registerInfo("arguments_json = #{arguments_json}")
          
          # DLM: this code mirrors that in UserScriptAdapter, refactor out somewhere
          userScript.arguments().each do |arg|
          
            puts "Looking for #{arg}"
          
            # look for arg.name() in options
            userArg = arguments_json[arg.name]
              
            # if found, set
            if userArg
              arg.setValue(userArg)
            end
              
            arguments[arg.name] = arg
          end
        end
        
        idf_objects = userScript.energyPlusOutputRequests(runner, arguments)
        
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