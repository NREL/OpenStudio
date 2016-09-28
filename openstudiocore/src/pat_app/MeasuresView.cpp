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

  auto footer = new QWidget();
  footer->setObjectName("Footer");
  footer->setStyleSheet(style);
  mainContentVLayout->addWidget(footer);

  auto layout = new QHBoxLayout();
  layout->setSpacing(0);
  footer->setLayout(layout);

  m_updateMeasuresButton = new BlueButton();
  m_updateMeasuresButton->setText("Sync Project Measures with Library");
  m_updateMeasuresButton->setToolTip("Check the Library for Newer Versions of the Measures in Your Project and Provides Sync Option");
  layout->addStretch();
  layout->addWidget(m_updateMeasuresButton);

  connect(m_updateMeasuresButton, &QPushButton::clicked, this, &MeasuresTabView::openUpdateMeasuresDlg);
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

