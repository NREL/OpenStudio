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

#ifndef OPENSTUDIO_PATMAINWINDOW_H
#define OPENSTUDIO_PATMAINWINDOW_H

#include <QMainWindow>
#include "../utilities/core/Logger.hpp"

class QStackedWidget;

class QSplitter;

class QNetworkProxy;

namespace openstudio{
  
namespace pat {

class HorizontalTabWidget;

class PatVerticalTabWidget;

class PatMainMenu;

class PatMainWindow : public QMainWindow
{
  Q_OBJECT;
  REGISTER_LOGGER("openstudio.pat.PatMainWindow");

  public:

  PatMainWindow(QWidget *parent = 0);

  virtual ~PatMainWindow() {}

  PatVerticalTabWidget * verticalTabWidget;

  PatMainMenu * mainMenu;

  void setMainRightColumnView(QWidget * widget);

  void closeSidebar();

  void openSidebar();

  void showRightColumn();

  void hideRightColumn();

  signals:

  void newClicked();

  void loadFileClicked();

  void saveFileClicked();

  void saveAsFileClicked();

  void clearAllResultsClicked();

  void exportXmlClicked();

  void exportSpreadsheetClicked();

  void showToolsClicked();

  void scanForToolsClicked();

  void changeMeasuresClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  void openBclDlgClicked();

  void openCloudDlgClicked();

  void openMonitorUseDlgClicked();

  protected:

  void closeEvent(QCloseEvent * event);

  void dragEnterEvent(QDragEnterEvent * event);
  
  void dropEvent(QDropEvent * event);

  QSize sizeHint() const;

  private:

  void readSettings();

  void writeSettings();

  void setRubyProxyEnvironment(const QNetworkProxy &t_proxy);

  QStackedWidget * m_mainRightColumnContainer;

  QSplitter * m_mainSplitter;

  private slots:
  
  void configureProxyClicked();
  void loadProxySettings();

};

}

} // openstudio

#endif // OPENSTUDIO_PATMAINWINDOW_H

