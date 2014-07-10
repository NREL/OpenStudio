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

#include "JSON.hpp"

#include "Job.hpp"
#include "JobFactory.hpp"
#include "WorkItem.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Json.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace runmanager {
namespace detail {

  QVariant JSON::toVariant(const Job &t_jobTree) {
    QVariantMap map;
    map["job_type"] = toVariant(t_jobTree.jobType());
    if (!t_jobTree.rawInputFiles().empty()) {
      map["files"] = toVariant(t_jobTree.rawInputFiles());
    }

    std::vector<JobParam> params = cleanupParams(t_jobTree.params(), t_jobTree.getBasePath()).params();
    if (!params.empty())
    {
      map["params"] = toVariant(params);
    }

    if (!t_jobTree.tools().empty()) {
      map["tools"] = toVariant(t_jobTree.tools());
    }
    map["uuid"] = toQString(removeBraces(t_jobTree.uuid()));

    if (t_jobTree.lastRun())
    {
      map["last_run"] = QVariant(toQString(t_jobTree.lastRun()->toISO8601()));
    }

    Files outputFiles = t_jobTree.relativeOutputFiles();
    if (!outputFiles.files().empty()) {
      map["output_files"] = toVariant(outputFiles.files());
    }

    map["errors"] = toVariant(t_jobTree.errors());
    map["status"] = toVariant(t_jobTree.status());

    if (t_jobTree.allParams().has("flatoutdir"))
    {
      openstudio::path outdir = t_jobTree.outdir().filename();
      map["outdir"] = toQString(outdir);
    }

    if (!t_jobTree.children().empty()) {
      map["children"] = toVariant(t_jobTree.children());
    }

    if (t_jobTree.finishedJob())
    {
      map["finished_job"] = toVariant(*t_jobTree.finishedJob());
    }

    return QVariant(map);
  }


  std::string JSON::toJSON(const std::vector<Job> &t_jobs)
  {
    QVariantMap result = jsonMetadata().toMap();
    result["jobs"] = toVariant(t_jobs);

    return openstudio::toJSON(QVariant(result));
  }


  /// \returns a JSON string representation of the given job tree
  std::string JSON::toJSON(const Job &t_jobTree) {
    QVariantMap result = jsonMetadata().toMap();
    result["job"] = toVariant(t_jobTree);
    return openstudio::toJSON(QVariant(result));
  }

  /// Updates the given job tree with the new values from the jsonString
  //        void JSON::updateFromJSON(Job &t_jobTree, const std::string &t_jsonString);

  Job JSON::toJob(const QVariant &t_variant, const VersionString& t_version, bool t_externallyManaged) {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("job_type")) {
      throw std::runtime_error("Unable to find Job object at expected location");
    }

    OptionalDateTime lastRun;
    if (map.contains("last_run")) {
      if (t_version < VersionString("1.0.4")) {
        lastRun = DateTime(map["last_run"].toString().toStdString());
      }
      else {
        lastRun = DateTime::fromISO8601(map["last_run"].toString().toStdString());
      }
    }

    JobParams params;    
    if (map.contains("params")) {
      params = toVectorOfJobParam(map["params"], t_version);
      if (params.has("jobExternallyManaged")) {
        params.remove("jobExternallyManaged");
      }
    }
    if (t_externallyManaged) {
      params.append("jobExternallyManaged", "true");
    }

    Job job = JobFactory::createJob(
        toJobType(map["job_type"],t_version),
        map.contains("tools") ? Tools(toVectorOfToolInfo(map["tools"],t_version)) : Tools(),
        params,
        map.contains("files") ? Files(toVectorOfFileInfo(map["files"],t_version)) : Files(),
        std::vector<openstudio::URLSearchPath>(),
        false,
        map.contains("uuid") ? toUUID(map["uuid"].toString().toStdString()) : boost::optional<openstudio::UUID>(),        
        JobState(
          lastRun,
          toJobErrors(map["errors"], t_version),
          map.contains("output_files") ? Files(toVectorOfFileInfo(map["output_files"],t_version)) : Files(),
          map.contains("status") ? toAdvancedStatus(map["status"], t_version) : AdvancedStatus(),
          map.contains("outdir") ? openstudio::toPath(map["outdir"].toString()) : openstudio::path()
          )
        );

    std::vector<Job> children;
    if (map.contains("children")) {
      children = toVectorOfJob(map["children"], t_version, t_externallyManaged);
    }

    for (std::vector<Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      job.addChild(*itr);
    }

    if (map.contains("finished_job"))
    {
      Job finishedJob = toJob(map["finished_job"], t_version, t_externallyManaged);
      job.setFinishedJob(finishedJob);
    }

    return job;
  }

  /// \returns a JSON string representation of the given job tree
  Job JSON::toJob(const std::string &t_json, bool t_externallyManaged) {
    QVariant variant = loadJSON(t_json);
    VersionString version = extractOpenStudioVersion(variant);

    QVariant jobData = variant.toMap()["job"];
    return toJob(jobData,version,t_externallyManaged);
  }

  JobParams JSON::cleanupParams(JobParams t_params, const openstudio::path &t_basedir)
  {
    if (t_params.has("jobExternallyManaged"))
    {
      t_params.remove("jobExternallyManaged");
    }

    if (t_params.has("outdir"))
    {
      try {
        /*
        openstudio::path p = openstudio::toPath(t_params.get("outdir").children.at(0).value);
        std::cout << "outdir: " << openstudio::toString(p) << std::endl;
        std::cout << "basedir: " << openstudio::toString(t_basedir) << std::endl;

        p = openstudio::relativePath(p, t_basedir);
        std::cout << "relativedir: " << openstudio::toString(p) << std::endl;
        */
        t_params.remove("outdir");
        //t_params.append("outdir", openstudio::toString(p));
      } catch (...) {
      }
    }

    return t_params;
  }

  QVariant JSON::toVariant(const WorkItem &t_workItem)
  {
    QVariantMap map;
    map["type"] = toVariant(t_workItem.type);
    if (!t_workItem.tools.tools().empty()) {
      map["tools"] = toVariant(t_workItem.tools.tools());
    }
    std::vector<JobParam> params = cleanupParams(t_workItem.params, openstudio::path()).params();
    if (!params.empty())
    {
      map["params"] = toVariant(params);
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
    QVariantMap result = jsonMetadata().toMap();
    result["work_item"] = toVariant(t_workItem);
    return openstudio::toJSON(QVariant(result));
  }

  WorkItem JSON::toWorkItem(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("type")) {
      throw std::runtime_error("Unable to find WorkItem object at expected location");
    }

    return WorkItem(
        toJobType(map["type"],t_version),
        map.contains("tools") ? Tools(toVectorOfToolInfo(map["tools"],t_version)) : Tools(),
        map.contains("params") ? JobParams(toVectorOfJobParam(map["params"],t_version)) : JobParams(),
        map.contains("files") ? Files(toVectorOfFileInfo(map["files"],t_version)) : Files(),
        map.contains("jobkeyname") ? toString(map["jobkeyname"].toString()) : std::string());
  }

  /// \returns a WorkItem from the given JSON string
  WorkItem JSON::toWorkItem(const std::string &t_json)
  {
    QVariant variant = loadJSON(t_json);
    VersionString version = extractOpenStudioVersion(variant);

    QVariant workItemData = variant.toMap()["work_item"];
    return toWorkItem(workItemData,version);
  }

  // VectorOfWorkItems

  QVariant JSON::toVariant(const std::vector<WorkItem> &t_workItems) {
    QVariantList qvl;

    int index(0);
    for (const auto & workItem : t_workItems)
    {
      QVariantMap qvm = toVariant(workItem).toMap();
      qvm["work_item_index"] = QVariant(index);
      qvl.push_back(qvm);
      ++index;
    }

    return QVariant(qvl);
  }

  bool JSON::saveJSON(const std::vector<WorkItem> &t_workItems,
                      const openstudio::path &t_p,
                      bool t_overwrite)
  {
    QVariantMap result = jsonMetadata().toMap();
    result["work_items"] = toVariant(t_workItems);
    return openstudio::saveJSON(QVariant(result),t_p,t_overwrite);
  }

  bool JSON::saveJSON(const std::vector<JobParam> &t_jobParams,
                      const openstudio::path &t_p,
                      bool t_overwrite)
  {
    QVariantMap result;
    result["metadata"] = jsonMetadata();
    result["job_params"] = toVariant(t_jobParams);
    return openstudio::saveJSON(QVariant(result),t_p,t_overwrite);
  }


  std::string JSON::toJSON(const std::vector<WorkItem> &t_workItems) {
    QVariantMap result = jsonMetadata().toMap();
    result["work_items"] = toVariant(t_workItems);
    return openstudio::toJSON(QVariant(result));
  }

  std::vector<WorkItem> JSON::toVectorOfWorkItem(const QVariant &t_variant,
                                                 const VersionString& version)
  {
    return deserializeOrderedVector<WorkItem>(
      t_variant.toList(),
      "work_item_index",
      std::function<WorkItem(const QVariant&)>(std::bind(static_cast<WorkItem (*)(const QVariant&, const VersionString&)>(&JSON::toWorkItem), std::placeholders::_1, version)));
  }

  std::vector<WorkItem> JSON::toVectorOfWorkItem(const openstudio::path &t_pathToJson) {
    QVariant variant = loadJSON(t_pathToJson);
    VersionString version = extractOpenStudioVersion(variant);

    QVariant workItemsData = variant.toMap()["work_items"];
    return toVectorOfWorkItem(workItemsData,version);
  }

  std::vector<JobParam> JSON::toVectorOfJobParam(const openstudio::path &t_pathToJson) {
    QVariant variant = loadJSON(t_pathToJson);
    VersionString version = extractOpenStudioVersion(variant);

    QVariant jobParamsData = variant.toMap()["job_params"];
    return toVectorOfJobParam(jobParamsData,version);
  }


  std::vector<WorkItem> JSON::toVectorOfWorkItem(const std::string &t_json) {
    QVariant variant = loadJSON(t_json);
    VersionString version = extractOpenStudioVersion(variant);

    QVariant workItemsData = variant.toMap()["work_items"];
    return toVectorOfWorkItem(workItemsData,version);
  }

  // JobType

  QVariant JSON::toVariant(const JobType &t_jobType) {
    return QVariant(toQString(t_jobType.valueName()));
  }

  JobType JSON::toJobType(const QVariant &t_variant, const VersionString& t_version) {
    return JobType(toString(t_variant.toString()));
  }

  // VectorOfJobs

  QVariant JSON::toVariant(const std::vector<Job> &t_jobs)
  {
    QVariantList qvl;

    // ETH@20130725 If order is important, should explicitly serialize an index.
    for (const auto & job : t_jobs)
    {
      qvl.push_back(toVariant(job));
    }

    return QVariant(qvl);
  }

  std::vector<Job> JSON::toVectorOfJob(const QVariant &t_variant, const VersionString& t_version, bool t_externallyManaged)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<Job> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toJob(*itr, t_version, t_externallyManaged));
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
    map["last_modified"] = toQString(t_file.lastModified.toISO8601());
    map["key"] = toQString(t_file.key);

    if (!t_file.requiredFiles.empty()) {
      map["required_files"] = toVariant(t_file.requiredFiles);
    }

    return QVariant(map);
  }

  FileInfo JSON::toFileInfo(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || !map.contains("full_path")) {
      throw std::runtime_error("Unable to find FileInfo object at expected location");
    }

    OptionalDateTime lastModified;
    if (t_version < VersionString("1.0.4")) {
      lastModified = openstudio::DateTime(toString(map["last_modified"].toString()));
    }
    else {
      lastModified = openstudio::DateTime::fromISO8601(map["last_modified"].toString().toStdString());
    }
    OS_ASSERT(lastModified);

    FileInfo fi(
        toString(map["file_name"].toString()),
        lastModified.get(),
        toString(map["key"].toString()),
        toPath(map["full_path"].toString()),
        map["exists"].toBool());

    if (map.contains("required_files")) {
      fi.requiredFiles = toRequiredFiles(map["required_files"],t_version);
    }

    return fi;
  }

  // RequiredFiles

  QVariant JSON::toVariant(const std::vector<std::pair<QUrl, openstudio::path> > &t_requiredFiles)
  {
    QVariantList qvl;

    for (const auto & requiredFile : t_requiredFiles)
    {
      QVariantMap qvm;
      qvm["url"] = requiredFile.first;
      qvm["path"] = toQString(requiredFile.second);
      qvl.push_back(qvm);
    }

    return qvl;
  }

  std::vector<std::pair<QUrl, openstudio::path> > JSON::toRequiredFiles(const QVariant &t_variant, const VersionString& t_version)
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

    for (const auto & fileInfo : t_files)
    {
      qvl.push_back(toVariant(fileInfo));
    }

    return QVariant(qvl);
  }

  std::vector<FileInfo> JSON::toVectorOfFileInfo(const QVariant &t_variant, const VersionString& t_version)
  {
    std::vector<FileInfo> retval;
    QVariantList qvl = t_variant.toList();

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toFileInfo(*itr,t_version));
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

  JobParam JSON::toJobParam(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantMap qvm = t_variant.toMap();

    if (qvm.empty() || !qvm.contains("value"))
    {
      throw std::runtime_error("Unable to find JobParam object at expected location");
    }

    JobParam param(toString(qvm["value"].toString()));
    if (qvm.contains("children")) {
      param.children = toVectorOfJobParam(qvm["children"],t_version);
    }

    return param;
  }

  // VectorOfJobParams

  QVariant JSON::toVariant(const std::vector<JobParam> &t_params)
  {
    QVariantList qvl;

    int index(0);
    for (const auto & jobParam : t_params)
    {
      QVariantMap qvm;
      qvm["param_index"] = index;
      qvm["param"] = toVariant(jobParam);
      qvl.push_back(QVariant(qvm));
      ++index;
    }

    return QVariant(qvl);
  }

  std::vector<JobParam> JSON::toVectorOfJobParam(const QVariant &t_variant, const VersionString& t_version)
  {
    return deserializeOrderedVector<JobParam>(
          t_variant.toList(),
          "param",
          "param_index",
          std::function<JobParam (const QVariant&)>(std::bind(runmanager::detail::JSON::toJobParam,std::placeholders::_1,t_version)));
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
    qvm["version"] = toQString(t_tool.version.toString());
    qvm["out_file_filter"] = toQString(boost::lexical_cast<std::string>(t_tool.outFileFilter));

    return QVariant(qvm);
  }

  ToolInfo JSON::toToolInfo(const QVariant &t_variant, const VersionString& t_version) {
    QVariantMap qvm = t_variant.toMap();

    // should check for "version", but there was a bug in which toVariant(ToolInfo) was not 
    // serializing the version. try to limp along by filling in a blank ToolVersion.
    if (qvm.empty() || !qvm.contains("name") || (!qvm.contains("version") && (t_version > VersionString("1.0.5")))) {
      throw std::runtime_error("Unable to find ToolInfo object at expected location");
    }

    return ToolInfo(
        toString(qvm["name"].toString()),
        qvm.contains("version") ? ToolVersion::fromString(toString(qvm["version"].toString())) : ToolVersion(),
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

    for (const auto & toolInfo : t_tools)
    {
      qvl.push_back(toVariant(toolInfo));
    }

    return QVariant(qvl);
  }

  std::vector<ToolInfo> JSON::toVectorOfToolInfo(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<ToolInfo> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toToolInfo(*itr,t_version));
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

  JobErrors JSON::toJobErrors(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantMap map = t_variant.toMap();

    if (map.empty() || (!map.contains("all_errors")))
    {
      throw std::runtime_error("Unable to find JobErrors object at expected location");
    }

    return JobErrors(
          openstudio::ruleset::OSResultValue(toString(map["result"].toString())),
          toVectorOfError(map["all_errors"],t_version)
        );
  }

  AdvancedStatus JSON::toAdvancedStatus(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantMap map = t_variant.toMap();

    return AdvancedStatus(
        AdvancedStatusEnum(openstudio::toString(map["status"].toString())),
        map.contains("description")?openstudio::toString(map["description"].toString()):std::string()
        );
  }

  QVariant JSON::toVariant(const AdvancedStatus &t_status)
  {
    QVariantMap map;
    map["status"] = openstudio::toQString(t_status.value().valueName());
    if (!t_status.description().empty())
    {
      map["description"] = openstudio::toQString(t_status.description());
    }

    return map;
  }


  // VectorOfErrors

  QVariant JSON::toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    QVariantList qvl;

    for (const auto & errorType : t_errors)
    {
      QVariantMap qvm;
      qvm["error_type"] = toQString(errorType.first.valueName());
      qvm["description"] = toQString(errorType.second);
      qvl.push_back(qvm);
    }

    return qvl;
  }

  std::vector<std::pair<ErrorType, std::string> > JSON::toVectorOfError(const QVariant &t_variant, const VersionString& t_version)
  {
    QVariantList qvl = t_variant.toList();

    std::vector<std::pair<ErrorType, std::string> > retval;

    for (const auto & variant : qvl)
    {
      QVariantMap qvm = variant.toMap();
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


