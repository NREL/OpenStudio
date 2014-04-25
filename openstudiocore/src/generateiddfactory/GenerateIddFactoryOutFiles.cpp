/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <generateiddfactory/GenerateIddFactoryOutFiles.hpp>
#include <generateiddfactory/IddFileFactoryData.hpp>

#include <utilities/core/Checksum.hpp>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>

#include <sstream>

namespace openstudio {

IddFactoryOutFile::IddFactoryOutFile(const std::string& filename,
                                     const openstudio::path& outPath,
                                     const std::string& outFileHeader) 
  : filename(filename), 
    finalPath(outPath / path(filename)), 
    tempPath(outPath / path(filename + ".temp"))
{
  std::stringstream ss;
  tempFile.open(tempPath);
  if (!tempFile) {
    ss << "Unable to open '" << tempPath.string() << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  tempFile << outFileHeader << std::endl;
}

std::string IddFactoryOutFile::finalize(const std::string& oldChecksum) {
  tempFile.close();
  std::string newChecksum = openstudio::checksum(tempPath);
  // ETH@20111122 Always copy for now. CMake/build process can't yet handle "sometimes generated" 
  // files.
  bool copyFile = true; // (newChecksum != oldChecksum);
  if (copyFile) {
    if (boost::filesystem::exists(finalPath)) {
      boost::filesystem::remove(finalPath);
    }
    boost::filesystem::copy_file(tempPath,finalPath);
  }
  boost::filesystem::remove(tempPath);
  return newChecksum;
}

GenerateIddFactoryOutFiles::GenerateIddFactoryOutFiles(
    const path& outPath,
    const std::string& outFileHeader,
    const std::vector<IddFileFactoryData>& iddFiles)
  : iddEnumsHxx("IddEnums.hxx",outPath,outFileHeader),
    iddFieldEnumsHxx("IddFieldEnums.hxx",outPath,outFileHeader),
    iddFieldEnumsIxx("IddFieldEnums.ixx",outPath,outFileHeader),
    iddFactoryHxx("IddFactory.hxx",outPath,outFileHeader),
    iddFactoryCxx("IddFactory.cxx",outPath,outFileHeader),
    m_fileIndexPath(outPath / path("IddFactoryFileIndex.hxx"))
{
  BOOST_FOREACH(const IddFileFactoryData& iddFile,iddFiles) {
    boost::shared_ptr<IddFactoryOutFile> cxxFile(new 
        IddFactoryOutFile("IddFactory_" + iddFile.fileName() + ".cxx",
                          outPath,
                          outFileHeader));
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
  BOOST_FOREACH(boost::shared_ptr<IddFactoryOutFile>& cxxFile,iddFactoryIddFileCxxs) {
    finalizeIddFactoryOutFile(*cxxFile);
  }

  writeIddFactoryFileIndex();
}

void GenerateIddFactoryOutFiles::finalizeIddFactoryOutFile(IddFactoryOutFile& outFile) {
  std::string cs = checksumMap[outFile.filename].first;
  cs = outFile.finalize(cs);
  checksumMap[outFile.filename] = std::pair<std::string,bool>(cs,true);
}

void GenerateIddFactoryOutFiles::loadIddFactoryFileIndex() {
  if (boost::filesystem::exists(m_fileIndexPath)) {
    boost::filesystem::ifstream fileIndex(m_fileIndexPath);
    std::string line;
    boost::regex re("// ([^,]*),(.*)");
    boost::smatch m;
    while (std::getline(fileIndex,line)) {
      if (boost::regex_match(line,m,re)) {
        std::string filename(m[1].first,m[1].second);
        std::string cs(m[2].first,m[2].second);
        checksumMap[filename] = std::pair<std::string,bool>(cs,false);
      }
    }
  }
}

void GenerateIddFactoryOutFiles::writeIddFactoryFileIndex() {
  boost::filesystem::ofstream fileIndex(m_fileIndexPath);
  for (std::map<std::string,std::pair<std::string,bool> >::const_iterator it = checksumMap.begin(),
       itEnd = checksumMap.end(); it != itEnd; ++it)
  {
    if (it->second.second) {
      fileIndex << "// " << it->first << "," << it->second.first << std::endl;
    }
  }
}

} // openstudio
