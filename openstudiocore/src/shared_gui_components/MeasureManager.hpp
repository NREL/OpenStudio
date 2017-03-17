/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/UUID.hpp"
#include "../model/Model.hpp"
#include "../measure/OSArgument.hpp"
#include <vector>
#include <map>
#include <QSharedPointer>
#include <QPointer>
#include <QApplication>
#include <QUrl>
#include <QMutex>
class QEvent;
class QNetworkAccessManager;

namespace Json{
  class Value;
}

namespace openstudio {

class BaseApp;
class BCLMeasure;

namespace osversion {
  class VersionTranslator;
}

namespace measure {
  class OSArgument;
  class OSArgumentType;
}
class LocalLibraryController;

/***
* MeasureManager provides a single interface for finding measures on the user's computer in a variety of places.  
*
* It also manages checking if a newer version of a given measure is available on the user's computer and updating
* the current project to use the updated measure.  Measures directories are searched in the following order 
* 1) my measures, 3) bcl measures.  This supports the following use cases:
*
* A user has started a measure in their my measures directory, then uploaded to BCL, then downloaded that measure. In this case
* a measure with the same uid will exist in both my measures and the bcl measures.  The my measures version is preferred.
* 
* A user has downloaded a measure from the bcl to their local bcl and manually copied the directory to my measures.  
* This use case is not supported but in this case the my measures version is preferred.
*
* A user has manually downloaded a measure from the bcl to their my measures directory as well as downloaded it to their local bcl.  
* In this case the my measures version is preferred.
*
* A user has manually copied a measure in their my measures directory to their my measures directory.  
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
    MeasureManager(BaseApp *t_app);

    virtual ~MeasureManager() {}

    QUrl url() const;

    void setUrl(const QUrl& url);

    void waitForStarted(int msec=10000);

    void setLibraryController(const QSharedPointer<LocalLibraryController> &t_libraryController);

    //// Get the temp model path
    openstudio::path tempModelPath() const;

    //// Saves the current model to a temp location, used when computing arguments
    void saveTempModel(const path& tempDir);

    //// Measures downloaded from the BCL.
    std::vector<BCLMeasure> bclMeasures() const;

    //// Measures saved in the user's home directory.
    std::vector<BCLMeasure> myMeasures() const;

    //// Get combined list of measures without duplicates, uses same logic as getMeasure.
    std::vector<BCLMeasure> combinedMeasures() const;

    //// Retrieve a measure from combinedMeasures by id.
    boost::optional<BCLMeasure> getMeasure(const UUID & id);
    
    //// Retrieve a measure from combinedMeasures by id then calls updateMeasure, returns measure in the project.
    //// Throws if measure cannot be found by id or if updateMeasure fails.
    //// If the measure already exists in the project the user is prompted if they want to replace it or not. 
    BCLMeasure insertReplaceMeasure(const UUID &t_id);

    //// Updates an individual measure in the project with a measure outside the project, computes arguments on the external measure before copying to project. 
    //// result.first is true if the update succeeded and arguments were computed for current model, result.second is the new measure dir name.
    //// result.first is false if the update failed or arguments were not computed for current model, result.second is error message.
    std::pair<bool,std::string> updateMeasure(const BCLMeasure &t_measure);

    //// Updates the given set of measures in the current project. Does not ask for user approval. Approval is assumed
    //// when this method is called.
    void updateMeasures(const std::vector<BCLMeasure>& newMeasures, bool t_showMessage=true);

    //// Get arguments for given measure using current model
    //// Will throw if arguments cannot be computed.
    std::vector<measure::OSArgument> getArguments(const BCLMeasure &t_measure);

    std::string suggestMeasureName(const BCLMeasure &t_measure);

    bool isMeasureSelected();

    bool reset();

  public slots:

    /// Update the UI display for all measures. Does recompute the measure's XML.
    /// Does not update the measures in the project at all
    void updateMeasuresLists();

    ///// Updates the UI for all measures.
    ///// For all measures in the "myMeasures" list which have changed relative to the version
    ///// in the project, update the project to the new version
    ///// 
    ///// Does not ask for user approval
    //void updateMyMeasures(analysisdriver::SimpleProject &t_project);

    ///// Updates the UI for all measures.
    ///// For all measures in the "bclMeasures" list which have changed relative to the version
    ///// in the project, update the project to the new version.
    /////
    ///// Also checks installed measures and updates the project's version of them if 
    ///// appropriate. 
    ///// 
    ///// Does not ask for user approval
    //void updateBCLMeasures(analysisdriver::SimpleProject &t_project);

    /// Downloads updated versions of all BCL measures
    void downloadBCLMeasures();

    void addMeasure();

    void duplicateSelectedMeasure();

  signals:

    void newMeasure(BCLMeasure newMeasure);

  private:
    REGISTER_LOGGER("openstudio.MeasureManager");

    void updateMeasuresLists(bool updateUserMeasures);

    bool checkForLocalBCLUpdates();

    bool checkForUpdates(const openstudio::path& measureDir, bool force=false);

    boost::optional<measure::OSArgument> getArgument(const measure::OSArgumentType& type, const Json::Value& jsonArgument);

    BaseApp *m_app;
    openstudio::path m_tempModelPath;
    std::map<UUID,BCLMeasure> m_myMeasures;
    std::map<UUID,BCLMeasure> m_bclMeasures;
    std::map<openstudio::path, std::vector<measure::OSArgument> > m_measureArguments;
    QUrl m_url;
    QSharedPointer<LocalLibraryController> m_libraryController;
    QNetworkAccessManager* m_networkAccessManager;
    bool m_started;
    QMutex m_mutex;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_MEASUREMANAGER_HPP
