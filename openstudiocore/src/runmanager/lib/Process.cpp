#include "Process.hpp"

namespace openstudio {
namespace runmanager {

  Process::Process()
    : m_status(AdvancedStatusEnum::Idle), m_stopped(false)
  {
  }

  void Process::emitOutputFileChanged(const openstudio::runmanager::FileInfo &file)
  {
    emit outputFileChanged(file);
  }

  void Process::emitStatusChanged(const openstudio::runmanager::AdvancedStatus &status)
  {
    if (status != m_status)
    {
      m_status = status;
      emit statusChanged(m_status);
    }
  }

  openstudio::runmanager::AdvancedStatus Process::status() const
  {
    return m_status;
  }

  void Process::stop()
  {
    m_stopped = true;
    stopImpl();
  }

  bool Process::stopped() const
  {
    return m_stopped;
  }

}
}
