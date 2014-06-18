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

#include "VariableView.hpp"
#include "OSViewSwitcher.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include "Buttons.hpp"
#include "LocalLibraryController.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QMimeData>
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
  
namespace measuretab {

NewGroupView::NewGroupView(bool t_fixed, bool t_fixedOnly)
  : QWidget()
{
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setMaximumWidth(660);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  auto newGroupHLayout = new QHBoxLayout();
  newGroupHLayout->setContentsMargins(0,0,0,0);
  newGroupHLayout->setSpacing(5);
  mainVLayout->addLayout(newGroupHLayout);

  auto addIconLabel = new QLabel();
  addIconLabel->setPixmap(QPixmap(":/shared_gui_components/images/gray_add_icon.png"));
  addIconLabel->setFixedSize(16,16);
  newGroupHLayout->addWidget(addIconLabel);

  QLabel * newGroupLabel;

  if (t_fixed)
  {
    if (t_fixedOnly)
    {
      newGroupLabel = new QLabel("<i><b>New Always Run Measure</b><i>");
    } else {
      newGroupLabel = new QLabel("<i><b>New Always Run Measure</b> (This measure is applied to the baseline and all design alternatives)<i>");
    }
  } else {
    newGroupLabel = new QLabel("<b>New Measure Group</b> (One measure from this group can be applied to each design alternative)");
  }

  newGroupHLayout->addWidget(newGroupLabel);

  dropZone = new NewGroupDropZone(t_fixed);
  mainVLayout->addWidget(dropZone);
}

RectangularDropZone::RectangularDropZone(bool t_fixed)
  : QWidget()
{
  setAcceptDrops(true);

  setFixedHeight(30);

  if (t_fixed)
  {
    setStyleSheet("openstudio--measuretab--RectangularDropZone {background: #F4F4F4; border: 2px dashed #A5A5A5;}");
  } else {
    setStyleSheet("openstudio--measuretab--RectangularDropZone {background: #E0E0E0; border: 2px dashed gray;}");
  }

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  mainHLayout->addWidget(nameLabel);
}

void RectangularDropZone::setAcceptedMimeType(const QString & type)
{
  m_acceptedMimeType = type;
}

void RectangularDropZone::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RectangularDropZone::dropEvent(QDropEvent * event)
{
  emit dataDropped(event);
}

void RectangularDropZone::dragEnterEvent(QDragEnterEvent * event)
{
  if(event->mimeData()->hasFormat(m_acceptedMimeType))
  {
    event->accept();
  }
}

NewGroupDropZone::NewGroupDropZone(bool t_fixed)
  : RectangularDropZone(t_fixed)
{
  if (t_fixed)
  {
    nameLabel->setText("<i>Drop Measure From Library to Create a New Always Run Measure</i>");
  } else {
    nameLabel->setText("Drop Measure From Library to Create a New Measure Group"); 
  }
  nameLabel->setStyleSheet("QLabel {color: #7D7D7D; }");
}

VariableHeaderView::VariableHeaderView(bool t_fixed)
  : OSHeader(new HeaderToggleButton()), measureListView(nullptr), m_fixed(t_fixed)
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

  variableNameEdit = new QLineEdit();
  variableNameEdit->setStyleSheet("font:italic");
  variableNameEdit->setAcceptDrops(false);
  variableNameEdit->setValidator(validator);
  mainHLayout->addWidget(variableNameEdit);

  if (t_fixed)
  {
    measureListView = new OSListView();
    measureListView->setContentsMargins(0,0,0,0);
    mainHLayout->addWidget(measureListView);

    toggleButton->setVisible(false);
    variableNameEdit->setVisible(false);
    setStyleSheet("openstudio--measuretab--VariableHeaderView { background: #F4F4F4; border: 2px solid #A5A5A5; }");
  } else {
    setStyleSheet("openstudio--measuretab--VariableHeaderView { background: #D5D5D5;  }");
  }

  removeButton = new SofterRemoveButton();
  mainHLayout->addWidget(removeButton);

  mainHLayout->addSpacing(50);

  downButton = new DownButton();
  mainHLayout->addWidget(downButton);

  upButton = new UpButton();
  mainHLayout->addWidget(upButton);
}


VariableGroupItemView::VariableGroupItemView(bool t_fixedMeasuresOnly, MeasureType measureType)
  : OSCollapsibleView(nullptr)
{
  variableGroupHeader = new LightGradientHeader();
  variableGroupHeader->setMeasureType(measureType);
  setHeader(variableGroupHeader);

  variableGroupContentView = new VariableGroupContentView(t_fixedMeasuresOnly);
  setContent(variableGroupContentView);

  setExpanded(true);
}

VariableGroupContentView::VariableGroupContentView(bool t_fixedMeasuresOnly)
  : QWidget(),
    newGroupView(nullptr),
    newFixedGroupView(nullptr)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,10,0,10);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  // Variable List

  variableListView = new OSListView(false);
  mainVLayout->addWidget(variableListView);

  // New Variable Item

  if (!t_fixedMeasuresOnly)
  {
    newGroupView = new NewGroupView(false, false);
    mainVLayout->addWidget(newGroupView);
  }

  newFixedGroupView = new NewGroupView(true, t_fixedMeasuresOnly);
  mainVLayout->addWidget(newFixedGroupView);
} 

VariableContentView::VariableContentView(bool t_fixed)
  : QWidget()
{
  setStyleSheet("openstudio--measuretab--VariableContentView { background: #D5D5D5; }");

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(40,0,132,5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  measureListView = new OSListView();
  measureListView->setContentsMargins(0,0,0,0);
  mainVLayout->addWidget(measureListView);

  dropZone = new MeasureDropZone();
  mainVLayout->addWidget(dropZone);
}



void VariableContentView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void VariableItemView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


VariableItemView::VariableItemView(bool t_fixed)
  : OSCollapsibleView(nullptr)
{
  setMaximumWidth(650);

  variableContentView = new VariableContentView(t_fixed); 
  setContent(variableContentView);

  variableHeaderView = new VariableHeaderView(t_fixed);
  setHeader(variableHeaderView);
 

  if (t_fixed)
  {
    setExpanded(false);
  } else {
    setExpanded(true);
  }
}


MeasureDropZone::MeasureDropZone()
  : RectangularDropZone(false)
{
  nameLabel->setStyleSheet("QLabel { color: #7D7D7D; }");
  nameLabel->setText("Drop Measure From Library");
}

MeasureItemButton::MeasureItemButton(bool t_fixed)
  : QAbstractButton()
{
  setFixedHeight(25);

  setHasEmphasis(false);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  if (t_fixed)
  {
    nameLabel->setStyleSheet("font:italic");
  }

  mainHLayout->addWidget(nameLabel);

  mainHLayout->addStretch();

  cautionLabel = new QLabel();
  cautionLabel->setPixmap(QPixmap(":/shared_gui_components/images/warning_icon.png"));
  mainHLayout->addWidget(cautionLabel);
  cautionLabel->setVisible(false);
}

void MeasureItemButton::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis )
  {
    setStyleSheet("openstudio--measuretab--MeasureItemButton { background: #FECD60; border: 2px solid #EE641A; }");
  }
  else
  {
    setStyleSheet("openstudio--measuretab--MeasureItemButton {background: #E0E0E0;}");
  }
}

void MeasureItemButton::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MeasureItemView::MeasureItemView(bool t_fixed)
  : QWidget()
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  measureItemButton = new MeasureItemButton(t_fixed);
  mainHLayout->addWidget(measureItemButton);

  duplicateButton = new SofterDuplicateButton();
  mainHLayout->addWidget(duplicateButton);

  removeButton = new SofterRemoveButton();
  mainHLayout->addWidget(removeButton);

  if (t_fixed)
  {
    duplicateButton->setVisible(false);
    removeButton->setVisible(false);
  }
}

void MeasureItemView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // measuretab

} // openstudio

