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
