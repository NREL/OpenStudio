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
#   Pulls measure data (arguments and outputs) from an OpenStudio 
#   SimpleProject containing at least one complete, not failed 
#   DataPoint. Outputs that data as two csv files for import into 
#   the spreadsheet tool.
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
require 'csv'

if ARGV[0].nil?
  puts "This script requires a path to a project directory as input."
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
      raise "Unable to load result from '${result_path}'" if result.empty?
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

# create csv files
csv_path = OpenStudio::Path.new(project_dir) / 
           OpenStudio::Path.new("spreadsheet_model_measures_export.csv")
model_measures_csv = CSV.open(csv_path.to_s,"wb")
csv_path = OpenStudio::Path.new(project_dir) / 
           OpenStudio::Path.new("spreadsheet_energyplus_measures_export.csv")
energyplus_measures_csv = CSV.open(csv_path.to_s,"wb")
csv_path = OpenStudio::Path.new(project_dir) / 
           OpenStudio::Path.new("spreadsheet_reporting_measures_export.csv")
reporting_measures_csv = CSV.open(csv_path.to_s,"wb")

measures.each { |measure|
  csv_file = nil
  if measure["bcl_measure"].measureType == "ModelMeasure".to_MeasureType
    csv_file = model_measures_csv
  elsif measure["bcl_measure"].measureType == "EnergyPlusMeasure".to_MeasureType
    csv_file = energyplus_measures_csv
  elsif measure["bcl_measure"].measureType == "ReportingMeasure".to_MeasureType
    csv_file = reporting_measures_csv
  else
    puts "Skipping #{measure["bcl_measure"].name}, because it is of unexpected type '#{measure["bcl_measure"].measureType.valueDescription}'." 
  end
  
  row = []
  row << "FALSE"
  row << measure["bcl_measure"].name
  row << OpenStudio::toString(measure["bcl_measure"].directory.stem)
  row << "RubyMeasure"
  csv_file << row
  row = []
  measure["arguments"].each { |arg|
    row << ""
    row << "argument"
    row << arg.displayName
    row << arg.name
    row << "static"
    if arg.type == "Choice".to_OSArgumentType
      row << "String"
      row << ""    
      if arg.hasValue
        row << arg.valueDisplayName
      elsif arg.hasDefaultValue
        row << arg.defaultValueDisplayName
      else
        row << ""
      end
    else
      row << arg.type.valueName
      row << ""    
      if arg.hasValue
        row << arg.valueAsString
      elsif arg.hasDefaultValue
        row << arg.defaultValueAsString
      else
        row << ""
      end
    end
    if arg.type == "Choice".to_OSArgumentType
      choices_str = String.new
      sep = "|"
      arg.choiceValueDisplayNames.each { |choice|
        choices_str << sep << choice
        sep = ","
      }
      choices_str << "|"
      row << choices_str
    end
    csv_file << row
    row = []
  }
}
model_measures_csv.close
energyplus_measures_csv.close
reporting_measures_csv.close

csv_path = OpenStudio::Path.new(project_dir) / 
           OpenStudio::Path.new("spreadsheet_outputs_export.csv")
csv_file = CSV.open(csv_path.to_s,"wb")
def add_rows_for_attribute(csv_file,measure,att,prefix)
  if att.valueType == "AttributeVector".to_AttributeValueType
    sub_prefix = prefix + "." + att.name
    att.valueAsAttributeVector.each { |sub_att|
      add_rows_for_attribute(csv_file,measure,sub_att,sub_prefix)
    }
  else
    row = []
    row << measure["bcl_measure"].name
    row << (att.displayName.empty? ? att.name : att.displayName.get)
    row << (prefix + att.name)
    row << (att.units.empty? ? "" : att.units.get)
    row << "FALSE"
    csv_file << row  
  end
end
measures.each { |measure|
  if measure.has_key?("outputs")
    measure["outputs"].each { |output|
      add_rows_for_attribute(csv_file,measure,output,"")
    }
  end
}
csv_file.close
