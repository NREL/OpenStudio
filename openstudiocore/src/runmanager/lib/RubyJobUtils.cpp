/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include "RubyJobUtils.hpp"
#include "RunManager.hpp"
#include "WorkItem.hpp"

#include <ruleset/OSArgument.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/bcl/BCLMeasure.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include <QDir>

namespace openstudio {
namespace runmanager {

RubyJobBuilder::RubyJobBuilder(bool t_userScriptJob)
  : m_userScriptJob(t_userScriptJob)
{}

RubyJobBuilder::RubyJobBuilder(const WorkItem &t_workItem)
  : m_userScriptJob(false)
{
  try {
    FileInfo fi = t_workItem.files.getLastByKey("rb");
    m_script = fi.fullPath;

    std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = fi.requiredFiles;

    for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
        itr != requiredFiles.end();
        ++itr)
    {
      m_requiredFiles.push_back(std::make_pair(toPath(itr->first.toLocalFile()), itr->second));
    }

  } catch (const std::exception &) {
    // carry on
  }


  initializeFromParams(t_workItem.params);
}

RubyJobBuilder::RubyJobBuilder(const JobParams &t_params)
  : m_userScriptJob(false)
{
  initializeFromParams(t_params);
}


/// Create from a BCLMeasure by:
///   Setting scriptFile to t_measure.primaryRubyScriptPath()
///   Calling addRequiredFile for each t_measure.files()
///   Calling addInputFile for appropriate t_measure.inputFileType()
///   Calling copyRequiredFiles for t_measure.inputFileType to t_measure.outputFileType()
RubyJobBuilder::RubyJobBuilder(const openstudio::BCLMeasure &t_measure,
    const std::vector<ruleset::OSArgument>& t_args,
      const openstudio::path &t_relativeTo,
      bool t_copyFileArguments)
  : m_userScriptJob(true)
{
  boost::optional<openstudio::path> script = t_measure.primaryRubyScriptPath();
  if (!script)
  {
    throw std::runtime_error("Passed in measure does not have a primaryRubyScriptPath set, which is required for RubyJobBuilder");
  }

  setAsUserScriptRubyJob(*script, t_args, t_relativeTo, t_copyFileArguments);

  std::vector<BCLFileReference> files = t_measure.files();

  for (std::vector<BCLFileReference>::const_iterator itr = files.begin();
       itr != files.end();
       ++itr)
  {
    addRequiredFile(itr->path(), itr->path().filename());
  }

  FileReferenceType infile = t_measure.inputFileType();

  if (infile != FileReferenceType::Unknown)
  {
    std::string outputname = "in." + infile.valueDescription();
    LOG(Info, "Adding input file of type: " + infile.valueName() + " to be copied to " + outputname);
    addInputFile(FileSelection::Last, FileSource::All, ".*\\." + infile.valueDescription(),  outputname);


    FileReferenceType outfile = t_measure.outputFileType();

    if (outfile != FileReferenceType::Unknown)
    {
      copyRequiredFiles(infile.valueDescription(), outfile.valueDescription(), std::string() );
      LOG(Info, "Copying all required files from " + infile.valueName() + " to " + outfile.valueName());
    } else {
      LOG(Error, "Output file type is \"Unknown\" unable to  copy required files from in to out.");
    }
  } else {
    LOG(Error, "Input file type is \"Unknown\" unable to add it.");
  }


}


void RubyJobBuilder::initializeFromParams(const JobParams &t_params)
{

  try {
    JobParam inputfiles = t_params.get("ruby_inputfiles");

    for (std::vector<JobParam>::const_iterator itr = inputfiles.children.begin();
         itr != inputfiles.children.end();
         ++itr)
    {
      try {
        m_inputfiles.push_back(
          boost::make_tuple(FileSelection(itr->value), FileSource(itr->children.at(0).value),
            itr->children.at(0).children.at(0).value,
            itr->children.at(0).children.at(0).children.at(0).value));
      } catch (const std::exception &) { }

    }

  } catch (const std::exception &) {}

  try {
    JobParam required = t_params.get("ruby_copyrequired");

    for (std::vector<JobParam>::const_iterator itr = required.children.begin();
         itr != required.children.end();
         ++itr)
    {
      try {
        m_copyRequiredFiles.push_back(
          boost::make_tuple(itr->value, itr->children.at(0).value,
            itr->children.at(0).children.at(0).value));
      } catch (const std::exception &) { }

    }

  } catch (const std::exception &) {}


  try {
    JobParam parameters = t_params.get("ruby_scriptparameters");

    for (std::vector<JobParam>::const_iterator itr = parameters.children.begin();
         itr != parameters.children.end();
         ++itr)
    {
      m_params.push_back( itr->value );
    }

  } catch (const std::exception &) {}

  try {
    JobParam parameters = t_params.get("ruby_toolparameters");

    for (std::vector<JobParam>::const_iterator itr = parameters.children.begin();
         itr != parameters.children.end();
         ++itr)
    {
      m_toolparams.push_back( itr->value );
    }

  } catch (const std::exception &) {}

  try {
    JobParam parameters = t_params.get("ruby_isuserscriptjob");

    std::string value = parameters.children.at(0).value;

    if (value == "true")
    {
      m_userScriptJob = true;
    }
  } catch (const std::exception &) {}
}


openstudio::path RubyJobBuilder::script() const {
  return m_script;
}

bool RubyJobBuilder::userScriptJob() const {
  return m_userScriptJob;
}

void RubyJobBuilder::setScriptFile(const openstudio::path &t_script)
{
  m_script = t_script;
}

std::vector< std::pair<openstudio::path, openstudio::path> > RubyJobBuilder::requiredFiles() const {
  return m_requiredFiles;
}

bool RubyJobBuilder::addRequiredFile(const openstudio::path& currentPath,
                                     const openstudio::path& copyPath,
                                     const openstudio::path& relativeTo)
{
  openstudio::path p = currentPath;

  if (!relativeTo.empty()) {
    p = boost::filesystem::complete(currentPath, relativeTo);
  }

  if (boost::filesystem::exists(p)) {
    m_requiredFiles.push_back(openstudio::PathPair(currentPath,copyPath));
    return true;
  }
  return false;
}

void RubyJobBuilder::copyRequiredFiles(const std::string &t_infileextension,
                                       const std::string &t_outextension,
                                       const std::string &t_requiredFilename)
{
  m_copyRequiredFiles.push_back(boost::make_tuple(t_infileextension, t_outextension, t_requiredFilename));
}

void RubyJobBuilder::addScriptParameter(const std::string &name, const std::string &value)
{
  m_params.push_back("--" + name + "=" + value);
}

void RubyJobBuilder::addScriptParameter(const std::string &name)
{
  m_params.push_back("--" + name);
}

void RubyJobBuilder::addScriptArgument(const std::string &name)
{
  m_params.push_back(name);
}

void RubyJobBuilder::setIncludeDir(const openstudio::path &value)
{
  if (!value.empty()){
    m_toolparams.push_back("-I");
    m_toolparams.push_back(toString(value));
  }
}

void RubyJobBuilder::addToolArgument(const std::string &name)
{
  m_toolparams.push_back(name);
}

void RubyJobBuilder::addInputFile(const FileSelection &t_fileselection, const FileSource &t_filesource,
    const std::string &t_inputregex, const std::string &t_outputname)
{
  m_inputfiles.push_back(boost::make_tuple(t_fileselection, t_filesource, t_inputregex, t_outputname));
}

WorkItem RubyJobBuilder::toWorkItem(const std::vector<openstudio::path> &t_requiredFiles) const
{
  Files files;
  std::set<openstudio::path> requiredfiles;

  if (!m_script.empty())
  {
    FileInfo fi(m_script, "rb");

    for (std::vector<openstudio::PathPair>::const_iterator itr = m_requiredFiles.begin();
         itr != m_requiredFiles.end();
         ++itr)
    {
      if (requiredfiles.count(itr->first) == 0)
      {
        requiredfiles.insert(itr->first);
        fi.addRequiredFile(itr->first, itr->second);
      } else {
        LOG(Info, "Skipping file, already exists as a required file: " << openstudio::toString(itr->first));
      }
    }

    for (std::vector<openstudio::path>::const_iterator fileitr = t_requiredFiles.begin();
        fileitr != t_requiredFiles.end();
        ++fileitr)
    {
      if (requiredfiles.count(*fileitr) == 0)
      {
        requiredfiles.insert(*fileitr);
        fi.addRequiredFile(*fileitr);
      } else {
        LOG(Info, "Skipping file, already exists as a required file: " << openstudio::toString(*fileitr));
      }
    }

    files.append(fi);
  }

  if (m_userScriptJob)
  {
    return runmanager::WorkItem(JobType::UserScript, Tools(), toParams(), files);
  } else {
    return runmanager::WorkItem(JobType::Ruby, Tools(), toParams(), files);
  }
}

void RubyJobBuilder::addToWorkflow(Workflow &t_wf, const std::vector<openstudio::path> &t_requiredFiles) const
{
  t_wf.addJob(toWorkItem(t_requiredFiles));
}

JobParams RubyJobBuilder::toParams() const
{
  JobParam inputfilelist("ruby_inputfiles");

  for(std::vector<boost::tuple<FileSelection, FileSource, std::string, std::string> >::const_iterator itr
         = m_inputfiles.begin();
      itr != m_inputfiles.end();
      ++itr)
  {
    JobParam fileselection(itr->get<0>().valueName());
    JobParam filesource(itr->get<1>().valueName());
    JobParam inputregex(itr->get<2>());
    JobParam outputname(itr->get<3>());

    inputregex.children.push_back(outputname);
    filesource.children.push_back(inputregex);
    fileselection.children.push_back(filesource);

    inputfilelist.children.push_back(fileselection);
  }

  JobParam copyrequiredfileslist("ruby_copyrequired");
  for (std::vector<boost::tuple<std::string, std::string, std::string> >::const_iterator itr
         = m_copyRequiredFiles.begin();
       itr != m_copyRequiredFiles.end();
       ++itr)
  {
    JobParam fileextension(itr->get<0>());
    JobParam fileoutextension(itr->get<1>());
    JobParam requiredextension(itr->get<2>());

    fileoutextension.children.push_back(requiredextension);
    fileextension.children.push_back(fileoutextension);

    copyrequiredfileslist.children.push_back(fileextension);
  }


  JobParam scriptparams("ruby_scriptparameters");

  for(std::vector<std::string>::const_iterator itr = m_params.begin();
      itr != m_params.end();
      ++itr)
  {
    JobParam p(*itr);
    scriptparams.children.push_back(p);
  }

  JobParam toolparams("ruby_toolparameters");

  for(std::vector<std::string>::const_iterator itr = m_toolparams.begin();
      itr != m_toolparams.end();
      ++itr)
  {
    JobParam p(*itr);
    toolparams.children.push_back(p);
  }


  JobParams params;
  params.append(scriptparams);
  params.append(inputfilelist);
  params.append(toolparams);
  params.append(copyrequiredfileslist);

  if (m_userScriptJob)
  {
    params.append("ruby_isuserscriptjob", "true");
  } else {
    params.append("ruby_isuserscriptjob", "false");
  }

  return params;
}

std::vector<std::string> RubyJobBuilder::getScriptParameters() const
{
  return m_params;
}

std::vector<std::string> RubyJobBuilder::getToolParameters() const
{
  return m_toolparams;
}

std::vector<boost::tuple<FileSelection, FileSource, std::string, std::string> > RubyJobBuilder::inputFiles() const
{
  return m_inputfiles;
}

std::vector<boost::tuple<std::string, std::string, std::string> > RubyJobBuilder::copyRequiredFiles() const
{
  return m_copyRequiredFiles;
}

void RubyJobBuilder::addJobsToWorkflow(
    Workflow &t_workflow,
    const std::vector<RubyJobBuilder> &t_jobs,
    const openstudio::path &t_includePath,
    const std::vector<openstudio::path> &t_requiredFiles)
{
  for (std::vector<RubyJobBuilder>::const_iterator itr = t_jobs.begin();
       itr != t_jobs.end();
       ++itr)
  {
    RubyJobBuilder job = *itr;
    job.setIncludeDir(t_includePath);

    job.addToWorkflow(t_workflow, t_requiredFiles);
  }
}

std::vector<RubyJobBuilder> RubyJobBuilder::createRubyJobsFromFolder(
    const openstudio::path &t_path,
    const std::vector<ruleset::UserScriptInfo> &t_userScripts,
    const std::string &t_inputExtension,
    const openstudio::path &t_relativePath,
    bool t_copyFileArguments)
{
  std::set<openstudio::path> ruby_files;

  if (!t_path.empty()
      && boost::filesystem::exists(t_path)
      && boost::filesystem::is_directory(t_path))
  {
    for(boost::filesystem::basic_directory_iterator<openstudio::path> itr(t_path);
        itr != boost::filesystem::basic_directory_iterator<openstudio::path>();
        ++itr)
    {
      openstudio::path p = itr->path();

      if (openstudio::toString(p.extension()) == ".rb")
      {
        ruby_files.insert(p);
      }
    }
  }

  std::vector<RubyJobBuilder> retval;

  for (std::set<openstudio::path>::const_iterator itr = ruby_files.begin();
       itr != ruby_files.end();
       ++itr)
  {
    RubyJobBuilder rjb;
    std::vector<ruleset::UserScriptInfo>::const_iterator it =
        std::find_if(t_userScripts.begin(),t_userScripts.end(),
                     boost::bind(firstOfPairEqual<openstudio::path,ruleset::OSArgumentVector>,_1,*itr));
    if (it != t_userScripts.end()) {
      rjb = createUserScriptRubyJob(it->first,it->second, t_relativePath, t_copyFileArguments);
    }
    else {
      rjb.setScriptFile(*itr);
    }
    if (t_inputExtension == "sql") {
      rjb.addInputFile(openstudio::runmanager::FileSelection("last"),
                       openstudio::runmanager::FileSource("All"),
                       ".*\\.osm",
                       "in.osm");
      rjb.addInputFile(openstudio::runmanager::FileSelection("last"),
                       openstudio::runmanager::FileSource("All"),
                       ".*\\.idf",
                       "in.idf");
      rjb.addInputFile(openstudio::runmanager::FileSelection("last"),
                       openstudio::runmanager::FileSource("All"),
                       "eplusout.sql",
                       "eplusout.sql");
    }
    else {
      rjb.addInputFile(openstudio::runmanager::FileSelection("last"),
                       openstudio::runmanager::FileSource("All"),
                       ".*\\." + t_inputExtension,
                       "in." + t_inputExtension);
      if (t_inputExtension == "idf") {
        rjb.copyRequiredFiles("idf", "idf", "in.epw");
      }
    }

    retval.push_back(rjb);
  }

  return retval;
}

std::string RubyJobBuilder::boolToString(bool t_v)
{
  return t_v?"true":"false";
}

bool RubyJobBuilder::stringToBool(const std::string &t_s)
{
  return t_s=="true";
}

JobParams RubyJobBuilder::toJobParams(const std::vector<ruleset::OSArgument> &t_args, const openstudio::path &t_basePath)
{
  JobParams params;

  for (std::vector<ruleset::OSArgument>::const_iterator itr = t_args.begin();
       itr != t_args.end();
       ++itr)
  {
    JobParams param;

    param.append("uuid", openstudio::toString(itr->uuid()));
    param.append("versionUUID", openstudio::toString(itr->versionUUID()));
    param.append("name", itr->name());
    param.append("displayName", itr->displayName());
    param.append("type", itr->type().valueName());
    param.append("required", boolToString(itr->required()));

    if (itr->hasValue())
    {
      if (itr->type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = itr->valueAsPath();
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        param.append("value", openstudio::toString(p));
      } else {
        param.append("value", itr->valueAsString());
      }
    }


    if (itr->hasDefaultValue())
    {
      if (itr->type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = itr->defaultValueAsPath();
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        param.append("defaultValue", openstudio::toString(p));
      } else {
        param.append("defaultValue", itr->defaultValueAsString());
      }
    }

    param.append("domainType", itr->domainType().valueName());

    std::vector<std::string> domainvec;
    if (itr->hasDomain())
    {
      domainvec = itr->domainAsString();
    }
    JobParams domain;
    for (std::vector<std::string>::const_iterator itr2 = domainvec.begin();
         itr2 != domainvec.end();
         ++itr2)
    {
      if (itr->type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = toPath(*itr2);
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        domain.append(openstudio::toString(p));
      } else {
        domain.append(*itr2);
      }
    }
    param.append("domain",domain);

    std::vector<std::string> choicesvec = itr->choiceValues();

    JobParams choices;
    for (std::vector<std::string>::const_iterator itr2 = choicesvec.begin();
         itr2 != choicesvec.end();
         ++itr2)
    {
      choices.append(*itr2);
    }
    param.append("choices", choices);

    std::vector<std::string> choiceDisplayNamesVec = itr->choiceValueDisplayNames();

    JobParams choiceDisplayNames;
    for (std::vector<std::string>::const_iterator itr2 = choiceDisplayNamesVec.begin();
         itr2 != choiceDisplayNamesVec.end();
         ++itr2)
    {
      choiceDisplayNames.append(*itr2);
    }
    param.append("choiceDisplayNames", choiceDisplayNames);

    param.append("isRead", boolToString(itr->isRead()));
    param.append("extension", itr->extension());

    params.append(itr->name(), param);
  }

  JobParams p;
  p.append("user_script_params", params);
  return p;
}

std::vector<ruleset::OSArgument> RubyJobBuilder::toOSArguments(const JobParams &t_params, const openstudio::path &t_basePath)
{
  std::vector<ruleset::OSArgument> retval;

  std::vector<JobParam> args = t_params.get("user_script_params").children;

  LOG(Debug, "Params found: " << args.size());
  for (std::vector<JobParam>::const_iterator itr = args.begin();
       itr != args.end();
       ++itr)
  {
    try {
      JobParams arg(itr->children);
      openstudio::UUID uuid = openstudio::toUUID(arg.get("uuid").children.at(0).value);
      openstudio::UUID versionUUID = openstudio::toUUID(arg.get("versionUUID").children.at(0).value);
      std::string name = arg.get("name").children.at(0).value;
      std::string displayName = arg.get("displayName").children.at(0).value;

      LOG(Debug, "Setting OSArgumentType " << arg.get("type").children.at(0).value);
      openstudio::ruleset::OSArgumentType type(arg.get("type").children.at(0).value);
      bool required = stringToBool(arg.get("required").children.at(0).value);
      boost::optional<std::string> value;
      try {
        if (type == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
        {
          openstudio::path p = toPath(arg.get("value").children.at(0).value);
          value = toString(boost::filesystem::complete(p, t_basePath));
        } else {
          value = arg.get("value").children.at(0).value;
        }
      } catch (...) {}

      boost::optional<std::string> defaultValue;
      try {
        defaultValue = arg.get("defaultValue").children.at(0).value;
      } catch (...) {}

      openstudio::ruleset::OSDomainType domainType(arg.get("domainType").children.at(0).value);
      std::vector<JobParam> domainvec = arg.get("domain").children;
      std::vector<std::string> domain;

      for (std::vector<JobParam>::const_iterator itr2 = domainvec.begin();
          itr2 != domainvec.end();
          ++itr2)
      {
        domain.push_back(itr2->value);
      }

      std::vector<JobParam> choicesvec = arg.get("choices").children;
      std::vector<std::string> choices;

      for (std::vector<JobParam>::const_iterator itr2 = choicesvec.begin();
          itr2 != choicesvec.end();
          ++itr2)
      {
        choices.push_back(itr2->value);
      }

      std::vector<JobParam> choiceDisplayNamesVec = arg.get("choiceDisplayNames").children;
      std::vector<std::string> choiceDisplayNames;

      for (std::vector<JobParam>::const_iterator itr2 = choiceDisplayNamesVec.begin();
          itr2 != choiceDisplayNamesVec.end();
          ++itr2)
      {
        choiceDisplayNames.push_back(itr2->value);
      }

      bool isRead = stringToBool(arg.get("isRead").children.at(0).value);
      std::string extension = arg.get("extension").children.at(0).value;

      retval.push_back(
          openstudio::ruleset::OSArgument(
              uuid,
              versionUUID,
              name,
              displayName,
              type,
              required,
              value,
              defaultValue,
              domainType,
              domain,
              choices,
              choiceDisplayNames,
              isRead,
              extension)
          );
    } catch (const std::exception &e) {
      LOG(Error, "Error loading job param: " << e.what());
    }
  }

  return retval;
}

void RubyJobBuilder::setAsUserScriptRubyJob(const openstudio::path& t_userScriptPath,
                                            const std::vector<ruleset::OSArgument>& t_args,
                                            const openstudio::path &t_relativeTo,
                                            bool t_copyFileArguments)
{
  m_userScriptJob = true;

  // Add user script as input file to be copied to job run directory/user_script.rb
  openstudio::path userScriptPath = boost::filesystem::system_complete(t_userScriptPath);

  if (!t_relativeTo.empty())
  {
    openstudio::path relative = openstudio::relativePath(userScriptPath, t_relativeTo);
    if (!relative.empty())
    {
      userScriptPath = relative;
    }
  }

  // Main script is UserScriptAdapter.rb
  openstudio::path adapterPath = toPath("openstudio/runmanager/rubyscripts/UserScriptAdapter.rb");
  setScriptFile(getOpenStudioRubyScriptsPath() / adapterPath);

  addRequiredFile(userScriptPath,toPath("user_script.rb"), t_relativeTo);

  // add arguments in -n, -v, name, value pairs
  BOOST_FOREACH(const ruleset::OSArgument& argument,t_args) {
    std::string argumentValue = argument.printValue(true);

    if (argument.type() == openstudio::ruleset::OSArgumentType::Path
        && t_copyFileArguments)
    {
      openstudio::path argpath = argument.valueAsPath();

      if (!t_relativeTo.empty())
      {
        openstudio::path relative = openstudio::relativePath(argpath, t_relativeTo);
        if (!relative.empty())
        {
          argpath = relative;
        }

      }

      argumentValue = openstudio::toString(argpath.filename());
      addRequiredFile(argpath, argpath.filename(), t_relativeTo);
    }

    if (!argumentValue.empty()) {
      addScriptParameter("argumentName",argument.name());
      addScriptParameter("argumentValue",argumentValue);
    }
  }


}

RubyJobBuilder RubyJobBuilder::createUserScriptRubyJob(const openstudio::path& t_userScriptPath,
                                                       const std::vector<ruleset::OSArgument>& t_args,
                                                       const openstudio::path &t_relativeTo,
                                                       bool t_copyFileArguments)
{
  RubyJobBuilder result(true);
  result.setAsUserScriptRubyJob(t_userScriptPath, t_args, t_relativeTo, t_copyFileArguments);
  return result;
}



runmanager::ScriptInfo RubyJobBuilder::updateArgumentsFromDb(runmanager::ScriptInfo t_info) {
  LOG(Debug, "updateArgumentsFromDb() called.");
  if (boost::filesystem::exists(t_info.dbPath))
  {
    LOG(Debug, "updateArgumentsFromDb, opening " << openstudio::toString(t_info.dbPath));
    openstudio::runmanager::RunManager rm(t_info.dbPath);

    std::vector<runmanager::Workflow> workflows = rm.loadWorkflows();
    LOG(Debug, "updateArgumentsFromDb, workflows found " << workflows.size());

    if (workflows.size() == 1u)
    {
      t_info.arguments.clear();

      if (workflows[0].type() == runmanager::JobType::Ruby)
      {
        t_info.isUserScript = false;
        throw runmanager::ScriptDetectionError("ExtractOSArguments.rb found that this script is not a formal user script, that is, not a Ruby class deriving from OpenStudio::Ruleset::ModelUserScript or OpenStudio::Ruleset::WorkspaceUserScript. If this is undesired behavior, please edit your script and try again by setting the 'Formal User Script' button to 'Yes'.", t_info);
      } else {
        LOG(Debug, "Unpacking user script arguments");
        ruleset::OSArgumentVector args = runmanager::RubyJobBuilder::toOSArguments(workflows[0].params(), t_info.scriptPath.parent_path().parent_path());
        LOG(Debug, "User script arguments found: " << args.size());
        BOOST_FOREACH(const ruleset::OSArgument& arg,args) {
          LOG(Debug, "Loading user script argument: " << arg.name());
          t_info.arguments[arg.name()] = arg;
          if ((!arg.hasValue()) &&
              (arg.type() == ruleset::OSArgumentType::Choice) &&
              (arg.required()))
          {
            if (arg.hasDefaultValue())
            {
              t_info.arguments[arg.name()].setValue(arg.defaultValueAsString());
              t_info.argsChanged = true;
            } else {
              std::vector<std::string> vals = arg.choiceValues();
              if (!vals.empty()) {
                t_info.arguments[arg.name()].setValue(vals[0]);
                t_info.argsChanged = true;
              }
            }
          }
        }
      }
      t_info.detectionSucceeded = true;
      return t_info;
    } else {
      t_info.detectionSucceeded = false;
      throw runmanager::ScriptDetectionError(
                               "The file at " + toString(t_info.dbPath) + " could not be opened or " +
                               "contains unexpected data. Please try recreating your user script " +
                               "arguments. You may need to delete the osp file, or even remove the " +
                               "parent rb and all related files, first.", t_info);

    }
  } else {
    // no db file--probably not an rb file at all.
    t_info.isUserScript = false;
    t_info.arguments.clear();
    t_info.detectionSucceeded = false;
    return t_info;
  }
}

runmanager::ScriptInfo RubyJobBuilder::saveArgumentsToDb(runmanager::ScriptInfo t_scriptInfo) {
  if (t_scriptInfo.isUserScript && t_scriptInfo.argsChanged) {
    if (boost::filesystem::exists(t_scriptInfo.dbPath)) {
      LOG(Debug,"Saving arguments to " << toString(t_scriptInfo.dbPath) << ".");
      runmanager::RunManager rm(t_scriptInfo.dbPath, false, false, false);

      std::vector<runmanager::Workflow> workflows = rm.loadWorkflows();
      if (workflows.size() == 1) {
        rm.deleteWorkflows();
        runmanager::Workflow wf("UserScript");

        std::vector<ruleset::OSArgument> args;
        for (std::map<std::string, ruleset::OSArgument>::const_iterator itr = t_scriptInfo.arguments.begin();
             itr != t_scriptInfo.arguments.end();
             ++itr)
        {
          args.push_back(itr->second);
        }

        wf.add(openstudio::runmanager::RubyJobBuilder::toJobParams(args, t_scriptInfo.scriptPath.parent_path().parent_path()));
        rm.persistWorkflow(wf);
        t_scriptInfo.argsChanged = false;
      } else {
        throw runmanager::ScriptDetectionError("The file at " + toString(t_scriptInfo.dbPath) + " contains unexpected data. Please refresh your user script arguments and try to save your model again.", t_scriptInfo);
      }
    }
  }

  return t_scriptInfo;
}


runmanager::ScriptInfo RubyJobBuilder::refreshArgumentsFromScript(runmanager::RunManager &t_rm, runmanager::ScriptInfo t_scriptInfo) {

  // set up run manager for running
  t_rm.setPaused(true);
  std::vector<openstudio::runmanager::Job> jobs = t_rm.getJobs();
  for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
      itr != jobs.end();
      ++itr)
  {
    t_rm.remove(*itr);
  }

  // set up ruby job
  openstudio::path outdir = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("OpenStudioGetUserScriptArguments");
  boost::filesystem::remove_all(outdir);
  boost::filesystem::create_directories(outdir);
  runmanager::RubyJobBuilder rubyJob;
  openstudio::path argExtractorPath = toPath("openstudio/runmanager/rubyscripts/ExtractUserScriptArguments.rb");
  rubyJob.setScriptFile(getOpenStudioRubyScriptsPath()/argExtractorPath);
  rubyJob.addScriptParameter("userScript", openstudio::toString(t_scriptInfo.scriptPath));
  runmanager::Workflow wf;
  rubyJob.setIncludeDir(getOpenStudioRubyIncludePath());
  rubyJob.addToWorkflow(wf);
  runmanager::Tools tools = t_rm.getConfigOptions().getTools();
  wf.add(tools);

  // run ruby job
  runmanager::Job j = wf.create(outdir);
  t_rm.enqueue(j,true);
  t_rm.setPaused(false);
  LOG(Debug, "Waiting for ExtractUserScriptArguments to finish");
  t_rm.waitForFinished();

  // extract arguments
  try {
    t_scriptInfo = updateArgumentsFromDb(t_scriptInfo);

    if (!t_scriptInfo.detectionSucceeded)
    {
      throw runmanager::ScriptDetectionError("Expected to create and retrieve a file at " +
                                             toString(t_scriptInfo.dbPath) +
                                             " using ExtractUserScriptArguments.rb, but the db file could not be located at the expected time.",
                                             t_scriptInfo);
    }

    return t_scriptInfo;
  } catch (...) {
    boost::filesystem::remove(t_scriptInfo.dbPath);
    throw;
  }
}



}
}
