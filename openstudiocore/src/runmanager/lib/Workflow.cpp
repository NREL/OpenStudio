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
#include "Workflow.hpp"
#include "WorkItem.hpp"
#include "RubyJobUtils.hpp"
#include <QCryptographicHash>

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"

namespace openstudio {
namespace runmanager {

  Workflow::Workflow()
    : m_uuid(createUUID())
  {
  }

  Workflow::Workflow(const Workflow &t_wf)
    : m_uuid(t_wf.uuid()), m_workflowName(t_wf.m_workflowName)
  {
    if (t_wf.m_job)
    {
      m_job = std::shared_ptr<Workflow::WorkflowJob>(new Workflow::WorkflowJob(*t_wf.m_job));
    }
  }

  void Workflow::swap(Workflow &t_other)
  {
    std::swap(m_uuid, t_other.m_uuid);
    std::swap(m_workflowName, t_other.m_workflowName);
    m_job.swap(t_other.m_job);
  }

  Workflow &Workflow::operator=(const Workflow &t_rhs)
  {
    Workflow temp(t_rhs);
    swap(temp);
    return *this;
  }

  Workflow::Workflow(Job t_job, const std::string &t_workflowName)
    : m_uuid(createUUID()), m_workflowName(t_workflowName), m_job(new Workflow::WorkflowJob(t_job))
  {
    if (t_workflowName.empty())
    {
      try {
        m_workflowName = t_job.jobParams().get("workflowname").children.at(0).value;
      } catch (const std::exception &) {
        // continue
      }

    }
  }

  Workflow::Workflow(const std::string &t_workflowstring, const std::string &t_workflowName)
    : m_uuid(createUUID()), m_workflowName(t_workflowName)
  {
    boost::regex re("[a-zA-Z0-9]+");
    boost::sregex_token_iterator i(t_workflowstring.begin(), t_workflowstring.end(), re);
    boost::sregex_token_iterator j;

    std::vector<openstudio::runmanager::JobType> jobs;

    while (i != j)
    {
      jobs.push_back(openstudio::runmanager::JobType(*i));
      ++i;
    }

    addJobs(jobs);
  }

  Workflow::Workflow(const std::vector<openstudio::runmanager::JobType> &t_types, const std::string &t_workflowName)
    : m_uuid(createUUID()), m_workflowName(t_workflowName)
  {
    addJobs(t_types);
  }

  Workflow::Workflow(const std::vector<WorkItem> &t_items, const std::string &t_workflowName)
    : m_uuid(createUUID()), m_workflowName(t_workflowName)
  {
    for (const auto & workItem : t_items)
    {
      addJob(workItem);
    }
  }

  void Workflow::addJob(const WorkItem &t_workItem)
  {
    addJob(t_workItem.type, t_workItem.tools, t_workItem.params, t_workItem.files, t_workItem.jobkeyname);
  }

  void Workflow::addJobs(const std::vector<openstudio::runmanager::JobType> &t_types)
  {
    for (const auto & jobType : t_types)
    {
      addJob(jobType,
          openstudio::runmanager::Tools(),
          openstudio::runmanager::JobParams(),
          openstudio::runmanager::Files());
    }
  }

  void Workflow::addJob(const WorkItem &t_wi, const std::string &t_jobkeyname)
  {
    addJob(t_wi.type, t_wi.tools, t_wi.params, t_wi.files, t_jobkeyname);
  }

  void Workflow::addJob(openstudio::runmanager::JobType type, const std::string &jobkeyname)
  {
    openstudio::runmanager::JobParams params;
    params.append("workflowjobkey", jobkeyname);
    addJob(type, openstudio::runmanager::Tools(), params, openstudio::runmanager::Files());
  }

  void Workflow::addJob(openstudio::runmanager::JobType type,
      const openstudio::runmanager::Tools &tools,
      const openstudio::runmanager::JobParams &params,
      const openstudio::runmanager::Files &files,
      const std::string &jobkeyname)
  {
    openstudio::runmanager::JobParams p(params);
    p.append("workflowjobkey", jobkeyname);
    addJob(type, tools, p, files);
  }

  void Workflow::addWorkflow(const openstudio::runmanager::Workflow &t_wf)
  {
    if (t_wf.m_job)
    {
      std::shared_ptr<Workflow::WorkflowJob> job(new Workflow::WorkflowJob(*t_wf.m_job));
      std::shared_ptr<Workflow::WorkflowJob> lastjob = getLastJob();

      if (lastjob)
      {
        lastjob->children.push_back(job);
      }

      if (!m_job)
      {
        m_job = job;
      }

      m_last_job = getLastJob(job);
    }
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      openstudio::runmanager::JobType type)
  {
    replaceJobs(jobkeyname, type,
       openstudio::runmanager::Tools(),
       openstudio::runmanager::JobParams(),
       openstudio::runmanager::Files());
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      openstudio::runmanager::JobType type,
      const openstudio::runmanager::Tools &tools,
      const openstudio::runmanager::JobParams &params,
      const openstudio::runmanager::Files &files)
  {
    std::shared_ptr<Workflow::WorkflowJob> newjob(new Workflow::WorkflowJob(type, tools, params, files));
    replaceJobs(jobkeyname, newjob);
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      const openstudio::runmanager::Workflow &t_wf)
  {
    if (!t_wf.m_job)
    {
      throw std::runtime_error("Workflow does not contain any jobs, cannot be used for the replaceJobs command");
    }

    std::shared_ptr<Workflow::WorkflowJob> newjob(new Workflow::WorkflowJob(*t_wf.m_job));
    replaceJobs(jobkeyname, newjob);
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      const openstudio::runmanager::Job &t_job)
  {
    replaceJobs(jobkeyname, openstudio::runmanager::Workflow(t_job));
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      const std::shared_ptr<Workflow::WorkflowJob> &t_job)
  {
    replaceJobs(jobkeyname, t_job, m_job);
  }

  void Workflow::replaceJobs(const std::string &jobkeyname,
      const std::shared_ptr<Workflow::WorkflowJob> &t_newjob,
      std::shared_ptr<Workflow::WorkflowJob> &t_oldjob)
  {

    if (!t_oldjob)
    {
      return;
      //nothing to do
    }

    try {
      if (t_oldjob->params.get("workflowjobkey").children.at(0).value == jobkeyname)
      {
        std::shared_ptr<Workflow::WorkflowJob> newjobcopy(new Workflow::WorkflowJob(*t_newjob));
        std::shared_ptr<Workflow::WorkflowJob> lastjob = getLastJob(newjobcopy);
        lastjob->children = t_oldjob->children;
        lastjob->finishedJob = t_oldjob->finishedJob;

        // Copy over the tools, params and files from the job we are replacing,
        // but if the new job had any of its own, give them precidence
        Tools tools(t_oldjob->tools);
        tools.append(newjobcopy->tools);

        Files files(t_oldjob->files);

        std::vector<openstudio::runmanager::FileInfo> fs = files.files();
        for (size_t i = 0; i < fs.size(); ++i)
        {
          LOG(Debug, "old job files: " << toString(fs[i].fullPath));
        }

        files.append(newjobcopy->files);

        fs = files.files();
        for (size_t i = 0; i < fs.size(); ++i)
        {
          LOG(Debug, "new job files: " << toString(fs[i].fullPath));
        }


        JobParams params(t_oldjob->params);
        params.remove("workflowjobkey"); // don't carry the name of the thing we replaced over to the new job
        params.append(newjobcopy->params);

        newjobcopy->tools = tools;
        newjobcopy->files = files;
        newjobcopy->params = params;

        t_oldjob = newjobcopy;
      }
    } catch (const std::exception &) {
      //not a match
    }

    for (auto & workflowJob : t_oldjob->children)
    {
      replaceJobs(jobkeyname, t_newjob, workflowJob);
    }

    replaceJobs(jobkeyname, t_newjob, t_oldjob->finishedJob);
  }



  void Workflow::addJob(openstudio::runmanager::JobType type)
  {
    addJob(type, Tools(), JobParams(), Files());
  }

  void Workflow::addJob(openstudio::runmanager::JobType type,
      const Tools &tools,
      const openstudio::runmanager::JobParams &params,
      const Files &files)
  {
    std::shared_ptr<Workflow::WorkflowJob> job(new Workflow::WorkflowJob(type, tools, params, files));
    std::shared_ptr<Workflow::WorkflowJob> lastjob = getLastJob();

    if (lastjob)
    {
      lastjob->children.push_back(job);
    }

    if (!m_job)
    {
      m_job = job;
    }

    m_last_job = job;
  }

  std::shared_ptr<Workflow::WorkflowJob> Workflow::getLastJob(const std::shared_ptr<Workflow::WorkflowJob> &t_job)
  {
    if (t_job)
    {
      if (t_job->finishedJob)
      {
        return getLastJob(t_job->finishedJob);
      } else {
        if (t_job->children.empty())
        {
          return t_job;
        } else {
          return getLastJob(t_job->children.front());
        }
      }
    } else {
      return std::shared_ptr<Workflow::WorkflowJob>();
    }
  }

  std::shared_ptr<Workflow::WorkflowJob> Workflow::getLastJob()
  {
    if (m_last_job)
    {
      return m_last_job;
    } else {
      m_last_job = getLastJob(m_job);
      return m_last_job;
    }
  }

  std::shared_ptr<Workflow::WorkflowJob> Workflow::getFirstJob() const {
    if (m_job) {
      return m_job;
    } else {
      throw std::runtime_error("No jobs yet added to workflow");
    }
  }

  void Workflow::addFile(const std::string &key, const openstudio::path &filepath)
  {
    getFirstJob()->files.append(FileInfo(filepath, key));
  }

  void Workflow::addTool(const ToolInfo &tool)
  {
    getFirstJob()->tools.append(tool);
  }

  void Workflow::addParam(const JobParam &param)
  {
    getFirstJob()->params.append(param);
  }

  void Workflow::add(const openstudio::runmanager::JobParams &t_params)
  {
    getFirstJob()->params.append(t_params);
  }

  void Workflow::add(const openstudio::runmanager::Tools &t_tools)
  {
    getFirstJob()->tools.append(t_tools);
  }

  void Workflow::add(const openstudio::runmanager::Files &t_files)
  {
    getFirstJob()->files.append(t_files);
  }

  openstudio::UUID Workflow::uuid() const {
    return m_uuid;
  }

  void Workflow::setUUID(const openstudio::UUID& uuid) {
    m_uuid = uuid;
  }

  boost::optional<FileReferenceType> Workflow::requiredSeedFileType() const {
    OptionalFileReferenceType result;
    try {
      JobType firstJobType(getFirstJob()->type);

      if (firstJobType == JobType::EnergyPlus) {
        result = FileReferenceType::IDF;
      }
      else if (firstJobType == JobType::ModelToIdf) {
        result = FileReferenceType::OSM;
      }
      else if (firstJobType == JobType::ExpandObjects) {
        result = FileReferenceType::IDF;
      }
      else if (firstJobType == JobType::XMLPreprocessor) {
        result = FileReferenceType::XML;
      }
      else if (firstJobType == JobType::ParallelEnergyPlusSplit) {
        result = FileReferenceType::IDF;
      }
    }
    catch (...) {}
    return result;
  }

  void Workflow::setInputFiles(const openstudio::path &t_infile,
                               const openstudio::path &t_epwdirorfile)
  {
    // Build list of job params
    openstudio::runmanager::JobParams params;

    // Build list of files
    std::string infileextension = getFileExtension(t_infile);

    openstudio::runmanager::Files files;

    openstudio::runmanager::FileInfo fi(t_infile, infileextension);
    if (boost::filesystem::is_directory(t_epwdirorfile))
    {
      LOG(Debug, "Setting epwdir to: " << toString(t_epwdirorfile));
      params.append("epwdir", toString(t_epwdirorfile));
    } else if (boost::filesystem::exists(t_epwdirorfile)) {
      LOG(Debug, "Setting in.epw to: " << toString(t_epwdirorfile));
      fi.addRequiredFile(t_epwdirorfile, toPath("in.epw"));
    }

    files.append(fi);

    add(files);
    add(params);
  }

  void Workflow::setInputFiles(const std::vector<openstudio::path> &t_infiles)
  {
    // set infiles
    openstudio::runmanager::JobParams params;
    openstudio::runmanager::Files files;

    if (!t_infiles.empty()) {
      std::string infileextension = getFileExtension(t_infiles[0]);
      openstudio::runmanager::FileInfo fi(t_infiles[0], infileextension);
      for (unsigned i = 1, n = t_infiles.size(); i < n; ++i) {
        if (!boost::filesystem::is_directory(t_infiles[i]) &&
            boost::filesystem::exists(t_infiles[i]))
        {
          LOG(Debug,"Attaching required file '" << toString(t_infiles[i])
              << "', which will be copied into outdir as " << toString(t_infiles[i].filename())
              << ".");
          fi.addRequiredFile(t_infiles[i],t_infiles[i].filename());
        }
        else {
          LOG(Warn,"Path specified in creation of Job from Workflow, '" << toString(t_infiles[i])
              << "' is a directory or does not exist, and so cannot be attached as a required "
              << "file.");
        }
      }
      files.append(fi);
    }

    add(files);
    add(params);
  }

  openstudio::runmanager::Job Workflow::create(
      const openstudio::path &t_outdir,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths)
  {
    // Build list of job params
    openstudio::runmanager::JobParams params;
    params.append("outdir", toString(t_outdir));
    add(params);

    return create(t_url_search_paths);
  }

  openstudio::runmanager::Job Workflow::create(
      const openstudio::path &t_outdir,
      const openstudio::path &t_infile,
      const openstudio::path &t_epwdirorfile,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths)
  {
    LOG(Debug, "Creating workflow with outdir " << toString(t_outdir) << " infile "
        << toString(t_infile) << " epwlocation " << toString(t_epwdirorfile));
    setInputFiles(t_infile, t_epwdirorfile);

    return create(t_outdir, t_url_search_paths);
  }

  openstudio::runmanager::Job Workflow::create(
      const openstudio::path& t_outdir,
      const std::vector<openstudio::path>& t_infiles,
      const std::vector<openstudio::URLSearchPath>& t_url_search_paths)
  {
    if (t_infiles.empty()) {
      LOG(Debug,"Creating workflow with outdir " << toString(t_outdir) << ".");
    }
    else {
      LOG(Debug,"Creating workflow with outdir " << toString(t_outdir) << " and input file "
          << toString(t_infiles[0]) << ".");
    }
    setInputFiles(t_infiles);

    return create(t_outdir,t_url_search_paths);
  }

  void Workflow::hash(const openstudio::runmanager::JobType &t_type, QCryptographicHash &t_hash) const
  {
    openstudio::runmanager::JobType::domain v = t_type.value();
    t_hash.addData(reinterpret_cast<const char *>(&v), sizeof(openstudio::runmanager::JobType::domain));
  }

  void Workflow::hash(const openstudio::runmanager::ToolInfo &t_tool, QCryptographicHash &t_hash) const
  {
    t_hash.addData(t_tool.name.data(), t_tool.name.length());
  }

  void Workflow::hash(const openstudio::runmanager::Tools &t_tools, QCryptographicHash &t_hash) const
  {
    std::vector<ToolInfo> tools = t_tools.tools();
    for (std::vector<ToolInfo>::const_iterator itr = tools.begin();
         itr != tools.end();
         ++itr)
    {
      hash(*itr, t_hash);
    }
  }

  void Workflow::hash(const openstudio::runmanager::FileInfo &t_file, QCryptographicHash &t_hash) const
  {
    std::string s = toString(t_file.fullPath);
    t_hash.addData(s.data(), s.length());

    for (const auto & requiredFile : t_file.requiredFiles)
    {
      std::string f1 = toString(requiredFile.first.toString());
      std::string f2 = toString(requiredFile.second);
      t_hash.addData(f1.data(), f1.length());
      t_hash.addData(f2.data(), f2.length());
    }
  }

  void Workflow::hash(const openstudio::runmanager::Files &t_files, QCryptographicHash &t_hash) const
  {
    std::vector<openstudio::runmanager::FileInfo> files = t_files.files();

    for (const auto & fileInfo : files)
    {
      hash(fileInfo, t_hash);

    }
  }

  void Workflow::hash(const openstudio::runmanager::JobParam &t_param, QCryptographicHash &t_hash) const
  {
    t_hash.addData(t_param.value.data(), t_param.value.size());

    std::vector<openstudio::runmanager::JobParam> params = t_param.children;
    for (const auto & jobParam : params)
    {
      hash(jobParam, t_hash);
    }
  }

  void Workflow::hash(const openstudio::runmanager::JobParams &t_params, QCryptographicHash &t_hash) const
  {
    std::vector<JobParam> params = t_params.params();

    for (const auto & jobParam : params)
    {
      hash(jobParam, t_hash);
    }
  }

  void Workflow::hash(const std::shared_ptr<Workflow::WorkflowJob> &t_job, QCryptographicHash &t_hash) const
  {
    if (t_job)
    {
      hash(t_job->type, t_hash);
      hash(t_job->tools, t_hash);
      hash(t_job->params, t_hash);
      hash(t_job->files, t_hash);

      hash(t_job->finishedJob, t_hash);

      for (const auto & workflowJob : t_job->children)
      {
        hash(workflowJob, t_hash);
      }
    }
  }

  std::string Workflow::key() const
  {
    QCryptographicHash hashdata(QCryptographicHash::Md5);

    hash(m_job, hashdata);

    return toString(QString(hashdata.result().toHex()));
  }

  openstudio::runmanager::Tools Workflow::tools() const {
    return getFirstJob()->tools;
  }

  openstudio::runmanager::JobParams Workflow::params() const {
    return getFirstJob()->params;
  }

  openstudio::runmanager::Files Workflow::files() const {
    return getFirstJob()->files;
  }

  openstudio::runmanager::JobType Workflow::type() const {
    return getFirstJob()->type;
  }

  Job Workflow::createJob(const std::shared_ptr<Workflow::WorkflowJob> &t_job,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths)
  {
    Job j = JobFactory::createJob(t_job->type, t_job->tools, t_job->params, t_job->files,
        t_url_search_paths);

    if (t_job->finishedJob)
    {
      j.setFinishedJob(createJob(t_job->finishedJob, t_url_search_paths));
    }

    for (const auto & workflowJob : t_job->children)
    {
      j.addChild(createJob(workflowJob, t_url_search_paths));
    }

    return j;
  }

  Job Workflow::create(const std::vector<openstudio::URLSearchPath> &t_url_search_paths)
  {
    LOG(Debug, "creating workflow");

    if (!m_job)
    {
      throw std::runtime_error("Unable to create workflow: no child jobs");
    }

    m_job->params.remove("workflowkey");

    m_job->params.remove("workflowname");

    openstudio::runmanager::JobParams params;
    std::string k = key();
    params.append("workflowkey", toString(k));
    if (!m_workflowName.empty())
    {
      params.append("workflowname", m_workflowName);
    }

    add(params);

    std::vector<FileInfo> files = getFirstJob()->files.files();

    for (size_t i = 0; i < files.size(); ++i)
    {
      LOG(Debug, "Workflow contains file " << toString(files[i].fullPath));
    }

    return createJob(m_job, t_url_search_paths);
  }

  std::vector<openstudio::path> Workflow::buildPathVector(const openstudio::path &t_path,
    const openstudio::path &t_relativeTo)
  {
    std::vector<openstudio::path> retval;

    if (!t_path.empty()
        && boost::filesystem::exists(t_path)
        && boost::filesystem::is_directory(t_path))
    {
      for(boost::filesystem::directory_iterator itr(t_path);
          itr != boost::filesystem::directory_iterator();
          ++itr)
      {
        openstudio::path p = itr->path();
        openstudio::path relative = openstudio::relativePath(p, t_relativeTo);

        if (!relative.empty())
        {
          retval.push_back(relative);
        } else {
          retval.push_back(p);
        }
      }
    }

    return retval;
  }

  bool Workflow::addScripts(const openstudio::path &t_scriptsPath,
                            const std::vector<ruleset::UserScriptInfo> &t_userScripts,
                            const openstudio::path &t_resourcesPath,
                            const std::string &t_inputExtension,
                            bool t_rubyInstalled,
                            const openstudio::path &t_rubyIncludePath,
                            const openstudio::path &t_relativeTo,
                            bool t_copyFileArguments)
  {
    LOG(Debug, "Analyzing script path for files: " << openstudio::toString(t_scriptsPath));
    if (!t_scriptsPath.empty()) {

      std::vector<openstudio::path> files = buildPathVector(t_resourcesPath, t_relativeTo);
      std::vector<openstudio::runmanager::RubyJobBuilder> rubyjobs =
          openstudio::runmanager::RubyJobBuilder::createRubyJobsFromFolder(t_scriptsPath,
                                                                           t_userScripts,
                                                                           t_inputExtension,
                                                                           t_relativeTo,
                                                                           t_copyFileArguments);

      LOG(Debug, "Files found: " << files.size() << " ruby jobs created: " << rubyjobs.size());

      if (t_rubyInstalled) {


        openstudio::runmanager::RubyJobBuilder::addJobsToWorkflow(
            *this,
            rubyjobs,
            t_rubyIncludePath,
            files);

        // there were scripts to add and we did add them
        return false;
      } else {
        if (!rubyjobs.empty())
        {
          // there were scripts to add, and we could not add them, no ruby
          return true;
        } else {
          // no jobs were created
          return false;
        }
      }
    }

    // We did not fail to add scripts, there were none to add
    return false;
  }

  bool Workflow::addStandardWorkflow(const openstudio::path &t_scriptsPath,
      bool t_rubyInstalled,
      const openstudio::path &t_rubyIncludePath,
      bool t_useRadianceForDaylightCalculations,
      const openstudio::path &t_radiancePath)
  {
    return addStandardWorkflow(t_scriptsPath, t_rubyInstalled, t_rubyIncludePath,
        std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >(),
        t_useRadianceForDaylightCalculations,
        t_radiancePath);
  }

  WorkItem Workflow::radianceDaylightCalculations(const openstudio::path &t_rubyIncludePath,
      const openstudio::path &t_radiancePath)
  {
    Workflow wf;

    RubyJobBuilder rjb;
    rjb.setIncludeDir(t_rubyIncludePath);
    rjb.addScriptArgument("in.osm");
    rjb.addScriptArgument(openstudio::toString(t_radiancePath));
    rjb.addInputFile(FileSelection::Last, FileSource::All, ".*\\.osm", "in.osm");
    openstudio::path scriptsPath = openstudio::getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/radiance/");
    rjb.addRequiredFile(scriptsPath / openstudio::toPath("ModelToRad.rb"), openstudio::toPath("ModelToRad.rb"));
    rjb.addRequiredFile(scriptsPath / openstudio::toPath("DaylightSim.rb"), openstudio::toPath("DaylightSim.rb"));
    rjb.addRequiredFile(scriptsPath / openstudio::toPath("DaylightSim-Simple.rb"), openstudio::toPath("DaylightSim-Simple.rb"));
    rjb.addRequiredFile(scriptsPath / openstudio::toPath("MakeSchedules.rb"), openstudio::toPath("MakeSchedules.rb"));
    rjb.addRequiredFile(scriptsPath / openstudio::toPath("DaylightMetrics.rb"), openstudio::toPath("DaylightMetrics.rb"));
    rjb.copyRequiredFiles("osm", "osm", "in.epw");
    rjb.setScriptFile(scriptsPath / openstudio::toPath("DaylightCalculations.rb"));
    rjb.addToWorkflow(wf);

    return wf.toWorkItems()[0];
  }

  bool Workflow::addStandardWorkflow(
      const openstudio::path &t_scriptsPath,
      bool t_rubyInstalled,
      const openstudio::path &t_rubyIncludePath,
      const std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >& t_userScriptsByFolder,
      bool t_useRadianceForDaylightCalculations,
      const openstudio::path &t_radiancePath,
      const openstudio::path &t_relativeTo,
      bool t_copyFileArguments)
  {

    if (t_useRadianceForDaylightCalculations)
    {
      addJob(radianceDaylightCalculations(t_rubyIncludePath, t_radiancePath));
    }

    openstudio::path modelScriptsPath;
    std::vector<ruleset::UserScriptInfo> modelUserScripts;
    openstudio::path modelResourcesPath;
    openstudio::path idfScriptsPath;
    std::vector<ruleset::UserScriptInfo> idfUserScripts;
    openstudio::path idfResourcesPath;
    openstudio::path postEnergyPlusScriptsPath;
    std::vector<ruleset::UserScriptInfo> postEnergyPlusUserScripts;

    if (!t_scriptsPath.empty()) {
      modelScriptsPath = t_scriptsPath / openstudio::toPath("model_scripts");

      auto itr = t_userScriptsByFolder.find(modelScriptsPath);
      if (itr != t_userScriptsByFolder.end())
      {
        modelUserScripts = itr->second;
      }

      modelResourcesPath = t_scriptsPath / openstudio::toPath("model_resources");
      idfScriptsPath = t_scriptsPath / openstudio::toPath("idf_scripts");


      itr = t_userScriptsByFolder.find(idfScriptsPath);
      if (itr != t_userScriptsByFolder.end())
      {
        idfUserScripts = itr->second;
      }

      idfResourcesPath = t_scriptsPath / openstudio::toPath("idf_resources");
      postEnergyPlusScriptsPath = t_scriptsPath / openstudio::toPath("post_energyplus");

      itr = t_userScriptsByFolder.find(postEnergyPlusScriptsPath);
      if (itr != t_userScriptsByFolder.end())
      {
        postEnergyPlusUserScripts = itr->second;
      }
    }

    bool rubyJobsSkipped = addScripts(modelScriptsPath,
                                      modelUserScripts,
                                      modelResourcesPath,
                                      "osm",
                                      t_rubyInstalled,
                                      t_rubyIncludePath,
                                      t_relativeTo,
                                      t_copyFileArguments);
    addJob(openstudio::runmanager::JobType::ModelToIdf);
    addJob(openstudio::runmanager::JobType::ExpandObjects);
    rubyJobsSkipped = rubyJobsSkipped || addScripts(idfScriptsPath,
                                                    idfUserScripts,
                                                    idfResourcesPath,
                                                    "idf",
                                                    t_rubyInstalled,
                                                    t_rubyIncludePath,
                                                    t_relativeTo,
                                                    t_copyFileArguments);
    addJob(openstudio::runmanager::JobType::EnergyPlusPreProcess);
    addJob(openstudio::runmanager::JobType::EnergyPlus);
    rubyJobsSkipped = rubyJobsSkipped || addScripts(postEnergyPlusScriptsPath,
                                                    postEnergyPlusUserScripts,
                                                    openstudio::path(),
                                                    "sql",
                                                    t_rubyInstalled,
                                                    t_rubyIncludePath,
                                                    t_relativeTo,
                                                    t_copyFileArguments);

    return rubyJobsSkipped;
  }


  std::vector<WorkItem> Workflow::toWorkItems() const
  {
    std::shared_ptr<WorkflowJob> wfi = m_job;

    std::vector<WorkItem> retval;

    while (wfi)
    {
      JobParams p = wfi->params;
      std::string jobkeyname;

      if (p.has("workflowjobkey"))
      {
        // capture and erase the jobkey if it exists
        if (!p.get("workflowjobkey").children.empty())
        {
          jobkeyname = p.get("workflowjobkey").children[0].value;
        }
        p.remove("workflowjobkey");
      }
      

      retval.push_back(WorkItem(wfi->type, wfi->tools, wfi->params, wfi->files, jobkeyname));

      if (wfi->children.empty())
      {
        wfi.reset();
      } else if (wfi->children.size() == 1 && !wfi->finishedJob) {
        wfi = wfi->children[0];
      } else {
        throw std::runtime_error("Cannot convert WorkflowJob with multiple children into a WorkItem");
      }
    }

    return retval;
  }

  std::string Workflow::getName() const
  {
    return m_workflowName;
  }

  void Workflow::setName(const std::string &t_name)
  {
    m_workflowName = t_name;
  }


  void Workflow::parallelizeEnergyPlus(int t_numSplits, int t_offset)
  {
    try {
      std::vector<WorkItem> workitems = toWorkItems();

      Workflow newwf;
      newwf.m_uuid = m_uuid;
      newwf.m_workflowName = m_workflowName;

      for (const auto & workItem : workitems)
      {
        if (workItem.type != JobType::EnergyPlus)
        {
          newwf.addJob(workItem);
        } else {
          Workflow parallelep(JobFactory::createParallelEnergyPlusJobTree(t_numSplits, t_offset));
          assert(parallelep.m_job);
          parallelep.m_job->params.append(workItem.params);
          parallelep.m_job->files.append(workItem.files);
          parallelep.m_job->tools.append(workItem.tools);
          newwf.addWorkflow(parallelep);
        }
      }

      swap(newwf);

    } catch (const std::exception &) {
      throw std::runtime_error("The parallelizeEnergyPlus process only works with linear workflows");
    }
  }

}
}
