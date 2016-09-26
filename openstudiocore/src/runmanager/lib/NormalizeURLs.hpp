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


