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

#include "MainWindow.hpp"

#include "HorizontalTabWidget.hpp"
#include "LibraryTabWidget.hpp"
#include "LoopLibraryDialog.hpp"
#include "MainMenu.hpp"
#include "MainTabView.hpp"
#include "VerticalTabWidget.hpp"

#include "../shared_gui_components/NetworkProxyDialog.hpp"

#include "../utilities/core/Assert.hpp"

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
#include <QMimeData>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

namespace openstudio {

MainWindow::MainWindow(bool isPlugin, QWidget *parent) : 
  QMainWindow(parent),
  m_isPlugin(isPlugin),
  m_displayIP(true)
{
  setMinimumSize(900,658);
  setAcceptDrops(true);

  readSettings();

  this->statusBar()->show();

#ifdef Q_OS_LINUX
  QIcon icon = QIcon(":/images/os_16.png");
  icon.addPixmap(QPixmap(":/images/os_32.png"));
  icon.addPixmap(QPixmap(":/images/os_48.png"));
  icon.addPixmap(QPixmap(":/images/os_64.png"));
  icon.addPixmap(QPixmap(":/images/os_128.png"));
  icon.addPixmap(QPixmap(":/images/os_256.png"));
  setWindowIcon(icon);
#endif

  setObjectName("MainWindow");
  setStyleSheet("QWidget#MainWindow { background-color: #2C3233; }");

  m_mainSplitter = new QSplitter(Qt::Horizontal); 

  m_verticalTabWidget = new VerticalTabWidget();
  connect(m_verticalTabWidget, &VerticalTabWidget::tabSelected, this, &MainWindow::verticalTabSelected);
  m_mainSplitter->addWidget(m_verticalTabWidget);

  m_mainRightColumnContainer = new QStackedWidget();
  m_mainRightColumnContainer->setMinimumWidth(235);
  m_mainSplitter->addWidget(m_mainRightColumnContainer);

  m_mainSplitter->setStretchFactor(0,10000);

  setCentralWidget(m_mainSplitter);  

  MainMenu * mainMenu = new MainMenu(m_displayIP, m_isPlugin);
  connect(mainMenu, &MainMenu::toggleUnitsClicked, this, &MainWindow::toggleUnits);
  connect(mainMenu, &MainMenu::downloadComponentsClicked, this, &MainWindow::downloadComponentsClicked);
  connect(mainMenu, &MainMenu::openLibDlgClicked, this, &MainWindow::openLibDlgClicked);

  this->setMenuBar(mainMenu);

  connect(mainMenu, &MainMenu::exportClicked, this, &MainWindow::exportClicked);
  connect(mainMenu, &MainMenu::exportgbXMLClicked, this, &MainWindow::exportgbXMLClicked);
  connect(mainMenu, &MainMenu::exportSDDClicked, this, &MainWindow::exportSDDClicked);
  connect(mainMenu, &MainMenu::importClicked, this, &MainWindow::importClicked);
  connect(mainMenu, &MainMenu::importgbXMLClicked, this, &MainWindow::importgbXMLClicked);
  connect(mainMenu, &MainMenu::importSDDClicked, this, &MainWindow::importSDDClicked);
  connect(mainMenu, &MainMenu::loadFileClicked, this, &MainWindow::loadFileClicked);
  connect(mainMenu, &MainMenu::loadLibraryClicked, this, &MainWindow::loadLibraryClicked);
  connect(mainMenu, &MainMenu::saveAsFileClicked, this, &MainWindow::saveAsFileClicked);
  connect(mainMenu, &MainMenu::saveFileClicked, this, &MainWindow::saveFileClicked);
  connect(mainMenu, &MainMenu::revertFileClicked, this, &MainWindow::revertFileClicked);
  connect(mainMenu, &MainMenu::newClicked, this, &MainWindow::newClicked);
  connect(mainMenu, &MainMenu::exitClicked, this, &MainWindow::exitClicked);
  connect(mainMenu, &MainMenu::helpClicked, this, &MainWindow::helpClicked);
  connect(mainMenu, &MainMenu::aboutClicked, this, &MainWindow::aboutClicked);
  connect(mainMenu, &MainMenu::scanForToolsClicked, this, &MainWindow::scanForToolsClicked);
  connect(mainMenu, &MainMenu::showRunManagerPreferencesClicked, this, &MainWindow::showRunManagerPreferencesClicked);
  connect(mainMenu, &MainMenu::showRubyConsoleClicked, this, &MainWindow::showRubyConsoleClicked);
  connect(mainMenu, &MainMenu::toggleUnitsClicked, this, &MainWindow::toggleUnitsClicked);
  connect(mainMenu, &MainMenu::changeMyMeasuresDir, this, &MainWindow::changeMyMeasuresDir);
  connect(mainMenu, &MainMenu::applyMeasureClicked, this, &MainWindow::applyMeasureClicked);
  connect(mainMenu, &MainMenu::downloadMeasuresClicked, this, &MainWindow::downloadMeasuresClicked);
  connect(mainMenu, &MainMenu::changeBclLogin, this, &MainWindow::changeBclLogin);
  connect(mainMenu, &MainMenu::configureProxyClicked, this, &MainWindow::configureProxyClicked);
  connect(this, &MainWindow::enableRevertToSaved, mainMenu, &MainMenu::enableRevertToSavedAction);
}

QSize MainWindow::sizeHint() const
{
  return QSize(1024,700);
}

void MainWindow::setMainRightColumnView(QWidget * widget)
{
  if( QWidget * oldwidget = m_mainRightColumnContainer->currentWidget() )
  {
    m_mainRightColumnContainer->removeWidget(oldwidget);
  }

  m_mainRightColumnContainer->addWidget(widget);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->ignore();

  writeSettings();

  emit closeClicked();
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event)
{
  if(event->mimeData()->hasUrls())
  {
    event->ignore();
    /*QUrl url = event->mimeData()->urls().first();
    if (url.scheme().compare("file", Qt::CaseInsensitive) == 0 && url.toString().toLower().endsWith(".osm")) 
    {
      //event->accept();
      event->ignore();
    }
    else
    {
      event->ignore();
    }*/
  }
}

void MainWindow::dropEvent(QDropEvent * event)
{
  if(event->mimeData()->hasUrls())
  {
    emit osmDropped(event->mimeData()->urls().first().toLocalFile());
  }
}

void MainWindow::addVerticalTabButton(int id,
  QString toolTip,
  const QString & selectedImagePath,
  const QString & unSelectedImagePath,
  const QString & disabledImagePath)
{
  m_verticalTabWidget->addTabButton(id, toolTip, selectedImagePath, unSelectedImagePath, disabledImagePath);
}

void MainWindow::setView(MainTabView * view, int id)
{
  m_verticalTabWidget->setView(view, id);
}

MainTabView * MainWindow::view() const
{
  return m_verticalTabWidget->view();
}

void MainWindow::selectVerticalTab(int id)
{
  // TODO new call
  //m_verticalTabWidget->setCurrentId(id);
}

void MainWindow::selectVerticalTabByIndex(int index)
{
  m_verticalTabWidget->setCurrentIndex(index);
}

int MainWindow::verticalTabIndex()
{
  return m_verticalTabWidget->verticalTabIndex();
}

void MainWindow::closeSidebar()
{
  QList<int> sizeList;

  sizeList.push_back(1);

  sizeList.push_back(0);

  m_mainSplitter->setSizes(sizeList);
}

void MainWindow::openSidebar()
{
  QList<int> sizeList;

  sizeList.push_back(2);

  sizeList.push_back(1);

  m_mainSplitter->setSizes(sizeList);
}

bool MainWindow::displayIP()
{
  return m_displayIP;
}

void MainWindow::enableRevertToSavedAction(bool enable)
{
  emit enableRevertToSaved(enable);
}

void MainWindow::readSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(600, 400)).toSize();
  m_lastPath = settings.value("lastPath", QDir::homePath()).toString();
  resize(size);
  move(pos);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("state").toByteArray());
  m_displayIP = settings.value("displayIP").toBool();
}

void MainWindow::writeSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("geometry", saveGeometry());
  settings.setValue("state", saveState());
  settings.setValue("displayIP", m_displayIP);
}

QString MainWindow::lastPath() const
{
  return QDir().exists(m_lastPath) ? m_lastPath : QDir::homePath();
}

void MainWindow::toggleUnits(bool displayIP)
{
  m_displayIP = displayIP;
}

void MainWindow::configureProxyClicked()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  NetworkProxyDialog dialog(settings, this);
  QDialog::DialogCode result = static_cast<QDialog::DialogCode>(dialog.exec());

  if (result == QDialog::Accepted)
  {
    QNetworkProxy proxy = dialog.createProxy();
    if (proxy.type() != QNetworkProxy::NoProxy)
    {
      if (dialog.testProxy(proxy))
      {
        QNetworkProxy::setApplicationProxy(proxy);
        //setRubyProxyEnvironment(proxy);
      } else {
        QMessageBox::critical(this, "Proxy Configuration Error", "There is an error in your proxy configuration.");
      }
    }
  }
}

void MainWindow::loadProxySettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  QNetworkProxy proxy = NetworkProxyDialog::createProxy(settings);
  if (proxy.type() != QNetworkProxy::NoProxy)
  {
    if (NetworkProxyDialog::testProxy(proxy, this))
    {
      QNetworkProxy::setApplicationProxy(proxy);
      //setRubyProxyEnvironment(proxy);
    } else {
      QMessageBox::critical(this, "Proxy Configuration Error", "There is an error in your proxy configuration.");
      configureProxyClicked();
    }
  }
}

} // openstudio
