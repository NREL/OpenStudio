/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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



