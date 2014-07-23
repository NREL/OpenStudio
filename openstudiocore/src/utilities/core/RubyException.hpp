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

#ifndef UTILITIES_CORE_RUBYEXCEPTION_HPP
#define UTILITIES_CORE_RUBYEXCEPTION_HPP

#include <stdexcept>

#include <QRegExp>
#include <QString>

namespace openstudio
{

  class RubyException : public std::runtime_error
  {
    public:

      RubyException(const std::string& msg, const std::string& location)
        : std::runtime_error(msg), m_location(location)
      {}

      virtual ~RubyException() throw() {}

      std::string location() const {return m_location;}

      std::string shortLocation() const 
      {
        std::string result;

        QRegExp regex("(\\w*\\.rb:\\d*)");
        int pos = regex.indexIn(QString::fromStdString(m_location));
        if (pos > -1) {
          result = regex.cap(1).toStdString();
        }
        
        return result;
      }

    private:

      std::string m_location;

  };

} // openstudio

#endif // UTILITIES_CORE_RUBYEXCEPTION_HPP
