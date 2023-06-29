/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Json.hpp"

#include "Assert.hpp"
#include "PathHelpers.hpp"
#include "Logger.hpp"
#include "FilesystemHelpers.hpp"

#include <OpenStudio.hxx>

namespace openstudio {

// assert key is present
void assertKey(const Json::Value& value, const std::string& key) {
  if (!checkKey(value, key)) {
    throw openstudio::Exception(std::string("Cannot find key '" + key + "'"));
  }
}

// assert type is correct if key is present
void assertType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  if (!checkType(value, key, valueType)) {
    throw openstudio::Exception(std::string("Key '" + key + "' is of wrong type"));
  }
}

// assert key is present and type is correct
void assertKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  assertKey(value, key);
  assertType(value, key, valueType);
}

/// check key is present, return false if key is not found
bool checkKey(const Json::Value& value, const std::string& key) {
  if (!value.isMember(key)) {
    return false;
  }
  return true;
}

/// check type is correct if key is present, return false if type is not correct
bool checkType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  if (value.isMember(key)) {
    if (!value[key].isConvertibleTo(valueType)) {
      return false;
    }
    if (valueType != Json::nullValue) {
      if (value[key].isNull()) {
        return false;
      }
    }
  }
  return true;
}

/// check key is present and type is correct, return false if key not found or type is not correct
bool checkKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  if (value.isMember(key)) {
    if (value[key].isConvertibleTo(valueType)) {
      if (value[key].isNull()) {
        if (valueType == Json::nullValue) {
          return true;
        } else {
          return false;
        }
      }
      // not null and is convertible
      return true;
    } else {
      // not convertible to valueType
      LOG_FREE(Warn, "JSON", "Key '" << key << "' exists but is not the correct type");
    }
  }
  return false;
}

}  // namespace openstudio
