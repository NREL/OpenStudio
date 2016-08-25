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

#include "RunTabView.hpp"

#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include <OpenStudio.hxx>

#include "../model/FileOperations.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/GlareSensor_Impl.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/IlluminanceMap_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"

//#include "../runmanager/lib/JobStatusWidget.hpp"
//#include "../runmanager/lib/RubyJobUtils.hpp"
//#include "../runmanager/lib/RunManager.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/core/Assert.hpp"

#include "../shared_gui_components/WorkflowTools.hpp"



#include "../energyplus/ForwardTranslator.hpp"

#include <QButtonGroup>
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPlainTextEdit>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QSysInfo>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QProcess>
#include <QStandardPaths>
#include <QFileSystemWatcher>
#include <QDesktopServices>
#include <QMessageBox>

namespace openstudio {

RunTabView::RunTabView(const model::Model & model,
  QWidget * parent)
  : MainTabView("Run Simulation", MainTabView::MAIN_TAB, parent),
    m_runView(new RunView())
{
  addTabWidget(m_runView);
}

RunView::RunView()
  : QWidget()
{
  auto mainLayout = new QGridLayout();
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(5);
  setLayout(mainLayout);

  m_playButton = new QToolButton();
  m_playButton->setText("Run");
  m_playButton->setCheckable(true);
  m_playButton->setChecked(false);
  QIcon playbuttonicon(QPixmap(":/images/run_simulation_button.png"));
  playbuttonicon.addPixmap(QPixmap(":/images/run_simulation_button.png"), QIcon::Normal, QIcon::Off);
  playbuttonicon.addPixmap(QPixmap(":/images/cancel_simulation_button.png"), QIcon::Normal, QIcon::On);
  m_playButton->setStyleSheet("QToolButton { background:transparent; font: bold; }");
  m_playButton->setIconSize(QSize(35,35));
  m_playButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_playButton->setIcon(playbuttonicon);
  m_playButton->setLayoutDirection(Qt::RightToLeft);
  m_playButton->setStyleSheet("QAbstractButton:!hover { border: none; }");
  
  mainLayout->addWidget(m_playButton, 0, 0);
  connect(m_playButton, &QToolButton::clicked, this, &RunView::playButtonClicked);
  
  // Progress bar area
  m_progressBar = new QProgressBar();
  
  auto progressbarlayout = new QVBoxLayout();
  progressbarlayout->addWidget(m_progressBar);
  mainLayout->addLayout(progressbarlayout, 0, 1);

  m_openSimDirButton = new QPushButton();
  m_openSimDirButton->setText("Show Simulation");
  m_openSimDirButton->setFlat(true);
  m_openSimDirButton->setObjectName("StandardGrayButton");
  connect(m_openSimDirButton, &QPushButton::clicked, this, &RunView::onOpenSimDirClicked);
  mainLayout->addWidget(m_openSimDirButton,0,2);

  m_textInfo = new QTextEdit();
  mainLayout->addWidget(m_textInfo,1,0,1,3);

  m_runProcess = new QProcess(this);
  connect(m_runProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &RunView::onRunProcessFinished);

  //m_simDirWatcher = new QFileSystemWatcher(this);
  //connect(m_simDirWatcher, &QFileSystemWatcher::directoryChanged, this, &RunView::onSimDirChanged);
  //connect(m_simDirWatcher, &QFileSystemWatcher::fileChanged, this, &RunView::onFileChanged);
  //m_eperrWatcher = new QFileSystemWatcher(this);
}

void RunView::onOpenSimDirClicked()
{
  std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
  QString url = QString::fromStdString((resourcePath(toPath(osdocument->savePath())) / "run").string());
  QUrl qurl = QUrl::fromLocalFile(url);
  if( ! QDesktopServices::openUrl(qurl) ) {
    QMessageBox::critical(this, "Unable to open simulation", "Please save the OpenStudio Model to view the simulation.");
  }
}

void RunView::onRunProcessFinished(int exitCode, QProcess::ExitStatus status)
{
  LOG(Debug, "run finished");
  m_playButton->setChecked(false);

  std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
  osdocument->enableTabsAfterRun();
}

//void RunView::onSimDirChanged(const QString &path)
//{
//  LOG(Debug, "Sim Dir Changed: " << path.toStdString());
//}
//
//void RunView::onFileChanged(const QString &path)
//{
//  LOG(Debug, "File Changed: " << path.toStdString());
//}

openstudio::path RunView::resourcePath(const openstudio::path & osmPath) const
{
  auto baseName = osmPath.stem();
  auto parentPath = osmPath.parent_path();
  auto resourcePath = parentPath / baseName;
  return resourcePath; 
}

void RunView::playButtonClicked(bool t_checked)
{
  LOG(Debug, "playButtonClicked " << t_checked);

  std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();

  if (t_checked) {
    // run

    if(osdocument->modified())
    {
      osdocument->save();
      // save dialog was canceled
      if(osdocument->modified()) {
        m_playButton->setChecked(false);
        return;
      }
    }

    QStringList paths;
    paths << QCoreApplication::applicationDirPath();
    auto openstudioExePath = QStandardPaths::findExecutable("openstudio", paths);

    auto workflowPath = resourcePath(toPath(osdocument->savePath())) / "workflow.osw";
    auto workflowJSONPath = QString::fromStdString(workflowPath.string());
    QStringList arguments;
    arguments << "run" << "-w" << workflowJSONPath;

    LOG(Debug, "run exe" << openstudioExePath.toStdString());
    LOG(Debug, "run arguments" << arguments.join(";").toStdString());

    // Not a great idea
    //m_simDirWatcher->addPath( QString::fromStdString((resourcePath / "run/").string()) );
    //std::ofstream outfile((resourcePath / "run/stdout-energyplus").string(),std::ofstream::trunc);
    //outfile.close();
    //m_simDirWatcher->addPath( QString::fromStdString((resourcePath / "run/stdout-energyplus").string()) );

    osdocument->disableTabsDuringRun();

    m_runProcess->start(openstudioExePath, arguments);
  } else {
    // stop running
    LOG(Debug, "Kill Simulation");
    m_runProcess->kill();
  }
}

} // openstudio
