#ifndef OPENSTUDIO_RUNAMANAGER_JOBSTATE_HPP
#define OPENSTUDIO_RUNAMANAGER_JOBSTATE_HPP

#include <QDateTime>
#include <boost/optional.hpp>
#include "JobErrors.hpp"
#include "FileInfo.hpp"

namespace openstudio {
namespace runmanager {

  struct JobState
  {
    JobState()
    {
    }

    JobState(const boost::optional<DateTime> &t_lastRun, const JobErrors &t_errors, const Files &t_outputFiles,
        const AdvancedStatus &t_status)
      : lastRun(toQDateTime(t_lastRun)), errors(t_errors), outputFiles(t_outputFiles), status(t_status)
    {
    }

    static boost::optional<QDateTime> toQDateTime(const boost::optional<DateTime> &t_dateTime)
    {
      if (!t_dateTime)
      {
        return boost::optional<QDateTime>();
      } else {
        return QDateTime(
            QDate(t_dateTime->date().year(), (t_dateTime->date().monthOfYear().value()) - openstudio::MonthOfYear::Jan + 1, t_dateTime->date().dayOfMonth()),
            QTime(t_dateTime->time().hours(), t_dateTime->time().minutes(), t_dateTime->time().seconds())
            );

      }
    }

    boost::optional<QDateTime> lastRun;
    JobErrors errors;
    Files outputFiles;
    AdvancedStatus status;
  };

}
}

#endif

