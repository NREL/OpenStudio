/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_UUID_HPP
#define UTILITIES_CORE_UUID_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/regex.hpp>

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

/// create a std::string from a UUID
UTILITIES_API std::string toString(const UUID& uuid);

/// create a unique name, prefix << " " << UUID.
UTILITIES_API std::string createUniqueName(const std::string& prefix);

/// create a std::string without curly brackets from a UUID
UTILITIES_API std::string removeBraces(const UUID& uuid);

UTILITIES_API std::ostream& operator<<(std::ostream& os, const UUID& uuid);

UTILITIES_API bool operator!=(const UUID& lhs, const UUID& rhs);
UTILITIES_API bool operator<(const UUID& lhs, const UUID& rhs);
UTILITIES_API bool operator==(const UUID& lhs, const UUID& rhs);
UTILITIES_API bool operator>(const UUID& lhs, const UUID& rhs);

/// Universally Unique Identifier
class UTILITIES_API UUID : public boost::uuids::uuid
{
 public:
  UUID();

  bool isNull() const {
    return is_nil();
  }

  bool isEqual(const UUID& other) const {
    return (*this == other);
  }

  using boost::uuids::uuid::iterator;
  using boost::uuids::uuid::const_iterator;
  using boost::uuids::uuid::begin;
  using boost::uuids::uuid::end;

 private:
  explicit UUID(const boost::uuids::uuid&);

  UTILITIES_API friend UUID openstudio::createUUID();
  UTILITIES_API friend UUID openstudio::toUUID(const std::string& str);
  UTILITIES_API friend std::string openstudio::toString(const UUID& uuid);
  UTILITIES_API friend std::string openstudio::createUniqueName(const std::string& prefix);
  UTILITIES_API friend std::string openstudio::removeBraces(const UUID& uuid);
  UTILITIES_API friend bool openstudio::operator!=(const UUID& lhs, const UUID& rhs);
  UTILITIES_API friend bool openstudio::operator<(const UUID& lhs, const UUID& rhs);
  UTILITIES_API friend bool openstudio::operator==(const UUID& lhs, const UUID& rhs);
  UTILITIES_API friend bool openstudio::operator>(const UUID& lhs, const UUID& rhs);

  static UUID random_generate();
  static UUID string_generate(const std::string&);
};

/// Find version 4 UUIDs in a string.
UTILITIES_API const boost::regex& uuidInString();

/// create a unique name, prefix << " " << UUID.
UTILITIES_API std::string createUniqueName(const std::string& prefix);

/// optional UUID
using OptionalUUID = boost::optional<UUID>;

/// vector of UUID
using UUIDVector = std::vector<UUID>;

}  // namespace openstudio

#endif  // UTILITIES_CORE_UUID_HPP
