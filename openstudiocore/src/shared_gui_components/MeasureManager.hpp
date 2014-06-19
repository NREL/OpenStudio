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

#ifndef SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP
#define SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP

#include "LocalLibraryController.hpp"
#include "../analysisdriver/SimpleProject.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/UUID.hpp"
#include "../ruleset/RubyUserScriptArgumentGetter.hpp"
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
    // Constructor taking a RubyUserScriptArgumentGetter
    MeasureManager(const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter, BaseApp *t_app);

    virtual ~MeasureManager() {}

    // Returns true if given measure is a pat application measure
    bool isPatApplicationMeasure(const UUID & id) const;

    // Measures shipped with the PAT application.
    std::vector<BCLMeasure> patApplicationMeasures() const;

    // Measures downloaded from the BCL.
    std::vector<BCLMeasure> bclMeasures() const;

    // Measures saved in the user's home directory.
    std::vector<BCLMeasure> myMeasures() const;

    // Get combined list of measures without duplicates, uses same logic as getMeasure
    // If includePatApplicationMeasures is false then pat application measures will not be included, otherwise they will be.
    std::vector<BCLMeasure> combinedMeasures(bool includePatApplicationMeasures=true) const;

    // Retrieve a measure from patApplicationMeasures, myMeasures, and bclMeasures by id.
    boost::optional<BCLMeasure> getMeasure(const UUID & id);

    /// Updates an individual measure. Does not ask for user approval, approval is assumed.
    /// \returns true if the update succeeded.
    std::pair<bool,std::string> updateMeasure(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure);

    // insert / replace a measure by UUID. If the measure already exists in the project
    // the user is prompted as to how to deal with it. OSArguments are loaded as needed
    BCLMeasure insertReplaceMeasure(analysisdriver::SimpleProject &t_project, const UUID &t_id);

    // Updates the given set of measures in the current project. Does not ask for user approval. Approval is assumed
    // when this method is called.
    void updateMeasures(analysisdriver::SimpleProject &t_project, const std::vector<BCLMeasure> &t_newMeasures, bool t_showMessage=true);

    void setLibraryController(const QSharedPointer<LocalLibraryController> &t_libraryController);

    std::vector<ruleset::OSArgument> getArguments(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure);

    std::string suggestMeasureGroupName(const BCLMeasure &t_measure);

    std::string suggestMeasureName(const BCLMeasure &t_measure, bool t_fixed);

    bool isMeasureSelected();

    QSharedPointer<ruleset::RubyUserScriptArgumentGetter> argumentGetter() const;

  public slots:
    /// Update the UI display for all measures. Does not re-get the arguments nor
    /// update the measures in the project at all
    void updateMeasuresLists();

    /// For all measures in the "patApplicationMeasures" list which have changed relative to the version
    /// in the project, update the project to the new version
    /// 
    /// Does not ask for user approval
    void updatePatApplicationMeasures(analysisdriver::SimpleProject &t_project);

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

    /// Downloads updated versions of all BCL measures
    void downloadBCLMeasures();

    void addMeasure();

    void duplicateSelectedMeasure();

  signals:
    void newMeasure(BCLMeasure newMeasure);

  private:
    REGISTER_LOGGER("openstudio.MeasureManager");

    BaseApp *m_app;
    std::map<UUID,BCLMeasure> m_patApplicationMeasures;
    std::map<UUID,BCLMeasure> m_myMeasures;
    std::map<UUID,BCLMeasure> m_bclMeasures;
    QSharedPointer<ruleset::RubyUserScriptArgumentGetter> m_argumentGetter;
    QSharedPointer<LocalLibraryController> m_libraryController;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP
