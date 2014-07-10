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

#ifndef RUNMANAGER_LIB_JOBSTATE_HPP
#define RUNMANAGER_LIB_JOBSTATE_HPP

#include <QDateTime>
#include <boost/optional.hpp>
#include "JobErrors.hpp"
#include "FileInfo.hpp"

#include "../../utilities/time/DateTime.hpp"

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

#endif // RUNMANAGER_LIB_JOBSTATE_HPP

