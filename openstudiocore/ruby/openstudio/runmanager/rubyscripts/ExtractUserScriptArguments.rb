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
# == Usage
#
#  ruby ExtractUserScriptArguments.rb --userScript=rbFilePath
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
       
end

optparse.parse!


# GET THE USER SCRIPT

if options[:userScript]
  user_script_path = OpenStudio::Path.new(options[:userScript])
else
  user_script_path = OpenStudio::Path.new("./user_script.rb")
end

puts "Extracting arguments from user script " + user_script_path.to_s

# Check list of objects in memory before loading the script
currentObjects = Hash.new
ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj| currentObjects[obj] = true }

ObjectSpace.garbage_collect
load user_script_path.to_s # need load in case have seen this script before

userScript = nil
type = String.new
ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) do |obj|
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
end

isUserScript = true
if not userScript
  isUserScript = false
end


# GET THE ARGUMENTS FROM A BLANK MODEL

args = nil
if isUserScript
  if type == "model"
    args = userScript.arguments(OpenStudio::Model::Model.new)
  elsif type == "workspace"
    args = userScript.arguments(OpenStudio::Workspace.new)
  elsif type == "translation"
    args = userScript.arguments(OpenStudio::Model::Model.new)    
    userScript.arguments(workspace).each { |arg|
      args << arg
    }      
  elsif type == "utility"
    args = userScript.arguments()
  elsif type == "report"
    args = userScript.arguments()   
  end
end


# TRY TO OPEN DB.
rm_path = OpenStudio::Path.new(user_script_path)
rm_path.replace_extension(OpenStudio::Path.new(".db").extension())

puts "Placing arguments in runmanager file " + rm_path.to_s

original = false
if OpenStudio::exists(rm_path)
  # IF EXISTS, MERGE DATA.
  # argument definitions from script
  # argument values from database  
  rm = OpenStudio::Runmanager::RunManager.new(rm_path,false,true,false)

  workflows = rm.loadWorkflows

  if workflows.size == 1
    workflow = workflows[0]
    params = workflow.params
    originalArgs = OpenStudio::Runmanager::RubyJobBuilder::toUserScriptArgs(params)

    original = true

    mergedArgs = OpenStudio::Ruleset::UserScriptArgumentVector.new
    args.each { |arg|
      foundOriginal = nil
      originalArgs.each { |oArg|
        if (oArg.name == arg.name) && (oArg.type == arg.type)
          foundOriginal = oArg
          break
        end
      }
      mergedArg = arg.clone
      if foundOriginal and foundOriginal.hasValue
        mergedArg.setValue(foundOriginal.valueAsString)
      end
      mergedArgs << mergedArg
    }
    args = mergedArgs    
    rm.deleteWorkflow(workflow.key)
  end
end

if not original
  # CREATE NEW
  rm = OpenStudio::Runmanager::RunManager.new(rm_path,true,true,false)
end

if isUserScript
  wf = OpenStudio::Runmanager::Workflow.new("UserScript")
else
  wf = OpenStudio::Runmanager::Workflow.new("Ruby")
end

wf.add(OpenStudio::Runmanager::RubyJobBuilder::toJobParams(args))
rm.persistWorkflow(wf)


