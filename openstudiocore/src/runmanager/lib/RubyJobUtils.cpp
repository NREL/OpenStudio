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

#include "RubyJobUtils.hpp"
#include "RunManager.hpp"
#include "WorkItem.hpp"
#include "JSON.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/bcl/BCLMeasure.hpp"

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

    for (const auto & requiredFile : requiredFiles)
    {
      m_requiredFiles.push_back(std::make_pair(toPath(requiredFile.first.toLocalFile()), requiredFile.second));
    }

  } catch (const std::exception &) {
    // carry on
  }


  initializeFromParams(t_workItem.params);
}

/// Construct a RubyJobBuilder from a json file
RubyJobBuilder::RubyJobBuilder(const openstudio::path &t_json)
  : m_userScriptJob(false)
{
  initializeFromParams(openstudio::runmanager::detail::JSON::toVectorOfJobParam(t_json));
}

RubyJobBuilder::RubyJobBuilder(const WorkItem &t_workItem, 
                               const openstudio::path& t_originalBasePath,
                               const openstudio::path& t_newBasePath)
  : m_userScriptJob(false), m_jobkeyname(t_workItem.jobkeyname)
{
  try {
    FileInfo fi = t_workItem.files.getLastByKey("rb");
    m_script = fi.fullPath;

    /// \todo this is a bit of a hack, but it works for now
    if (toString(m_script.filename()) == "UserScriptAdapter.rb") {
      openstudio::path adapterPath = toPath("openstudio/runmanager/rubyscripts/UserScriptAdapter.rb");
      LOG(Debug, "Updating path of UserScriptAdapter.rb to " << openstudio::toString(adapterPath));
      setScriptFile(getOpenStudioRubyScriptsPath() / adapterPath);
    } else if (toString(m_script.filename()) == "DaylightCalculations.rb") {
      const std::string jobkeyname = t_workItem.jobkeyname;
      RubyJobBuilder rjb(t_workItem);
      LOG(Info, "Attempting to rebuild radiance job");

      setScriptFile(getOpenStudioRubyScriptsPath() / toPath("openstudio/radiance/DaylightCalculations.rb"));


      openstudio::path radianceLocation;

      // check old radiance location
      std::vector<std::string> params = rjb.getScriptParameters();
      if (params.size() > 1)
      {
        openstudio::path possibleRadianceLocation = openstudio::toPath(params[1]);
        if (boost::filesystem::exists(possibleRadianceLocation))
        {
          LOG(Info, "Radiance found at path in script params: " << openstudio::toString(possibleRadianceLocation));
          radianceLocation = possibleRadianceLocation;
        } else {
          LOG(Info, "Radiance NOT found at path in script params: " << openstudio::toString(possibleRadianceLocation));
        }
      }

      // check current settings for runmanager for current application
      if (radianceLocation.empty())
      {
        runmanager::ConfigOptions co(true);

        std::vector<ToolInfo> loc = co.getTools().getAllByName("rad").tools();

        if (!loc.empty())
        {
          openstudio::path possibleRadianceLocation = loc[0].localBinPath.parent_path();
          if (boost::filesystem::exists(possibleRadianceLocation))
          {
            LOG(Info, "Radiance found at path set by configoptions object set for current application " << openstudio::toString(possibleRadianceLocation));
            radianceLocation = possibleRadianceLocation;
          } else {
            LOG(Info, "Radiance NOT found at path set by configoptions object set for current application " << openstudio::toString(possibleRadianceLocation));
          }
        }
      }

      // use the toolfinder to find it quickly 
      if (radianceLocation.empty())
      {
        runmanager::ConfigOptions co(true);
        co.fastFindRadiance();

        std::vector<ToolInfo> loc = co.getTools().getAllByName("rad").tools();

        if (!loc.empty())
        {
          openstudio::path possibleRadianceLocation = loc[0].localBinPath.parent_path();
          if (boost::filesystem::exists(possibleRadianceLocation))
          {
            LOG(Info, "Radiance found at path set by configoptions object after fastFindRadiance " << openstudio::toString(possibleRadianceLocation));
            radianceLocation = possibleRadianceLocation;
          } else {
            LOG(Info, "Radiance NOT found at path set by configoptions object after fastFindRadiance " << openstudio::toString(possibleRadianceLocation));
          }
        }
      }

      // something went wrong, just use the known default locations
      if (radianceLocation.empty())
      {
        LOG(Info, "Radiance path being set for OS default ");
#ifdef Q_OS_WIN32
        radianceLocation = openstudio::toPath("C:\\Program Files (x86)\\Radiance\bin");
#else
        radianceLocation = openstudio::toPath("/usr/local/radiance/bin");
#endif
      }

      WorkItem rebuiltRadianceJob = Workflow::radianceDaylightCalculations(getOpenStudioRubyIncludePath(), radianceLocation);
      rebuiltRadianceJob.jobkeyname = jobkeyname;
      m_jobkeyname = jobkeyname;

      LOG(Info, "Initializing from rebuiltRadianceJob.params, JSON Source: " << rebuiltRadianceJob.toJSON());
      initializeFromParams(rebuiltRadianceJob.params, t_originalBasePath, t_newBasePath);
      LOG(Info, "setting ruby include dir: " << openstudio::toString(getOpenStudioRubyIncludePath()));
      clearIncludeDir();
      setIncludeDir(getOpenStudioRubyIncludePath());
      LOG(Info, "Rebuild of radiance job successful, returning");

      return;
    } else {
      openstudio::path temp = relocatePath(m_script,t_originalBasePath,t_newBasePath);
      if (!temp.empty()) {
        m_script = temp;
      }
    }

    std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = fi.requiredFiles;

    for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
        itr != requiredFiles.end();
        ++itr)
    {
      openstudio::path source = toPath(itr->first.toLocalFile());
      openstudio::path temp = relocatePath(source, t_originalBasePath, t_newBasePath);
      if (!temp.empty()) {
        source = temp;
      }
      m_requiredFiles.push_back(std::make_pair(source, itr->second));
    }



  } catch (const std::exception &e) {
    LOG(Info, "Exception occurred while building job: " << e.what());
    // carry on
  }

  initializeFromParams(t_workItem.params,t_originalBasePath,t_newBasePath);
  if (userScriptJob()) {
    clearIncludeDir();
    setIncludeDir(getOpenStudioRubyIncludePath());
  }
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
                               bool t_copyFileTrue)
  : m_userScriptJob(true)
{
  constructFromBCLMeasure(t_measure,t_args,t_relativeTo,t_copyFileTrue);
}

RubyJobBuilder::RubyJobBuilder(const openstudio::BCLMeasure &t_measure,
                               const std::map<std::string,ruleset::OSArgument>& t_args,
                               const openstudio::path &t_relativeTo,
                               bool t_copyFileTrue)
  : m_userScriptJob(true)
{
  ruleset::OSArgumentVector argVector;
  for (const auto & t_arg : t_args)
  {
    argVector.push_back(t_arg.second);
  }
  constructFromBCLMeasure(t_measure,argVector,t_relativeTo,t_copyFileTrue);
}

const std::vector<RubyJobBuilder> &RubyJobBuilder::mergedJobs() const
{
  return m_mergedJobs;
}

void RubyJobBuilder::initializeFromParams(const JobParams &t_params,
                                          const openstudio::path& t_originalBasePath,
                                          const openstudio::path& t_newBasePath)
{

  if (t_params.has("merged_ruby_jobs"))
  {
    size_t i = 0;

    JobParams merged(t_params.get("merged_ruby_jobs").children);
    while (merged.has(boost::lexical_cast<std::string>(i)))
    {
      JobParams job(merged.get(boost::lexical_cast<std::string>(i)).children);
      m_mergedJobs.push_back(RubyJobBuilder(job));
      ++i;
    }
  }

  try {
    JobParam uuid = t_params.get("original_job_uuid");
    m_originalUUID = openstudio::toUUID(uuid.children.at(0).value);
  } catch (const std::exception &) {}

  try {
    JobParam inputfiles = t_params.get("ruby_inputfiles");

    for (const auto & child : inputfiles.children)
    {
      try {
        m_inputfiles.push_back(
          std::make_tuple(FileSelection(child.value), FileSource(child.children.at(0).value),
            child.children.at(0).children.at(0).value,
            child.children.at(0).children.at(0).children.at(0).value));
      } catch (const std::exception &) { }

    }

  } catch (const std::exception &) {}

  try {
    JobParam required = t_params.get("ruby_copyrequired");

    for (const auto & child : required.children)
    {
      try {
        m_copyRequiredFiles.push_back(
          std::make_tuple(child.value, child.children.at(0).value,
            child.children.at(0).children.at(0).value));
      } catch (const std::exception &) { }

    }

  } catch (const std::exception &) {}


  try {
    JobParam parameters = t_params.get("ruby_scriptparameters");

    for (const auto & child : parameters.children)
    {
      m_params.push_back( child.value );
    }

  } catch (const std::exception &) {}

  try {
    JobParam parameters = t_params.get("ruby_toolparameters");

    for (const auto & child : parameters.children)
    {
      m_toolparams.push_back( child.value );
    }

  } catch (const std::exception &) {}

  try {
    JobParam parameters = t_params.get("ruby_requiredfiles");

    for (const auto & child : parameters.children)
    {
      openstudio::path source = openstudio::toPath(child.value);
      openstudio::path destination = openstudio::toPath(child.children.at(0).value);
      // in constructor from WorkItem, files listed here may have already been added, so see if there already
      std::vector<std::pair<openstudio::path,openstudio::path> >::const_iterator finderIt = 
          std::find_if(m_requiredFiles.begin(),
                       m_requiredFiles.end(),
                       std::bind(secondOfPairEqual<openstudio::path,openstudio::path>,std::placeholders::_1,destination));
      if (finderIt == m_requiredFiles.end()) {
        if (!(t_originalBasePath.empty() || t_newBasePath.empty())) {
          openstudio::path temp = relocatePath(source,t_originalBasePath,t_newBasePath);
          if (!temp.empty()) {
            source = temp;
          }
        }
        m_requiredFiles.push_back(std::make_pair(source, destination));
      }
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

  if (m_script.empty()) {
    try {
      JobParam parameters = t_params.get("ruby_scriptfile");
      m_script = openstudio::toPath(parameters.children.at(0).value);
    } catch (const std::exception &) {}
  }

  try {
    JobParam parameters = t_params.get("ruby_bclmeasureparameters");

    for (const auto & child : parameters.children)
    {
      if (child.value == "bcl_measure_uuid"){
        openstudio::UUID bclMeasureUUID = openstudio::toUUID(child.children.at(0).value);
        m_bclMeasureUUID = bclMeasureUUID;
      }
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
                                     const openstudio::path& relativeTo,
                                     bool verifyExistence)
{
  openstudio::path p = currentPath;

  if (!relativeTo.empty()) {
    p = boost::filesystem::complete(currentPath, relativeTo);
  }

  LOG(Trace, "addRequiredFile: " << openstudio::toString(currentPath) << " to " << openstudio::toString(copyPath) << " relative to: " << openstudio::toString(relativeTo));
  if (!verifyExistence) 
  {
    LOG(Trace, "addRequiredFile: not checking file existence " << openstudio::toString(p));
    m_requiredFiles.push_back(openstudio::PathPair(currentPath,copyPath));
    return true;
  }

  if (boost::filesystem::exists(p)) {
    LOG(Trace, "addRequiredFile: file exists " << openstudio::toString(p));
    m_requiredFiles.push_back(openstudio::PathPair(currentPath,copyPath));
    return true;
  } else {
    LOG(Trace, "addRequiredFile: file does not exist " << openstudio::toString(p));
    return false;
  }

}

void RubyJobBuilder::copyRequiredFiles(const std::string &t_infileextension,
                                       const std::string &t_outextension,
                                       const std::string &t_requiredFilename)
{
  m_copyRequiredFiles.push_back(std::make_tuple(t_infileextension, t_outextension, t_requiredFilename));
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

void RubyJobBuilder::clearIncludeDir() {
  auto it = m_toolparams.begin();
  while (it != m_toolparams.end()) {
    if (*it == std::string("-I")) {
      it = m_toolparams.erase(it); // erase -I
      OS_ASSERT(it != m_toolparams.end());
      it = m_toolparams.erase(it); // and path it was pointing to
    }
    else {
      ++it;
    }
  }
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
  m_inputfiles.push_back(std::make_tuple(t_fileselection, t_filesource, t_inputregex, t_outputname));
}

WorkItem RubyJobBuilder::toWorkItem(const std::vector<openstudio::path> &t_requiredFiles) const
{
  Files files;
  std::set<openstudio::path> requiredfiles;

  if (!m_script.empty())
  {
    FileInfo fi(m_script, "rb");

    for (const auto & requiredFile : m_requiredFiles)
    {
      if (requiredfiles.count(requiredFile.first) == 0)
      {
        requiredfiles.insert(requiredFile.first);
        fi.addRequiredFile(requiredFile.first, requiredFile.second);
      } else {
        LOG(Info, "Skipping file, already exists as a required file: " << openstudio::toString(requiredFile.first));
      }
    }

    for (const auto & requiredFile : t_requiredFiles)
    {
      if (requiredfiles.count(requiredFile) == 0)
      {
        requiredfiles.insert(requiredFile);
        fi.addRequiredFile(requiredFile);
      } else {
        LOG(Info, "Skipping file, already exists as a required file: " << openstudio::toString(requiredFile));
      }
    }

    files.append(fi);
  }

  if (m_userScriptJob)
  {
    return runmanager::WorkItem(JobType::UserScript, Tools(), toParams(), files, m_jobkeyname);
  } else {
    return runmanager::WorkItem(JobType::Ruby, Tools(), toParams(), files, m_jobkeyname);
  }
}

void RubyJobBuilder::addToWorkflow(Workflow &t_wf, const std::vector<openstudio::path> &t_requiredFiles) const
{
  t_wf.addJob(toWorkItem(t_requiredFiles));
}

JobParams RubyJobBuilder::toParams() const
{

 
  JobParam inputfilelist("ruby_inputfiles");

  for(const auto & inputFile : m_inputfiles)
  {
    JobParam fileselection(std::get<0>(inputFile).valueName());
    JobParam filesource(std::get<1>(inputFile).valueName());
    JobParam inputregex(std::get<2>(inputFile));
    JobParam outputname(std::get<3>(inputFile));

    inputregex.children.push_back(outputname);
    filesource.children.push_back(inputregex);
    fileselection.children.push_back(filesource);

    inputfilelist.children.push_back(fileselection);
  }

  JobParam copyrequiredfileslist("ruby_copyrequired");
  for (const auto & copyRequiredFile : m_copyRequiredFiles)
  {
    JobParam fileextension(std::get<0>(copyRequiredFile));
    JobParam fileoutextension(std::get<1>(copyRequiredFile));
    JobParam requiredextension(std::get<2>(copyRequiredFile));

    fileoutextension.children.push_back(requiredextension);
    fileextension.children.push_back(fileoutextension);

    copyrequiredfileslist.children.push_back(fileextension);
  }


  JobParam scriptparams("ruby_scriptparameters");

  for(const auto &  param : m_params)
  {
    JobParam p(param);
    scriptparams.children.push_back(p);
  }

  JobParam toolparams("ruby_toolparameters");
  for(const auto & toolParam : m_toolparams)
  {
    JobParam p(toolParam);
    toolparams.children.push_back(p);
  }

  JobParam requiredFiles("ruby_requiredfiles");

  for (const auto & requiredFile : m_requiredFiles)
  {
    JobParam p(openstudio::toString(requiredFile.first));
    p.children.push_back(JobParam(openstudio::toString(requiredFile.second)));
    requiredFiles.children.push_back(p);
  }

  JobParams params;
  params.append(scriptparams);
  params.append(inputfilelist);
  params.append(toolparams);
  params.append(copyrequiredfileslist);
  params.append(requiredFiles);

  if (m_originalUUID)
  {
    params.append("original_job_uuid", openstudio::toString(*m_originalUUID));
  }


  if (m_userScriptJob)
  {
    params.append("ruby_isuserscriptjob", "true");
  } else {
    params.append("ruby_isuserscriptjob", "false");
  }

  if (!m_script.empty())
  {
    params.append("ruby_scriptfile", openstudio::toString(m_script));
  }

  if (m_bclMeasureUUID){
    JobParam bclmeasureparams("ruby_bclmeasureparameters");
    JobParam p("bcl_measure_uuid");
    p.children.push_back(openstudio::toString(*m_bclMeasureUUID));
    bclmeasureparams.children.push_back(p);
    params.append(bclmeasureparams);
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

std::vector<std::tuple<FileSelection, FileSource, std::string, std::string> > RubyJobBuilder::inputFiles() const
{
  return m_inputfiles;
}

boost::optional<openstudio::UUID> RubyJobBuilder::originalUUID() const
{
  return m_originalUUID;
}

boost::optional<openstudio::UUID> RubyJobBuilder::bclMeasureUUID() const {
  return m_bclMeasureUUID;
}

std::vector<std::tuple<std::string, std::string, std::string> > RubyJobBuilder::copyRequiredFiles() const
{
  return m_copyRequiredFiles;
}

void RubyJobBuilder::addJobsToWorkflow(
    Workflow &t_workflow,
    const std::vector<RubyJobBuilder> &t_jobs,
    const openstudio::path &t_includePath,
    const std::vector<openstudio::path> &t_requiredFiles)
{
  for (const auto & elem : t_jobs)
  {
    RubyJobBuilder job = elem;
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
    for(boost::filesystem::directory_iterator itr(t_path);
        itr != boost::filesystem::directory_iterator();
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

  for (const auto & rubyFile : ruby_files)
  {
    RubyJobBuilder rjb;
    auto it = std::find_if(t_userScripts.begin(),t_userScripts.end(),
                           std::bind(firstOfPairEqual<openstudio::path,ruleset::OSArgumentVector>,std::placeholders::_1,rubyFile));
    if (it != t_userScripts.end()) {
      rjb = createUserScriptRubyJob(it->first,it->second, t_relativePath, t_copyFileArguments);
    }
    else {
      rjb.setScriptFile(rubyFile);
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

  for (const auto & arg : t_args)
  {
    JobParams param;

    param.append("uuid", openstudio::toString(arg.uuid()));
    param.append("versionUUID", openstudio::toString(arg.versionUUID()));
    param.append("name", arg.name());
    param.append("displayName", arg.displayName());
    param.append("type", arg.type().valueName());
    param.append("required", boolToString(arg.required()));

    if (arg.hasValue())
    {
      if (arg.type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = arg.valueAsPath();
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        param.append("value", openstudio::toString(p));
      } else {
        param.append("value", arg.valueAsString());
      }
    }


    if (arg.hasDefaultValue())
    {
      if (arg.type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = arg.defaultValueAsPath();
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        param.append("defaultValue", openstudio::toString(p));
      } else {
        param.append("defaultValue", arg.defaultValueAsString());
      }
    }

    param.append("domainType", arg.domainType().valueName());

    std::vector<std::string> domainvec;
    if (arg.hasDomain())
    {
      domainvec = arg.domainAsString();
    }
    JobParams domain;
    for (const auto & dom : domainvec)
    {
      if (arg.type() == openstudio::ruleset::OSArgumentType::Path && !t_basePath.empty())
      {
        openstudio::path p = toPath(dom);
        openstudio::path relative = openstudio::relativePath(p, t_basePath);

        if (!relative.empty())
        {
          p = relative;
        }

        domain.append(openstudio::toString(p));
      } else {
        domain.append(dom);
      }
    }
    param.append("domain",domain);

    std::vector<std::string> choicesvec = arg.choiceValues();

    JobParams choices;
    for (const auto & choice : choicesvec)
    {
      choices.append(choice);
    }
    param.append("choices", choices);

    std::vector<std::string> choiceDisplayNamesVec = arg.choiceValueDisplayNames();

    JobParams choiceDisplayNames;
    for (const auto & choiceDisplayName : choiceDisplayNamesVec)
    {
      choiceDisplayNames.append(choiceDisplayName);
    }
    param.append("choiceDisplayNames", choiceDisplayNames);

    param.append("isRead", boolToString(arg.isRead()));
    param.append("extension", arg.extension());

    params.append(arg.name(), param);
  }

  JobParams p;
  p.append("user_script_params", params);
  return p;
}

std::vector<ruleset::OSArgument> RubyJobBuilder::toOSArguments(const JobParams &t_params, const openstudio::path &t_basePath)
{
  std::vector<ruleset::OSArgument> retval;

  std::vector<JobParam> args;
  try {
    args = t_params.get("user_script_params").children;
  }
  catch (...) {
    return retval;
  }

  LOG(Debug, "Params found: " << args.size());
  for (const auto & jobParam : args)
  {
    try {
      JobParams arg(jobParam.children);
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

      for (const auto & dom : domainvec)
      {
        domain.push_back(dom.value);
      }

      std::vector<JobParam> choicesvec = arg.get("choices").children;
      std::vector<std::string> choices;

      for (const auto & choice : choicesvec)
      {
        choices.push_back(choice.value);
      }

      std::vector<JobParam> choiceDisplayNamesVec = arg.get("choiceDisplayNames").children;
      std::vector<std::string> choiceDisplayNames;

      for (const auto & choiceDisplayName : choiceDisplayNamesVec)
      {
        choiceDisplayNames.push_back(choiceDisplayName.value);
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

  addRequiredFile(userScriptPath,toPath("user_script.rb"), t_relativeTo, true);

  // add arguments in -n, -v, name, value pairs
  for (const ruleset::OSArgument& argument : t_args) {
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
        for (const ruleset::OSArgument& arg : args) {
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
        for (const auto & arg : t_scriptInfo.arguments)
        {
          args.push_back(arg.second);
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
  for (const auto & job : jobs)
  {
    t_rm.remove(job);
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

void RubyJobBuilder::constructFromBCLMeasure(const openstudio::BCLMeasure &t_measure,
                                             const std::vector<ruleset::OSArgument>& t_args,
                                             const openstudio::path &t_relativeTo,
                                             bool t_copyFileTrue)
{
  boost::optional<openstudio::path> script = t_measure.primaryRubyScriptPath();
  if (!script)
  {
    throw std::runtime_error("Passed in measure does not have a primaryRubyScriptPath set, which is required for RubyJobBuilder");
  }

  m_bclMeasureUUID = t_measure.uuid();

  setAsUserScriptRubyJob(*script, t_args, t_relativeTo, t_copyFileTrue);

  std::vector<BCLFileReference> files = t_measure.files();

  for (const auto & bclFileRef : files)
  {
    openstudio::path relativePath = openstudio::relativePath(bclFileRef.path(), t_measure.directory());

    bool isTestPath = false;
    for (const auto & path : relativePath)
    {
      if (boost::iequals(openstudio::toString(path), "tests"))
      {
        isTestPath = true;
        break;
      }
    }

    if (!isTestPath)
    {
      LOG(Trace, "Adding required file from measure: " << openstudio::toString(bclFileRef.path()) << " to: " << openstudio::toString(bclFileRef.path().filename()));
      addRequiredFile(bclFileRef.path(), relativePath);
    } else {
      LOG(Trace, "Skipping test file from measure: " << openstudio::toString(bclFileRef.path()));
    }

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

}
}
