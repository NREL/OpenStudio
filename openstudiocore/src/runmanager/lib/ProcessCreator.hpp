#ifndef OPENSTUDIO_RUNMANAGER_PROCESSCREATOR_HPP__
#define OPENSTUDIO_RUNMANAGER_PROCESSCREATOR_HPP__

#include "Process.hpp"
#include "ToolInfo.hpp"
#include <utilities/core/UUID.hpp>

namespace openstudio {
namespace runmanager {

  /// Interface for Creating processes. Abstracted such that the Job does not need to
  /// care if the processes are running locally or remotely
  class ProcessCreator : public QObject
  {
    Q_OBJECT;

    public:
      /** \param[in] t_tooltoexecute The tool to execute locally
       *  \param[in] t_requiredFiles The vector of files needed to execute the tool
       *  \param[in] t_parameters The vector of parameters to pass to tool during execution
       *  \param[in] t_outdir Directory to watch for output files to be created 
       *  \param[in] t_expectedOutputFiles Output files in various directories to explicitly watch
       *  \param[in] t_stdin Input to send to the process over stdin after it has started
       *  \param[in] t_basePath Base path from which required files should be evaluated if the required file
       *                        is a relative path and does not reside in the tool path
       *  \param[in] t_remoteId integer identifier
       *  \returns the created Process */
      virtual boost::shared_ptr<Process> createProcess(
          const ToolInfo &t_tooltoexecute,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath,
          const boost::optional<std::pair<int, int> > &t_remoteId) = 0;

      /// \returns true if the ProcessCreator implementation creates remote processes
      virtual bool isRemoteManager() const = 0;



  };

}
}

#endif
