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

#ifndef UTILITIES_CORE_JSON_HPP
#define UTILITIES_CORE_JSON_HPP

#include "../UtilitiesAPI.hpp"

#include "Path.hpp"
#include "Compare.hpp"

#include <QVariant>

#include <string>
#include <vector>

namespace openstudio {

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
