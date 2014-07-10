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

#ifndef RUNMANAGER_LIB_NORMALIZEURLS_HPP
#define RUNMANAGER_LIB_NORMALIZEURLS_HPP

#include <QUrl>
#include "FileInfo.hpp"
#include "../../utilities/idf/URLSearchPath.hpp"
#include "../../utilities/core/UUID.hpp"

namespace openstudio {
namespace runmanager {

  /// Utility for normalizing remote and local URLs of an IDF or OSM to well defined local locations
  /// while collecting the set of moved files and making sure that no relocated files conflict.
  class NormalizeURLs
  {
    public:
      NormalizeURLs();

      /// Takes the input file and analyzes it for URLs that need to be downloaded or copied to a new location
      ///
      /// \param[in] t_infile IDF/OSM to analyze
      /// \param[in] t_search_paths Search locations to find URLs in
      /// \param[in] t_uuid UUID of the Job this input file is being analyzed for
      FileInfo normalize(const FileInfo &t_infile, const std::vector<URLSearchPath> &t_search_paths, const openstudio::UUID &t_uuid);

    private:
      REGISTER_LOGGER("openstudio.runmanager.NormalizeURLs");

      openstudio::path getFilename(const QUrl &t_url, const openstudio::path
          &t_filename);

      std::multiset<openstudio::path> m_filenames;
      std::map<QUrl, openstudio::path> m_url_map;

  };
}
}


#endif // RUNMANAGER_LIB_NORMALIZEURLS_HPP


