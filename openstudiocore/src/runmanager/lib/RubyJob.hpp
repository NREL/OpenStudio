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

#ifndef RUNMANAGER_LIB_RUBYJOB_HPP
#define RUNMANAGER_LIB_RUBYJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "ToolBasedJob.hpp"
#include "MergedJobResults.hpp"
#include "../../energyplus/ErrorFile.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
  class RubyJobBuilder;

namespace detail {

  /// Job which executes a generic Ruby script and makes
  /// the result values of the script available to subsequent jobs
  ///
  /// \sa \ref RubyJobType
  class RubyJob : public ToolBasedJob
  {
    Q_OBJECT;

    public:
      RubyJob(const UUID &t_uuid,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
      const JobState &t_restoreData);

      virtual ~RubyJob();


    protected:
      RubyJob(const UUID &t_uuid,
          const JobType &t_jobtype,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
      const JobState &t_restoreData);


      virtual std::string descriptionImpl() const override;
      virtual std::string detailedDescriptionImpl() const override
      {
        return "";
      }

      virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const override;
      virtual void startHandlerImpl() override;

      virtual void basePathChanged() override;

      virtual void mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job) override;

      virtual bool hasMergedJobsImpl() const override;
      virtual std::vector<MergedJobResults> mergedJobResultsImpl() const override;

    private:
      REGISTER_LOGGER("openstudio.runmanager.RubyJob");

      void getFiles(const RubyJobBuilder &t_rjb);
      static std::vector<JobParams> getMergedJobs(const JobParams &t_params);

      std::vector<std::pair<int, std::pair<Files, std::string> > > m_inputfiles;
      std::string m_description;
      std::vector<JobParams> m_mergedJobs;
   }; 

}
}
}
#endif // RUNMANAGER_LIB_RUBYJOB_HPP
