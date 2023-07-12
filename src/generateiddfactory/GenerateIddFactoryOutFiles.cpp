/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GenerateIddFactoryOutFiles.hpp"
#include "IddFileFactoryData.hpp"

#include "../utilities/core/Checksum.hpp"

#include <boost/regex.hpp>

#include <sstream>

namespace openstudio {

IddFactoryOutFile::IddFactoryOutFile(const std::string& filename, const openstudio::path& outPath, const std::string& outFileHeader)
  : filename(filename), finalPath(outPath / path(filename)), tempPath(outPath / path(filename + ".temp")) {
  std::stringstream ss;
  tempFile.open(tempPath);
  if (!tempFile) {
    ss << "Unable to open '" << tempPath.string() << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  tempFile << outFileHeader << '\n';
}

std::string IddFactoryOutFile::finalize(const std::string& /*oldChecksum*/) {
  tempFile.close();
  std::string newChecksum = openstudio::checksum(tempPath);
  // ETH@20111122 Always copy for now. CMake/build process can't yet handle "sometimes generated"
  // files.
  bool copyFile = true;  // (newChecksum != oldChecksum);
  // cppcheck-suppress knownConditionTrueFalse
  if (copyFile) {
    if (openstudio::filesystem::exists(finalPath)) {
      openstudio::filesystem::remove(finalPath);
    }
    openstudio::filesystem::copy_file(tempPath, finalPath);
  }
  openstudio::filesystem::remove(tempPath);
  return newChecksum;
}

GenerateIddFactoryOutFiles::GenerateIddFactoryOutFiles(const path& outPath, const std::string& outFileHeader,
                                                       const std::vector<IddFileFactoryData>& iddFiles)
  : iddEnumsHxx("IddEnums.hxx", outPath, outFileHeader),
    iddFieldEnumsHxx("IddFieldEnums.hxx", outPath, outFileHeader),
    iddFieldEnumsIxx("IddFieldEnums.ixx", outPath, outFileHeader),
    iddFactoryHxx("IddFactory.hxx", outPath, outFileHeader),
    iddFactoryCxx("IddFactory.cxx", outPath, outFileHeader),
    m_fileIndexPath(outPath / path("IddFactoryFileIndex.hxx")) {
  for (const IddFileFactoryData& iddFile : iddFiles) {
    std::shared_ptr<IddFactoryOutFile> cxxFile(new IddFactoryOutFile("IddFactory_" + iddFile.fileName() + ".cxx", outPath, outFileHeader));
    iddFactoryIddFileCxxs.push_back(cxxFile);
  }

  loadIddFactoryFileIndex();
}

void GenerateIddFactoryOutFiles::finalize() {
  finalizeIddFactoryOutFile(iddEnumsHxx);
  finalizeIddFactoryOutFile(iddFieldEnumsHxx);
  finalizeIddFactoryOutFile(iddFieldEnumsIxx);
  finalizeIddFactoryOutFile(iddFactoryHxx);
  finalizeIddFactoryOutFile(iddFactoryCxx);
  for (std::shared_ptr<IddFactoryOutFile>& cxxFile : iddFactoryIddFileCxxs) {
    finalizeIddFactoryOutFile(*cxxFile);
  }

  writeIddFactoryFileIndex();
}

void GenerateIddFactoryOutFiles::finalizeIddFactoryOutFile(IddFactoryOutFile& outFile) {
  std::string cs = checksumMap[outFile.filename].first;
  cs = outFile.finalize(cs);
  checksumMap[outFile.filename] = std::pair<std::string, bool>(cs, true);
}

void GenerateIddFactoryOutFiles::loadIddFactoryFileIndex() {
  if (openstudio::filesystem::exists(m_fileIndexPath)) {
    openstudio::filesystem::ifstream fileIndex(m_fileIndexPath);
    std::string line;
    boost::regex re("// ([^,]*),(.*)");
    boost::smatch m;
    while (std::getline(fileIndex, line)) {
      if (boost::regex_match(line, m, re)) {
        std::string filename(m[1].first, m[1].second);
        std::string cs(m[2].first, m[2].second);
        checksumMap[filename] = std::pair<std::string, bool>(cs, false);
      }
    }
  }
}

void GenerateIddFactoryOutFiles::writeIddFactoryFileIndex() {
  openstudio::filesystem::ofstream fileIndex(m_fileIndexPath);
  for (auto it = checksumMap.begin(), itEnd = checksumMap.end(); it != itEnd; ++it) {
    if (it->second.second) {
      fileIndex << "// " << it->first << "," << it->second.first << '\n';
    }
  }
}

}  // namespace openstudio
