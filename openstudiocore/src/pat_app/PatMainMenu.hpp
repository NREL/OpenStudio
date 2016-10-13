/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  void exitClicked(bool fromCloseEvent);

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

