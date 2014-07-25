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

#ifndef UTILITIES_DATA_TAG_HPP
#define UTILITIES_DATA_TAG_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/UUID.hpp"

#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace openstudio {

class VersionString;

class UTILITIES_API Tag {
 public:
  explicit Tag(const std::string& name);

  /** Deserialization constructor, not for general use. */
  Tag(const UUID& uuid,const std::string& name);

  Tag clone() const;

  UUID uuid() const;

  std::string name() const;

 private:
  UUID m_uuid;
  std::string m_name;
};

/** \relates Tag */
typedef boost::optional<Tag> OptionalTag;

/** \relates Tag */
typedef std::vector<Tag> TagVector;

namespace detail {

  /** Places tag's data in a QVariant for JSON serialization. */
  UTILITIES_API QVariant toVariant(const Tag& tag);

  /** Deserializes json variant to Tag. */
  UTILITIES_API Tag toTag(const QVariant& variant, const VersionString& version);

}

} // openstudio

#endif // UTILITIES_DATA_TAG_HPP
