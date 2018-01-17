/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
    m_mainLayout(nullptr),
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
  connect(m_filtersOnBtn, &QRadioButton::clicked, this, &OSCollapsibleItem::filtersOnClicked);

  m_filtersOffBtn = new QRadioButton(this);
  m_filtersOffBtn->setText(FILTERS_OFF);
  connect(m_filtersOffBtn, &QRadioButton::clicked, this, &OSCollapsibleItem::filtersOffClicked);

  m_filterBtnGroup = new QButtonGroup(this);
  m_filterBtnGroup->setExclusive(true);
  m_filterBtnGroup->addButton(m_filtersOnBtn);
  m_filterBtnGroup->addButton(m_filtersOffBtn);

  auto radioBtnVLayout = new QVBoxLayout();
  radioBtnVLayout->addWidget(m_filtersOnBtn);
  radioBtnVLayout->addWidget(m_filtersOffBtn);

  // RADIO BUTTONS AND PUSHBUTTON

  m_openLibDlgButton = new QPushButton(this);
  m_openLibDlgButton->setObjectName("OpenLibDlgButton");
  m_openLibDlgButton->setLayoutDirection(Qt::RightToLeft);
  m_openLibDlgButton->setText("Edit");
  connect(m_openLibDlgButton, &QPushButton::clicked, this, &OSCollapsibleItem::openLibDlgClicked);

  auto btnHLayout = new QHBoxLayout();
  btnHLayout->addLayout(radioBtnVLayout);
  btnHLayout->addWidget(m_openLibDlgButton);

  // BUTTONS AND COMBOBOX

  m_sortLabel = new QLabel("Sort by:", this);

  m_sortComboBox = new QComboBox(this);
  m_sortComboBox->setFocusPolicy(Qt::ClickFocus);
  m_sortComboBox->addItem("Relevance");
  m_sortComboBox->addItem("Recently Downloaded from BCL");
  m_sortComboBox->addItem("BCL Components");
  connect(m_sortComboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &OSCollapsibleItem::comboBoxClicked);

  auto vLayout = new QVBoxLayout();
  vLayout->addLayout(btnHLayout);
  vLayout->addWidget(m_sortLabel);
  vLayout->addWidget(m_sortComboBox);

  m_filterWidget->setLayout(vLayout);

  m_mainLayout->addWidget(m_filterWidget);

  connect(collapsibleItemHeader, &OSCollapsibleItemHeader::clicked,
          this, &OSCollapsibleItem::onHeaderClicked);

  // item list
  m_mainLayout->addWidget(m_itemList);

  connect(itemList, &OSItemList::itemSelected, this, &OSCollapsibleItem::itemSelected);

  connect(itemList, &OSItemList::itemRemoveClicked, this, &OSCollapsibleItem::itemRemoveClicked);

  connect(itemList, &OSItemList::itemReplacementDropped, this, &OSCollapsibleItem::itemReplacementDropped);

  connect(itemList, &OSItemList::selectionCleared, this, &OSCollapsibleItem::selectionCleared);

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
