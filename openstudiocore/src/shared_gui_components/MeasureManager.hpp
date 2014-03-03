/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_MEASUREMANAGER_HPP
#define OPENSTUDIO_MEASUREMANAGER_HPP

#include "LocalLibraryController.hpp"
#include <analysisdriver/SimpleProject.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>
#include <ruleset/RubyUserScriptArgumentGetter.hpp>
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

#if defined(openstudio_lib_EXPORTS) || defined(COMPILING_FROM_OSAPP)
#include <openstudio_lib/OpenStudioAPI.hpp>
class OPENSTUDIO_API MeasureManager : public QObject
#else
class MeasureManager : public QObject
#endif
{
  Q_OBJECT;

  public:
    MeasureManager(const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter, BaseApp *t_app);

    virtual ~MeasureManager() {}

    // Measures downloaded from the BCL and shipped with the Pat application.
    std::vector<BCLMeasure> bclMeasures();

    // Measures saved in the user's home directory.
    std::vector<BCLMeasure> myMeasures();

    // Retrieve measures from both bclMeasures and myMeasures by id.
    boost::optional<BCLMeasure> getMeasure(const UUID & id);

    /// Updates an individial measure. Does not ask for user approval, approval is assumed.
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

  public slots:
    /// Update the UI display for all measures. Does not re-get the arguments nor
    /// update the measures in the project at all
    void updateMeasuresLists();

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
    std::map<UUID,BCLMeasure> m_bclMeasures;
    std::map<UUID,BCLMeasure> m_myMeasures;
    QSharedPointer<ruleset::RubyUserScriptArgumentGetter> m_argumentGetter;
    QSharedPointer<LocalLibraryController> m_libraryController;
};


} // openstudio

#endif // OPENSTUDIO_MEASUREMANAGER_HPP
