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

#ifndef UTILITIES_CORE_COMMANDLINE_HPP
#define UTILITIES_CORE_COMMANDLINE_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/program_options.hpp>

#include <string>
#include <vector>

/** Extend boost::program_options namespace to include platform specific functions for parsing
*   command line arguments.  Assume UTF-16LE on Windows and UTF-8 elsewhere. 
**/

namespace boost{
namespace program_options{

#ifdef _WINDOWS
  typedef std::wstring string;
  typedef std::vector<std::wstring> string_vector;

  boost::program_options::typed_value<string, wchar_t>* value(string* str){
    return boost::program_options::wvalue<string>(str);
  }
  boost::program_options::typed_value<string_vector, wchar_t>* value(string_vector* vec){
    return boost::program_options::wvalue<string_vector>(vec)->multitoken();
  }
#else
  typedef std::string string;
  typedef std::vector<std::string> string_vector;

  boost::program_options::typed_value<string, char>* value(string* str){
    return boost::program_options::value<string>(str);
  }
  boost::program_options::typed_value<string_vector, char>* value(string_vector* vec){
    return boost::program_options::value<string_vector>(vec)->multitoken();
  }
#endif
}
}

#endif // UTILITIES_CORE_COMMANDLINE_HPP
