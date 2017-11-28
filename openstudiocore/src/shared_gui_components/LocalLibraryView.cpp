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

#include "LocalLibraryView.hpp"

#include "Buttons.hpp"
#include "OSViewSwitcher.hpp"

#include "../openstudio_lib/OSItem.hpp"

#include "MeasureBadge.hpp"

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
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(0);
  mainVLayout->setContentsMargins(0,0,0,0);
  setLayout(mainVLayout);

  auto buttonGroup = new QButtonGroup();
  buttonGroup->setExclusive(true);

  mainViewSwitcher = new OSViewSwitcher();
  mainVLayout->addWidget(mainViewSwitcher);

  QString style;
  style.append("QWidget#Footer {");
  style.append("border-top: 1px solid black; ");
  style.append("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #B6B5B6, stop: 1 #737172); ");
  style.append("}");

  auto footer = new QWidget();
  footer->setObjectName("Footer");
  footer->setStyleSheet(style);
  //footer->setFixedHeight(40);
  mainVLayout->addWidget(footer);

  auto footerVLayout = new QVBoxLayout();
  footerVLayout->setContentsMargins(5,5,5,5);
  footerVLayout->setSpacing(5);
  footer->setLayout(footerVLayout);

  auto footerHLayout = new QHBoxLayout();
  footerHLayout->setContentsMargins(0,0,0,0);
  footerHLayout->setSpacing(5);
  footerVLayout->addLayout(footerHLayout);

  duplicateMeasureButton = new DuplicateButton();
  duplicateMeasureButton->setToolTip("Copy Selected Measure and Add to My Measures");
  footerHLayout->addWidget(duplicateMeasureButton);
  duplicateMeasureButton->setDisabled(true);

  addMeasureButton = new AddScriptButton();
  addMeasureButton->setToolTip("Create a Measure from Template and add to My Measures");
  footerHLayout->addWidget(addMeasureButton);
  
  footerHLayout->addStretch();

  myMeasuresFolderButton = new MyMeasuresFolderButton();
  myMeasuresFolderButton->setToolTip("Open the My Measures Directory");
  footerHLayout->addWidget(myMeasuresFolderButton);

  auto footerHLayout2 = new QHBoxLayout();
  footerHLayout2->setContentsMargins(10,0,10,0);
  footerHLayout2->setSpacing(5);
  footerVLayout->addLayout(footerHLayout2);

  addBCLMeasureButton = new BlueButton();
  addBCLMeasureButton->setText("Find Measures on BCL");
  addBCLMeasureButton->setToolTip("Connect to Online BCL to Download New Measures and Update Existing Measures to Library");
  footerHLayout2->addWidget(addBCLMeasureButton);

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

  auto mainHBoxLayout = new QHBoxLayout();
  mainHBoxLayout->setContentsMargins(5,0,5,0);

  setLayout(mainHBoxLayout);

  m_measureTypeBadge = new QLabel(this);
  m_measureTypeBadge->setFixedSize(25,25);
  m_measureTypeBadge->setVisible(false);
  mainHBoxLayout->addWidget(m_measureTypeBadge,Qt::AlignLeft);

  m_measureBadge = new MeasureBadge();
  m_measureBadge->setFixedWidth(25);
  m_measureBadge->setMeasureBadgeType(MeasureBadgeType::MyMeasure);
  
  mainHBoxLayout->addWidget(m_measureBadge,Qt::AlignLeft);

  label = new QLabel("Measure");
  mainHBoxLayout->addWidget(label,Qt::AlignLeft);

  mainHBoxLayout->addStretch();

  errorLabel = new QLabel();
  //errorLabel->setPixmap(QPixmap(":/shared_gui_components/images/error-alert.png"));
  errorLabel->setPixmap(QPixmap(":/shared_gui_components/images/broken_script.png").scaled(24,24));
  errorLabel->setFixedSize(24, 24);
  errorLabel->setVisible(false);
  mainHBoxLayout->addWidget(errorLabel, Qt::AlignRight);
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

