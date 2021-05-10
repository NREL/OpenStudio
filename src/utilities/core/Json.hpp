/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
