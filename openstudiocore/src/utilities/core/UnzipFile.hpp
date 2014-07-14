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
      UnzipFile(const openstudio::path &filename);

      ~UnzipFile();

      /// Returns a list of files contained in the current archive
      std::vector<openstudio::path> listFiles() const;

      /// Extracts the given file to the specified path. Relative paths are preserved
      /// Example: extractFile("dir/filename", "outputpath") creates the file "outputpath/dir/filename"
      openstudio::path extractFile(const openstudio::path &filename, const openstudio::path &outputPath) const;

      /// Extracts all files in the archive to the given path, preserving relative paths.
      std::vector<openstudio::path> extractAllFiles(const openstudio::path &outputPath) const;

    private:
      void *m_unzFile;

  };

}

#endif

