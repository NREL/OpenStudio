#ifndef OPENSTUDIO_RUNMANAGER_SLURMPROCESS_HPP_
#define OPENSTUDIO_RUNMANAGER_SLURMPROCESS_HPP_

#include "Process.hpp"
#include "FileInfo.hpp"
#include <utilities/core/UUID.hpp>

namespace openstudio {
namespace runmanager {

  /// A thin implementation of Process. Most of the actualy worked is accomplished via callbacks into
  /// SLRUMManager, which manages the SSH connection used
  class SLURMProcess : public Process
  {
    Q_OBJECT;

    public:
      /// \param[in] t_procid openstudio::UUID uniquely identifying this process
      /// \param[in] t_inputFiles vector of FileInfo objects used for input to this process
      SLURMProcess(const openstudio::UUID &t_procid, const std::vector<FileInfo> &t_inputFiles);
      virtual ~SLURMProcess() {}

      // Virtual overloads
      virtual void waitForFinished();
      virtual void start();
      virtual bool running() const;
      virtual void cleanup(const std::vector<std::string> &t_filenames);
      virtual std::vector<FileInfo> outputFiles() const;
      virtual std::vector<FileInfo> inputFiles() const;
      virtual void cleanUpRequiredFiles() { LOG(Error, "cleanUpRequiredFiles not implemented yet for SLURMProcess."); }

      /// \returns process uuid
      openstudio::UUID procid() const;

    protected:
      virtual void stopImpl();

    signals:
      /// Emitted when the start() method is called
      void requestStart(const openstudio::UUID &t_procid);

      /// Emitted when the cleanup() method is called
      void requestCleanup(const openstudio::UUID &t_procid, const std::vector<std::string> &t_files);

      /// emitted when the stop() method is called
      void requestStop(const openstudio::UUID &t_procid);

    private:
      bool needsFinalStdOut() const;
      void setNeedsFinalStdOut(bool);


      REGISTER_LOGGER("openstudio.runmanager.SLURMProcess");
      friend class SLURMManager;

      openstudio::UUID m_procid;
      bool m_starting; //< true if process is starting up
      bool m_running;  //< true if process is currently running
      bool m_needsFinalStdOut; //< true if process is finished, but needs last bit of stdout
      std::vector<FileInfo> m_inputFiles;  //< vector of input files used
      std::vector<FileInfo> m_outputFiles; //< vector of output files created

    private slots:
      /// called when SLURMManager has recognized output files
      void outputFilesNotice(const std::vector<FileInfo> &t_outputFiles);

      /// called when SLURMManager has started the process
      void startedNotice(int t_slurmid, int t_slurmTaskNumber);

      /// Called when SLURMManager has recognized a file change
      void outputFileChangedNotice(const openstudio::runmanager::FileInfo &file);

      /// Called when SLURMManager has recognized a process error
      void errorNotice(QProcess::ProcessError e, const std::string &);

      /// Called when SLURMManager has recognized a process completion
      void finishedNotice(int exitcode, QProcess::ExitStatus e, int t_slurmid, int t_slrumTaskNumber);

      /// Called when SLURMManager has recognized standard out data
      void stdOutNotice(const std::string &data);

      /// Called when SLURMManager has recognized a state change
      void statusChangedNotice(const openstudio::runmanager::AdvancedStatus &status);

  };

}
}

#endif
