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

#ifndef PATAPP_PATMAINWINDOW_HPP
#define PATAPP_PATMAINWINDOW_HPP

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

  PatMainWindow(QWidget *parent = nullptr);

  virtual ~PatMainWindow() {}

  PatVerticalTabWidget * verticalTabWidget;

  PatMainMenu * mainMenu;

  void setMainRightColumnView(QWidget * widget);

  void closeSidebar();

  void openSidebar();

  void showRightColumn();

  void hideRightColumn();

  QString lastPath() const;

  void setLastPath(const QString& t_lastPath);

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

  void exitClicked(bool fromCloseEvent);

  void helpClicked();

  void aboutClicked();

  void openBclDlgClicked();

  void openCloudDlgClicked();

  void openMonitorUseDlgClicked();

  protected:

  void closeEvent(QCloseEvent * event) override;

  void dragEnterEvent(QDragEnterEvent * event) override;
  
  void dropEvent(QDropEvent * event) override;

  QSize sizeHint() const override;

  private:

  void readSettings();

  void writeSettings();

  void setRubyProxyEnvironment(const QNetworkProxy &t_proxy);

  QStackedWidget * m_mainRightColumnContainer;

  QSplitter * m_mainSplitter;

  QString m_lastPath;

  private slots:
  
  void configureProxyClicked();
  void loadProxySettings();

};

}

} // openstudio

#endif // PATAPP_PATMAINWINDOW_HPP

