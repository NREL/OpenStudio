/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDFILE_HPP
#define UTILITIES_IDD_IDDFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "IddObject.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"

#include <string>
#include <ostream>
#include <vector>

namespace openstudio {

// forward declarations
class IddFactorySingleton;
namespace detail {
  class IddFile_Impl;
}  // namespace detail

class VersionString;

/** IddFile provides parsing and printing of text files in Input Data Definition (IDD) format.
 *  IDD is a schema definition format defined by the EnergyPlus team, adopted with slight
 *  modifications for the OpenStudio project. IddFile is a shared object. */
class UTILITIES_API IddFile
{
 public:
  /** @name Constructors */
  //@{

  /** Default constructor returns an IddFile with no version, no header, and no objects. */
  IddFile();

  /** Copy constructor returns an IddFile that shares its data with other. */
  // IddFile(const IddFile& other);

  /** Returns an IddFile containing a single, default constructed IddObject of Catchall type. */
  static IddFile catchallIddFile();

  //@}
  /** @name Getters */
  //@{

  /** Returns the file version (as specified in the header). */
  std::string version() const;

  /** Returns the file build (as specified in the header). */
  std::string build() const;

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
  // cppcheck-suppress functionConst
  bool save(const openstudio::path& p, bool overwrite = false);

  /** Returns the version and build SHA from the given Idd. If build SHA is not present .second will be empty.
   *
   *  \throws an exception with a meaningful error message if something goes wrong
   */
  static std::pair<VersionString, std::string> parseVersionBuild(const openstudio::path& p);

  //@}
 protected:
  friend class IddFactorySingleton;

  /// set version
  void setVersion(const std::string& version);

  /// set build
  void setBuild(const std::string& build);

  /// set header
  void setHeader(const std::string& header);

  /// add an object
  void addObject(const IddObject& object);

 private:
  // impl
  std::shared_ptr<detail::IddFile_Impl> m_impl;

  IddFile(const std::shared_ptr<detail::IddFile_Impl>& impl) noexcept;
  IddFile(std::shared_ptr<detail::IddFile_Impl>&& impl) noexcept;

  // configure logging
  REGISTER_LOGGER("utilities.idd.IddFile");
};

/** \relates IddFile */
using OptionalIddFile = boost::optional<IddFile>;

/** \relates IddFile */
using IddFileVector = std::vector<IddFile>;

/** \relates IddFile */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddFile& iddFile);

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDFILE_HPP
