#ifndef OPENSTUDIO_WORKFLOW_TOOLS_HPP
#define OPENSTUDIO_WORKFLOW_TOOLS_HPP

#include <vector>
#include <string>
#include <boost/optional.hpp>

namespace openstudio
{
  namespace runmanager
  {
    class RunManager;
  }

  namespace model
  {
    class Model;
  }

  namespace analysis
  {
    class Problem;
  }

  namespace analysisdriver
  {
    class SimpleProject;
  }

  void getRadiancePreRunWarningsAndErrors(std::vector<std::string> &t_warnings, 
      std::vector<std::string> &t_errors,
      openstudio::runmanager::RunManager &t_runManager,
      boost::optional<openstudio::model::Model> &t_model);

  bool projectHasRadiance(const openstudio::analysisdriver::SimpleProject &t_project);

  void addRadianceToProject(openstudio::analysisdriver::SimpleProject &t_project);

  void removeRadianceFromProject(openstudio::analysisdriver::SimpleProject &t_project);

  boost::optional<int> getModelMeasureInsertStep(const openstudio::analysis::Problem &t_problem);
}

#endif
