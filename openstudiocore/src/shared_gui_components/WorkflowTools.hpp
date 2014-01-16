#ifndef OPENSTUDIO_WORKFLOW_TOOLS_HPP
#define OPENSTUDIO_WORKFLOW_TOOLS_HPP

#include <vector>
#include <string>

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

  void getRadiancePreRunWarningsAndErrors(std::vector<std::string> &_warnings, std::vector<std::string> &t_errors,
      openstudio::runmanager::RunManager &t_runManager,
      openstudio::model::Model &t_model);
}

#endif
