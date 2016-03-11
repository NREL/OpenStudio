/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_CORE_ENCODING_HPP
#define UTILITIES_CORE_ENCODING_HPP

#include "../UtilitiesAPI.hpp"

#include <string>
#include <vector>

#include <QByteArray>

/** \file Encoding.hpp
 *  
 *  Utilities for calls to base 64 and compression
 */

namespace openstudio {


  UTILITIES_API QByteArray toQByteArray(const std::string &t_str);

  UTILITIES_API std::string toStdString(const QByteArray &t_array);

  UTILITIES_API std::string compress(const std::string &t_str, const int t_level);

  UTILITIES_API std::string uncompress(const std::string &t_str);

  UTILITIES_API std::string base64decode(const std::string &t_str);

  UTILITIES_API std::string base64encode(const std::string &t_str);

  UTILITIES_API std::string compressToBase64(const std::string &t_str, const int t_level);

  UTILITIES_API std::string uncompressFromBase64(const std::string &t_str);


} // openstudio


#endif
