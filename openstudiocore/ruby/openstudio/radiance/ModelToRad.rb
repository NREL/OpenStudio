########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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


