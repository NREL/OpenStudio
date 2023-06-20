/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_JSON_HPP
#define UTILITIES_CORE_JSON_HPP

#include "../UtilitiesAPI.hpp"

#include "Path.hpp"
#include "Compare.hpp"

#include <json/json.h>

#include <string>
#include <vector>

namespace openstudio {

/// assert key is present, throws if key is not found
UTILITIES_API void assertKey(const Json::Value& value, const std::string& key);

/// assert type is correct if key is present, throws if type is not correct
UTILITIES_API void assertType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);

/// assert key is present and type is correct, throws if key not found or type is not correct
UTILITIES_API void assertKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);

/// check key is present, return false if key is not found
UTILITIES_API bool checkKey(const Json::Value& value, const std::string& key);

/// check type is correct if key is present, return false if type is not correct
UTILITIES_API bool checkType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);

/// check key is present and type is correct, return false if key not found or type is not correct
UTILITIES_API bool checkKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);

}  // namespace openstudio

#endif  // UTILITIES_CORE_JSON_HPP
