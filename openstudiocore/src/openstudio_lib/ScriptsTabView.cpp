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

#include "ScriptsTabView.hpp"

#include "MainWindow.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/SyncMeasuresDialog.hpp"

#include "../analysisdriver/SimpleProject.hpp"

#include <QLabel>
#include <QVBoxLayout>

namespace openstudio {

ScriptsTabView::ScriptsTabView(QWidget * parent)
  : MainTabView("Measures",false,parent)
{
  //setTitle("Organize and Edit Measures for Project");

  // Main Content

  mainContent = new QWidget();

  QVBoxLayout * mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);
  mainContent->setLayout(mainContentVLayout);

  addTabWidget(mainContent);

  variableGroupListView = new OSListView(true);
  variableGroupListView->setContentsMargins(0,0,0,0);
  variableGroupListView->setSpacing(0);
  mainContentVLayout->addWidget(variableGroupListView);

  QString style;
  style.append("QWidget#Footer {");
  style.append("border-top: 1px solid black; ");
  style.append("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #B6B5B6, stop: 1 #737172); ");
  style.append("}");

  QWidget * footer = new QWidget();
  footer->setObjectName("Footer");
  footer->setStyleSheet(style);
  mainContentVLayout->addWidget(footer);

  QHBoxLayout * layout = new QHBoxLayout();
  layout->setSpacing(0);
  footer->setLayout(layout);

  m_updateMeasuresButton = new BlueButton();
  m_updateMeasuresButton->setText("Sync Project Measures with Library");
  m_updateMeasuresButton->setToolTip("Check the Library for Newer Versions of the Measures in Your Project and Provides Sync Option");
  layout->addStretch();
  layout->addWidget(m_updateMeasuresButton);

  bool isConnected = false;
  isConnected = connect(m_updateMeasuresButton,SIGNAL(clicked()), this,SLOT(openUpdateMeasuresDlg()));
  OS_ASSERT(isConnected);
}

void ScriptsTabView::showEvent(QShowEvent *e)
{
  MainTabView::showEvent(e);

  boost::optional<openstudio::analysisdriver::SimpleProject> project = OSAppBase::instance()->project();
  if (project)
  {
    // DLM: why is this necessary?
    OSAppBase::instance()->measureManager().updateMeasures(*project, project->measures(), false);
  }
  variableGroupListView->refreshAllViews();
}

//*****SLOTS*****

void ScriptsTabView::openUpdateMeasuresDlg()
{
  openstudio::OSAppBase * app = OSAppBase::instance();

  boost::optional<analysisdriver::SimpleProject> project = app->project();
  OS_ASSERT(project);

  m_syncMeasuresDialog = boost::shared_ptr<SyncMeasuresDialog>(new SyncMeasuresDialog(&(project.get()),&(app->measureManager())));
  m_syncMeasuresDialog->setGeometry(app->currentDocument()->mainWindow()->geometry());
  m_syncMeasuresDialog->exec();
}

} // openstudio

