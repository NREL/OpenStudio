#ifndef OPENSTUDIO_RUNMANAGER_JSON
#define OPENSTUDIO_RUNMANAGER_JSON

#include "RunManagerAPI.hpp"

#include <utilities/core/Path.hpp>
#include <utilities/core/Json.hpp>

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
    class FileInfo;
    class ToolInfo;
    class JobParam;
    class ErrorType;
    class JobErrors;

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

          static Job toJob(const QVariant &t_variant, const VersionString& version);

          /// \returns a job tree created from the passed in json string
          static Job toJob(const std::string &t_jsonString);

          //@}
          /** @name WorkItem High-Level Methods */
          //@{

          static QVariant toVariant(const WorkItem &t_workItem);

          static std::string toJSON(const WorkItem &t_workItem);

          static WorkItem toWorkItem(const QVariant &t_variant, const VersionString& version);

          static WorkItem toWorkItem(const std::string &t_json);

          //@}
        private:
          REGISTER_LOGGER("openstudio.runmanager.JSON");

          template<typename T>
          friend std::vector<T> deserializeUnorderedVector(const QVariantList& list,boost::function<T (QVariant*)> typeConverter);

          /** @name toVariant overloads */
          //@{

          static QVariant toVariant(const JobType &t_jobType);
          static QVariant toVariant(const std::vector<Job> &t_jobs);
          static QVariant toVariant(const FileInfo &t_file);
          static QVariant toVariant(const std::vector<std::pair<QUrl, openstudio::path> > &t_requiredFiles);
          static QVariant toVariant(const std::vector<FileInfo> &t_files);
          static QVariant toVariant(const JobParam &t_param);
          static QVariant toVariant(const std::vector<JobParam> &t_params);
          static QVariant toVariant(const ToolInfo &t_tool);
          static QVariant toVariant(const std::vector<ToolInfo> &t_tools);
          static QVariant toVariant(const JobErrors &t_errors);
          static QVariant toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors);

          //@}
          /** @name Deserialization from QVariants */
          //@{

          static JobType toJobType(const QVariant &t_variant, const VersionString& version);
          static std::vector<Job> toVectorOfJob(const QVariant &t_variant, const VersionString& version);
          static FileInfo toFileInfo(const QVariant &t_variant, const VersionString& version);
          static std::vector<std::pair<QUrl, openstudio::path> > toRequiredFiles(const QVariant &t_variant, const VersionString& version);
          static std::vector<FileInfo> toVectorOfFileInfo(const QVariant &t_variant, const VersionString& version);
          static JobParam toJobParam(const QVariant &t_variant, const VersionString& version);
          static std::vector<JobParam> toVectorOfJobParam(const QVariant &t_variant, const VersionString& version);
          static ToolInfo toToolInfo(const QVariant &t_variant, const VersionString& version);
          static std::vector<ToolInfo> toVectorOfToolInfo(const QVariant &t_variant, const VersionString& version);
          static JobErrors toJobErrors(const QVariant &t_variant, const VersionString& version);
          static std::vector<std::pair<ErrorType, std::string> > toVectorOfError(const QVariant &t_variant, const VersionString& version);

          //@}
      };
    }
  }
}
#endif

