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

#include "MeasuresView.hpp"

#include "PatApp.hpp"
#include "PatMainWindow.hpp"

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/SyncMeasuresDialog.hpp"
#include "../shared_gui_components/MeasureManager.hpp"

#include <QDragEnterEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>

namespace openstudio{
  
namespace pat {

MeasuresTabView::MeasuresTabView()
  : PatMainTabView()
{
  setTitle("Organize and Edit Measures for Project");

  // Main Content

  mainContent = new QWidget();

  auto mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);
  mainContent->setLayout(mainContentVLayout);

  viewSwitcher->setView(mainContent);

  // Select Baseline Header

  auto selectBaselineHeader = new QWidget();
  selectBaselineHeader->setFixedHeight(30);
  selectBaselineHeader->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  selectBaselineHeader->setObjectName("SelectBaselineHeader");
  selectBaselineHeader->setStyleSheet("QWidget#SelectBaselineHeader { background: #494949; }");

  mainContentVLayout->addWidget(selectBaselineHeader);

  auto baselineHeaderHLayout = new QHBoxLayout(); 
  baselineHeaderHLayout->setContentsMargins(5,5,5,5);
  baselineHeaderHLayout->setSpacing(10);

  selectBaselineHeader->setLayout(baselineHeaderHLayout);

  baselineCautionLabel = new QLabel();
  baselineCautionLabel->setPixmap(QPixmap(":shared_gui_components/images/warning_icon.png"));
  baselineHeaderHLayout->addWidget(baselineCautionLabel);

  QLabel * selectBaselineLabel = new QLabel("Select Your Baseline Model");
  selectBaselineLabel->setStyleSheet("QLabel { color: white; font: bold; }");
  baselineHeaderHLayout->addWidget(selectBaselineLabel);

  baselineNameBackground = new QWidget();
  baselineNameBackground->setObjectName("BaselineNameBackground");
  baselineNameBackground->setStyleSheet("QWidget#BaselineNameBackground { background: #D9D9D9 }");
  baselineNameBackground->setMinimumWidth(250);
  auto baselineNameBackgroundLayout = new QHBoxLayout();
  baselineNameBackgroundLayout->setContentsMargins(5,2,5,2);
  baselineNameBackgroundLayout->setSpacing(5);
  baselineNameBackground->setLayout(baselineNameBackgroundLayout);
  baselineHeaderHLayout->addWidget(baselineNameBackground);

  baselineLabel = new QLabel();
  baselineNameBackgroundLayout->addWidget(baselineLabel);

  selectBaselineButton = new GrayButton();
  selectBaselineButton->setText("Browse");
  baselineHeaderHLayout->addWidget(selectBaselineButton);

  baselineHeaderHLayout->addStretch();

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

//*****SLOTS*****

void MeasuresTabView::openUpdateMeasuresDlg()
{
  PatApp * app = PatApp::instance();

  boost::optional<analysisdriver::SimpleProject> project = app->project();
  OS_ASSERT(project);

  m_syncMeasuresDialog = boost::shared_ptr<SyncMeasuresDialog>(new SyncMeasuresDialog(&(project.get()),&(app->measureManager())));
  m_syncMeasuresDialog->setGeometry(app->mainWindow->geometry());
  m_syncMeasuresDialog->exec();
}

} // pat

} // openstudio

