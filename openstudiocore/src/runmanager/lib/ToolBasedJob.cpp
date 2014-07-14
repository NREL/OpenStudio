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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "ToolBasedJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/PathHelpers.hpp"

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  ToolBasedJob::ErrorInfo::ErrorInfo()
  {
  }

  void ToolBasedJob::ErrorInfo::exitCode(int code)
  {
    m_exit_code = code;
  }

  void ToolBasedJob::ErrorInfo::exitStatus(QProcess::ExitStatus e)
  {
    m_exit_status = e;
  }

  void ToolBasedJob::ErrorInfo::processError(QProcess::ProcessError e)
  {
    m_process_error = std::make_pair(e, "");
  }

  void ToolBasedJob::ErrorInfo::processError(QProcess::ProcessError e, const std::string &t_message)
  {
    m_process_error = std::make_pair(e, t_message);
  }


  void ToolBasedJob::ErrorInfo::errorFile(const openstudio::energyplus::ErrorFile &e)
  {
    m_error_file = e;
  }

  void ToolBasedJob::ErrorInfo::osResult(const openstudio::ruleset::OSResult &r)
  {
    m_osresult = r;
  }

  void ToolBasedJob::ErrorInfo::osResult(const std::vector<openstudio::ruleset::OSResult> &r)
  {
    if (r.size() > 1)
    {
      boost::optional<LogMessage> initialCondition;
      boost::optional<LogMessage> finalCondition;


      openstudio::ruleset::OSResultValue value = openstudio::ruleset::OSResultValue::NA;

      openstudio::ruleset::OSResult result;

      for (size_t i = 0; i < r.size(); ++i)
      {
        boost::optional<LogMessage> rinitialCondition = r[i].initialCondition();
        boost::optional<LogMessage> rfinalCondition = r[i].finalCondition();

        std::vector<LogMessage> rerrors = r[i].errors();
        std::vector<LogMessage> rwarnings = r[i].warnings();
        std::vector<LogMessage> rinfo = r[i].info();

        openstudio::ruleset::OSResultValue rvalue = r[i].value();


        if (initialCondition && rinitialCondition)
        {
          initialCondition = LogMessage(initialCondition->logLevel(), initialCondition->logChannel(), initialCondition->logMessage() + " Script: " + boost::lexical_cast<std::string>(i+1) + " " + rinitialCondition->logMessage());
        } else if (rinitialCondition) {
          initialCondition = LogMessage(rinitialCondition->logLevel(), rinitialCondition->logChannel(), "Script: " + boost::lexical_cast<std::string>(i+1) + " " + rinitialCondition->logMessage());
        }

        if (finalCondition && rfinalCondition)
        {
          finalCondition = LogMessage(finalCondition->logLevel(), finalCondition->logChannel(), finalCondition->logMessage() + " Script: " + boost::lexical_cast<std::string>(i+1) + " " + rfinalCondition->logMessage());
        } else if (rfinalCondition) {
          finalCondition = LogMessage(rfinalCondition->logLevel(), rfinalCondition->logChannel(), "Script: " + boost::lexical_cast<std::string>(i+1) + " " + rfinalCondition->logMessage());
        }

        for (const auto & rerror : rerrors)
        {
          result.addError(rerror.logChannel(), "Script: " + boost::lexical_cast<std::string>(i+1) + " " + rerror.logMessage());
        }

        for (const auto & rwarning : rwarnings)
        {
          result.addWarning(rwarning.logChannel(), "Script: " + boost::lexical_cast<std::string>(i+1) + " " + rwarning.logMessage());
        }

        for (const auto & info : rinfo)
        {
          result.addInfo(info.logChannel(), "Script: " + boost::lexical_cast<std::string>(i+1) + " " + info.logMessage());
        }

        if (value == openstudio::ruleset::OSResultValue::Fail || rvalue == openstudio::ruleset::OSResultValue::Fail)
        {
          value = openstudio::ruleset::OSResultValue::Fail;
        } else if (value == openstudio::ruleset::OSResultValue::Success || rvalue == openstudio::ruleset::OSResultValue::Success) {
          value = openstudio::ruleset::OSResultValue::Success;
        }
      }

      result.setValue(value);

      if (initialCondition)
      {
        result.setInitialCondition(initialCondition->logChannel(), initialCondition->logMessage());
      }

      if (finalCondition)
      {
        result.setFinalCondition(finalCondition->logChannel(), finalCondition->logMessage());
      }
    } else if (r.size() == 1) {
      m_osresult = r.front();
    }
  }


  void ToolBasedJob::ErrorInfo::addLogMessages(openstudio::runmanager::ErrorType t_type, 
      const std::vector<openstudio::LogMessage> &t_msgs, std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    for (const auto & msg : t_msgs)
    {
      addLogMessage(t_type, msg, t_errors);
    }
  }

  void ToolBasedJob::ErrorInfo::addLogMessage(openstudio::runmanager::ErrorType t_type, 
      const boost::optional<openstudio::LogMessage> &t_msg, std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    if (t_msg)
    {
      addLogMessage(t_type, *t_msg, t_errors);
    }
  }

  void ToolBasedJob::ErrorInfo::addLogMessage(openstudio::runmanager::ErrorType t_type, 
      const openstudio::LogMessage &t_msg, std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    std::string channel = t_msg.logChannel();

    if (!channel.empty())
    {
      //t_errors.push_back(std::make_pair(t_type, "[" + channel + "] " + t_msg.logMessage()));
      t_errors.push_back(std::make_pair(t_type, t_msg.logMessage()));
    } else {
      t_errors.push_back(std::make_pair(t_type, t_msg.logMessage()));
    }
  }

  JobErrors ToolBasedJob::ErrorInfo::errors()
  {
    // Check which types of error information has been set (exitstatus, processerror and energyplus error_file)
    // and generate a JobErrors object with string representations of each error and warning discovered
    std::vector<std::pair<ErrorType, std::string> > errors;

    ruleset::OSResultValue result = ruleset::OSResultValue::Success;

    if (m_exit_status)
    {
      if (*m_exit_status == QProcess::CrashExit)
      {
        result = ruleset::OSResultValue::Fail;
        errors.push_back(std::make_pair(ErrorType::Error, "Process crashed."));
      }
    }

    if (m_exit_code)
    {
      if (*m_exit_code != 0)
      {
        result = ruleset::OSResultValue::Fail;
        errors.push_back(std::make_pair(ErrorType::Error, "Process exited with a non-zero exit code of: " + boost::lexical_cast<std::string>(*m_exit_code)));
      }
    }

    if (m_process_error)
    {
      std::string description = m_process_error->second;
      if (!description.empty())
      {
        description = " " + description;
      }

      result = ruleset::OSResultValue::Fail;
      switch(m_process_error->first)
      {
        case QProcess::FailedToStart:
          errors.push_back(std::make_pair(ErrorType::Error, "Process failed to start." + description));
          break;
        case QProcess::Crashed:
          errors.push_back(std::make_pair(ErrorType::Error, "Process crashed." + description));
          break;
        case QProcess::WriteError:
          errors.push_back(std::make_pair(ErrorType::Error, "Error writing to process." + description));
          break;
        case QProcess::ReadError:
          errors.push_back(std::make_pair(ErrorType::Error, "Error reading from process." + description));
          break;
        default:
          errors.push_back(std::make_pair(ErrorType::Error, "Unknown error occurred." + description));
          break;
      }
    }

    if (m_osresult)
    {
      if (result != ruleset::OSResultValue::Fail)
      {
        // If we haven't already failed for some reason above, such as a process error,
        // then trust the value parsed from the osresult file
        result = m_osresult->value();
      }

      addLogMessages(runmanager::ErrorType::Error, m_osresult->errors(), errors);
      addLogMessages(runmanager::ErrorType::Warning, m_osresult->warnings(), errors);
      addLogMessages(runmanager::ErrorType::Info, m_osresult->info(), errors);
      addLogMessage(runmanager::ErrorType::InitialCondition, m_osresult->initialCondition(), errors);
      addLogMessage(runmanager::ErrorType::FinalCondition, m_osresult->finalCondition(), errors);
    }

    if (m_error_file) 
    {
      if (result == ruleset::OSResultValue::Success
                   && m_error_file->completed() 
                   && m_error_file->completedSuccessfully())
      {
        result = ruleset::OSResultValue::Success;
      } else {
        result = ruleset::OSResultValue::Fail;
      }

      if (!m_error_file->completed())
      {
        errors.push_back(std::make_pair(ErrorType::Error, "Error report file indicates that the process did not complete"));
      }

      if (!m_error_file->completedSuccessfully())
      {
        errors.push_back(std::make_pair(ErrorType::Error, "Error report file indicates that the process did not complete successfully"));
      }

      std::vector<std::string> efwarnings = m_error_file->warnings();
      std::vector<std::string> efsevereErrors = m_error_file->severeErrors();
      std::vector<std::string> effatalErrors = m_error_file->fatalErrors();

      for (const auto & efwarning : efwarnings)
      {
        errors.push_back(std::make_pair(ErrorType::Warning, efwarning));
      }

      for (const auto & efsevereError : efsevereErrors)
      {
        errors.push_back(std::make_pair(ErrorType::Error, efsevereError));
      }

      for (const auto & effatalError : effatalErrors)
      {
        errors.push_back(std::make_pair(ErrorType::Error, effatalError));
      }

    }

    return JobErrors(result, errors);
  }


  ToolBasedJob::ToolBasedJob(const UUID &t_uuid,
          const JobType &t_type,
          const openstudio::runmanager::Tools &t_tools,
          const std::string &t_toolName,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_inputFiles,
          bool t_noOutputError,
          const JobState &t_restoreState)
    : Job_Impl(t_uuid, t_type, t_tools, t_params, t_inputFiles, t_restoreState),
      m_stopRequested(false),
      m_last_out_of_date(true),
      m_noOutputError(t_noOutputError),
      m_currentToolIndex(0)
  {
    m_toolNames.push_back(t_toolName);
  }

  ToolBasedJob::ToolBasedJob(const UUID &t_uuid,
          const JobType &t_type,
          const openstudio::runmanager::Tools &t_tools,
          const std::vector<std::string> &t_toolNames,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_inputFiles,
          bool t_noOutputError,
          const JobState &t_restoreState)
    : Job_Impl(t_uuid, t_type, t_tools, t_params, t_inputFiles, t_restoreState),
      m_stopRequested(false),
      m_toolNames(t_toolNames),
      m_last_out_of_date(true),
      m_noOutputError(t_noOutputError),
      m_currentToolIndex(0)
  {
  }

  ToolBasedJob::~ToolBasedJob()
  {
    disconnect();
    requestStop();
    /*
    QWriteLocker l(&m_mutex);
    if (m_currentprocess)
    {
      m_currentprocess->waitForFinished();
      m_currentprocess.reset();
    }
    l.unlock();
    */
    assert(QThread::currentThread() != this);
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }


  bool ToolBasedJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    QReadLocker l(&m_mutex);
    bool ret = false;
    if (!t_lastrun)
    {
      ret = true;
    } else {     
      ret = filesChanged(m_trackedFiles, *t_lastrun);
    }

    l.unlock();

    updateOutOfDateStatus(ret);

    return ret;
  }

  void ToolBasedJob::cleanup()
  {
    QWriteLocker l(&m_mutex);

    std::set<std::string> filteredfiles;

    for (const auto & process : m_processes)
    {
      std::vector<FileInfo> files = process.second->outputFiles();

      for(const auto & file : files)
      {
        if (boost::regex_match(file.filename, process.first.outFileFilter))
        {
          filteredfiles.insert(file.filename);
        }
      }
    }

    JobOutputCleanup joc(nullptr,nullptr,std::vector<std::string>(filteredfiles.begin(), filteredfiles.end()));

    if (joc.exec() == QDialog::Accepted)
    {
      std::vector<std::string> result = joc.getSelectedFiles();

      for (const auto & process : m_processes)
      {
        process.second->cleanup(result);
      }
    }
  }

  void ToolBasedJob::addParameter(const std::string &t_toolName, const std::string &t_param)
  {
    QWriteLocker l(&m_mutex);
    m_parameters[t_toolName].push_back(t_param);
  }

  void ToolBasedJob::pushBackRequiredFile(const QUrl &t_from, const openstudio::path &t_local)
  {
    m_required_files.push_back(std::make_pair(t_from, t_local));

    if (t_from.scheme() == "file")
    {
      addFile(m_trackedFiles, openstudio::toPath(t_from.toLocalFile()));
    }
  }

  void ToolBasedJob::addRequiredFile(const openstudio::path &t_from, const openstudio::path &t_local)
  { 
    QWriteLocker l(&m_mutex);
    pushBackRequiredFile(QUrl::fromLocalFile(toQString(t_from.native())), t_local);
  }

  void ToolBasedJob::addRequiredFile(const QUrl &t_from, const openstudio::path &t_local)
  { 
    QWriteLocker l(&m_mutex);
    pushBackRequiredFile(t_from, t_local);
  }

  void ToolBasedJob::addRequiredFile(const FileInfo &t_from, const openstudio::path &t_local)
  { 
    QWriteLocker l(&m_mutex);
    pushBackRequiredFile(QUrl::fromLocalFile(toQString(t_from.fullPath.native())), t_local);

    std::vector<std::pair<QUrl, openstudio::path> > filereqs = t_from.requiredFiles;
//    m_inputfile_watcher.addPath(toQString(t_from.fullPath));

    l.unlock();

    for (const auto & filereq : filereqs)
    {
      addRequiredFile(filereq.first, filereq.second);
    }
  }

  void ToolBasedJob::addExpectedOutputFile(const openstudio::path &t_outfile)
  {
    QWriteLocker l(&m_mutex);
    m_expectedOutputFiles.insert(t_outfile);
  }

  std::vector<std::pair<QUrl, openstudio::path> > ToolBasedJob::complete_required_files()
  {
    std::vector<std::pair<QUrl, openstudio::path> > ret;

    openstudio::path dir = outdir();

    for (const auto & requiredFile : m_required_files)
    {
      if (requiredFile.second.is_complete())
      {
        LOG(Debug, "Not completing required file " << openstudio::toString(requiredFile.second) << " it is already complete");
      } else {
        ret.push_back(std::make_pair(requiredFile.first, dir / requiredFile.second));
      }
    }

    return ret;
  }

  const ToolInfo &ToolBasedJob::getTool(const std::string &t_toolName) const
  {
    QWriteLocker l(&m_mutex);
    if (m_foundTools.find(t_toolName) == m_foundTools.end())
    {
      m_foundTools[t_toolName] = getTool(t_toolName, getToolVersion(t_toolName));
    }

    return m_foundTools[t_toolName];
  }

  ToolInfo ToolBasedJob::getTool(const std::string &t_name, const ToolVersion &t_version) const
  {
    LOG(Info, "Requesting Tool: " << t_name << " version: " << t_version.toString());
    ToolInfo tool = allTools().getTool(t_name, t_version);
    LOG(Info, "Got Tool: " << tool.name << " version: " << tool.version.toString());

    return tool;
  }

  std::vector<std::pair<openstudio::path, openstudio::path> > ToolBasedJob::acquireRequiredFiles(
      const std::vector<std::pair<QUrl, openstudio::path> > &t_urls)
  {
    class RequiredFileChecker
    {
      public:
        /// Track which required files have been added, returning false if the destination exists
        /// while logging an error as to what exactly happened
        bool addFile(const openstudio::path &t_from, const openstudio::path &t_to)
        {
          std::map<openstudio::path, openstudio::path>::const_iterator itr = m_files.find(t_to);

          if (itr != m_files.end())
          {
            LOG(Error, "acquireRequiredFiles: Unable to add file From: '" << openstudio::toString(t_from) << "' to '" << openstudio::toString(t_to) << "'. Existing required file from: '" << openstudio::toString(itr->second) << "' to '" << openstudio::toString(itr->first) << "' already exists.");
            return false;
          } else {
            m_files[t_to] = t_from;
            return true;
          }
        }

      private:
      std::map<openstudio::path, openstudio::path> m_files;
    };

    RequiredFileChecker rfc;

    std::vector<std::pair<openstudio::path, openstudio::path> > retval;

    for (const auto & url : t_urls)
    {
      if (url.first.scheme() == "file")
      {
        openstudio::path file = openstudio::toPath(url.first.toLocalFile());

        
        if (openstudio::toString(file.extension()) == ".osm")
        {
          openstudio::path osmdir = file.parent_path() / boost::filesystem::basename(file);
          LOG(Debug, "Examining osm for requirements: " << openstudio::toString(file) << " with basepath: " << openstudio::toString(getBasePath()));

          openstudio::path filespath = osmdir / openstudio::toPath("files");
          openstudio::path addfolder;

          if (!boost::filesystem::exists(filespath) || !boost::filesystem::is_directory(filespath))
          {
            osmdir = getBasePath();
            filespath = getBasePath() / openstudio::toPath("files");
            addfolder = boost::filesystem::basename(file);
            LOG(Debug, "OSM dir did not exist, now using basepath: " << openstudio::toString(osmdir) );
          }

          if (!boost::filesystem::exists(filespath) || !boost::filesystem::is_directory(filespath))
          {
            osmdir = file.parent_path();
            filespath = osmdir / openstudio::toPath("resources") / openstudio::toPath("files");
            addfolder = boost::filesystem::basename(file);
            LOG(Debug, "OSM dir did not exist, now using parent resources folder: " << openstudio::toString(osmdir) );
          }

          if (!boost::filesystem::exists(filespath) || !boost::filesystem::is_directory(filespath))
          {
            osmdir = file.parent_path().parent_path();
            filespath = osmdir / openstudio::toPath("files");
            addfolder = boost::filesystem::basename(file);
            LOG(Debug, "OSM dir did not exist, now using parent resources folder: " << openstudio::toString(osmdir) );
          }


          if (boost::filesystem::exists(filespath)
              && boost::filesystem::is_directory(filespath))
          {
            // it's an OSM file, we want to also bring along its reqs
            for(boost::filesystem::recursive_directory_iterator pitr(filespath);
                pitr != boost::filesystem::recursive_directory_iterator();
                ++pitr)
            {
              openstudio::path p = pitr->path();
              if (boost::filesystem::is_regular_file(p) || boost::filesystem::is_symlink(p))
              {
                openstudio::path relative = openstudio::relativePath(p, osmdir);

                openstudio::path::iterator pathitr = relative.begin();
                if (!relative.empty() && *pathitr == openstudio::toPath("resources"))
                {
                  // drop the resources bit

                  ++pathitr;
                  openstudio::path newpath;
                  while (pathitr != relative.end())
                  {
                    newpath /= *pathitr;
                    ++pathitr;
                  }

                  relative = newpath;
                }

                if (!relative.empty())
                {
                  if (!addfolder.empty())
                  {
                    relative = addfolder / relative;
                  }

                  if (rfc.addFile(p, url.second.parent_path() / relative))
                  {
                    m_addedRequiredFiles.insert(std::make_pair(p, relative));

                    LOG(Info, "Adding OSM requisite file: " << openstudio::toString(p) 
                        << " to be installed at: " << openstudio::toString(relative));
                    retval.push_back(std::make_pair(p, url.second.parent_path() / relative));
                  }
                }
              }
            }
          }
        }

        if (rfc.addFile(file, url.second))
        {
          retval.push_back(std::make_pair(file, url.second));
        }

      } else {
        throw std::runtime_error("Unsupported file scheme: " + toString(url.first.scheme()));
      }
    }

    return retval;
  }

  void ToolBasedJob::startTool(const std::string &t_toolName)
  {
    LOG(Info, "Starting Tool: " << t_toolName);

    ToolInfo ti = getTool(t_toolName);

    openstudio::path outpath = outdir();

    QWriteLocker l(&m_mutex);
  std::vector<std::pair<openstudio::path, openstudio::path> > requiredFiles = acquireRequiredFiles(complete_required_files());
  //m_copiedRequiredFiles.insert(requiredFiles.begin(), requiredFiles.end());
    std::shared_ptr<Process> process = m_process_creator->createProcess(ti,
        requiredFiles, m_parameters[t_toolName],
        outpath, std::vector<openstudio::path>(m_expectedOutputFiles.begin(), m_expectedOutputFiles.end()), "\n\n",
        getBasePath(),
        getRemoteId());

    if (m_currentprocess)
    {
      m_currentprocess->disconnect(this);
    }

    m_currentprocess = process;
    m_processes[ti] = process;

    connect(process.get(), SIGNAL(started()), 
        this, SLOT(processStarted()));
    connect(process.get(), SIGNAL(outputFileChanged(const openstudio::runmanager::FileInfo &)), 
        this, SLOT(processOutputFileChanged(const openstudio::runmanager::FileInfo &)));
    connect(process.get(), SIGNAL(standardErrDataAdded(const std::string &)),
        this, SLOT(processStandardErrDataAdded(const std::string &)));
    connect(process.get(), SIGNAL(standardOutDataAdded(const std::string &)),
        this, SLOT(processStandardOutDataAdded(const std::string &)));
    connect(process.get(), SIGNAL(error(QProcess::ProcessError)),
        this, SLOT(processError(QProcess::ProcessError)));
    connect(process.get(), SIGNAL(error(QProcess::ProcessError, const std::string &)),
      this, SLOT(processError(QProcess::ProcessError, const std::string &)));
    connect(process.get(), SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(process.get(), SIGNAL(statusChanged(const openstudio::runmanager::AdvancedStatus &)),
        this, SLOT(processStatusChanged(const openstudio::runmanager::AdvancedStatus &)));
    connect(process.get(), SIGNAL(remoteStarted(int, int)),
        this, SLOT(remoteStarted(int, int)));
    connect(process.get(), SIGNAL(remoteFinished(int, int)),
        this, SLOT(remoteFinished(int, int)));

    process->start();
  }

  std::string ToolBasedJob::getNextToolName()
  {
    QWriteLocker l(&m_mutex);

    if (m_currentToolIndex >= m_toolNames.size())
    {
      throw std::runtime_error("No more tool names");
    }

    std::string name = m_toolNames[m_currentToolIndex];
    ++m_currentToolIndex;

    return name;
  }

  void ToolBasedJob::startNextTool()
  {
    std::string nextToolName;

    try {
//      LOG(Debug, "Getting next tool name: " << toString(uuid()));
      nextToolName = getNextToolName();
    } catch (const std::runtime_error &) {
      LOG(Debug, "Last Tool Completed: " << toString(uuid()));
      updateOutOfDateStatus(outOfDate());
      quit();
      return;
    }

    startTool(nextToolName);
  }

  void ToolBasedJob::startImpl(const std::shared_ptr<ProcessCreator> &t_pc)
  {
    // This means there's no reason to start up
    if (stopRequested())
    {
      quit();
      return;
    }
    
    {
      openstudio::path outpath = outdir();
      boost::filesystem::create_directories(outpath);
      openstudio::path stdoutpath = outpath / toPath("stdout");
      // this ofstream exists to handle a corner case timing issue, to ensure that stdout is created no matter what
      std::ofstream(toString(stdoutpath).c_str(), std::ios_base::out | std::ios_base::trunc);
    }

    QWriteLocker l(&m_mutex);
    m_currentToolIndex=0;
    m_currentprocess.reset();
    m_process_creator.reset();
    m_processes.clear();

    /*
    if (!runnable())
    {
      LOG(Info, "Refusing to start canceled ToolBasedJob: " << toString(uuid()));
      return;
    }
    */

    LOG(Info, "Starting ToolBasedJob: " << toString(uuid()));
    
    m_process_creator = t_pc;

    using namespace boost::filesystem;

    m_error_info = ErrorInfo(); // Reset error tracking for this run

    // reset parameters and files
    m_required_files.clear();
    resetFiles(m_trackedFiles);
    m_parameters.clear();
    m_expectedOutputFiles.clear();

    l.unlock();

    emitStarted();    


    try {
      startHandler();
      startNextTool();
    } catch (const std::exception &e) {
      handleToolStartError(e);
      return;
    }

    exec();

    endHandlerImpl();

    setErrors(m_error_info.errors());

    l.relock();

    for (const auto & process : m_processes)
    {
      process.second->stop();
      process.second->waitForFinished();
//      process.second->disconnect();
    }

    l.unlock();

//    while (openstudio::Application::instance().processEvents())
//    {
//    };

    l.relock();
    m_currentprocess->disconnect();
    m_currentprocess.reset();
    l.unlock();

  }

  void ToolBasedJob::handleToolStartError(const std::exception &e)
  {
    QWriteLocker l(&m_mutex);
    std::vector<std::pair<ErrorType, std::string> > err;
    err.push_back(std::make_pair(ErrorType::Error, e.what()));
    setErrors(JobErrors(ruleset::OSResultValue::Fail, err));
    LOG(Error, "ToolBasedJob error starting job: " << e.what());
  }


  void ToolBasedJob::remoteStarted(int t_remoteid, int t_remoteTaskId)
  {
    emit remoteProcessStarted(uuid(), t_remoteid, t_remoteTaskId);
  }

  void ToolBasedJob::remoteFinished(int t_remoteid, int t_remoteTaskId)
  {
    emit remoteProcessFinished(uuid(), t_remoteid, t_remoteTaskId);
  }

  void ToolBasedJob::requestStop()
  {
    QWriteLocker l(&m_stopMutex);
    m_stopRequested = true;
    if (m_currentprocess)
    {
      m_currentprocess->stop();
    }

    l.unlock();

    quit();
  }

  bool ToolBasedJob::stopRequested() const
  {
    QReadLocker l(&m_stopMutex);
    return m_stopRequested;
  }

  void ToolBasedJob::shutdownJob()
  {
    assert(QThread::currentThread() != this);
    requestStop();
    while (!wait(10))
    {
      //handle race condition if "exec" has not yet started
      requestStop();
    }
    // disconnect any remaining slots
    disconnect(this);
  }


  void ToolBasedJob::processOutputFileChanged(const openstudio::runmanager::FileInfo &f)
  {
    emitOutputFileChanged(f);
  }

  void ToolBasedJob::updateOutOfDateStatus(bool v) const 
  {
    QWriteLocker l(&m_mutex);
    bool outofdate = v;
    bool changed = (outofdate != m_last_out_of_date);

    if (changed)
    {
      l.unlock();
      m_last_out_of_date = outofdate;
      emitStateChanged();
    }
  }

  void ToolBasedJob::standardCleanImpl()
  {
    
    QReadLocker l(&m_mutex);

    for (const auto & process : m_processes)
    {
      process.second->cleanUpRequiredFiles();
    }  
    
  }

  void ToolBasedJob::copyRequiredFiles(const FileInfo &infile, const std::string &extin, const openstudio::path &filename)
  {
    QWriteLocker l(&m_mutex);
    m_copyRequiredFiles.push_back(std::make_tuple(infile, extin, filename));
  }

  Files ToolBasedJob::outputFilesImpl() const
  {
    QReadLocker l(&m_mutex);

    Files f = outputFilesHandlerImpl();

    for (const auto & process : m_processes)
    {
      std::vector<FileInfo> v = process.second->outputFiles();
      f.append(v.begin(), v.end());

      for (const auto copyRequiredFile : m_copyRequiredFiles)
      {
        std::vector<FileInfo> &fileinfos = f.files();

        for (auto & fi : fileinfos)
        {
          if (openstudio::toString(fi.fullPath.extension()) == "." + std::get<1>(copyRequiredFile))
          {

            // Copy the required files inherited from the input file to this found output file

            if (openstudio::toString(fi.fullPath.extension()) == ".osm")
            {
              //we manually handled some reqs already probably
              for (const auto & addedRequiredFile : m_addedRequiredFiles)
              {
                LOG(Debug, "Copying over required OSM file " << openstudio::toString(addedRequiredFile.first) << " to " << openstudio::toString(addedRequiredFile.second));

                try {
                  fi.addRequiredFile(addedRequiredFile.first, addedRequiredFile.second);
                } catch (const std::exception &e) {
                  LOG(Info, "Required file already existed, that's OK: " << e.what());
                }
              }
            }

            std::vector<std::pair<QUrl, openstudio::path> > requiredFiles 
              = std::get<0>(copyRequiredFile).requiredFiles;

            for (const auto & requiredFile : requiredFiles)
            {
              if (requiredFile.second == std::get<2>(copyRequiredFile) || std::get<2>(copyRequiredFile).empty())
              {
                fi.requiredFiles.push_back(requiredFile);
              }
            }
          }

        } 
      }
    }


    // sort all of the expected files to the end of the list 
    // so that they show up with higher priority in lists
    // (in the "findLast" sense)
    std::vector<FileInfo> &files = f.files();
    auto enditr = files.rbegin();
    for (auto itr = files.begin();
         itr != files.end();
         ++itr)
    {
      if (m_expectedOutputFiles.count(openstudio::toPath(itr->filename)) != 0)
      {
        std::swap(*itr, *enditr);
        ++enditr;
      }
    }

    return f;
  }

  std::string ToolBasedJob::getOutput() const {
    std::string result;
    QWriteLocker l(&m_mutex);
    std::ifstream ifs(toString(outdir() / toPath("stdout")).c_str(), std::ios_base::in | std::ios::binary);
    if (ifs) {
      ifs.seekg(0,std::ios::end);
      result.resize(ifs.tellg());
      ifs.seekg(0,std::ios::beg);
      ifs.read(&result[0],result.size());
      ifs.close();
    }
    l.unlock();
    return result;
  }

  void ToolBasedJob::processError(QProcess::ProcessError t_e, const std::string &t_desc)
  {
    Files outfiles = outputFiles();
    openstudio::path outpath = outdir();

    QWriteLocker l(&m_mutex);
    LOG(Error, "ToolBasedJob processError: " << toString(uuid()) << " " << t_e << " " << t_desc);
    m_error_info.processError(t_e, t_desc);
    processResultFiles(outpath, outfiles);
    JobErrors e = m_error_info.errors();
    l.unlock();
    setErrors(e);
    quit();
  }


  void ToolBasedJob::processError(QProcess::ProcessError t_e)
  {
    Files outfiles = outputFiles();
    openstudio::path outpath = outdir();

    QWriteLocker l(&m_mutex);
    LOG(Error, "ToolBasedJob processError: " << toString(uuid()) << " " << t_e);
    m_error_info.processError(t_e);

    processResultFiles(outpath, outfiles);
    JobErrors e = m_error_info.errors();
    l.unlock();
    setErrors(e);
    quit();
  }

  void ToolBasedJob::processResultFiles(const openstudio::path &t_outpath, const Files &t_outfiles)
  {
    // If an eplusout.err file was created, let's parse it
    openstudio::path errpath = t_outpath / toPath("eplusout.err");
    if (boost::filesystem::exists(errpath))
    {
      LOG(Debug, "Setting error file: " << openstudio::toString(errpath));
      m_error_info.errorFile(openstudio::energyplus::ErrorFile(errpath));
    }

    std::vector<FileInfo> resultpaths = t_outfiles.getAllByFilename("result.ossr").files();
    if (!resultpaths.empty())
    {
      std::vector<openstudio::ruleset::OSResult> results;
      for (const auto & resultPath : resultpaths)
      {
        openstudio::path p = resultPath.fullPath;
        std::string parent_path = openstudio::toString(p.parent_path().filename());
        size_t numpos = parent_path.find("mergedjob-");
        int num = 0;
        if (numpos == 0)
        {
          num = atoi(parent_path.substr(std::string("mergedjob-").size()).c_str());
        }


        if (results.size() < static_cast<size_t>(num+1))
        {
          results.resize(num+1);
        }

        LOG(Debug, "Attempting to load results for merged job: " << num);

        boost::optional<openstudio::ruleset::OSResult> osresult = openstudio::ruleset::OSResult::load(p);

        if (osresult)
        {
          results[num] = *osresult;
        } else {
          LOG(Error, "Error loading osresult file: " << openstudio::toString(p));
        }
      }

      LOG(Debug, "Setting osresult files: " << results.size());
      m_error_info.osResult(results);

    } else {
      LOG(Debug, "No osresult file found at: " << openstudio::toString(t_outpath));
    }
  }

  void ToolBasedJob::processFinished(int t_exitCode, QProcess::ExitStatus t_exitStatus)
  {
    Files outfiles = outputFiles();
    openstudio::path outpath = outdir();


    QWriteLocker l(&m_mutex);
    LOG(Debug, "ToolBasedJob processFinished: " << toString(uuid()) << " " << t_exitCode << " " << t_exitStatus); 


    m_error_info.exitCode(t_exitCode);
    m_error_info.exitStatus(t_exitStatus);

    processResultFiles(outpath, outfiles);

    JobErrors e = m_error_info.errors();
    if (outfiles.files().empty())
    {
      if (m_noOutputError)
      {
        LOG(Debug, "ToolBasedJob processFinished, no output files generated");
        e.allErrors.push_back(std::make_pair(ErrorType::Error, "No output files generated"));
        e.result = ruleset::OSResultValue::Fail;
      }
    }

    ruleset::OSResultValue result = e.result;
    LOG(Debug, "ToolBasedJob processFinished, success: " << result.valueName());

    if (result == ruleset::OSResultValue::Fail)
    {
      std::vector<std::pair<ErrorType, std::string> > errors = e.allErrors;
      for (const auto & error : errors)
      {
        LOG(Debug, "ToolBasedJob " << error.first.valueName() << ": " << error.second);
      }
    }

    l.unlock();

    setErrors(e);

    if (result != ruleset::OSResultValue::Fail)
    {
      try {
        startNextTool();
      } catch (const std::exception &e) {
        handleToolStartError(e);
      }
    } else {
      quit();
    }

  }

  void ToolBasedJob::processStandardErrDataAdded(const std::string &data)
  {
    QWriteLocker l(&m_mutex);
    std::ofstream ofs(toString(outdir() / toPath("stderr")).c_str(), std::ios_base::out | std::ios_base::app);
    ofs << data;
    l.unlock();
  }

  void ToolBasedJob::processStandardOutDataAdded(const std::string &data) {
    LOG(Trace, "stdout: " << boost::trim_copy(data));

    QWriteLocker l(&m_mutex);
    std::ofstream ofs(toString(outdir() / toPath("stdout")).c_str(), std::ios_base::out | std::ios_base::app);
    ofs << data;
    l.unlock();

    emitOutputDataAdded(data);
  }

  void ToolBasedJob::processStatusChanged(const AdvancedStatus &t_stat)
  {
    emitStatusChanged(t_stat);
  }

  void ToolBasedJob::processStarted()
  {
    LOG(Debug, "ToolBasedJob processStarted: " << toString(uuid()) );
  }

  std::string ToolBasedJob::description() const
  {
    QMutexLocker l(&m_impl_mutex);
    return descriptionImpl();
  }

  std::string ToolBasedJob::detailedDescription() const
  {
    QMutexLocker l(&m_impl_mutex);
    return detailedDescriptionImpl();
  }


  ToolVersion ToolBasedJob::getToolVersion(const std::string &t_toolName) const
  {
    QMutexLocker l(&m_impl_mutex);
    return getToolVersionImpl(t_toolName);
  }

  void ToolBasedJob::startHandler()
  {
    QMutexLocker l(&m_impl_mutex);
    startHandlerImpl();
  }



} // detail
} // runmanager
} // openstudio



