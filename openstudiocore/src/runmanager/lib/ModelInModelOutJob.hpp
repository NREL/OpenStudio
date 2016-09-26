/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const override;
      virtual std::string description() const override;
      virtual Files outputFilesImpl() const override;
      virtual std::string getOutput() const override;
      virtual void cleanup() override;

      virtual void mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job) override;

      virtual void requestStop() override;

    protected:
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &t_creator) override;

      /// Executes the required manipulation of the model and returns a new copy.
      /// 
      /// This method needs to be implemented by any subclass
      virtual model::Model modelToModelRun(const model::Model &t_model) = 0;

      virtual void basePathChanged() override;
      virtual void standardCleanImpl() override { /* nothing to do here */ }

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
