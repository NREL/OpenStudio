#include "JSON.hpp"

#include "Job.hpp"
#include "JobFactory.hpp"
#include "WorkItem.hpp"

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace openstudio {
namespace runmanager {
namespace detail {

  /// \returns a JSON string representation of the given job tree
  std::string JSON::toJSON(const Job &t_jobTree)
  {
    return toJSON(toVariant(t_jobTree));
  }

  std::string JSON::toJSON(const QVariant &t_variant)
  {
    QJson::Serializer serializer;
    bool ok = false;
    QByteArray qba = serializer.serialize(t_variant, &ok);

    if (ok)
    {
      return toString(QString(qba));
    } else {
      throw std::runtime_error("Error serializing to JSON: " + toString(serializer.errorMessage()));
    }
  }

  QVariant JSON::toVariant(const JobType &t_jobType)
  {
    return QVariant(toQString(t_jobType.valueName()));
  }

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

  QVariant JSON::toVariant(const FileInfo &t_file)
  {
    QVariantMap map;
    map["fullPath"] = toQString(t_file.fullPath);
    map["filename"] = toQString(t_file.filename);
    map["exists"] = t_file.exists;
    map["lastModified"] = toQString(t_file.lastModified.toString());
    map["key"] = toQString(t_file.key);

    map["requiredFiles"] = toVariant(t_file.requiredFiles);

    QVariantMap parentMap;
    parentMap["FileInfo"] = map;

    return QVariant(parentMap);
  }



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

  QVariant JSON::toVariant(const JobParam &t_param)
  {
    QVariantMap qvm;

    qvm["value"] = toQString(t_param.value);
    qvm["children"] = toVariant(t_param.children);

    QVariantMap parent;
    parent["JobParam"] = qvm;
    return parent;
  }

  QVariant JSON::toVariant(const std::vector<JobParam> &t_params)
  {
    QVariantList qvl;

    for (std::vector<JobParam>::const_iterator itr = t_params.begin();
         itr != t_params.end();
         ++itr)
    {
      qvl.push_back(toVariant(*itr));
    }

    return QVariant(qvl);
  }

  QVariant JSON::toVariant(const ToolInfo &t_tool)
  {
    QVariantMap qvm;

    qvm["name"] = toQString(t_tool.name);
    qvm["version"] = toQString(t_tool.version.toString());
    qvm["localBinPath"] = toQString(t_tool.localBinPath);
    qvm["remoteArchive"] = toQString(t_tool.remoteArchive);
    qvm["remoteExe"] = toQString(t_tool.remoteExe);
    qvm["outFileFilter"] = toQString(boost::lexical_cast<std::string>(t_tool.outFileFilter));

    QVariantMap parent;
    parent["ToolInfo"] = qvm;
    return QVariant(qvm);
  }

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

  QVariant JSON::toVariant(const std::vector<Job> &t_jobs)
  {
    QVariantList qvl;

    for (std::vector<Job>::const_iterator itr = t_jobs.begin();
         itr != t_jobs.end();
         ++itr)
    {
      qvl.push_back(toVariant(*itr));
    }

    return QVariant(qvl);
  }

  QVariant JSON::toVariant(const std::vector<std::pair<ErrorType, std::string> > &t_errors)
  {
    QVariantList qvl;
    
    for (std::vector<std::pair<ErrorType, std::string> >::const_iterator itr = t_errors.begin();
         itr != t_errors.end();
         ++itr)
    {
      QVariantMap qvm;
      qvm["errorType"] = toQString(itr->first.valueName());
      qvm["description"] = toQString(itr->second);
      qvl.push_back(qvm);
    }

    return qvl;
  }

  QVariant JSON::toVariant(const JobErrors &t_errors)
  {
    QVariantMap map;
    map["result"] = toQString(t_errors.result.valueName());
    map["allErrors"] = toVariant(t_errors.allErrors);

    QVariantMap parent;
    parent["JobErrors"] = map;
    return parent;
  }

  QVariant JSON::toVariant(const Job &t_jobTree)
  {
    QVariantMap map;
    map["type"] = toVariant(t_jobTree.jobType());
    map["files"] = toVariant(t_jobTree.rawInputFiles());
    map["params"] = toVariant(t_jobTree.params());
    map["tools"] = toVariant(t_jobTree.tools());
    map["uuid"] = QVariant(t_jobTree.uuid().toString());

    if (t_jobTree.lastRun())
    {
      map["lastRun"] = QVariant(toQString(t_jobTree.lastRun()->toString()));
    }

    map["outputFiles"] = toVariant(t_jobTree.outputFiles());
    map["errors"] = toVariant(t_jobTree.errors());

    map["children"] = toVariant(t_jobTree.children());

    if (t_jobTree.finishedJob())
    {
      map["finishedJob"] = toVariant(*t_jobTree.finishedJob());
    }

    QVariantMap parentMap;
    parentMap["Job"] = map;

    return QVariant(parentMap);
  }

  /// Updates the given job tree with the new values from the jsonString
  //        void JSON::updateFromJSON(Job &t_jobTree, const std::string &t_jsonString);

  /// \returns a JSON string representation of the WorkItem
  std::string JSON::toJSON(const WorkItem &t_workItem)
  {
    return toJSON(toVariant(t_workItem));    
  }

  QVariant JSON::toVariant(const WorkItem &t_workItem)
  {
    QVariantMap map;
    map["type"] = toVariant(t_workItem.type);
    map["tools"] = toVariant(t_workItem.tools.tools());
    map["params"] = toVariant(t_workItem.params.params());
    map["files"] = toVariant(t_workItem.files.files());
    map["jobkeyname"] = toQString(t_workItem.jobkeyname);
    
    QVariantMap parent;
    parent["WorkItem"] = map;
    return parent;
  }

  /// \returns a WorkItem from the given JSON string
  WorkItem JSON::toWorkItem(const std::string &t_json)
  {
    QJson::Parser parser;
    bool ok = false;
    QVariant variant = parser.parse(toQString(t_json).toUtf8(), &ok);

    if (ok)
    {
      return toWorkItem(variant);
    } else {
      throw std::runtime_error("Error parsing JSON: " + toString(parser.errorString()));
    }
  }

  WorkItem JSON::toWorkItem(const QVariant &t_variant)
  {
    QVariantMap map = t_variant.toMap()["WorkItem"].toMap();

    if (map.empty())
    {
      throw std::runtime_error("Unable to find WorkItem object at expected location");
    }

    return WorkItem(
        JobType(toString(map["type"].toString())),
        Tools(toVectorOfToolInfo(map["tools"])),
        JobParams(toVectorOfJobParam(map["params"])),
        Files(toVectorOfFileInfo(map["files"])),
        toString(map["jobkeyname"].toString())
        );
  }


  /// \returns a JSON string representation of the given job tree
  Job JSON::toJob(const std::string &t_json)
  {
    QJson::Parser parser;
    bool ok = false;
    QVariant variant = parser.parse(toQString(t_json).toUtf8(), &ok);

    if (ok)
    {
      return toJob(variant);
    } else {
      throw std::runtime_error("Error parsing JSON: " + toString(parser.errorString()));
    }
  }


  JobType JSON::toJobType(const QVariant &t_variant)
  {
    return JobType(toString(t_variant.toString()));
  }

  std::vector<std::pair<QUrl, openstudio::path> > JSON::toRequiredFiles(const QVariant &t_variant)
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

  FileInfo JSON::toFileInfo(const QVariant &t_variant)
  {
    QVariantMap map = t_variant.toMap()["FileInfo"].toMap();

    if (map.empty())
    {
      throw std::runtime_error("Unable to find FileInfo object at expected location");
    }

    FileInfo fi(
        toString(map["filename"].toString()),
        openstudio::DateTime(toString(map["lastModified"].toString())),
        toString(map["key"].toString()),
        toPath(map["fullPath"].toString()),
        map["exists"].toBool());
        
    fi.requiredFiles = toRequiredFiles(map["requiredFiles"]);

    return fi;
  }



  std::vector<FileInfo> JSON::toVectorOfFileInfo(const QVariant &t_variant)
  {
    std::vector<FileInfo> retval;
    QVariantList qvl = t_variant.toList();

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toFileInfo(*itr));
    }

    return retval;
  }

  JobParam JSON::toJobParam(const QVariant &t_variant)
  {
    QVariantMap qvm = t_variant.toMap()["JobParam"].toMap();

    if (qvm.empty())
    {
      throw std::runtime_error("Unable to find JobParam object at expected location");
    }

    JobParam param(toString(qvm["value"].toString()));
    param.children = toVectorOfJobParam(qvm["children"]);

    return param;
  }

  std::vector<JobParam> JSON::toVectorOfJobParam(const QVariant &t_variant)
  {
    QVariantList qvl = t_variant.toList();

    std::vector<JobParam> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toJobParam(*itr));
    }

    return retval;
  }

  ToolInfo JSON::toToolInfo(const QVariant &t_variant)
  {
    QVariantMap qvm = t_variant.toMap()["ToolInfo"].toMap();

    if (qvm.empty())
    {
      throw std::runtime_error("Unable to find ToolInfo object at expected location");
    }

    return ToolInfo(
        toString(qvm["name"].toString()),
        ToolVersion::fromString(toString(qvm["version"].toString())),
        toPath(qvm["localBinPath"].toString()),
        toPath(qvm["remoteArchive"].toString()),
        toPath(qvm["remoteExe"].toString()),
        boost::regex(toString(qvm["outFileFilter"].toString()))
      );
  }

  std::vector<ToolInfo> JSON::toVectorOfToolInfo(const QVariant &t_variant)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<ToolInfo> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toToolInfo(*itr));
    }

    return retval;
  }

  std::vector<Job> JSON::toVectorOfJob(const QVariant &t_variant)
  {
    QVariantList qvl = t_variant.toList();
    std::vector<Job> retval;

    for (QVariantList::const_iterator itr = qvl.begin();
         itr != qvl.end();
         ++itr)
    {
      retval.push_back(toJob(*itr));
    }

    return retval;
  }

  std::vector<std::pair<ErrorType, std::string> > JSON::toVectorOfError(const QVariant &t_variant)
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
            ErrorType(toString(qvm["errorType"].toString())),
            toString(qvm["description"].toString())
            )
          );
    }

    return retval;
  }

  JobErrors JSON::toJobErrors(const QVariant &t_variant)
  {
    
    QVariantMap map = t_variant.toMap()["JobErrors"].toMap();

    if (map.empty())
    {
      throw std::runtime_error("Unable to find JobErrors object at expected location");
    }

    return JobErrors(
          openstudio::ruleset::OSResultValue(toString(map["result"].toString())),
          toVectorOfError(map["allErrors"])
        );

  }

  Job JSON::toJob(const QVariant &t_variant)
  {
    QVariantMap map = t_variant.toMap()["Job"].toMap();

    if (map.empty())
    {
      throw std::runtime_error("Unable to find Job object at expected location");
    }

    Job job = JobFactory::createJob(
        JobType(toString(map["type"].toString())),
        Tools(toVectorOfToolInfo(map["tools"])),
        JobParams(toVectorOfJobParam(map["params"])),
        Files(toVectorOfFileInfo(map["files"])),
        std::vector<openstudio::URLSearchPath>(),
        false,
        map.contains("uuid")?openstudio::UUID(map["uuid"].toString()):boost::optional<openstudio::UUID>(),
        map.contains("lastRun")?openstudio::DateTime(toString(map["lastRun"].toString())):boost::optional<openstudio::DateTime>(),
        toJobErrors(map["errors"]),
        Files(toVectorOfFileInfo(map["outputFiles"]))
        );


    std::vector<Job> children = toVectorOfJob(map["children"]);

    for (std::vector<Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      job.addChild(*itr);
    }

    if (map.contains("finishedJob"))
    {
      Job finishedJob = toJob(map["finishedJob"]);
      job.setFinishedJob(finishedJob);
    }

    return job;
  }

}
}
}


