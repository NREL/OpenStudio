/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <pat_app/MeasuresView.hpp>
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDragEnterEvent>

namespace openstudio{
  
namespace pat {

MeasuresTabView::MeasuresTabView()
  : PatMainTabView()
{
  setTitle("Organize and Edit Measures for Project");

  // Main Content

  mainContent = new QWidget();

  QVBoxLayout * mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);
  mainContent->setLayout(mainContentVLayout);

  viewSwitcher->setView(mainContent);

  // Select Baseline Header

  QWidget * selectBaselineHeader = new QWidget();
  selectBaselineHeader->setFixedHeight(30);
  selectBaselineHeader->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  selectBaselineHeader->setObjectName("SelectBaselineHeader");
  selectBaselineHeader->setStyleSheet("QWidget#SelectBaselineHeader { background: #494949; }");

  mainContentVLayout->addWidget(selectBaselineHeader);

  QHBoxLayout * baselineHeaderHLayout = new QHBoxLayout(); 
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
  QHBoxLayout * baselineNameBackgroundLayout = new QHBoxLayout();
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
}


} // pat

} // openstudio

