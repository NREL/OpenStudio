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

#ifndef OPENSTUDIO_MAINMENU_HPP
#define OPENSTUDIO_MAINMENU_HPP

#include <QMenuBar>

class QMenu;

namespace openstudio {

class MainMenu : public QMenuBar
{
  Q_OBJECT

  public:

  MainMenu(bool isIP, bool isPlugin, QWidget *parent = 0);

  virtual ~MainMenu();

  signals:

  void exportClicked();

  void exportgbXMLClicked();

  void exportSDDClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void loadFileClicked();

  void loadLibraryClicked();

  void revertFileClicked(); 

  void saveAsFileClicked(); 

  void saveFileClicked();

  void newClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  void showRubyConsoleClicked();

  void scanForToolsClicked();

  void showRunManagerPreferencesClicked();

  void toggleUnitsClicked(bool displayIP);

  void openLibDlgClicked();

  void configureProxyClicked();

  void applyMeasureClicked();

  void downloadMeasuresClicked();

  void downloadComponentsClicked();

  void changeMyMeasuresDir();

  void changeBclLogin();

  private:

  bool m_isPlugin;

  QMenu * m_fileMenu;

  QMenu * m_measureMenu;

  QMenu * m_helpMenu;

  QMenu * m_preferencesMenu;

  QAction * m_displayIPUnitsAction;

  QAction * m_displaySIUnitsAction;

  QAction * m_openLibDlgAction;

  QAction * m_configureProxy;

  QAction * m_revertToSavedAction;

  bool m_isIP;

  public slots:

  void enableRevertToSavedAction(bool enable);

  private slots:

  void displaySIUnitsClicked();

  void displayIPUnitsClicked();

};

} // openstudio

#endif // OPENSTUDIO_MAINMENU_HPP

