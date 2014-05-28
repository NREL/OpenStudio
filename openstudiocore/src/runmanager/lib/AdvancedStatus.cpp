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

#include "AdvancedStatus.hpp"

namespace openstudio {
namespace runmanager {

  AdvancedStatus::AdvancedStatus()
    : m_enum(AdvancedStatusEnum::Idle)
  {
  }

  AdvancedStatus::AdvancedStatus(const AdvancedStatusEnum &t_enum)
    : m_enum(t_enum)
  {
  }

  AdvancedStatus::AdvancedStatus(const AdvancedStatusEnum &t_enum, const std::string &t_description)
    : m_enum(t_enum), m_description(t_description)
  {
  }

  std::string AdvancedStatus::toString() const
  {
    if (m_description.empty())
    {
      return m_enum.valueDescription();
    } else {
      return m_enum.valueDescription() + ": " + m_description;
    }
  }

  AdvancedStatusEnum AdvancedStatus::value() const
  {
    return m_enum;
  }

  std::string AdvancedStatus::description() const
  {
    return m_description;
  }


  bool AdvancedStatus::operator==(const AdvancedStatus &rhs) const
  {
    return m_enum == rhs.m_enum && m_description == rhs.m_description;
  }

  bool AdvancedStatus::operator!=(const AdvancedStatus &rhs) const
  {
    return m_enum != rhs.m_enum || m_description != rhs.m_description;
  }

  bool AdvancedStatus::operator<(const AdvancedStatus &rhs) const
  {
    return m_enum < rhs.m_enum 
       || ( m_enum == rhs.m_enum && m_description < rhs.m_description);
  }



}
}


