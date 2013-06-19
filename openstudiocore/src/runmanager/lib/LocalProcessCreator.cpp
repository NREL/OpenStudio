
#include "LocalProcessCreator.hpp"
#include "LocalProcess.hpp"

namespace openstudio {
namespace runmanager {

  LocalProcessCreator::LocalProcessCreator()
  {
  }

  boost::shared_ptr<Process> LocalProcessCreator::createProcess(
      const openstudio::runmanager::ToolInfo &t_tool,
      const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
      const std::vector<std::string> &t_parameters,
      const openstudio::path &t_outdir,
      const std::vector<openstudio::path> &t_expectedOutputFiles,
      const std::string &t_stdin,
      const openstudio::path &t_basePath,
      const boost::optional<std::pair<int,int> > &t_remoteId)
  {
    if (t_remoteId) { throw std::runtime_error("remote id set when creating local process"); }

    return boost::shared_ptr<Process>(
        new detail::LocalProcess(
          t_tool,
          t_requiredFiles,
          t_parameters,
          t_outdir,
          t_expectedOutputFiles,
          t_stdin,
          t_basePath));
  }



}
}

