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

#ifndef GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP
#define GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP

#include <generateiddfactory/GenerateIddFactoryOutFiles.hpp>

#include <boost/filesystem/path.hpp>

#include <vector>

namespace openstudio {

typedef boost::filesystem::path path;
typedef std::pair<std::string,std::string> StringPair;

class IddFileFactoryData {
 public:
  /** Parses input argument fileNameAndPathPair */
  IddFileFactoryData(const std::string& fileNameAndPathPair);

  /** Opens and parses actual IDD file, saving data for later use, and writing out object-level
   *  data (create functions, field enums) */
  void parseFile(const path& outPath,
                 const std::string& outFileHeader,
                 GenerateIddFactoryOutFiles& outFiles,
                 int iddFileIndex);

  std::string fileName() const;

  std::string version() const;

  std::string header() const;

  std::vector<StringPair> objectNames() const;

  typedef std::pair<std::string,std::vector<std::string> > FileNameRemovedObjectsPair;

  unsigned numIncludedFiles() const;

  FileNameRemovedObjectsPair includedFile(unsigned index) const;

 private:
  std::string m_fileName;
  openstudio::path m_filePath;

  std::string m_version;
  std::string m_header;
  std::vector<StringPair> m_objectNames; // first is cleaned version
  std::vector<FileNameRemovedObjectsPair> m_includedFiles;

  std::string m_convertName(const std::string& originalName) const;
  std::string m_readyLineForOutput(const std::string& line) const;
};

typedef std::vector<IddFileFactoryData> IddFileFactoryDataVector;

}

#endif // GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP
