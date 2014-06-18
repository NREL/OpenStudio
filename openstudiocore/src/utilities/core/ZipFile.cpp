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

#include "ZipFile.hpp"

#include <zlib/zconf.h>
#include <zlib/zlib.h>
#include <zlib/contrib/minizip/zip.h>

#include <QDir>

#include <fstream>

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
      std::ifstream ifs(openstudio::toString(localPath).c_str(), std::ios_base::in | std::ios_base::binary);

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
    QDir srcDir(toQString(localDir));

    for (const QFileInfo& info : srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
      QString srcItemPath = toQString(localDir) + "/" + info.fileName();
      QString dstItemPath = toQString(destinationDir) + "/" + info.fileName();
      if (info.isDir()) {
        addDirectory(toPath(srcItemPath),toPath(dstItemPath));
      }
      else if (info.isFile()) {
        addFile(toPath(srcItemPath),toPath(dstItemPath));
      }
    }
  }


}


