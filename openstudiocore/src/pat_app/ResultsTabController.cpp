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

#include "ResultsTabController.hpp"

#include "CloudMonitor.hpp"
#include "PatApp.hpp"
#include "ResultsView.hpp"

#include "../analysis/DataPoint.hpp"

#include "../analysisdriver/CloudAnalysisDriver.hpp"
#include "../analysisdriver/CloudAnalysisDriver_Impl.hpp"

#include "../model/UtilityBill.hpp"

#include "../runmanager/lib/FileInfo.hpp"
#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FileReference.hpp"

#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QRegExp>

namespace openstudio {

namespace pat {

ResultsTabController::ResultsTabController()
  : QObject()
{
  resultsView = new ResultsView();

  bool test = false;

  test = connect(resultsView, SIGNAL(openButtonClicked(bool)), this, SLOT(onOpenButtonClicked()));
  OS_ASSERT(test);

  test = connect(resultsView, SIGNAL(openDirButtonClicked(bool)), this, SLOT(openDirectory()));
  OS_ASSERT(test); 
  
  test = connect(resultsView, SIGNAL(downloadResultsButtonClicked(bool)), this, SLOT(downloadResults()));
  OS_ASSERT(test);

  test = connect(resultsView, SIGNAL(viewSelected(int)), this, SLOT(selectView(int)));
  OS_ASSERT(test);

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){
    // ensure that baseline exists
    analysis::DataPoint baselineDataPoint = project->baselineDataPoint();

    analysis::Analysis analysis = project->analysis();

    m_baselineDataPointResultListController = QSharedPointer<BaselineDataPointResultListController>(new BaselineDataPointResultListController(analysis));
    m_dataPointResultsListController = QSharedPointer<DataPointResultsListController>(new DataPointResultsListController(analysis));
    m_dataPointResultItemDelegate = QSharedPointer<DataPointResultItemDelegate>(new DataPointResultItemDelegate());
    m_dataPointCalibrationListController = QSharedPointer<DataPointCalibrationListController>(new DataPointCalibrationListController(analysis));
    m_dataPointCalibrationItemDelegate = QSharedPointer<DataPointCalibrationItemDelegate>(new DataPointCalibrationItemDelegate(resultsView->calibrationMaxNMBE(), resultsView->calibrationMaxCVRMSE()));

    // can only select one item between both lists
    m_dataPointResultsListController->setSelectionController(m_baselineDataPointResultListController->selectionController());

    bool bingo = false;

    bingo = connect(resultsView,SIGNAL(calibrationThresholdsChanged(double, double)),m_dataPointCalibrationItemDelegate.data(),SLOT(setCalibrationThresholds(double, double)));
    OS_ASSERT(bingo);

    // want to reset the list after changing the delegate
    bingo = connect(resultsView,SIGNAL(calibrationThresholdsChanged(double, double)),resultsView->dataPointCalibrationListView,SLOT(refreshAllViews()));
    OS_ASSERT(bingo);

    resultsView->baselineDataPointResultListView->setListController(m_baselineDataPointResultListController);
    resultsView->baselineDataPointResultListView->setDelegate(m_dataPointResultItemDelegate);

    resultsView->dataPointResultsListView->setListController(m_dataPointResultsListController);
    resultsView->dataPointResultsListView->setDelegate(m_dataPointResultItemDelegate);

    resultsView->dataPointCalibrationListView->setListController(m_dataPointCalibrationListController);
    resultsView->dataPointCalibrationListView->setDelegate(m_dataPointCalibrationItemDelegate);
   
    boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
    if(cloudAnalysisDriver){
      bingo = cloudAnalysisDriver->connect(SIGNAL(dataPointDetailsComplete(const openstudio::UUID&, const openstudio::UUID&)),this,SLOT(dataPointDetailsComplete(const openstudio::UUID&, const openstudio::UUID&)));
      OS_ASSERT(bingo);
    }
  }

  selectView(0);
}

ResultsTabController::~ResultsTabController()
{
  if( resultsView ) { delete resultsView; }
}

void ResultsTabController::selectView(int index)
{
  bool bingo = false;

  if (m_currentSelectionController){
    bingo = disconnect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableViewFileButton()));
    OS_ASSERT(bingo);
    bingo = disconnect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableDownloadResultsButton()));
    OS_ASSERT(bingo);
    bingo = disconnect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableOpenDirectoryButton()));
    OS_ASSERT(bingo);
  }

  if (index == 0){
    m_currentSelectionController = m_dataPointResultsListController->selectionController();
  }else if (index == 1){
    m_currentSelectionController = m_dataPointCalibrationListController->selectionController();
  }

  if (m_currentSelectionController){
    bingo = connect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableViewFileButton()));
    OS_ASSERT(bingo);
    bingo = connect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableDownloadResultsButton()));
    OS_ASSERT(bingo);
    bingo = connect(m_currentSelectionController.data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(enableOpenDirectoryButton()));
    OS_ASSERT(bingo);

    //m_currentSelectionController->unselectAllItems();
    m_currentSelectionController->emitSelectionChanged();
  }

  resultsView->selectView(index);
}

void ResultsTabController::onOpenButtonClicked()
{
  std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();

  if (!selectedItems.empty()){
    DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
    if (dataPointResultListItem){
      analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();

      openstudio::path osmPath;
      OptionalFileReference finalInputModel = dataPoint.osmInputData();
      if (finalInputModel && (finalInputModel->fileType() == FileReferenceType::OSM)) {
        osmPath = finalInputModel->path(); // official final input model
      }
      else if (boost::optional<runmanager::Job> topLevelJob = dataPoint.topLevelJob()) {
        try {
          // if this can be done while running, may get intermediate model
          runmanager::FileInfo fileInfo = topLevelJob->treeAllFiles().getLastByExtension("osm");
          osmPath = fileInfo.fullPath;

          if (!exists(osmPath)){
            // check if baseline
            boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
            if (project){
              if (dataPoint == project->baselineDataPoint()){
                osmPath = project->seed().path();
              }
            }
          }

          if (!exists(osmPath)){
            // path may be from another computer try to fix it up here
            QRegExp regex(".*dataPoint\\d*\\D(.*osm)");
            regex.setCaseSensitivity(Qt::CaseInsensitive);
            if (regex.indexIn(toQString(osmPath)) != -1) {
              QString partialPath = regex.cap(1);
              openstudio::path dir = dataPoint.directory();
              osmPath = dir / toPath(partialPath);
            }
          }

        }catch(const std::exception&){
        }

        if (!exists(osmPath)){
          // no osm found
          QMessageBox::warning(resultsView, "Unable to Launch OpenStudio", "Could not find last osm file for data point.");
          return;
        }
      }

      if (!osmPath.empty()) {

#ifdef Q_OS_MAC
        openstudio::path openstudioApp
            = openstudio::getApplicationRunDirectory() / openstudio::toPath("../../../OpenStudio.app/Contents/MacOS/OpenStudio");
#else
        openstudio::path openstudioApp
            = openstudio::getApplicationRunDirectory() / openstudio::toPath("OpenStudio");
#endif

        QStringList args;
        args.push_back(toQString(osmPath));
        args.push_back("noSavePath");

        if (QProcess::startDetached(toQString(openstudioApp), args)){
          // disable the button for now to prevent tons of instances being opened
          // will get re-enabled when clicking another data point
          resultsView->enableViewFileButton(false);
        }else{
          QMessageBox::critical(resultsView, "Unable to Launch OpenStudio", "OpenStudio was not found in the expected location:\n" + toQString(openstudioApp));
        }

      }else{
        // no final input model yet
      }
    }else{
      // wrong type of item selected
    }
  }else{
    // nothing selected
  }

}

void ResultsTabController::openDirectory()
{
  std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();

  if (!selectedItems.empty()){
    DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
    if (dataPointResultListItem){
      analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();
      openstudio::path dir = dataPoint.directory();
      QString path = QDir::toNativeSeparators(toQString(dir));
      QDesktopServices::openUrl(QUrl("file:///" + path));    }
  }

}

void ResultsTabController::downloadResults()
{
  if( resultsView ){
    std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();
    if (!selectedItems.empty()){
      DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
      if (dataPointResultListItem){
        analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();
        
        boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
        if(project){
          boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
          if(cloudAnalysisDriver){

            if (dataPoint.complete() || (cloudAnalysisDriver->status() == analysisdriver::AnalysisStatus::Running)){

              bool sameSession = cloudAnalysisDriver->inSession(dataPoint);
              if (sameSession){
                cloudAnalysisDriver->requestDownloadDetailedResults(dataPoint);
              }else{
                // DLM: should not get here
                QMessageBox::information(resultsView, "Results Unavailable", "Cannot download results from a previous cloud session.");
                resultsView->enableDownloadResultsButton(RESULTS_UNAVAILABLE);
              }

            }else{
              // request detailed run type
              dataPoint.setRunType(analysis::DataPointRunType::CloudDetailed);
            }

            enableDownloadResultsButton();
          }
        }
      }
    }
  }
}

void ResultsTabController::dataPointDetailsComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint)
{
  if( resultsView ){
    std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();
    if (!selectedItems.empty()){
      DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
      if (dataPointResultListItem){
        analysis::DataPoint selectedDataPoint = dataPointResultListItem->dataPoint();
        if (selectedDataPoint.uuid() == dataPoint){
          enableDownloadResultsButton();
          enableViewFileButton();
          enableOpenDirectoryButton();
        }
      }
    }
  }
}

void ResultsTabController::enableDownloadResultsButton()
{
  if( resultsView ){

    // check cloud status
    QSharedPointer<CloudMonitor> cloudMonitor = PatApp::instance()->cloudMonitor();
    CloudStatus cloudStatus = cloudMonitor->status(); // CLOUD_STARTING, CLOUD_RUNNING, CLOUD_STOPPING, CLOUD_STOPPED, CLOUD_ERROR 
    if (cloudStatus != CLOUD_RUNNING){
      resultsView->enableDownloadResultsButton(LOCAL_MODE);
      return;
    }

    // cloud is running

    // find datapoint
    boost::optional<analysis::DataPoint> dataPoint;
    std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();
    if (!selectedItems.empty()){
      DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
      if (dataPointResultListItem){
        dataPoint = dataPointResultListItem->dataPoint();
      }
    }
    if (!dataPoint){
      resultsView->enableDownloadResultsButton(RESULTS_DISABLED);
      return;
    }

    // data point is selected

    // check for project, this should be true
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    OS_ASSERT(project);

    // Determine if datapoint already has detailed data
    bool hasDetailedResults = !dataPoint->directory().empty();
    if (hasDetailedResults){
      resultsView->enableDownloadResultsButton(RESULTS_DOWNLOADED);
      return;
    }

    // data point does not have detailed results

    // check if data point is selected to run
    if (!dataPoint->selected()){
      // not selected to run
      resultsView->enableDownloadResultsButton(RESULTS_DISABLED);
      return;
    }

    // selected to run

    // check if data point has already requested detailed results
    if (dataPoint->runType() == analysis::DataPointRunType::CloudDetailed){
      // already requested detailed results
      resultsView->enableDownloadResultsButton(RUNNING_DETAILED);
      return;
    }

    // data point has not requested detailed results

    // Determine if datapoint is already complete
    if (!dataPoint->complete()){
      // not yet complete, this will just set run type to cloud detailed
      resultsView->enableDownloadResultsButton(RUNNING_SLIM);
      return;
    }

    // data point is complete

    // get cloud analysis driver, this should be true
    boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
    OS_ASSERT(cloudAnalysisDriver);

    // check if data point ran in the current cloud session
    if (!cloudAnalysisDriver->inSession(*dataPoint)){
      // ran in previous session, can't download
      resultsView->enableDownloadResultsButton(RESULTS_UNAVAILABLE);
      return;
    }

    // ran in same session

    // already ran but no detailed results requested
    resultsView->enableDownloadResultsButton(RESULTS_AVAILABLE);
  }
}

void ResultsTabController::enableViewFileButton()
{
  if( resultsView ) {

    bool enabled = false;

    std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();
    if (!selectedItems.empty()){
      DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
      if (dataPointResultListItem){
        analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();

        // Determine if datapoint has detailed data
        if(dataPoint.complete() && !dataPoint.directory().empty()){
          enabled = true;
        }
      }
    }

    resultsView->enableViewFileButton(enabled);
  }
}

void ResultsTabController::enableOpenDirectoryButton()
{
  if( resultsView ) {

    bool enabled = false;

    std::vector<QPointer<OSListItem> > selectedItems = m_currentSelectionController->selectedItems();
    if (!selectedItems.empty()){
      DataPointResultListItem* dataPointResultListItem = dynamic_cast<DataPointResultListItem*>(selectedItems[0].data());
      if (dataPointResultListItem){
        analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();

        // Determine if datapoint has already has detailed data
        bool hasDetailedResults = !dataPoint.directory().empty();

        if(hasDetailedResults){
          enabled = true;
        }
      }
    }
    resultsView->enableOpenDirectoryButton(enabled);
  }
}

DataPointResultListItem::DataPointResultListItem(const openstudio::analysis::DataPoint& dataPoint,
                                                 const openstudio::analysis::DataPoint& baselineDataPoint,
                                                 bool alternateRow)
  : m_dataPoint(dataPoint), m_baselineDataPoint(baselineDataPoint), m_alternateRow(alternateRow)
{
}

openstudio::analysis::DataPoint DataPointResultListItem::dataPoint() const
{
  return m_dataPoint;
}

openstudio::analysis::DataPoint DataPointResultListItem::baselineDataPoint() const
{
  return m_baselineDataPoint;
}

bool DataPointResultListItem::alternateRow() const
{
  return m_alternateRow;
}

QWidget * DataPointResultItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QSharedPointer<DataPointResultListItem> dataPointResultListItem = dataSource.dynamicCast<DataPointResultListItem>();
  openstudio::analysis::DataPoint dataPoint = dataPointResultListItem->dataPoint();
  openstudio::analysis::DataPoint baselineDataPoint = dataPointResultListItem->baselineDataPoint();
  bool alternateRow = dataPointResultListItem->alternateRow();

  auto result = new DataPointResultsView(dataPoint, baselineDataPoint, alternateRow);
  result->setHasEmphasis(dataPointResultListItem->isSelected());

  bool test = connect(result,SIGNAL(clicked()),dataPointResultListItem.data(),SLOT(toggleSelected()));
  OS_ASSERT(test);

  test = connect(dataPointResultListItem.data(),SIGNAL(selectedChanged(bool)),result,SLOT(setHasEmphasis(bool)));
  OS_ASSERT(test);

  return result;
}

DataPointCalibrationItemDelegate::DataPointCalibrationItemDelegate(double maxNMBE, double maxCVRMSE)
 : m_calibrationMaxNMBE(maxNMBE), m_calibrationMaxCVRMSE(maxCVRMSE)
{}

QWidget * DataPointCalibrationItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QSharedPointer<DataPointResultListItem> dataPointCalibrationListItem = dataSource.dynamicCast<DataPointResultListItem>();
  OS_ASSERT(dataPointCalibrationListItem);

  openstudio::analysis::DataPoint dataPoint = dataPointCalibrationListItem->dataPoint();
  openstudio::analysis::DataPoint baselineDataPoint = dataPointCalibrationListItem->baselineDataPoint();
  bool alternateRow = dataPointCalibrationListItem->alternateRow();

  auto result = new DataPointCalibrationView(dataPoint, baselineDataPoint, alternateRow, m_calibrationMaxNMBE, m_calibrationMaxCVRMSE);
  result->setHasEmphasis(dataPointCalibrationListItem->isSelected());

  bool test = connect(result,SIGNAL(clicked()),dataPointCalibrationListItem.data(),SLOT(toggleSelected()));
  OS_ASSERT(test);

  test = connect(dataPointCalibrationListItem.data(),SIGNAL(selectedChanged(bool)),result,SLOT(setHasEmphasis(bool)));
  OS_ASSERT(test);

  return result;
}

void DataPointCalibrationItemDelegate::setCalibrationThresholds(double maxNMBE, double maxCVRMSE)
{
  m_calibrationMaxNMBE = maxNMBE;
  m_calibrationMaxCVRMSE = maxCVRMSE;
}

BaselineDataPointResultListController::BaselineDataPointResultListController(const openstudio::analysis::Analysis& analysis)
  : m_analysis(analysis)
{
}

QSharedPointer<OSListItem> BaselineDataPointResultListController::itemAt(int i)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(!project){
    return QSharedPointer<OSListItem>();
  }

  analysis::DataPoint baselineDataPoint = project.get().baselineDataPoint();

  QSharedPointer<OSListItem> item = QSharedPointer<OSListItem>(new DataPointResultListItem(baselineDataPoint, baselineDataPoint, false));

  item->setController(this);

  return item;
}

int BaselineDataPointResultListController::count()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(!project){
    return 0;
  }
  return 1;
}

DataPointResultsListController::DataPointResultsListController(const openstudio::analysis::Analysis& analysis)
  : m_analysis(analysis)
{
}

QSharedPointer<OSListItem> DataPointResultsListController::itemAt(int i)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(project){
    analysis::DataPoint baselineDataPoint = project.get().baselineDataPoint();
    std::vector<openstudio::analysis::DataPoint> dataPoints = this->dataPoints();
    if (i >= 0 && i < (int)dataPoints.size()){
      bool alternateRow = ((i % 2) == 1);
      QSharedPointer<OSListItem> item = QSharedPointer<OSListItem>(new DataPointResultListItem(dataPoints[i], baselineDataPoint, alternateRow));
      item->setController(this);
      return item;
    }
  }
  return QSharedPointer<OSListItem>();
}

int DataPointResultsListController::count()
{
  return (int)this->dataPoints().size();
}

std::vector<openstudio::analysis::DataPoint> DataPointResultsListController::dataPoints()
{
  std::vector<openstudio::analysis::DataPoint> result;

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(!project){
    return result;
  }

  analysis::DataPoint baselineDataPoint = project.get().baselineDataPoint();
  for (const analysis::DataPoint& dataPoint : m_analysis.dataPoints()) {
    if (dataPoint.uuid() != baselineDataPoint.uuid()){
      result.push_back(dataPoint);
    }
  }

  return result;
}

DataPointCalibrationListController::DataPointCalibrationListController(const openstudio::analysis::Analysis& analysis)
  : m_analysis(analysis)
{
}

QSharedPointer<OSListItem> DataPointCalibrationListController::itemAt(int i)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(project){
    analysis::DataPoint baselineDataPoint = project.get().baselineDataPoint();
    std::vector<openstudio::analysis::DataPoint> dataPoints = this->dataPoints();
    if (i >= 0 && i < (int)dataPoints.size()){
      bool alternateRow = ((i % 2) == 1);
      QSharedPointer<OSListItem> item = QSharedPointer<OSListItem>(new DataPointResultListItem(dataPoints[i], baselineDataPoint, alternateRow));
      item->setController(this);
      return item;
    }
  }
  return QSharedPointer<OSListItem>();
}

int DataPointCalibrationListController::count()
{
  return (int)this->dataPoints().size();
}

std::vector<openstudio::analysis::DataPoint> DataPointCalibrationListController::dataPoints()
{
  std::vector<openstudio::analysis::DataPoint> result;

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if(!project){
    return result;
  }

  analysis::DataPoint baselineDataPoint = project.get().baselineDataPoint();
  for (const analysis::DataPoint& dataPoint : m_analysis.dataPoints()) {
    result.push_back(dataPoint);
  }

  return result;
}

}

} // openstudio
