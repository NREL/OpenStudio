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

#ifndef UTILITIES_IDF_IDFFILE_HPP
#define UTILITIES_IDF_IDFFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "../idd/IddFileAndFactoryWrapper.hpp"
#include "IdfObject.hpp"

#include "../core/Path.hpp"

#include <string>
#include <ostream>
#include <vector>

namespace openstudio {

// FORWARD DECLARATIONS
class ProgressBar;
class StrictnessLevel;
class ValidityReport;
class VersionString;
namespace detail {
  class Workspace_Impl;
}

/** IdfFile provides parsing and printing of text files in EnergyPlus Input Data File (IDF)
 *  format. This class can be used for ready-to-simulate EnergyPlus .idf files, OpenStudio .osm
 *  files, and partial idf/osm/osc files. This class expects to be constructed with the
 *  appropriate IddFile. An instance of the IddFileType enumeration class can be used to specify
 *  the latest EnergyPlus or OpenStudio IDD files. Previous versions of these IDD files, or IDD
 *  files of a different origin altogether, can be specified by explicitly calling IddFile::load,
 *  and then using that IddFile instance to load or construct an IdfFile.
 *
 *  IdfFile is not a shared object. It is composed of an ordered list of \link IdfObject
 *  IdfObjects\endlink, each of which is a shared object. */
class UTILITIES_API IdfFile
{
 public:
  /** @name Constructors */
  //@{

  /** Default constructor with optional IddFileType argument (default is IddFileType::OpenStudio).
   *  Initializes the file by adding a version IdfObject. */
  IdfFile(IddFileType iddFileType = IddFileType("OpenStudio"));

  /** Construct from iddFile. Typically, iddFile is of IddFileType::UserCustom, that is, loaded
   *  from disk. Legacy OpenStudio IDD files back through Version 0.7.0 are available through the
   *  IddFactory. If possible, a version IdfObject is added. */
  IdfFile(const IddFile& iddFile);

  //@}
  /** @name Getters */
  // @{

  /** Returns the file header, that is, the comment block at the top of the file. */
  std::string header() const;

  /** Returns the version of this model, as determined by the IddFile header. */
  VersionString version() const;

  /** Returns the underlying IddFile (the schema for this IdfFile). */
  IddFile iddFile() const;

  /** Returns the underlying IddFile's IddFileType. */
  IddFileType iddFileType() const;

  /** Returns the version IdfObject in this file, if it exists and is unique. Typically, the
   *  OS_ASSERT below will pass (but is not a trivial test).
   *  \code
   *  IdfObject vo = versionObject().get();
   *  OS_ASSERT(VersionString(vo.getString(vo.numFields() - 1,true).get()) == version());
   *  \endcode */
  boost::optional<IdfObject> versionObject() const;

  /** Returns the object at index. */
  boost::optional<IdfObject> getObject(unsigned index) const;

  /** Returns the object with .handle() == handle, if such an object exists in this file. */
  boost::optional<IdfObject> getObject(const Handle& handle) const;

  /** Returns all the objects in this file, excluding versionObject(). */
  std::vector<IdfObject> objects() const;

  /** Returns all objects of objectType (e.g. IddObjectType("Zone"), IddObjectType("OS:Space"))
   *  in this file. */
  std::vector<IdfObject> getObjectsByType(IddObjectType objectType) const;

  /** Returns all objects in this file for which .iddObject() == objectType. */
  std::vector<IdfObject> getObjectsByType(const IddObject& objectType) const;

  /** Returns all objects named name (case insensitive). */
  std::vector<IdfObject> getObjectsByName(const std::string& name) const;

  //@}
  /** @name Setters */
  //@{

  /** Set the header for this file. Will be printed as a comment block at the top of the file,
   *  before the objects. Automatically prefixes comment characters as needed. */
  void setHeader(const std::string& header);

  /** Append object to the end of this file. */
  void addObject(const IdfObject& object);

  /** Append objects to the end of this file. */
  void addObjects(const std::vector<IdfObject>& objects);

  /** Insert object immediately before the first object in this file whose IddObjectType value
   *  is greater than object's. */
  void insertObjectByIddObjectType(const IdfObject& object);

  /** Removes object from this file if it is in this file. Otherwise returns false. */
  bool removeObject(const IdfObject& object);

  /** Attempts to remove each element of objects from this file, returning the number of
   *  successfully removed objects. */
  int removeObjects(const std::vector<IdfObject>& objects);

  //@}
  /** @name Queries */
  //@{

  /** Returns true if there is no header and no objects. (A versionObject does not count against
   *  emptiness.) */
  bool empty() const;

  /** Returns the number of non-version objects in this file. */
  unsigned numObjects() const;

  /** Returns the number of objects of objectType in this file. */
  unsigned numObjectsOfType(IddObjectType objectType) const;

  /** Returns the number of objects of objectType in this file. */
  unsigned numObjectsOfType(const IddObject& objectType) const;

  /** Returns true if this IdfFile is valid, that is, has no \link DataError DataErrors\endlink
   *  at or below level. */
  bool isValid(StrictnessLevel level) const;

  /** Returns a ValidityReport for this IdfFile containing all \link DataError DataErrors\endlink
   *  at or below level. */
  ValidityReport validityReport(StrictnessLevel level) const;

  //@}
  /** @name Serialization */
  //@{

  /** Load an IdfFile from std::istream using the IDD defined by IddFactory and iddFileType, if
   *  possible. */
  static boost::optional<IdfFile> load(std::istream& is, const IddFileType& iddFileType, ProgressBar* progressBar = nullptr);

  /** Load an IdfFile from std::istream using iddFile, if possible. */
  static boost::optional<IdfFile> load(std::istream& is, const IddFile& iddFile, ProgressBar* progressBar = nullptr);

  /** Load an IdfFile from path using the IddFactory, and choosing iddFileType based on file
   *  extension, if possible. (IddFileType::OpenStudio if extension is modelFileExtension() or
   *  componentFileExtension(), IddFileType::EnergyPlus otherwise.) */
  static boost::optional<IdfFile> load(const path& p, ProgressBar* progressBar = nullptr);

  /** Load an IdfFile from path using the IddFactory and iddFileType, if possible. Will attempt to
   *  complete the path by tacking on .osm or .idf as appropriate. */
  static boost::optional<IdfFile> load(const path& p, const IddFileType& iddFileType, ProgressBar* progressBar = nullptr);

  /** Load an IdfFile from path using iddFile, if possible. If no file extension is provided, will
   *  try "idf". */
  static boost::optional<IdfFile> load(const path& p, const IddFile& iddFile, ProgressBar* progressBar = nullptr);

  /** Quick load method that uses the IddFile::catchallIddFile and stops parsing once a version
   *  identifier is found. Used to determine the appropriate IddFile to use for a full load. */
  static boost::optional<VersionString> loadVersionOnly(std::istream& is);

  /** Quick load method that uses the IddFile::catchallIddFile and stops parsing once a version
   *  identifier is found. Used to determine the appropriate IddFile to use for a full load. */
  static boost::optional<VersionString> loadVersionOnly(const path& p);

  /** Print this file to std::ostream os. */
  std::ostream& print(std::ostream& os) const;

  /** Save this file to path p. Will construct the parent folder if necessary and if its parent
   *  folder already exists. Will only overwrite an existing file if overwrite==true. If no
   *  extension is provided will use modelFileExtension() for files using IddFileType::OpenStudio,
   *  and 'idf' otherwise. Returns true if the save operation is successful; false otherwise. */
  bool save(const openstudio::path& p, bool overwrite = false);

  //@}

 protected:
  friend class detail::Workspace_Impl;

  IddFileAndFactoryWrapper iddFileAndFactoryWrapper() const;
  void setIddFileAndFactoryWrapper(const IddFileAndFactoryWrapper& iddFileAndFactoryWrapper);

 private:
  std::string m_header;
  std::vector<IdfObject> m_objects;
  std::set<unsigned> m_versionObjectIndices;

  IddFileAndFactoryWrapper m_iddFileAndFactoryWrapper;

  // SETTERS

  void addVersionObject();

  // QUERIES

  std::vector<IdfObject> m_objectsWithConflictingNames(const std::string& name, bool getAll) const;

  // SERIALIZATION

  /// private load function that uses m_iddFile and m_iddFileType initialized elsewhere
  bool m_load(std::istream& is, ProgressBar* progressBar = nullptr, bool versionOnly = false);

  // configure logging
  REGISTER_LOGGER("utilities.idf.IdfFile");
};

/// optional idf file
using OptionalIdfFile = boost::optional<IdfFile>;

/// vector of idf file
using IdfFileVector = std::vector<IdfFile>;

// ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IdfFile& idfFile);

}  // namespace openstudio

#endif  //UTILITIES_IDF_IDFFILE_HPP
