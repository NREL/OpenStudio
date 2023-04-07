/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
