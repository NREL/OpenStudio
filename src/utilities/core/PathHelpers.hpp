/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_PATHHELPERS_HPP
#define UTILITIES_CORE_PATHHELPERS_HPP

#include "../UtilitiesAPI.hpp"

#include "Path.hpp"

namespace openstudio {

/** Single location for storing the default extension for Model serialization files. (That is,
 *  the file extension to be used for Idf files following the IddFileType::OpenStudio Idd, or a
 *  subset thereof.) */
UTILITIES_API std::string modelFileExtension();

/** Single location for storing the default extension for Component serialization files. (That is,
 *  the file extension to be used for Idf files following the IddFileType::OpenStudio Idd, and
 *  containing a single Component.) */
UTILITIES_API std::string componentFileExtension();

UTILITIES_API std::string tableFileExtension();

UTILITIES_API std::string documentFileExtension();

/** Single location for storing the default extension for rulesengine::Ruleset serialization files. */
UTILITIES_API std::string rulesetFileExtension();

/** Returns completed path if p is a path to a file on this system. Otherwise, returns an empty
 *  path. If base is not empty, completion is done relative to base. If ext is not empty, and p
 *  has no extension, then p's extension is set to ext. If ext is not empty and p has an
 *  extension, then a warning may be logged if they do not match. */
UTILITIES_API path completePathToFile(const path& p, const path& base = path(), const std::string& ext = std::string(), bool warnOnMismatch = false);

/** Get p's extension as a string with the dot removed. */
UTILITIES_API std::string getFileExtension(const path& p);

/** Set p's extension to ext. If p has an extension not equal to ext, p's extension can be
 *  replaced and/or a warning can be issued. Returns an empty path if there is a mismatch and
 *  replaceOnMismatch == false. */
UTILITIES_API path setFileExtension(const path& p, const std::string& ext, bool replaceOnMismatch = false, bool warnOnMismatch = true);

/** As necessary, makes the parent folder for path p. If recursive == false, only one folder may
 *  be constructed. If recursive == true, any number of folders may be constructed. Returns false
 *  if parent folder does not exist after calling this function. */
UTILITIES_API bool makeParentFolder(const path& p, const path& base = path(), bool recursive = false);

/** Determines the relative path to p from base, if possible. Otherwise, returns empty path. */
UTILITIES_API path relativePath(const path& p, const path& base);

UTILITIES_API path completeAndNormalize(const path& p);

UTILITIES_API path relocatePath(const path& originalPath, const path& originalBase, const path& newBase);

/* Given the path to an OSM file, get the companion folder in which files, measures, etc, are stored
 * eg if /path/to/model.osm, returns the folder /path/to/model/ */
UTILITIES_API path getCompanionFolder(const path& osmPath);

/* Given the path to a **directory**, return the name of the last level directory
 * eg if /path/to/folder, returns 'folder' */
UTILITIES_API path getLastLevelDirectoryName(const path& directory);

/** Print information about path p available through openstudio::filesystem. */
UTILITIES_API std::ostream& printPathInformation(std::ostream& os, const path& p);

/** Recursively remove a directory, not subject to MAX_PATH */
UTILITIES_API bool removeDirectory(const path& dirName);

/** Recursively copy a directory, not subject to MAX_PATH */
UTILITIES_API bool copyDirectory(const path& source, const path& destination);

/** Determines if a directory is empty, returns false if directory does not exist, not subject to MAX_PATH */
UTILITIES_API bool isEmptyDirectory(const path& dirName);

/** Returns the drive letter, e.g. if path is "C:\temp" returns "C".*/
UTILITIES_API boost::optional<std::string> windowsDriveLetter(const path& p);

/** Determines if a path is on a network drive.
  * Returns false if path is not absolute.
  * Currently only implemented for Windows, returns false on other platforms. */
UTILITIES_API bool isNetworkPath(const path& p);

/** Determines if a path is on a network drive and if that network resources is available.
  * Returns false if path is not absolute or if path is not a network path.
  * Currently only implemented for Windows, returns false on other platforms. */
UTILITIES_API bool isNetworkPathAvailable(const path& p);

/** Helper to determine if a path begins with another path */
UTILITIES_API bool pathBeginsWith(const openstudio::path& t_shorter, const openstudio::path& t_longer);

/** Tries to locate a file in your PATH, returning unchanged if p isn't just a filename (has a parent_path) or if not found */
UTILITIES_API path findInSystemPath(const path& p);

/** Returns the delimiter for the $PATH env variable. Basically ";" on windows, ":" otherwise */
UTILITIES_API const char pathDelimiter();

}  // namespace openstudio

#endif  //UTILITIES_CORE_PATHHELPERS_HPP
