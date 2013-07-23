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

#include <qjson/serializer.h>

#include <QVariant>

#include <string>

namespace openstudio {

/** Helper function to standardize JSON serialization settings. */
void configureJsonSerializer(QJson::Serializer& serializer);

/** Helper function to construct version meta-data object for JSON files. */
QVariant jsonMetadata();

/** Helper function to save top-level json files. */
bool saveJSON(const QVariant& json, openstudio::path p, bool overwrite);

/** Helper function to print top-level json files to string. */
std::string toJSON(const QVariant& json);

}

#endif // UTILITIES_CORE_JSON_HPP
