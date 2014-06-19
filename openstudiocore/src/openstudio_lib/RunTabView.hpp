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

      int rowCount( const QModelIndex & parent = QModelIndex() ) const;

      QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

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
      void useRadianceStateChanged(bool);

    public slots:
      void updateToolsWarnings();

    private slots:
      void playButtonClicked(bool);
      void runManagerStatsChanged();
      void outputDataAdded(const openstudio::UUID &, const std::string &t_data);
      void treeChanged(const openstudio::UUID &t_uuid);
      void on_radianceWarningsAndErrorsClicked(bool checked);
      void on_radianceGroupClicked(int idx);

    private:
      REGISTER_LOGGER("openstudio::RunView");
      void runFinished(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputFile);
      static openstudio::runmanager::ToolVersion getRequiredEnergyPlusVersion();
      void getRadiancePreRunWarningsAndErrors(std::vector<std::string> & warnings,
                                              std::vector<std::string> & errors);
      void showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
                                         const std::vector<std::string> & errors);
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
      QButtonGroup * m_radianceGroup;
      QRadioButton * m_radianceButton;
      QRadioButton * m_energyPlusButton;
      std::vector<std::string> m_radianceWarnings;
      std::vector<std::string> m_radianceErrors;
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
                  QWidget * parent = 0);

      virtual ~RunTabView() {}

    //signals:
    //  void resultsGenerated(const openstudio::path &t_sqlFile);

    private:

      //RunView * m_runView;
      //openstudio::runmanager::JobStatusWidget * m_status;
  };

} // openstudio

#endif // OPENSTUDIO_RUNTABVIEW_HPP
