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
UTILITIES_API path completePathToFile(const path& p,
                                      const path& base=path(),
                                      const std::string& ext=std::string(),
                                      bool warnOnMismatch=false);

/** Get p's extension as a string with the dot removed. */
UTILITIES_API std::string getFileExtension(const path& p);

/** Set p's extension to ext. If p has an extension not equal to ext, p's extension can be 
 *  replaced and/or a warning can be issued. Returns an empty path if there is a mismatch and 
 *  replaceOnMismatch == false. */
UTILITIES_API path setFileExtension(const path& p,
                                    const std::string& ext,
                                    bool replaceOnMismatch=false,
                                    bool warnOnMismatch=true);

/** As necessary, makes the parent folder for path p. If recursive == false, only one folder may 
 *  be constructed. If recursive == true, any number of folders may be constructed. Returns false 
 *  if parent folder does not exist after calling this function. */
UTILITIES_API bool makeParentFolder(const path& p,const path& base=path(),bool recursive=false);

/** Determines the relative path to p from base, if possible. Otherwise, returns empty path. */
UTILITIES_API path relativePath(const path& p,const path& base);

UTILITIES_API path completeAndNormalize(const path& p);

UTILITIES_API path relocatePath(const path& originalPath,
                                const path& originalBase,
                                const path& newBase);

/** Print information about path p available through boost::filesystem. */
UTILITIES_API std::ostream& printPathInformation(std::ostream& os,const path& p);

/** Recursively remove a directory, not subject to MAX_PATH */
UTILITIES_API bool removeDirectory(const path& dirName);

/** Recursively copy a directory, not subject to MAX_PATH */
UTILITIES_API bool copyDirectory(const path& source,
                                 const path& destination);

/** Determines if a directory is empty, returns false if directory does not exist, not subject to MAX_PATH */
UTILITIES_API bool isEmptyDirectory(const path& dirName);

} // openstudio

#endif //UTILITIES_CORE_PATHHELPERS_HPP
