/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/time/DateTime.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/PathHelpers.hpp>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>

#ifdef Q_WS_WIN
#include <Windows.h>
#else
#include <signal.h>
#endif


namespace openstudio {
namespace runmanager {
namespace detail {

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
      m_stdin(t_stdin)
  {
    LOG(Info, "Creating LocalProcess");

    using namespace boost::filesystem;

    QFileInfo qfi(openstudio::toQString(t_tool.localBinPath));
    if (!qfi.isFile() || !qfi.isExecutable())
    {
      throw std::runtime_error("Unable to find valid executable while creating local process: " + toString(t_tool.localBinPath.external_file_string()));
    }

    for (std::vector<std::pair<openstudio::path, openstudio::path> >::const_iterator itr = m_requiredFiles.begin();
         itr != m_requiredFiles.end();
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
          frompath = m_tool.localBinPath.parent_path() / frompath;
        }
      }

      if (exists(frompath) && !is_directory(frompath))
      {
        create_directories(itr->second.parent_path());
        if (frompath != itr->second) {
          remove(itr->second);
          LOG(Debug, "Copying required file from " << openstudio::toString(frompath) << " to " << openstudio::toString(itr->second));
          copy_file(frompath, itr->second, copy_option::overwrite_if_exists);
          m_copiedRequiredFiles.insert(itr->second);
        }
      } else if (exists(frompath) && is_directory(frompath)) {
        typedef boost::filesystem::basic_directory_iterator<openstudio::path> diritr;

        diritr begin(frompath);
        diritr end;

        create_directories(itr->second);
        m_copiedRequiredFiles.insert(itr->second);

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
              m_copiedRequiredFiles.insert(f);
            }
          }
          ++begin;
        }

      } else {
        throw std::runtime_error("Unable to find required file while creating LocalProcess: " + toString(itr->first) + ": " + toString(itr->second) + " basepath: " + toString(t_basePath));
      }
    }

    connect(&m_process, SIGNAL(error(QProcess::ProcessError)), 
        this, SLOT(processError(QProcess::ProcessError)));
    connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), 
        this, SLOT(processFinished(int, QProcess::ExitStatus)));

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

  void LocalProcess::directoryChanged()
  {
    directoryChanged(openstudio::toQString(m_outdir));
  }

  void LocalProcess::start()
  {
    directoryChanged(openstudio::toQString(m_outdir));
    m_fileCheckTimer.start(2000); // check for updated files every 2 seconds.
    connect(&m_fileCheckTimer, SIGNAL(timeout()), this, SLOT(directoryChanged()));
//    m_outfiles = dirFiles(openstudio::toQString(m_outdir));

//    m_fswatcher = boost::shared_ptr<QFileSystemWatcher>(new QFileSystemWatcher());

//    connect(m_fswatcher.get(), SIGNAL(directoryChanged(const QString &)),
//         this, SLOT(directoryChanged(const QString &)));
//    connect(m_fswatcher.get(), SIGNAL(fileChanged(const QString &)),
//         this, SLOT(fileChanged(const QString &)));

//    m_fswatcher->addPath(openstudio::toQString(m_outdir));

 //   monitorFiles(m_outfiles.begin(), m_outfiles.end());

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
#ifdef Q_WS_WIN
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
#ifdef Q_WS_WIN
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
    for (size_t i = 0; i < t_files.size(); ++i)
    {
      QString path = toQString(m_outdir / toPath(t_files[i]));
      QFile::remove(path);
      fileChanged(path);
    }
  }

  std::vector<FileInfo> LocalProcess::outputFiles() const
  {

    return std::vector<FileInfo>(m_outfiles.begin(), m_outfiles.end());
  }

  std::vector<FileInfo> LocalProcess::inputFiles() const
  {
    std::vector<FileInfo> ret;
    
    for (std::vector<std::pair<openstudio::path, openstudio::path> >::const_iterator itr = m_requiredFiles.begin();
         itr != m_requiredFiles.end();
         ++itr)
    {
      ret.push_back(RunManager_Util::dirFile(QFileInfo(toQString(itr->second))));
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
    
    std::set_symmetric_difference(fs.begin(), fs.end(), 
                        m_outfiles.begin(), m_outfiles.end(),
                        std::back_inserter(diff));
 
    m_outfiles = fs;

//    monitorFiles(diff.begin(), diff.end());
   
    std::for_each(diff.begin(), diff.end(), boost::bind(&LocalProcess::emitUpdatedFileInfo, this, _1));
  }


  LocalProcess::FileSet LocalProcess::dirFiles(const QString &dir) const
  {
    QDir d(dir, "", QDir::Name, QDir::Files);
    QFileInfoList fil = d.entryInfoList();

    QFileInfoList filtered;

    // Filter out all files that are part of the set of input files. Everything remaining should be an outputfile
    for (QFileInfoList::const_iterator itr = fil.begin();
         itr != fil.end();
         ++itr)
    {
      bool partofinput = false;
      for (std::vector<std::pair<openstudio::path, openstudio::path> >::const_iterator itr2 = m_requiredFiles.begin();
          itr2 != m_requiredFiles.end();
          ++itr2)
      {
        QString fileName = itr->fileName();
        QString fileName2 = toQString(itr2->second.filename());
        if (fileName == fileName2)
        {
          partofinput = true;
          break;
        }
      }

      if (!partofinput)
      {
        filtered.push_back(*itr);
      }
    }

    FileSet out;

    typedef FileInfo (*filetransform)(QFileInfo);

    try{
      std::transform(filtered.begin(), filtered.end(), std::inserter(out, out.end()), 
          static_cast<filetransform>(&RunManager_Util::dirFile));
    } catch(openstudio::Exception e) {
      LOG_AND_THROW("Exception caught " << e.what());
    }

    return out;
  }

  void LocalProcess::cleanUpRequiredFiles()
  {
    for (std::set<openstudio::path>::const_iterator itr = m_copiedRequiredFiles.begin();
         itr != m_copiedRequiredFiles.end();
         ++itr)
    {
      LOG(Debug, "cleanUpRequiredFiles: " << openstudio::toString(*itr));
	    try {
        boost::filesystem::remove(*itr);
	    } catch (const std::exception &e) {
        LOG(Trace, "Unable to remove file: " << e.what());
		    // no error if it doesn't manage to delete it
	    }

	    try {
        openstudio::path p = itr->parent_path();

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



    if (m_process.state() != QProcess::Running
        && t_e == QProcess::WriteError)
    {
      LOG(Info, "WriteError occured when process was not running, ignoring it");
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

    m_fswatcher.reset();

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



