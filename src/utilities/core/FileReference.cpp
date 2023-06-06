/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FileReference.hpp"

#include "Assert.hpp"
#include "Checksum.hpp"
#include "PathHelpers.hpp"
#include "../core/FilesystemHelpers.hpp"

namespace openstudio {

FileReference::FileReference(const openstudio::path& p)
  : m_uuid(createUUID()),
    m_versionUUID(createUUID()),
    m_name(toString(p)),
    m_displayName(toString(p.filename())),
    m_path(completeAndNormalize(p)),
    m_timestampLast(),
    m_checksumCreate(checksum(m_path)),
    m_checksumLast(m_checksumCreate) {
  try {
    m_fileType = FileReferenceType(getFileExtension(p));
  } catch (...) {
    m_fileType = FileReferenceType::Unknown;
  }
  update(openstudio::path());
}

/** De-serialization constructor. Not for general use. */
FileReference::FileReference(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                             const std::string& displayName, const std::string& description, const openstudio::path& p,
                             const FileReferenceType& fileType, const DateTime& timestampLast, const std::string& checksumCreate,
                             const std::string& checksumLast)
  : m_uuid(uuid),
    m_versionUUID(versionUUID),
    m_name(name),
    m_displayName(displayName),
    m_description(description),
    m_path(p),
    m_fileType(fileType),
    m_timestampLast(timestampLast),
    m_checksumCreate(checksumCreate),
    m_checksumLast(checksumLast) {}

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

openstudio::path FileReference::path() const {
  return m_path;
}

FileReferenceType FileReference::fileType() const {
  return m_fileType;
}

DateTime FileReference::timestampLast() const {
  return m_timestampLast;
}

std::string FileReference::checksumCreate() const {
  return m_checksumCreate;
}

std::string FileReference::checksumLast() const {
  return m_checksumLast;
}

void FileReference::setName(const std::string& newName) {
  m_name = newName;
  m_versionUUID = createUUID();
}

void FileReference::setDisplayName(const std::string& newDisplayName) {
  m_displayName = newDisplayName;
  m_versionUUID = createUUID();
}

void FileReference::setDescription(const std::string& newDescription) {
  m_description = newDescription;
  m_versionUUID = createUUID();
}

void FileReference::setPath(const openstudio::path& newPath) {
  if (newPath != m_path) {
    m_path = newPath;
    m_versionUUID = createUUID();
  }
}

bool FileReference::makePathAbsolute(const openstudio::path& searchDirectory) {
  // trivial completion
  openstudio::path currentPath = path();
  if (currentPath.is_complete() && openstudio::filesystem::exists(currentPath)) {
    return true;
  }
  openstudio::path workingPath(currentPath);
  // if currentPath is complete but does not exist, go to extreme measures
  if (currentPath.is_complete()) {
    workingPath = currentPath.filename();
  }
  if (searchDirectory.empty()) {
    return false;
  }
  openstudio::path newPath = openstudio::filesystem::complete(workingPath, searchDirectory);
  if (newPath.empty() || !openstudio::filesystem::exists(newPath)) {
    return false;
  }
  m_path = completeAndNormalize(newPath);
  m_versionUUID = createUUID();
  return true;
}

bool FileReference::makePathRelative(const openstudio::path& basePath) {
  openstudio::path newPath;
  if (basePath.empty()) {
    newPath = path().filename();
  } else {
    newPath = relativePath(path(), basePath);
  }
  if (newPath.empty()) {
    return false;
  }
  m_path = newPath;
  m_versionUUID = createUUID();
  return true;
}

bool FileReference::update(const openstudio::path& searchDirectory) {
  makePathAbsolute(searchDirectory);
  openstudio::path p = path();
  if (openstudio::filesystem::exists(p)) {
    m_timestampLast = DateTime::fromEpoch(openstudio::filesystem::last_write_time_as_time_t(p));
    m_checksumLast = checksum(p);
    m_versionUUID = createUUID();
    return true;
  }
  return false;
}

}  // namespace openstudio
