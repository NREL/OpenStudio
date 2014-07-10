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

#include "OSCollapsibleItem.hpp"

#include "OSCollapsibleItemHeader.hpp"
#include "OSItemList.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QStyleOption>

#define FILTERS_ON  "Filters On "
#define FILTERS_OFF "Filters Off "

namespace openstudio {

OSCollapsibleItem::OSCollapsibleItem(OSCollapsibleItemHeader * collapsibleItemHeader,
                                     OSItemList * itemList,
                                     QWidget * parent)
  : QWidget(parent),
    m_collapsibleItemHeader(collapsibleItemHeader),
    m_itemList(itemList),
    m_mainLayout(NULL),
    m_showFilterLayout(false)
{
  OS_ASSERT(m_collapsibleItemHeader);
  OS_ASSERT(m_itemList);

  //m_collapsibleItemHeader->setDraggable(false);
  //m_collapsibleItemHeader->setRemoveable(false);

  setObjectName("OSCollapsibleItem");

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->setSpacing(0);
  m_mainLayout->setContentsMargins(0,0,0,0);
  setLayout(m_mainLayout);

  // collapsible header
  m_mainLayout->addWidget(m_collapsibleItemHeader);

  bool isConnected = false;

  m_filterWidget = new QWidget();
  m_filterWidget->setObjectName("GrayWidget");

  QString style;
  style.append("QWidget#GrayWidget {");
  style.append(" background: #E6E6E6;");
  style.append(" border-bottom: 1px solid black;");
  style.append("}");
  m_filterWidget->setStyleSheet(style);

  // RADIO BUTTONS

  m_filtersOnBtn = new QRadioButton(this);
  m_filtersOnBtn->setText(FILTERS_ON);
  m_filtersOnBtn->setChecked(true);
  isConnected = connect(m_filtersOnBtn,SIGNAL(clicked()),this,SLOT(filtersOnClicked()));
  OS_ASSERT(isConnected);

  m_filtersOffBtn = new QRadioButton(this);
  m_filtersOffBtn->setText(FILTERS_OFF);
  isConnected = connect(m_filtersOffBtn,SIGNAL(clicked()),this,SLOT(filtersOffClicked()));
  OS_ASSERT(isConnected);

  m_filterBtnGroup = new QButtonGroup(this);
  m_filterBtnGroup->setExclusive(true);
  m_filterBtnGroup->addButton(m_filtersOnBtn);
  m_filterBtnGroup->addButton(m_filtersOffBtn);

  QVBoxLayout * radioBtnVLayout = new QVBoxLayout();
  radioBtnVLayout->addWidget(m_filtersOnBtn);
  radioBtnVLayout->addWidget(m_filtersOffBtn);

  // RADIO BUTTONS AND PUSHBUTTON

  m_openLibDlgButton = new QPushButton(this);
  m_openLibDlgButton->setObjectName("OpenLibDlgButton");
  m_openLibDlgButton->setLayoutDirection(Qt::RightToLeft);
  m_openLibDlgButton->setText("Edit");
  isConnected = connect(m_openLibDlgButton,SIGNAL(clicked()),this,SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  QHBoxLayout * btnHLayout = new QHBoxLayout();
  btnHLayout->addLayout(radioBtnVLayout);
  btnHLayout->addWidget(m_openLibDlgButton);

  // BUTTONS AND COMBOBOX

  m_sortLabel = new QLabel("Sort by:", this);

  m_sortComboBox = new QComboBox(this);
  m_sortComboBox->setFocusPolicy(Qt::ClickFocus);
  m_sortComboBox->addItem("Relevance");
  m_sortComboBox->addItem("Recently Downloaded from BCL");
  m_sortComboBox->addItem("BCL Components");
  isConnected = connect(m_sortComboBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(comboBoxClicked(const QString &)));
  OS_ASSERT(isConnected);

  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->addLayout(btnHLayout);
  vLayout->addWidget(m_sortLabel);
  vLayout->addWidget(m_sortComboBox);

  m_filterWidget->setLayout(vLayout);

  m_mainLayout->addWidget(m_filterWidget);

  isConnected = connect(collapsibleItemHeader, SIGNAL(clicked(OSCollapsibleItemHeader *)),
                        this, SLOT(onHeaderClicked(OSCollapsibleItemHeader *)));
  OS_ASSERT(isConnected);

  // item list
  m_mainLayout->addWidget(m_itemList);
  
  isConnected = connect(itemList, SIGNAL(itemSelected(OSItem*)),
                        this, SIGNAL(itemSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(itemList, SIGNAL(itemRemoveClicked(OSItem*)),
                        this, SIGNAL(itemRemoveClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(itemList, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                        this, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(itemList, SIGNAL(selectionCleared()),
                        this, SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  m_mainLayout->addStretch();

  setShowFilterWidgets(m_showFilterLayout);

  setExpanded(false);
}

OSCollapsibleItemHeader * OSCollapsibleItem::collapsibleItemHeader() const
{
  return m_collapsibleItemHeader;
}

OSItemList * OSCollapsibleItem::itemList() const
{
  return m_itemList;
}

bool OSCollapsibleItem::itemsDraggable() const
{
  return m_itemList->itemsDraggable();
}

void OSCollapsibleItem::setItemsDraggable(bool itemsDraggable)
{
  m_itemList->setItemsDraggable(itemsDraggable);
}

bool OSCollapsibleItem::itemsRemoveable() const
{
  return m_itemList->itemsRemoveable();
}

void OSCollapsibleItem::setItemsRemoveable(bool itemsRemoveable)
{
  m_itemList->setItemsRemoveable(itemsRemoveable);
}

OSItemType OSCollapsibleItem::itemsType() const
{
  return m_itemList->itemsType();
}

void OSCollapsibleItem::setItemsType(OSItemType type)
{
  m_itemList->setItemsType(type);
}

bool OSCollapsibleItem::isSelected() const
{
  return this->m_collapsibleItemHeader->selected();
}

void OSCollapsibleItem::setSelected(bool selected)
{
  this->m_collapsibleItemHeader->setSelected(selected);
  if (selected){
    emit collapsableItemSelected(this);
  }
}

bool OSCollapsibleItem::expanded() const
{
  return this->m_collapsibleItemHeader->expanded();
}

void OSCollapsibleItem::setExpanded(bool expanded)
{
  this->m_collapsibleItemHeader->setExpanded(expanded);
  this->m_itemList->setVisible(expanded);
  if(m_showFilterLayout){
    setShowFilterWidgets(expanded);
  }
}

void OSCollapsibleItem::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool OSCollapsibleItem::showFilterLayout()
{
  return m_showFilterLayout;
}

void OSCollapsibleItem::setShowFilterLayout(const bool showFilterLayout)
{
  // TODO Hack to remove once we do stuff with the filters
  return;

  m_showFilterLayout = showFilterLayout;

  setShowFilterWidgets(showFilterLayout);
}

void OSCollapsibleItem::setShowFilterWidgets(const bool showFilterWidgets)
{
  //this->m_filtersOnBtn->setVisible(showFilterWidgets);
  //this->m_filtersOffBtn->setVisible(showFilterWidgets);
  //this->m_openLibDlgButton->setVisible(showFilterWidgets);
  //this->m_sortLabel->setVisible(showFilterWidgets);
  //this->m_sortComboBox->setVisible(showFilterWidgets);
  m_filterWidget->setVisible(showFilterWidgets);
}

void OSCollapsibleItem::onHeaderClicked(OSCollapsibleItemHeader * header)
{
  setExpanded(!expanded());
  setSelected(true);
}

void OSCollapsibleItem::filtersOnClicked()
{
  // TODO
}

void OSCollapsibleItem::filtersOffClicked()
{
  // TODO
}

void OSCollapsibleItem::comboBoxClicked(const QString & string)
{
  // TODO
}

} // openstudio
