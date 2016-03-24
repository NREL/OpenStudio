/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_CORE_FILESYSTEM_HPP
#define UTILITIES_CORE_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>


#include "../UtilitiesAPI.hpp"


namespace openstudio {
  typedef boost::filesystem::path path;

namespace filesystem {

  // types
  using boost::filesystem::path;
  using boost::filesystem::wpath;

  using boost::filesystem::basic_ofstream;

  using boost::filesystem::ofstream;
  using boost::filesystem::ifstream;

  using boost::filesystem::recursive_directory_iterator;
  using boost::filesystem::filesystem_error;
  using boost::filesystem::copy_option;

  // functions
  using boost::filesystem::basename;
  using boost::filesystem::canonical;
  using boost::filesystem::complete;
  using boost::filesystem::copy;
  using boost::filesystem::copy_file;
  using boost::filesystem::create_directories;
  using boost::filesystem::create_directory;
  using boost::filesystem::directory_iterator;
  using boost::filesystem::equivalent;
  using boost::filesystem::exists;
  using boost::filesystem::extension;
  using boost::filesystem::file_size;
  using boost::filesystem::initial_path;
  using boost::filesystem::is_directory;
  using boost::filesystem::is_empty;
  using boost::filesystem::is_regular_file;
  using boost::filesystem::is_symlink;
  using boost::filesystem::last_write_time;
  using boost::filesystem::remove;
  using boost::filesystem::remove_all;
  using boost::filesystem::system_complete;
  using boost::filesystem::temp_directory_path;

  UTILITIES_API std::vector<uint8_t> read_file(const openstudio::path &t_path);

}
}

#endif

    

