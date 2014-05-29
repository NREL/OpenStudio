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

#ifndef RUNMANAGER_LIB_USERSCRIPTJOB_HPP
#define RUNMANAGER_LIB_USERSCRIPTJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "RubyJob.hpp"
#include "../../energyplus/ErrorFile.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
  class UserScriptJobBuilder;

namespace detail {

  /// Job which executes a generic Ruby script and makes
  /// the result values of the script available to subsequent jobs.
  /// 
  ///
  /// \sa \ref UserScriptJobType
  class UserScriptJob : public RubyJob
  {
    Q_OBJECT;

    public:
      UserScriptJob(const UUID &t_uuid,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
          const JobState &t_restoreData);

      virtual ~UserScriptJob();


    protected:

    private:
      REGISTER_LOGGER("openstudio.runmanager.UserScriptJob");

   }; 

}
}
}
#endif // RUNMANAGER_LIB_USERSCRIPTJOB_HPP
