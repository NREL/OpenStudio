/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_ZIPFILE_HPP
#define UTILITIES_CORE_ZIPFILE_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"

#include <vector>

namespace openstudio {
class UTILITIES_API ZipFile
{
 public:
  /// Constructs a ZipFile object, opening the given file name. Throws if file could not be opened
  /// \param[in] filename filename to open
  /// \param[in] add If true, add new files to existing archive, if false, overwrite existing archive
  ZipFile(const openstudio::path& filename, bool add);

  ~ZipFile();

  /// Adds localPath to the ZipFile, placing it at relative location destinationPath
  /// in the archive.
  void addFile(const openstudio::path& localPath, const openstudio::path& destinationPath);

  /// Recursively adds all files in localDir to the ZipFile, placing them in the archive
  /// relative to destinationDir.
  void addDirectory(const openstudio::path& localDir, const openstudio::path& destinationDir);

 private:
  void* m_zipFile;
};

}  // namespace openstudio

#endif
