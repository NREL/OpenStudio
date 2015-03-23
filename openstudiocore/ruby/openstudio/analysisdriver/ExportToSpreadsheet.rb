######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
#   Pulls measure data (arguments and outputs) from an OpenStudio 
#   SimpleProject containing at least one complete, not failed 
#   DataPoint. Outputs that data formatted for the spreadsheet tool.
#
# == Usage
#
#   ruby ExportToSpreadsheet.rb ./path/to/project/directory
#
# == Examples
#
#   ruby ExportToSpreadsheet.rb C:/working/MyOpenStudioProjects/20140210_TestProject
#
######################################################################

require 'openstudio'
require 'fileutils'
require 'csv'

if ARGV[0].nil?
  raise "This script requires a path to a project directory as input."
end

# Open the SimpleProject
project_dir = ARGV[0].to_s

puts "Importing measure information from " + project_dir + "."

OpenStudio::Application::instance.processEvents
project = OpenStudio::AnalysisDriver::SimpleProject::open(project_dir)
raise "Unable to open " + project_dir + "." if project.empty?

project = project.get

# Ensure project is sufficient to generate the data we need
data_points = project.analysis.successfulDataPoints
if data_points.empty?
  raise "This script requires at least one successful data point from which to pull outputs."
end  

# set up directories
export_dir = OpenStudio::Path.new(project_dir) / OpenStudio::Path.new("analysis_spreadsheet_export")
if File.exists?(export_dir.to_s)
  FileUtils.rm_rf(export_dir.to_s)
end
if File.exists?(export_dir.to_s)
  raise "Could not remove #{export_dir}"
end
FileUtils.mkdir_p(export_dir.to_s)
if not File.exists?(export_dir.to_s)
  raise "Could not create #{export_dir}"
end

def extractMeasureClass(measurePath)
  currentObjects = Hash.new
  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj| currentObjects[obj] = true }

  ObjectSpace.garbage_collect
  load(measurePath) # need load in case have seen this script before

  userScript = nil
  type = String.new
  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj|
    if not currentObjects[obj]
      if obj.is_a? OpenStudio::Ruleset::ModelUserScript
        userScript = obj
        type = "model"
      elsif obj.is_a? OpenStudio::Ruleset::WorkspaceUserScript
        userScript = obj
        type = "workspace"
      elsif obj.is_a? OpenStudio::Ruleset::TranslationUserScript
        userScript = obj
        type = "translation"
      elsif obj.is_a? OpenStudio::Ruleset::UtilityUserScript
        userScript = obj
        type = "utility"    
      elsif obj.is_a? OpenStudio::Ruleset::ReportingUserScript
        userScript = obj
        type = "report"    
      end
    end
  }
  
  return userScript.class
end

# Step through the workflow and construct ordered array of measure information
# Each element of measures will be hash
#   "workflow_step"      - used to match up outputs with measures
#   "ruby_measure"       - primarily useful for == comparisions
#   "bcl_measure"
#   "arguments"
#   "outputs"
measures = []
compound_measure = nil   # variable for grouping a string of 
                         # RubyContinuousVariables all pointing to the same
                         # RubyMeasure
project.analysis.problem.workflow.each { |workflow_step|
  if workflow_step.isInputVariable
    ivar = workflow_step.inputVariable
    if not ivar.to_MeasureGroup.empty?
      measures << compound_measure if not compound_measure.nil?
      compound_measure = nil
      
      last_measure = nil # for a given MeasureGroup, only take distinct 
                         # RubyMeasures. allow duplicates that are in 
                         # different groups to support use cases such as
                         # SetWindowToWallRatioByFacade for 'South' and 
                         # 'North'.
      ivar.to_MeasureGroup.get.measures(false).each { |measure|
        if not measure.to_RubyMeasure.empty?
          rm = measure.to_RubyMeasure.get
          if last_measure.nil? or not (last_measure.bclMeasureUUID == rm.bclMeasureUUID)
            # keep this measure
            last_measure = rm
            measure_info = Hash.new
            measure_info["workflow_step"] = workflow_step
            measure_info["ruby_measure"] = rm
            raise "Unable to load BCLMeasure from RubyMeasure '#{rm.name}'" if rm.bclMeasure.empty?
            measure_info["bcl_measure"] = rm.bclMeasure.get
            measure_info["arguments"] = rm.arguments
            measures << measure_info
          end
        end
      }
    elsif ivar.to_RubyContinuousVariable.empty?
      rcv = ivar.to_RubyContinuousVariable.get
      rm = rcv.measure
      if not compound_measure.nil? and (rm == compound_measure["ruby_measure"])
        # append argument, keep last workflow_step
        compound_measure["workflow_step"] = workflow_step
        compound_measure["arguments"] << rcv.argument  
      else
        measures << compound_measure if not compound_measure.nil?
        
        # start new compound_measure
        compound_measure = Hash.new
        compound_measure["workflow_step"] = workflow_step
        compound_measure["ruby_measure"] = rm
        raise "Unable to load BCLMeasure from RubyMeasure '#{rm.name}'" if rm.bclMeasure.empty?
        compound_measure["bcl_measure"] = rm.bclMeasure.get
        compound_measure["arguments"] = rm.arguments
        compound_measure["arguments"] << rcv.argument
      end
    end
  elsif workflow_step.workItemType == "UserScript".to_JobType
    measures << compound_measure if not compound_measure.nil?
    compound_measure = nil
  
    rjb = OpenStudio::Runmanager::RubyJobBuilder.new(workflow_step.workItem)
    measure_info = Hash.new
    measure_info["workflow_step"] = workflow_step
    bcl_dir = nil
    rjb.requiredFiles().each { |req_file|
      if req_file.second.to_s == "user_script.rb"
        bcl_dir = req_file.first.parent_path
        break
      end
    }
    raise "Unable to locate BCLMeasure directory." if bcl_dir.nil?    
    begin
      measure_info["bcl_measure"] = OpenStudio::BCLMeasure.new(bcl_dir)
    rescue
      raise "Unable to open measure at '#{bcl_dir}'."
    end
    measure_info["arguments"] = OpenStudio::Runmanager::RubyJobBuilder::toOSArguments(rjb.toParams)
    measures << measure_info
  end
}
measures << compound_measure if not compound_measure.nil?

# Step through data_points and populate measures[i]["outputs"] based on results
# TODO: Save OSResult::attributes in a database so do not have to load individual
# .ossr files.
n = measures.size
data_points.each { |data_point|
  measure_index = 0
  project.analysis.problem.getJobsByWorkflowStep(data_point,true).each { |job_data|
    # see if has output attributes
    if not job_data.outputFiles.empty?
      result_path = nil
      begin
        result_path = job_data.outputFiles.get.getLastByExtension("ossr").fullPath
      rescue
        next
      end
      result = OpenStudio::Ruleset::OSResult::load(result_path)
      # DLM: this results in a crash for cloud data points that are not downloaded, issue #959
      if result.empty?
        next
      end
      result = result.get
      if not result.attributes.empty?
        # find measure
        while (measure_index < n) and not (job_data.step == measures[measure_index]["workflow_step"])
          measure_index += 1
        end
        raise "Unable to locate measure that produced result with attributes." if measure_index == n
        if not measures[measure_index].has_key?("outputs")
          measures[measure_index]["outputs"] = result.attributes
        end
      end
    end
  }  
}

model_measures = []
energyplus_measures = []
reporting_measures = []

measures.each { |measure| 
  measure_type = nil
  if measure["bcl_measure"].measureType == "ModelMeasure".to_MeasureType
    measure_type = "RubyMeasure"
    measure['measure_type'] = measure_type
    model_measures << measure
  elsif measure["bcl_measure"].measureType == "EnergyPlusMeasure".to_MeasureType
    measure_type = "EnergyPlusMeasure"
    measure['measure_type'] = measure_type
    energyplus_measures << measure
  elsif measure["bcl_measure"].measureType == "ReportingMeasure".to_MeasureType
    measure_type = "ReportingMeasure"
    measure['measure_type'] = measure_type
    reporting_measures << measure
  else
    puts "Skipping #{measure["bcl_measure"].name}, because it is of unexpected type '#{measure["bcl_measure"].measureType.valueDescription}'." 
  end
}

measures.clear
measures.concat(model_measures)
measures.concat(energyplus_measures)
measures.concat(reporting_measures)

# create variable csv file
csv_path = export_dir / OpenStudio::Path.new("spreadsheet_variables_export.csv")
csv_file = CSV.open(csv_path.to_s,"wb")

def write_variable_headers(csv_file)
  # column meaning for measure
  csv_file << ['Measure Enabled', 'Measure Display Name', 'Measure Directory Name', 'Measure Class Name', 'Measure Type', '-', '-', '-', '-', '-']
  # column meaning for argument
  csv_file << ['-', 'Type', '-', 'Parameter Display Name', 'Parameter Name in Measure', 'Parameter Short Display Name', 'Variable Type', 'Units', 'Static/Default Value', 'Enumerations']
  csv_file << []
end
write_variable_headers(csv_file)

measures.each { |measure|

  # do not export report request measure, implemented separately in server
  next if measure["bcl_measure"].uuid.to_s == OpenStudio::BCLMeasure.reportRequestMeasure.uuid.to_s

  measure_dir_name = OpenStudio::toString(measure["bcl_measure"].directory.stem)
  if !OpenStudio::toUUID(measure_dir_name).isNull
    # measure dir name is a uuid, rename it something else
    # DLM: do not rename uuid to human readable dir name per Nick and Brian
    # DLM: do rename to human readable dir name per Brian, 3/19/15
    measure_dir_name = measure["bcl_measure"].name
  end
  # DLM: do not clean up dir name per Nick and Brian
  #parts = measure_dir_name.split(" ")
  #parts.each {|part| part[0] = part[0].capitalize}
  #measure_dir_name = parts.join(" ").gsub(/[^0-9A-Za-z.\-]/, "")
  
  measure_class_name = extractMeasureClass(measure["bcl_measure"].primaryRubyScriptPath.get.to_s)

  # ensure directory name is unique
  measure_path = export_dir / OpenStudio::Path.new(measure_dir_name)
  index = 0
  while File.exists?(measure_path.to_s)
    index += 1
    measure_path = export_dir / OpenStudio::Path.new(measure_dir_name + index.to_s)
  end
  
  measure_type = measure["measure_type"]
 
  # copy the measure
  FileUtils.cp_r(measure["bcl_measure"].directory.to_s, measure_path.to_s)
  
  # write out measure
  row = []
  row << "TRUE" # Measure Enabled
  row << measure["bcl_measure"].displayName # Measure Display Name
  row << measure_dir_name # Measure Directory Name
  row << measure_class_name # Measure Class Name
  row << measure_type # Measure Type
  csv_file << row
  
  # write out arguments
  row = []
  measure["arguments"].each { |arg|
    row << ""
    row << "argument" # Type
    row << ""
    row << arg.displayName # Parameter Display Name
    row << arg.name # Parameter Name in Measure
    row << "" # Parameter Short Display Name
    
    row << arg.type.valueName.gsub("Boolean", "Bool") # Variable Type
    row << "" # Units   
    if arg.hasValue
      row << arg.valueAsString # Static/Default Value
    elsif arg.hasDefaultValue
      row << arg.defaultValueAsString # Static/Default Value
    else
      row << "" # Static/Default Value
    end

    if arg.type == "Choice".to_OSArgumentType
      choices_str = String.new
      #sep = "|"
      choices_str << "|"
      choices_str << arg.choiceValueDisplayNames.reject{|s| s.empty?}.join(',')
      #arg.choiceValueDisplayNames.each { |choice|
      #  choices_str << sep << choice
      #  sep = ","
      #}
      choices_str << "|"
      row << choices_str # Enumerations
    end
    csv_file << row
    row = []
  }
}
csv_file.close

# write outputs
csv_path = export_dir / OpenStudio::Path.new("spreadsheet_outputs_export.csv")
csv_file = CSV.open(csv_path.to_s,"wb")

def write_outputs_headers(csv_file)
  # column name
  csv_file << ['Variable Display Name', 'Short Display Name', 'Taxonomy Identifier', 'Name', 'Units', 'Variable Type', 'Visualize', 'Export', 'Objective Function']
  csv_file << []
end
write_outputs_headers(csv_file)

def add_rows_for_attribute(csv_file,measure,att,prefix)
  if att.valueType == "AttributeVector".to_AttributeValueType
    sub_prefix = prefix + "." + att.name
    att.valueAsAttributeVector.each { |sub_att|
      add_rows_for_attribute(csv_file,measure,sub_att,sub_prefix)
    }
  else
    row = []
    row << (att.displayName.empty? ? att.name : att.displayName.get) # Variable Display Name
    row << "" # Short Display Name
    row << "" # Taxonomy Identifier
    row << (prefix + att.name) # Name
    row << (att.units.empty? ? "" : att.units.get) # Units
    row << att.valueType.valueName.to_s # Variable Type
    row << "TRUE" # Visualize
    row << "TRUE" # Export
    row << "FALSE" # Objective Function
    csv_file << row  
  end
  
end
measures.each { |measure|
  if measure.has_key?("outputs")
    measure["outputs"].each { |output|
      add_rows_for_attribute(csv_file,measure,output,"#{measure['bcl_measure'].name}.")
    }
  end
}
csv_file.close
