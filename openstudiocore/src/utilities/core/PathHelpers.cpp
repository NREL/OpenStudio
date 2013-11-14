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

#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Logger.hpp>
#include <boost/filesystem.hpp>

#include <QDir>
#include <QFile>
#include <QFileInfo>

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
  path::string_type pext = boost::filesystem::extension(p);
  if (!pext.empty()) {
    // remove '.'
    pext = path::string_type(++pext.begin(),pext.end());
  }
  return toString(pext);
}

path setFileExtension(const path& p,
                      const std::string& ext,
                      bool replaceOnMismatch,
                      bool warnOnMismatch)
{
  path result(p);
  path wext = toPath(ext);
  path::string_type pext = boost::filesystem::extension(p);
  if (!pext.empty()) {
    // remove '.' from pext
    pext = path::string_type(++pext.begin(),pext.end());
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
    LOG_FREE(Debug,"openstudio.utilities.core","Reloacting path to '" << toString(result) << "'.");
  }
  return result;
}

std::ostream& printPathInformation(std::ostream& os,const path& p) {
  os << "p.string() = " << toString(p.string()) << std::endl;
  os << "p.file_string() = " << toString(p.file_string()) << std::endl;
  os << "p.directory_string() = " << toString(p.directory_string()) << std::endl;

  os << "p.external_file_string() = " << toString(p.external_file_string()) << std::endl;
  os << "p.external_directory_string() = " << toString(p.external_directory_string()) << std::endl;

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
    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
    {
      if (info.isDir()) 
      {
        result = removeDirectory(toPath(info.absoluteFilePath()));
        if (result == false) std::cout << "First: " << toString(info.absoluteFilePath()) << std::endl;
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
    if (result == false) std::cout << "Second: " << toString(dirName) << std::endl;
  }

  return result;
}

bool copyDirectory(const path& source, const path& destination) {

  if (!QDir().mkpath(toQString(destination)))
  {
    return false;
  }

  QDir srcDir(toQString(source));

  Q_FOREACH(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
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
  if (!exists(dirName)){
    return false;
  }
  if (!is_directory(dirName)){
    return false;
  }

  QDir dir(toQString(dirName));
  return dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).empty();
}

} // openstudio
