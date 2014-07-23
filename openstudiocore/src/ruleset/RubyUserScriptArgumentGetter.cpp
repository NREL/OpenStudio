/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "RubyUserScriptArgumentGetter.hpp"
#include "OSArgument.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/Workspace.hpp"

#include <sstream>

namespace openstudio {
namespace ruleset {

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
