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

#ifndef UTILITIES_CORE_EXCEPTION_HPP
#define UTILITIES_CORE_EXCEPTION_HPP

#include <string>
#include <exception>
#include "../UtilitiesAPI.hpp"

namespace openstudio{

  /** Base class for exceptions. */
  class UTILITIES_API Exception: public std::exception { 
  public:

    /// Constructor takes a message
    Exception(const std::string& msg)
      : m_msg(msg)
    {}

    /// Needed for rb_raise support in swig
    Exception(const char *msg)
      : m_msg(msg)
    {
    }

    /// virtual destructor, no throw
    virtual ~Exception() throw() {}

    /// return the message
    virtual const std::string& message() const {return m_msg;}

    /// return the message
    virtual const char* what() const throw() {return m_msg.c_str();}
  
  private:

    std::string m_msg;

  };

} // openstudio

#endif // UTILITIES_CORE_EXCEPTION_HPP
