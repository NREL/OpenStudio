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


#include "Encoding.hpp"


namespace openstudio {


  QByteArray toQByteArray(const std::string &t_str) {
    return QByteArray::fromRawData(t_str.data(), t_str.size());
  }

  std::string toStdString(const QByteArray &t_array) {
    return std::string(t_array.data(), t_array.size());
  }

  std::string compress(const std::string &t_str, const int t_level) {
    return toStdString(qCompress(toQByteArray(t_str), t_level));
  }

  std::string uncompress(const std::string &t_str) {
    return toStdString(qUncompress(toQByteArray(t_str)));
  }

  std::string base64decode(const std::string &t_str) {
    return toStdString(QByteArray::fromBase64(toQByteArray(t_str)));
  }

  std::string base64encode(const std::string &t_str) {
    return toStdString(toQByteArray(t_str).toBase64());
  }

  std::string compressToBase64(const std::string &t_str, const int t_level)
  {
    return toStdString(qCompress(toQByteArray(t_str), t_level).toBase64());
  }

  std::string uncompressFromBase64(const std::string &t_str)
  {
    return toStdString(qUncompress(QByteArray::fromBase64(toQByteArray(t_str))));
  }


} // openstudio



