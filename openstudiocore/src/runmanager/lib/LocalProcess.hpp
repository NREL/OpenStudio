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

      virtual void start();
      virtual bool running() const;
      virtual void waitForFinished();
      virtual void cleanup(const std::vector<std::string> &t_files);
      virtual std::vector<FileInfo> outputFiles() const;
      virtual std::vector<FileInfo> inputFiles() const;


    protected:
      virtual void stopImpl();
      virtual void cleanUpRequiredFiles();

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
      const std::vector<std::pair<openstudio::path, openstudio::path> > m_requiredFiles; //< Files requried to execute tool
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
