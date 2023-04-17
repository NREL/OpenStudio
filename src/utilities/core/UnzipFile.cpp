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

#include "UnzipFile.hpp"
#include <minizip/unzip.h>

namespace openstudio {

UnzipFile::UnzipFile(const openstudio::path& filename) : m_unzFile(unzOpen(openstudio::toString(filename).c_str())), m_chunksize(32768) {
  if (!m_unzFile) {
    if (!openstudio::filesystem::exists(filename)) {
      throw std::runtime_error("UnzipFile " + openstudio::toString(filename) + " does not exist, could not be opened");
    } else {
      throw std::runtime_error("UnzipFile " + openstudio::toString(filename) + "  exists, could not be opened");
    }
  }
}

UnzipFile::~UnzipFile() {
  unzClose(m_unzFile);
}

unsigned long UnzipFile::chunksize() const {
  return m_chunksize;
}
void UnzipFile::setChunksize(unsigned long chunksize) {
  m_chunksize = chunksize;
}

std::vector<openstudio::path> UnzipFile::extractAllFiles(const openstudio::path& outputPath) const {

  // We do not call listFiles() then for each extractFile, for performance reasons (some of the work would be done twice). cf #4456

  bool cont_files = unzGoToFirstFile(m_unzFile) == UNZ_OK;

  std::vector<openstudio::path> filesOnDisk;

  // Start by creating the output directory itself if need be
  openstudio::filesystem::create_directories(outputPath);

  std::vector<char> buffer(m_chunksize);
  std::vector<char> filenamebuffer(300);

  do {
    unz_file_info file_info;

    unzGetCurrentFileInfo(m_unzFile, &file_info, &filenamebuffer.front(), filenamebuffer.size(), nullptr, 0, nullptr, 0);

    std::string fileName(&filenamebuffer.front(), file_info.size_filename);

    openstudio::path zippedFileRelPath = openstudio::toPath(fileName);
    openstudio::path createdFilePath = outputPath / zippedFileRelPath;

    if ((fileName.back() == '/') || (fileName == ".")) {
      // This is a directory - skip it
    } else {
      // Entry is a file, so extract it.
      // First we do need to create the parent directory(ies). The order is not necessarilly good so we can't do it in the other branch of if above
      openstudio::filesystem::create_directories(createdFilePath.parent_path());

      // Open the zipped file
      if (unzOpenCurrentFile(m_unzFile) != UNZ_OK) {
        throw std::runtime_error("Unable to open file in archive: " + openstudio::toString(zippedFileRelPath));
      }

      try {
        bool cont_read = true;

        // Open the target file on disk
        openstudio::filesystem::ofstream file(createdFilePath, std::ios_base::trunc | std::ios_base::binary);
        while (cont_read) {

          int bytesread = unzReadCurrentFile(m_unzFile, &buffer.front(), buffer.size());

          if (bytesread == 0) {
            cont_read = false;
          } else if (bytesread < 0) {
            throw std::runtime_error("Unable to read from file: " + openstudio::toString(zippedFileRelPath));
          } else {
            file.write(&buffer.front(), bytesread);
            if (!file.good()) {
              throw std::runtime_error("Error writing to output file: " + toString(createdFilePath));
            }
          }
        }
        file.close();

        filesOnDisk.push_back(createdFilePath);
      } catch (...) {
        unzCloseCurrentFile(m_unzFile);
        throw;
      }
    }

    cont_files = unzGoToNextFile(m_unzFile) == UNZ_OK;
  } while (cont_files);

  return filesOnDisk;
}

openstudio::path UnzipFile::extractFile(const openstudio::path& filename, const openstudio::path& outputPath) const {
  if (unzLocateFile(m_unzFile, openstudio::toString(filename).c_str(), 1) != UNZ_OK) {
    throw std::runtime_error("File does not exist in archive: " + openstudio::toString(filename));
  }

  if (unzOpenCurrentFile(m_unzFile) != UNZ_OK) {
    throw std::runtime_error("Unable to open file in archive: " + openstudio::toString(filename));
  }

  std::vector<char> buffer(m_chunksize);

  try {
    bool cont = true;

    openstudio::path createdFile = outputPath / filename;
    openstudio::filesystem::create_directories(createdFile.parent_path());

    openstudio::filesystem::ofstream file(createdFile, std::ios_base::trunc | std::ios_base::binary);
    while (cont) {
      int bytesread = unzReadCurrentFile(m_unzFile, &buffer.front(), buffer.size());

      if (bytesread == 0) {
        cont = false;
      } else if (bytesread < 0) {
        throw std::runtime_error("Unable to read from file");
      } else {
        file.write(&buffer.front(), bytesread);
        if (!file.good()) {
          throw std::runtime_error("Error writing to output file: " + toString(createdFile));
        }
      }
    }
    file.close();

    return createdFile;
  } catch (...) {
    unzCloseCurrentFile(m_unzFile);
    throw;
  }

  unzCloseCurrentFile(m_unzFile);
  return openstudio::path{};
}

std::vector<openstudio::path> UnzipFile::listFiles() const {
  bool cont = unzGoToFirstFile(m_unzFile) == UNZ_OK;

  std::vector<openstudio::path> paths;

  std::vector<char> filename(300);

  do {
    unz_file_info file_info;

    unzGetCurrentFileInfo(m_unzFile, &file_info, &filename.front(), filename.size(), nullptr, 0, nullptr, 0);

    paths.push_back(openstudio::toPath(std::string(&filename.front(), file_info.size_filename)));
    cont = unzGoToNextFile(m_unzFile) == UNZ_OK;
  } while (cont);

  return paths;
}

}  // namespace openstudio
