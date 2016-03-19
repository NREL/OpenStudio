/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "OSDropZone.hpp"

#include "IconLibrary.hpp"
#include "InspectorController.hpp"
#include "InspectorView.hpp"
#include "MainRightColumnController.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "OSVectorController.hpp"

#include "../model/ModelObject_Impl.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QApplication>
#include <QBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFocusEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
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
                       const QString & text,
                       const QSize & size,
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
    m_useLargeIcon(false),
    m_text(text),
    m_size(size)
{
  auto mainBox = new QWidget();
  mainBox->setObjectName("mainBox");
  mainBox->setStyleSheet("QWidget#mainBox { background: #CECECE; }");

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setWidget(mainBox);
  m_scrollArea->setMinimumWidth(OSItem::ITEM_WIDTH);

  QBoxLayout * mainLayout = nullptr;

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
  if(m_size.height() && m_size.width()){ 
    mainLayout->setContentsMargins(0,0,0,0);
    mainBoxStyle.append(" border: none;"); 
  } else {
    mainBoxStyle.append(" border: 2px dashed #808080;");
    mainBoxStyle.append(" border-radius: 10px;"); 
  }
  mainBoxStyle.append("}");
  setStyleSheet(mainBoxStyle);

  m_mainBoxLayout->setContentsMargins(0,0,0,0);
  m_mainBoxLayout->setSpacing(10);
  mainBox->setLayout(m_mainBoxLayout);

  connect(m_addButton, &QPushButton::clicked, this, &OSDropZone::addButtonClicked);

  connect(this, &OSDropZone::itemsRequested, vectorController, &OSVectorController::reportItems, Qt::QueuedConnection);

  connect(this, &OSDropZone::itemRemoveClicked, vectorController, &OSVectorController::removeItem);

  connect(this, &OSDropZone::itemReplacementDropped, vectorController, &OSVectorController::replaceItem);

  connect(this, &OSDropZone::itemDropped, vectorController, &OSVectorController::drop);

  connect(this, &OSDropZone::addButtonClicked, vectorController, &OSVectorController::makeNewItem);

  connect(vectorController, &OSVectorController::itemIds, this, &OSDropZone::setItemIds, Qt::QueuedConnection);

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
      if(m_size.height() && m_size.width())
      { 
        m_scrollArea->setFixedHeight(m_size.height());
        m_scrollArea->setMaximumWidth(m_size.width());
        setMaximumHeight(m_size.height());
        setMaximumWidth(m_size.width());
      }
      else if(m_growsHorizontally){
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
      if(m_size.height() && m_size.width())
      {
        m_scrollArea->setFixedHeight(m_size.height());
        m_scrollArea->setMaximumWidth(m_size.width());
        setMaximumHeight(m_size.height());
        setMaximumWidth(m_size.width());
      }
      else if(m_growsHorizontally){
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
  while( (child = m_mainBoxLayout->takeAt(0)) != nullptr ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }

  int numItems = 0;
  OSItemType type;
  if((this->m_maxItems > 1) && (m_growsHorizontally)){
    type = OSItemType::DropzoneSquare;
    // somewhere set show / hide scroll bar TODO
  }
  else{
    type = OSItemType::DropzoneRectangle;
  }

  for (const OSItemId& itemId : itemIds){

    OSItem* item = OSItem::makeItem(itemId, type);
    if (!item){
      continue;
    }

    ++numItems;

    connect(item, &OSItem::itemRemoveClicked, this, &OSDropZone::itemRemoveClicked);

    connect(item, &OSItem::itemClicked, this, &OSDropZone::itemClicked);

    connect(item, &OSItem::itemReplacementDropped, this, &OSDropZone::itemReplacementDropped);

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
    auto dropZone = new OSItemDropZone(this->m_growsHorizontally, m_text, m_size);
    m_mainBoxLayout->addWidget(dropZone,0,Qt::AlignLeft);

    connect(dropZone, &OSItemDropZone::dropped, this, &OSDropZone::handleDrop);

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
                               const QString & text,
                               const QSize & size,
                               QWidget * parent)
  : QWidget(parent),
  m_size(size),
  m_growsHorizontally(growsHorizontally)
{
  setAcceptDrops(true);

  setObjectName("DropBox");

  QBoxLayout * mainLayout = nullptr;

  if(m_growsHorizontally){
    mainLayout = new QHBoxLayout();
  }
  else{
    mainLayout = new QVBoxLayout();
  }
  mainLayout->setContentsMargins(10,10,10,10);
  setLayout(mainLayout);

  auto label = new QLabel();
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  label->setText(text);
  label->setWordWrap(true);

  label->setStyleSheet("QLabel { font: bold; color: #808080}");

  mainLayout->addWidget(label,0,Qt::AlignCenter);

  setExtensible(false);
}

void OSItemDropZone::setExtensible(bool extensible)
{
  QString style;

  if(m_size.height() && m_size.width())
  {
    style.append("QWidget#DropBox {");
    style.append(" background: #CECECE;");
    style.append(" border: 2px dashed #808080;");
    style.append(" border-radius: 3px;");
    style.append("}");

    setFixedWidth(m_size.width());
    setFixedHeight(m_size.height()); 
  }
  else if( extensible )
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

OSDropZoneItem::OSDropZoneItem()
  : QGraphicsObject(), 
    m_mouseDown(false)
{
  setAcceptHoverEvents(true);
  setAcceptDrops(true);

  setSize(100,50);
}

void OSDropZoneItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  event->accept();

  if(event->proposedAction() == Qt::CopyAction)
  {
    OSItemId id = OSItemId(event->mimeData());

    emit componentDropped(id);
  }
}

QRectF OSDropZoneItem::boundingRect() const
{
  return QRectF(0,0,m_width,m_height);
}

void OSDropZoneItem::setSize(double width, double height)
{
  prepareGeometryChange();
  m_width = width;
  m_height = height;
}

void OSDropZoneItem::setText(const QString & text)
{
  m_text = text;
  update();
}

void OSDropZoneItem::paint( QPainter *painter, 
                                                 const QStyleOptionGraphicsItem *option, 
                                                 QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));

  painter->drawRect(boundingRect());

  QFont font = painter->font();
  font.setPointSize(25);
  painter->setFont(font);
  painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));
  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,m_text);
}

void OSDropZoneItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  this->update();

  event->accept();
}

void OSDropZoneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      emit mouseClicked();
    }
  }
}

OSDropZone2::OSDropZone2()
  : QWidget()
{
  setObjectName("OSDropZone");

  QString style("QWidget#OSDropZone {\
    background: #CECECE;\
    border: 2px dashed #808080;\
    border-radius: 5px; }");
  setStyleSheet(style);

  auto layout = new QVBoxLayout();
  layout->setContentsMargins(5,5,5,5);
  setLayout(layout);

  m_label = new QLabel();
  layout->addWidget(m_label);

  setFixedHeight(25);
  setMinimumWidth(75);
  setMaximumWidth(150);
}

OSDropZone2::~OSDropZone2()
{
}

void OSDropZone2::refresh()
{
  boost::optional<model::ModelObject> modelObject;

  if (m_item)
  {
    modelObject = OSAppBase::instance()->currentDocument()->getModelObject(m_item->itemId());
  } else if (m_get && *m_get) {
    modelObject = (*m_get)();
  }

  if (modelObject) {
    QString temp = QString::fromStdString(modelObject->name().get());
    if (m_label->text() == temp){
      return;
    }
    else {
      m_label->setText(temp);
    }

    emit inFocus(true, hasData());

    //// Adjust the width to accommodate the text
    //QFont myFont;
    //QFontMetrics fm(myFont);
    //auto width = fm.width(m_text);
    //setFixedWidth(width + 10);
  }

  update();
}

void OSDropZone2::bind(
  model::ModelObject & modelObject,
  OptionalModelObjectGetter get,
  ModelObjectSetter set,
  boost::optional<NoFailAction> reset)
{
  m_get = get;
  m_set = set;
  m_reset = reset;
  m_modelObject = modelObject;
  setAcceptDrops(true);

  refresh();
}

void OSDropZone2::unbind()
{
  m_modelObject.reset();
  m_get.reset();
  m_set.reset();
  m_reset.reset();
  setAcceptDrops(false);

  refresh();
}

void OSDropZone2::paintEvent( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  //p.drawText(rect(),Qt::AlignCenter,m_text);
}

void OSDropZone2::dragEnterEvent(QDragEnterEvent *event)
{
  if(event->proposedAction() == Qt::CopyAction){
    event->accept();
  }
}

void OSDropZone2::dropEvent(QDropEvent *event)
{
  event->accept();

  if((event->proposedAction() == Qt::CopyAction) && m_modelObject && m_set){

    OSItemId itemId(event->mimeData());
    boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
    m_item = OSItem::makeItem(itemId, OSItemType::ListItem);
    m_item->setParent(this);

    OS_ASSERT(modelObject);

    connect(m_item, &OSItem::itemRemoveClicked, this, &OSDropZone2::onItemRemoveClicked);

    connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSDropZone2::refresh);

    if(modelObject){
      if(OSAppBase::instance()->currentDocument()->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(m_modelObject->model());
        if (m_set)
        {
          bool success = (*m_set)(modelObject.get());
          if (!success) {
            modelObject->remove();
          }
        }
        refresh();
      }
      else
      {
        if (m_set)
        {
          (*m_set)(modelObject.get());
        }
        refresh();
      }
    }

  }
}

void OSDropZone2::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton){
    event->accept();

    if (!m_item) {
      makeItem();
    }

    if (m_item) {
      emit itemClicked(m_item);
    }
  }
}

void OSDropZone2::focusInEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason)
  {
    if (hasData()) {
      QString style("QWidget#OSDropZone {\
                     background: #ffc627;\
                     border: 2px dashed #808080;\
                     border-radius: 5px; }");
      setStyleSheet(style);
    }

    emit inFocus(true, hasData());
  }

  QWidget::focusInEvent(e);
}

void OSDropZone2::focusOutEvent(QFocusEvent * e)
{
  if (e->reason() == Qt::MouseFocusReason)
  {
    QString style("QWidget#OSDropZone {\
                   background: #CECECE;\
                   border: 2px dashed #808080;\
                   border-radius: 5px; }");
    setStyleSheet(style);

    emit inFocus(false, false);

    auto mouseOverInspectorView = OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectorController()->inspectorView()->mouseOverInspectorView();
    if (!mouseOverInspectorView) {
      emit itemClicked(nullptr);
    }
  }

  QWidget::focusOutEvent(e);
}

void OSDropZone2::onItemRemoveClicked()
{
  if (m_reset)
  {
    boost::optional<model::ModelObject> modelObject = (*m_get)();
    boost::optional<model::ParentObject> parent = boost::none;
    if (modelObject) {
     parent = modelObject->parent();  
    }
    (*m_reset)();
    if (m_deleteObject) {
      m_modelObject->remove();
    }
    emit objectRemoved(parent);
 }
}
void OSDropZone2::makeItem()
{
  if (!m_item && m_get && *m_get) {
    boost::optional<model::ModelObject> modelObject = (*m_get)();

    if (modelObject)
    {
      m_item = OSItem::makeItem(modelObjectToItemId(*modelObject, false));
      m_item->setParent(this);
      connect(m_item, &OSItem::itemRemoveClicked, this, &OSDropZone2::onItemRemoveClicked);

      connect(modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &OSDropZone2::refresh);
    }
  }
}

void OSDropZone2::setIsDefaulted(bool defaulted)
{
  if (!m_item) {
    makeItem();
  }

  if (m_item) {
    m_item->setIsDefaulted(defaulted);
  }

  if (defaulted) {
    m_label->setStyleSheet("QLabel { color:green }"); // color: #006837
  } else {
    m_label->setStyleSheet("QLabel { color:black }");
  }
}

bool OSDropZone2::isDefaulted()
{
  bool isDefaluted = false;
  if (m_item) {
    isDefaluted = m_item->isDefaulted();
  }
  return isDefaluted;
}

} // openstudio
