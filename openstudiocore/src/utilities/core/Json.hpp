/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <jsoncpp/json.h>

#include <QVariant>

#include <string>
#include <vector>

namespace Json{
  class Value;
}

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

// DLM: need to start getting rid of all this Qt Json stuff

/** Helper function to construct meta-data object containing openstudio_version for JSON files. */
UTILITIES_API QVariant jsonMetadata();

/** Helper function to save top-level json files. */
UTILITIES_API bool saveJSON(const QVariant& json, openstudio::path p, bool overwrite);

/** Helper function to print top-level json files to string. */
UTILITIES_API std::string toJSON(const QVariant& json);

/** Helper function to load top-level json files. */
UTILITIES_API QVariant loadJSON(const openstudio::path& p);

/** Helper function to load top-level json data. Retrieved data is in .first,
 *  OpenStudio version of serialization is in .second. */
UTILITIES_API QVariant loadJSON(const std::string& json);

/** Returns the openstudio_version stored in the top-level JSON variant. */
UTILITIES_API VersionString extractOpenStudioVersion(const QVariant& variant);

template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& valueKey,
                                        const std::string& indexKey,
                                        std::function<T (QVariant*)> typeConverter)
{
  unsigned n = list.size();
  std::vector<T> result(n,T());
  for (const QVariant& listItem : list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    T value = typeConverter(&listItemMap[toQString(valueKey)]);
    result[index] = value;
  }
  return result;
}

template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& indexKey,
                                        std::function<T (QVariant*)> typeConverter)
{
  unsigned n = list.size();
  std::vector<T> result(n,T());
  for (const QVariant& listItem : list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    QVariant listItemVariant(listItemMap);
    T value = typeConverter(&listItemVariant);
    result[index] = value;
  }
  return result;
}

/** Deserializes vectors where a QVariantList holds maps containing an index entry and a
 *  value entry. */
template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& valueKey,
                                        const std::string& indexKey,
                                        std::function<T (const QVariant&)> typeConverter)
{
  std::vector<std::pair<int,T> > data;
  for (const QVariant& listItem : list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    T value = typeConverter(listItemMap[toQString(valueKey)]);
    data.push_back(std::make_pair(index,value));
  }

  std::sort(data.begin(),data.end(),FirstOfPairLess<std::pair<int,T> >());
  std::vector<T> result;
  std::transform(data.begin(), data.end(), std::back_inserter(result), GetSecondOfPair<int,T>());

  return result;
}

/** Deserializes vectors where a QVariantList holds serialized objects to which an
 *  index entry has been added. */
template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& indexKey,
                                        std::function<T (const QVariant&)> typeConverter)
{
  std::vector<std::pair<int,T> > data;
  for (const QVariant& listItem : list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    T value = typeConverter(listItemMap);
    data.push_back(std::make_pair(index,value));
  }

  std::sort(data.begin(),data.end(),FirstOfPairLess<std::pair<int,T> >());
  std::vector<T> result;
  std::transform(data.begin(), data.end(), std::back_inserter(result), GetSecondOfPair<int,T>());

  return result;
}

template<typename T>
std::vector<T> deserializeUnorderedVector(const QVariantList& list,
                                          std::function<T (QVariant*)> typeConverter)
{
  std::vector<T> result;
  for (const QVariant& listItem : list) {
    QVariant listItemCopy(listItem);
    T value = typeConverter(&listItemCopy);
    result.push_back(value);
  }
  return result;
}

template<typename T>
std::vector<T> deserializeUnorderedVector(const QVariantList& list,
                                          std::function<T (const QVariant&)> typeConverter)
{
  std::vector<T> result;
  for (const QVariant& listItem : list) {
    T value = typeConverter(listItem);
    result.push_back(value);
  }
  return result;
}

}

#endif // UTILITIES_CORE_JSON_HPP
