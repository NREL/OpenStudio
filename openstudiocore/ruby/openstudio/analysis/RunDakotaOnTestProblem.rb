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
#   Basic driver function for using Dakota to solve or explore an 
#   explicitly specified OptimizationProblem.
#
# == Usage
#
# == Example
#
#   ruby RunDakotaOnTestProblem.rb -o "./results/Rosenbrock_NCSU" -p "../../problems/Rosenbrock.rb" "../../initializers/NCSU.rb" -d -x0 2.0 -1.0
######################################################################

require 'openstudio'
require 'optparse'

options = Hash.new
options[:arguments] = []

optparse = OptionParser.new do|opts|

  opts.on('-o','--outputDirectory OUTPUTDIRECTORY', String, "Output directory for the analysis." ) do |outDir|
    options[:outDir] = outDir
  end
  
  opts.on('-p','--problemPath PROBLEMPATH', String, "Path to file containing class derived from OptimizationProblem." ) do |problemPath|
    options[:problemPath] = problemPath
  end
  
  opts.on('-i','--initializerPath INITIALIZERPATH', String, "Path to file containing class derived from DakotaInitializer." ) do |initializerPath|
    options[:initializerPath] = initializerPath
  end
  
  opts.on( '-x0', '--startPoint x,y,z', Array, "Optional initial point at which to start the algorithm.") do |x0| 
    options[:x0] = x0
  end
  
  opts.on( '-d', '--[no-]driver', "Use an explicit Dakota driver.") do |driver|
    options[:driver] = driver
  end

  opts.on('-n', '--nodes NODES', Integer, "Number of nodes available. 
Default is 1.") do |numNodes|
    options[:numNodes] = numNodes
  end

  opts.on('-c', '--cores CORES', Integer, "Number of cores per node. 
Default is 4.") do |numCores|
    options[:numCores] = numCores
  end    
  
  opts.on( '-v', '--[not-]verbose', "Whether to print detailed debug messages.") do |verbose|
    options[:verbose] = verbose
  end
  
  opts.on( '-r', '--restartPath RESTARTPATH', String, "Path to dakota restart (.rst) file.") do |restartPath|
    options[:restartPath] = restartPath
  end
     
end

optparse.parse!

# Create the output directory.
if not options[:outDir] or not options[:problemPath] or not options[:initializerPath]
  puts optparse
  exit  
end

puts "Creating output directory."
out_dir = OpenStudio::Path.new(options[:outDir].to_s)
if not OpenStudio::exists(out_dir)
  ok = OpenStudio::create_directory(out_dir)
  raise "Could not locate or create output directory." if not ok
end
cur_dir = Dir.getwd

prob_file = OpenStudio::Path.new(options[:problemPath].to_s)
init_file = OpenStudio::Path.new(options[:initializerPath].to_s)
x0 = []
x0 = options[:x0] if options[:x0]
driver = options[:driver]
options[:numNodes] ? nodes = options[:numNodes] : nodes = 1
options[:numCores] ? cores = options[:numCores] : cores = 4
puts "Will run Dakota on " + nodes.to_s + " nodes with " + cores.to_s + " cores each."
verbose = options[:verbose]
restart_file = OpenStudio::Path.new
if options[:restartPath]
  restart_file = OpenStudio::Path.new(options[:restartPath].to_s)
end

Dir.chdir(out_dir.to_s)
raise "Unable to locate problem file." if not OpenStudio::exists(prob_file)
raise "Unable to locate Dakota initializer file." if not OpenStudio::exists(init_file)
Dir.chdir(cur_dir)

require init_file.to_s
dakInt = nil
ObjectSpace.each_object(DakotaInitializer) { |obj|
  #dakInt = eval(obj.class.to_s + ".new(out_dir,prob_file,x0,driver,nodes,cores,verbose,restart_file)")
  dakInt = eval(obj.class.to_s + ".new(out_dir,prob_file,x0,driver,nodes,cores,verbose)")
  break
}
raise "Unable to locate DakotaInitializer class." if not dakInt

puts "Will now invoke Dakota to solve or explore the optimization problem:"
puts dakInt.problem
puts "Using method:"
puts dakInt.in_file_method
puts "And model:"
puts dakInt.in_file_model

start = OpenStudio::Time::currentTime

dakInt.set_up_run()
dakInt.kick_off_run()

run_time = OpenStudio::Time::currentTime - start
puts "Total run time: " + run_time.to_s

