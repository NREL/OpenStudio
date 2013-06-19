#include "ZipFile.hpp"
#include <zlib/zconf.h>
#include <zlib/zlib.h>
#include <zlib/contrib/minizip/zip.h>
#include <fstream>

namespace openstudio {

  ZipFile::ZipFile(const openstudio::path &t_filename, bool t_add)
    : m_zipFile(zipOpen(openstudio::toString(t_filename).c_str(), t_add?APPEND_STATUS_ADDINZIP:APPEND_STATUS_CREATE))
  {
    if (!m_zipFile) {
      throw std::runtime_error("ZipFile " + openstudio::toString(t_filename) + " could not be opened");
    }
  }

  ZipFile::~ZipFile()
  {
    zipClose(m_zipFile, 0);
  }

  void ZipFile::addFile(const openstudio::path &t_localPath, const openstudio::path &t_destinationPath)
  {
    if (zipOpenNewFileInZip(m_zipFile, openstudio::toString(t_destinationPath).c_str(),
          0,
          0, 0,
          0, 0,
          0,
          Z_DEFLATED,
          Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
      throw std::runtime_error("Unable to create new file in archive: " + openstudio::toString(t_destinationPath));
    }

    try {
      std::ifstream ifs(openstudio::toString(t_localPath).c_str(), std::ios_base::in | std::ios_base::binary);

      if (!ifs.is_open() || ifs.fail())
      {
        throw std::runtime_error("Unable to open local file: " + openstudio::toString(t_localPath));
      }

      while (!ifs.eof())
      {
        std::vector<char> buffer(1024);
        ifs.read(&buffer.front(), buffer.size());
        std::streamsize bytesread = ifs.gcount();

        if (ifs.fail() && !ifs.eof())
        {
          throw std::runtime_error("Error reading from local file: " + openstudio::toString(t_localPath));
        }

        zipWriteInFileInZip(m_zipFile, &buffer.front(), static_cast<unsigned int>(bytesread));
      }
    } catch (...) {
      zipCloseFileInZip(m_zipFile);
      throw;
    }

    zipCloseFileInZip(m_zipFile);
  }


}


