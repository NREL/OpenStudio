/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "WorkflowTools.hpp"

#include <QSysInfo>

//#include "../runmanager/lib/RunManager.hpp"
//#include "../runmanager/lib/WorkItem.hpp"
//#include "../runmanager/lib/Workflow.hpp"

#include "../model/ThermalZone.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/Space.hpp"
#include "../model/Model.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/GlareSensor.hpp"

#include "../model/ThermalZone_Impl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/IlluminanceMap_Impl.hpp"
#include "../model/GlareSensor_Impl.hpp"

//#include "../analysis/Problem.hpp"
//#include "../analysis/Analysis.hpp"
//#include "../analysisdriver/SimpleProject.hpp"

namespace openstudio {
  /*
void getRadiancePreRunWarningsAndErrors(std::vector<std::string> &t_warnings,
    std::vector<std::string> &t_errors,
    openstudio::runmanager::RunManager &t_runManager,
    boost::optional<openstudio::model::Model> &t_model)
{

  t_warnings.clear();
  t_errors.clear();

  openstudio::runmanager::ConfigOptions co(true);
  t_runManager.setConfigOptions(co);
  bool ruby_not_installed = co.getTools().getAllByName("ruby").tools().size() == 0;
  bool radiance_not_installed = co.getTools().getAllByName("rtrace").tools().size() == 0;

  if(radiance_not_installed){
    t_errors.push_back("Radiance is required but not found, install Radiance and scan for tools.");
  }

  if(ruby_not_installed){
    t_errors.push_back("Ruby is required but not found, locate Ruby in scan for tools.");
  }

  // TODO remove when fixed
  #if defined(Q_OS_WIN)
  if(QSysInfo::windowsVersion() == QSysInfo::WV_XP){
    t_errors.push_back("OpenStudio is currently unable to run Radiance on XP operating systems.");
  }
  #endif

  if (t_model)
  {
    // ThermalZone
    std::vector<model::ThermalZone> thermalZones = t_model->getModelObjects<model::ThermalZone>();
    if(thermalZones.size() > 0){
      //for (model::ThermalZone thermalZone : thermalZones){
      //  std::vector<model::Space> spaces = thermalZone.spaces();
      //  if(spaces.size() > 0){
      //  }
      //  boost::optional<model::IlluminanceMap> illuminanceMap = thermalZone.illuminanceMap();
      //  if(illuminanceMap){
      //  }
      //  boost::optional<model::DaylightingControl> primaryDaylightingControl = thermalZone.primaryDaylightingControl();
      //  if(primaryDaylightingControl){
      //  }
      //}
    }
    else{
      t_errors.push_back("The OpenStudio model has no ThermalZone objects.");
    }

    // Space
    std::vector<model::Space> spaces = t_model->getModelObjects<model::Space>();
    if(spaces.size() == 0){
      t_errors.push_back("The OpenStudio model has no Space objects.");
    }

    // IlluminanceMap
    std::vector<model::IlluminanceMap> illuminanceMaps = t_model->getModelObjects<model::IlluminanceMap>();
    if(illuminanceMaps.size() > 0){
      for (model::IlluminanceMap illuminanceMap : illuminanceMaps){
        boost::optional<model::Space> space = illuminanceMap.space();
        if(!space){
          t_errors.push_back("An OpenStudio model IlluminanceMap object is not assigned to a Space object.");
          break;
        }
        else{
          //boost::optional<model::ThermalZone> thermalZone = space->thermalZone();
          //std::vector<model::DaylightingControl> daylightingControls = space->daylightingControls();
          //std::vector<model::IlluminanceMap> illuminanceMaps = space->illuminanceMaps();
          //std::vector<model::GlareSensor> glareSensors = space->glareSensors();
        }
      }
    }
    else{
      t_errors.push_back("The OpenStudio model has no IlluminanceMap objects.");
    }

    // DaylightingControl
    std::vector<model::DaylightingControl> daylightingControls = t_model->getModelObjects<model::DaylightingControl>();
    if(daylightingControls.size() > 0){
      for (model::DaylightingControl daylightingControl : daylightingControls){
        boost::optional<model::Space> space = daylightingControl.space();
        if(!space){
          t_errors.push_back("An OpenStudio model DaylightingControl object is not assigned to a Space object.");
          break;
        }
      }
    }
    else{
      t_errors.push_back("The OpenStudio model has no DaylightingControl objects.");
    }

    // GlareSensor
    std::vector<model::GlareSensor> glareSensors = t_model->getModelObjects<model::GlareSensor>();
    if(glareSensors.size() == 0){
      t_warnings.push_back("The OpenStudio model has no GlareSensor objects.");
    }
  }
}

OptionalInt getProjectRadianceJobIndex(const openstudio::analysisdriver::SimpleProject &t_project)
{
  openstudio::analysis::Problem problem = t_project.analysis().problem();
  OptionalInt index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
  LOG_FREE(Trace, "WorkflowTools", "ModelToIdf job found " << (index?"true":"false"));

  if (index)
  {
    LOG_FREE(Trace, "WorkflowTools", "ModelToIdf job found at " << *index);
  } else {
    return OptionalInt();
  }

  int i = *index - 1;

  std::vector<openstudio::analysis::WorkflowStep> workflow = problem.workflow();
  if (i > 0 && workflow[i].isWorkItem() && workflow[i].workItem().jobkeyname == "pat-radiance-job")
  {
    return i;
  } else {
    return OptionalInt();
  }
}

*/
//OptionalInt getModelMeasureInsertStep(const openstudio::analysis::Problem &t_problem)
//{
//  OptionalInt stopIndex = t_problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
//  if (stopIndex)
//  {
//    int idx = *stopIndex;
//    if (idx > 0)
//    {
//      --idx;
//      std::vector<openstudio::analysis::WorkflowStep> workflow = t_problem.workflow();
//
//      if (workflow[idx].isWorkItem() && workflow[idx].workItem().jobkeyname == "pat-radiance-job")
//      {
//        return idx;
//      }
//    }
//  }
//
//  return stopIndex;
//}

/*
bool projectHasRadiance(const openstudio::analysisdriver::SimpleProject &t_project)
{
  return getProjectRadianceJobIndex(t_project);
}

void removeRadianceFromProject(openstudio::analysisdriver::SimpleProject &t_project)
{
  openstudio::analysis::Problem problem = t_project.analysis().problem();
  OptionalInt index = getProjectRadianceJobIndex(t_project);
  if (index)
  {
    problem.erase(problem.workflow()[*index]);
  }
}

void addRadianceToProject(openstudio::analysisdriver::SimpleProject &t_project)
{
  openstudio::analysis::Problem problem = t_project.analysis().problem();
  OptionalInt index = getProjectRadianceJobIndex(t_project);
  if (index)
  {
    return; // nothing to do
  }

  openstudio::runmanager::ConfigOptions co(true);
  std::vector<openstudio::runmanager::ToolInfo> rad = co.getTools().getAllByName("rad").tools();

  if (!rad.empty())
  {
    openstudio::path radiancePath = rad.back().localBinPath.parent_path();
    OptionalInt index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);

    if (index)
    {
      LOG_FREE(Debug, "WorkflowTools", "Adding radiance job at location " << *index);
      openstudio::runmanager::WorkItem wi = runmanager::Workflow::radianceDaylightCalculations(getOpenStudioRubyIncludePath(), radiancePath);
      wi.jobkeyname = "pat-radiance-job";
      problem.insert(*index, wi);
    }
  }

}
*/
}
