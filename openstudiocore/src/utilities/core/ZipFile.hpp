#ifndef UTILITIES_CORE_ZIPFILE_HPP
#define UTILITIES_CORE_ZIPFILE_HPP

#include <utilities/UtilitiesAPI.hpp>
#include "Path.hpp"

#include <vector>

namespace openstudio {
  class UTILITIES_API ZipFile 
  {
    public:
      /// Constructs a ZipFile object, opening the given file name. Throws if file could not be opened
      /// \param[in] t_filename filename to open
      /// \param[in] t_add If true, add new files to existing archive, if false, overwrite existing archive
      ZipFile(const openstudio::path &t_filename, bool t_add);

      ~ZipFile();

      /// Extracts all files in the archive to the given path, preserving relative paths.
      void addFile(const openstudio::path &t_localPath, const openstudio::path &t_destinationPath);

    private:
      void *m_zipFile;

  };

}

#endif

