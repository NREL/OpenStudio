/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "InitRubyBindings.hpp"
#include "RubyEval.hpp"
#include <ruby.h>
#include <stdexcept>
#include <iostream>

extern "C"
{
  void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  void Init_openstudioenergyplus(void);
  void Init_openstudioepjson(void);
  void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  void Init_openstudiogbxml(void);
  void Init_openstudiogltf(void);
  void Init_openstudiomodelgenerators(void);
  void Init_openstudioradiance(void);
  void Init_openstudioutilitiestime(void);
  void Init_openstudioisomodel(void);
  void Init_openstudiomodelgeometry(void);
  void Init_openstudiosdd(void);
  void Init_openstudioutilitiesfiletypes(void);
  void Init_openstudioutilitiesunits(void);
  void Init_openstudiomodelhvac(void);
  void Init_openstudiomodelzonehvac(void);
  void Init_openstudiomodelavailabilitymanager(void);
  void Init_openstudiomodelplantequipmentoperationscheme(void);
  void Init_openstudiomodelstraightcomponent(void);
  void Init_openstudiomodelairflow(void);
  void Init_openstudioutilities(void);
  void Init_openstudioutilitiesgeometry(void);
  void Init_openstudiomeasure(void);
  void Init_openstudiomodelrefrigeration(void);
  void Init_openstudioutilitiesbcl(void);
  void Init_openstudioutilitiesidd(void);
  void Init_openstudiomodel(void);
  void Init_openstudiomodelresources(void);
  void Init_openstudioutilitiesidf(void);
  void Init_openstudioutilitiesxml(void);

  ////void Init_openstudiomodeleditor(void); # happens separately in openstudio.so only, for SketchUp plug-in
}

namespace openstudio {
namespace ruby {

void initBasicRubyBindings() {
  rb_provide("openstudio");
  rb_provide("openstudio.so");

  Init_openstudioutilitiescore();
  rb_provide("openstudioutilitiescore");
  rb_provide("openstudioutilitiescore.so");
  Init_openstudioutilitiestime();
  rb_provide("openstudioutilitiestime");
  rb_provide("openstudioutilitiestime.so");
  Init_openstudioutilitiesdata();
  rb_provide("openstudioutilitiesdata");
  rb_provide("openstudioutilitiesdata.so");
  Init_openstudioutilitiesplot();
  rb_provide("openstudioutilitiesplot");
  rb_provide("openstudioutilitiesplot.so");
  Init_openstudioutilitiesgeometry();
  rb_provide("openstudioutilitiesgeometry");
  rb_provide("openstudioutilitiesgeometry.so");
  Init_openstudioutilitiessql();
  rb_provide("openstudioutilitiessql");
  rb_provide("openstudioutilitiessql.so");
  Init_openstudioutilitiesbcl();
  rb_provide("openstudioutilitiesbcl");
  rb_provide("openstudioutilitiesbcl.so");
  Init_openstudioutilitiesunits();
  rb_provide("openstudioutilitiesunits");
  rb_provide("openstudioutilitiesunits.so");
  Init_openstudioutilitiesidd();
  rb_provide("openstudioutilitiesidd");
  rb_provide("openstudioutilitiesidd.so");
  Init_openstudioutilitiesidf();
  rb_provide("openstudioutilitiesidf");
  rb_provide("openstudioutilitiesidf.so");
  Init_openstudioutilitiesfiletypes();
  rb_provide("openstudioutilitiesfiletypes");
  rb_provide("openstudioutilitiesfiletypes.so");
  Init_openstudioutilitiesxml();
  rb_provide("openstudioutilitiesxml");
  rb_provide("openstudioutilitiesxml.so");
  Init_openstudioutilities();
  rb_provide("openstudioutilities");
  rb_provide("openstudioutilities.so");
}

void initExtendedRubyBindings() {
  Init_openstudiomodel();
  rb_provide("openstudiomodel");
  rb_provide("openstudiomodel.so");
  Init_openstudiomodelcore();
  rb_provide("openstudiomodelcore");
  rb_provide("openstudiomodelcore.so");
  Init_openstudiomodelsimulation();
  rb_provide("openstudiomodelsimulation");
  rb_provide("openstudiomodelsimulation.so");
  Init_openstudiomodelresources();
  rb_provide("openstudiomodelresources");
  rb_provide("openstudiomodelresources.so");
  Init_openstudiomodelgeometry();
  rb_provide("openstudiomodelgeometry");
  rb_provide("openstudiomodelgeometry.so");
  Init_openstudiomodelhvac();
  rb_provide("openstudiomodelhvac");
  rb_provide("openstudiomodelhvac.so");
  Init_openstudiomodelzonehvac();
  rb_provide("openstudiomodelzonehvac");
  rb_provide("openstudiomodelzonehvac.so");
  Init_openstudiomodelavailabilitymanager();
  rb_provide("openstudiomodelavailabilitymanager");
  rb_provide("openstudiomodelavailabilitymanager.so");
  Init_openstudiomodelplantequipmentoperationscheme();
  rb_provide("openstudiomodelplantequipmentoperationscheme");
  rb_provide("openstudiomodelplantequipmentoperationscheme.so");
  Init_openstudiomodelstraightcomponent();
  rb_provide("openstudiomodelstraightcomponent");
  rb_provide("openstudiomodelstraightcomponent.so");
  Init_openstudiomodelairflow();
  rb_provide("openstudiomodelairflow");
  rb_provide("openstudiomodelairflow.so");
  Init_openstudiomodelrefrigeration();
  rb_provide("openstudiomodelrefrigeration");
  rb_provide("openstudiomodelrefrigeration.so");
  Init_openstudiomodelgenerators();
  rb_provide("openstudiomodelgenerators");
  rb_provide("openstudiomodelgenerators.so");

  Init_openstudioenergyplus();
  rb_provide("openstudioenergyplus");
  rb_provide("openstudioenergyplus.so");
  Init_openstudioepjson();
  rb_provide("openstudioepjson");
  rb_provide("openstudioepjson.so");
  Init_openstudioradiance();
  rb_provide("openstudioradiance");
  rb_provide("openstudioradiance.so");
  Init_openstudiogbxml();
  rb_provide("openstudiogbxml");
  rb_provide("openstudiogbxml.so");
  Init_openstudiogltf();
  rb_provide("openstudiogltf");
  rb_provide("openstudiogltf.so");
  Init_openstudioairflow();
  rb_provide("openstudioairflow");
  rb_provide("openstudioairflow.so");
  Init_openstudioosversion();
  rb_provide("openstudioversion");
  rb_provide("openstudioversion.so");
  Init_openstudiomeasure();
  rb_provide("openstudiomeasure");
  rb_provide("openstudiomeasure.so");
  Init_openstudioisomodel();
  rb_provide("openstudioisomodel");
  rb_provide("openstudioisomodel.so");
  Init_openstudiosdd();
  rb_provide("openstudiosdd");
  rb_provide("openstudiosdd.so");

  //Init_openstudiomodeleditor(); # happens separately in openstudio.so only, for SketchUp plug-in
  //rb_provide("openstudiomodeleditor");
  //rb_provide("openstudiomodeleditor.so");

  // "typedefs" for backwards compatibility
  // keep synchronized with \openstudiocore\src\utilities\core\RubyInterpreter.hpp
  std::string ruby_typedef_script = R"END(
module OpenStudio
module Ruleset

  # support for name deprecated as of 0.10.1
  class UserScriptArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgument is deprecated, use OpenStudio::Measure::Argument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class OptionalUserScriptArgument < OpenStudio::Measure::OptionalOSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OptionalUserScriptArgument is deprecated, use OpenStudio::Measure::OptionalOSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class UserScript < OpenStudio::Measure::OSMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScript is deprecated, use OpenStudio::Measure::OSMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ModelUserScript < OpenStudio::Measure::ModelMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ModelUserScript is deprecated, use OpenStudio::Measure::ModelMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class WorkspaceUserScript < OpenStudio::Measure::EnergyPlusMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "WorkspaceUserScript is deprecated, use OpenStudio::Measure::EnergyPlusMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ReportingUserScript < OpenStudio::Measure::ReportingMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ReportingUserScript is deprecated, use OpenStudio::Measure::ReportingMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgument is deprecated, use OpenStudio::Measure::OSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  def self.convertOSArgumentVectorToMap(argument_vector)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OpenStudio::Ruleset::convertOSArgumentVectorToMap is deprecated, use OpenStudio::Measure::convertOSArgumentVectorToMap instead.")
    return OpenStudio::Measure::convertOSArgumentVectorToMap(argument_vector)
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # class was replaced by OpenStudio::WorkflowStepResult
#  # support for name deprecated as of 2.0.0
#  class OSResult < OpenStudio::Measure::OSResult
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResult is deprecated, use OpenStudio::Measure::OSResult instead.")
#      super
#    end
#  end
#
#  # support for name deprecated as of 2.0.0
#  class OSResultVector < OpenStudio::Measure::OSResultVector
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResultVector is deprecated, use OpenStudio::Measure::OSResultVector instead.")
#      super
#    end
#  end

  # support for name deprecated as of 2.0.0
  class OSRunner < OpenStudio::Measure::OSRunner
    def initialize(workflow_json = nil)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSRunner is deprecated, use OpenStudio::Measure::OSRunner instead.")
      if workflow_json.nil?
        workflow_json = OpenStudio::WorkflowJSON.new
        OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "No workflow provided, using empty WorkflowJSON.")
      end
      super(workflow_json)
    end
  end

  # support for name deprecated as of 2.0.0
  class RubyUserScriptInfo < OpenStudio::Measure::OSMeasureInfo
    def initialize(*args)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "RubyUserScriptInfo is deprecated, use OpenStudio::Measure::OSMeasureInfo instead.")
      if args.size == 1
        super(args[0])
      elsif args.size == 8
        super(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
      end
    end
  end

  # support for name deprecated as of 2.0.0
  def self.infoExtractorRubyFunction
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::infoExtractorRubyFunction instead.")
    return OpenStudio::Measure.infoExtractorRubyFunction
  end

  # support for name deprecated as of 2.0.0
  def self.getInfo(measure, model, workspace)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::getInfo instead.")
    return OpenStudio::Measure.getInfo(measure, model, workspace)
  end

  # support for name deprecated as of 2.0.0
  def self.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required=true)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::makeChoiceArgumentOfWorkspaceObjects instead.")
    return OpenStudio::Measure.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required)
  end

end # module Ruleset
end # module OpenStudio

module OpenStudio
  def self.getSharedResourcesPath()
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio", "getSharedResourcesPath is deprecated.")
    return OpenStudio::Path.new()
  end
end # module OpenStudio

)END";

  openstudio::evalString(ruby_typedef_script);
}

void initRubyBindings() {
  initBasicRubyBindings();
  initExtendedRubyBindings();
}

}  // namespace ruby
}  // namespace openstudio
