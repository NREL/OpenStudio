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

#include <utilities/data/Tag.hpp>

namespace openstudio {

Tag::Tag(const std::string& name)
  : m_uuid(createUUID()), m_name(name)
{}

Tag::Tag(const UUID& uuid,const std::string& name)
  : m_uuid(uuid), m_name(name)
{}

Tag Tag::clone() const {
  Tag result(*this);
  result.m_uuid = createUUID();
  return result;
}

UUID Tag::uuid() const {
  return m_uuid;
}

std::string Tag::name() const {
  return m_name;
}

} // openstudio
