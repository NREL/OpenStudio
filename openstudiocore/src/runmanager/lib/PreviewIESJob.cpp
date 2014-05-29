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

#include "PreviewIESJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  PreviewIESJob::PreviewIESJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::PreviewIES, t_tools, toolNames(),
        t_params, t_files, false, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("ies");
  }

  PreviewIESJob::~PreviewIESJob()
  {
    shutdownJob();
  }

  std::vector<std::string> PreviewIESJob::toolNames()
  {
    std::vector<std::string> vec;
    vec.push_back("ies2rad");
    vec.push_back("rad");
    vec.push_back("ra_image");
    return vec;
  }

  std::string PreviewIESJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion PreviewIESJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    std::vector<std::string> names = toolNames();
    if (std::find(names.begin(), names.end(), t_toolName) == names.end()) {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    return ToolVersion();
  }

  void PreviewIESJob::startHandlerImpl()
  {
    LOG(Debug, "Starting job");
    getFiles(allInputFiles());

    openstudio::path theoutdir = boost::filesystem::complete(outdir(true));

    boost::filesystem::create_directories(theoutdir);

    addParameter("ies2rad", "-o");
    addParameter("ies2rad", "ov");
    addParameter("ies2rad", toString(theoutdir/toPath("in.ies")));

    addParameter("rad", toString(theoutdir/toPath("ov.rif")));

    addParameter("ra_image", "-z");
    addParameter("ra_image", "-b");
    addParameter("ra_image", "ov_y.hdr");

#ifdef Q_OS_WIN
    addParameter("ra_image", "out.bmp");
#else
    addParameter("ra_image", "out.tiff");
#endif

    // copy ies to "in.ies"
    addRequiredFile(*m_ies, toPath("in.ies"));


    writeSurfFile(theoutdir/toPath("surf.rad"), 24);
    writeRifFile(theoutdir/toPath("ov.rif"), theoutdir/toPath("ov.rad"), theoutdir/toPath("surf.rad"),
        "Z", "y", theoutdir/toPath("ov.oct"), "-ab 1 -ds .15");
  }

  void PreviewIESJob::writeSurfFile(const openstudio::path &t_path, int boxsize) const
  {
    LOG(Debug, "Writing out surffile to " << toString(t_path));
    std::ofstream outfile(toString(t_path).c_str(), std::ios_base::trunc);

    outfile << "void plastic surf\n"
            << "0\n"
            << "0\n"
            << "5\n"
            << ".5 .5 .5 0 0\n"

            << "surf polygon floor\n"
            << "0\n"
            << "0\n"
            << "12\t -" << boxsize << " " << boxsize << " -" << boxsize << "\n"
            << "\t -" << boxsize << " -" << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " -" << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " " << boxsize << " -" << boxsize << "\n"

            << "surf polygon clg\n"
            << "0\n"
            << "0\n"
            << "12\t -" << boxsize << " " << boxsize << " " << boxsize << "\n"
            << "\t -" << boxsize << " -" << boxsize << " " << boxsize << "\n"
            << "\t " << boxsize << " -" << boxsize << " " << boxsize << "\n"
            << "\t " << boxsize << " " << boxsize << " " << boxsize << "\n"

            << "surf polygon wall-north\n"
            << "0\n"
            << "0\n"
            << "12\t -" << boxsize << " " << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " " << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " " << boxsize << " " << boxsize << "\n"
            << "\t -" << boxsize << " " << boxsize << " " << boxsize << "\n"

            << "surf polygon wall-east\n"
            << "0\n"
            << "0\n"
            << "12\t " << boxsize << " " << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " -" << boxsize << " -" << boxsize << "\n"
            << "\t " << boxsize << " -" << boxsize << " " << boxsize << "\n"
            << "\t " << boxsize << " " << boxsize << " " << boxsize << "\n"


            << "surf polygon wall-west\n"
            << "0\n"
            << "0\n"
            << "12\t -" << boxsize << " " << boxsize << " -" << boxsize << "\n"
            << "\t -" << boxsize << " " << boxsize << " " << boxsize << "\n"
            << "\t -" << boxsize << " -" << boxsize << " " << boxsize << "\n"
            << "\t -" << boxsize << " -" << boxsize << " -" << boxsize << "\n"

            ;
  }


  void PreviewIESJob::writeRifFile(const openstudio::path &t_path, const openstudio::path &t_radfile,
      const openstudio::path &t_surffile, const std::string &t_up, const std::string &t_view, 
      const openstudio::path &t_octree,
      const std::string &t_rendopts) const
  {
    LOG(Debug, "Writing out rif file to " << toString(t_path));
   
    std::ofstream outfile(toString(t_path).c_str(), std::ios_base::trunc);

    outfile << "scene= \"" << toString(t_radfile) << "\" \"" << toString(t_surffile) << "\"\n"
            << "EXPOSURE= 1\n"
            << "UP= " << t_up << "\n"
            << "view= " << t_view << "\n"
            << "OCTREE= \"" << toString(t_octree) << "\"\n"
            << "oconv= -f -r 16384\n"
            << "render= " << t_rendopts << "\n"

            ;
  }

  void PreviewIESJob::basePathChanged()
  {
    m_ies.reset();   
  }

  void PreviewIESJob::getFiles(const Files &t_files) const
  {
    if (!m_ies)
    {
      try {
        m_ies = t_files.getLastByExtension("ies");
      } catch (const std::exception &) {
      }
    }
  }

} // detail
} // runmanager
} // openstudio



