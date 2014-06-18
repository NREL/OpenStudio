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

#include "DesignAlternativesView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include "../utilities/core/Assert.hpp"

#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QLineEdit>
#include <QTextEdit>

namespace openstudio {

namespace pat {

DesignAlternativesTabView::DesignAlternativesTabView()
  : PatMainTabView()
{
  setTitle("Select Measures and Create Design Alternatives");

  auto splitter = new QSplitter(Qt::Horizontal);

  viewSwitcher->setView(splitter);

  // Left content

  auto leftContent = new QWidget();
  leftContent->setMinimumWidth(270);
  splitter->addWidget(leftContent);

  auto mainLeftContentVLayout = new QVBoxLayout();
  mainLeftContentVLayout->setContentsMargins(0,0,0,0);
  mainLeftContentVLayout->setSpacing(0);
  mainLeftContentVLayout->setAlignment(Qt::AlignTop);
  leftContent->setLayout(mainLeftContentVLayout);

  auto header = new QWidget();
  header->setObjectName("Header");
  header->setStyleSheet("QWidget#Header { background: #D5D5D5; border-bottom: 1px solid #8C8C8C;}");

  auto headerVLayout = new QVBoxLayout();
  headerVLayout->setContentsMargins(5,5,5,5);
  headerVLayout->setSpacing(5);
  header->setLayout(headerVLayout);

  mainLeftContentVLayout->addWidget(header);

  QLabel * leftTitleLabel = new QLabel("Select Measure(s) from Project Measures");
  leftTitleLabel->setObjectName("H2");
  headerVLayout->addWidget(leftTitleLabel);

  auto selectionButtonLayout = new QHBoxLayout();
  selectionButtonLayout->setContentsMargins(0,0,0,0);
  selectionButtonLayout->setSpacing(5);
  selectionButtonLayout->setAlignment(Qt::AlignLeft);
  headerVLayout->addLayout(selectionButtonLayout);

  selectAllButton = new GrayButton();
  selectAllButton->setText("Select All");
  selectAllButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  selectionButtonLayout->addWidget(selectAllButton);

  clearSelectionsButton = new GrayButton();
  clearSelectionsButton->setText("Clear Selection");
  clearSelectionsButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  selectionButtonLayout->addWidget(clearSelectionsButton);
  clearSelectionsButton->setEnabled(false);

  measuresListView = new OSListView(true); 
  measuresListView->setContentsMargins(0,0,0,0);
  mainLeftContentVLayout->addWidget(measuresListView);

  // Right content

  designAltsView = new altstab::DesignAltsView();
  splitter->addWidget(designAltsView);
  splitter->setStretchFactor(1,100);

}

namespace altstab {

DesignAltsView::DesignAltsView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  auto header = new QWidget();
  header->setObjectName("Header");
  header->setStyleSheet("QWidget#Header { background: #D5D5D5; border-bottom: 1px solid #8C8C8C;}");

  auto headerVLayout = new QVBoxLayout();
  headerVLayout->setAlignment(Qt::AlignTop);
  headerVLayout->setContentsMargins(5,5,5,5);
  headerVLayout->setSpacing(5);
  header->setLayout(headerVLayout);

  QLabel * headerTitleLabel = new QLabel("Create Design Alternatives");
  headerTitleLabel->setObjectName("H2");
  headerVLayout->addWidget(headerTitleLabel);

  auto headerButtonHLayout = new QHBoxLayout();
  headerButtonHLayout->setAlignment(Qt::AlignLeft);
  headerButtonHLayout->setContentsMargins(0,0,0,0);
  headerButtonHLayout->setSpacing(5);
  headerVLayout->addLayout(headerButtonHLayout);

  createOneForEachSelectedMeasureButton = new CreateOneForEachSelectedMeasureButton();
  headerButtonHLayout->addWidget(createOneForEachSelectedMeasureButton);
  createOneForEachSelectedMeasureButton->setEnabled(false);

  createOneWithSelectedMeasuresButton = new CreateOneWithSelectedMeasuresButton();
  headerButtonHLayout->addWidget(createOneWithSelectedMeasuresButton);
  createOneWithSelectedMeasuresButton->setEnabled(false);

  createFromFileButton = new CreateFromFileButton();
  headerButtonHLayout->addWidget(createFromFileButton);

  headerButtonHLayout->addStretch();

  mainVLayout->addWidget(header);

  designAltsListView = new OSListView(true);
  designAltsListView->setContentsMargins(5,5,5,5);
  mainVLayout->addWidget(designAltsListView);
}

VariableGroupItemView::VariableGroupItemView()
  : OSCollapsibleView(nullptr)
{
  variableGroupHeaderView = new DarkGradientHeader();
  setHeader(variableGroupHeaderView);

  variableGroupContentView = new OSListView();
  variableGroupContentView->setContentsMargins(0,5,0,0);
  setContent(variableGroupContentView);

  setExpanded(true);
}

VariableItemView::VariableItemView()
  : OSCollapsibleView(nullptr)
{
  variableHeaderView = new QLabel();
  variableHeaderView->setObjectName("H2");
  variableHeaderView->setContentsMargins(5,0,5,0);
  setHeader(variableHeaderView);

  variableContentView = new OSListView();
  variableContentView->setContentsMargins(0,5,0,0);
  setContent(variableContentView);

  setExpanded(true);
}

MeasureItemView::MeasureItemView()
  : QAbstractButton()
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(20,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);
  
  label = new QLabel();
  mainHLayout->addWidget(label);
}

void MeasureItemView::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis )
  {
    setStyleSheet("openstudio--pat--altstab--MeasureItemView { background: #FECD60; border: 2px solid #EE641A; }");
  }
  else
  {
    setStyleSheet("openstudio--pat--altstab--MeasureItemView {background: transparent;}");
  }
}

void MeasureItemView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

DesignAltHeaderView::DesignAltHeaderView(bool t_isBaseline)
  : OSHeader(new HeaderToggleButton())
{
  setFixedHeight(40);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,0,5,0);
  mainHLayout->setSpacing(5);
  mainHLayout->setAlignment(Qt::AlignLeft);
  setLayout(mainHLayout);

  mainHLayout->addWidget(toggleButton);

  QRegExp nameRegex("^\\S.*");
  auto validator = new QRegExpValidator(nameRegex, this);

  designAltNameEdit = new QLineEdit();
  designAltNameEdit->setAcceptDrops(false);
  designAltNameEdit->setValidator(validator);
  mainHLayout->addWidget(designAltNameEdit);

  removeButton = new SofterRemoveButton();
  mainHLayout->addWidget(removeButton);

  if (t_isBaseline)
  {
    designAltNameEdit->setVisible(false);
    removeButton->setVisible(false);

    QLabel *lbl = new QLabel("Baseline and Measures Applied to All");
    lbl->setStyleSheet("color:#FFFFFF; font:italic bold");
    mainHLayout->addWidget(lbl);
  }

 }

DesignAltContentView::DesignAltContentView(bool t_isBaseline)
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(25,0,5,5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  perturbationListView = new OSListView();
  perturbationListView->setContentsMargins(0,0,0,0);

  mainVLayout->addWidget(perturbationListView);

  QLabel * descriptionTitleLabel = new QLabel("Description");
  descriptionTitleLabel->setObjectName("H2");
  mainVLayout->addWidget(descriptionTitleLabel);

  descriptionTextEdit = new QTextEdit();
  descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  descriptionTextEdit->setMaximumWidth(600);
  descriptionTextEdit->setFixedHeight(60);
  descriptionTextEdit->setStyleSheet("QTextEdit { background: #E0E0E0; }");
  mainVLayout->addWidget(descriptionTextEdit);

  if (t_isBaseline)
  {
    perturbationListView->setStyleSheet("color:#D5D5D5");
    descriptionTitleLabel->setVisible(false);
    descriptionTextEdit->setVisible(false);
  }


  bool bingo = connect(descriptionTextEdit,SIGNAL(textChanged()),this,SLOT(onDescriptionTextChanged()));

  OS_ASSERT(bingo);
}

void DesignAltContentView::onDescriptionTextChanged()
{
  emit descriptionChanged(descriptionTextEdit->toPlainText());
}

DesignAltItemView::DesignAltItemView(bool t_isBaseline)
  : OSCollapsibleView(nullptr)
{
  designAltHeaderView = new DesignAltHeaderView(t_isBaseline);
  setHeader(designAltHeaderView);

  designAltContentView = new DesignAltContentView(t_isBaseline);
  setContent(designAltContentView);
  
  if (t_isBaseline)
  {
    setStyleSheet("openstudio--pat--altstab--DesignAltItemView { background: #5B5B5B; } ");
  } else {
    setStyleSheet("openstudio--pat--altstab--DesignAltItemView { background: #D5D5D5; }");
  }

  setMaximumWidth(650);

  setExpanded(true);
}

void DesignAltItemView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // altstab

} // pat

} // openstudio
