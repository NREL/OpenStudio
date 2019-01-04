/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ZipFile.hpp"
#include "FilesystemHelpers.hpp"

#include <zlib/contrib/minizip/zip.h>



namespace openstudio {

  ZipFile::ZipFile(const openstudio::path &filename, bool add)
    : m_zipFile(zipOpen(openstudio::toString(filename).c_str(), add?APPEND_STATUS_ADDINZIP:APPEND_STATUS_CREATE))
  {
    if (!m_zipFile) {
      throw std::runtime_error("ZipFile " + openstudio::toString(filename) + " could not be opened");
    }
  }

  ZipFile::~ZipFile()
  {
    zipClose(m_zipFile, nullptr);
  }

  void ZipFile::addFile(const openstudio::path &localPath, const openstudio::path &destinationPath)
  {
    if (zipOpenNewFileInZip(m_zipFile, openstudio::toString(destinationPath).c_str(),
          nullptr,
          nullptr, 0,
          nullptr, 0,
          nullptr,
          Z_DEFLATED,
          Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
      throw std::runtime_error("Unable to create new file in archive: " + openstudio::toString(destinationPath));
    }

    try {
      std::ifstream ifs(openstudio::toSystemFilename(localPath), std::ios_base::in | std::ios_base::binary);

      if (!ifs.is_open() || ifs.fail())
      {
        throw std::runtime_error("Unable to open local file: " + openstudio::toString(localPath));
      }

      while (!ifs.eof())
      {
        std::vector<char> buffer(1024);
        ifs.read(&buffer.front(), buffer.size());
        std::streamsize bytesread = ifs.gcount();

        if (ifs.fail() && !ifs.eof())
        {
          throw std::runtime_error("Error reading from local file: " + openstudio::toString(localPath));
        }

        zipWriteInFileInZip(m_zipFile, &buffer.front(), static_cast<unsigned int>(bytesread));
      }
    } catch (...) {
      zipCloseFileInZip(m_zipFile);
      throw;
    }

    zipCloseFileInZip(m_zipFile);
  }

  void ZipFile::addDirectory(const openstudio::path& localDir, const openstudio::path& destinationDir) {
    // following conventions in openstudio::copyDirectory

    for (const auto& file: openstudio::filesystem::recursive_directory_files(localDir)) {
      const auto srcItemPath = localDir / file;
      const auto dstItemPath = destinationDir / file;
      addFile(srcItemPath, dstItemPath);
    }
  }


}


