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

#ifndef UTILITIES_CORE_JSON_HPP
#define UTILITIES_CORE_JSON_HPP

#include <utilities/core/Path.hpp>

#include <boost/function.hpp>

#include <QVariant>

#include <string>
#include <vector>

namespace QJson {
  class Serializer;
}

namespace openstudio {

class VersionString;

namespace detail {

  /** Helper function to standardize JSON serialization settings. */
  void configureJsonSerializer(QJson::Serializer& serializer);

}

/** Helper function to construct version meta-data object for JSON files. */
QVariant jsonMetadata();

/** Helper function to save top-level json files. */
bool saveJSON(const QVariant& json, openstudio::path p, bool overwrite);

/** Helper function to print top-level json files to string. */
std::string toJSON(const QVariant& json);

/** Helper function to load top-level json files. Retrieved data is in .first,
 *  OpenStudio version of file is in .second. */
std::pair<QVariant,VersionString> loadJSON(const openstudio::path& p);

/** Helper function to load top-level json data. Retrieved data is in .first,
 *  OpenStudio version of serialization is in .second. */
std::pair<QVariant,VersionString> loadJSON(const std::string& json);

template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& valueKey,
                                        const std::string& indexKey,
                                        boost::function<T (QVariant*)> typeConverter)
{
  unsigned n = list.size();
  std::vector<T> result(n,T());
  Q_FOREACH(const QVariant& listItem,list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    T value = typeConverter(&listItemMap[toQString(valueKey)]);
    result[index] = value;
  }
  return result;
}

template<typename T>
std::vector<T> deserializeOrderedVector(const QVariantList& list,
                                        const std::string& valueKey,
                                        const std::string& indexKey,
                                        boost::function<T (const QVariant&)> typeConverter)
{
  unsigned n = list.size();
  std::vector<T> result(n,T());
  Q_FOREACH(const QVariant& listItem,list) {
    QVariantMap listItemMap = listItem.toMap();
    int index = listItemMap[toQString(indexKey)].toInt();
    T value = typeConverter(listItemMap[toQString(valueKey)]);
    result[index] = value;
  }
  return result;
}

template<typename T>
std::vector<T> deserializeUnorderedVector(const QVariantList& list,
                                          boost::function<T (QVariant*)> typeConverter)
{
  std::vector<T> result;
  Q_FOREACH(const QVariant& listItem,list) {
    T value = typeConverter(&listItem);
    result.push_back(value);
  }
  return result;
}

template<typename T>
std::vector<T> deserializeUnorderedVector(const QVariantList& list,
                                          boost::function<T (const QVariant&)> typeConverter)
{
  std::vector<T> result;
  Q_FOREACH(const QVariant& listItem,list) {
    T value = typeConverter(listItem);
    result.push_back(value);
  }
  return result;
}

}

#endif // UTILITIES_CORE_JSON_HPP
