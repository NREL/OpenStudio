/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP
#define GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP

#include "../utilities/core/Filesystem.hpp"

#include <map>
#include <memory>
#include <vector>

namespace openstudio {

using path = openstudio::filesystem::path;

class IddFileFactoryData;

struct IddFactoryOutFile
{
  std::string filename;
  openstudio::path finalPath;
  openstudio::path tempPath;
  openstudio::filesystem::ofstream tempFile;

  IddFactoryOutFile(const std::string& filename, const openstudio::path& outPath, const std::string& outFileHeader);

  // Prevent copy and assignment
  IddFactoryOutFile(const IddFactoryOutFile&) = delete;
  IddFactoryOutFile operator=(const IddFactoryOutFile&) = delete;

  std::string finalize(const std::string& oldChecksum);
};

/** Structure to hold GenerateIddFactory's output files as they are being written. */
struct GenerateIddFactoryOutFiles
{
  IddFactoryOutFile iddEnumsHxx;
  IddFactoryOutFile iddFieldEnumsHxx;
  IddFactoryOutFile iddFieldEnumsIxx;
  IddFactoryOutFile iddFactoryHxx;
  IddFactoryOutFile iddFactoryCxx;
  std::vector<std::shared_ptr<IddFactoryOutFile>> iddFactoryIddFileCxxs;
  std::map<std::string, std::pair<std::string, bool>> checksumMap;  // filename, (checksum, encountered)

  GenerateIddFactoryOutFiles(const path& outPath, const std::string& outFileHeader, const std::vector<IddFileFactoryData>& iddFiles);

  void finalize();

  void finalizeIddFactoryOutFile(IddFactoryOutFile& outFile);

 private:
  openstudio::path m_fileIndexPath;
  void loadIddFactoryFileIndex();
  void writeIddFactoryFileIndex();
};

}  // namespace openstudio

#endif  // GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP
