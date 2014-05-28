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

#ifndef UTILITIES_IDD_IDDFILE_HPP
#define UTILITIES_IDD_IDDFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "IddObject.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"

#include <string>
#include <ostream>
#include <vector>

namespace openstudio{

// forward declarations
class IddFactorySingleton;
namespace detail{
  class IddFile_Impl;
} // detail

/** IddFile provides parsing and printing of text files in Input Data Definition (IDD) format. 
 *  IDD is a schema definition format defined by the EnergyPlus team, adopted with slight 
 *  modifications for the OpenStudio project. IddFile is a shared object. */
class UTILITIES_API IddFile {
 public:
  /** @name Constructors */
  //@{

  /** Default constructor returns an IddFile with no version, no header, and no objects. */
  IddFile();

  /** Copy constructor returns an IddFile that shares its data with other. */
  IddFile(const IddFile& other);

  /** Returns an IddFile containing a single, default constructed IddObject of Catchall type. */
  static IddFile catchallIddFile();

  //@}
  /** @name Getters */
  //@{

  /** Returns the file version (as specified in the header). */
  std::string version() const;

  /** Returns the file header. */
  std::string header() const;

  /** Returns all the objects in the file. */
  std::vector<IddObject> objects() const;

  /** Returns all groups in the file (e.g. "" and "Simulation Parameters"). */
  std::vector<std::string> groups() const;

  /** Returns all objects in group (e.g. "Simulation Parameters"). */
  std::vector<IddObject> getObjectsInGroup(const std::string& group) const;

  /** Returns the objects whose names match objectRegex. */
  std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;

  /** Returns the version IddObject for this file, if possible. */
  boost::optional<IddObject> versionObject() const;

  /** Returns the object named objectName (case-insensitive). Return value will evaluate to false
   *  if there is no such object in the IddFile. */
  boost::optional<IddObject> getObject(const std::string& objectName) const;

  /** Returns the object of type objectType, if possible. Return value will evaluate to false if 
   *  there is no such object in the IddFile, or if IddObjectType == IddObjectType::UserCustom. */
  boost::optional<IddObject> getObject(IddObjectType objectType) const;

  /** Returns all objects in the file that are required. An IdfFile or Workspace conforming to 
   *  this IddFile must contain at least one object of each of these types to be valid at 
   *  StrictnessLevel::Final. */
  std::vector<IddObject> requiredObjects() const;

  /** Returns all objects in the file that are marked as unique. An IdfFile or Workspace 
   *  conforming to this IddFile must contain at most one object of each of these types to be 
   *  valid at StrictnessLevel::Final. */
  std::vector<IddObject> uniqueObjects() const;

  //@}
  /** @name Serialization */
  //@{

  /** Load an IddFile from std::istream, if possible. */
  static boost::optional<IddFile> load(std::istream& is);

  /** Load an IddFile from path p, if possible. */
  static boost::optional<IddFile> load(const openstudio::path& p);

  /** Prints this file to std::ostream os. */
  std::ostream& print(std::ostream& os) const;

  /** Saves file to path p. Will construct the parent folder if necessary and if its parent
   *  folder already exists. Will only overwrite an existing file if overwrite==true. If no 
   *  extension is provided will use 'idd'. */
  bool save(const openstudio::path& p, bool overwrite=false);

  //@}
 protected:
  friend class IddFactorySingleton;

  /// set version
  void setVersion(const std::string& version);

  /// set header
  void setHeader(const std::string& header);

  /// add an object
  void addObject(const IddObject& object);

 private:
  // impl
  std::shared_ptr<detail::IddFile_Impl> m_impl;

  IddFile(const std::shared_ptr<detail::IddFile_Impl>& impl);

  // configure logging
  REGISTER_LOGGER("utilities.idd.IddFile");
};

/** \relates IddFile */
typedef boost::optional<IddFile> OptionalIddFile;

/** \relates IddFile */
typedef std::vector<IddFile> IddFileVector;

/** \relates IddFile */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddFile& iddFile);

} // openstudio

#endif // UTILITIES_IDD_IDDFILE_HPP
