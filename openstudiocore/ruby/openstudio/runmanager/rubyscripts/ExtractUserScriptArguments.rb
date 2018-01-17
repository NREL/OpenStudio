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


