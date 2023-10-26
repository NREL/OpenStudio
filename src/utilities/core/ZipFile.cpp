/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZipFile.hpp"
#include "FilesystemHelpers.hpp"

#include <minizip/zip.h>

namespace openstudio {

ZipFile::ZipFile(const openstudio::path& filename, bool add)
  : m_zipFile(zipOpen(openstudio::toString(filename).c_str(), add ? APPEND_STATUS_ADDINZIP : APPEND_STATUS_CREATE)) {
  if (!m_zipFile) {
    throw std::runtime_error("ZipFile " + openstudio::toString(filename) + " could not be opened");
  }
}

ZipFile::~ZipFile() {
  zipClose(m_zipFile, nullptr);
}

void ZipFile::addFile(const openstudio::path& localPath, const openstudio::path& destinationPath) {
  if (zipOpenNewFileInZip(m_zipFile, openstudio::toString(destinationPath).c_str(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED,
                          Z_DEFAULT_COMPRESSION)
      != ZIP_OK) {
    throw std::runtime_error("Unable to create new file in archive: " + openstudio::toString(destinationPath));
  }

  try {
    std::ifstream ifs(openstudio::toSystemFilename(localPath), std::ios_base::in | std::ios_base::binary);

    if (!ifs.is_open() || ifs.fail()) {
      throw std::runtime_error("Unable to open local file: " + openstudio::toString(localPath));
    }

    while (!ifs.eof()) {
      std::vector<char> buffer(1024);
      ifs.read(&buffer.front(), buffer.size());
      std::streamsize bytesread = ifs.gcount();

      if (ifs.fail() && !ifs.eof()) {
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

  for (const auto& file : openstudio::filesystem::recursive_directory_files(localDir)) {
    const auto srcItemPath = localDir / file;
    const auto dstItemPath = destinationDir / file;
    addFile(srcItemPath, dstItemPath);
  }
}

}  // namespace openstudio
