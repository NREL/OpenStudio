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

#ifndef RUNMANAGER_LIB_CALCULATEECONOMICSJOB_HPP
#define RUNMANAGER_LIB_CALCULATEECONOMICSJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "JobParam.hpp"
#include "JobFactory.hpp"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /**
   * Job implementation stub for calculating economics from an energy plus run. Currently does nothing.
   *
   * \sa \ref CalculateEconomicsJobType
   */
  class CalculateEconomicsJob : public Job_Impl
  {
    Q_OBJECT;

    public:
      /// CalculateEconomics constructor. 
      CalculateEconomicsJob(const UUID &t_uuid,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
      const JobState &t_restoreData);

      virtual ~CalculateEconomicsJob();

      // Reimplemented virtual functions from Job_Impl
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const;
      virtual std::string description() const;
      virtual Files outputFilesImpl() const;
      virtual std::string getOutput() const;
      virtual void cleanup();

      /// Cannot yet run remotely
      /// \returns false
      virtual bool remoteRunnable() const
      {
        return false;
      }

      virtual void requestStop();

    protected:
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &);
      virtual void basePathChanged();
      virtual void standardCleanImpl() { /* nothing to do for this job type */ }


    private:
      REGISTER_LOGGER("openstudio.runmanager.CalculateEconomics");
  }; 

}
}
}
#endif // RUNMANAGER_LIB_CALCULATEECONOMICSJOB_HPP
