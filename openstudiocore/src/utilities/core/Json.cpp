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

#include <utilities/core/Json.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/String.hpp>

#include <OpenStudio.hxx>

#include <QFile>

namespace openstudio {

void configureJsonSerializer(QJson::Serializer& serializer) {

  // pretty print json so it is easier to debug
  serializer.setIndentMode(QJson::IndentFull);

}

QVariant jsonMetadata() {
  QVariantMap metadata;
  metadata["version"] = toQString(openStudioVersion());
  return metadata;
}

bool saveJSON(const QVariant& json, openstudio::path p, bool overwrite) {
  // Ensures file extension is .json. Warns if there is a mismatch.
  p = setFileExtension(p,"json",true);

  // Use QFile and QIODevice serialize
  QFile file(toQString(p));
  if (file.open(QFile::WriteOnly)){
    QJson::Serializer serializer;
    configureJsonSerializer(serializer);

    bool ok(false);
    serializer.serialize(json,&file,&ok);
    file.close();

    if (ok) {
      return true;
    }
    LOG_FREE_AND_THROW("openstudio.Json","Could not serialize to JSON format, because "
                       << toString(serializer.errorMessage()));
  }

  LOG_FREE(Error,"openstudio.Json","Could not open file " << toString(p) << " for writing.");
  return false;
}

std::string toJSON(const QVariant& json) {
  QJson::Serializer serializer;
  configureJsonSerializer(serializer);

  bool ok(false);
  QByteArray qba = serializer.serialize(json,&ok);

  if (ok) {
    return toString(QString(qba));
  }

  LOG_FREE_AND_THROW("openstudio.Json","Could not serialize to JSON format, because "
                     << toString(serializer.errorMessage()));
  return std::string();
}

} // openstudio
