/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_UNZIPFILE_HPP
#define UTILITIES_CORE_UNZIPFILE_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"

#include <vector>

namespace openstudio {

class UTILITIES_API UnzipFile
{
 public:
  /// Constructs a UnzipFile object, opening the given file name. Throws if file could not be opened
  /// \param[in] filename filename to open
  UnzipFile(const openstudio::path& filename);

  ~UnzipFile();

  /// Returns a list of files contained in the current archive
  std::vector<openstudio::path> listFiles() const;

  /// Extracts the given file to the specified path. Relative paths are preserved
  /// Example: extractFile("dir/filename", "outputpath") creates the file "outputpath/dir/filename"
  openstudio::path extractFile(const openstudio::path& filename, const openstudio::path& outputPath) const;

  /// Extracts all files in the archive to the given path, preserving relative paths.
  std::vector<openstudio::path> extractAllFiles(const openstudio::path& outputPath) const;

  unsigned long chunksize() const;
  void setChunksize(unsigned long chunksize);

 private:
  void* m_unzFile;
  unsigned long m_chunksize;
};

}  // namespace openstudio

#endif
