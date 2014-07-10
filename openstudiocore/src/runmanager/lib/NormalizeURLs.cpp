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




