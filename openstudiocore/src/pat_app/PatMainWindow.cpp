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
  qobject_cast<PatApp *>(QApplication::instance())->quit();
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

