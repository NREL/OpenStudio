/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PathHelpers.hpp"
#include "Assert.hpp"
#include "FilesystemHelpers.hpp"

#include <boost/regex.hpp>

#if (defined(_WIN32) || defined(_WIN64))
#  include <Windows.h>
#endif

namespace openstudio {

std::string modelFileExtension() {
  return {"osm"};
}

std::string componentFileExtension() {
  return {"osc"};
}

std::string tableFileExtension() {
  return {"ost"};
}

std::string documentFileExtension() {
  return {"osd"};
}

std::string rulesetFileExtension() {
  return {"osr"};
}

bool pathBeginsWith(const openstudio::path& t_shorter, const openstudio::path& t_longer) {
  openstudio::path::const_iterator begin1 = t_shorter.begin();
  openstudio::path::const_iterator end1 = t_shorter.end();

  openstudio::path::const_iterator begin2 = t_longer.begin();
  openstudio::path::const_iterator end2 = t_longer.end();

  if (std::distance(begin1, end1) > std::distance(begin2, end2)) {
    return false;  // the run dir has fewer elements than the build dir - cannot be running from builddir
  }

  // if the rundir begins with the builddir, we know it's running from the builddir
  return std::equal(begin1, end1, begin2);
}

path completePathToFile(const path& p, const path& base, const std::string& ext, bool warnOnMismatch) {
  path result(p);

  // handle file extension
  if (!ext.empty()) {
    result = setFileExtension(p, ext, false, warnOnMismatch);
    if (result.empty()) {
      result = p;
    }
  }

  // complete path
  if (!result.is_complete()) {
    try {
      if (!base.empty()) {
        result = openstudio::filesystem::complete(result, base);
      } else {
        result = openstudio::filesystem::complete(result);
      }
    } catch (...) {
      LOG_FREE(Info, "openstudio.completePathToFile", "Unable to compete path '" << toString(p) << "'. Returning an empty path.");
      return {};
    }
  }

  // check that result is a file
  if (!openstudio::filesystem::is_regular_file(result)) {
    LOG_FREE(Info, "openstudio.completePathToFile",
             "Path '" << toString(p) << "' could not be resolved to an existing file. Returning an empty path.");
    return {};
  }

  return result;
}

std::string getFileExtension(const path& p) {
  std::string pext = openstudio::filesystem::extension(p);
  if (!pext.empty()) {
    // remove '.'
    pext = std::string(++pext.begin(), pext.end());
  }
  return pext;
}

path setFileExtension(const path& p, const std::string& ext, bool replaceOnMismatch, bool warnOnMismatch) {
  path result(p);
  path wext = toPath(ext);
  std::string pext = getFileExtension(p);
  if (!pext.empty()) {
    if (pext != wext.string()) {
      if (warnOnMismatch) {
        LOG_FREE(Warn, "openstudio.setFileExtension",
                 "Path p, '" << toString(p) << "', has an unexpected file extension. Was expecting '" << toString(wext) << "'.");
      }
      if (!replaceOnMismatch) {
        return result;
      }
    }
  }  // if

  result.replace_extension(wext.string());
  return result;
}

bool makeParentFolder(const path& p, const path& base, bool recursive) {

  // get path to last directory
  path wp(p);
  if (base.empty()) {
    wp = openstudio::filesystem::complete(wp);
  } else {
    wp = openstudio::filesystem::complete(wp, base);
  }
  if (wp.has_filename()) {
    wp = wp.parent_path();
  }

  // make one or more directories as needed and as directed by recursive
  bool result = true;
  if (openstudio::filesystem::is_directory(wp)) {
    return result;
  } else if (recursive) {
    try {
      result = create_directories(wp);
    } catch (...) {
      result = false;
    }
  } else {
    try {
      result = create_directory(wp);
    } catch (...) {
      result = false;
    }
  }

  return result;
}

path relativePath(const path& p, const path& base) {
  // Note: could just use openstudio::filesystem::relative(p, base);

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
      LOG_FREE(Debug, "openstudio.utilities.core",
               "Path '" << toString(p) << "' does not extend base '" << toString(base) << "'. Try again after completing both paths.");
      return relativePath(completeP, completeBase);
    } else {
      LOG_FREE(Debug, "openstudio.utilities.core", "Path '" << toString(p) << "' does not extend base '" << toString(base) << "'.");
      return {};
    }
  }

  // p is an extension of base, keep whatever remains of p
  path result;
  while (wpIt != wpEnd) {
    result = result / (*wpIt);
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
  if (p.parent_path().empty()) {
    std::istringstream pathstream(getenv("PATH"));
    LOG_FREE(Debug, "PathHelpers", "findInSystemPath, searching for '" << p << "' in PATH'");

    std::string pathstring;
    while (std::getline(pathstream, pathstring, pathDelimiter())) {
      LOG_FREE(Trace, "PathHelpers", "findInSystemPath, searching for '" << p << "' in '" << pathstring << "'");

      auto maybepath = toPath(pathstring) / p;
      if (openstudio::filesystem::exists(maybepath) && !openstudio::filesystem::is_directory(maybepath)) {
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
  if (!openstudio::filesystem::exists(temp)) {  // || openstudio::filesystem::is_directory( temp ) ) {
    LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: temp doesn't exists");
  }

  while (openstudio::filesystem::is_symlink(temp)) {
    auto linkpath = openstudio::filesystem::read_symlink(temp);
    LOG_FREE(Trace, "PathHelpers", "completeAndNormalize: It's a symlink, linkpath = " << linkpath);

    if (linkpath.is_absolute()) {
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
  for (openstudio::path::iterator it = temp.begin(); it != temp.end(); ++it) {
    if (*it == toPath("..")) {
      if (openstudio::filesystem::is_symlink(result) || (result.filename() == toPath(".."))) {
        result /= *it;
      } else {
        result = result.parent_path();
      }
    } else if (*it != toPath(".")) {
      result /= *it;
    }
  }

  LOG_FREE(Debug, "PathHelpers", "completeAndNormalize: result = " << result);

  return result;
}

path relocatePath(const path& originalPath, const path& originalBase, const path& newBase) {
  path result;
  path temp = relativePath(originalPath, originalBase);
  LOG_FREE(Debug, "openstudio.utilities.core",
           "Original path '" << toString(originalPath) << "', relative to '" << toString(originalBase) << "' is '" << toString(temp) << "'.");
  if (!temp.empty()) {
    result = newBase / temp;
    LOG_FREE(Debug, "openstudio.utilities.core", "Relocating path to '" << toString(result) << "'.");
  }
  return result;
}

path getCompanionFolder(const path& osmPath) {
  // Note: JM 2018-09-05
  // We could include a variety of checks (verify that we passed a file, that the file exists, that extension is osm)
  // if( boost::filesystem::is_regular_file(osmPath) && (getFileExtension(osmPath) == "osm") && boost::filesystem::exists(osmPath) )
  // Not doing it for speed, we assume we do only use this with the path to an OSM, so stem() will do the right thing, that is split on the last "."
  return osmPath.parent_path() / osmPath.stem();
}

path getLastLevelDirectoryName(const path& directory) {
  // Note: JM 2018-09-05
  // We could do a variety of checks here to ensure that we did pass a directory and not a file, not doing it for speed
  // (boost::filesystem::is_directory and boost::filesystem::exists)

  // We are using filename() (badly named here) and not stem() because we passed a directory, and if there is a "." in the directory name
  // stem() is going to think the part after the last '.' is the extension.
  // filename() just strips out the parent_path which is exactly what we need
  return directory.filename();
}

std::ostream& printPathInformation(std::ostream& os, const path& p) {
  os << "p.string() = " << toString(p.string()) << '\n';
  os << "p.native() = " << toString(p.native()) << '\n';

  os << "p.root_name() = " << toString(p.root_name()) << '\n';
  os << "p.root_directory() = " << toString(p.root_directory()) << '\n';

  os << "p.parent_path() = " << toString(p.parent_path()) << '\n';
  os << "p.filename() = " << toString(p.filename()) << '\n';

  os << "p.stem() = " << toString(p.stem()) << '\n';
  os << "p.extension() = " << toString(p.extension()) << '\n';

  os << "p.empty() = " << std::boolalpha << p.empty() << '\n';
  os << "p.is_complete() = " << p.is_complete() << '\n';

  os << "openstudio::filesystem::complete(p) = " << toString(openstudio::filesystem::complete(p)) << '\n';
  os << "openstudio::filesystem::system_complete(p) = " << toString(openstudio::filesystem::system_complete(p)) << '\n';

  return os;
}

bool removeDirectory(const path& dirName) {
  try {
    openstudio::filesystem::remove_all(dirName);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

bool copyDirectory(const path& source, const path& destination) {
  // note : we are not using openstudio::filesystem::copy to copy recursively
  // because that copies the entire directory into the destination, not just the
  // contents of the directory

  // Start by creating the destination directory if it doesn't exist
  openstudio::filesystem::create_directories(destination);

  for (const auto& dirEnt : openstudio::filesystem::directory_iterator{source}) {

    const auto& srcFolderPath = dirEnt.path();
    const auto& relativeFolderPath = openstudio::filesystem::relative(srcFolderPath, source);

    try {
      // copy with recursive will deal with creating subfolders as needed
      openstudio::filesystem::copy(srcFolderPath, destination / relativeFolderPath,
                                   openstudio::filesystem::copy_options::recursive | openstudio::filesystem::copy_options::overwrite_existing);
    } catch (const std::exception&) {
      return false;
    }
  }

  return true;
}

bool isEmptyDirectory(const path& dirName) {
  if (!openstudio::filesystem::exists(dirName)) {
    return false;
  }

  if (!openstudio::filesystem::is_directory(dirName)) {
    return false;
  }

  return openstudio::filesystem::is_empty(dirName);
}

boost::optional<std::string> windowsDriveLetter(const path& p) {
  boost::optional<std::string> result;

  std::string path_str = toString(p);

  boost::regex regex("^([a-zA-Z]):");
  boost::smatch m;
  if (boost::regex_search(path_str, m, regex)) {
    result = m[1];
  }

  return result;
}

bool isNetworkPath(const path& p) {
  if (p.empty() || !p.is_absolute()) {
    return false;
  }

#if (defined(_WIN32) || defined(_WIN64))

  // TODO: JM 2018-11-06: couldn't this entire block be replaced by "PathIsNetworkPath"?

  // if this is a windows drive, check if this is mapped to a remote drive
  boost::optional<std::string> wdl = windowsDriveLetter(p);
  if (wdl) {
    std::string pstring = wdl.get() + ":\\";
    if (GetDriveType(pstring.c_str()) == DRIVE_REMOTE) {
      return true;
    }
  }

  // check if path begins with \\, e.g. \\server\file
  // Shouldn't convert backslashes to slashes
  boost::regex regex("^\\\\\\\\");
  if (boost::regex_search(p.string(), regex)) {
    return true;
  }

#endif

  return false;
}

bool isNetworkPathAvailable(const path& p) {
  // cppcheck-suppress knownConditionTrueFalse
  if (!isNetworkPath(p)) {
    return false;
  }

#if (defined(_WIN32) || defined(_WIN64))

  std::string fullyQualifiedName;

  // if we get a drive letter, use WNetGetConnection
  boost::optional<std::string> wdl = windowsDriveLetter(p);
  if (wdl) {
    std::string pstring = wdl.get() + ":";

    TCHAR szDeviceName[MAX_PATH];
    DWORD dwResult, cchBuff = sizeof(szDeviceName);
    dwResult = WNetGetConnection(pstring.c_str(), szDeviceName, &cchBuff);
    if (dwResult != NO_ERROR) {
      return false;
    }

    fullyQualifiedName = szDeviceName;
  } else {

    // otherwise we have a fully qualified resource name, e.g. \\server\file
    fullyQualifiedName = p.string();  // toString(p) converts backslashes to slashes
  }

  // use WNetGetResourceInformation to check status
  OS_ASSERT(!fullyQualifiedName.empty());

  DWORD dwBufferSize = sizeof(NETRESOURCE);
  LPBYTE lpBuffer;  // buffer
  NETRESOURCE nr;
  LPTSTR pszSystem = NULL;  // variable-length strings

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
  if (lpBuffer == NULL) {
    return false;
  }

  bool moreData = true;
  while (moreData) {
    DWORD dwResult = WNetGetResourceInformation(&nr, lpBuffer, &dwBufferSize, &pszSystem);
    if (dwResult == NO_ERROR) {
      free(lpBuffer);
      return true;
    } else if (dwResult == ERROR_MORE_DATA) {

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

}  // namespace openstudio
