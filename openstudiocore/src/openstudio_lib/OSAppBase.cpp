/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <openstudio_lib/OSAppBase.hpp>
#include <analysisdriver/SimpleProject.hpp>
#include "MainRightColumnController.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "OSDocument.hpp"
#include "MainWindow.hpp"
#include <utilities/bcl/LocalBCL.hpp>
#include <QMessageBox>
#include <QDir>

namespace openstudio {

OSAppBase::OSAppBase( int & argc, char ** argv, const QSharedPointer<MeasureManager> &t_measureManager )
  : QApplication(argc, argv), m_measureManager(t_measureManager)
{
  LOG(Debug, "Measures dir: " << openstudio::toString(BCLMeasure::userMeasuresDir()));
  if (!QDir().exists(toQString(BCLMeasure::userMeasuresDir()))){
    BCLMeasure::setUserMeasuresDir(BCLMeasure::userMeasuresDir());
  }

  m_measureManager->updateMeasuresLists();
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
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->project();
  } else {
    return boost::optional<openstudio::analysisdriver::SimpleProject>();
  }
}

QWidget *OSAppBase::mainWidget()
{
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->mainWindow();
  } else {
    return 0;
  }
}

boost::optional<openstudio::model::Model> OSAppBase::currentModel()
{
  boost::shared_ptr<OSDocument> document = currentDocument();
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
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    boost::shared_ptr<MainRightColumnController> mainRightColumnController = document->mainRightColumnController();

    if (mainRightColumnController)
    {
      if (measureManager().isMeasureSelected()){
        if(document->mainTabId() == openstudio::OSDocument::SITE){
          //if(document->m_locationTabController->mainContentWidget()->currentId() == openstudio::LocationTabController::UTILITY_BILLS){
          //  // only 1 bill per fuel type allowed
          //  return;
          //}
        }
        mainRightColumnController->measuresLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(true);
      }else{
        mainRightColumnController->measuresLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(false);
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
  boost::shared_ptr<OSDocument> document = currentDocument();

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
  boost::shared_ptr<OSDocument> document = currentDocument();

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
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    document->openMeasuresBclDlg();
  }
}


void OSAppBase::chooseHorizontalEditTab()
{
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    document->mainRightColumnController()->chooseEditTab();
  }
}

QSharedPointer<EditController> OSAppBase::editController()
{
  boost::shared_ptr<OSDocument> document = currentDocument();

  if (document)
  {
    return document->mainRightColumnController()->measuresEditController();
  } else {
    return QSharedPointer<EditController>();
  }
}

} // openstudio

