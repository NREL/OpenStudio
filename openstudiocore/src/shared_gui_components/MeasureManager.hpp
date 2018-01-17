/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP
#define SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP

#include "LocalLibraryController.hpp"
#include "../analysisdriver/SimpleProject.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/UUID.hpp"
#include "../ruleset/RubyUserScriptInfoGetter.hpp"
#include "../model/Model.hpp"
#include <vector>
#include <map>
#include <QSharedPointer>
#include <QPointer>
#include <QApplication>

class QEvent;

namespace openstudio {

class BaseApp;
class BCLMeasure;

namespace osversion {

  class VersionTranslator;

}

class LocalLibraryController;

/***
* MeasureManager provides a single interface for finding measures on the user's computer in a variety of places.  
*
* It also manages checking if a newer version of a given measure is available on the user's computer and updating
* a SimpleProject to use the updated measure.  Measures directories are searched in the following order 
* 1) pat application measures, 2) my measures, 3) bcl measures.  This supports the following use cases:
*
* A user has started a measure in their my measures directory, then uploaded to BCL, then downloaded that measure then
* a measure with the same uid will exist in both my measures and the bcl measures.  The my measures version is prefered.
* 
* A user has downloaded a measure from the bcl to their local bcl and manually copied the directory to my measures.  
* This use case is not supported but in this case the my measures version is preferred.
*
* A user has manually downloaded a measure from the bcl to their my measures directory as well as downloaded it to their local bcl.  
* In this case the my measures version is preferred.
*
* A user has downloaded a copy of a built in measure from the bcl to their local bcl.  
* In this case the built in version is preferred.
* 
* A user has manually downloaded a copy of a built in measure from the bcl to their my measures directory.  
* In this case the built in version is preferred.
*
* A user has downloaded a copy of a built in measure from the bcl to their local bcl then duplicated it to my measures.  
* In this case there is no uuid conflict.
* 
* A user has manually downloaded a copy of a built in measure from the bcl to their my measures directory then duplicated it to my measures.  
* In this case there is no uuid conflict.
*
* A user has manually copies a measure in their my measures directory to their my measures directory.  
* In this case only one of the measures is displayed.
*
**/
#if defined(openstudio_lib_EXPORTS) || defined(COMPILING_FROM_OSAPP)
#include "../openstudio_lib/OpenStudioAPI.hpp"
class OPENSTUDIO_API MeasureManager : public QObject
#else
class MeasureManager : public QObject
#endif
{
  Q_OBJECT;

  public:
    // Constructor taking a RubyUserScriptInfoGetter
    MeasureManager(const QSharedPointer<ruleset::RubyUserScriptInfoGetter> &t_infoGetter, BaseApp *t_app);

    virtual ~MeasureManager() {}

    // Returns true if given measure is managed by the application and not the user
    bool isManagedMeasure(const UUID & id) const;

    // Measures shipped with OpenStudio.
    std::vector<BCLMeasure> openstudioMeasures() const;

    // Measures downloaded from the BCL.
    std::vector<BCLMeasure> bclMeasures() const;

    // Measures saved in the user's home directory.
    std::vector<BCLMeasure> myMeasures() const;

    // Get combined list of measures without duplicates, uses same logic as getMeasure
    // If includeOpenStudioMeasures is false then OpenStudio measures will not be included, otherwise they will be.
    std::vector<BCLMeasure> combinedMeasures(bool includeOpenStudioMeasures = true) const;

    // Retrieve a measure from openstudioMeasures, myMeasures, and bclMeasures by id.
    boost::optional<BCLMeasure> getMeasure(const UUID & id);

    /// Updates an individual measure. Does not ask for user approval, approval is assumed.
    /// \returns true if the update succeeded.
    std::pair<bool,std::string> updateMeasure(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure);

    // insert / replace a measure by UUID. If the measure already exists in the project
    // the user is prompted as to how to deal with it. OSArguments are loaded as needed
    BCLMeasure insertReplaceMeasure(analysisdriver::SimpleProject &t_project, const UUID &t_id);

    // Updates the given set of measures in the current project. Does not ask for user approval. Approval is assumed
    // when this method is called.
    void updateMeasures(analysisdriver::SimpleProject &t_project, 
      const std::vector<BCLMeasure> &t_newMeasures, 
      bool t_showMessage=true);

    void setLibraryController(const QSharedPointer<LocalLibraryController> &t_libraryController);

    std::vector<ruleset::OSArgument> getArguments(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure);

    std::string suggestMeasureGroupName(const BCLMeasure &t_measure);

    std::string suggestMeasureName(const BCLMeasure &t_measure, bool t_fixed);

    bool isMeasureSelected();

    QSharedPointer<ruleset::RubyUserScriptInfoGetter> infoGetter() const;

  public slots:
    /// Update the UI display for all measures. Does recompute the measure's XML.
    /// Does not update the measures in the project at all
    void updateMeasuresLists();

    /// For all measures in the "openstudioMeasures" list which have changed relative to the version
    /// in the project, update the project to the new version
    /// 
    /// Does not ask for user approval
    void updateOpenStudioMeasures(analysisdriver::SimpleProject &t_project);

    /// Updates the UI for all measures.
    /// For all measures in the "myMeasures" list which have changed relative to the version
    /// in the project, update the project to the new version
    /// 
    /// Does not ask for user approval
    void updateMyMeasures(analysisdriver::SimpleProject &t_project);

    /// Updates the UI for all measures.
    /// For all measures in the "bclMeasures" list which have changed relative to the version
    /// in the project, update the project to the new version.
    ///
    /// Also checks installed measures and updates the project's version of them if 
    /// appropriate. 
    /// 
    /// Does not ask for user approval
    void updateBCLMeasures(analysisdriver::SimpleProject &t_project);

    /// Checks a BCL measure for updates, returns true if updated
    bool checkForUpdates(BCLMeasure& measure, bool force=false);

    /// Downloads updated versions of all BCL measures
    void downloadBCLMeasures();

    void addMeasure();

    void duplicateSelectedMeasure();

  signals:
    void newMeasure(BCLMeasure newMeasure);

  private:
    REGISTER_LOGGER("openstudio.MeasureManager");

    void updateMeasuresLists(bool updateUserMeasures);

    BaseApp *m_app;
    std::map<UUID,BCLMeasure> m_managedMeasures;
    std::map<UUID,BCLMeasure> m_openstudioMeasures;
    std::map<UUID,BCLMeasure> m_myMeasures;
    std::map<UUID,BCLMeasure> m_bclMeasures;
    QSharedPointer<ruleset::RubyUserScriptInfoGetter> m_infoGetter;
    QSharedPointer<LocalLibraryController> m_libraryController;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP
