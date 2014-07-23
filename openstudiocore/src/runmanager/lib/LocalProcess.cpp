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

#include "LocalProcess.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"

#include <QDir>
#include <QDateTime>
#include <QMutexLocker>

#ifdef Q_OS_WIN
#include <Windows.h>
#else
#include <signal.h>
#include <sys/wait.h>
#endif


namespace openstudio {
namespace runmanager {
namespace detail {
  MyQProcess::MyQProcess()
    : m_exitedCount(0)
  {
  }

  void MyQProcess::checkProcessStatus()
  {
#ifndef Q_OS_WIN
    // Check for process status if the exit was not handled correctly. 

    Q_PID qpid = pid();

    if (qpid != 0 && atEnd() && state() != QProcess::Starting)
    {
      pid_t result = waitpid(qpid, nullptr, WNOHANG);
      if (result != 0 && m_exitedCount != -1)
      {
        LOG(Debug, "PROCESS EXITED");
        ++m_exitedCount;
        setProcessState(QProcess::NotRunning);
        if (m_exitedCount > 2)
        {
          m_exitedCount = -1;
          emit zombied(error());
        }
      }
    }

#endif
  }

  LocalProcess::LocalProcess(const openstudio::runmanager::ToolInfo &t_tool,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath)
    : m_tool(t_tool), m_requiredFiles(t_requiredFiles),
      m_parameters(t_parameters), m_outdir(t_outdir),
      m_expectedOutputFiles(t_expectedOutputFiles),
      m_stdin(t_stdin),
      m_copiedRequiredFiles(copyRequiredFiles(t_tool, t_requiredFiles, t_basePath))
  {
    LOG(Info, "Creating LocalProcess");


    QFileInfo qfi(openstudio::toQString(t_tool.localBinPath));
    if (!qfi.isFile() || !qfi.isExecutable())
    {
      throw std::runtime_error("Unable to find valid executable while creating local process: " + toString(t_tool.localBinPath.native()));
    }


    connect(&m_process, SIGNAL(error(QProcess::ProcessError)), 
        this, SLOT(processError(QProcess::ProcessError)));
    connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), 
        this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&m_process, SIGNAL(zombied(QProcess::ProcessError)),
        this, SLOT(processZombied(QProcess::ProcessError)));

    connect(&m_process, SIGNAL(readyReadStandardError()), 
         this, SLOT(processReadyReadStandardError()));
 

    connect(&m_process, SIGNAL(readyReadStandardOutput()), 
         this, SLOT(processReadyReadStandardOutput()));
    connect(&m_process, SIGNAL(started()), 
         this, SLOT(processStarted()));

    /*
    connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), 
         this, SLOT(processStateChanged(QProcess::ProcessState)));
*/

    LOG(Debug, "Setting working directory: " << toString(m_outdir));
    m_process.setWorkingDirectory(openstudio::toQString(m_outdir));


    directoryChanged(openstudio::toQString(m_outdir));
  }

  std::set<openstudio::path> LocalProcess::copyRequiredFiles(const ToolInfo &t_tool, const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles, 
      const openstudio::path &t_basePath)
  {
    using namespace boost::filesystem;
    std::set<openstudio::path> retval;

    for (std::vector<std::pair<openstudio::path, openstudio::path> >::const_iterator itr = t_requiredFiles.begin();
         itr != t_requiredFiles.end();
         ++itr)
    {
      openstudio::path frompath = itr->first;
      if (!frompath.has_root_path())
      {
        // we are dealing with a relative dir, relative dirs are
        // first evaluated with respect to the basePath, and if they are not found
        // there, then with respect to the tool path
        openstudio::path baserelative = t_basePath / frompath;

        if (boost::filesystem::exists(baserelative))
        {
          frompath = baserelative;
        } else {
          frompath = t_tool.localBinPath.parent_path() / frompath;
        }
      }

      if (exists(frompath) && !is_directory(frompath))
      {
        create_directories(itr->second.parent_path());
        if (frompath != itr->second) {
          remove(itr->second);
          LOG(Debug, "Copying required file from " << openstudio::toString(frompath) << " to " << openstudio::toString(itr->second));
          copy_file(frompath, itr->second, copy_option::overwrite_if_exists);
          retval.insert(itr->second);
        }
      } else if (exists(frompath) && is_directory(frompath)) {
        typedef boost::filesystem::directory_iterator diritr;

        diritr begin(frompath);
        diritr end;

        create_directories(itr->second);
        retval.insert(itr->second);

        while (begin != end)
        {
          if (!is_directory(*begin))
          {
            openstudio::path p = *begin;
            openstudio::path::const_iterator fileitr = p.end();
            if (fileitr != p.begin())
            {
              --fileitr;
              openstudio::path f = itr->second / *fileitr;
              remove(f);
              LOG(Debug, "Copying required file from " << openstudio::toString(*begin) << " to " << openstudio::toString(f));

              copy_file(openstudio::path(*begin), f, copy_option::overwrite_if_exists);
              retval.insert(f);
            }
          }
          ++begin;
        }

      } else {
        throw std::runtime_error("Unable to find required file while creating LocalProcess: " + toString(itr->first) + ": " + toString(itr->second) + " basepath: " + toString(t_basePath));
      }
    }

    return retval;
  }


  void LocalProcess::directoryChanged()
  {
    directoryChanged(openstudio::toQString(m_outdir));
  }

  void LocalProcess::start()
  {
    directoryChanged(openstudio::toQString(m_outdir));

    m_fileCheckTimer.start(2000); // check for updated files every 2 seconds.
    connect(&m_fileCheckTimer, SIGNAL(timeout()), this, SLOT(directoryChanged()));


    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    LOG(Error, "Starting LocalProcess: " << openstudio::toString(m_tool.localBinPath));

    QStringList list;
    for (std::vector<std::string>::const_iterator itr = m_parameters.begin();
         itr != m_parameters.end();
         ++itr)
    {
      LOG(Debug, "Adding param: " << *itr);
      list.push_back(toQString(*itr));
    }

    // set up path to be binary directory to catch ancillary tools
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
#ifdef Q_OS_WIN
    env.insert("PATH", openstudio::toQString(m_tool.localBinPath.parent_path()) + ";" + env.value("PATH"));
#else
    env.insert("PATH", openstudio::toQString(m_tool.localBinPath.parent_path()) + ":" + env.value("PATH"));
#endif

    m_process.setProcessEnvironment(env);

   
    m_process.start(openstudio::toQString(m_tool.localBinPath), list, QIODevice::ReadWrite);
  }

  LocalProcess::~LocalProcess()
  {
    m_process.disconnect();
    kill(m_process, true);
    m_process.waitForFinished();
  }


  void LocalProcess::stopImpl()
  {
    kill(m_process, true);

//    if (!m_process.waitForFinished(100))
//    {
//      kill(m_process, true);
//    }
  }

  void LocalProcess::kill(QProcess &t_process, bool t_force)
  {
    if (t_process.state() == QProcess::Running)
    {
#ifdef Q_OS_WIN
      PROCESS_INFORMATION *pinfo = (PROCESS_INFORMATION*)t_process.pid();
      if (pinfo)
      {
        QString abort_cmd;
        LOG(Info, "Killing process tree using taskkill " << pinfo->dwProcessId << " force " << t_force);
        if (t_force)
        {
          abort_cmd = QString("taskkill /PID %1 /T /F").arg(pinfo->dwProcessId);
        } else {
          abort_cmd = QString("taskkill /PID %1 /T").arg(pinfo->dwProcessId);
        }
        QProcess::execute(abort_cmd);
      } 
#else
      pid_t pid(t_process.pid());

      if (pid > 0)
      {
        openstudio::path killtree = openstudio::getSharedResourcesPath()/openstudio::toPath("runmanager")/openstudio::toPath("killtree.sh");
        LOG(Info, "Killing process tree using killtree" << openstudio::toString(killtree) << " for pid " << pid << " force " << t_force);

        QStringList qsl;
        qsl.push_back(QString::number(pid));
        if (t_force)
        {
          qsl.push_back(QString::number(SIGKILL));
        } else {
          qsl.push_back(QString::number(SIGTERM));
        }
        QProcess::execute(openstudio::toQString(killtree), qsl);
      }
#endif
    }

    if (t_force)
    {
      t_process.kill();
    } else {
      t_process.terminate();
    }

  }

  void LocalProcess::cleanup(const std::vector<std::string> &t_files)
  {
    for (const auto & file : t_files)
    {
      QString path = toQString(m_outdir / toPath(file));
      QFile::remove(path);
      fileChanged(path);
    }
  }

  std::vector<FileInfo> LocalProcess::outputFiles() const
  {
    QMutexLocker l(&m_mutex);
    return std::vector<FileInfo>(m_outfiles.begin(), m_outfiles.end());
  }

  std::vector<FileInfo> LocalProcess::inputFiles() const
  {
    std::vector<FileInfo> ret;
    
    for (const auto & requiredFile : m_requiredFiles)
    {
      ret.push_back(RunManager_Util::dirFile(QFileInfo(toQString(requiredFile.second))));
    }

    return ret;
  }


  void LocalProcess::waitForFinished()
  {
    m_process.waitForFinished(-1);
  }

  void LocalProcess::fileChanged(const QString &str)
  {
    emitOutputFileChanged(RunManager_Util::dirFile(QFileInfo(str)));
  }

  void LocalProcess::emitUpdatedFileInfo(const FileInfo &fi)
  {
    emitOutputFileChanged(RunManager_Util::dirFile(QFileInfo(toQString(fi.fullPath))));
  }

  void LocalProcess::directoryChanged(const QString &str)
  {
    FileSet fs = dirFiles(openstudio::toQString(m_outdir));

    std::vector<FileSet::value_type> diff;

    {
      QMutexLocker l(&m_mutex);
      std::set_symmetric_difference(fs.begin(), fs.end(), 
          m_outfiles.begin(), m_outfiles.end(),
          std::back_inserter(diff));

      m_outfiles = fs;
    }

    std::for_each(diff.begin(), diff.end(), std::bind(&LocalProcess::emitUpdatedFileInfo, this, std::placeholders::_1));

    m_process.checkProcessStatus();
  }


  LocalProcess::FileSet LocalProcess::dirFiles(const QString &dir) const
  {
    QFileInfoList fil;

    
    QDir subdirs(dir, "mergedjob-*", QDir::Name, QDir::Dirs);
    QFileInfoList mergedjobdirs = subdirs.entryInfoList();

    for (QFileInfoList::const_iterator itr = mergedjobdirs.begin();
         itr != mergedjobdirs.end();
         ++itr)
    {

      QDir mergeddir(itr->absoluteFilePath(), "", QDir::Name, QDir::Files);
      fil.append(mergeddir.entryInfoList());
    }
  

    QDir d(dir, "", QDir::Name, QDir::Files);
    fil.append(d.entryInfoList());

    QFileInfoList filtered;

    // Filter out all files that are part of the set of input files. Everything remaining should be an outputfile
    for (const auto & fileInfo : fil)
    {
      bool partofinput = false;
      for (const auto & requiredFile : m_requiredFiles)
      {
        QString fileName = fileInfo.fileName();
        QString fileName2 = toQString(requiredFile.second.filename());
        if (fileName == fileName2)
        {
          partofinput = true;
          break;
        }
      }

      if (!partofinput)
      {
        filtered.push_back(fileInfo);
      }
    }

    FileSet out;

    typedef FileInfo (*filetransform)(QFileInfo);

    try{
      std::transform(filtered.begin(), filtered.end(), std::inserter(out, out.end()), 
          static_cast<filetransform>(&RunManager_Util::dirFile));
    } catch(openstudio::Exception& e) {
      LOG_AND_THROW("Exception caught " << e.what());
    }

    return out;
  }

  void LocalProcess::cleanUpRequiredFiles()
  {
    for (const auto & copiedRequiredFile : m_copiedRequiredFiles)
    {
      LOG(Debug, "cleanUpRequiredFiles: " << openstudio::toString(copiedRequiredFile));
      try {
        boost::filesystem::remove(copiedRequiredFile);
      } catch (const std::exception &e) {
        LOG(Trace, "Unable to remove file: " << e.what());
        // no error if it doesn't manage to delete it
      }

      try {
        openstudio::path p = copiedRequiredFile.parent_path();

        while (!p.empty() && !relativePath(p, m_outdir).empty() && m_outdir != p)
        {
          // remove the directory if it happens to be empty. 
          boost::filesystem::remove(p);
          p = p.parent_path();
        }
      } catch (const std::exception &e) {
        LOG(Trace, "Unable to remove directory: " << e.what());
        // no error if it doesn't manage to delete it
      }
    }
  }

  void LocalProcess::processZombied(QProcess::ProcessError /*t_e*/)
  {
    m_fileCheckTimer.stop();

    LOG(Info, "Process appears to be zombied"); 

    // but this isn't necessarily an error because it's probably due to miscaught signals.
    //emit error(t_e);

    QCoreApplication::processEvents();
    directoryChanged(openstudio::toQString(m_outdir));

    emit finished(0, QProcess::NormalExit);
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Idle));

  }

  void LocalProcess::processError(QProcess::ProcessError t_e)
  {
    m_fileCheckTimer.stop();
    QFileInfo qfi(toQString(m_tool.localBinPath));
    QFileInfo outdirfi(toQString(m_outdir));
    LOG(Error, "LocalProcess processError: " << t_e 
        << " exe: " << toString(m_tool.localBinPath)
        << " workingdir: " << toString(m_outdir)
        << " fileexists: " << qfi.isFile()
        << " fileexecutable: " << qfi.isExecutable()
        << " ErrorValue: " << t_e
        << " outdirexists: " << outdirfi.isFile()
        << " outdirisdirectory: " << outdirfi.isDir());

    QCoreApplication::processEvents();
    directoryChanged(openstudio::toQString(m_outdir));

    QProcess::ProcessState state = m_process.state();

    if (state != QProcess::Running
        && t_e == QProcess::WriteError)
    {
      LOG(Info, "WriteError occurred when process was not running, ignoring it");
    } else {
      //directoryChanged(openstudio::toQString(m_outdir));
      emit error(t_e);
    }

 }

  void LocalProcess::handleOutput(const QByteArray &qba, bool stderror)
  {
    // Store in the raw output variable
    std::string rawoutput(qba.constData(), qba.size());

    if (stderror)
    {
      emit standardErrDataAdded(rawoutput);
    } else {
      emit standardOutDataAdded(rawoutput);
    }
  }

  void LocalProcess::processFinished(int t_exitCode, QProcess::ExitStatus t_exitStatus)
  {
    m_fileCheckTimer.stop();

    directoryChanged(openstudio::toQString(m_outdir));
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Finishing));

    if (!stopped())
    {
      handleOutput(m_process.readAllStandardOutput(), false);
      handleOutput(m_process.readAllStandardError(), true);
    }

    QCoreApplication::processEvents();
    directoryChanged(openstudio::toQString(m_outdir));

    emit finished(t_exitCode, t_exitStatus);
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Idle));
  }

  void LocalProcess::processReadyReadStandardError()
  {
    directoryChanged(openstudio::toQString(m_outdir));

    if (!stopped())
    {
      handleOutput(m_process.readAllStandardError(), true);
    }
  }

  void LocalProcess::processReadyReadStandardOutput()
  {
    directoryChanged(openstudio::toQString(m_outdir));
    if (!stopped())
    {
      handleOutput(m_process.readAllStandardOutput(), false);
    }
  }

  void LocalProcess::processStarted()
  {
    // Send stdin input meant for process
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));
    emit started();
    directoryChanged(openstudio::toQString(m_outdir));

    // If there is stdin to write and the process has not already finished by the time we process
    // this signal...
    if (!m_stdin.empty() && m_process.state() != QProcess::Running)
    {
      // write the stdin
      m_process.write(m_stdin.c_str(), m_stdin.size());
    }
  }

  bool LocalProcess::running() const
  {
    return m_process.state() == QProcess::Running
      || m_process.state() == QProcess::Starting;
  }


} // detail
} // runmanager
} // openstudio



