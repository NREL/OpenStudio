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

#include "LocalLibraryView.hpp"
#include "Buttons.hpp"
#include "OSViewSwitcher.hpp"

#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QVBoxLayout>

namespace openstudio{
  

LocalLibraryView::LocalLibraryView(QWidget * parent)
  : QWidget(parent)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(0);
  mainVLayout->setContentsMargins(0,0,0,0);
  setLayout(mainVLayout);

  QWidget * buttonContainer = new QWidget();
  buttonContainer->setFixedHeight(40);
  
  QHBoxLayout * buttonLayout = new QHBoxLayout();
  buttonLayout->setContentsMargins(5,5,5,5);
  buttonLayout->setSpacing(5);
  buttonContainer->setLayout(buttonLayout);

  mainVLayout->addWidget(buttonContainer);

  QButtonGroup * buttonGroup = new QButtonGroup();
  buttonGroup->setExclusive(true);

  bclMeasuresButton = new GrayButton();
  bclMeasuresButton->setCheckable(true);
  buttonGroup->addButton(bclMeasuresButton);
  bclMeasuresButton->setText("BCL Measures");
  buttonLayout->addWidget(bclMeasuresButton);

  myMeasuresButton = new GrayButton();
  myMeasuresButton->setCheckable(true);
  myMeasuresButton->setText("My Measures");
  buttonGroup->addButton(myMeasuresButton);
  buttonLayout->addWidget(myMeasuresButton);

  mainViewSwitcher = new OSViewSwitcher();
  mainVLayout->addWidget(mainViewSwitcher);

  QString style;
  style.append("QWidget#Footer {");
  style.append("border-top: 1px solid black; ");
  style.append("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #B6B5B6, stop: 1 #737172); ");
  style.append("}");

  QWidget * footer = new QWidget();
  footer->setObjectName("Footer");
  footer->setStyleSheet(style);
  //footer->setFixedHeight(40);
  mainVLayout->addWidget(footer);

  QVBoxLayout * footerVLayout = new QVBoxLayout();
  footerVLayout->setContentsMargins(5,5,5,5);
  footerVLayout->setSpacing(5);
  footer->setLayout(footerVLayout);

  QHBoxLayout * footerHLayout = new QHBoxLayout();
  footerHLayout->setContentsMargins(0,0,0,0);
  footerHLayout->setSpacing(5);
  footerVLayout->addLayout(footerHLayout);

  duplicateMeasureButton = new DuplicateButton();
  duplicateMeasureButton->setToolTip("Copy Selected Measure and Add to My Measures");
  footerHLayout->addWidget(duplicateMeasureButton);
  duplicateMeasureButton->setDisabled(true);

  addMeasureButton = new AddScriptButton();
  addMeasureButton->setToolTip("Create New Template Script File");
  footerHLayout->addWidget(addMeasureButton);
  
  footerHLayout->addStretch();

  myMeasuresFolderButton = new MyMeasuresFolderButton();
  myMeasuresFolderButton->setToolTip("Open the My Measures Directory");
  footerHLayout->addWidget(myMeasuresFolderButton);

  QHBoxLayout * footerHLayout2 = new QHBoxLayout();
  footerHLayout2->setContentsMargins(10,0,10,0);
  footerHLayout2->setSpacing(5);
  footerVLayout->addLayout(footerHLayout2);

  addBCLMeasureButton = new BlueButton();
  addBCLMeasureButton->setText("Add Measures From Online BCL"); 
  footerHLayout2->addWidget(addBCLMeasureButton);

  QHBoxLayout * footerHLayout3 = new QHBoxLayout();
  footerHLayout3->setContentsMargins(10,0,10,0);
  footerHLayout3->setSpacing(5);
  footerVLayout->addLayout(footerHLayout3);

  updateMyMeasuresButton = new BlueButton();
  updateMyMeasuresButton->setToolTip("Looks for new and updated measures in your My Measures directory and updates instances of them in your current project");
  updateMyMeasuresButton->setText("Update All of My Measures");
  footerHLayout3->addWidget(updateMyMeasuresButton);

  updateBCLMeasuresButton = new BlueButton();
  updateBCLMeasuresButton->setToolTip("Updates instances of BCL measures in your current project");
  updateBCLMeasuresButton->setText("Update All BCL Measures");
  footerHLayout3->addWidget(updateBCLMeasuresButton);
}

LibraryGroupItemHeader::LibraryGroupItemHeader()
  : LightGradientHeader()
{
  countLabel = new QLabel();

  qobject_cast<QHBoxLayout *>(layout())->addStretch();

  layout()->addWidget(countLabel);
}

void LibraryGroupItemHeader::setCount(int count)
{
  QString text = count > 0 ? QString::number(count) : QString();
  countLabel->setText(text);
}

LibrarySubGroupItemHeader::LibrarySubGroupItemHeader()
  : LightHeader()
{
  countLabel = new QLabel();

  qobject_cast<QHBoxLayout *>(layout())->addStretch();

  layout()->addWidget(countLabel);
}

void LibrarySubGroupItemHeader::setCount(int count)
{
  QString text = count > 0 ? QString::number(count) : QString();
  countLabel->setText(text);
}

LibraryItemView::LibraryItemView(QWidget * parent)
  : OSDragableView(parent)
{
  setFixedHeight(35);

  setObjectName("Container");
  setHasEmphasis(false);

  QHBoxLayout * mainHBoxLayout = new QHBoxLayout();
  mainHBoxLayout->setContentsMargins(5,0,5,0);

  setLayout(mainHBoxLayout);

  m_measureTypeBadge = new QLabel(this);
  m_measureTypeBadge->setFixedSize(25,25);
  m_measureTypeBadge->setVisible(false);
  mainHBoxLayout->addWidget(m_measureTypeBadge,Qt::AlignLeft);

  m_bclBadge = new QLabel("BCL",this);
  m_bclBadge->setFixedWidth(25);
  m_bclBadge->setStyleSheet("QLabel { color: #4B7DB0; font-size: 10pt; }");
  m_bclBadge->setVisible(false);
  mainHBoxLayout->addWidget(m_bclBadge,Qt::AlignLeft);

  label = new QLabel("Measure");
  mainHBoxLayout->addWidget(label,Qt::AlignLeft);

  mainHBoxLayout->addStretch();
}

void LibraryItemView::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis )
  {
    setStyleSheet("QWidget#Container { background: #FECD60; border: 2px solid #EE641A; }");
  }
  else
  {
    setStyleSheet("QWidget#Container { background: #D5D5D5; border: 2px solid #A9A9A9; }");
  }
}

void LibraryItemView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // openstudio

