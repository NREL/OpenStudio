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

#include "RubyJob.hpp"
#include "RubyJobUtils.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include <utilities/time/DateTime.hpp>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {

  RubyJob::RubyJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::Ruby, t_tools, "ruby", t_params, t_files, false, t_restoreData)
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
    : ToolBasedJob(t_uuid, t_jobType, t_tools, "ruby", t_params, t_files, false, t_restoreData)
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

    for (std::vector<std::pair<Files, std::string> >::const_iterator pairs = m_inputfiles.begin();
         pairs != m_inputfiles.end();
         ++pairs)
    {
      std::vector<FileInfo> files = pairs->first.files();
      for (std::vector<FileInfo>::const_iterator itr = files.begin();
          itr != files.end();
          ++itr)
      {
        std::string outfilename = pairs->second.empty()?itr->filename:pairs->second;

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
      addRequiredFile(inputfiles.getLastByExtension("rb"), toPath("in.rb"));
      addParameter("ruby", toString("in.rb"));
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


    typedef std::vector<boost::tuple<FileSelection, FileSource, std::string, std::string> > FileReqs;
    FileReqs inputFiles = t_rjb.inputFiles();

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

        m_inputfiles.push_back(std::make_pair(found, itr->get<3>()));
      } catch (const std::exception &) {
      }

    }

  }


} // detail
} // runmanager
} // openstudio



