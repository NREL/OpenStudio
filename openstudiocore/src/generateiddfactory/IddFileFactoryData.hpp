/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP
#define GENERATEIDDFACTORY_IDDFILEFACTORYDATA_HPP

#include "../utilities/core/Filesystem.hpp"
#include "GenerateIddFactoryOutFiles.hpp"


#include <vector>

namespace openstudio {

typedef openstudio::filesystem::path path;
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
