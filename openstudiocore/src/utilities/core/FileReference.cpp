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

#include <utilities/core/FileReference.hpp>

#include <utilities/time/DateTime.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/PathHelpers.hpp>

namespace openstudio {

FileReference::FileReference(const openstudio::path& p)
  : m_uuid(createUUID()), m_versionUUID(createUUID()),
    m_name(toString(p)),
    m_displayName(toString(p.stem())),
    m_path(completeAndNormalize(p)),
    m_timestampCreate(DateTime::now()), m_timestampLast(m_timestampCreate),
    m_checksumCreate(checksum(m_path)), m_checksumLast(m_checksumCreate)
{
  try {
    m_fileType = FileReferenceType(getFileExtension(p));
  }
  catch (...) {
    m_fileType = FileReferenceType::Unknown;
  }
}

/** De-serialization constructor. Not for general use. */
FileReference::FileReference(const openstudio::UUID& uuid,
                             const openstudio::UUID& versionUUID,
                             const std::string& name,
                             const std::string& displayName,
                             const std::string& description,
                             const openstudio::path& p,
                             const FileReferenceType& fileType,
                             const DateTime& timestampCreate,
                             const DateTime& timestampLast,
                             const std::string& checksumCreate,
                             const std::string& checksumLast)
  : m_uuid(uuid), m_versionUUID(versionUUID),
    m_name(name), m_displayName(displayName), m_description(description),
    m_path(p), m_fileType(fileType),
    m_timestampCreate(timestampCreate), m_timestampLast(timestampLast), 
    m_checksumCreate(checksumCreate), m_checksumLast(checksumLast)
{}

FileReference FileReference::clone() const {
  FileReference result(*this);
  result.m_uuid = createUUID();
  result.m_versionUUID = createUUID();
  return result;
}

openstudio::UUID FileReference::uuid() const {
  return m_uuid;
}

openstudio::UUID FileReference::versionUUID() const {
  return m_versionUUID;
}

std::string FileReference::name() const {
  return m_name;
}

std::string FileReference::displayName() const {
  return m_displayName;
}

std::string FileReference::description() const {
  return m_description;
}

openstudio::path FileReference::path() const { return m_path; }

FileReferenceType FileReference::fileType() const { return m_fileType; }

DateTime FileReference::timestampCreate() const { return m_timestampCreate; }

DateTime FileReference::timestampLast() const { return m_timestampLast; }

std::string FileReference::checksumCreate() const { return m_checksumCreate; }

std::string FileReference::checksumLast() const { return m_checksumLast; }

void FileReference::setName(const std::string& newName) {
  m_name = newName;
}

void FileReference::setDisplayName(const std::string& newDisplayName) {
  m_displayName = newDisplayName;
}

void FileReference::setDescription(const std::string& newDescription) {
  m_description = newDescription;
}

bool FileReference::makePathAbsolute(const openstudio::path& searchDirectory) {
  // trivial completion
  openstudio::path currentPath = path();
  if (currentPath.is_complete() && boost::filesystem::exists(currentPath)) {
    return true;
  }
  openstudio::path workingPath(currentPath);
  // if currentPath is complete but does not exist, go to extreme measures
  if (currentPath.is_complete()) {
    workingPath = toPath(currentPath.filename());
  }
  openstudio::path newPath = boost::filesystem::complete(workingPath,searchDirectory);
  if (newPath.empty() || !boost::filesystem::exists(newPath)) { return false; }
  m_path = completeAndNormalize(newPath);
  return true;
}

bool FileReference::makePathRelative(const openstudio::path& basePath) {
  openstudio::path newPath;
  if (basePath.empty()) { 
    newPath = toPath(path().filename());
  }
  else {
    newPath = relativePath(path(),basePath);
  }
  if (newPath.empty()) { return false; }
  m_path = newPath;
  return true;
}

bool FileReference::update(const openstudio::path& searchDirectory) {
  openstudio::path originalPath = path();
  bool ok = makePathAbsolute(searchDirectory);
  if (ok) {
    m_timestampLast = DateTime::now();
    m_checksumLast = checksum(path());
  }
  else {
    m_path = originalPath;
  }
  return ok;
}

namespace detail {

  QVariant toVariant(const FileReference& fileReference) {
    QVariantMap fileReferenceData;

    fileReferenceData["uuid"] = toQString(toUID(fileReference.uuid()));
    fileReferenceData["version_uuid"] = toQString(toUID(fileReference.versionUUID()));
    std::string str = fileReference.name();
    if (!str.empty()) {
      fileReferenceData["name"] = toQString(str);
    }
    str = fileReference.displayName();
    if (!str.empty()) {
      fileReferenceData["display_name"] = toQString(str);
    }
    str = fileReference.description();
    if (!str.empty()) {
      fileReferenceData["description"] = toQString(str);
    }
    fileReferenceData["path"] = toQString(fileReference.path());
    fileReferenceData["file_type"] = toQString(fileReference.fileType().valueName());
    fileReferenceData["timestamp_create"] = int(fileReference.timestampCreate().toEpoch());
    fileReferenceData["timestamp_last"] = int(fileReference.timestampLast().toEpoch());
    fileReferenceData["checksum_create"] = toQString(fileReference.checksumCreate());
    fileReferenceData["checksum_last"] = toQString(fileReference.checksumLast());

    return QVariant(fileReferenceData);
  }

  FileReference toFileReference(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();
    OptionalDateTime timestampCreate, timestampLast;
    if (version < VersionString("1.0.4")) {
      timestampCreate = DateTime(map["timestamp_create"].toString().toStdString());
      timestampLast = DateTime(map["timestamp_last"].toString().toStdString());
    }
    else {
      timestampCreate = DateTime::fromEpoch(std::time_t(map["timestamp_create"].toInt()));
      timestampLast = DateTime::fromEpoch(std::time_t(map["timestamp_last"].toInt()));
    }
    OS_ASSERT(timestampCreate);
    OS_ASSERT(timestampLast);
    return FileReference(toUUID(map["uuid"].toString().toStdString()),
                         toUUID(map["version_uuid"].toString().toStdString()),
                         map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                         map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                         map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                         toPath(map["path"].toString()),
                         FileReferenceType(map["file_type"].toString().toStdString()),
                         timestampCreate.get(),
                         timestampLast.get(),
                         map["checksum_create"].toString().toStdString(),
                         map["checksum_last"].toString().toStdString());
  }

} // detail

} // openstudio

