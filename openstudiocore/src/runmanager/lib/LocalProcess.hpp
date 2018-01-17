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

#ifndef RUNMANAGER_LIB_LOCALPROCESS_HPP
#define RUNMANAGER_LIB_LOCALPROCESS_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Path.hpp"
#include "Job_Impl.hpp"
#include "../../energyplus/ErrorFile.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QMutex>

namespace openstudio {
namespace runmanager {
namespace detail {

  // We are subclassing QProcess to deal with the case where a parent or embedded context, such as
  // node.js, intercepts child signals before they are handled by QProcessManager.
  // http://stackoverflow.com/questions/7113506/qprocess-becomes-defunct-and-unable-to-start-again/7122340#7122340
  class MyQProcess : public QProcess
  {
    Q_OBJECT;
    private:
      REGISTER_LOGGER("openstudio.runmanager.MyQProcess");

    public:
      MyQProcess();
      void checkProcessStatus();

    signals:
      void zombied(QProcess::ProcessError t_e);

    private:
      int m_exitedCount;
      int m_postExitCheckCount;
  };

  /**
   * A utility class for creating a Process object that runs on the local system
   * \sa openstudio::runmanager::detail::Process for details on what a Process performs
   */
  class LocalProcess : public Process
  {
    Q_OBJECT;

    public:
      /// LocalProcess constructor
      /// \param[in] t_tool The tool to execute locally
      /// \param[in] t_requiredFiles The vector of files needed to execute the tool
      /// \param[in] t_parameters The vector of parameters to pass to tool during execution
      /// \param[in] t_outdir Directory to watch for output files to be created 
      /// \param[in] t_expectedOutputFiles Output files in various directories to explicitly watch
      /// \param[in] t_stdin Input to send to the process over stdin after it has started
      /// \param[in] t_basePath Base path from which required files should be evaluated if the required file
      ///                       is a relative path and does not reside in the tool path
      LocalProcess(
          const openstudio::runmanager::ToolInfo &t_tool,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath);

      virtual ~LocalProcess();

      virtual void start() override;
      virtual bool running() const override;
      virtual void waitForFinished() override;
      virtual void cleanup(const std::vector<std::string> &t_files) override;
      virtual std::vector<FileInfo> outputFiles() const override;
      virtual std::vector<FileInfo> inputFiles() const override;


    protected:
      virtual void stopImpl() override;
      virtual void cleanUpRequiredFiles() override;

    private:
      REGISTER_LOGGER("openstudio.runmanager.LocalProcess");

      typedef std::set<FileInfo> FileSet;


      /// \param[in] qba QByteArray of output data to process
      void handleOutput(const QByteArray &qba, bool stderror);

      /// Return the ordered set of files in the given directory
      /// used to determine when files have changed
      FileSet dirFiles(const QString &dir) const;

      static void kill(QProcess &t_process, bool t_force); //< Does an appropriate process tree kill on Windows

      static std::set<openstudio::path> copyRequiredFiles(const ToolInfo &t_tool, const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles, 
          const openstudio::path &t_basePath);

      /// Immutable members, do not need thread mutex protection
      const openstudio::runmanager::ToolInfo m_tool; //< Tool that is executing
      const std::vector<std::pair<openstudio::path, openstudio::path> > m_requiredFiles; //< Files required to execute tool
      const std::vector<std::string> m_parameters; //< parameters to pass to tool
      const openstudio::path m_outdir;  //< Tool's output directory
      const std::vector<openstudio::path> m_expectedOutputFiles; //< List of expected output files
      const std::string m_stdin; //< Stdin to send after process has tarted

      /// Set of files that have been copied into place because they were required and can be deleted after the process has completed
      const std::set<openstudio::path> m_copiedRequiredFiles;

      /// List of files that are known about.
      FileSet m_outfiles;

      /// QProcess used to monitor the execution of the process.
      MyQProcess m_process;


      QTimer m_fileCheckTimer;

      mutable QMutex m_mutex;


    private slots:
      /// connected to QProcess::error
      void processError(QProcess::ProcessError t_e);

      /// connected to MyQProcess::zombied
      void processZombied(QProcess::ProcessError t_e);

      /// connected to QProcess::finished
      void processFinished(int t_exitCode, QProcess::ExitStatus t_exitStatus);

      /// connected to QProcess::readyReadStandardOutput
      void processReadyReadStandardOutput();

      /// connected to QProcess::readyReadStandardOutput
      void processReadyReadStandardError();

      /// connected to QProcess::started
      void processStarted();
      
      /// connected to QProcess::stateChanged
      void processStateChanged(QProcess::ProcessState s);

      /// connected to QFilesystemWatcher::directoryChanged
      void directoryChanged(const QString& d);

      /// connected to fileCheckTimer
      void directoryChanged();

      /// connected to QFilesystemWatcher::fileChanged
      void fileChanged(const QString& d);

      void emitUpdatedFileInfo(const FileInfo &fi);

  }; 

}
}
}
#endif // RUNMANAGER_LIB_LOCALPROCESS_HPP
