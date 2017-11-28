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

  MainWindow(bool isPlugin, QWidget *parent = nullptr);
  virtual ~MainWindow() {}

  void addVerticalTabButton(int id,
    QString toolTip,
    const QString & selectedImagePath,
    const QString & unSelectedImagePath,
    const QString & disabledImagePath);

  void setView(MainTabView * view, int id);

  MainTabView * view() const;

  void setMainRightColumnView(QWidget * widget);

  void selectVerticalTab(int id);

  void selectVerticalTabByIndex(int index);

  int verticalTabIndex();

  void closeSidebar();

  void openSidebar();

  bool displayIP();

  void enableRevertToSavedAction(bool enable);

  QString lastPath() const;

  VerticalTabWidget * verticalTabWidget() { return m_verticalTabWidget; }

  signals:

  void closeClicked();

  void verticalTabSelected(int);

  void exportClicked();

  void exportgbXMLClicked();

  void exportSDDClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void importIFCClicked();

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

  void closeEvent(QCloseEvent * event) override;

  void dragEnterEvent(QDragEnterEvent * event) override;

  void dropEvent(QDropEvent * event) override;

  QSize sizeHint() const override;

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
