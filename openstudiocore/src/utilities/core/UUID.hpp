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

#ifndef UTILITIES_CORE_UUID_HPP
#define UTILITIES_CORE_UUID_HPP

#include "../UtilitiesAPI.hpp"

#include <QMetaType>
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <ostream>
#include <string>

namespace openstudio {
       class UTILITIES_API UUID;

       /// create a UUID
       UTILITIES_API UUID createUUID();

       /// create a UUID from a std::string, does not throw, may return a null UUID
       UTILITIES_API UUID toUUID(const std::string& str);

       /// create a UUID from a std::string, does not throw, may return a null UUID
       UTILITIES_API UUID toUUID(const QString& str);

       /// create a std::string from a UUID
       UTILITIES_API std::string toString(const UUID& uuid);

       /// create a QString from a UUID
       UTILITIES_API QString toQString(const UUID& uuid);

       /// create a unique name, prefix << " " << UUID.
       UTILITIES_API std::string createUniqueName(const std::string& prefix);

       /// create a std::string without curly brackets from a UUID
       UTILITIES_API std::string removeBraces(const UUID& uuid);

       UTILITIES_API std::ostream& operator<<(std::ostream& os, const UUID& uuid);

       UTILITIES_API bool operator!= (const UUID & lhs, const UUID & rhs);
       UTILITIES_API bool operator< (const UUID & lhs, const UUID & rhs);
       UTILITIES_API bool operator== (const UUID & lhs, const UUID & rhs);
       UTILITIES_API bool operator> (const UUID & lhs, const UUID & rhs);

  /// Universally Unique Identifier
  class UTILITIES_API UUID : private boost::uuids::uuid
  {
    public:
      UUID();
      using boost::uuids::uuid::is_nil;

      bool isNull() const {
        return is_nil();
      }

      using boost::uuids::uuid::iterator;
      using boost::uuids::uuid::const_iterator;
      using boost::uuids::uuid::begin;
      using boost::uuids::uuid::end;

    private:
      explicit UUID(const boost::uuids::uuid &);

      UTILITIES_API friend UUID openstudio::createUUID();
      UTILITIES_API friend UUID openstudio::toUUID(const std::string& str);
      UTILITIES_API friend UUID openstudio::toUUID(const QString& str);
      UTILITIES_API friend std::string openstudio::toString(const UUID& uuid);
      UTILITIES_API friend QString openstudio::toQString(const UUID& uuid);
      UTILITIES_API friend std::string openstudio::createUniqueName(const std::string& prefix);
      UTILITIES_API friend std::string openstudio::removeBraces(const UUID& uuid);
      UTILITIES_API friend bool openstudio::operator!= (const UUID & lhs, const UUID & rhs);
      UTILITIES_API friend bool openstudio::operator< (const UUID & lhs, const UUID & rhs);
      UTILITIES_API friend bool openstudio::operator== (const UUID & lhs, const UUID & rhs);
      UTILITIES_API friend bool openstudio::operator> (const UUID & lhs, const UUID & rhs);

      static UUID random_generate();
      static UUID string_generate(const std::string &);

  };

  /// optional UUID
  typedef boost::optional<UUID> OptionalUUID;

  /// vector of UUID
  typedef std::vector<UUID> UUIDVector;


} // openstudio

Q_DECLARE_METATYPE(openstudio::UUID);


#endif // UTILITIES_CORE_UUID_HPP
