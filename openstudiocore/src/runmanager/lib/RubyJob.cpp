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

#include <utilities/time/DateTime.hpp>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>
#include <boost/tuple/tuple_comparison.hpp>

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
    LOG(Debug, "Creating a RubyJob");

    m_description = buildDescription("rb");

    LOG(Debug, "RubyJob Created");
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
    LOG(Debug, "Creating a RubyJob");

    m_description = buildDescription("rb");

    LOG(Debug, "RubyJob Created");
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

  void RubyJob::mergeJobImpl(const boost::shared_ptr<Job_Impl> &t_parent, const boost::shared_ptr<Job_Impl> &t_job) 
  {

    // only work on UserScriptJobs
    boost::shared_ptr<UserScriptJob> usjob = boost::dynamic_pointer_cast<UserScriptJob>(t_job);
    boost::shared_ptr<UserScriptJob> usparentjob = boost::dynamic_pointer_cast<UserScriptJob>(t_parent);

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
    std::vector<boost::shared_ptr<Job_Impl> > children = t_job->children();
    std::for_each(children.begin(), children.end(), boost::bind(&Job_Impl::addChild, t_parent, _1));

    std::vector<JobParams> existing_merged_jobs = usjob->m_mergedJobs;
    JobParams job_to_merge = usjob->params();
    if (job_to_merge.has("merged_ruby_jobs"))
    {
      job_to_merge.remove("merged_ruby_jobs");
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
        if (!rjb.mergedJobs().empty() && pairs->first != 0)
        {
          ssfilename << pairs->first << "/";
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
              boost::filesystem::path(outfilename).stem() + "-" + boost::lexical_cast<std::string>(namecount) 
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
    typedef std::vector<boost::tuple<std::string, std::string, std::string> > copyvectype;
    copyvectype copyfiles = rjb.copyRequiredFiles();

    Files inputfiles = allInputFiles();

    for (copyvectype::const_iterator itr = copyfiles.begin();
        itr != copyfiles.end();
        ++itr)
    {
      try {
        copyRequiredFiles(inputfiles.getLastByExtension(itr->get<0>()), itr->get<1>(), toPath(itr->get<2>()));
      } catch (const std::exception &e) {
        LOG(Error, "Error establishing file to copy required files from / to: " << e.what());
      }
    }


    // Add ruby script file name
    try {
      addRequiredFile(rjb.toWorkItem().files.getLastByExtension("rb"), toPath("in.rb"));
//      addRequiredFile(inputfiles.getLastByExtension("rb"), toPath("in.rb"));
      addParameter("ruby", toString("in.rb"));

      for (size_t i = 0; i < rjb.mergedJobs().size(); ++i)
      {
        openstudio::path p = openstudio::toPath(boost::lexical_cast<std::string>(i+1));

        FileInfo fi = rjb.mergedJobs()[i].toWorkItem().files.getLastByExtension("rb");
        fi.prependRequiredFilePath(p);

        addRequiredFile(fi, p / openstudio::toPath("in.rb"));
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

      try {
        FileInfo osm = allInputFiles().getLastByExtension("osm");
        std::string lastOpenStudioModelPathArgument = "--lastOpenStudioModelPath=" + toString(osm.fullPath);
        addParameter("ruby", lastOpenStudioModelPathArgument);
      } catch (const std::exception &) {
      }

      try {
        FileInfo idf = allInputFiles().getLastByExtension("idf");
        std::string lastEnergyPlusWorkspacePathArgument = "--lastEnergyPlusWorkspacePath=" + toString(idf.fullPath);
        addParameter("ruby", lastEnergyPlusWorkspacePathArgument);
      } catch (const std::exception &) {
      }

      try {
        FileInfo sql = allInputFiles().getLastByFilename("eplusout.sql");
        std::string lastEnergyPlusSqlFilePathArgument = "--lastEnergyPlusSqlFilePath=" + toString(sql.fullPath);
        addParameter("ruby", lastEnergyPlusSqlFilePathArgument);
      } catch (const std::exception &) {
      }
    }

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
      typedef std::vector<boost::tuple<FileSelection, FileSource, std::string, std::string> > FileReqs;

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
          const Files &source = PickFileSource::pick(itr->get<1>(), allinputfiles, myInputFiles, parentInputFiles);

          Files found;
          switch (itr->get<0>().value())
          {
            case FileSelection::Last:
              found.append(source.getLastByRegex(itr->get<2>()));
              break;
            case FileSelection::All:
              found.append(source.getAllByRegex(itr->get<2>()));
              break;
          }

          m_inputfiles.push_back(std::make_pair(i, std::make_pair(found, itr->get<3>())));
        } catch (const std::exception &) {
        }
      }
    }

  }


} // detail
} // runmanager
} // openstudio



