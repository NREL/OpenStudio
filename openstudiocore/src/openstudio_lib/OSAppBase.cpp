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

#include <QDir>
#include <QMessageBox>

namespace openstudio {

OSAppBase::OSAppBase( int & argc, char ** argv, const QSharedPointer<MeasureManager> &t_measureManager )
  : QApplication(argc, argv), m_measureManager(t_measureManager)
{
  LOG(Debug, "Measures dir: " << openstudio::toString(BCLMeasure::userMeasuresDir()));
  if (!QDir().exists(toQString(BCLMeasure::userMeasuresDir()))){
    BCLMeasure::setUserMeasuresDir(BCLMeasure::userMeasuresDir());
  }

  m_measureManager->updateMeasuresLists();

  m_waitDialog = boost::shared_ptr<WaitDialog>(new WaitDialog("Loading Model","Loading Model"));
}

OSAppBase::~OSAppBase()
{
}

OSAppBase * OSAppBase::instance()
{
  return qobject_cast<OSAppBase *>(QApplication::instance());
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
    return 0;
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

