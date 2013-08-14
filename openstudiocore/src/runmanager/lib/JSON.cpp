#include "JSON.hpp"

#include "Job.hpp"
#include "JobFactory.hpp"
#include "WorkItem.hpp"

#include <utilities/core/Json.hpp>
#include <utilities/core/Compare.hpp>

#include <qjson/parser.h>

namespace openstudio {
namespace runmanager {
namespace detail {

  QVariant JSON::toVariant(const Job &t_jobTree) {
    QVariantMap map;
    map["job_type"] = toVariant(t_jobTree.jobType());
    if (!t_jobTree.rawInputFiles().empty()) {
      map["files"] = toVariant(t_jobTree.rawInputFiles());
    }
    if (!t_jobTree.params().empty()) {
      map["params"] = toVariant(t_jobTree.params());
    }
    if (!t_jobTree.tools().empty()) {
      map["tools"] = toVariant(t_jobTree.tools());
    }
    map["uuid"] = toQString(toUID(t_jobTree.uuid()));

    if (t_jobTree.lastRun())
    {
      map["last_run"] = QVariant(toQString(t_jobTree.lastRun()->toString()));
    }

    if (!t_jobTree.outputFiles().empty()) {
      map["output_files"] = toVariant(t_jobTree.outputFiles());
    }
    map["errors"] = toVariant(t_jobTree.errors());

    if (!t_jobTree.children().empty()) {
      map["children"] = toVariant(t_jobTree.children());
    }

    if (t_jobTree.finishedJob())
    {
      map["finished_job"] = toVariant(*t_jobTree.finishedJob());
    }

    return QVariant(map);
  }

  /// \returns a JSON string representation of the given job tree
  std::string JSON::toJSON(const Job &t_jobTree) {
    QVariantMap result;
    result["metadata"] = jsonMetadata();
    result["job"] = toVariant(t_jobTree);
    return openstudio::toJSON(QVariant(result));
  }

  /// Updates the given job tree with the new values from the jsonString
  //        void JSON::updateFromJSON(Job &t_jobTree, const std::string &t_jsonString);

  Job JSON::toJob(const QVariant &t_variant, const VersionString& version) {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("job_type")) {
      throw std::runtime_error("Unable to find Job object at expected location");
    }

    Job job = JobFactory::createJob(
        toJobType(map["job_type"],version),
        map.contains("tools") ? Tools(toVectorOfToolInfo(map["tools"],version)) : Tools(),
        map.contains("params") ? JobParams(toVectorOfJobParam(map["params"],version)) : JobParams(),
        map.contains("files") ? Files(toVectorOfFileInfo(map["files"],version)) : Files(),
        std::vector<openstudio::URLSearchPath>(),
        false,
        map.contains("uuid") ? toUUID(map["uuid"].toString().toStdString()) : boost::optional<openstudio::UUID>(),
        map.contains("last_run") ? openstudio::DateTime(toString(map["last_run"].toString())) : boost::optional<openstudio::DateTime>(),
        toJobErrors(map["errors"],version),
        map.contains("output_files") ? Files(toVectorOfFileInfo(map["output_files"],version)) : Files());

    std::vector<Job> children;
    if (map.contains("children")) {
      children = toVectorOfJob(map["children"],version);
    }

    for (std::vector<Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      job.addChild(*itr);
    }

    if (map.contains("finished_job"))
    {
      Job finishedJob = toJob(map["finished_job"],version);
      job.setFinishedJob(finishedJob);
    }

    return job;
  }

  /// \returns a JSON string representation of the given job tree
  Job JSON::toJob(const std::string &t_json) {
    std::pair<QVariant,VersionString> parseResult = loadJSON(t_json);

    QVariant jobData = parseResult.first.toMap()["job"];
    return toJob(jobData,parseResult.second);
  }

  QVariant JSON::toVariant(const WorkItem &t_workItem)
  {
    QVariantMap map;
    map["type"] = toVariant(t_workItem.type);
    if (!t_workItem.tools.tools().empty()) {
      map["tools"] = toVariant(t_workItem.tools.tools());
    }
    if (!t_workItem.params.params().empty()) {
      map["params"] = toVariant(t_workItem.params.params());
    }
    if (!t_workItem.files.files().empty()) {
      map["files"] = toVariant(t_workItem.files.files());
    }
    if (!t_workItem.jobkeyname.empty()) {
      map["jobkeyname"] = toQString(t_workItem.jobkeyname);
    }

    return QVariant(map);
  }

  /// \returns a JSON string representation of the WorkItem
  std::string JSON::toJSON(const WorkItem &t_workItem)
  {
    QVariantMap result;
    result["metadata"] = jsonMetadata();
    result["work_item"] = toVariant(t_workItem);
    return openstudio::toJSON(QVariant(result));
  }

  WorkItem JSON::toWorkItem(const QVariant &t_variant, const VersionString& version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("type")) {
      throw std::runtime_error("Unable to find WorkItem object at expected location");
    }

    return WorkItem(
        toJobType(map["type"],version),
        map.contains("tools") ? Tools(toVectorOfToolInfo(map["tools"],version)) : Tools(),
        map.contains("params") ? JobParams(toVectorOfJobParam(map["params"],version)) : JobParams(),
        map.contains("files") ? Files(toVectorOfFileInfo(map["files"],version)) : Files(),
        map.contains("jobkeyname") ? toString(map["jobkeyname"].toString()) : std::string());
  }

  /// \returns a WorkItem from the given JSON string
  WorkItem JSON::toWorkItem(const std::string &t_json)
  {
    std::pair<QVariant,VersionString> parseResult = loadJSON(t_json);

    QVariant workItemData = parseResult.first.toMap()["work_item"];
    return toWorkItem(workItemData,parseResult.second);
  }

  // JobType

  QVariant JSON::toVariant(const JobType &t_jobType) {
    return QVariant(toQString(t_jobType.valueName()));
  }

  JobType JSON::toJobType(const QVariant &t_variant, const VersionString& version) {
    return JobType(toString(t_variant.toString()));
  }

  // VectorOfJobs

  QVariant JSON::toVariant(const std::vector<Job> &t_jobs)
  {
    QVariantList qvl;

    // ETH@20130725 If order is important, should explicitly serialize an index.
    for (std::vector<Job>::const_iterator itr = t_jobs.begin();
         itr != t_jobs.end();
         ++itr)
    {
      qvl.push_back(toVariant(*itr));
    }

    return QVariant(qvl);
  }

  std::vector<Job> JSON::toVectorOfJob(const QVariant &t_variant, const VersionString& version)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<Job> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toJob(*itr, version));
    }

    return retval;
  }

  // FileInfo

  QVariant JSON::toVariant(const FileInfo &t_file)
  {
    QVariantMap map;
    map["full_path"] = toQString(t_file.fullPath);
    map["file_name"] = toQString(t_file.filename);
    map["exists"] = t_file.exists;
    map["last_modified"] = toQString(t_file.lastModified.toString());
    map["key"] = toQString(t_file.key);

    if (!t_file.requiredFiles.empty()) {
      map["required_files"] = toVariant(t_file.requiredFiles);
    }

    return QVariant(map);
  }

  FileInfo JSON::toFileInfo(const QVariant &t_variant, const VersionString& version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("full_path")) {
      throw std::runtime_error("Unable to find FileInfo object at expected location");
    }

    FileInfo fi(
        toString(map["file_name"].toString()),
        openstudio::DateTime(toString(map["last_modified"].toString())),
        toString(map["key"].toString()),
        toPath(map["full_path"].toString()),
        map["exists"].toBool());

    if (map.contains("required_files")) {
      fi.requiredFiles = toRequiredFiles(map["required_files"],version);
    }

    return fi;
  }

  // RequiredFiles

  QVariant JSON::toVariant(const std::vector<std::pair<QUrl, openstudio::path> > &t_requiredFiles)
  {
    QVariantList qvl;

    for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = t_requiredFiles.begin();
         itr != t_requiredFiles.end();
         ++itr)
    {
      QVariantMap qvm;
      qvm["url"] = itr->first;
      qvm["path"] = toQString(itr->second);
      qvl.push_back(qvm);
    }

    return qvl;
  }

  std::vector<std::pair<QUrl, openstudio::path> > JSON::toRequiredFiles(const QVariant &t_variant, const VersionString& version)
  {
    std::vector<std::pair<QUrl, openstudio::path> > retval;

    QVariantList qvl = t_variant.toList();

    for (QVariantList::iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      QVariantMap qvm = itr->toMap();

      retval.push_back(std::make_pair(qvm["url"].toUrl(), openstudio::toPath(qvm["path"].toString())));
    }

    return retval;
  }

  // VectorOfFileInfos

  QVariant JSON::toVariant(const std::vector<FileInfo> &t_files)
  {
    QVariantList qvl;

    for (std::vector<FileInfo>::const_iterator itr = t_files.begin();
         itr != t_files.end();
         ++itr)
    {
      qvl.push_back(toVariant(*itr));
    }

    return QVariant(qvl);
  }

  std::vector<FileInfo> JSON::toVectorOfFileInfo(const QVariant &t_variant, const VersionString& version)
  {
    std::vector<FileInfo> retval;
    QVariantList qvl = t_variant.toList();

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toFileInfo(*itr,version));
    }

    return retval;
  }

  // JobParam

  QVariant JSON::toVariant(const JobParam &t_param)
  {
    QVariantMap qvm;

    qvm["value"] = toQString(t_param.value);
    if (!t_param.children.empty()) {
      qvm["children"] = toVariant(t_param.children);
    }

    return qvm;
  }

  JobParam JSON::toJobParam(const QVariant &t_variant, const VersionString& version)
  {
    QVariantMap qvm = t_variant.toMap();

    if (qvm.empty() || !qvm.contains("value"))
    {
      throw std::runtime_error("Unable to find JobParam object at expected location");
    }

    JobParam param(toString(qvm["value"].toString()));
    if (qvm.contains("children")) {
      param.children = toVectorOfJobParam(qvm["children"],version);
    }

    return param;
  }

  // VectorOfJobParams

  QVariant JSON::toVariant(const std::vector<JobParam> &t_params)
  {
    QVariantList qvl;

    int index(0);
    for (std::vector<JobParam>::const_iterator itr = t_params.begin();
         itr != t_params.end();
         ++itr)
    {
      QVariantMap qvm;
      qvm["param_index"] = index;
      qvm["param"] = toVariant(*itr);
      qvl.push_back(QVariant(qvm));
      ++index;
    }

    return QVariant(qvl);
  }

  std::vector<JobParam> JSON::toVectorOfJobParam(const QVariant &t_variant, const VersionString& version)
  {
    return deserializeOrderedVector<JobParam>(
          t_variant.toList(),
          "param",
          "param_index",
          boost::function<JobParam (const QVariant&)>(boost::bind(runmanager::detail::JSON::toJobParam,_1,version)));
  }

  // ToolInfo

  QVariant JSON::toVariant(const ToolInfo &t_tool)
  {
    QVariantMap qvm;

    qvm["name"] = toQString(t_tool.name);
    qvm["version"] = toQString(t_tool.version.toString());
    qvm["local_bin_path"] = toQString(t_tool.localBinPath);
    qvm["remote_archive"] = toQString(t_tool.remoteArchive);
    qvm["remote_exe"] = toQString(t_tool.remoteExe);
    qvm["out_file_filter"] = toQString(boost::lexical_cast<std::string>(t_tool.outFileFilter));

    return QVariant(qvm);
  }

  ToolInfo JSON::toToolInfo(const QVariant &t_variant, const VersionString& version) {
    QVariantMap qvm = t_variant.toMap();

    if (qvm.empty() || !qvm.contains("name") || !qvm.contains("version"))
    {
      throw std::runtime_error("Unable to find ToolInfo object at expected location");
    }

    return ToolInfo(
        toString(qvm["name"].toString()),
        ToolVersion::fromString(toString(qvm["version"].toString())),
        toPath(qvm["local_bin_path"].toString()),
        toPath(qvm["remote_archive"].toString()),
        toPath(qvm["remote_exe"].toString()),
        boost::regex(toString(qvm["out_file_filter"].toString()))
      );
  }

  // VectorOfToolInfos

  QVariant JSON::toVariant(const std::vector<ToolInfo> &t_tools)
  {
    QVariantList qvl;

    for (std::vector<ToolInfo>::const_iterator itr = t_tools.begin();
         itr != t_tools.end();
         ++itr)
    {
      qvl.push_back(toVariant(*itr));
    }

    return QVariant(qvl);
  }

  std::vector<ToolInfo> JSON::toVectorOfToolInfo(const QVariant &t_variant, const VersionString& version)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<ToolInfo> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toToolInfo(*itr,version));
    }

    return retval;
  }

  // JobErrors

  QVariant JSON::toVariant(const JobErrors &t_errors)
  {
    QVariantMap map;
    map["result"] = toQString(t_errors.result.valueName());
    map["all_errors"] = toVariant(t_errors.allErrors);

    return map;
  }

  JobErrors JSON::toJobErrors(const QVariant &t_variant, const VersionString& version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || (!map.contains("all_errors")))
    {
      throw std::runtime_error("Unable to find JobErrors object at expected location");
    }

    return JobErrors(
          openstudio::ruleset::OSResultValue(toString(map["result"].toString())),
          toVectorOfError(map["all_errors"],version)
        );
  }

  // VectorOfErrors

  QVariant JSON::toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    QVariantList qvl;

    for (std::vector<std::pair<ErrorType, std::string> >::const_iterator itr = t_errors.begin();
         itr != t_errors.end();
         ++itr)
    {
      QVariantMap qvm;
      qvm["error_type"] = toQString(itr->first.valueName());
      qvm["description"] = toQString(itr->second);
      qvl.push_back(qvm);
    }

    return qvl;
  }

  std::vector<std::pair<ErrorType, std::string> > JSON::toVectorOfError(const QVariant &t_variant, const VersionString& version)
  {
    QVariantList qvl = t_variant.toList();

    std::vector<std::pair<ErrorType, std::string> > retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      QVariantMap qvm = itr->toMap();
      retval.push_back(
          std::make_pair(
            ErrorType(toString(qvm["error_type"].toString())),
            toString(qvm["description"].toString())
            )
          );
    }

    return retval;
  }

} // detail
} // runmanager
} // openstudio


