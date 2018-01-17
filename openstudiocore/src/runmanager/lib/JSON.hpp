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

#ifndef RUNMANAGER_LIB_JSON_HPP
#define RUNMANAGER_LIB_JSON_HPP

#include "RunManagerAPI.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/Json.hpp"

#include <string>
#include <vector>

#include <QVariant>
#include <QUrl>

#include "Job.hpp"

namespace openstudio {
  class URLSearchPath;

  namespace runmanager {

    class WorkItem;
    class JobType;
    struct FileInfo;
    struct ToolInfo;
    struct JobParam;
    class ErrorType;
    struct JobErrors;

    namespace detail
    {
      class RUNMANAGER_API JSON {
        public:
          /** @name Job High-Level Methods */
          //@{

          static QVariant toVariant(const Job &t_jobTree);

          /// \returns a JSON string representation of the given job tree
          static std::string toJSON(const Job &t_jobTree);

          /// Updates the given job tree with the new values from the jsonString
          //  void updateFromJSON(Job &t_jobTree, const std::string &t_jsonString);

          static Job toJob(const QVariant &t_variant, const VersionString& t_version, bool t_externallyManaged);

          /// \returns a job tree created from the passed in json string
          static Job toJob(const std::string &t_jsonString, bool t_externallyManaged);

          //@}
          /** @name std::vector<Job> High-Level Methods */
          //@{

          static std::vector<Job> toVectorOfJob(const QVariant &t_variant, const VersionString& t_version, bool t_externallyManaged);

          static QVariant toVariant(const std::vector<Job> &t_jobs);

          static std::string toJSON(const std::vector<Job> &t_jobs);

          //@}
          /** @name WorkItem High-Level Methods */
          //@{

          static QVariant toVariant(const WorkItem &t_workItem);

          static std::string toJSON(const WorkItem &t_workItem);

          static WorkItem toWorkItem(const QVariant &t_variant, const VersionString& t_version);

          static WorkItem toWorkItem(const std::string &t_json);

          //@}
          /** @name std::vector<WorkItem> High-Level Methods */
          //@{

          static QVariant toVariant(const std::vector<WorkItem> &t_workItems);

          static bool saveJSON(const std::vector<WorkItem> &t_workItems,
                               const openstudio::path &t_p,
                               bool t_overwrite=false);

          static bool saveJSON(const std::vector<JobParam> &t_jobParams,
                               const openstudio::path &t_p,
                               bool t_overwrite=false);

          static std::string toJSON(const std::vector<WorkItem> &t_workItems);

          static std::vector<WorkItem> toVectorOfWorkItem(const QVariant &t_variant,
                                                          const VersionString& version);

          static std::vector<WorkItem> toVectorOfWorkItem(const openstudio::path &t_pathToJson);

          static std::vector<JobParam> toVectorOfJobParam(const openstudio::path &t_pathToJson);

          static std::vector<WorkItem> toVectorOfWorkItem(const std::string &t_json);

          //@}
        private:
          REGISTER_LOGGER("openstudio.runmanager.JSON");

          template<typename T>
          friend std::vector<T> deserializeUnorderedVector(const QVariantList& list,std::function<T (QVariant*)> typeConverter);

          /** @name toVariant overloads */
          //@{

          static QVariant toVariant(const JobType &t_jobType);
          static QVariant toVariant(const FileInfo &t_file);
          static QVariant toVariant(const std::vector<std::pair<QUrl, openstudio::path> > &t_requiredFiles);
          static QVariant toVariant(const std::vector<FileInfo> &t_files);
          static QVariant toVariant(const JobParam &t_param);
          static QVariant toVariant(const std::vector<JobParam> &t_params);
          static QVariant toVariant(const ToolInfo &t_tool);
          static QVariant toVariant(const std::vector<ToolInfo> &t_tools);
          static QVariant toVariant(const JobErrors &t_errors);
          static QVariant toVariant(const AdvancedStatus &t_status);
          static QVariant toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors);

          //@}
          /** @name Deserialization from QVariants */
          //@{

          static AdvancedStatus toAdvancedStatus(const QVariant &t_variant, const VersionString &t_version);
          static JobType toJobType(const QVariant &t_variant, const VersionString& t_version);
          static FileInfo toFileInfo(const QVariant &t_variant, const VersionString& t_version);
          static std::vector<std::pair<QUrl, openstudio::path> > toRequiredFiles(const QVariant &t_variant, const VersionString& t_version);
          static std::vector<FileInfo> toVectorOfFileInfo(const QVariant &t_variant, const VersionString& t_version);
          static JobParam toJobParam(const QVariant &t_variant, const VersionString& t_version);
          static std::vector<JobParam> toVectorOfJobParam(const QVariant &t_variant, const VersionString& t_version);
          static ToolInfo toToolInfo(const QVariant &t_variant, const VersionString& t_version);
          static std::vector<ToolInfo> toVectorOfToolInfo(const QVariant &t_variant, const VersionString& t_version);
          static JobErrors toJobErrors(const QVariant &t_variant, const VersionString& t_version);
          static std::vector<std::pair<ErrorType, std::string> > toVectorOfError(const QVariant &t_variant, const VersionString& t_version);

          //@}
          
          /// \returns JobParams with job state management params (jobExternallyManaged) removed. Similar 
          ///          work is performed when savings Jobs to the RunManager database
          static JobParams cleanupParams(JobParams t_params, const openstudio::path &t_baseDir);
      };
    }
  }
}
#endif // RUNMANAGER_LIB_JSON_HPP

