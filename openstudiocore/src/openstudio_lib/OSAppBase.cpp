/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../analysisdriver/SimpleProject.hpp"

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

  m_measureManager->updateMeasuresLists();

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

boost::optional<openstudio::analysisdriver::SimpleProject> OSAppBase::project()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->project();
  } else {
    return boost::optional<openstudio::analysisdriver::SimpleProject>();
  }
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

boost::optional<openstudio::Workspace> OSAppBase::currentWorkspace()
{
  std::shared_ptr<OSDocument> document = currentDocument();
  if (document)
  {
    return document->workspace();
  } else {
    return boost::none;
  }
}

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
    boost::optional<analysisdriver::SimpleProject> project = document->project();
    if (project)
    {
      mainWidget()->setEnabled(false);
      measureManager().updateMyMeasures(*project);
      mainWidget()->setEnabled(true);
    } else {
      LOG(Error, "Unable to update measures, there is no project set...");
    }
  } 
}

void OSAppBase::updateBCLMeasures()
{
  std::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    boost::optional<analysisdriver::SimpleProject> project = document->project();
    if (project)
    {
      mainWidget()->setEnabled(false);
      measureManager().updateBCLMeasures(*project);
      mainWidget()->setEnabled(true);
    } else {
      LOG(Error, "Unable to update measures, there is no project set...");
    }
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

