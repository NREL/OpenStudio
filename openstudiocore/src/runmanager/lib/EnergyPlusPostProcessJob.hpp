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

#ifndef RUNMANAGER_LIB_ENERGYPLUSPOSTPROCESSJOB_HPP
#define RUNMANAGER_LIB_ENERGYPLUSPOSTPROCESSJOB_HPP


#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "JobParam.hpp"
#include "../../utilities/core/Checksum.hpp"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /**
   * Job type that postprocesses an EnergyPlus run
   *
   * This job runs in process as a separate thread.
   *
   * \sa \ref EnergyPlusPostProcessJobType
   */
  class EnergyPlusPostProcessJob : public Job_Impl
  {
    Q_OBJECT;

    public:
      /// EnergyPlusPostProcessJob constructor. 
      EnergyPlusPostProcessJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData);

      virtual ~EnergyPlusPostProcessJob();

      // Reimplemented virtual functions from Job_Impl
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &) const;
      virtual std::string description() const;
      virtual Files outputFilesImpl() const;
      virtual std::string getOutput() const;
      virtual void cleanup();

      virtual bool remoteRunnable() const
      {
        return false;
      }

      virtual void requestStop();

      virtual void basePathChanged();

    protected:
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &t_creator);
      virtual void standardCleanImpl() { /* nothing to do for this job type */ }


    private:
      REGISTER_LOGGER("openstudio.runmanager.EnergyPlusPostProcessJob");


      /// Returns the generated SQL file
      FileInfo sqlFile() const;

      mutable QReadWriteLock m_mutex;

      std::map<openstudio::path, FileTrack> m_files; //< Files tracked for outOfDate status
      boost::optional<FileInfo> m_sql; //< EnergyPlus Sql 
      mutable boost::optional<Files> m_outputfiles; //< Xml file that was created from run

      std::string m_description; //< Description of job
  }; 

}
}
}
#endif // RUNMANAGER_LIB_ENERGYPLUSPOSTPROCESSJOB_HPP
