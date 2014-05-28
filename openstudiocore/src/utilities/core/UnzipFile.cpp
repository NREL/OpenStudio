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

#include "UnzipFile.hpp"
#include <zlib/zconf.h>
#include <zlib/zlib.h>
#include <zlib/contrib/minizip/unzip.h>
#include <fstream>
#include <boost/filesystem.hpp>

#include <QDir>
#include <QFile>

namespace openstudio {

  UnzipFile::UnzipFile(const openstudio::path &filename)
    : m_unzFile(unzOpen(openstudio::toString(filename).c_str()))
  {
    if (!m_unzFile) {
      if (!boost::filesystem::exists(filename))
      {
        throw std::runtime_error("UnzipFile " + openstudio::toString(filename) + " does not exist, could not be opened");
      } else {
        throw std::runtime_error("UnzipFile " + openstudio::toString(filename) + "  exists, could not be opened");
      }
    }
  }

  UnzipFile::~UnzipFile()
  {
    unzClose(m_unzFile);
  }

  std::vector<openstudio::path> UnzipFile::extractAllFiles(const openstudio::path &outputPath) const
  {
    std::vector<openstudio::path> files = listFiles();

    std::vector<openstudio::path> retfiles;

    for (std::vector<openstudio::path>::const_iterator itr = files.begin();
         itr != files.end();
         ++itr)
    {
      if (toString(itr->filename())=="." || toString(itr->filename())=="/")
      {
        // This is a directory - skip it
      } else {
        retfiles.push_back(extractFile(*itr, outputPath));
      }
    }

    return retfiles;
  }

  openstudio::path UnzipFile::extractFile(const openstudio::path &filename, const openstudio::path &outputPath) const
  {
    if (unzLocateFile(m_unzFile, openstudio::toString(filename).c_str(), 1) != UNZ_OK)
    {
      throw std::runtime_error("File does not exist in archive: " + openstudio::toString(filename));
    }

    if (unzOpenCurrentFile(m_unzFile) != UNZ_OK)
    {
      throw std::runtime_error("Unable to open file in archive: " + openstudio::toString(filename));
    }

    try {
      bool cont = true;

      openstudio::path createdFile = outputPath / filename;

      QDir().mkpath(toQString(createdFile.parent_path()));

      QFile file(toQString(createdFile));
      file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);
      while (cont)
      {
        std::vector<char> buffer(1024);
        int bytesread = unzReadCurrentFile(m_unzFile, &buffer.front(), buffer.size());

        if (bytesread == 0)
        {
          cont = false;
        }
        else if (bytesread < 0)
        {
          throw std::runtime_error("Unable to read from file");
        }
        else
        {
          if (file.write(QByteArray(&buffer.front(), bytesread)) < 0)
          {
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

  }


  std::vector<openstudio::path> UnzipFile::listFiles() const
  {
    bool cont = unzGoToFirstFile(m_unzFile) == UNZ_OK;

    std::vector<openstudio::path> paths;

    do {
      unz_file_info file_info;
      std::vector<char> filename(300);

      unzGetCurrentFileInfo(m_unzFile, &file_info,
          &filename.front(), filename.size(),
          nullptr,0,
          nullptr,0);

      paths.push_back(openstudio::toPath(std::string(&filename.front(), file_info.size_filename)));
      cont = unzGoToNextFile(m_unzFile) == UNZ_OK;
    } while (cont);

    return paths;
  }


}


