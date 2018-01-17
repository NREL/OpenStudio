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

#include "NormalizeURLs.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include <boost/filesystem.hpp>
#include <boost/timer.hpp>
#include <fstream>

namespace openstudio {
namespace runmanager {

  NormalizeURLs::NormalizeURLs()
  {
  }

  FileInfo NormalizeURLs::normalize(const FileInfo &t_infile, const std::vector<URLSearchPath> &t_search_paths, const openstudio::UUID &t_uuid)
  {
    FileInfo retval = t_infile;

    std::string fileExtension = toString(t_infile.fullPath.extension());

    if (fileExtension == ".osm" || fileExtension == ".idf")
    {
      LOG(Debug, "Analyzing file for URLs to update " << toString(t_infile.fullPath));

      boost::timer t;
      OptionalIdfFile idf = IdfFile::load(t_infile.fullPath);
      double elapsed = t.elapsed();
      LOG(Debug, "time to load idf: " << elapsed);

      if (idf)
      {
        Workspace ws(*idf);
        LOG(Debug, "Successfully loaded idf file into workspace containing " << ws.numObjects() << " objects");


        t.restart();

        /// \todo This locateUrls call needs to be eventually updated to fixup the urls to be relative, for various reasons
        ///       involving remote jobs and the like
        std::vector<std::pair<QUrl, openstudio::path> > paths = ws.locateUrls(t_search_paths, false, t_infile.fullPath);
        elapsed = t.elapsed();

        LOG(Debug, "time to locate URL's: " << elapsed);

        if (!paths.empty())
        {
          openstudio::path dir = openstudio::tempDir() / toPath("OpenStudioRunManager") / toPath(toString(t_uuid));
          boost::filesystem::create_directories(dir);
          openstudio::path newfilename = dir / toPath(t_infile.filename);
          LOG(Debug, "Writing out newly modified file: " << toString(newfilename));
          std::ofstream ofs(toString(newfilename).c_str(), std::ios_base::trunc);
          if (ofs.is_open() && ofs.good())
          {
            ofs << ws;
            ofs.close();
          } else {
            throw std::runtime_error("NormalizeURLs: Unable to write updated file out: " + toString(newfilename));
          }
          FileInfo fi(newfilename, retval.key);
          fi.requiredFiles = retval.requiredFiles;

          for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = paths.begin();
               itr != paths.end();
               ++itr)
          {
            fi.addRequiredFile(itr->first, itr->second);
          }

          retval = fi;
        } else {
          LOG(Info, "No URLs found in input");
        }
      }
    }

    return retval;
  }

  openstudio::path NormalizeURLs::getFilename(const QUrl &t_url, const openstudio::path &t_filename)
  {
    const std::map<QUrl, openstudio::path>::const_iterator itr = m_url_map.find(t_url);

    if (itr != m_url_map.end())
    {
      // we've already mapped this url, let's return the same one
      return itr->second;
    }

    const size_t existingcount = m_filenames.count(t_filename);

    if (existingcount == 0)
    {
      m_filenames.insert(t_filename);
      m_url_map[t_url] = t_filename;
      return t_filename;
    } else {
      openstudio::path newfilename 
        = toPath(t_filename.stem().string() + toPath("-" + boost::lexical_cast<std::string>(existingcount)).string() + t_filename.extension().string());

      // Make sure both the newly generated file name and the passed in file name are tracked for counting purposes
      m_filenames.insert(newfilename);
      m_filenames.insert(t_filename);

      m_url_map[t_url] = newfilename;
      return newfilename;
    }

  }

    
}
}




