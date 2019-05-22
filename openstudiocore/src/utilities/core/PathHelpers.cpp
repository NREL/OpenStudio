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

#include "PathHelpers.hpp"
#include "Assert.hpp"
#include "FilesystemHelpers.hpp"

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
      if (!base.empty()) { result = openstudio::filesystem::complete(result,base); }
      else { result = openstudio::filesystem::complete(result); }
    }
    catch (...) {
      LOG_FREE(Info,"openstudio.completePathToFile","Unable to compete path '" << toString(p)
               << "'. Returning an empty path.");
      return path();
    }
  }

  // check that result is a file
  if (!openstudio::filesystem::is_regular_file(result)) {
    LOG_FREE(Info,"openstudio.completePathToFile","Path '" << toString(p)
             << "' could not be resolved to an existing file. Returning an empty path.");
    return path();
  }

  return result;

}

std::string getFileExtension(const path& p) {
  std::string pext = openstudio::filesystem::extension(p);
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
  std::string pext = getFileExtension(p);
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
    wp = openstudio::filesystem::complete(wp);
  }
  else {
    wp = openstudio::filesystem::complete(wp,base);
  }
  if (wp.has_filename()) {
    wp = wp.parent_path();
  }

  // make one or more directories as needed and as directed by recursive
  bool result = true;
  if (openstudio::filesystem::is_directory(wp)) {
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
    path completeP = openstudio::filesystem::complete(p);
    path completeBase = openstudio::filesystem::complete(base);
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

const char pathDelimiter() {
  #if defined _WIN32
    const char delimiter = ';';
  #else
    const char delimiter = ':';
  #endif
  return delimiter;
}

path findInSystemPath(const path& p) {

  path result;
  // Ensure that this is just a name and not a path
  if ( p.parent_path().empty() ) {
    std::istringstream pathstream( getenv("PATH") );
    LOG_FREE(Debug, "PathHelpers", "findInSystemPath, searching for '" << p << "' in PATH'");

    std::string pathstring;
    while ( std::getline(pathstream, pathstring, pathDelimiter()) ) {
      LOG_FREE(Trace, "PathHelpers", "findInSystemPath, searching for '" << p << "' in '" << pathstring << "'");

      auto maybepath = toPath(pathstring) / p;
      if( openstudio::filesystem::exists( maybepath ) && !openstudio::filesystem::is_directory( maybepath ) ) {
        LOG_FREE(Debug, "PathHelpers", "findInSystemPath, found '" << p << "' in PATH: '" << pathstring);
        result = maybepath;
        break;
      }
    }
    if (result.empty()) {
      LOG_FREE(Debug, "PathHelpers", "findInSystemPath, p wasn't found in PATH, leaving as is");
      result = p;
    }

  } else {
    LOG_FREE(Debug, "PathHelpers", "findInSystemPath, p isn't just a name, leaving as is");
    result = p;
  }


  return result;

}

path completeAndNormalize(const path& p) {

  path temp = openstudio::filesystem::system_complete(p);

  LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: looking for p = " << p);
  LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp = " << temp);

  // TODO: is there a point continuing if temp doesn't exist?
  if( !openstudio::filesystem::exists( temp )) { // || openstudio::filesystem::is_directory( temp ) ) {
    LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp doesn't exists");
  }

  while ( openstudio::filesystem::is_symlink(temp) ) {
    auto linkpath = openstudio::filesystem::read_symlink(temp);
    LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: It's a symlink, linkpath = " << linkpath);

    if ( linkpath.is_absolute() ) {
      temp = linkpath;
      LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp is an absolute symlink (= linkpath)");

    } else {
      // Note JM 2019-04-24: temp will end up absolute but not canonical yet
      // eg:
      // temp="/home/a_folder/a_symlink"
      // linkpath="../another_folder/a_file"
      temp = temp.parent_path() / linkpath;
      // eg: temp ="/home/a_folder/../another_folder/a_file"

      LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp is a relative symlink, pointing to = " << temp);

    }
  }
  // TODO: can this actually happen?
  if (temp.empty() && !p.empty()) {
    LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp is empty, reseting to p");
    temp = p;
  }
  path result;

  // TODO: In develop3, which has boost 1.68, we can replace it with boost::filesystem::weakly_canonical
  // Note JM 2019-04-24: temp right now is absolute, but it isn't necessarilly canonical.
  // This block resolves a canonical path, even if it doesn't exist (yet?) on disk.
  for(openstudio::path::iterator it=temp.begin(); it!=temp.end(); ++it) {
    if (*it == toPath("..")) {
      if (openstudio::filesystem::is_symlink(result) || (result.filename() == toPath(".."))) {
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

  LOG_FREE(Debug, "PathHelpers", "completeAndNormalize: result = " << result);

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

path getCompanionFolder(const path& osmPath)
{
  // Note: JM 2018-09-05
  // We could include a variety of checks (verify that we passed a file, that the file exists, that extension is osm)
  // if( boost::filesystem::is_regular_file(osmPath) && (getFileExtension(osmPath) == "osm") && boost::filesystem::exists(osmPath) )
  // Not doing it for speed, we assume we do only use this with the path to an OSM, so stem() will do the right thing, that is split on the last "."
  return osmPath.parent_path() / osmPath.stem();
}

path getLastLevelDirectoryName(const path& directory)
{
  // Note: JM 2018-09-05
  // We could do a variety of checks here to ensure that we did pass a directory and not a file, not doing it for speed
  // (boost::filesystem::is_directory and boost::filesystem::exists)

  // We are using filename() (badly named here) and not stem() because we passed a directory, and if there is a "." in the directory name
  // stem() is going to think the part after the last '.' is the extension.
  // filename() just strips out the parent_path which is exactly what we need
  return directory.filename();

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

  os << "openstudio::filesystem::complete(p) = " << toString(openstudio::filesystem::complete(p)) << std::endl;
  os << "openstudio::filesystem::system_complete(p) = " << toString(openstudio::filesystem::system_complete(p)) << std::endl;

  return os;
}

bool removeDirectory(const path& dirName) {
  try {
    openstudio::filesystem::remove_all(dirName);
    return true;
  } catch (const std::exception &) {
    return false;
  }
}

bool copyDirectory(const path& source, const path& destination) {
  // note : we are not using openstudio::filesystem::copy to copy recursively
  // because that copies the entire directory into the destination, not just the
  // contents of the directory
  for (const auto &file : openstudio::filesystem::recursive_directory_files(source))
  {
    try {
      openstudio::filesystem::create_directories( (destination / file).parent_path());
      openstudio::filesystem::copy_file(source / file, destination / file);
    } catch (const std::exception &) {
      return false;
    }
  }

  return true;
}

bool isEmptyDirectory(const path& dirName)
{
  if (!openstudio::filesystem::exists(dirName)) {
    return false;
  }

  if (!openstudio::filesystem::is_directory(dirName)) {
    return false;
  }

  return openstudio::filesystem::is_empty(dirName);
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
