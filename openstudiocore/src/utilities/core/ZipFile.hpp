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

#ifndef UTILITIES_CORE_ZIPFILE_HPP
#define UTILITIES_CORE_ZIPFILE_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"

#include <vector>

namespace openstudio {
  class UTILITIES_API ZipFile 
  {
    public:
      /// Constructs a ZipFile object, opening the given file name. Throws if file could not be opened
      /// \param[in] filename filename to open
      /// \param[in] add If true, add new files to existing archive, if false, overwrite existing archive
      ZipFile(const openstudio::path &filename, bool add);

      ~ZipFile();

      /// Adds localPath to the ZipFile, placing it at relative location destinationPath
      /// in the archive.
      void addFile(const openstudio::path &localPath, const openstudio::path &destinationPath);

      /// Recursively adds all files in localDir to the ZipFile, placing them in the archive
      /// relative to destinationDir.
      void addDirectory(const openstudio::path& localDir, const openstudio::path& destinationDir);

    private:
      void *m_zipFile;

  };

}

#endif

