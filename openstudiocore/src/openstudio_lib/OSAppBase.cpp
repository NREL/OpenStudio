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

#include "OSAppBase.hpp"

#include "ApplyMeasureNowDialog.hpp"
#include "MainRightColumnController.hpp"
#include "MainWindow.hpp"
#include "OSDocument.hpp"

#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/WaitDialog.hpp"

#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include <QDir>
#include <QEvent>
#include <QMessageBox>
#include <QTimer>

namespace openstudio {

OSAppBase::OSAppBase( int & argc, char ** argv, const QSharedPointer<MeasureManager> &t_measureManager )
  : QApplication(argc, argv), m_measureManager(t_measureManager)
{
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();

  if (isNetworkPath(userMeasuresDir) && !isNetworkPathAvailable(userMeasuresDir)) {
    QTimer::singleShot(0, this, SLOT(showMeasureUpdateDlg()));
  }
  else {
    LOG(Debug, "Measures dir: " << openstudio::toString(userMeasuresDir));
    if (!QDir().exists(toQString(userMeasuresDir))){
      BCLMeasure::setUserMeasuresDir(userMeasuresDir);
    }
  }

  // DLM: todo rethink this
  //m_measureManager->updateMeasuresLists();

  m_waitDialog = boost::shared_ptr<WaitDialog>(new WaitDialog("Loading Model","Loading Model"));
}

OSAppBase::~OSAppBase()
{
}

bool OSAppBase::notify(QObject * receiver, QEvent * e)
{
  return QApplication::notify(receiver, e);
}

bool OSAppBase::event(QEvent * e)
{
  return QApplication::event(e);
}

void OSAppBase::childEvent(QChildEvent * e)
{
  QApplication::childEvent(e);
}

OSAppBase * OSAppBase::instance()
{
  return qobject_cast<OSAppBase *>(QApplication::instance());
}

void OSAppBase::showMeasureUpdateDlg()
{
  QMessageBox::information(this->mainWidget(), "Cannot Update User Measures", "Your My Measures Directory appears to be on a network drive that is not currently available.\nYou can change your specified My Measures Directory using 'Preferences->Change My Measures Directory'.", QMessageBox::Ok);
}

//boost::optional<openstudio::analysisdriver::SimpleProject> OSAppBase::project()
//{
//  std::shared_ptr<OSDocument> document = currentDocument();
//
//  if (document)
//  {
//    return document->project();
//  } else {
//    return boost::optional<openstudio::analysisdriver::SimpleProject>();
//  }
//}

void OSAppBase::addWorkspaceObject(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid) {
  // Emit QT Signal
  emit workspaceObjectAdded(workspaceObject, type, uuid);
}

void OSAppBase::addWorkspaceObjectPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid) {
  // Emit QT Signal
  emit workspaceObjectAddedPtr(wPtr, type, uuid);
}

void OSAppBase::removeWorkspaceObject(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid) {
  emit workspaceObjectRemoved(workspaceObject, type, uuid);
}

void OSAppBase::removeWorkspaceObjectPtr(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid ) {
  emit workspaceObjectRemovedPtr(wPtr, type, uuid);
}

QWidget *OSAppBase::mainWidget()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->mainWindow();
  } else {
    return nullptr;
  }
}

boost::optional<openstudio::path> OSAppBase::tempDir()
{
  std::shared_ptr<OSDocument> document = currentDocument();
  if (document)
  {
    return toPath(document->modelTempDir());
  }
  return boost::none;
}


boost::optional<openstudio::model::Model> OSAppBase::currentModel()
{
  std::shared_ptr<OSDocument> document = currentDocument();
  if (document)
  {
    return document->model();
  } else {
    return boost::optional<openstudio::model::Model>();
  }
}

//boost::optional<openstudio::Workspace> OSAppBase::currentWorkspace()
//{
//  std::shared_ptr<OSDocument> document = currentDocument();
//  if (document)
//  {
//    return document->workspace();
//  } else {
//    return boost::none;
//  }
//}

MeasureManager &OSAppBase::measureManager()
{
  return *m_measureManager;
}

void OSAppBase::updateSelectedMeasureState()
{
  // DLM: this slot seems out of place here, seems like the connection from the measure list to enabling duplicate buttons, etc
  // should be tighter
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    std::shared_ptr<MainRightColumnController> mainRightColumnController = document->mainRightColumnController();

    if (mainRightColumnController)
    {
      if (measureManager().isMeasureSelected()){
        // DLM: this logic is why we cannot remove m_applyMeasureNowDialog as member of OSDocument
        if( document->m_applyMeasureNowDialog && document->m_applyMeasureNowDialog->isVisible()) {
          document->m_applyMeasureNowDialog->displayMeasure();
          document->m_applyMeasureNowDialog->m_localLibraryController->localLibraryView->duplicateMeasureButton->setEnabled(true);
        } else {
          mainRightColumnController->measuresLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(true);
        }
      }else{
        if( document->m_applyMeasureNowDialog && document->m_applyMeasureNowDialog->isVisible()) {
          document->m_applyMeasureNowDialog->m_localLibraryController->localLibraryView->duplicateMeasureButton->setEnabled(false);
        } else {
          mainRightColumnController->measuresLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(false);
        }
      }
    }
  }
}

void OSAppBase::addMeasure()
{
  measureManager().addMeasure();
}

void OSAppBase::duplicateSelectedMeasure()
{
  measureManager().duplicateSelectedMeasure();
}

void OSAppBase::updateMyMeasures()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    //boost::optional<analysisdriver::SimpleProject> project = document->project();
    //if (project)
    //{
    //  mainWidget()->setEnabled(false);
    //  measureManager().updateMyMeasures(*project);
    //  mainWidget()->setEnabled(true);
    //} else {
      LOG(Error, "Unable to update measures, there is no project set...");
    //}
  }
}

void OSAppBase::updateBCLMeasures()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    //boost::optional<analysisdriver::SimpleProject> project = document->project();
    //if (project)
    //{
    //  mainWidget()->setEnabled(false);
    //  measureManager().updateBCLMeasures(*project);
    //  mainWidget()->setEnabled(true);
    //} else {
      LOG(Error, "Unable to update measures, there is no project set...");
    //}
  }

}

void OSAppBase::downloadUpdatedBCLMeasures()
{
  measureManager().downloadBCLMeasures();
}

void OSAppBase::openBclDlg()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    document->openMeasuresBclDlg();
  }
}

void OSAppBase::chooseHorizontalEditTab()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    document->mainRightColumnController()->chooseEditTab();
  }
}

QSharedPointer<EditController> OSAppBase::editController()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->mainRightColumnController()->measuresEditController();
  } else {
    return QSharedPointer<EditController>();
  }
}

} // openstudio

