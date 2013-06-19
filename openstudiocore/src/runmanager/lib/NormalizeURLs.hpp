#ifndef OPENSTUDIO_RUNMANAGER_NORMALIZEURLS_HPP
#define OPENSTUDIO_RUNMANAGER_NORMALIZEURLS_HPP

#include <QUrl>
#include "FileInfo.hpp"
#include <utilities/idf/URLSearchPath.hpp>
#include <utilities/core/UUID.hpp>

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


#endif


