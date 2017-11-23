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

#ifndef OPENSTUDIO_RUNTABVIEW_HPP
#define OPENSTUDIO_RUNTABVIEW_HPP

#include "../model/Model.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"
#include <boost/smart_ptr.hpp>
#include "MainTabView.hpp"
#include <QComboBox>
#include <QWidget>
#include <QProcess>
//#include "../runmanager/lib/ConfigOptions.hpp"
//#include "../runmanager/lib/RunManager.hpp"
//#include "../runmanager/lib/Workflow.hpp"

class QButtonGroup;
class QPlainTextEdit;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QStackedWidget;
class QToolButton;
class QTextEdit;
class QFileSystemWatcher;
class QTcpServer;
class QTcpSocket;
class QComboBox;

namespace openstudio {

  class RunView;

  class RunView : public QWidget
  {
    Q_OBJECT;

    public:

    RunView();

    private:

    REGISTER_LOGGER("openstudio::RunView");

    enum RUNMODE { RUN_NONE=0, RUN_ENERGY, RUN_BEC, RUN_ENERGY_BEC};
    void playButtonClicked(bool t_checked);
    void onRunProcessFinished(int exitCode, QProcess::ExitStatus status);

    //void onSimDirChanged(const QString &path);

    //void onFileChanged(const QString &path);

    void onOpenSimDirClicked();
    void onNewConnection();
    void onRunDataReady();

    //NOTE: BEC CODE
    bool doBecInput(const QString &path, const model::Model &model, QString &outpath, QString &err);
    double getPV(openstudio::model::Model* model);
    void addPVAndBenchmarkToFile(const QString &fileName, int mode);
    void updatePVInfile();
    void callRealBEC(const QString &dir);
    void logErrorText(const QString & text, const QString colorName=QString());
    void logNormalText(const QString & text, const QString colorName=QString());
    void logH1Text(const QString & text, const QString colorName=QString());
    void logH2Text(const QString & text, const QString colorName=QString());
    void playButtonClicked00(bool t_checked, RUNMODE runmode);
    void becFinished();
    //END BEC CODE.

    // Given an osm file, return the companion directory
    path resourcePath(const path & osmPath) const;
    
    QHash<QString, QList<double>> sunlits;
    float wwr_totoal;

    QToolButton * m_playButton;
    QProgressBar * m_progressBar;
    QLabel * m_statusLabel;
    QTextEdit * m_textInfo;
    QProcess * m_runProcess;
    QPushButton * m_openSimDirButton;
    QComboBox* m_runMode;
    QTcpServer * m_runTcpServer;
    QTcpSocket * m_runSocket;
    //QFileSystemWatcher * m_simDirWatcher;
    //QFileSystemWatcher * m_eperrWatcher;
    openstudio::path m_tempFolder;
    QString becoutputPath;
    double buildingArea;
    RUNMODE runmode;
    enum State { stopped = 0, initialization = 1, os_measures = 2, translator = 3, ep_measures = 4, preprocess = 5, simulation = 6, reporting_measures = 7, postprocess = 8, complete = 9 };
    State m_state = State::stopped;
    void doFinish();
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

      RunView * m_runView;
      //openstudio::runmanager::JobStatusWidget * m_status;
  };

} // openstudio

#endif // OPENSTUDIO_RUNTABVIEW_HPP
