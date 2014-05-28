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

#ifndef PATAPP_PATMAINMENU_HPP
#define PATAPP_PATMAINMENU_HPP

#include <QMenuBar>

class QMenu;

namespace openstudio{
  
namespace pat {

class PatMainMenu : public QMenuBar
{
  Q_OBJECT

  public:

  PatMainMenu(QWidget *parent = nullptr);

  virtual ~PatMainMenu();

  void configure(bool haveCurrentProject);

  signals:

  void newClicked();

  void loadFileClicked();

  void saveFileClicked();

  void saveAsFileClicked();

  void clearAllResultsClicked();

  void exportXmlClicked();

  void exportSpreadsheetClicked();

  void scanForToolsClicked();

  void showToolsClicked();

  void openBclDlgClicked();

  void openCloudDlgClicked();

  void openMonitorUseDlgClicked();

  void changeMeasuresClicked();

  void configureProxyClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  void toggleUnitsClicked(bool displayIP);

  private:

  QMenu * m_fileMenu;

  QMenu * m_preferencesMenu;

  QMenu * m_windowMenu;

  QMenu * m_cloudMenu;

  QMenu * m_helpMenu;

  QAction * m_openBclDlgAction;

  QAction * m_openCloudDlgAction;

  QAction * m_openMonitorUseDlgAction;

  QAction * m_newAction;
  QAction * m_loadFileAction;
  QAction * m_saveFileAction;
  QAction * m_saveAsFileAction;
  QAction * m_clearAllResultsAction;
  QAction * m_exportXmlAction;
  QAction * m_exportSpreadsheetAction;
  QAction * m_scanForToolsAction;
  QAction * m_showToolsAction;
  QAction * m_exitAction;
  QAction * m_changeMeasuresDir;
  QAction * m_configureProxy;
  QAction * m_helpAction;
  QAction * m_aboutAction;

};

}

} // openstudio

#endif // PATAPP_PATMAINMENU_HPP

