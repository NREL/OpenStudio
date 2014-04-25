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

#ifndef GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP
#define GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP

#include <map>
#include <vector>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

namespace openstudio {

typedef boost::filesystem::path path;

class IddFileFactoryData;

struct IddFactoryOutFile {
  std::string filename;
  openstudio::path finalPath;
  openstudio::path tempPath;
  boost::filesystem::ofstream tempFile;

  IddFactoryOutFile(const std::string& filename,
                    const openstudio::path& outPath,
                    const std::string& outFileHeader);

  IddFactoryOutFile(const IddFactoryOutFile& other) { 
    throw std::runtime_error("Cannot copy IddFactoryOutFiles.");
  }

  IddFactoryOutFile operator=(const IddFactoryOutFile& other) { 
    throw std::runtime_error("Cannot copy IddFactoryOutFiles.");
  }

  std::string finalize(const std::string& oldChecksum);
};

/** Structure to hold GenerateIddFactory's output files as they are being written. */
struct GenerateIddFactoryOutFiles {
  IddFactoryOutFile iddEnumsHxx;
  IddFactoryOutFile iddFieldEnumsHxx;
  IddFactoryOutFile iddFieldEnumsIxx;
  IddFactoryOutFile iddFactoryHxx;
  IddFactoryOutFile iddFactoryCxx;
  std::vector< boost::shared_ptr<IddFactoryOutFile> > iddFactoryIddFileCxxs;
  std::map<std::string,std::pair<std::string,bool> > checksumMap; // filename, (checksum, encountered)
  
  GenerateIddFactoryOutFiles(const path& outPath,
                             const std::string& outFileHeader,
                             const std::vector<IddFileFactoryData>& iddFiles);

  void finalize();

  void finalizeIddFactoryOutFile(IddFactoryOutFile& outFile);

 private:
  openstudio::path m_fileIndexPath;
  void loadIddFactoryFileIndex();
  void writeIddFactoryFileIndex();
};

} // openstudio

#endif // GENERATEIDDFACTORY_GENERATEIDDFACTORYOUTFILES_HPP
