#ifndef OPENSTUDIO_RUNMANAGER_JSON
#define OPENSTUDIO_RUNMANAGER_JSON

#include <string>
#include <vector>
#include <QVariant>
#include <QUrl>
#include <utilities/core/Path.hpp>


namespace openstudio {
  namespace runmanager {

    class Job;
    class WorkItem;
    class JobType;
    class FileInfo;
    class ToolInfo;
    class JobParam;
    class ErrorType;
    class JobErrors;

    class JSON
    {
      public:
        /// \returns a job tree created from the passed in json string
        static Job toJob(const std::string &t_jsonString);

        /// \returns a JSON string representation of the given job tree
        static std::string toJSON(const Job &t_jobTree);

        /// Updates the given job tree with the new values from the jsonString
//        void updateFromJSON(Job &t_jobTree, const std::string &t_jsonString);

/// \returns a JSON string representation of the WorkItem
        static std::string toJSON(const WorkItem &t_workItem);

        /// \returns a WorkItem from the given JSON string
        static WorkItem toWorkitem(const std::string &t_jsonString);

        static Job toJob(const QVariant &t_variant);


        static std::string toJSON(const QVariant &t_variant);
        static QVariant toVariant(const std::vector<Job> &t_jobs);
        static QVariant toVariant(const JobType &t_jobType);
        static QVariant toVariant(const std::vector<std::pair<QUrl, openstudio::path> > &t_requiredFiles);
        static QVariant toVariant(const FileInfo &t_file);
        static QVariant toVariant(const std::vector<FileInfo> &t_files);
        static QVariant toVariant(const JobParam &t_param);
        static QVariant toVariant(const std::vector<JobParam> &t_params);
        static QVariant toVariant(const ToolInfo &t_tool);
        static QVariant toVariant(const std::vector<ToolInfo> &t_tools);
        static QVariant toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors);
        static QVariant toVariant(const JobErrors &t_errors);
        static QVariant toVariant(const Job &t_jobTree);

    };
  }
}
#endif

