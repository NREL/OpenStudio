/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP
#define GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP

#include "../utilities/core/Filesystem.hpp"
#include "GenerateIddFactoryOutFiles.hpp"

#include <vector>

namespace openstudio {

using path = openstudio::filesystem::path;
using StringPair = std::pair<std::string, std::string>;

class IddFileFactoryData
{
 public:
  /** Parses input argument fileNameAndPathPair */
  IddFileFactoryData(const std::string& fileNameAndPathPair);

  /** Opens and parses actual IDD file, saving data for later use, and writing out object-level
   *  data (create functions, field enums) */
  void parseFile(const path& outPath, const std::string& outFileHeader, GenerateIddFactoryOutFiles& outFiles, int iddFileIndex);

  std::string fileName() const;

  std::string version() const;

  std::string header() const;

  std::vector<StringPair> objectNames() const;

  using FileNameRemovedObjectsPair = std::pair<std::string, std::vector<std::string>>;

  unsigned numIncludedFiles() const;

  FileNameRemovedObjectsPair includedFile(unsigned index) const;

 private:
  std::string m_fileName;
  openstudio::path m_filePath;

  std::string m_version;
  std::string m_header;
  std::vector<StringPair> m_objectNames;  // first is cleaned version
  std::vector<FileNameRemovedObjectsPair> m_includedFiles;

  static std::string m_convertName(const std::string& originalName);
  static std::string m_readyLineForOutput(const std::string& line);
};

using IddFileFactoryDataVector = std::vector<IddFileFactoryData>;

}  // namespace openstudio

#endif  // GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP
