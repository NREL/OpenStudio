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

#ifndef OPENSTUDIO_RUNTABVIEW_HPP
#define OPENSTUDIO_RUNTABVIEW_HPP

#include "../model/Model.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"
#include <boost/smart_ptr.hpp>
#include "MainTabView.hpp"
#include <QComboBox>
#include <QWidget>
#include "../runmanager/lib/ConfigOptions.hpp"
#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/Workflow.hpp"

class QButtonGroup;
class QPlainTextEdit;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QStackedWidget;
class QToolButton;

namespace openstudio {

  class RunView;

  namespace runmanager {
    class JobStatusWidget;
  }

  class RunListModel : public QAbstractListModel
  {
    public:

      RunListModel( RunView * runView );

      int rowCount( const QModelIndex & parent = QModelIndex() ) const override;

      QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;

      bool removeRows(int row, int count);

      void emitDataChanged();

    private:

      RunView * m_runView;
  };

  class RunItemView : public QWidget
  {
    Q_OBJECT

    public:

      //RunItemView(const model::myModelClass & run);
      RunItemView();

      virtual ~RunItemView() {}

      //model::myModelClass run();
      void run();

    private:

      //model::myModelClass m_run;
  };

  class RunView : public QWidget
  {
    Q_OBJECT;


    public:

      RunView(const model::Model & model,
          const openstudio::path &t_modelPath,
          const openstudio::path &t_tempFolder,
          openstudio::runmanager::RunManager t_runManager);

      virtual ~RunView() {}

      QStackedWidget * runStack();

      openstudio::runmanager::RunManager runManager();

    signals:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsFile);
      void toolsUpdated();
      //void useRadianceStateChanged(bool);

    public slots:
      void updateToolsWarnings();

    private slots:
      void playButtonClicked(bool);
      void runManagerStatsChanged();
      void outputDataAdded(const openstudio::UUID &, const std::string &t_data);
      void treeChanged(const openstudio::UUID &t_uuid);
      //void on_radianceWarningsAndErrorsClicked(bool checked);
      //void on_radianceGroupClicked(int idx);
      void requestStartRunManager();

    private:
      REGISTER_LOGGER("openstudio::RunView");
      void runFinished(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputFile);
      static openstudio::runmanager::ToolVersion getRequiredEnergyPlusVersion();
      //void getRadiancePreRunWarningsAndErrors(std::vector<std::string> & warnings,
      //                                        std::vector<std::string> & errors);
      //void showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
      //                                   const std::vector<std::string> & errors);
      void locateEnergyPlus();
      void updateRunManagerStats(openstudio::runmanager::RunManager t_runManager);

      //openstudio::path loadRmdbPath() const;
      //openstudio::path simulationDir() const;
      //openstudio::path modelPath() const;
      //openstudio::path scriptsDir() const;

      QWidget * m_runNavigatorWidget;
      QToolButton * m_playButton;
      QComboBox * m_chooser;
      QStackedWidget * m_runStack;
      QLabel * m_toolWarningLabel;
      QLabel * m_warningsLabel;
      QLabel * m_errorsLabel;
      QLabel * m_statusLabel;
      model::Model m_model;
      QProgressBar * m_progressBar;
      QPlainTextEdit * m_outputWindow;
      //QButtonGroup * m_radianceGroup;
      //QRadioButton * m_radianceButton;
      //QRadioButton * m_energyPlusButton;
      //std::vector<std::string> m_radianceWarnings;
      //std::vector<std::string> m_radianceErrors;
      std::shared_ptr<RunListModel> m_runListModel;
      openstudio::path m_modelPath;
      openstudio::path m_tempFolder;
  
      bool m_canceling;
      bool m_statsConnected;
  };

  class RunTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      RunTabView(const model::Model & model,
                  QWidget * parent = nullptr);

      virtual ~RunTabView() {}

    //signals:
    //  void resultsGenerated(const openstudio::path &t_sqlFile);

    private:

      //RunView * m_runView;
      //openstudio::runmanager::JobStatusWidget * m_status;
  };

} // openstudio

#endif // OPENSTUDIO_RUNTABVIEW_HPP
