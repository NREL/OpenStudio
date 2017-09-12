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

#ifndef OPENSTUDIO_RESULTSTABVIEW_HPP
#define OPENSTUDIO_RESULTSTABVIEW_HPP

#include "MainTabView.hpp"

#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/units/Unit.hpp"

//#include "../runmanager/lib/RunManager.hpp"

#include <QWidget>
#include <QWebEngineView>
#include <QProgressBar>

class QComboBox;
class QPushButton;

namespace openstudio {

  // main widget

  class ResultsView : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsView(QWidget *t_parent = nullptr);
      virtual ~ResultsView();
      void searchForExistingResults(const openstudio::path &t_runDir, const openstudio::path &t_reportsDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);
      void treeChanged(const openstudio::UUID &t_uuid);

    private slots:
      void refreshClicked();
      void openDViewClicked();
      void comboBoxChanged(int index);

      // DLM: for debugging
      void 	onLoadFinished(bool ok);
      void 	onLoadProgress(int progress);
      void 	onLoadStarted();
      void 	onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode);

    private:
      REGISTER_LOGGER("openstudio::ResultsView");
      //openstudio::runmanager::RunManager runManager();
      void populateComboBox(std::vector<openstudio::path> reports);

      bool m_isIP;

      // utility bill results
      QLabel * m_reportLabel;

      QProgressBar * m_progressBar;
      QPushButton * m_refreshBtn;
      QPushButton * m_openDViewBtn;
      
      openstudio::path m_sqlFilePath;
      openstudio::path m_radianceResultsPath;

      QWebEngineView * m_view;
      QComboBox * m_comboBox;
  };

  class ResultsTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      ResultsTabView(const QString & tabLabel,
        TabType tabType,
        QWidget * parent = nullptr);
      virtual ~ResultsTabView() {}

    public slots:

      void onUnitSystemChange(bool t_isIP);

    signals:

      void removeResultClicked();

      void importResultClicked();

      void treeChanged(const openstudio::UUID &t_uuid);

    private:
      ResultsView * m_resultsView;
      REGISTER_LOGGER("openstudio::ResultsTabView");
  };

} // openstudio

#endif // OPENSTUDIO_RESULTSTABVIEW_HPP
