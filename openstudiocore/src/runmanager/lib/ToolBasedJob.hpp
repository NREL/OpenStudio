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

#ifndef RUNMANAGER_LIB_TOOLBASEDJOB_HPP
#define RUNMANAGER_LIB_TOOLBASEDJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include <boost/optional.hpp>

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <boost/regex.hpp>
#include "../../energyplus/ErrorFile.hpp"

namespace openstudio {
namespace runmanager {
namespace detail {

  /// Base class for all Jobs that are simply ToolInfo based. EnergyPlusJob 
  /// as well as several others derive from this class.
  class ToolBasedJob : public Job_Impl
  {
    Q_OBJECT;

    public:
      /// Creates a ToolBasedJob that uses a single tool
      /// \param[in] t_uuid Process uuid
      /// \param[in] t_jobtype JobType
      /// \param[in] t_tools Tools object for Job creation
      /// \param[in] t_toolName name of tool to execute
      /// \param[in] t_params JobParams for Job creation
      /// \param[in] t_inputFiles Files for Job creation
      /// \param[in] t_noOutputError generate an error if no output files are created during the execution of this job
      ToolBasedJob(const UUID &t_uuid,
          const JobType &t_jobtype,
          const openstudio::runmanager::Tools &t_tools,
          const std::string &t_toolName,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_inputFiles,
          bool t_noOutputError,      
          const JobState &t_restoreData);

      /// Creates a ToolBasedJob that uses multiple tools
      /// \param[in] t_uuid Process uuid
      /// \param[in] t_jobtype JobType
      /// \param[in] t_tools Tools object for Job creation
      /// \param[in] t_toolName Vector of tool names of tools to execute
      /// \param[in] t_params JobParams for Job creation
      /// \param[in] t_inputFiles Files for Job creation
      /// \param[in] t_noOutputError generate an error if no output files are created during the execution of this job
      ToolBasedJob(const UUID &t_uuid,
          const JobType &t_jobtype,
          const openstudio::runmanager::Tools &t_tools,
          const std::vector<std::string> &t_toolNames,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_inputFiles,
          bool t_noOutputError,      
          const JobState &t_restoreData);

       virtual ~ToolBasedJob();

      // Reimplemented virtual functions from Job_Impl
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &t_pc);
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const;
      virtual Files outputFilesImpl() const;
      virtual std::string getOutput() const;
      virtual void cleanup();
      virtual std::string description() const;
      virtual std::string detailedDescription() const;


      /// \returns true if the set of ToolInfo has remote execution capabilities
      virtual bool remoteRunnable() const
      {
        if (!runnable())
        {
          return false;
        } else {
          try {
            for (size_t i = 0; i < m_toolNames.size(); ++i)
            {
              if (!getTool(m_toolNames[i]).remoteExecutable())
              {
                return false; // abort on the first not remote executable tool
              }
            }
            return true; // all of the required tools are remote executable
          } catch (const std::exception &) {
            // we couldn't find the required tool
            return false;
          }
        }
      }

      /// Requests that the tool stop execution
      virtual void requestStop();

    protected:

      bool stopRequested() const;

      /// Ensures run thread has completed before returning
      void shutdownJob();
 
      /// Called internally under a lock to implement the description()
      virtual std::string descriptionImpl() const = 0;

      /// Called internally under a lock to implement the detailedDescription()
      virtual std::string detailedDescriptionImpl() const = 0;

      /// Called internally when the job is in a runnable state and we are ready to get
      /// the version of the tool required to run the job, just in case it is dependent on 
      /// input files
      /// Job is guaranteed to be in arunnable state before the function is called.
      virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const = 0;

      /// Called internally when the process is starting. To be reimplemented in derived class, meant
      /// as a utility for derived class if it needs to perform some operation on this event
      virtual void startHandlerImpl() = 0;

      /// Called internally when all tools have finished executing.
      virtual void endHandlerImpl() {}

      /// Lets derived class notify base of parameters to send to process
      void addParameter(const std::string &t_toolname, const std::string &t_param);

      /// Lets derived class notify base of files required
      void addRequiredFile(const QUrl &t_from, const openstudio::path &t_local);

      /// Lets derived class notify base of files required
      void addRequiredFile(const openstudio::path &t_from, const openstudio::path &t_local);

      /// Lets derived class notify base of files required
      /// adds FileInfo file and all FileInfo required files
      void addRequiredFile(const FileInfo &t_from, const openstudio::path &t_local);

      /// lets derived class notify base of expected output files
      void addExpectedOutputFile(const openstudio::path &t_outfile);

      /// Notify the ToolBasedJob to copy the requirements of the first file 
      /// to the second file by extension. This is used to copy the weather file
      /// from in.xml to in.idf, for example. 
      ///
      /// \param[in] filename The required file (by name) to copy from input to output. If filename is an empty string
      ///                     then all required files are copied.
      void copyRequiredFiles(const FileInfo &infile, const std::string &extout, const openstudio::path &filename);

      /// Return the tool that is going to be used
      const ToolInfo &getTool(const std::string &t_toolName) const;

      /// Returns outputFiles provided explicitly by the subclass
      virtual Files outputFilesHandlerImpl() const { return Files(); }

      virtual void standardCleanImpl();

      /// Holds and handles all errors generated during a toolbasedjob run and returns them as a JobErrors
      /// object after the job has completed.
      class ErrorInfo
      {
        public:
          ErrorInfo();

          void exitCode(int);
          void exitStatus(QProcess::ExitStatus);
          void processError(QProcess::ProcessError);
          void processError(QProcess::ProcessError, const std::string &t_description);
          void errorFile(const openstudio::energyplus::ErrorFile &);
          void osResult(const openstudio::ruleset::OSResult &);
          void osResult(const std::vector<openstudio::ruleset::OSResult> &);

          /// Return a JobErrors object that represents all currently collected error information
          JobErrors errors();

        private:
          void addLogMessages(openstudio::runmanager::ErrorType t_type, 
              const std::vector<openstudio::LogMessage> &t_msgs, std::vector<std::pair<ErrorType, std::string> > &t_errors);

          void addLogMessage(openstudio::runmanager::ErrorType t_type, 
              const boost::optional<openstudio::LogMessage> &t_msg, std::vector<std::pair<ErrorType, std::string> > &t_errors);

          void addLogMessage(openstudio::runmanager::ErrorType t_type, 
              const openstudio::LogMessage &t_msg, std::vector<std::pair<ErrorType, std::string> > &t_errors);

          boost::optional<int> m_exit_code;
          boost::optional<openstudio::energyplus::ErrorFile> m_error_file;
          boost::optional<openstudio::ruleset::OSResult> m_osresult;
          boost::optional<QProcess::ExitStatus> m_exit_status;
          boost::optional<std::pair<QProcess::ProcessError, std::string> > m_process_error;
      };

     private:
      /// Called internally when the job is in a runnable state and we are ready to get
      /// the version of the tool required to run the job, just in case it is dependent on 
      /// input files
      /// Job is guaranteed to be in a runnable state before the function is called.
      ToolVersion getToolVersion(const std::string &t_toolName) const;

      /// Called internally when the process is starting. To be reimplemented in derived class, meant
      /// as a utility for derived class if it needs to perform some operation on this event
      void startHandler();


      REGISTER_LOGGER("openstudio.runmanager.ToolBasedJob");

      void updateOutOfDateStatus(bool newvalue) const;
      std::vector<std::pair<QUrl, openstudio::path> > complete_required_files();
      std::vector<std::pair<openstudio::path, openstudio::path> > acquireRequiredFiles(
          const std::vector<std::pair<QUrl, openstudio::path> > &t_urls);

      ToolInfo getTool(const std::string &t_name, const ToolVersion &t_version) const;
      std::string getNextToolName();
      void startTool(const std::string &t_toolName);
      void startNextTool();
      void handleToolStartError(const std::exception &e);
      void pushBackRequiredFile(const QUrl &t_from, const openstudio::path &t_local); //< Add a file to the list to track 


      typedef std::set<FileInfo> FileSet;


      mutable QReadWriteLock m_mutex;
      mutable QMutex m_impl_mutex;
      mutable QReadWriteLock m_stopMutex;

      bool m_stopRequested;

      std::vector<std::string> m_toolNames;

      mutable std::map<std::string, openstudio::runmanager::ToolInfo> m_foundTools;

      std::map<openstudio::path, FileTrack> m_trackedFiles; //< List of files being tracked for outOfDate status

      /// Output directory
      openstudio::path m_outdir;

      /// Used to watch the input files for changes, to update outofdate status
//      QFileSystemWatcher m_inputfile_watcher;

      /// List of files that are known about
      FileSet m_outfiles;

      mutable bool m_last_out_of_date;


      std::vector<std::pair<QUrl, openstudio::path> > m_required_files;
      std::map<std::string, std::vector<std::string> > m_parameters;
      std::set<openstudio::path> m_expectedOutputFiles;
      std::shared_ptr<ProcessCreator> m_process_creator;
      std::map<ToolInfo, std::shared_ptr<Process> > m_processes;
      std::shared_ptr<Process> m_currentprocess;
      std::vector<std::tuple<FileInfo, std::string, openstudio::path> > m_copyRequiredFiles;

      std::set<std::pair<openstudio::path, openstudio::path> > m_addedRequiredFiles;

      /// Current collected error information for the running job
      ErrorInfo m_error_info;

      bool m_noOutputError; ///< if no output file is generated then it is an error case

      size_t m_currentToolIndex; ///< Index of the currently processing tool for this job

    private slots:
      void remoteStarted(int t_remoteid, int t_remotetaskid);
      void remoteFinished(int t_remoteid, int t_remotetaskid);

      /// Connected to m_process
      void processStarted();

      /// Connected to m_process
      void processOutputFileChanged(const openstudio::runmanager::FileInfo &t_info);

      /// Connected to m_process
      void processStandardOutDataAdded(const std::string &data);

      /// Connected to m_process
      void processStandardErrDataAdded(const std::string &data);

      /// Connected to m_process
      void processError(QProcess::ProcessError t_error);

      /// Connected to m_process
      void processError(QProcess::ProcessError, const std::string &t_description);

      /// Connected to m_process
      void processFinished(int exitcode, QProcess::ExitStatus);

      /// Connected to m_process
      void processStatusChanged(const openstudio::runmanager::AdvancedStatus &t_stat);

      /// handles processing of output files
      void processResultFiles(const openstudio::path &t_outpath, const Files &t_outfiles);

  }; 

}
}
}
#endif // RUNMANAGER_LIB_TOOLBASEDJOB_HPP
