/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_MAINMENU_HPP
#define OPENSTUDIO_MAINMENU_HPP

#include <QMenuBar>

class QMenu;

namespace openstudio {

class MainMenu : public QMenuBar
{
  Q_OBJECT

  public:

  MainMenu(bool isIP, bool isPlugin, QWidget *parent = nullptr);

  virtual ~MainMenu();

  signals:

  void exportClicked();

  void exportgbXMLClicked();

  void exportSDDClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void importIFCClicked();

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

