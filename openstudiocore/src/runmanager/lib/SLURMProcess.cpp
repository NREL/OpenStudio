#include "SLURMProcess.hpp"

namespace openstudio {
namespace runmanager {

  SLURMProcess::SLURMProcess(const openstudio::UUID &t_procid, const std::vector<FileInfo> &t_inputFiles)
    : m_procid(t_procid), m_starting(false), m_running(false), m_needsFinalStdOut(false),
      m_inputFiles(t_inputFiles)
  {
  }

  openstudio::UUID SLURMProcess::procid() const
  {
    return m_procid;
  }

  void SLURMProcess::waitForFinished()
  {
    LOG(Error, "waitForFinished not supported with SLURMProcess");
  }

  void SLURMProcess::start() 
  {
    LOG(Debug, "Requesting job start");
    m_starting = true;
    emit requestStart(m_procid); 
  }

  void SLURMProcess::stopImpl()
  {
//    emit terminate();
//    emit requestStop(m_procid);
  }

  bool SLURMProcess::running() const { 
    return m_starting || m_running;
  }

  void SLURMProcess::cleanup(const std::vector<std::string> &t_filenames)
  {
    emit requestCleanup(m_procid, t_filenames);
  }

  std::vector<FileInfo> SLURMProcess::outputFiles() const
  {
    return m_outputFiles;
  }

  std::vector<FileInfo> SLURMProcess::inputFiles() const
  {
    return m_inputFiles;
  }

  void SLURMProcess::statusChangedNotice(const openstudio::runmanager::AdvancedStatus &status)
  {
    emit statusChanged(status);
  }



  void SLURMProcess::outputFilesNotice(const std::vector<FileInfo> &t_outputFiles)
  {
    m_outputFiles = t_outputFiles;

    for (std::vector<FileInfo>::const_iterator itr = t_outputFiles.begin();
         itr != t_outputFiles.end();
         ++itr)
    {
      emit outputFileChanged(*itr);
    }
  }

  void SLURMProcess::startedNotice(int t_slurmid, int t_slurmtaskid)
  {
    m_running = true;
    m_starting = false;
    emit started();
    emit remoteStarted(t_slurmid, t_slurmtaskid);
  }

  bool SLURMProcess::needsFinalStdOut() const
  {
    return m_needsFinalStdOut;
  }

  void SLURMProcess::setNeedsFinalStdOut(bool t_new)
  {
    m_needsFinalStdOut = t_new;
  }

  void SLURMProcess::outputFileChangedNotice(const openstudio::runmanager::FileInfo &file)
  {
    emit outputFileChanged(file);
  }

  void SLURMProcess::errorNotice(QProcess::ProcessError e, const std::string &t_message)
  {
    m_running = false;
    emit error(e, t_message);
  }

  void SLURMProcess::finishedNotice(int exitcode, QProcess::ExitStatus e, int t_slurmid, int t_slurmtasknumber)
  {
    m_running = false;
    emit finished(exitcode, e);
    emit remoteFinished(t_slurmid, t_slurmtasknumber);
  }

  void SLURMProcess::stdOutNotice(const std::string &data)
  {
    emit standardOutDataAdded(data);
  }



}
}

