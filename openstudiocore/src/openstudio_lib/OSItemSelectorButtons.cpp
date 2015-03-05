/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "OSItemSelectorButtons.hpp"
#include "OSDropZone.hpp"
#include "OSItem.hpp"
#include "OSVectorController.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QRadioButton>

#include <iostream>

namespace openstudio {

class AlwaysEmptyDropZoneVectorController : public OSVectorController
{
protected:
  virtual std::vector<OSItemId> makeVector() { return std::vector<OSItemId>(); }
};

OSItemSelectorButtons::OSItemSelectorButtons(QWidget * parent)
  : QWidget(parent)
{
  this->setObjectName("OSItemSelectorButtons");
  this->setStyleSheet("QWidget#OSItemSelectorButtons { background: #E6E6E6; border-top: 1px solid black; }");

  m_vLayout = new QVBoxLayout();
  m_vLayout->setContentsMargins(0,0,0,0);
  m_vLayout->setSpacing(0);
  this->setLayout(m_vLayout);

  // drop zone
  
  m_dropZoneLayout = new QHBoxLayout();
  m_dropZoneController = new AlwaysEmptyDropZoneVectorController();
  m_dropZone = new OSDropZone(m_dropZoneController);
  m_dropZone->setMaxItems(1);
  m_dropZoneLayout->addWidget(m_dropZone);
  m_vLayout->addLayout(m_dropZoneLayout);
  m_dropZoneLayout->setContentsMargins(10,10,10,10);

  connect(m_dropZone, &OSDropZone::itemDropped, this, &OSItemSelectorButtons::itemDropped);
    
  // buttons
  QWidget * buttonBox = new QWidget();
  buttonBox->setObjectName("ButtonBox");
  buttonBox->setStyleSheet("QWidget#ButtonBox { background: #808080; border-top: 1px solid black; }");
  m_vLayout->addWidget(buttonBox);

  QHBoxLayout * buttonLayout = new QHBoxLayout();
  buttonLayout->setContentsMargins(10,10,10,10);
  buttonLayout->setSpacing(5);
  buttonBox->setLayout(buttonLayout);

  m_addButton = new QPushButton();
  m_addButton->setFlat(true);
  m_addButton->setObjectName("AddButton");
  m_addButton->setToolTip("Add new object");
  m_addButton->setFixedSize(24,24);
  buttonLayout->addWidget(m_addButton);
  
  connect(m_addButton, &QPushButton::clicked, this, &OSItemSelectorButtons::addClicked);

  m_copyButton = new QPushButton();
  m_copyButton->setEnabled(false);
  m_copyButton->setFlat(true);
  m_copyButton->setObjectName("CopyButton");
  m_copyButton->setToolTip("Copy selected object");
  m_copyButton->setFixedSize(24,24);
  buttonLayout->addWidget(m_copyButton);
  
  connect(m_copyButton, &QPushButton::clicked, this, &OSItemSelectorButtons::copyClicked);

  m_removeButton = new QPushButton();
  m_removeButton->setEnabled(false);
  m_removeButton->setFlat(true);
  m_removeButton->setObjectName("DeleteButton");
  m_removeButton->setToolTip("Remove selected objects");
  m_removeButton->setFixedSize(24,24);
  buttonLayout->addWidget(m_removeButton);
  
  connect(m_removeButton, &QPushButton::clicked, this, &OSItemSelectorButtons::removeClicked);

  buttonLayout->addStretch();

  m_purgeButton = new QPushButton();
  m_purgeButton->setFlat(true);
  m_purgeButton->setObjectName("PurgeButton");
  m_purgeButton->setToolTip("Purge unused objects");
  m_purgeButton->setFixedSize(24,24);
  buttonLayout->addWidget(m_purgeButton);
  
  connect(m_purgeButton, &QPushButton::clicked, this, &OSItemSelectorButtons::purgeClicked);

  //m_openBclDlgButton = new QPushButton(this);
  //m_openBclDlgButton->setObjectName("OpenBclDlgButton");
  //m_openBclDlgButton->setText("Online BCL");
  //m_openBclDlgButton->hide();
  //m_vLayout->addWidget(m_openBclDlgButton);
  //
  //connect(m_openBclDlgButton, &QPushButton::clicked, this, &OSItemSelectorButtons::downloadComponentsClicked);
}

void OSItemSelectorButtons::showDropZone()
{
  m_dropZone->show();
  m_dropZoneLayout->setContentsMargins(10,10,10,10);
}
void OSItemSelectorButtons::hideDropZone()
{
  m_dropZone->hide();
  m_dropZoneLayout->setContentsMargins(0,0,0,0);
}
void OSItemSelectorButtons::enableDropZone()
{
  m_dropZone->setEnabled(true);
}
void OSItemSelectorButtons::disableDropZone()
{
  m_dropZone->setEnabled(false);
}
   
void OSItemSelectorButtons::showAddButton()
{
  m_addButton->show();
}
void OSItemSelectorButtons::hideAddButton()
{
  m_addButton->hide();
}
void OSItemSelectorButtons::enableAddButton()
{
  m_addButton->setEnabled(true);
}
void OSItemSelectorButtons::disableAddButton()
{
  m_addButton->setEnabled(false);
}

void OSItemSelectorButtons::showCopyButton()
{
  m_copyButton->show();
}
void OSItemSelectorButtons::hideCopyButton()
{
  m_copyButton->hide();
}
void OSItemSelectorButtons::enableCopyButton()
{
  m_copyButton->setEnabled(true);
}
void OSItemSelectorButtons::disableCopyButton()
{
  m_copyButton->setEnabled(false);
}

void OSItemSelectorButtons::showRemoveButton()
{
  m_removeButton->show();
}
void OSItemSelectorButtons::hideRemoveButton()
{
  m_removeButton->hide();
}
void OSItemSelectorButtons::enableRemoveButton()
{
  m_removeButton->setEnabled(true);
}
void OSItemSelectorButtons::disableRemoveButton()
{
  m_removeButton->setEnabled(false);
}

void OSItemSelectorButtons::showPurgeButton()
{
  m_purgeButton->show();
}
void OSItemSelectorButtons::hidePurgeButton()
{
  m_purgeButton->hide();
}
void OSItemSelectorButtons::enablePurgeButton()
{
  m_purgeButton->setEnabled(true);
}
void OSItemSelectorButtons::disablePurgeButton()
{
  m_purgeButton->setEnabled(false);
}
  
void OSItemSelectorButtons::showBclDlgButton()
{
  if(m_openBclDlgButton) m_openBclDlgButton->show();
}
void OSItemSelectorButtons::hideBclDlgButton()
{
  if(m_openBclDlgButton) m_openBclDlgButton->hide();
}
void OSItemSelectorButtons::enableBclDlgButton()
{
  if(m_openBclDlgButton) m_openBclDlgButton->setEnabled(true);
}
void OSItemSelectorButtons::disableBclDlgButton()
{
  if(m_openBclDlgButton) m_openBclDlgButton->setEnabled(false);
}

void OSItemSelectorButtons::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // openstudio
