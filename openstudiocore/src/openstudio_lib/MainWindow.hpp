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

#ifndef OPENSTUDIO_MAINWINDOW_HPP
#define OPENSTUDIO_MAINWINDOW_HPP

#include <QMainWindow>

class QStackedWidget;

class QSplitter;

namespace openstudio {

class HorizontalTabWidget;

class MainTabView;

class VerticalTabWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

  MainWindow(bool isPlugin, QWidget *parent = 0);
  virtual ~MainWindow() {}

  void addVerticalTab( QWidget * widget,
                       int id,
                       QString toolTip,
                       const QString & selectedImagePath,
                       const QString & unSelectedImagePath );

  void deleteAllVerticalTabs();

  void setMainRightColumnView(QWidget * widget);

  //void addHorizontalTab( QWidget * widget,
  //                       int id,
  //                       const QString & label );

  void selectVerticalTab(int id);

  void selectVerticalTabByIndex(int index);

  MainTabView* verticalTabByIndex(int index);

  int verticalTabIndex();

  //void selectHorizontalTab(int id);

  void closeSidebar();

  void openSidebar();

  bool displayIP();

  void enableRevertToSavedAction(bool enable);

  QString lastPath() const;

  signals:

  void closeClicked();

  void verticalTabSelected(int);

  void exportClicked();

  void exportgbXMLClicked();

  void exportSDDClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void loadFileClicked();

  void osmDropped(QString path);

  void loadLibraryClicked();

  void saveAsFileClicked(); 

  void saveFileClicked();

  void revertFileClicked();

  void scanForToolsClicked();

  void showRunManagerPreferencesClicked();

  void newClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  void showRubyConsoleClicked();

  void toggleUnitsClicked(bool displayIP);

  void downloadComponentsClicked();

  void openLibDlgClicked();

  void changeMyMeasuresDir();

  void changeBclLogin();

  void applyMeasureClicked();

  void downloadMeasuresClicked();

  void enableRevertToSaved(bool enable);

  protected:

  void closeEvent(QCloseEvent * event);

  void dragEnterEvent(QDragEnterEvent * event);

  void dropEvent(QDropEvent * event);

  QSize sizeHint() const;

  private:

  void readSettings();

  void writeSettings();

  bool m_isPlugin;

  QStackedWidget * m_mainRightColumnContainer;

  VerticalTabWidget * m_verticalTabWidget;

  QSplitter * m_mainSplitter;

  bool m_displayIP;

  QString m_lastPath;

  private slots:

  void toggleUnits(bool displayIP);

  void configureProxyClicked();

  void loadProxySettings();

};

} // openstudio

#endif // OPENSTUDIO_MAINWINDOW_HPP

