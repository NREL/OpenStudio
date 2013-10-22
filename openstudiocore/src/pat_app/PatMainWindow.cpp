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

#include <pat_app/PatMainWindow.hpp>

#include <pat_app/CloudMonitor.hpp>
#include <pat_app/HorizontalTabWidget.hpp>
#include <pat_app/PatApp.hpp>
#include <pat_app/PatMainMenu.hpp>
#include <pat_app/PatVerticalTabWidget.hpp>

#include <utilities/core/Assert.hpp>

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QColumnView>
#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsView>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

namespace openstudio{

namespace pat {

PatMainWindow::PatMainWindow(QWidget *parent) : 
  QMainWindow(parent)
{
  setMinimumSize(950,640);
  setAcceptDrops(true);

  readSettings();

  this->statusBar()->show();

#ifdef Q_OS_LINUX
  QIcon icon = QIcon(":/images/pa_16.png");
  icon.addPixmap(QPixmap(":/images/pa_32.png"));
  icon.addPixmap(QPixmap(":/images/pa_48.png"));
  icon.addPixmap(QPixmap(":/images/pa_64.png"));
  icon.addPixmap(QPixmap(":/images/pa_128.png"));
  icon.addPixmap(QPixmap(":/images/pa_256.png"));
  setWindowIcon(icon);
#endif

  setWindowTitle(QCoreApplication::applicationName());

  setObjectName("MainWindow");
  setStyleSheet("QWidget#MainWindow { background-color: #2C3233; }");

  m_mainSplitter = new QSplitter(Qt::Horizontal); 

  verticalTabWidget = new PatVerticalTabWidget();

  bool isConnected = false;

  m_mainSplitter->addWidget(verticalTabWidget);

  m_mainRightColumnContainer = new QStackedWidget();
  m_mainRightColumnContainer->setMinimumWidth(235);
  m_mainSplitter->addWidget(m_mainRightColumnContainer);

  m_mainSplitter->setStretchFactor(0,10000);

  setCentralWidget(m_mainSplitter);  

  mainMenu = new PatMainMenu();

  this->setMenuBar(mainMenu);

  isConnected = connect(mainMenu, SIGNAL(newClicked()), this, SIGNAL(newClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(loadFileClicked()), this, SIGNAL(loadFileClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(saveFileClicked()), this, SIGNAL(saveFileClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(saveAsFileClicked()), this, SIGNAL(saveAsFileClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(clearAllResultsClicked()), this, SIGNAL(clearAllResultsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(exportXmlClicked()), this, SIGNAL(exportXmlClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(scanForToolsClicked()),this,SIGNAL(scanForToolsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(showToolsClicked()),this,SIGNAL(showToolsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(exitClicked()),this,SIGNAL(exitClicked()));
  OS_ASSERT(isConnected);
  
  isConnected = connect(mainMenu, SIGNAL(changeMeasuresClicked()),this,SIGNAL(changeMeasuresClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(openBclDlgClicked()), this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(openCloudDlgClicked()), this, SIGNAL(openCloudDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(openMonitorUseDlgClicked()), this, SIGNAL(openMonitorUseDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(helpClicked()),this,SIGNAL(helpClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainMenu, SIGNAL(aboutClicked()),this,SIGNAL(aboutClicked()));
  OS_ASSERT(isConnected);
}

void PatMainWindow::showRightColumn()
{
  m_mainRightColumnContainer->show();
}

void PatMainWindow::hideRightColumn()
{
  m_mainRightColumnContainer->hide();
}

QSize PatMainWindow::sizeHint() const
{
  return QSize(1024,700);
}

void PatMainWindow::setMainRightColumnView(QWidget * widget)
{
  if( QWidget * oldwidget = m_mainRightColumnContainer->currentWidget() )
  {
    m_mainRightColumnContainer->removeWidget(oldwidget);
  }

  m_mainRightColumnContainer->addWidget(widget);
}

void PatMainWindow::closeEvent(QCloseEvent *event)
{
  CloudStatus status = PatApp::instance()->cloudMonitor()->status();

  if (status == CLOUD_STARTING || status == CLOUD_STOPPING) {
    QMessageBox::warning(this, 
      "Cannot Exit PAT", 
      "PAT cannot be closed while the cloud is starting or stopping.  The current cloud operation should be completed shortly.", 
      QMessageBox::Ok);

    event->ignore();
    return;
  } else if (status == CLOUD_RUNNING) {

    // if project is running we can quit, user might want to leave cloud on
    // if project is idle we can quit, 99% sure we should turn cloud off
    // if project is starting can't quit
    // if project is stopping we can quit, 90% sure we should turn cloud off
    // if project is error we can quit, 90% sure we should turn cloud off
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    if (project && (project->status() == analysisdriver::AnalysisStatus::Starting)){
      QMessageBox::warning(this, 
        "Cannot Exit PAT", 
        "PAT cannot be closed while the remote analysis is starting.  The current cloud operation should be completed shortly.", 
        QMessageBox::Ok);

      event->ignore();
      return;
    }else{
      int result = QMessageBox::warning(this, 
                     "Close PAT?", 
                     "The cloud is currently running and charges are accruing.  Are you sure you want to exit PAT?", 
                     QMessageBox::Ok, 
                     QMessageBox::Cancel);

      if(result == QMessageBox::Cancel) {
        event->ignore();
        return;
      }
    }

  } else if (status == CLOUD_RUNNING) {

    // DLM: check if running locally?

  } else if (status == CLOUD_ERROR) {
    int result = QMessageBox::warning(this, 
                   "Close PAT?", 
                   "You are disconnected from the cloud, but it may currently be running and accruing charges.  Are you sure you want to exit PAT?", 
                   QMessageBox::Ok, 
                   QMessageBox::Cancel);

    if(result == QMessageBox::Cancel) {
      event->ignore();
      return;
    }
  }

  qobject_cast<PatApp *>(QApplication::instance())->quit(true);
  writeSettings();

  QMainWindow::closeEvent(event);
}

void PatMainWindow::dragEnterEvent(QDragEnterEvent * event)
{
  if(event->mimeData()->hasUrls())
  {
    QUrl url = event->mimeData()->urls().first();
    if (url.scheme().compare("file", Qt::CaseInsensitive) == 0 && url.toString().endsWith("project.osp")) 
    {
      event->accept();
    }
    else
    {
      event->ignore();
    }
  }
}

void PatMainWindow::dropEvent(QDropEvent * event)
{
  if (event->mimeData()->hasUrls()) {
    PatApp::instance()->userInteractiveOpenProject(event->mimeData()->urls().first().toLocalFile());
  }
}

void PatMainWindow::closeSidebar()
{
  QList<int> sizeList;

  sizeList.push_back(1);

  sizeList.push_back(0);

  m_mainSplitter->setSizes(sizeList);
}

void PatMainWindow::openSidebar()
{
  QList<int> sizeList;

  sizeList.push_back(2);

  sizeList.push_back(1);

  m_mainSplitter->setSizes(sizeList);
}

void PatMainWindow::readSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(600, 400)).toSize();
  resize(size);
  move(pos);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("state").toByteArray());
}

void PatMainWindow::writeSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("geometry", saveGeometry());
  settings.setValue("state", saveState());
}

}

} // openstudio

