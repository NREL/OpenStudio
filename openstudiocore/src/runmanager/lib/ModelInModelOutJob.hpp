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

#ifndef RUNMANAGER_LIB_MODELINMODELOUTJOB_HPP
#define RUNMANAGER_LIB_MODELINMODELOUTJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "JobParam.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../model/Model.hpp"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /**
   * Base class for jobs which take one input model and create one output model.
   * Jobs which implement this base class can be merged with other jobs of the same exact
   * type.
   */
  class ModelInModelOutJob : public Job_Impl
  {
    Q_OBJECT;

    public:
      ModelInModelOutJob(const UUID &t_uuid,
          JobType t_type,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData);

      virtual ~ModelInModelOutJob();

      // Reimplemented virtual functions from Job_Impl
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const;
      virtual std::string description() const;
      virtual Files outputFilesImpl() const;
      virtual std::string getOutput() const;
      virtual void cleanup();

      /// By definition these Jobs are in-process and cannot be run remotely
      virtual bool remoteRunnable() const
      {
        return false;
      }

      virtual void mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job);

      virtual void requestStop();

    protected:
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &t_creator);

      /// Executes the required manipulation of the model and returns a new copy.
      /// 
      /// This method needs to be implemented by any subclass
      virtual model::Model modelToModelRun(const model::Model &t_model) = 0;

      virtual void basePathChanged();
      virtual void standardCleanImpl() { /* nothing to do here */ }

    private:
      REGISTER_LOGGER("openstudio.runmanager.ModelInModelOutJob");

      /// Returns the model file used for input.
      FileInfo modelFile() const;

      mutable QReadWriteLock m_mutex;

      std::map<openstudio::path, FileTrack> m_files; //< Files tracked for outOfDate status
      boost::optional<FileInfo> m_model; //< Model that is being converted
      boost::optional<QDateTime> m_lastrun; //< time of last job run
      mutable boost::optional<Files> m_outputfiles; //< IDF file that was created from run

      std::string m_description; //< Description of job

      std::vector<std::shared_ptr<ModelInModelOutJob> > m_mergedJobs; //< List of jobs merged into this one.

  }; 

}
}
}
#endif // RUNMANAGER_LIB_MODELINMODELOUTJOB_HPP
