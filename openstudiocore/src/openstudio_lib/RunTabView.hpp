/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

namespace openstudio {

  class RunView;

  class RunView : public QWidget
  {
    Q_OBJECT;

    public:

    RunView();

    private:

    REGISTER_LOGGER("openstudio::RunView");

    void playButtonClicked(bool t_checked);

    void onRunProcessFinished(int exitCode, QProcess::ExitStatus status);

    //void onSimDirChanged(const QString &path);

    //void onFileChanged(const QString &path);

    void onOpenSimDirClicked();

    // Given an osm file, return the companion directory
    path resourcePath(const path & osmPath) const;

    QToolButton * m_playButton;
    QProgressBar * m_progressBar;
    QLabel * m_statusLabel;
    QTextEdit * m_textInfo;
    QProcess * m_runProcess;
    QPushButton * m_openSimDirButton;
    //QFileSystemWatcher * m_simDirWatcher;
    //QFileSystemWatcher * m_eperrWatcher;
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
