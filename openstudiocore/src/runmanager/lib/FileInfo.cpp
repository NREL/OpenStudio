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

#include "FileInfo.hpp"

namespace openstudio {
namespace runmanager {

FileInfo::FileInfo(const std::string &t_filename, 
                   const openstudio::DateTime &t_lastModified,
                   const std::string &t_key,
                   const openstudio::path &t_fullPath, 
                   bool t_exists)
  : fullPath(t_fullPath), filename(t_filename), 
    exists(t_exists), lastModified(t_lastModified), key(t_key)
{
  if (fullPath.empty())
  {
    fullPath = openstudio::toPath(t_filename);
  }
}

FileInfo::FileInfo(const openstudio::path &t_path, const std::string &t_key)
  : fullPath(t_path),
    filename(toString(fullPath.filename())),
    exists(boost::filesystem::exists(fullPath)),
    key(t_key)
{
  if (exists)
  {
    time_t tt = boost::filesystem::last_write_time(fullPath);
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE        
    tm t;
    gmtime_r(&tt, &t);
    lastModified = openstudio::DateTime(t);
#else
    tm *t = 0;
    t = gmtime(&tt);
    lastModified = openstudio::DateTime(*t);
#endif

  } else {
    lastModified = openstudio::DateTime();
  }
}

FileInfo::FileInfo() {}

FileInfo FileInfo::complete(const openstudio::path &t_basePath) const
{
  if (t_basePath.empty() || fullPath.empty())
  {
    return *this;
  } else {
    FileInfo fi = *this;
    fi.fullPath = boost::filesystem::complete(fullPath, t_basePath);
    return fi;
  }
}

bool FileInfo::operator<(const FileInfo &fi) const
{
  return (fullPath < fi.fullPath)
         || (fullPath == fi.fullPath && lastModified < fi.lastModified);
}

bool FileInfo::operator==(const FileInfo &t_rhs) const
{
  return filename == t_rhs.filename
         && key == t_rhs.key
         && requiredFiles == t_rhs.requiredFiles
         && fullPath == t_rhs.fullPath;
}

bool FileInfo::operator!=(const FileInfo &t_rhs) const
{
  return !(*this == t_rhs);
}

void FileInfo::addRequiredFile(const QUrl &t_url, const openstudio::path &t_target) {
  for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
       itr != requiredFiles.end();
       ++itr)
  {
    if (itr->first == t_url && itr->second == t_target) {
      LOG(Info,"Skipping duplicate request for required file at " 
          << openstudio::toString(itr->first.toString()));
      return;
    }

    /// we only actually care if the destinations are the same, not the sources,
    /// let the user copy the same file to multiple places all day long if required.
    if (itr->second == t_target)
    {
      throw std::runtime_error("required file conflict from: " + openstudio::toString(itr->first.toString()) + " to: " + openstudio::toString(itr->second));
    }
  }

  requiredFiles.push_back(std::make_pair(t_url, t_target));
}

void FileInfo::prependRequiredFilePath(const openstudio::path &t_path)
{
  for (auto & requiredFile : requiredFiles)
  {
    requiredFile.second = t_path / requiredFile.second;
  }
}

void FileInfo::addRequiredFile(const QUrl &t_url)
{
  addRequiredFile(t_url, toPath(t_url.toLocalFile()).filename());
}
  
void FileInfo::addRequiredFile(const openstudio::path &t_location, const openstudio::path &t_target)
{
  addRequiredFile(QUrl::fromLocalFile(toQString(t_location.native())), t_target);
}

void FileInfo::addRequiredFile(const openstudio::path &t_location)
{
  addRequiredFile(QUrl::fromLocalFile(toQString(t_location.native())));
}

bool FileInfo::hasRequiredFile(const openstudio::path &filename)
{
  for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
       itr != requiredFiles.end();
       ++itr)
  {
    if (itr->second.filename() == filename)
    {
      return true;
    }
  }

  return false;
}

std::pair<QUrl, openstudio::path> FileInfo::getRequiredFile(const openstudio::path &filename)
{
  for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
       itr != requiredFiles.end();
       ++itr)
  {
    if (itr->second.filename() == filename)
    {
      return *itr;
    }
  }

  throw std::runtime_error("No such file in list of required files: " + toString(filename));
}

bool Files::operator==(const Files &t_rhs) const
{
  return m_files == t_rhs.m_files;
}

const FileInfo &Files::getLastByKey(const std::string &t_key) const
{
  // we're going to try and work around what seems to be a bug in the stdlib used
  // by apple. We're going to save the error, then
  try {
    return getLast(std::bind(&Files::keyCompare, t_key, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByKey: " + std::string(e.what()) + ": " + t_key);
  }
}

const FileInfo &Files::getLastByRegex(const std::string &t_regex) const
{
  try {
    return getLast(std::bind(&Files::regexCompare, QRegExp(toQString(t_regex), Qt::CaseInsensitive), std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByRegex: " + std::string(e.what()) + ": " + t_regex);
  }
}

Files Files::getAllByRegex(const std::string &t_regex) const
{
  try {
    return getAll(std::bind(&Files::regexCompare, QRegExp(toQString(t_regex), Qt::CaseInsensitive), std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByRegex: " + std::string(e.what()) + ": " + t_regex);
  }
}

Files Files::getAllByKey(const std::string &t_key) const
{
  try {
    return getAll(std::bind(&Files::keyCompare, t_key, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getAllByKey: " + std::string(e.what()) + ": " + t_key);
  }
}

const FileInfo &Files::getLastByFilename(const std::string &t_filename) const
{
  try {
    return getLast(std::bind(&Files::filenameCompare, t_filename, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByFilename: " + std::string(e.what()) + ": " + t_filename);
  }
}

Files Files::getAllByFilename(const std::string &t_filename) const
{
  try {
    return getAll(std::bind(&Files::filenameCompare, t_filename, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getAllByFilename: " + std::string(e.what()) + ": " + t_filename);
  }
}

FileInfo &Files::getLastByRegex(const std::string &t_regex)
{
  try {
    return getLast(std::bind(&Files::regexCompare, QRegExp(toQString(t_regex), Qt::CaseInsensitive), std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByRegex: " + std::string(e.what()) + ": " + t_regex);
  }
}

const FileInfo &Files::getLastByExtension(const std::string &t_ext) const
{
  try {
    return getLast(std::bind(&Files::extensionCompare, t_ext, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByExtension: " + std::string(e.what()) + ": " + t_ext);
  }
}

Files Files::getAllByExtension(const std::string &t_ext) const
{
  try {
    return getAll(std::bind(&Files::extensionCompare, t_ext, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getAllByExtension: " + std::string(e.what()) + ": " + t_ext);
  }
}

FileInfo &Files::getLastByKey(const std::string &t_key) 
{
  try {
    return getLast(std::bind(&Files::keyCompare, t_key, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByKey: " + std::string(e.what()) + ": " + t_key);
  }
}

FileInfo &Files::getLastByFilename(const std::string &t_filename)
{
  try {
    return getLast(std::bind(&Files::filenameCompare, t_filename, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByFilename: " + std::string(e.what()) + ": " + t_filename);
  }
}

FileInfo &Files::getLastByExtension(const std::string &t_ext) 
{
  try {
    return getLast(std::bind(&Files::extensionCompare, t_ext, std::placeholders::_1));
  } catch (const std::runtime_error &e) {
    throw std::runtime_error("getLastByExtension: " + std::string(e.what()) + ": " + t_ext);
  }
}

}
}
