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

      /// Adds t_localPath to the ZipFile, placing it at relative location t_destinationPath
      /// in the archive.
      void addFile(const openstudio::path &t_localPath, const openstudio::path &t_destinationPath);

      /// Recursively adds all files in t_localDir to the ZipFile, placing them in the archive
      /// relative to t_destinationDir.
      void addDirectory(const openstudio::path& t_localDir, const openstudio::path& t_destinationDir);

    private:
      void *m_zipFile;

  };

}

#endif

