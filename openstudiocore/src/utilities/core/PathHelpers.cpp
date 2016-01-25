/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "PathHelpers.hpp"
#include "Logger.hpp"
#include "Assert.hpp"
#include <boost/filesystem.hpp>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace openstudio {

std::string modelFileExtension() {
  return std::string("osm");
}

std::string componentFileExtension() {
  return std::string("osc");
}

std::string tableFileExtension() {
  return std::string("ost");
}

std::string documentFileExtension() {
  return std::string("osd");
}

std::string rulesetFileExtension() {
  return std::string("osr");
}

path completePathToFile(const path& p,const path& base,const std::string& ext,bool warnOnMismatch)
{
  path result(p);

  // handle file extension
  if (!ext.empty()) {
    result = setFileExtension(p,ext,false,warnOnMismatch);
    if (result.empty()) { result = p; }
  }

  // complete path
  if (!result.is_complete()) {
    try {
      if (!base.empty()) { result = boost::filesystem::complete(result,base); }
      else { result = boost::filesystem::complete(result); }
    }
    catch (...) {
      LOG_FREE(Info,"openstudio.completePathToFile","Unable to compete path '" << toString(p)
               << "'. Returning an empty path.");
      return path();
    }
  }

  // check that result is a file
  if (!boost::filesystem::is_regular_file(result)) {
    LOG_FREE(Info,"openstudio.completePathToFile","Path '" << toString(p)
             << "' could not be resolved to an existing file. Returning an empty path.");
    return path();
  }

  return result;

}

std::string getFileExtension(const path& p) {
  std::string pext = boost::filesystem::extension(p);
  if (!pext.empty()) {
    // remove '.'
    pext = std::string(++pext.begin(),pext.end());
  }
  return pext;
}

path setFileExtension(const path& p,
                      const std::string& ext,
                      bool replaceOnMismatch,
                      bool warnOnMismatch)
{
  path result(p);
  path wext = toPath(ext);
  std::string pext = boost::filesystem::extension(p);
  if (!pext.empty()) {
    // remove '.' from pext
    pext = std::string(++pext.begin(),pext.end());
  }
  if (!pext.empty()) {
    if (pext != wext.string()) {
      if (warnOnMismatch) {
        LOG_FREE(Warn,"openstudio.setFileExtension","Path p, '" << toString(p)
                 << "', has an unexpected file extension. Was expecting '" << toString(wext)
                 << "'.");
      }
      if (!replaceOnMismatch) { return result; }
    }
  } // if

  result.replace_extension(wext.string());
  return result;
}

bool makeParentFolder(const path& p,const path& base,bool recursive) {

  // get path to last directory
  path wp(p);
  if (base.empty()) {
    wp = boost::filesystem::complete(wp);
  }
  else {
    wp = boost::filesystem::complete(wp,base);
  }
  if (wp.has_filename()) {
    wp = wp.parent_path();
  }

  // make one or more directories as needed and as directed by recursive
  bool result = true;
  if (boost::filesystem::is_directory(wp)) {
    return result;
  }
  else if (recursive) {
    try {
      result = create_directories(wp);
    }
    catch (...) {
      result = false;
    }
  }
  else {
    try {
      result = create_directory(wp);
    }
    catch (...) {
      result = false;
    }
  }

  return result;
}

path relativePath(const path& p,const path& base) {
  path wp = p;
  path wBase = base;

  path::const_iterator wpIt = wp.begin();
  path::const_iterator wpEnd = wp.end();
  path::const_iterator wBaseIt = wBase.begin();
  path::const_iterator wBaseEnd = wBase.end();
  while ((wpIt != wpEnd) && (wBaseIt != wBaseEnd) && ((*wpIt) == (*wBaseIt))) {
    ++wpIt;
    ++wBaseIt;
  }
  // p is not an extension of base, try to complete, then return p
  if (!((wBaseIt == wBaseEnd) || (toString(*wBaseIt) == "."))) { 
    path completeP = boost::filesystem::complete(p);
    path completeBase = boost::filesystem::complete(base);
    if ((completeP != wp) || (completeBase != wBase)) { 
      LOG_FREE(Debug,"openstudio.utilities.core","Path '" << toString(p) 
        << "' does not extend base '" << toString(base) 
        << "'. Try again after completing both paths.");
      return relativePath(completeP,completeBase);
    }
    else {
      LOG_FREE(Debug,"openstudio.utilities.core","Path '" << toString(p) 
        << "' does not extend base '" << toString(base) << "'.");
      return path(); 
    }
  }

  // p is an extension of base, keep whatever remains of p
  path result;
  while (wpIt != wpEnd) {
    result = result/(*wpIt);
    ++wpIt;
  }
  return result;
}

path completeAndNormalize(const path& p) {
  path temp = boost::filesystem::system_complete(p);
  if (temp.empty() && !p.empty()) {
    temp = p;
  }
  path result;

  for(openstudio::path::iterator it=temp.begin(); it!=temp.end(); ++it) {
    if (*it == toPath("..")) {
      if (boost::filesystem::is_symlink(result) || (result.filename() == toPath(".."))) {
        result /= *it;
      }
      else {
        result = result.parent_path();
      }
    }
    else if(*it != toPath(".")) {
      result /= *it;
    }
  }

  return result;
}

path relocatePath(const path& originalPath,
                  const path& originalBase,
                  const path& newBase)
{
  path result;
  path temp = relativePath(originalPath,originalBase);
  LOG_FREE(Debug,"openstudio.utilities.core","Original path '" << toString(originalPath) 
      << "', relative to '" << toString(originalBase) << "' is '" << toString(temp) << "'.");
  if (!temp.empty()) {
    result = newBase / temp;
    LOG_FREE(Debug,"openstudio.utilities.core","Relocating path to '" << toString(result) << "'.");
  }
  return result;
}

std::ostream& printPathInformation(std::ostream& os,const path& p) {
  os << "p.string() = " << toString(p.string()) << std::endl;
  os << "p.native() = " << toString(p.native()) << std::endl;

  os << "p.root_name() = " << toString(p.root_name()) << std::endl;
  os << "p.root_directory() = " << toString(p.root_directory()) << std::endl;

  os << "p.parent_path() = " << toString(p.parent_path()) << std::endl;
  os << "p.filename() = " << toString(p.filename()) << std::endl;

  os << "p.stem() = " << toString(p.stem()) << std::endl;
  os << "p.extension() = " << toString(p.extension()) << std::endl;

  os << "p.empty() = " << std::boolalpha << p.empty() << std::endl;
  os << "p.is_complete() = " << p.is_complete() << std::endl;

  os << "boost::filesystem::complete(p) = " << toString(boost::filesystem::complete(p)) << std::endl;
  os << "boost::filesystem::system_complete(p) = " << toString(boost::filesystem::system_complete(p)) << std::endl;

  return os;
}

bool removeDirectory(const path& dirName) {

  bool result = true;
  QDir dir(toQString(dirName));

  if (dir.exists()) 
  {
    for (const QFileInfo& info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
    {
      if (info.isDir()) 
      {
        result = removeDirectory(toPath(info.absoluteFilePath()));
      }
      else 
      {
        result = QFile::remove(info.absoluteFilePath());
      }

      if (!result) 
      {
        return result;
      }
    }
    result = QDir().rmdir(toQString(dirName));
  }

  return result;
}

bool copyDirectory(const path& source, const path& destination) {

  if (!QDir().mkpath(toQString(destination)))
  {
    return false;
  }

  QDir srcDir(toQString(source));

  for (const QFileInfo &info : srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
  {
    QString srcItemPath = toQString(source) + "/" + info.fileName();
    QString dstItemPath = toQString(destination) + "/" + info.fileName();
    if (info.isDir()) 
    {
      if (!copyDirectory(toPath(srcItemPath), toPath(dstItemPath)))
      {
        return false;
      }
    } 
    else if (info.isFile()) 
    {
      if (!QFile::copy(srcItemPath, dstItemPath))
      {
        return false;
      }
    } 
  }

  return true;
}

bool isEmptyDirectory(const path& dirName)
{
  if (!QFile::exists(toQString(dirName))){
    return false;
  }
  if (!QFileInfo(toQString(dirName)).isDir()){
    return false;
  }

  QDir dir(toQString(dirName));
  return dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).empty();
}


boost::optional<std::string> windowsDriveLetter(const path& p)
{
  boost::optional<std::string> result;
  QString q = toQString(p);

  QRegularExpression regex("^([a-zA-Z]):");
  QRegularExpressionMatch match = regex.match(q);
  if (match.hasMatch()){
    result = toString(match.captured(1));
  }

  return result;
}

bool isNetworkPath(const path& p)
{
  if (p.empty() || !p.is_absolute()){
    return false;
  }

#ifdef Q_OS_WIN

  // if this is a windows drive, check if this is mapped to a remote drive
  boost::optional<std::string> wdl = windowsDriveLetter(p);
  if (wdl){
    std::string pstring = wdl.get() + ":\\";
    if (GetDriveType(pstring.c_str()) == DRIVE_REMOTE){
      return true;
    }
  } 

  // check if path begins with \\, e.g. \\server\file
  QString q = toQString(p.string()); // toQString(p) converts backslashes to slashes
  QRegularExpression regex("^\\\\\\\\");
  QRegularExpressionMatch match = regex.match(q);
  if (match.hasMatch()){
    return true;
  }

#endif

  return false;
}

bool isNetworkPathAvailable(const path& p)
{
  if (!isNetworkPath(p)){
    return false;
  }

#ifdef Q_OS_WIN

  std::string fullyQualifiedName;

  // if we get a drive letter, use WNetGetConnection
  boost::optional<std::string> wdl = windowsDriveLetter(p);
  if (wdl){
    std::string pstring = wdl.get() + ":";

    TCHAR szDeviceName[MAX_PATH];
    DWORD dwResult, cchBuff = sizeof(szDeviceName);
    dwResult = WNetGetConnection(pstring.c_str(), szDeviceName, &cchBuff);
    if (dwResult != NO_ERROR){
      return false;
    }

    fullyQualifiedName = szDeviceName;
  } else{

    // otherwise we have a fully qualified resource name, e.g. \\server\file
    fullyQualifiedName = p.string(); // toString(p) converts backslashes to slashes
  }

  // use WNetGetResourceInformation to check status
  OS_ASSERT(!fullyQualifiedName.empty());

  DWORD dwBufferSize = sizeof(NETRESOURCE);
  LPBYTE lpBuffer;                  // buffer
  NETRESOURCE nr;
  LPTSTR pszSystem = NULL;          // variable-length strings

  // Set the block of memory to zero; then initialize
  // the NETRESOURCE structure. 
  ZeroMemory(&nr, sizeof(nr));

  nr.dwScope = RESOURCE_GLOBALNET;
  nr.dwType = RESOURCETYPE_ANY;
  nr.lpRemoteName = &fullyQualifiedName[0];

  // First call the WNetGetResourceInformation function with 
  // memory allocated to hold only a NETRESOURCE structure. This 
  // method can succeed if all the NETRESOURCE pointers are NULL.
  // If the call fails because the buffer is too small, allocate
  // a larger buffer.
  lpBuffer = (LPBYTE)malloc(dwBufferSize);
  if (lpBuffer == NULL){
    return false;
  }

  bool moreData = true;
  while (moreData){
    DWORD dwResult = WNetGetResourceInformation(&nr, lpBuffer, &dwBufferSize, &pszSystem);
    if (dwResult == NO_ERROR){
      free(lpBuffer);
      return true;
    } else if (dwResult == ERROR_MORE_DATA){

      auto newptr = (LPBYTE)realloc(lpBuffer, dwBufferSize);
      if (newptr == nullptr) {
        // whoops we had a memory allocation error and we need to clean things up
        free(lpBuffer);
        throw std::runtime_error("Memory error while attempting to allocate buffer");
      } else {
        lpBuffer = newptr;
      }

      moreData = true;

    } else {
      moreData = false;
    }
  }

  free(lpBuffer);

#endif

  return false;
}

} // openstudio
