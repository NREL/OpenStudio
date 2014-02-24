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

if ARGV[0].nil?
  puts "This script requires a path to a project directory as input."
end

# Open the SimpleProject
project_dir = ARGV[0].to_s

puts "Importing measure information from " + project_dir + "."

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

# Create csv file
def table_element(data)
  return OpenStudio::TableElement.new(data)
end
table = OpenStudio::Table.new
row = OpenStudio::TableRow.new
measures.each { |measure|
  row << OpenStudio::TableElement.new("FALSE",OpenStudio::TableLoadOptions.new(false,false,false))
  row << table_element(measure["bcl_measure"].name)
  row << table_element(OpenStudio::toString(measure["bcl_measure"].directory.stem))
  row << table_element("RubyMeasure")
  table.appendRow(row)
  row.clear
  measure["arguments"].each { |arg|
    row << table_element("")
    row << table_element("argument")
    row << table_element(arg.displayName)
    row << table_element(arg.name)
    row << table_element("static")
    row << table_element(arg.type.valueName)
    row << table_element("")
    if arg.hasValue
      row << table_element(arg.valueAsString)
    elsif arg.hasDefaultValue
      row << table_element(arg.defaultValueAsString)
    else
      row << table_element("")
    end
    if arg.type == "Choice".to_OSArgumentType
      choices_str = String.new
      sep = "|"
      arg.choiceValueDisplayNames.each { |choice|
        choices_str << sep << choice
        sep = ","
      }
      choices_str << "|"
      row << table_element(choices_str)
    end
    table.appendRow(row)
    row.clear
  }
  if measure.has_key?("outputs")
    measure["outputs"].each { |output|
      row << table_element("")
      row << table_element("output")
      row << (output.displayName.empty? ? table_element(output.name) : table_element(output.displayName.get))
      row << table_element(output.name)
      row << table_element("")
      row << table_element(output.valueType.valueName)
      row << (output.units.empty? ? table_element("") : table_element(output.units.get))
      row << table_element(output.toString)
      table.appendRow(row)
      row.clear
    }
  end
}
csv_path = OpenStudio::Path.new(project_dir) / OpenStudio::Path.new("spreadsheet_export.csv")
table.save(csv_path,true)
