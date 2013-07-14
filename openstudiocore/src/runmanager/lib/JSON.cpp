#include "JSON.hpp"

#include "Job.hpp"
#include "WorkItem.hpp"

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace openstudio {
namespace runmanager {

  Job JSON::toJob(const QVariant &t_variant)
  {
    /*
    QVariantMap map = t_variant.toMap();

    boost::optional<QUuid> uuid = map.contains("uuid")?map["uuid"].toUuid():boost::optional<QUuid>();

    Tools tools = map["tools"];
    Files files = map["files"];
    Params params = map["params"];
    JobErrors errors = maps["errors"];
    QString 
    */

    /*
    QVariantMap map = t_variant.toMap();
    QVariantMap obj = map["Job"].toMap();

    Tools tools = toTools(obj["tools"]);
    Files files = toFiles(obj["files"]);
    Params params = toParams(obj["params"]);

    JobErrors errors = toErrors(obj["errors"]);
    */
  }

  /// \returns a job tree created from the passed in json string
  Job JSON::toJob(const std::string &t_jsonString)
  {
    /*
    QJson::Parser parser;

    bool ok = false;
    QVariantMap result = parser.parse(openstudio::toQString(t_jsonString), &ok);

    if (!ok)
    {
      throw std::runtime_error("Error parsing JSON string: " + openstudio::toString(parser.errorString()));
    }

    return toJob(result);
    */
  }


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
    map["files"] = toVariant(t_jobTree.inputFiles());
    map["params"] = toVariant(t_jobTree.params());
    map["tools"] = toVariant(t_jobTree.tools());
    map["uuid"] = QVariant(t_jobTree.uuid());

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
    
  }

  /// \returns a WorkItem from the given JSON string
  WorkItem JSON::toWorkitem(const std::string &t_jsonString)
  {
  }

}
}


