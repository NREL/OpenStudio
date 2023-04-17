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

#ifndef UTILITIES_IDD_IDDFILEANDFACTORYWRAPPER_HPP
#define UTILITIES_IDD_IDDFILEANDFACTORYWRAPPER_HPP

#include "../UtilitiesAPI.hpp"
#include "IddFile.hpp"
#include "IddEnums.hpp"

namespace openstudio {

/** Wraps the functionality of IddFile and \link IddFactorySingleton IddFactory \endlink
 *  into one class for use by IdfFile and Workspace. */
class UTILITIES_API IddFileAndFactoryWrapper
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor sets IddFileType to IddFileType::OpenStudio. */
  IddFileAndFactoryWrapper();

  /** Construct from UserCustom IddFile. */
  IddFileAndFactoryWrapper(const IddFile& iddFile);

  /** Construct wrapper that provides access to the iddFileType IddFile through the \link
   *  IddFactorySingleton IddFactory \endlink. Logs a warning if iddFileType ==
   *  IddFileType::UserCustom, since this results in an empty IddFile. */
  IddFileAndFactoryWrapper(IddFileType iddFileType);

  //@}
  /** @name Getters */
  //@{

  /** Get the file version. */
  std::string version() const;

  /** Get the file header. */
  std::string header() const;

  /** Get all the objects in the file. */
  std::vector<IddObject> objects() const;

  /** Get all groups in the file (e.g. "" and "Simulation Parameters"). */
  std::vector<std::string> groups() const;

  /** Get all the objects in group (e.g. "Simulation Parameters"). */
  std::vector<IddObject> getObjectsInGroup(const std::string& group) const;

  /** Get the IddObjects that match objectRegex. */
  std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;

  /** If possible, returns the version IddObject for this IddFile. */
  boost::optional<IddObject> versionObject() const;

  /** Get the IddObject named objectName. Return value will evaluate to false if there is no
   *  such object in the file. */
  boost::optional<IddObject> getObject(const std::string& objectName) const;

  /** Get the IddObject of type objectType. Return value will evaluate to false if there is no
   *  such object in the IddFile, or if IddObjectType == IddObjectType::UserCustom. */
  boost::optional<IddObject> getObject(IddObjectType objectType) const;

  /** Get all \link IddObject IddObjects\endlink in the file that are required. An IdfFile
   *  (or Workspace) conforming to this IddFile must contain at least one object of each of these
   *  types. */
  std::vector<IddObject> requiredObjects() const;

  /** Get all \link IddObject IddObjects\endlink in the file that are marked as unique. An IdfFile
   *  (or Workspace) conforming to this IddFile must contain at most one object of each of these
   *  types. */
  std::vector<IddObject> uniqueObjects() const;

  /** Get the IddFile, which may have been explicitly specified by the user, or may be implicitly
   *  specified by reference to the \link IddFactorySingleton IddFactory\endlink. */
  IddFile iddFile() const;

  /** Get the IddFileType. If iddFileType() == IddFileType::UserCustom, then the file has been
   *  explicitly specified by the user. Otherwise, the file is implicitly defined and accessed
   *  through IddFactory.  */
  IddFileType iddFileType() const;

  //@}
  /** @name Setters */
  //@{

  /** Set the IddFile explicitly. This option does not use the \link IddFactorySingleton
   *  IddFactory\endlink. */
  void setIddFile(const IddFile& iddFile);

  /** Set the IddFile by specifying iddFileType. This option uses the \link IddFactorySingleton
   *  IddFactory\endlink, unless iddFileType == IddFileType::UserCustom, in which case a warning
   *  is logged. */
  void setIddFile(IddFileType iddFileType);

  //@}
  /** @name Queries */
  //@{

  /** Returns whether an IddObject of objectType is in iddFile(), unless objectType ==
   *  IddObjectType::UserCustom. In the latter case, false will always be returned. To test whether
   *  a UserCustom file is being used, check the value of iddFileType(). */
  bool isInFile(IddObjectType objectType) const;

  /** Returns whether there is an IddObject named objectName in iddFile(). */
  bool isInFile(const std::string& objectName) const;

  //@}
 private:
  boost::optional<IddFile> m_iddFile;
  boost::optional<IddFileType> m_iddFileType;

  // configure logging
  REGISTER_LOGGER("utilities.IddFileAndFactoryWrapper");
};

// helper function to get a particular Idd File that was released with CBECC-Com but not OpenStudio
UTILITIES_API IddFile get_1_9_0_CBECC_IddFile();

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDFILEANDFACTORYWRAPPER_HPP
