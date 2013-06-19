#include "SLURMManager.hpp"
#include "SLURMProcess.hpp"
#include "RunManager_Util.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <QCryptographicHash>
#include <QMessageBox>
#include <boost/filesystem.hpp>

namespace openstudio {
namespace runmanager {

  namespace detail {
    bool SSHFileChecksums::fileMatch(const openstudio::path &t_local, const openstudio::path &t_remote) const
    {
      std::map<openstudio::path, QByteArray>::const_iterator local = m_locals.find(t_local);
      std::map<openstudio::path, QByteArray>::const_iterator remote = m_remotes.find(t_remote);

      return (local != m_locals.end() && remote != m_remotes.end()
              && (local->second) == (remote->second));
    }

    void SSHFileChecksums::setRemoteChecksum(const openstudio::path &t_remote, const QByteArray &t_checksum)
    {
      m_remotes[t_remote] = t_checksum;
    }

    void SSHFileChecksums::updateLocalChecksum(const openstudio::path &t_path)
    {
      QFile f(toQString(t_path));

      if (f.exists())
      {
        f.open(QIODevice::ReadOnly);

        QCryptographicHash hash(QCryptographicHash::Sha1);

        while (f.isOpen() && f.isReadable() && !f.atEnd())
        {
          QByteArray buf = f.read(10000);
          hash.addData(buf);
        }

        m_locals[t_path] = hash.result();
      }
    }

    SSHQueueItem::SSHQueueItem(int id)
      : m_id(id), m_running(false)
    {
    }

    int SSHQueueItem::id() const
    {
      return m_id;
    }

    bool SSHQueueItem::apply(const boost::shared_ptr<SSHConnection> &t_conn)
    {
      m_running = true;
      return apply_impl(t_conn);
    }

    void SSHQueueItem::reset()
    {
      m_running = false;
    }

    bool SSHQueueItem::running() const
    {
      return m_running;
    }


    /// SSHQueueItem implementation for executing a local command
    SSHLocalCommand::SSHLocalCommand(int t_id, boost::function<void ()> t_func)
      : SSHQueueItem(t_id), m_func(t_func)
    { }

    void SSHLocalCommand::stdoutReceived(const std::string &)
    { }

    void SSHLocalCommand::finished(bool timedout)
    { }

    bool SSHLocalCommand::apply_impl(const boost::shared_ptr<SSHConnection> &)
    {
      // Call the registered function
      m_func();
      return false;
    }


    SSHPutFile::SSHPutFile(int t_id, const openstudio::path &t_from, const openstudio::path &t_to,
      const boost::function<bool (const openstudio::path &, const openstudio::path &)> &t_check)
      : SSHQueueItem(t_id), m_from(t_from), m_to(t_to), m_check(t_check)
    {
    }

    bool SSHPutFile::apply_impl(const boost::shared_ptr<SSHConnection> &t_conn)
    {
      try {

        if ( !m_check || (m_check && !m_check(m_from, m_to)) )
        {
          t_conn->putFile(id(), m_from, m_to, 1000 * 60 * 10); // 10 minute timeout wait
          return true;
        } else {
          return false;
        }

      } catch (const std::exception &e) {
        throw std::runtime_error("Error applying SSHPutFile from: " + toString(m_from)
          + " to: " + toString(m_to) + " desc: " + e.what());
      }
    }

    void SSHPutFile::finished(bool)
    {
      //nothing to do yet
    }


    SSHGetFile::SSHGetFile(int t_id, const openstudio::path &t_from, const openstudio::path &t_to,
      const boost::function<bool (const openstudio::path &, const openstudio::path &)> &t_check)
      : SSHQueueItem(t_id), m_from(t_from), m_to(t_to), m_check(t_check)
    {
    }

    bool SSHGetFile::apply_impl(const boost::shared_ptr<SSHConnection> &t_conn)
    {
      if ( !m_check || (m_check && !m_check(m_from, m_to)) )
      {
        //Create directory for receiving file
        boost::filesystem::create_directories(m_to.parent_path());
        t_conn->getFile(id(), m_from, m_to, 1000 * 60 * 10); // 10 minute timeout wait
        return true;
      } else {
        return false;
        // File does not need to be got
      }
    }

    void SSHGetFile::finished(bool)
    {
      //nothing to do yet
    }

    SSHCommand::SSHCommand(int t_id, const std::string &cmd,
      const boost::function<void (const std::string &)> &t_handler)
      : SSHQueueItem(t_id), m_command(cmd), m_datahandler(t_handler)
    {
    }

    bool SSHCommand::apply_impl(const boost::shared_ptr<SSHConnection> &t_conn)
    {
      t_conn->execCommand(id(), m_command, 1000 * 60 * 10); // 10 minute timeout wait
      return true;
    }

    void SSHCommand::stdoutReceived(const std::string &t_data)
    {
      m_data += t_data;
    }

    void SSHCommand::finished(bool timedout)
    {
      if (m_datahandler)
      {
        m_datahandler(m_data);
      }
    }
  }

  openstudio::path SLURMManager::simplifyPath(const openstudio::path &t_path)
  {
    return openstudio::toPath(boost::algorithm::replace_all_regex_copy(toString(t_path.string()), boost::regex("[^a-zA-Z0-9/+.]"), std::string("_")));
  }


  SLURMManager::ProcessInfo::ProcessInfo(
      const openstudio::runmanager::ToolInfo &t_tool,
      const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
      const std::vector<std::string> &t_parameters,
      const openstudio::path &t_outdir,
      const std::vector<openstudio::path> &t_expectedOutputFiles,
      const std::string &t_stdin)
    : m_tool(t_tool),
      m_requiredFiles(t_requiredFiles), m_parameters(t_parameters),
      m_outdir(t_outdir), m_expectedOutputFiles(t_expectedOutputFiles), m_stdin(t_stdin),
      m_batchFile(SLURMManager::simplifyPath(m_outdir.relative_path()) / toPath("runjob.sh")),
      m_SLURMId(0), m_taskNumber(0), m_lastLineRead(0), m_finalFileListRequested(false),
      m_needsBatching(false)
  {
  }


  SLURMManager::SLURMManager()
    : m_queue_id(0), m_status_query_id(0), m_connectErrorCount(0)
  {
    QObject::connect(&m_timer, SIGNAL(timeout()),
      this, SLOT(sendStatusQuery()), Qt::QueuedConnection);
  }

  bool SLURMManager::hasConnection()
  {
    try {
      QMutexLocker l(&m_mutex);
      return checkConnection(false);
    } catch (...) {
      return false;
    }
  }

  void SLURMManager::activate()
  {
    QMutexLocker l(&m_mutex);

    if (m_timer.isActive() || m_connection)
    {
      return;
    }

    checkConnection(true);
  }

  void SLURMManager::sendStatusQuery()
  {
    static const int statustimeout = 2;

    if (m_mutex.tryLock(1))
    {
      pumpQueue(); // See if the queue needs to be pumped

      if (m_processes.empty() || (m_lastStatusQuery
          && !((boost::posix_time::microsec_clock::universal_time() - *m_lastStatusQuery).total_seconds() > statustimeout + 1)
         ))
      {
        m_mutex.unlock();
        return; // it hasn't been long enough to update the status of the jobs
      }

      m_lastStatusQuery = boost::posix_time::microsec_clock::universal_time();

      try {
        switch (m_status_query_id)
        {
          case 0:
            checkConnection()->execOOBCommand(0, "squeue", statustimeout * 1000 /* seconds */  );
            ++m_status_query_id;
            break;
          case 1:
            {
              std::stringstream cmd;
              int processcount = 0;
              std::set<int> slurmidsqueried;
              for (std::map<openstudio::UUID, ProcessData>::const_iterator itr = m_processes.begin();
                  itr != m_processes.end() && processcount < 5;
                  ++itr)
              {
                int slurmid = itr->second.second.m_SLURMId;

                if (slurmid > 0
                    && slurmidsqueried.count(slurmid) == 0
                    && (itr->second.first->running()
                      || itr->second.first->needsFinalStdOut())
                   )
                {
                  cmd << "cat -n slurm-" << slurmid
                    << ".out | sed 's/.*/" << slurmid << "\\t&/'; ";

                  if (!itr->second.first->running()
                      && itr->second.first->needsFinalStdOut())
                  {
                    itr->second.first->setNeedsFinalStdOut(false);
                  }

                  // make sure each id is only queried once
                  slurmidsqueried.insert(slurmid);
                  ++processcount; // we want to limit the number of processes we query for at once to limit the command size and data returned.
                }
              }
              std::string cmdstring = cmd.str();
              checkConnection()->execOOBCommand(1, cmdstring, statustimeout * 1000 /* seconds */  );
            }
            m_status_query_id = 0;
            break;
          default:
            m_status_query_id = 0;
        };
      } catch (const std::runtime_error &) {
        // There was an error queuing up a command, best to reset the connection
        // and reconnect on the next try
        m_connection.reset();
        if (!m_queue.empty())
        {
          m_queue.front()->reset();
        }

      }

      m_mutex.unlock();
    }
  }

  void SLURMManager::setConfiguration(const SSHCredentials &t_creds, const SLURMConfigOptions &t_config)
  {
    QMutexLocker l(&m_mutex);
    bool credschanged = false;
    if (m_ssh_creds != t_creds
        && m_staged_ssh_creds != t_creds)
    {
      credschanged = true;
    }
    m_staged_ssh_creds = t_creds;
    m_slurm_config_options = t_config;

    if (m_connection || m_timer.isActive())
    {
      // Only warn if the creds have actually changed
      if (credschanged)
      {
        // unlock to avoid deadlock
        l.unlock();
        QMessageBox::warning(0, "Active SLURM Connection", "There is currently an active SLURM connection, changes to the SSH SLURM connection will not apply until the application restarts");
      }
      // but quit regardless, we don't want to update the credentials if running
      return;
    }

    m_ssh_creds = t_creds;
  }

  SLURMManager::ProcessData &SLURMManager::getProcessData(int slurmid, int tasknumber)
  {
    for (std::map<openstudio::UUID, ProcessData>::iterator itr = m_processes.begin();
         itr != m_processes.end();
         ++itr)
    {
      if (itr->second.second.m_SLURMId == slurmid
          && itr->second.second.m_taskNumber == tasknumber)
      {
        return itr->second;
      }
    }

    throw std::out_of_range("No process with slurmid: " +
      boost::lexical_cast<std::string>(slurmid) + " known");
  }

  const SLURMManager::ProcessData &SLURMManager::getProcessData(int slurmid, int tasknumber) const
  {
    for (std::map<openstudio::UUID, ProcessData>::const_iterator itr = m_processes.begin();
         itr != m_processes.end();
         ++itr)
    {
      if (itr->second.second.m_SLURMId == slurmid
          && itr->second.second.m_taskNumber == tasknumber)
      {
        return itr->second;
      }
    }

    throw std::out_of_range("No process with slurmid: " +
      boost::lexical_cast<std::string>(slurmid) + " known");
  }

  SLURMManager::ProcessData &SLURMManager::getProcessData(const openstudio::UUID &t_uuid)
  {
    std::map<openstudio::UUID, ProcessData>::iterator itr = m_processes.find(t_uuid);
    if (itr != m_processes.end())
    {
      return itr->second;
    } else {
      throw std::out_of_range("No process by uuid: " + toString(t_uuid) + " known");
    }
  }

  const SLURMManager::ProcessData &SLURMManager::getProcessData(const openstudio::UUID &t_uuid) const
  {
    std::map<openstudio::UUID, ProcessData>::const_iterator itr = m_processes.find(t_uuid);
    if (itr != m_processes.end())
    {
      return itr->second;
    } else {
      throw std::out_of_range("No process by uuid: " + toString(t_uuid) + " known");
    }
  }

  boost::shared_ptr<SSHConnection> SLURMManager::checkConnection(bool t_allow_ui)
  {
    if (m_connectErrorCount > 0)
    {
      throw std::runtime_error("Previous connection attempts failed, not reattempting");
    }

    if (!m_connection)
    {
      try {
        m_connection = boost::shared_ptr<SSHConnection>(new SSHConnection(m_ssh_creds, t_allow_ui));
      } catch (const std::exception &e) {
        ++m_connectErrorCount;
        if (t_allow_ui)
        {
          QMessageBox::warning(0, "Error Making SSH Connection", e.what());
        }
        throw;
      }

      m_ssh_creds = m_connection->getCredentials();

      QObject::connect(m_connection.get(), SIGNAL(execCommandStandardOut(int, const std::string &, const std::string &)),
        this, SLOT(sshCommandStandardOut(int, const std::string &, const std::string &)));
      QObject::connect(m_connection.get(), SIGNAL(execCommandFinished(int, const std::string &, bool)),
        this, SLOT(sshCommandFinished(int, const std::string &, bool)));

      QObject::connect(m_connection.get(), SIGNAL(getFileFinished(int, const openstudio::path &, const openstudio::path &, bool)),
        this, SLOT(sshGetFileFinished(int, const openstudio::path &, const openstudio::path &, bool)));
      QObject::connect(m_connection.get(), SIGNAL(putFileFinished(int, const openstudio::path &, const openstudio::path &, bool)),
        this, SLOT(sshPutFileFinished(int, const openstudio::path &, const openstudio::path &, bool)));

      QObject::connect(m_connection.get(), SIGNAL(execOOBCommandStandardOut(int, const std::string &, const std::string &)),
        this, SLOT(sshOOBCommandStandardOut(int, const std::string &, const std::string &)));
      QObject::connect(m_connection.get(), SIGNAL(execOOBCommandFinished(int, const std::string &, bool)),
        this, SLOT(sshOOBCommandFinished(int, const std::string &, bool)));

    }

    return m_connection;
  }

  void SLURMManager::sshOOBCommandStandardOut(int, const std::string &cmd, const std::string &data)
  {
    m_outofbanddata += data;
  }

  void SLURMManager::handleRunningJobsData(const std::string &data)
  {
    std::set<int> runningjobs;

    std::vector<std::string> lines;
    boost::split(lines, data, boost::is_any_of("\r\n"), boost::algorithm::token_compress_on);
    /* whitespace (ws), jobid, ws, partition, ws, name, ws, user, ws, st, ws, time, ws, nodes, ws, nodelist, ws */
    boost::regex reg("\\s*([0-9]+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+([0-9]+)\\s+(\\S+)\\s*");

    for (std::vector<std::string>::const_iterator itr = lines.begin();
      itr != lines.end();
      ++itr)
    {
      boost::smatch results;
      if (boost::regex_match(*itr, results, reg))
      {
        int slurmid = atoi(results[1].str().c_str());
        // `std::string usernam = results[4].str();
        // int numnodes = atoi(results[7].str().c_str());

        runningjobs.insert(slurmid);
      }
    }

    for(std::map<openstudio::UUID, ProcessData>::iterator itr = m_processes.begin();
      itr != m_processes.end();
      ++itr)
    {
      // Only if it's running. If it's not running, then we've already notified
      // that it has ended
      if(itr->second.second.m_SLURMId != 0
         && runningjobs.count(itr->second.second.m_SLURMId) == 0
         && itr->second.first->running())
      {
        if (!itr->second.second.m_finalFileListRequested)
        {
          itr->second.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::Finishing));
          // The process was not found in the queue, but a slurmid is set... so it must be completed?
          queueGetFinalFileList(itr->second);
          itr->second.second.m_finalFileListRequested = true;
        }
      }
    }
  }

  void SLURMManager::filesFinished(const std::vector<openstudio::path> &t_files,
    const UUID &t_uuid)
  {
    try {
      ProcessData &pd = getProcessData(t_uuid);

      std::vector<FileInfo> finfo;

      for (std::vector<openstudio::path>::const_iterator itr = t_files.begin();
           itr != t_files.end();
           ++itr)
      {
        finfo.push_back(openstudio::runmanager::RunManager_Util::dirFile(*itr));
      }

      pd.first->setNeedsFinalStdOut(true);
      pd.first->outputFilesNotice(finfo);
      pd.first->finishedNotice(0, QProcess::NormalExit, pd.second.m_SLURMId, pd.second.m_taskNumber);
      pd.second.m_SLURMId = 0; // we are done with the SLURM side
      /// \too implement clearing of process id
      pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::Idle));
    } catch (const std::out_of_range &e) {
      LOG(Error, "Error with processing filesFinished " << e.what())
    }
  }

  void SLURMManager::finalFileListHandler(const UUID &t_uuid, const std::string &output)
  {
    std::vector<std::string> lines;
    boost::split(lines, output, boost::is_any_of("\r\n"), boost::algorithm::token_compress_on);

    try {
      ProcessData &pd = getProcessData(t_uuid);
      std::vector<openstudio::path> files;
      for (std::vector<std::string>::const_iterator itr = lines.begin();
        itr != lines.end();
        ++itr)
      {
        if (boost::regex_match(*itr, pd.second.m_tool.outFileFilter))
        {
          queueLocalCommand(boost::bind(&SLURMManager::sendStatusChanged, this, pd.first->procid(),
            AdvancedStatus(AdvancedStatusEnum::CopyingResultFiles, *itr)));

          openstudio::path localfile = pd.second.m_outdir / toPath(*itr);
          queueGetSSHFile(simplifyPath(pd.second.m_outdir.relative_path()) / toPath(*itr),
            localfile, false);
          files.push_back(localfile);
        }
      }
      queueLocalCommand(boost::bind(&SLURMManager::filesFinished, this, files, t_uuid));
    } catch (const std::out_of_range &e) {
      LOG(Error, "Error with processing finalFileListHandler " << e.what())
    }


  }

  void SLURMManager::queueGetFinalFileList(const ProcessData &pd)
  {
    std::string cmd = "ls -A1 " + toString(simplifyPath(pd.second.m_outdir.relative_path()));
    queueSSHCommand(cmd, boost::bind(&SLURMManager::finalFileListHandler, this, pd.first->procid(), _1));
  }

  void SLURMManager::handleRunningJobsStdOut(const std::string &data)
  {
    std::set<int> runningjobs;

    std::vector<std::string> lines;
    boost::split(lines, data, boost::is_any_of("\r\n"), boost::algorithm::token_compress_on);
    /* procnum ws numnum tab data */
    boost::regex reg("([0-9]+)\\s+([0-9]+)\\t([0-9]+):(.*)");

    for (std::vector<std::string>::const_iterator itr = lines.begin();
         itr != lines.end();
         ++itr)
    {
      boost::smatch results;
      if (boost::regex_match(*itr, results, reg))
      {
        int slurmid = atoi(results[1].str().c_str());
        int linenum = atoi(results[2].str().c_str());
        int tasknumber = atoi(results[3].str().c_str());
        std::string data = results[4].str() + "\n";

        try {
          ProcessData &pd = getProcessData(slurmid, tasknumber);
          if (pd.second.m_lastLineRead <= linenum - 1)
          {
            pd.first->stdOutNotice(data);
            pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::Processing));
            pd.second.m_lastLineRead = linenum;
          }

        } catch (const std::out_of_range &e) {
          LOG(Error, "Error with processing handleRunningJobsStdOut " << e.what())
        }

      }
    }

  }

  void SLURMManager::sshOOBCommandFinished(int id, const std::string &, bool timedout)
  {
    if (!timedout)
    {
      if (id == 0)
      {
        handleRunningJobsData(m_outofbanddata);
      } if (id == 1) {
        handleRunningJobsStdOut(m_outofbanddata);
      }
    }

    m_outofbanddata.clear();
  }


  void SLURMManager::sshGetFileFinished(int id, const openstudio::path &from, const openstudio::path &to, bool timedout)
  {
    QMutexLocker l(&m_mutex);

    if (m_queue.front()->id() == id)
    {
      LOG(Debug, "SSHGetFile finished: " << toString(from) << " " << toString(to));
      assert(!timedout && "SSHGetFile Timed out: need to handle");
      m_queue.front()->finished(timedout);
      m_queue.pop_front();
      //pumpQueue();
    } else {
      assert("SSHGetFile finished out of order");
    }
  }

  void SLURMManager::sshPutFileFinished(int id, const openstudio::path &from, const openstudio::path &to, bool timedout)
  {
    QMutexLocker l(&m_mutex);
    if (m_queue.front()->id() == id)
    {
      LOG(Debug, "SSHPutFile finished: " << toString(from) << " " << toString(to));
      assert(!timedout && "SSHPutFile Timed out: need to handle");
      m_queue.front()->finished(timedout);
      m_queue.pop_front();
//      pumpQueue();
    } else {
      assert(!"SSHPutFile finished out of order");
    }
  }

  void SLURMManager::sshCommandStandardOut(int id, const std::string &, const std::string &data)
  {
    QMutexLocker l(&m_mutex);
    if (m_queue.front()->id() == id)
    {
      m_queue.front()->stdoutReceived(data);
    } else {
      assert(!"SSHCommand data received out of order");
    }
  }

  void SLURMManager::getSLURMId(const std::vector<UUID> &t_uuids, const std::string &data)
  {
    LOG(Debug, "getSLURMId " << data);
    boost::smatch results;
    boost::regex reg(".*Submitted batch job ([0-9]+).*");

    for (std::vector<UUID>::const_iterator itr = t_uuids.begin();
         itr != t_uuids.end();
         ++itr)
    {
      try {
        ProcessData &pd = getProcessData(*itr);

        if (boost::regex_match(data, results, reg))
        {
          int slurmid = atoi(results[1].str().c_str());

          pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::WaitingInQueue));
          pd.second.m_SLURMId = slurmid;
          pd.first->startedNotice(slurmid, pd.second.m_taskNumber);
        } else {
          pd.first->errorNotice(QProcess::FailedToStart, "Error parsing slurmid, " + data);
        }
      } catch (const std::out_of_range &e) {
        LOG(Error, "Error with processing getSLURMId " << e.what());
      }
    }
  }

  void SLURMManager::getSHA1Checksum(const openstudio::path &remotefile, const std::string &data)
  {
    boost::smatch results;
    boost::regex reg("([a-f0-9]+)\\s+(.+)");
    if (boost::regex_match(data, results, reg))
    {
      std::string checksum = results[1].str();
      std::string filename = results[2].str();

      if (filename.find_last_of('\n') == filename.size() - 1)
      {
        //Trim off the last char if it's a stray \n
        filename.resize(filename.size() - 1);
      }

      if (toPath(filename) == remotefile)
      {
        m_checksums.setRemoteChecksum(remotefile, QByteArray::fromHex(QByteArray(checksum.data(), checksum.size())));
      }
    }
  }

  void SLURMManager::sshCommandFinished(int id, const std::string &cmd, bool timedout)
  {
    QMutexLocker l(&m_mutex);
    if (m_queue.front()->id() == id)
    {
      LOG(Debug, "SSHCommand finished: " << cmd);
      assert(!timedout && "SSHCommand Timed out: need to handle");
      m_queue.front()->finished(timedout);
      m_queue.pop_front();
//      pumpQueue();
    } else {
      assert("SSH commands finished out of order");
    }
  }

  void SLURMManager::pumpQueue()
  {

    boost::posix_time::ptime curtime = boost::posix_time::microsec_clock::universal_time();


    if (m_lastBatchBuild.is_not_a_date_time())
    {
      m_lastBatchBuild = curtime;
    } else {
      // if 5 minutes since last batch job, add batching job
      if ( (curtime - m_lastBatchBuild).total_seconds() >= 5 * 60)
      {
        std::vector<boost::reference_wrapper<ProcessData> > pds;
        for (std::map<openstudio::UUID, ProcessData>::iterator itr = m_processes.begin();
             itr != m_processes.end();
             ++itr)
        {
          if (itr->second.second.m_needsBatching)
          {
            pds.push_back(boost::ref(itr->second));
          }
        }

        if (!pds.empty())
        {
          queueDoSBatchFile(pds, m_slurm_config_options);
        }

        m_lastBatchBuild = curtime;
      }
    }

    boost::shared_ptr<SSHConnection> con = checkConnection();

    if (con->idle() && !m_queue.empty())
    {
      if (!m_queue.front()->running())
      {
        try {
          // Apply queue items until one of them returns true
          // this is to allow for items with no work to push through
          while (!m_queue.empty() && !m_queue.front()->apply(con))
          {
            m_queue.pop_front();
          }
        } catch (const std::exception &e) {
          LOG(Error, "Error applying queue task: " << e.what());
          // There was an error queuing up a command, best to reset the connection
          // and reconnect on the next try
          m_connection.reset();
          if (!m_queue.empty())
          {
            m_queue.front()->reset();
          }
        }
      } else {
        LOG(Error, "SSH Connection is idle but the front queue item is running, which does not make sense");
      }

    } else {
      LOG(Debug, "SSH Connection is busy or queue is not empty - nothing to do");
    }
  }

  void SLURMManager::processRequestStop(const openstudio::UUID &t_uuid)
  {
    QMutexLocker l(&m_mutex);
    /// nothing to do?
  }


  void SLURMManager::processRequestStart(const openstudio::UUID &t_uuid)
  {
    QMutexLocker l(&m_mutex);

    if (!m_timer.isActive())
    {
      m_timer.start(500);
    }

    try {
      ProcessData &pd = getProcessData(t_uuid);
      if (pd.second.m_SLURMId == 0)
      {
        // if it's 0, we aren't recovering from a restarted job.
        pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::Starting));
        queueTools(pd);
        queueExtractTools(pd);
        queueRequiredFiles(pd);
        queueCreateBatchFile(pd, m_slurm_config_options);
      } else {
        pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::WaitingInQueue));
      }
    } catch (const std::out_of_range &e) {
      LOG(Error, "Error with processing processRequestStart " << e.what())
    }


//    pumpQueue();
  }

  void SLURMManager::queueTools(const ProcessData &pd)
  {
    queueLocalCommand(boost::bind(&SLURMManager::sendStatusChanged, this, pd.first->procid(),
      AdvancedStatus(AdvancedStatusEnum::CopyingRequiredTools, pd.second.m_tool.name)));

    m_queue.push_back(
      boost::shared_ptr<detail::SSHQueueItem>(new detail::SSHCommand(++m_queue_id, "mkdir -p "
          + toString(toPath("~") / pd.second.m_tool.remoteToolPath())))
      );

    queuePutSSHFile(pd.second.m_tool.remoteArchive,
          pd.second.m_tool.remoteToolPath() / *(--pd.second.m_tool.remoteArchive.end()), false);
  }

  void SLURMManager::queueSSHCommand(const std::string &cmd, const boost::function<void (const std::string &handler)> &t_handler)
  {
    m_queue.push_back(
      boost::shared_ptr<detail::SSHQueueItem>(
      new detail::SSHCommand(++m_queue_id, cmd, t_handler)));
  }

  void SLURMManager::queueLocalCommand(const boost::function<void ()> &t_func)
  {
    m_queue.push_back(
      boost::shared_ptr<detail::SSHQueueItem>(new detail::SSHLocalCommand(++m_queue_id,
          t_func)));
  }

  void SLURMManager::queueGetSSHFile(const openstudio::path& t_from, const openstudio::path &t_to, bool force)
  {
    boost::function<bool (const openstudio::path &, const openstudio::path &)> check;

    if (!force)
    {
      m_checksums.updateLocalChecksum(t_to);
      queueSSHCommand("sha1sum " + toString(t_from),
          boost::bind(&SLURMManager::getSHA1Checksum, this, t_from, _1)
        );

      check = boost::bind(&openstudio::runmanager::detail::SSHFileChecksums::fileMatch,
        &this->m_checksums, _2, _1);
    }

    m_queue.push_back(
      boost::shared_ptr<detail::SSHQueueItem>(new detail::SSHGetFile(++m_queue_id,
          t_from, t_to, check)));
  }

  void SLURMManager::queuePutSSHFile(const openstudio::path& t_from, const openstudio::path &t_to, bool force)
  {
    boost::function<bool (const openstudio::path &, const openstudio::path &)> check;

    if (!force)
    {
      m_checksums.updateLocalChecksum(t_from);

      queueSSHCommand("sha1sum " + toString(t_to),
          boost::bind(&SLURMManager::getSHA1Checksum, this, t_to, _1)
        );

      check = boost::bind(&openstudio::runmanager::detail::SSHFileChecksums::fileMatch,
        &this->m_checksums, _1, _2);
    }

    m_queue.push_back(
      boost::shared_ptr<detail::SSHQueueItem>(new detail::SSHPutFile(++m_queue_id,
          t_from, t_to, check)));
  }

  void SLURMManager::queueExtractTools(const ProcessData &pd)
  {
    queueLocalCommand(boost::bind(&SLURMManager::sendStatusChanged, this, pd.first->procid(),
      AdvancedStatus(AdvancedStatusEnum::ExtractingTools, pd.second.m_tool.name)));


    openstudio::path archivefilename = *(--pd.second.m_tool.remoteArchive.end());

    std::string extractcmd
      = "[ " + toString(archivefilename) + " -nt tools_extracted_stamp ] && tar -xvf " + toString(archivefilename) + " && touch tools_extracted_stamp";

    std::string fullcmd
      = "cd " + toString(toPath("~") / pd.second.m_tool.remoteToolPath()) + " && " + extractcmd;

    queueSSHCommand(fullcmd);

  }

  std::string SLURMManager::buildParamsString(const std::vector<std::string> &t_params) const
  {
    std::string param;

    for (std::vector<std::string>::const_iterator itr = t_params.begin();
         itr != t_params.end();
         ++itr)
    {
      param += " " + *itr;
    }

    return param;
  }

  void SLURMManager::queueCreateBatchFile(ProcessData &pd, const SLURMConfigOptions &t_co)
  {
    std::string batchName = toString(pd.second.m_batchFile);
    std::stringstream batch;

    batch << "echo \"#!/bin/sh\" > " << batchName << ";";

    batch << "echo \"cd " << toString(simplifyPath(pd.second.m_outdir.relative_path())) << "\" >> " << batchName << ";";
    batch << "echo \"" << toString(toPath("~") / pd.second.m_tool.remoteToolExePath()) << buildParamsString(pd.second.m_parameters) << "\" >> " << batchName << ";";

    queueSSHCommand(batch.str());

    std::stringstream makeexe;
    makeexe << "chmod +x " << batchName;

    queueSSHCommand(makeexe.str(), boost::bind(&SLURMManager::setNeedsBatching, this, pd.first->procid(), _1));

    queueLocalCommand(boost::bind(&SLURMManager::sendStatusChanged, this, pd.first->procid(),
      AdvancedStatus(AdvancedStatusEnum::Queuing, pd.second.m_tool.name)));

  }

  void SLURMManager::setNeedsBatching(const UUID &t_id, const std::string &t_data)
  {
    try {
      ProcessData &pd = getProcessData(t_id);

      pd.second.m_needsBatching = true;
    } catch (const std::out_of_range &e) {
      LOG(Error, "Error with processing setNeedsBatching for id " << e.what());
    }
  }



  void SLURMManager::queueDoSBatchFile(const std::vector<boost::reference_wrapper<ProcessData> > &pds,
    const SLURMConfigOptions &t_co)
  {
    std::string batchName = toString(simplifyPath(pds.at(0).get().second.m_outdir.relative_path()) / toPath("sbatch.sh"));
    std::string srunName = toString(simplifyPath(pds.at(0).get().second.m_outdir.relative_path()) / toPath("srun.conf"));
    std::stringstream batch;

    batch << "echo \"#!/bin/sh\" > " << batchName << ";";

    batch << "echo \"#SBATCH ";
    if (!t_co.m_account.empty())
    {
      batch << " --account=" << t_co.m_account;
    }

    batch << " --time=" << t_co.m_maxRunTime << " \" >> " << batchName << ";";
    batch << "echo \"srun -l --multi-prog  " << srunName << "\" >> " << batchName << ";";

    queueSSHCommand(batch.str());

    std::stringstream conf;

    conf << "echo \"# srun multi-prog file for RunManager jobs \" > " << srunName << ";";

    for (size_t i = 0; i < pds.size(); ++i)
    {
      conf << "echo \"" << i << "\t/bin/sh " << toString(pds[i].get().second.m_batchFile) << "\" >> " << srunName << ";";
      pds[i].get().second.m_taskNumber = i;
      pds[i].get().second.m_needsBatching = false;
    }

    queueSSHCommand(conf.str());


    std::stringstream makeexe;
    makeexe << "chmod +x " << batchName;

    queueSSHCommand(makeexe.str());


    std::vector<UUID> procids;

    for (size_t i = 0; i < pds.size(); ++i)
    {
      procids.push_back(pds[i].get().first->procid());
    }

    queueSSHCommand("module load slurm; sbatch -D ~ -n" + boost::lexical_cast<std::string>(pds.size()) // number of tasks to run
        + ((!t_co.m_partition.empty())?(" --partition="+t_co.m_partition):"") + " " // establish partition name if one is provided
        + batchName,
        boost::bind(&SLURMManager::getSLURMId, this, procids, _1)
     );

  }


  void SLURMManager::sendStatusChanged(const UUID &uuid, const AdvancedStatus &s)
  {
    try {
      getProcessData(uuid).first->statusChangedNotice(s);
    } catch (const std::out_of_range &e) {
      LOG(Error, "Error processing sendStatusChanged " << e.what());
    }
  }

  void SLURMManager::queueRequiredFiles(const ProcessData &pd)
  {
    typedef std::vector<std::pair<openstudio::path, openstudio::path> > pathvector;
    const pathvector &files
      = pd.second.m_requiredFiles;

    for (pathvector::const_iterator itr = files.begin();
         itr != files.end();
         ++itr)
    {
      queueLocalCommand(boost::bind(&SLURMManager::sendStatusChanged, this, pd.first->procid(),
        AdvancedStatus(AdvancedStatusEnum::CopyingRequiredFiles, toString(itr->first.filename()))));

      queueSSHCommand("mkdir -p " + toString(simplifyPath(itr->second.relative_path().parent_path())));

      if (itr->first.has_root_path())
      {
        // Is an absolute "from" location, so copy it from local
        queuePutSSHFile(itr->first, simplifyPath(itr->second.relative_path()), false);
      } else {
        // is a relative location, so copy it from remote to remote
        queueSSHCommand("cp -a " + toString(toPath("~") / pd.second.m_tool.remoteToolPath() / itr->first) + " " + toString(simplifyPath(itr->second.relative_path())));
      }
    }
  }

  void SLURMManager::processRequestCleanup(const openstudio::UUID &t_uuid, const std::vector<std::string> &)
  {
    LOG(Error, "processRequestCleanup called " << toString(t_uuid) << " NOT YET IMPLEMENTED");
  }

  /// \todo Implement base path searches for slurm jobs
  boost::shared_ptr<Process> SLURMManager::createProcess(
      const openstudio::runmanager::ToolInfo &t_tool,
      const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
      const std::vector<std::string> &t_parameters,
      const openstudio::path &t_outdir,
      const std::vector<openstudio::path> &t_expectedOutputFiles,
      const std::string &t_stdin,
      const openstudio::path & /*t_basePath*/ ,
      const boost::optional<std::pair<int,int> > &t_remoteId)
  {
    QMutexLocker l(&m_mutex);

    if (t_tool.remoteArchive.empty() || t_tool.remoteExe.empty())
    {
      LOG(Warn, "Creating remote process with no remote tools set");
    } else if (!exists(t_tool.remoteArchive)) {
      throw std::runtime_error("Unable to find remote tools while creating SLRUM process: " + toString(t_tool.remoteArchive.external_file_string()));
    }

    openstudio::UUID uuid = openstudio::UUID::createUuid();

    std::vector<FileInfo> inputFiles;
    for (unsigned int i = 0; i < t_requiredFiles.size(); ++i)
    {
      inputFiles.push_back(RunManager_Util::dirFile(t_requiredFiles[i].first));
    }

    boost::shared_ptr<SLURMProcess> p(new SLURMProcess(uuid, inputFiles));

    QObject::connect(p.get(), SIGNAL(requestStart(const openstudio::UUID &)),
        this, SLOT(processRequestStart(const openstudio::UUID &)));

    QObject::connect(p.get(), SIGNAL(requestStop(const openstudio::UUID &)),
        this, SLOT(processRequestStop(const openstudio::UUID &)));

    QObject::connect(p.get(), SIGNAL(requestCleanup(const openstudio::UUID &, const std::vector<std::string> &)),
        this, SLOT(processRequestCleanup(const openstudio::UUID &, const std::vector<std::string> &)));

    m_processes[uuid] = std::make_pair(p,
        ProcessInfo(t_tool,
          t_requiredFiles,
          t_parameters, t_outdir, t_expectedOutputFiles, t_stdin));

    if (t_remoteId)
    {
      ProcessData &pd = m_processes[uuid];
      pd.second.m_SLURMId = t_remoteId->first;
      pd.second.m_taskNumber = t_remoteId->second;
      pd.first->statusChangedNotice(AdvancedStatus(AdvancedStatusEnum::WaitingInQueue));
    }

    return p;
  }


}
}

