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

#ifndef UTILITIES_BCL_LOCALBCL_HPP
#define UTILITIES_BCL_LOCALBCL_HPP

#include "BCL.hpp"
#include "../core/Optional.hpp"
#include "../core/Path.hpp"

#include <string>
#include <vector>

class QSqlDatabase;
class QWidget;

namespace openstudio{

  /// \todo This class is currently a singleton implemented with a first use static
  ///       this may cause problems with threading in the future and should be moved
  ///       into some other singleton implementation. ONE OPTION would be to
  ///       instantiate the singleton at the time of QApplication construction in the Application singleton
  ///       this would avoid all known problems with threading. The Singleton<> template cannot be used because
  ///       of static initialize and destruction order problems caused by statics used in the QDatabase stuff
  class UTILITIES_API LocalBCL : public BCL {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// Static method to retrieve the current local BCL singleton
    /// If a local BCL singleton does not exist, one will be created at the default library path stored in user preferences
    static LocalBCL &instance();

    /// Static method to retrieve the local BCL singleton at the given library path
    /// If a local BCL singleton does not exist, one will be created at the given library path
    /// If a local BCL singleton does exist, but is not at the given path,
    /// the current one will be closed and a new one will be created at the given library path
    static LocalBCL &instance(const path& libraryPath);

    //@}
    /** @name Destructors */
    //@{

    /// Static method to close the current local BCL singleton if it exists
    static void close();

    /// virtual destructor
    virtual ~LocalBCL();

    //@}
    /** @name Inherited members */
    //@{

    /// Get the component by uid
    virtual boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId = "") const override;

    /// Get the measure by uid
    virtual boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId = "") const override;

    /// Get all components
    std::vector<BCLComponent> components() const;

    /// Get all measures
    std::vector<BCLMeasure> measures() const;

     /// Get all measure uids
    std::vector<std::string> measureUids() const;

    // TODO: make this take a vector of remote bcl filters
    /// Perform a component search of the library
    std::vector<BCLComponent> searchComponents(const std::string& searchTerm,
      const std::string& componentType) const;
    std::vector<BCLComponent> searchComponents(const std::string& searchTerm,
      const unsigned componentTypeTID) const;

    // TODO: make this take a vector of remote bcl filters
    /// Perform a measure search of the library
    virtual std::vector<BCLMeasure> searchMeasures(const std::string& searchTerm,
      const std::string& componentType) const;
    virtual std::vector<BCLMeasure> searchMeasures(const std::string& searchTerm,
      const unsigned componentTypeTID) const;

    //@}
    /** @name Class members */
    //@{

    /// Add a component to the local library
    bool addComponent(BCLComponent& component);

    /// Remove a component from the local library and delete its directory
    bool removeComponent(BCLComponent& component);

    /// Add a measure to the local library
    bool addMeasure(BCLMeasure& measure);

    /// Remove a measure from the local library and delete its directory
    bool removeMeasure(BCLMeasure& measure);

    /// Search for components with attributes matching those in searchTerms
    std::vector<BCLComponent> componentAttributeSearch(const std::vector<std::pair<std::string, std::string> >& searchTerms) const;

    /// Search for measures with attributes matching those in searchTerms
    std::vector<BCLMeasure> measureAttributeSearch(const std::vector<std::pair<std::string, std::string> >& searchTerms) const;

    /// Prompt the user for auth key using a modal dialog, this is a blocking call that requires user input.
    /// If user enters input returns the value of setAuthKey, otherwise returns false
    bool prodAuthKeyUserPrompt(QWidget* parent = nullptr);
    bool devAuthKeyUserPrompt(QWidget* parent = nullptr);

    /// Return production OAuth key
    std::string prodAuthKey() const;

    /// Set the production OAuth key, validates the auth key before saving
    bool setProdAuthKey(const std::string& prodAuthKey);

    /// Return development OAuth key
    std::string devAuthKey() const;

    /// Set the development OAuth key, validates the auth key before saving
    bool setDevAuthKey(const std::string& devAuthKey);

    /// Returns the path to the local BCL library
    QString libraryPath() const;

    /// Relocates the local BCL library, stores the library path in user preferences
    bool setLibraryPath(const std::string& libraryPath);

    //@}
  private:

    /// private constructor
    LocalBCL(const path& libraryPath);

    // no body on purpose, do not want this generated
    LocalBCL(const LocalBCL& other);

    bool initializeLocalDb();

    bool updateLocalDb();

    bool validateProdAuthKey(const std::string& authKey);
    bool validateDevAuthKey(const std::string& authKey);

    QString escape(const std::string& s) const;

    std::set<std::pair<std::string, std::string> > attributeSearch(
      const std::vector<std::pair<std::string, std::string> >& searchTerms,
      const std::string& componentType) const;

    std::string formatString(double d, uint prec = 15);

    static std::shared_ptr<LocalBCL> &instanceInternal();

    QString m_libraryPath;
    const QString m_dbName;
    QString dbVersion;
    std::string m_prodAuthKey;
    std::string m_devAuthKey;
  };

} // openstudio

#endif // UTILITIES_BCL_LOCALBCL_HPP
