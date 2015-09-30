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
#   Translate an OpenStudio Building Model to Radiance Simulation format. 
#
# == Usage
#
#  ruby ModelToRad.rb ARGV[0] (ARGV[1])
#
#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus. 
#            EnergyPlus simulation must be pre-run with the SQLite output including tabular data 
#            and the EnvelopeSummary table must be requested.  The "Exterior 
#            Horizontal Illuminance From Sky", and "Exterior Beam Normal 
#            Illuminance" output variables must be requested at hourly reporting 
#            frequency.
#
# == Examples
#
#   ruby ModelToRad.rb 'C:\path\to\model.osm' 
#   ruby ModelToRad.rb 'C:\path\to\model.osm' 'C:\path\to\eplusout.sql' 
#
######################################################################

require 'openstudio'
require 'fileutils'

require 'optparse'
require 'ostruct'
  
class ParseOptions

  #
  # Return a structure describing the commandline options.
  #
  def self.parse(args)
    # The options specified on the command line will be collected in *options*.
    options = OpenStruct.new
    # We set some default values here.
    options.verbose = ''

    opts = OptionParser.new do |opts|
      opts.banner = "\nUsage: ModelToRad.rb [ARGV[0] (ARGV[1]) [options]]\n\nARGV[0] - Path to OpenStudio Model\nARGV[1] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus."
      opts.separator ""
      opts.separator "Optional variables:"        
      opts.on("-v", "--verbose", "Verbose (debug) mode") do |verbose|
        options.verbose = 'v'
      end
      opts.separator "Other options:"
      # No argument, shows at tail.  This will print an options summary.
      opts.on_tail("-h", "--help", "Show this message") do
        puts opts
        exit
      end
    end
    opts.parse!(args)
    options
  end  # parse()
  
end  # class ParseOptions

options = ParseOptions.parse(ARGV)
if options.verbose == 'v'
  puts options
end

# quit if no model given
if not ARGV[0]
  puts "No OpenStudio model provided, quitting. Try 'ModelToRad.rb -h' for options."
  exit false
end

#
# utility functions
#
# print statement and execute as system call
def exec_statement(s)
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    s = s.gsub("/", "\\")
  end
  puts "'#{s}'"
  result = system(s)
  puts 
  return result
end

#
# setup
#
modelPath = OpenStudio::Path.new(ARGV[0])
modelPath = OpenStudio::system_complete(modelPath)
outPath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem) / OpenStudio::Path.new("model") / OpenStudio::Path.new("radiance")
if options.verbose == 'v'
  puts "Working Directory: #{outPath}"
end

resourcePath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem) 

# make a workflow
workflow = OpenStudio::Runmanager::Workflow.new()

# if no sql path, run a simulation
if (not ARGV[1] or not File::file?(ARGV[1]) or File::directory?(ARGV[1]))
  puts "no sql file given, running EnergyPlus..."
 
  # find EnergyPlus
  co = OpenStudio::Runmanager::ConfigOptions.new 
  co.fastFindEnergyPlus

  # create the workflow
  workflow.addWorkflow(OpenStudio::Runmanager::Workflow.new("ModelToRadPreprocess->ModelToIdf->ExpandObjects->EnergyPlus"))
  workflow.add(co.getTools)
  
  #add rest of workflow
  modelToRad = OpenStudio::Runmanager::Workflow.new("ModelToRad")
  workflow.addWorkflow(modelToRad)

else
  # just use argument
  sqlPath = OpenStudio::Path.new(ARGV[1])
  sqlPath = OpenStudio::system_complete(sqlPath)
  puts "sql file passed as argument " + OpenStudio::toString(sqlPath)

  #add rest of workflow
  modelToRad = OpenStudio::Runmanager::Workflow.new("ModelToRad")
  modelToRad.addFile("sql", sqlPath) 
  workflow.addWorkflow(modelToRad)
  
end

# try to minimize file path lengths
workflow.addParam(OpenStudio::Runmanager::JobParam.new("flatoutdir"))

# make a run manager
runDir = outPath / OpenStudio::Path.new("gen_eplus/")
runmanager_path = OpenStudio::Path.new("runmanager.db")
runmanager = OpenStudio::Runmanager::RunManager.new(runmanager_path, true, true, false, false)

# run 
runDir = OpenStudio::system_complete(runDir)
puts "Creating rundir: #{runDir}"
OpenStudio::makeParentFolder(runDir, OpenStudio::Path.new(), true)
puts "Creating workflow"
jobtree = workflow.create(OpenStudio::system_complete(runDir), OpenStudio::system_complete(modelPath), OpenStudio::Path.new())
runmanager.enqueue(jobtree, true)
runmanager.getJobs.each {|job| job.setBasePath(resourcePath)}
puts "Running jobs"
runmanager.setPaused(false)
runmanager.waitForFinished()

#check for fatals on EPlus run
runmanager.getJobs.each { |job| 

  if not job.errors.succeeded   
    puts "The job in '" + job.outdir.to_s + "' did not finish successfully."
    job.errors.errors.each { |err|
      puts "ERROR: " + err
    }
    puts "Radiance export aborted."
    exit false
  end
  
  if not job.errors.warnings.empty?
    puts "The job in '" + job.outdir.to_s + "' has warnings."
    job.errors.warnings.each { |warn|
      puts "WARN: " + warn
    }
  end
  
  if job.jobType() == "ModelToRad".to_JobType
    outputFiles = job.outputFiles()

    outputFiles.each { |file|
      radfile = outPath / OpenStudio::relativePath(file.fullPath, job.outdir)
      puts "Copying #{file.fullPath} to #{radfile}"
      OpenStudio::create_directory(radfile.parent_path().parent_path())
      OpenStudio::create_directory(radfile.parent_path())
      OpenStudio::copy_file(file.fullPath, radfile)
    }
  end
}

# the end
if options.verbose == 'v'
  radDir = outPath / OpenStudio::Path.new("radiance")
  puts "Done. Radiance model located at: #{radDir}."
end


