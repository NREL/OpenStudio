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

#include <openstudio_lib/OSDropZone.hpp>
#include <openstudio_lib/OSVectorController.hpp>
#include <openstudio_lib/IconLibrary.hpp>

#include <model/Model_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QLayoutItem>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QStyleOption>
#include <QTimer>

using namespace openstudio::model;

namespace openstudio {

OSDropZone::OSDropZone(OSVectorController* vectorController,
                       bool growsHorizontally,
                       QWidget * parent )
  : QWidget(parent),
    m_vectorController(vectorController),
    m_minItems(0),
    m_itemsDraggable(false),
    m_itemsAcceptDrops(false), 
    m_itemsRemoveable(true),
    m_allowAdd(false),
    m_growsHorizontally(growsHorizontally),
    m_useLargeIcon(false)
{
  QWidget * mainBox = new QWidget();
  mainBox->setObjectName("mainBox");
  mainBox->setStyleSheet("QWidget#mainBox { background: #CECECE; }");

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setWidget(mainBox);
  m_scrollArea->setMinimumWidth(OSItem::ITEM_WIDTH);

  QBoxLayout * mainLayout = NULL;

  if(m_growsHorizontally){
    m_scrollArea->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mainLayout = new QHBoxLayout();
    m_mainBoxLayout = new QHBoxLayout();
  }
  else{
    m_scrollArea->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    mainLayout = new QVBoxLayout();
    m_mainBoxLayout = new QVBoxLayout();
  }

  mainLayout->setContentsMargins(10,10,10,10);
  setLayout(mainLayout);

  mainLayout->addWidget(m_scrollArea);

 // for now we will allow this drop zone to manage memory of the vectorController
  OS_ASSERT(!m_vectorController->parent());
  m_vectorController->setParent(this);

  m_addButton = new QPushButton(this);
  m_addButton->setFlat(true);
  m_addButton->setObjectName("AddButton");
  m_addButton->setFixedSize(24,24);
  mainLayout->addWidget(m_addButton,0,Qt::AlignBottom);

  setObjectName("OSDropZone");
  QString mainBoxStyle;
  mainBoxStyle.append("QWidget#OSDropZone {");
  mainBoxStyle.append(" background: #CECECE;");
  mainBoxStyle.append(" border: 2px dashed #808080;");
  mainBoxStyle.append(" border-radius: 10px;");
  mainBoxStyle.append("}");
  setStyleSheet(mainBoxStyle);

  m_mainBoxLayout->setContentsMargins(0,0,0,0);
  m_mainBoxLayout->setSpacing(10);
  mainBox->setLayout(m_mainBoxLayout);

  bool isConnected = false;

  isConnected = connect(m_addButton,SIGNAL(clicked()),this,SIGNAL(addButtonClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemsRequested()),
                        vectorController, SLOT(reportItems()),
                        Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemRemoveClicked(OSItem*)),
                        vectorController, SLOT(removeItem(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                        vectorController, SLOT(replaceItem(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemDropped(const OSItemId&)),
                        vectorController, SLOT(drop(const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(addButtonClicked()),
                        vectorController, SLOT(makeNewItem()));
  OS_ASSERT(isConnected);

  isConnected = connect(vectorController, SIGNAL(itemIds(const std::vector<OSItemId>&)),
                        this, SLOT(setItemIds(const std::vector<OSItemId>&)),
                        Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  emit itemsRequested();

  setMaxItems(std::numeric_limits<int>::max());

  hideAddButton();
}

void OSDropZone::paintEvent( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int OSDropZone::maxItems() const
{
  return m_maxItems;
}

bool OSDropZone::setMaxItems(int max)
{
  if (max >= m_minItems && max >= 0){
    m_maxItems = max;
    emit itemsRequested();
    if( max == 1 )
    {
      if(m_growsHorizontally){
        m_scrollArea->setFixedHeight(OSItem::ITEM_HEIGHT);
        m_scrollArea->setMaximumWidth(OSItem::ITEM_WIDTH);
        setMaximumWidth(OSItem::ITEM_WIDTH + 20);
      }
      else{
        m_scrollArea->setMaximumHeight(OSItem::ITEM_HEIGHT);
        m_scrollArea->setFixedWidth(OSItem::ITEM_WIDTH);
        setMaximumHeight(OSItem::ITEM_HEIGHT + 20);
      }
    }
    else
    {
      if(m_growsHorizontally){
        m_scrollArea->setFixedHeight(OSItem::ITEM_SIDE);
        m_scrollArea->setMaximumWidth(QWIDGETSIZE_MAX);
        setMaximumWidth(QWIDGETSIZE_MAX);
      }
      else{
        m_scrollArea->setFixedWidth(OSItem::ITEM_WIDTH);
        m_scrollArea->setMaximumHeight(QWIDGETSIZE_MAX);
        setMaximumHeight(QWIDGETSIZE_MAX);
      }

    }
    return true;
  }
  return false;
}

int OSDropZone::minItems() const
{
  return m_minItems;
}

bool OSDropZone::setMinItems(int min)
{
  if (min <= m_maxItems && min >= 0){
    m_minItems = min;
    emit itemsRequested();
    return true;
  }
  return false;
}

bool OSDropZone::itemsDraggable() const
{
  return m_itemsDraggable;
}

void OSDropZone::setItemsDraggable(bool itemsDraggable)
{
  m_itemsDraggable = itemsDraggable;
}

bool OSDropZone::itemsAcceptDrops() const
{
  return m_itemsAcceptDrops;
}

void OSDropZone::setItemsAcceptDrops(bool itemsAcceptDrops)
{
  m_itemsAcceptDrops = itemsAcceptDrops;
}

bool OSDropZone::itemsRemoveable() const
{
  return m_itemsRemoveable;
}

void OSDropZone::setItemsRemoveable(bool itemsRemoveable)
{
  m_itemsRemoveable = itemsRemoveable;
}

void OSDropZone::onDrop(const OSItemId& itemId)
{
  emit itemDropped(itemId);
}

void OSDropZone::setItemIds(const std::vector<OSItemId>& itemIds)
{
  QLayoutItem* child;
  while( (child = m_mainBoxLayout->takeAt(0)) != 0 ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }

  int numItems = 0;
  OSItem::Type type;
  if((this->m_maxItems > 1) && (m_growsHorizontally)){
    type = OSItem::DROPZONE_SQUARE;
    // somewhere set show / hide scroll bar TODO
  }
  else{
    type = OSItem::DROPZONE_RECTANGLE;
  }

  BOOST_FOREACH(const OSItemId& itemId, itemIds){

    OSItem* item = OSItem::makeItem(itemId, type);
    if (!item){
      continue;
    }

    ++numItems;

    bool isConnected = false;
    isConnected = connect( item,SIGNAL(itemRemoveClicked(OSItem*)),
                           this,SIGNAL(itemRemoveClicked(OSItem*)) );
    OS_ASSERT(isConnected);

    isConnected = connect( item,SIGNAL(itemClicked(OSItem*)),
                           this,SIGNAL(itemClicked(OSItem*)) );
    OS_ASSERT(isConnected);

    isConnected = connect( item,SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                           this,SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)) );
    OS_ASSERT(isConnected);

    item->setDraggable(m_itemsDraggable);

    item->setAcceptDrops(m_itemsAcceptDrops);

    item->setUseLargeIcon(m_useLargeIcon);

    if (item->removeable()){
      if (numItems > m_minItems){
        item->setRemoveable(true);
      }else{
        item->setRemoveable(false);
      }
    }

    if (!m_itemsRemoveable){
      item->setRemoveable(false);
    }

    m_mainBoxLayout->addWidget(item,0,Qt::AlignLeft);
  }

  if (numItems < m_maxItems){
    OSItemDropZone* dropZone = new OSItemDropZone(this->m_growsHorizontally);
    m_mainBoxLayout->addWidget(dropZone,0,Qt::AlignLeft);

    bool isConnected = connect(dropZone, SIGNAL(dropped(QDropEvent*)), this, SLOT(handleDrop(QDropEvent*)));
    OS_ASSERT(isConnected);

    if( m_maxItems == 1 )
    {
      dropZone->setExtensible(false);
    }else{
      dropZone->setExtensible(true);
    }

    if (m_allowAdd){
      this->showAddButton();
    }

  }else{
    this->hideAddButton();
  }

  m_mainBoxLayout->addStretch();
}

void OSDropZone::handleDrop(QDropEvent* event)
{
  OSItemId itemId(event->mimeData());
  this->onDrop(itemId);
}

void OSDropZone::showAddButton()
{
  m_allowAdd = true;
  m_addButton->show();
}

void OSDropZone::hideAddButton()
{
  m_addButton->hide();
}

bool OSDropZone::useLargeIcon()
{
  return m_useLargeIcon;
}

void OSDropZone::setUseLargeIcon(bool useLargeIcon)
{
  m_useLargeIcon = useLargeIcon;
}

OSItemDropZone::OSItemDropZone(bool growsHorizontally,
                               QWidget * parent)
  : QWidget(parent),
  m_growsHorizontally(growsHorizontally)
{
  setAcceptDrops(true);

  setObjectName("DropBox");

  QBoxLayout * mainLayout = NULL;

  if(m_growsHorizontally){
    mainLayout = new QHBoxLayout();
  }
  else{
    mainLayout = new QVBoxLayout();
  }
  mainLayout->setContentsMargins(10,10,10,10);
  setLayout(mainLayout);

  QLabel * label = new QLabel();
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setText("Drag From Library");
  label->setWordWrap(true);

  label->setStyleSheet("QLabel { font: bold; color: #808080}");

  mainLayout->addWidget(label,0,Qt::AlignCenter);

  setExtensible(false);
}

void OSItemDropZone::setExtensible(bool extensible)
{
  QString style;

  if( extensible )
  {
    style.append("QWidget#DropBox {");
    style.append(" background: #CECECE;");
    style.append(" border: 2px dashed #808080;");
    style.append(" border-radius: 10px;");
    style.append("}");

    if(m_growsHorizontally){
      setFixedWidth(OSItem::ITEM_SIDE);
      setFixedHeight(OSItem::ITEM_SIDE);
    }
    else{
      setFixedWidth(OSItem::ITEM_WIDTH);
      setFixedHeight(OSItem::ITEM_HEIGHT);
    }
  }
  else
  {
    style.append("QWidget#DropBox {");
    style.append(" background: #CECECE;");
    style.append(" border: none;");
    style.append("}");

    setFixedWidth(OSItem::ITEM_WIDTH);
    setFixedHeight(OSItem::ITEM_HEIGHT);
  }

  setStyleSheet(style);
}

void OSItemDropZone::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OSItemDropZone::dragEnterEvent(QDragEnterEvent *event)
{
  if(event->proposedAction() == Qt::CopyAction){
    event->accept();
  }
}

void OSItemDropZone::dropEvent(QDropEvent *event)
{
  event->accept();
  if(event->proposedAction() == Qt::CopyAction){
    emit dropped(event);
  }
}

} // openstudio
