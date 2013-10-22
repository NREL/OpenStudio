#ifndef OPENSTUDIO_RUNAMANAGER_JOBSTATE_HPP
#define OPENSTUDIO_RUNAMANAGER_JOBSTATE_HPP

#include <QDateTime>
#include <boost/optional.hpp>
#include "JobErrors.hpp"
#include "FileInfo.hpp"

#include <utilities/time/DateTime.hpp>

namespace openstudio {
namespace runmanager {

  struct JobState
  {
    JobState()
    {
    }

    JobState(const boost::optional<DateTime> &t_lastRun, 
             const JobErrors &t_errors, 
             const Files &t_outputFiles,
             const AdvancedStatus &t_status,
             const openstudio::path &t_outdir)
      : lastRun(t_lastRun ? toQDateTime(*t_lastRun) : boost::optional<QDateTime>()), 
        errors(t_errors), 
        outputFiles(t_outputFiles), 
        status(t_status),
        outdir(t_outdir)
    {
    }

    boost::optional<QDateTime> lastRun;
    JobErrors errors;
    Files outputFiles;
    AdvancedStatus status;
    openstudio::path outdir;
  };

}
}

#endif

