#ifndef OPENSTUDIO_RUNMANAGER_SLURM_MANAGER_HPP__
#define OPENSTUDIO_RUNMANAGER_SLURM_MANAGER_HPP__

#include "ProcessCreator.hpp"
#include "SSHConnection.hpp"
#include "SLURMProcess.hpp"
#include <QTimer>

namespace openstudio {
namespace runmanager {

namespace detail {
  /// Internal class for keeping a database of remote and local file checksums to determine
  /// when and if a file needs to be transfered
  class SSHFileChecksums
  {
  public:
    void updateLocalChecksum(const openstudio::path &t_path);
    void setRemoteChecksum(const openstudio::path &t_path, const QByteArray &t_checksum);

    bool fileMatch(const openstudio::path &t_local, const openstudio::path &t_remote) const;

  private:
    std::map<openstudio::path, QByteArray> m_locals;
    std::map<openstudio::path, QByteArray> m_remotes;
  };


  /// Internal base class for creating a list of sequential events that need to
  /// occur over the SSH connection
  /// \todo this and its related classes may want to be moved into the SSHConnection object
  ///       for reuse by other types of remote connections
  class SSHQueueItem
  {
  public:
    SSHQueueItem(int t_id);
    virtual ~SSHQueueItem() {}

    /// Execute the SSHQueueItem on the remote connection
    /// \returns true if remote work was initiated, false if there was nothing to do
    bool apply(const boost::shared_ptr<SSHConnection> &t_conn);

    /// \return true if currently processing
    bool running() const;

    /// \return tracking id of Queue item
    int id() const;

    /// Called when the connection is reset 
    void reset();

    virtual void stdoutReceived(const std::string &) = 0;
    virtual void finished(bool timedout) = 0;

  protected:
    virtual bool apply_impl(const boost::shared_ptr<SSHConnection> &t_conn) = 0;

  private:
    int m_id;
    bool m_running;
  };

  /// SSHQueueItem implementation for executing a local command
  class SSHLocalCommand : public SSHQueueItem
  {
  public:
    SSHLocalCommand(int t_id,
      boost::function<void ()> t_func);
    virtual ~SSHLocalCommand() {}

    virtual void stdoutReceived(const std::string &t_data);
    virtual void finished(bool timedout);

  protected:
    virtual bool apply_impl(const boost::shared_ptr<SSHConnection> &t_conn);

  private:
    boost::function<void ()> m_func;
  };


  /// SSHQueueItem implementation for executing a remote command
  class SSHCommand : public SSHQueueItem
  {
  public:
    SSHCommand(int t_id, const std::string &t_command,
      const boost::function<void (const std::string &)> &t_handler = boost::function<void (const std::string &)>());
    virtual ~SSHCommand() {}

    virtual void stdoutReceived(const std::string &t_data);
    virtual void finished(bool timedout);

  protected:
    virtual bool apply_impl(const boost::shared_ptr<SSHConnection> &t_conn);

  private:
    std::string m_command;
    std::string m_data;
    boost::function<void (const std::string &)> m_datahandler;
  };


  /// SSHQueueItem implementation for downloading a remote file
  class SSHGetFile : public SSHQueueItem
  {
  public:
    /// \param[in] t_id item id
    /// \param[in] t_from remote file location
    /// \param[in] t_to local file location
    /// \param[in] t_check function to call to see if transfer needs to occur (say, if checksums don't match)
    SSHGetFile(int t_id, const openstudio::path &t_from, const openstudio::path &t_to,
      const boost::function<bool (const openstudio::path &, const openstudio::path &)> &t_check
          = boost::function<bool (const openstudio::path &, const openstudio::path &)>());
    virtual ~SSHGetFile() {}

    virtual void stdoutReceived(const std::string &t_data) { assert(!"SSHGetFile should never receive stdout"); }
    virtual void finished(bool timedout);

  protected:
    virtual bool apply_impl(const boost::shared_ptr<SSHConnection> &t_conn);

  private:
    openstudio::path m_from;
    openstudio::path m_to;
    boost::function<bool (const openstudio::path &, const openstudio::path &)> m_check;
  };

  /// SSHQueueItem implementation for uploading a file to the remote host
  class SSHPutFile : public SSHQueueItem
  {
  public:
    /// \param[in] t_id item id
    /// \param[in] t_from local file location
    /// \param[in] t_to remote file location
    /// \param[in] t_check function to call to see if transfer needs to occur (say, if checksums don't match)
    SSHPutFile(int t_id, const openstudio::path &t_from, const openstudio::path &t_to,
      const boost::function<bool (const openstudio::path &, const openstudio::path &)> &t_check 
        = boost::function<bool (const openstudio::path &, const openstudio::path &)>());
    virtual ~SSHPutFile() {}
    virtual void stdoutReceived(const std::string &t_data) { assert(!"SSHGetFile should never receive stdout"); }
    virtual void finished(bool timedout);

  protected:
    virtual bool apply_impl(const boost::shared_ptr<SSHConnection> &t_conn);

  private:
    openstudio::path m_from;
    openstudio::path m_to;
    boost::function<bool (const openstudio::path &, const openstudio::path &)> m_check;
  };
} // namespace detail

  /// Configuration Options for remotely executed SLURM jobs 
  struct SLURMConfigOptions 
  {
    SLURMConfigOptions()
    {
    }

    SLURMConfigOptions(int t_maxRunTime, const std::string &t_partition, const std::string &t_account)
      : m_maxRunTime(t_maxRunTime), m_partition(t_partition), m_account(t_account)
    {
    }

    int m_maxRunTime;        //< Max time in seconds the SLURM manager will allow the job to run
    std::string m_partition; //< SLURM partition name to use when queueing a job
    std::string m_account;   //< SLURM account name to use when queueing a job
  };

  /// ProcessCreator implementation for SLURM based jobs
  class SLURMManager : public ProcessCreator
  {
    Q_OBJECT;

    public:
      SLURMManager();

      /// Set SSHCredentials to use during connection to SLURM cluster.
      void setConfiguration(const SSHCredentials &t_creds, const SLURMConfigOptions &t_config);

      virtual boost::shared_ptr<Process> createProcess(
          const openstudio::runmanager::ToolInfo &t_tool,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath,
          const boost::optional<std::pair<int,int> > &t_remoteId);

      /// \returns true
      virtual bool isRemoteManager() const
      {
        return true;
      }

      void activate();

      /// \return true if the SLURM Manager has a valid SSH connection or if one can be
      ///         obtained with no user interaction
      bool hasConnection();

    private:
      REGISTER_LOGGER("openstudio.runmanager.SLURMManager");

      QMutex m_mutex;

      /// Internal holder for maintaining a list of all Processes that are
      /// being handled by this SLURM connection
      struct ProcessInfo
      {
        ProcessInfo() {}

        ProcessInfo(
            const openstudio::runmanager::ToolInfo &t_tool,
            const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
            const std::vector<std::string> &t_parameters,
            const openstudio::path &t_outdir,
            const std::vector<openstudio::path> &t_expectedOutputFiles,
            const std::string &t_stdin);

        openstudio::runmanager::ToolInfo m_tool;
        std::vector<std::pair<openstudio::path, openstudio::path> > m_requiredFiles;
        std::vector<std::string> m_parameters;
        openstudio::path m_outdir;
        std::vector<openstudio::path> m_expectedOutputFiles;
        std::string m_stdin;
        openstudio::path m_batchFile;
        int m_SLURMId;
        int m_taskNumber;
        int m_lastLineRead;
        bool m_finalFileListRequested;
        bool m_needsBatching;
      };

      typedef std::pair<boost::shared_ptr<SLURMProcess>, ProcessInfo>  ProcessData;

      /// Queue up an individual file put
      void queuePutSSHFile(const openstudio::path& t_from, const openstudio::path &t_to, bool force);

      /// Queue up an individual file get
      void queueGetSSHFile(const openstudio::path& t_from, const openstudio::path &t_to, bool force);

      /// Queue up a local command to be processed in sequence with the ssh commands
      void queueLocalCommand(const boost::function<void ()> &t_func);

      /// Queue up an ssh command
      /// \param t_handler Function to call after ssh exec has finished, passing all collected stdout to it
      void queueSSHCommand(const std::string &cmd, 
                           const boost::function<void (const std::string &handler)> &t_handler
                               = boost::function<void (const std::string &)>());

      /// Queues up transfer of tools to remote system
      void queueTools(const ProcessData &d);

      /// Queues up transfer of required (input) files for process to remote system
      void queueRequiredFiles(const ProcessData &d);

      /// Queues up ssh command to extract tools archive
      void queueExtractTools(const ProcessData &d);

      /// Queues up an ssh command to get the directory listing of the output
      /// directory for the given job
      void queueGetFinalFileList(const ProcessData &pd);

      /// Queues up ssh command to dynamically create batch file needed by SLURM to queue up job
      void queueCreateBatchFile(ProcessData &pd, const SLURMConfigOptions &t_co);

      /// \returns the ProcessData for the given uuid, throws if not found
      const ProcessData &getProcessData(const openstudio::UUID &t_uuid) const;

      /// \returns the ProcessData for the given uuid, throws if not found
      ProcessData &getProcessData(const openstudio::UUID &t_uuid);

      /// \returns the ProcessData for the given slurmid and tasknumber, throws if not found
      const ProcessData &getProcessData(int slurmid, int tasknumber) const;

      /// \returns the ProcessData for the given slurmid and tasknumber, throws if not found
      ProcessData &getProcessData(int slurmid, int tasknumber);

      /// \returns a formatted string of the parameters to be used by this process.
      /// Used in the creation of the sbatch file
      std::string buildParamsString(const std::vector<std::string> &t_params) const;

      /// Called when the list of output files form a process is received.
      void finalFileListHandler(const UUID &t_uuid, const std::string &output);

      /// Called when all files have finished transfer for the remote job
      void filesFinished(const std::vector<openstudio::path> &t_files,
          const UUID &t_uuid);

      /// processes the id returned by the SLURM queue command for the given process uuids
      void getSLURMId(const std::vector<UUID> &t_uuids, const std::string &data);
  
      /// processes the SHA1 checkums returned for a remote file path when an sha1sum command
      /// has been executed remotely
      void getSHA1Checksum(const openstudio::path &remotefile, const std::string &data);

      /// pumps the queue of SSHQueueItem objects that need to be executed remotely
      void pumpQueue();

      /// Handles the set of output lines for running processes, as read from oob data
      void handleRunningJobsStdOut(const std::string &data);

      /// Handles the set of running processes, as read from oob data
      void handleRunningJobsData(const std::string &data);

      /// sends the status update to the SLURMProcess implementation
      void sendStatusChanged(const UUID &uuid, const AdvancedStatus &s);

      /// Checks the status of the remote connection, connects if needed, and returns it
      /// \todo hide m_connection from user to that this function is forced to be used
      boost::shared_ptr<SSHConnection> checkConnection(bool t_allowui = false);

      /// Sets the m_needsBatching flag for the given UUID
      void setNeedsBatching(const UUID &t_id, const std::string &t_data);

      /// Queues up the writing and execution of an SBATCH file for a set of processes
      void queueDoSBatchFile(const std::vector<boost::reference_wrapper<ProcessData> > &pds,
        const SLURMConfigOptions &t_co);

      /// takes a path and simplifies it for easier use on the remote server
      static openstudio::path simplifyPath(const openstudio::path &t_path);

      int m_queue_id;
      SSHCredentials m_ssh_creds;
      SSHCredentials m_staged_ssh_creds;
      SLURMConfigOptions m_slurm_config_options;
      boost::posix_time::ptime m_lastBatchBuild;

      std::map<openstudio::UUID, ProcessData > m_processes;
      boost::shared_ptr<SSHConnection> m_connection;
      std::deque<boost::shared_ptr<detail::SSHQueueItem> > m_queue;

      /// data received from an SSH command executing on the outofband channel on the SSHConnection object
      std::string m_outofbanddata;       

      QTimer m_timer; //< Timer for initiating status query checks against remote processes

      /// Local and remote collected file checksums
      detail::SSHFileChecksums m_checksums;

      int m_status_query_id; //< The next status query id to set  
      boost::optional<boost::posix_time::ptime> m_lastStatusQuery; //< The time of the last status query run

      int m_connectErrorCount; //< The number of consequtive connection errors that have occured

    private slots:
      /// Handler for SSHProcess requestStart
      void processRequestStart(const openstudio::UUID &t_uuid);
      /// Handler for SSHProcess requestStop
      void processRequestStop(const openstudio::UUID &t_uuid);
      /// Handler for SSHProcess requestCleanup
      void processRequestCleanup(const openstudio::UUID &t_uuid, const std::vector<std::string> &);

      /// Handler for SSHConnection getFileFinished
      void sshGetFileFinished(int, const openstudio::path &from, const openstudio::path &to, bool timedout);
      /// Handler for SSHConnection putFileFinished
      void sshPutFileFinished(int id, const openstudio::path &from, const openstudio::path &to, bool timedout);

      /// Handler for SSHConnection commandStandardOut
      void sshCommandStandardOut(int, const std::string &, const std::string &);
      /// Handler for SSHConnection commandFinished
      void sshCommandFinished(int id, const std::string &cmd, bool timedout);

      /// Handler for SSHConnection oobCommandStandardOut
      void sshOOBCommandStandardOut(int id, const std::string &, const std::string &);
      /// Handler for SSHConnection oobCommandStandardFinished
      void sshOOBCommandFinished(int id, const std::string &cmd, bool timedout);


      void sendStatusQuery(); //< sends status request checks for remote processes on outofband channel

  };

}
}

#endif
