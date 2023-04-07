/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
