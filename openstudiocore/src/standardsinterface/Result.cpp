/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <standardsinterface/Result.hpp>

using namespace openstudio::standardsinterface::result;

namespace openstudio {
namespace standardsinterface {

namespace detail {
  Result_Base::Result_Base(Code code, const std::string& msg) 
    : m_code(code), m_message(msg) {}

  bool Result_Base::good() const { return (m_code == Good); }

  Code Result_Base::code() const { return m_code; }

  std::string Result_Base::what() const { return m_message; }
} // detail

} // standardsinterface
} // openstudio
