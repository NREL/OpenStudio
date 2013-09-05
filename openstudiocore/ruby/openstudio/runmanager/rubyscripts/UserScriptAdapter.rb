######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
# == Usage
#
#  ruby UserScriptAdapter.rb --userScript=filePath
#
# == Examples
#
######################################################################

require 'openstudio'
require 'optparse'

options = Hash.new
options[:arguments] = []

optparse = OptionParser.new do|opts|

  opts.on('-u','--userScript USERSCRIPT', String, "Path to .rb file that contains an OpenStudio::Ruleset::UserScript" ) do |userScript|
    options[:userScript] = userScript
  end
  
  opts.on('-i','--inputPath INPUTPATH', String, "Path to input model (OpenStudio::Model::Model or OpenStudio::Workspace)." ) do |inputPath|
    options[:inputPath] = inputPath
  end
  
  opts.on('-o','--outputPath OUTPUTPATH', String, "Output file to write (OpenStudio::Model::Model or OpenStudio::Workspace)." ) do |outputPath|
    options[:outputPath] = outputPath
  end
  
  opts.on('-n','--argumentName ARGUMENTNAME', String, "Name of next argument.") do |argumentName|
    options[:tempName] = argumentName
  end
  
  opts.on('-v','--argumentValue ARGUMENTVALUE', String, "Value of next argument.") do |argumentValue|
    puts "Found argument '" + options[:tempName] + "' with value '" + argumentValue + "'."
    argValPair = [options[:tempName].to_s,argumentValue]
    options[:arguments].push(argValPair)
  end
     
end

optparse.parse!

# GET THE USER SCRIPT

if options[:userScript]
  user_script_path = OpenStudio::Path.new(options[:userScript])
else
  user_script_path = OpenStudio::Path.new("./user_script.rb")
end

require user_script_path.to_s

userScript = nil
type = String.new
ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj|
  if obj.is_a? OpenStudio::Ruleset::ModelUserScript
    userScript = obj
    type = "model"
    break
  elsif obj.is_a? OpenStudio::Ruleset::WorkspaceUserScript
    userScript = obj
    type = "workspace"
    break
  elsif obj.is_a? OpenStudio::Rulset::TranslationUserScript
    userScript = obj
    type = "translation"
    break
  end
}

if not userScript
  raise "Unable to locate UserScript class in " + user_script_path.to_s + 
        ", you may need to instantiate the newly defined OpenStudio::Rulset::UserScript class at "
        "the end of the file to enable finding by introspection."
else
  puts "Found UserScript '" + userScript.name + "'."
end

# GET THE INPUT MODEL

input_path = nil
if options[:inputPath]
  input_path = OpenStudio::Path.new(options[:inputPath])
else
  osmDefaultPath = OpenStudio::Path.new("in.osm")  
  idfDefaultPath = OpenStudio::Path.new("in.idf")
  if type == "model"
    input_path = osmDefaultPath
  elsif type == "workspace"
    input_path = idfDefaultPath
  else
    if OpenStudio::exists(osmDefaultPath)
      input_path = osmDefaultPath
    elsif OpenStudio::exists(idfDefaultPath)
      input_path = idfDefaultPath
    end
  end
end

if not input_path or not OpenStudio::exists(input_path)
  raise "No input file could be located. Was looking for '" + input_path.to_s + "'."
end

model = OpenStudio::Model::Model.new
workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel,"EnergyPlus".to_IddFileType)
input_data = nil
save_model = true
if type == "model"
  model = OpenStudio::Model::Model::load(input_path)
  raise "Unable to load OpenStudio Model from '" + input_path.to_s + "'." if model.empty?
  model = model.get
  input_data = model
elsif type == "workspace"
  workspace = OpenStudio::Workspace::load(input_path,"EnergyPlus".to_IddFileType)
  raise "Unable to load OpenStudio Workspace from '" + input_path.to_s + "'." if workspace.empty?
  workspace = workspace.get
  save_model = false
  input_data = workspace
else
  # let file extension determine IDD file type
  workspace = OpenStudio::Workspace::load(input_path)
  raise "Unable to load OpenStudio Workspace from '" + input_path.to_s + "'." if workspace.empty?
  workspace = workspace.get
  if workspace.iddFileType.valueName == "OpenStudio"
    model = OpenStudio::Model::Model.new(workspace)
    workspace = OpenStudio::Workspace("Draft".to_StrictnessLevel,"EnergyPlus".to_IddFileType)
    save_model = false
    input_data = model
  else
    input_data = workspace
  end
end

# GET THE ARGUMENTS

userArguments = options[:arguments]
arguments = OpenStudio::Ruleset::OSArgumentMap.new
userScript.arguments(input_data).each { |arg|
  # look for arg.name() in options
  userArg = nil
  userArguments.each { |candidate|
    if candidate[0] == arg.name
      userArg = candidate
      break
    end
  }
  
  # if found, set
  if userArg
    arg.setValue(userArg[1])
  end
  
  arguments[arg.name] = arg
}

# RUN SCRIPT WITH DEFAULT RUNNER AND SAVE OUTPUT

runner = OpenStudio::Ruleset::OSRunner.new

result = true
if type == "model"
  result = userScript.run(model,runner,arguments)
elsif type == "workspace"
  result = userScript.run(workspace,runner,arguments)
else
  result = userScript.run(workspace,model,runner,arguments)
end

# SAVE OUTPUT MODEL

output_path = nil
if options[:outputPath]
  output_path = OpenStudio::Path.new(options[:outputPath])
elsif save_model
  output_path = OpenStudio::Path.new("out.osm")
else
  output_path = OpenStudio::Path.new("out.idf")
end

if save_model
  model.save(output_path,true)
else
  workspace.save(output_path,true)
end

# SAVE SCRIPT RESULT

runner.result.save(OpenStudio::Path.new("result.ossr"),true)

# make doubly sure RunManager flags this job as failed
raise "Error encountered. See result.ossr." if not result

