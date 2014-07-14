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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "UserScriptJob.hpp"
#include "RubyJob.hpp"
#include "RubyJobUtils.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "MergeJobError.hpp"
#include "WorkItem.hpp"
#include "JSON.hpp"

#include "../../utilities/time/DateTime.hpp"

#include <QDir>
#include <QDateTime>
#include <QUrl>

namespace openstudio {
namespace runmanager {
namespace detail {

  RubyJob::RubyJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::Ruby, t_tools, "ruby", t_params, t_files, false, t_restoreData),
      m_mergedJobs(getMergedJobs(t_params))
  {
    LOG(Trace, "Creating a RubyJob");

    m_description = buildDescription("rb");

    LOG(Trace, "RubyJob Created");
  }

  RubyJob::RubyJob(const UUID &t_uuid,
      const JobType &t_jobType,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, t_jobType, t_tools, "ruby", t_params, t_files, false, t_restoreData),
      m_mergedJobs(getMergedJobs(t_params))
  {
    LOG(Trace, "Creating a RubyJob");

    m_description = buildDescription("rb");

    LOG(Trace, "RubyJob Created");
  }


  RubyJob::~RubyJob()
  { 
    assert(QThread::currentThread() != this);
    shutdownJob();
  }

  std::vector<JobParams> RubyJob::getMergedJobs(const JobParams &t_params)
  {
    std::vector<RubyJobBuilder> jobs = RubyJobBuilder(t_params).mergedJobs();

    std::vector<JobParams> jp;
    for (std::vector<RubyJobBuilder>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      jp.push_back(itr->toParams());
    }

    return jp;
  }

  ToolVersion RubyJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "ruby") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    return ToolVersion();
  }

  std::string RubyJob::descriptionImpl() const
  {
    return m_description;
  }

  void RubyJob::basePathChanged()
  {
  }

  void RubyJob::mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job) 
  {

    // only work on UserScriptJobs
    std::shared_ptr<UserScriptJob> usjob = std::dynamic_pointer_cast<UserScriptJob>(t_job);
    std::shared_ptr<UserScriptJob> usparentjob = std::dynamic_pointer_cast<UserScriptJob>(t_parent);

    if (!usjob || !usparentjob)
    {
      throw MergeJobError("Mismatched job types");
    }

    if (usjob->jobType() != usparentjob->jobType())
    {
      throw MergeJobError("Mismatched job types");
    }

    if (t_parent->finishedJob() == t_job)
    {
      throw MergeJobError("RHS is finished job - refusing to merge");
    }

    RubyJobBuilder rjbparent(usparentjob->params());
    RubyJobBuilder rjbthis(usjob->params());
   
    if (rjbparent.copyRequiredFiles() != rjbthis.copyRequiredFiles())
    {
      throw MergeJobError("Cannot merge user script jobs, the required files do not match.");
    }


    LOG(Info, "Merging Job " << openstudio::toString(t_job->uuid()) << " into " << openstudio::toString(uuid()));
    
    removeChild(t_job);
    std::vector<std::shared_ptr<Job_Impl> > children = t_job->children();
    std::for_each(children.begin(), children.end(), std::bind(&Job_Impl::addChild, t_parent, std::placeholders::_1));

    std::vector<JobParams> existing_merged_jobs = usjob->m_mergedJobs;
    JobParams job_to_merge = usjob->params();
    if (job_to_merge.has("merged_ruby_jobs"))
    {
      job_to_merge.remove("merged_ruby_jobs");
    }

    if (!job_to_merge.has("original_job_uuid"))
    {
      job_to_merge.append("original_job_uuid", openstudio::toString(t_job->uuid()));
    }

    existing_merged_jobs.insert(existing_merged_jobs.begin(), job_to_merge);
    usjob->m_mergedJobs.clear();
    m_mergedJobs.insert(m_mergedJobs.end(), existing_merged_jobs.begin(), existing_merged_jobs.end());

    JobParams myParams = params();
    if (myParams.has("merged_ruby_jobs"))
    {
      myParams.remove("merged_ruby_jobs");
    }

    JobParams merged;
    for (size_t i = 0; i < m_mergedJobs.size(); ++i)
    {
      std::stringstream ss;
      ss << i;
      // update the jobParams of this job
      merged.append(ss.str(), m_mergedJobs[i]);
    }

    myParams.append("merged_ruby_jobs", merged);

    setParams(myParams);

    Files files = rawInputFiles();

    Files usjobfiles = usjob->rawInputFiles();

    std::vector<FileInfo> fis = usjobfiles.files();

    for (std::vector<FileInfo>::const_iterator itr = fis.begin();
         itr != fis.end();
         ++itr)
    {
      files.append(*itr);
    }

    std::set<FileInfo> filteredfis(files.files().begin(), files.files().end()); // get the unique set of files
    setFiles(Files(std::vector<FileInfo>(filteredfis.begin(), filteredfis.end())));
  }

  void RubyJob::startHandlerImpl()
  {
    LOG(Info, "Starting job");

#if defined(_MSC_VER) && defined(_M_X64)
    // If we're on 64bit windows, we're using ruby 2.0.0 and we want to add
    // the current folder to the search path. This may need to be opened up to
    // all platforms and versions of ruby, but this seems to be working for now
    addParameter("ruby", "-I.");
#endif

    // Note: we only use params passed directly to us
    RubyJobBuilder rjb(params());
    getFiles(rjb);

    assert(rjb.mergedJobs().empty() || dynamic_cast<UserScriptJob*>(this) != 0); // this must only run with user script jobs if merged

    std::map<std::string, int> filenames;

    for (std::vector<std::pair<int, std::pair<Files, std::string> > >::const_iterator pairs = m_inputfiles.begin();
        pairs != m_inputfiles.end();
        ++pairs)
    {
      std::vector<FileInfo> files = pairs->second.first.files();
      for (std::vector<FileInfo>::const_iterator itr = files.begin();
          itr != files.end();
          ++itr)
      {
        std::stringstream ssfilename;
        if (!rjb.mergedJobs().empty())
        {
          ssfilename << "mergedjob-" << pairs->first << "/";
        }

        ssfilename << (pairs->second.second.empty()?itr->filename:pairs->second.second);
        std::string outfilename = ssfilename.str();

        int namecount = ++filenames[outfilename];

        openstudio::path to;

        if (namecount == 1)
        {
          to = toPath(outfilename);
        } else {
          to = toPath(
              boost::filesystem::path(outfilename).stem().string() + "-" + boost::lexical_cast<std::string>(namecount) 
              + boost::filesystem::extension(outfilename));
        }

        LOG(Info, "Adding required file " << toString(itr->fullPath) << " to: " 
            << toString(to));

        addRequiredFile(*itr, to);
      }
    }


    // Add tool parameters that go straight to ruby
    std::vector<std::string> toolparams = rjb.getToolParameters();

    for (std::vector<std::string>::const_iterator itr = toolparams.begin();
         itr != toolparams.end();
         ++itr)
    {
      addParameter("ruby", *itr);
    }

    // set up files that need to have "requiredFiles" copied from input to output
    typedef std::vector<std::tuple<std::string, std::string, std::string> > copyvectype;
    copyvectype copyfiles = rjb.copyRequiredFiles();

    Files inputfiles = allInputFiles();

    for (copyvectype::const_iterator itr = copyfiles.begin();
        itr != copyfiles.end();
        ++itr)
    {
      try {
        copyRequiredFiles(inputfiles.getLastByExtension(std::get<0>(*itr)), std::get<1>(*itr), toPath(std::get<2>(*itr)));
      } catch (const std::exception &e) {
        LOG(Error, "Error establishing file to copy required files from / to: " << e.what());
      }
    }

    // Add ruby script file name
    openstudio::path outpath = outdir(false);

    try {
      FileInfo fi = rjb.toWorkItem().files.getLastByExtension("rb");
      if (!rjb.mergedJobs().empty())
      {
        openstudio::path p = openstudio::toPath("mergedjob-0");
        outpath = outpath / p;
        addParameter("ruby", toString(p / openstudio::toPath("in.rb")));
        boost::filesystem::create_directory(outpath);
        fi.prependRequiredFilePath(p);
        addRequiredFile(fi, p / toPath("in.rb"));
      } else {
        addParameter("ruby", toString("in.rb"));
        addRequiredFile(fi, toPath("in.rb"));
      }

      runmanager::detail::JSON::saveJSON(rjb.toParams().params(), outpath / openstudio::toPath("params.json"));

      for (size_t i = 0; i < rjb.mergedJobs().size(); ++i)
      {
        openstudio::path p = openstudio::toPath("mergedjob-" + boost::lexical_cast<std::string>(i+1));

        FileInfo fi = rjb.mergedJobs()[i].toWorkItem().files.getLastByExtension("rb");
        fi.prependRequiredFilePath(p);
        addRequiredFile(fi, p / openstudio::toPath("in.rb"));
        boost::filesystem::create_directory(outdir()/p);
        runmanager::detail::JSON::saveJSON(rjb.mergedJobs()[i].toParams().params(), outdir()/p/openstudio::toPath("params.json"));
      }
    } catch (const std::exception &) {
      throw std::runtime_error("No rb file found in input files");
    }

    // Add remaining of parameters, which go to script
    std::vector<std::string> scriptparams = rjb.getScriptParameters();

    for (std::vector<std::string>::const_iterator itr = scriptparams.begin();
         itr != scriptparams.end();
         ++itr)
    {
      addParameter("ruby", *itr);
    }

    if (rjb.userScriptJob()){

      typedef std::pair<QUrl, openstudio::path> RequiredFileType;

      boost::optional<openstudio::path> lastEpwFilePath;

      try {
        FileInfo osm = allInputFiles().getLastByExtension("osm");
        std::string lastOpenStudioModelPathArgument = "--lastOpenStudioModelPath=" + toString(osm.fullPath);
        addParameter("ruby", lastOpenStudioModelPathArgument);

        // DLM: this does not seem to work on the top level input OSM, we may need to make the call to compute these required files
        // before this code is run
        for (const RequiredFileType& requiredFile : osm.requiredFiles){
          if (istringEqual(".epw", toString(requiredFile.second.extension()))){
            lastEpwFilePath = requiredFile.second;
            LOG(Info, "Found last EpwFile '" << toString(lastEpwFilePath->filename()) << "' attached to last OpenStudio Model");
          }
        }
      } catch (const std::exception &) {
      }

      try {
        FileInfo idf = allInputFiles().getLastByExtension("idf");
        std::string lastEnergyPlusWorkspacePathArgument = "--lastEnergyPlusWorkspacePath=" + toString(idf.fullPath);
        addParameter("ruby", lastEnergyPlusWorkspacePathArgument);

        // DLM: assume that an EPW file attached to last idf is more recent that EPW file attached to last osm
        for (const RequiredFileType& requiredFile : idf.requiredFiles){
          if (istringEqual(".epw", toString(requiredFile.second.extension()))){
            lastEpwFilePath = requiredFile.second;
            LOG(Info, "Found last EpwFile '" << toString(lastEpwFilePath->filename()) << "' attached to last EnergyPlus Workspace");
          }
        }
      } catch (const std::exception &) {
      }

      try {
        FileInfo sql = allInputFiles().getLastByFilename("eplusout.sql");
        std::string lastEnergyPlusSqlFilePathArgument = "--lastEnergyPlusSqlFilePath=" + toString(sql.fullPath);
        addParameter("ruby", lastEnergyPlusSqlFilePathArgument);
      } catch (const std::exception &) {
      }

      if (lastEpwFilePath){
        std::string lastEpwFilePathArgument = "--lastEpwFilePathArgument=" + toString(boost::filesystem::complete(*lastEpwFilePath, outpath));
        addParameter("ruby", lastEpwFilePathArgument);
      }
    }

  }

  bool RubyJob::hasMergedJobsImpl() const
  {
    return !RubyJobBuilder(params()).mergedJobs().empty();
  }

  std::vector<MergedJobResults> RubyJob::mergedJobResultsImpl() const
  {
    std::vector<MergedJobResults> results;

    RubyJobBuilder rjb(params());

    if (rjb.mergedJobs().empty())
    {
      return results;
    }

    Files oFiles = outputFiles();

    // search for stderr file in case of failure
    boost::optional<FileInfo> stderrFile;
    try { 
      stderrFile = oFiles.getLastByFilename("stderr"); 
    } catch (...) {
    }

    std::vector<FileInfo> files = oFiles.files();

    // loop over all merged jobs
    bool errorAssigned = false;
    std::set<std::string> reportedErrors;
    for (size_t i = 0; i <= rjb.mergedJobs().size(); ++i)
    {
      openstudio::UUID id;

      if (i == 0) { 
        id = uuid(); 
      } else {
        boost::optional<openstudio::UUID> optid = rjb.mergedJobs()[i-1].originalUUID();

        if (optid)
        {
          id = *optid;
        }
      }

      std::stringstream ss;
      ss << "mergedjob-" << i;
      openstudio::path expectedPath = openstudio::toPath(ss.str());

      Files jobfiles;

      for (std::vector<FileInfo>::const_iterator itr = files.begin();
           itr != files.end();
           ++itr)
      {
        if (itr->fullPath.parent_path().filename() == expectedPath)
        {
          jobfiles.append(*itr);
        }
      }

      JobErrors e; // default is failed case

      // there's no way to collate the errors if the job has never run
      if (lastRun() && !isRunning())
      {
        try {
          openstudio::path p = jobfiles.getLastByExtension("ossr").fullPath;
          boost::optional<openstudio::ruleset::OSResult> osresult = openstudio::ruleset::OSResult::load(p);
          if (osresult)
          {
            ErrorInfo ei;
            ei.osResult(*osresult);
            e = ei.errors();

            // these errors are reported with this merged job
            for (std::vector<std::pair<ErrorType, std::string> >::const_iterator itr = e.allErrors.begin();
                itr != e.allErrors.end();
                ++itr)
            {
              std::string error = itr->first.valueName() + itr->second;
              reportedErrors.insert(error);
            }
          } 
        } catch (const std::exception &) {

          // if this was the first merged job without an ossr we know this was the error
          if (!errorAssigned){

            // look through all JobErrors from all the merged jobs
            JobErrors thisErrors = errors();
            for (std::vector<std::pair<ErrorType, std::string> >::const_iterator itr = thisErrors.allErrors.begin();
                itr != thisErrors.allErrors.end();
                ++itr)
            {
              std::string error = itr->first.valueName() + itr->second;
              if (reportedErrors.find(error) == reportedErrors.end()){
                // error not yet reported, report here
                e.addError(itr->first, itr->second);
              }
            }

            // assign stderrFile here if it exists
            if (stderrFile){
              // should have at least one error here
              if (e.errors().empty()){
                e.addError(ErrorType::Error, "Merged job failed.");
              }
              jobfiles.append(*stderrFile);
            }else{
              e.addError(ErrorType::Error, "Merged job failed but cannot find stderr file, check " + toString(outdir(false)) + " for output.");
            }

            errorAssigned = true;
          }
        }
      }

      results.push_back(MergedJobResults(id, e, jobfiles));
    }

    return results;
  }

  void RubyJob::getFiles(const RubyJobBuilder &t_rjb)
  {
    LOG(Info, "Getting files");
    m_inputfiles.clear();

    Files allinputfiles = allInputFiles();
    Files myInputFiles = inputFiles();
    Files parentInputFiles;

    if (parent())
    {
      parentInputFiles = parent()->allFiles();
    }


    for (size_t i = 0; i <= t_rjb.mergedJobs().size(); ++i)
    {
      typedef std::vector<std::tuple<FileSelection, FileSource, std::string, std::string> > FileReqs;

      FileReqs inputFiles;
      if (i == 0) { 
        inputFiles = t_rjb.inputFiles();
      } else {
        inputFiles = t_rjb.mergedJobs()[i-1].inputFiles();
      }

      struct PickFileSource
      {
        static const Files& pick(const FileSource &t_filesource, const Files &t_all, const Files &t_mine,
            const Files &t_parent)
        {
          switch (t_filesource.value())
          {
            case FileSource::Parent:
              return t_parent;
              break;
            case FileSource::All:
              return t_all;
              break;
            case FileSource::Self:
              return t_mine;
              break;
          }

          throw std::domain_error("Unknown filesource type");
        }
      };

      for (FileReqs::const_iterator itr = inputFiles.begin();
          itr != inputFiles.end();
          ++itr)
      {
        try {
          const Files &source = PickFileSource::pick(std::get<1>(*itr), allinputfiles, myInputFiles, parentInputFiles);

          Files found;
          switch (std::get<0>(*itr).value())
          {
            case FileSelection::Last:
              found.append(source.getLastByRegex(std::get<2>(*itr)));
              break;
            case FileSelection::All:
              found.append(source.getAllByRegex(std::get<2>(*itr)));
              break;
          }

          m_inputfiles.push_back(std::make_pair(i, std::make_pair(found, std::get<3>(*itr))));
        } catch (const std::exception &) {
        }
      }
    }

  }


} // detail
} // runmanager
} // openstudio



