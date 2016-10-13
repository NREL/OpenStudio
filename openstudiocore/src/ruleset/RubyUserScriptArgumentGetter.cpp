/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "RubyUserScriptArgumentGetter.hpp"
#include "OSArgument.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/Workspace.hpp"

#include <sstream>

namespace openstudio {
namespace ruleset {

RubyUserScriptArgumentGetter::~RubyUserScriptArgumentGetter()
{}

std::vector<OSArgument> RubyUserScriptArgumentGetter::getArguments(
    const BCLMeasure& measure,
    const boost::optional<model::Model>& model,
    const boost::optional<Workspace>& workspace)
{
  if (model) {
    if (workspace) {
      return this->getArguments(measure,*model,*workspace);
    }
    else {
      return this->getArguments(measure,*model);
    }
  }
  else if (workspace) {
    return this->getArguments(measure,*workspace);
  }
  else {
    return this->getArguments(measure);
  }
}

std::string argumentExtractorRubyFunction() {
  std::stringstream ss;
  ss << "def argumentExtractor(bclMeasure, optionalModel, optionalWorkspace)" << std::endl;
  ss << "  # GET THE USER SCRIPT" << std::endl;
  ss << "  scriptPath = bclMeasure.primaryRubyScriptPath()" << std::endl;
  ss << "  if scriptPath.empty?" << std::endl;
  ss << "    raise \"Unable to locate primary Ruby script path for BCLMeasure '\" + " << std::endl;
  ss << "        bclMeasure.name + \"' located at \" + bclMeasure.directory.to_s + \".\"" << std::endl;
  ss << "  end" << std::endl << std::endl;
  // Check list of objects in memory before loading the script
  ss << "  currentObjects = Hash.new" << std::endl;
  ss << "  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj| currentObjects[obj] = true }" << std::endl << std::endl; 
  ss << "  ObjectSpace.garbage_collect" << std::endl;
  // This line is REQUIRED or the ObjectSpace order will change when garbage collection runs automatically
  // If ~12 measures are added and garbage collection runs, the following loop to grab the first userscript
  //   will get the wrong one and return incorrect arguments
  ss << "  ObjectSpace.garbage_collect" << std::endl;
  ss << "  load scriptPath.get.to_s # need load in case have seen this script before" << std::endl;
  ss << std::endl;
  ss << "  userScript = nil" << std::endl;
  ss << "  type = String.new" << std::endl;
  ss << "  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) do |obj|" << std::endl;
  ss << "    if not currentObjects[obj]" << std::endl;
  ss << "      if obj.is_a? OpenStudio::Ruleset::UtilityUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"utility\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::ModelUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"model\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::WorkspaceUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"workspace\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::TranslationUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"translation\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::UtilityUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"utility\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::ReportingUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"report\"" << std::endl;
  ss << "      end" << std::endl;
  ss << "    end" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
  ss << "  if not userScript" << std::endl;
  ss << "    raise \"Unable to extract OpenStudio::Ruleset::UserScript object from \" + " << std::endl;
  ss << "        scriptPath.get.to_s + \". The script should contain a class that derives \" + " << std::endl;
  ss << "        \"from OpenStudio::Ruleset::UserScript and should close with a line stating \" + " << std::endl;
  ss << "        \"the class name followed by .new.registerWithApplication.\"" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
  ss << "  args = OpenStudio::Ruleset::OSArgumentVector.new" << std::endl;
  ss << "  model = OpenStudio::Model::Model.new" << std::endl;
  ss << "  workspace = OpenStudio::Workspace.new(\"Draft\".to_StrictnessLevel," << std::endl;
  ss << "                                        \"EnergyPlus\".to_IddFileType)" << std::endl;
  ss << "  model = optionalModel.get if not optionalModel.empty?" << std::endl;
  ss << "  workspace = optionalWorkspace.get if not optionalWorkspace.empty?" << std::endl;
  ss << "  if type == \"utility\"" << std::endl;
  ss << "    args = userScript.arguments" << std::endl;
  ss << "  elsif type == \"report\"" << std::endl;
  ss << "    args = userScript.arguments()" << std::endl;
  ss << "  elsif type == \"model\"" << std::endl;
  ss << "    args = userScript.arguments(model)" << std::endl;
  ss << "  elsif type == \"workspace\"" << std::endl;
  ss << "    args = userScript.arguments(workspace)" << std::endl;
  ss << "  elsif type == \"translation\"" << std::endl;
  ss << "    args = userScript.arguments(model)" << std::endl;
  ss << "    userScript.arguments(workspace).each { |arg|" << std::endl;
  ss << "      args << arg" << std::endl;
  ss << "    }" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
  ss << "  return args" << std::endl;
  ss << "end" << std::endl;
  return ss.str();
}

} // ruleset
} // openstudio
