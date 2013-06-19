#ifndef OPENSTUDIO_RUNMANAGER_LOCALPROCESSCREATOR_HPP__
#define OPENSTUDIO_RUNMANAGER_LOCALPROCESSCREATOR_HPP__

#include "ProcessCreator.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <utilities/core/Path.hpp>

namespace openstudio {
namespace runmanager {


  /// Implements ProcessCreator interface to create a process locally.
  ///
  /// Inherits from openstudio::runmanager::ProcessCreator
  class RUNMANAGER_API LocalProcessCreator : public ProcessCreator
  {
    Q_OBJECT;

    public:
      LocalProcessCreator();

      /// Creates a local process
      /// \param[in] t_tool The tool to execute locally
      /// \param[in] t_requiredFiles The vector of files needed to execute the tool
      /// \param[in] t_parameters The vector of parameters to pass to tool during execution
      /// \param[in] t_outdir Directory to watch for output files to be created 
      /// \param[in] t_expectedOutputFiles Output files in various directories to explicitly watch
      /// \param[in] t_stdin Input to send to the process over stdin after it has started
      /// \param[in] t_basePath Base path from which required files should be evaluated if the required file
      ///                       is a relative path and does not reside in the tool path
      /// \param[in] t_remoteId optional remote id for recreating remote process. Throws exception if set
      /// \returns the created Process
      virtual boost::shared_ptr<Process> createProcess(
          const openstudio::runmanager::ToolInfo &t_tool,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath,
          const boost::optional<std::pair<int,int> > &t_remoteId);

      /// \returns false, LocalProcessCreator does not create remote jobs
      virtual bool isRemoteManager() const
      {
        return false;
      }

  };

}
}

#endif
