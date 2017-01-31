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

#include "OSItem.hpp"

#include "BCLComponentItem.hpp"
#include "IconLibrary.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"
#include "ScriptItem.hpp"

#include "../shared_gui_components/MeasureBadge.hpp"

#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QDrag>
#include <QLabel>
#include <QMetaType>
#include <QMimeData>
#include <QMouseEvent>
#include <QPaintEngine>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QStyleOption>

namespace openstudio {

int __OSItemId_type = qRegisterMetaType<OSItemId>("OSItemId");
int __OSItemIdVector_type = qRegisterMetaType<std::vector<OSItemId> >("std::vector<OSItemId>");

const QString OSItemId::BCL_SOURCE_ID = QString("BCL");

OSItemId::OSItemId()
{}

OSItemId::OSItemId(const QString& itemId, const QString& sourceId, bool isDefaulted, const QString& otherData)
  : m_itemId(itemId), m_sourceId(sourceId), m_otherData(otherData), m_isDefaulted(isDefaulted)
{}

OSItemId::OSItemId(const QMimeData* mimeData)
  : m_isDefaulted(false)
{
  QStringList strings = mimeData->text().split(",");
  if (strings.size() > 0){
    m_itemId = strings[0];
  }
  if (strings.size() > 1){
    m_sourceId = strings[1];
  }
  if (strings.size() > 2){
    if (strings[2] == "True"){
      m_isDefaulted = true;
    }else{
      m_isDefaulted = false;
    }
  }
  for (int i = 3; i < strings.size(); ++i){
    m_otherData += strings[i];
    if (i < strings.size() - 1){
      m_otherData += ",";
    }
  }
}

QString OSItemId::itemId() const
{
  return m_itemId;
}

QString OSItemId::sourceId() const
{
  return m_sourceId;
}

QString OSItemId::otherData() const
{
  return m_otherData;
}

QString OSItemId::mimeDataText() const
{
  QString isDefaultedString((m_isDefaulted?"True":"False"));
  QString result = m_itemId + "," + m_sourceId + "," + isDefaultedString + "," + m_otherData;
  return result;
}

bool OSItemId::isDefaulted() const
{
  return m_isDefaulted;
}

void OSItemId::setIsDefaulted(bool isDefaulted)
{
  m_isDefaulted = isDefaulted;
}

bool OSItemId::operator==(const OSItemId& other) const
{
  bool result = false;
  if (!this->mimeDataText().isEmpty()){
    result = (this->mimeDataText() == other.mimeDataText());
  }
  return result;
}

OSItem::OSItem(const OSItemId& itemId, OSItemType osItemType, QWidget * parent)
               : QWidget(parent),
               m_itemId(itemId),
               m_selectionWidget(nullptr),
               m_borderWidget(nullptr),
               m_removeButton(nullptr),
               m_textLbl(nullptr),
               m_imageLeftLbl(nullptr),
               m_imageRightLbl(nullptr),
               m_mouseDown(false),
               m_selected(false),
               m_draggable(true),
               m_inspectable(false),
               m_acceptsDrops(false),
               m_size(QSize()),
               m_osItemType(osItemType),
               m_borderColor(QColor(Qt::black)),
               m_useLargeIcon(false)
{
  this->setObjectName("OSItem");

  createLayout();

  setAttributes(osItemType);
}

OSItem* OSItem::makeItem(const OSItemId& itemId, OSItemType osItemType)
{
  OSItem* result = nullptr;

  OSAppBase* app = OSAppBase::instance();

  if(itemId.sourceId() == OSItemId::BCL_SOURCE_ID)
  {
    boost::optional<BCLComponent> comp = LocalBCL::instance().getComponent(itemId.itemId().toStdString());
    if( comp )
    {
      result = new BCLComponentItem(comp.get(),osItemType);
    }
  }
  else
  {
    boost::optional<model::ModelObject> modelObject = app->currentDocument()->getModelObject(itemId);
    if (modelObject){
      result = new ModelObjectItem(*modelObject,itemId.isDefaulted(),osItemType);
    } else {
      openstudio::path p = openstudio::toPath(itemId.itemId());
      if (openstudio::filesystem::exists(p))
      {
        result = new ScriptItem(p, osItemType);
      }
    }
  }
  return result;
}

void OSItem::createLayout()
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10,5,10,5);
  mainHLayout->setAlignment(Qt::AlignVCenter);
  setLayout(mainHLayout);

  auto leftVBoxLayout = new QVBoxLayout();

  m_imageLeftLbl = new QLabel(this);
  leftVBoxLayout->addWidget(m_imageLeftLbl);

  m_measureBadge = new MeasureBadge();
  //m_measureBadge->setFixedWidth(25); NOTE mirror other instance?

  leftVBoxLayout->addWidget(m_measureBadge);

  mainHLayout->addLayout(leftVBoxLayout);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  if(doc){
    boost::optional<IddObjectType> iddObjectType = doc->getIddObjectType(m_itemId);
    if(iddObjectType){
      const QPixmap * pixmap = IconLibrary::Instance().findMiniIcon(iddObjectType->value());
      if(pixmap){
        setLeftPixmap(*pixmap);
      }

      pixmap = IconLibrary::Instance().findIcon(iddObjectType->value());
      if(pixmap){
        m_largePixmap = *pixmap;
      }
    }
  }
  {
    int w = leftPixmap().size().width();
    int h = leftPixmap().size().height();
    if(w==-1 || h==-1){
      setLeftPixmap(QPixmap(":/images/lilBug.png"));
    }
    else{
      setLeftPixmap(leftPixmap());
    }
  }

  m_textLbl = new QLabel(this);
  m_textLbl->setWordWrap(true);
  mainHLayout->addWidget(m_textLbl);

  mainHLayout->addStretch();

  m_removeButton = new QPushButton(this);
  mainHLayout->addWidget(m_removeButton);

  QString style;
  style.append("QWidget { ");
  style.append("border: none;");
  style.append(" background-image: url(\":/images/delete.png\")");
  style.append("}");

  m_removeButton->setFlat(true);
  m_removeButton->setStyleSheet(style);
  m_removeButton->setFixedSize(20,20);

  if (m_itemId.isDefaulted()){
    m_textLbl->setStyleSheet("QLabel { color: #006837 }");
    this->setRemoveable(false);
  }

  connect(m_removeButton, &QPushButton::clicked, this, &OSItem::onRemoveClicked);
}

void OSItem::setAttributes(OSItemType osItemType)
{
  if(osItemType == OSItemType::ListItem){
    setMinimumSize(QSize(ITEM_WIDTH,ITEM_HEIGHT));
  }
  else if(osItemType == OSItemType::DropzoneSquare){
    setFixedSize(QSize(ITEM_SIDE,ITEM_SIDE));
    m_textLbl->hide();
  }
  else if(osItemType == OSItemType::DropzoneRectangle){
    setMinimumSize(QSize(ITEM_WIDTH,ITEM_HEIGHT));
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
  }
  else{
    ///! should never get here
    OS_ASSERT(false);
  }
}

OSItemId OSItem::itemId() const
{
  return m_itemId;
}

bool OSItem::isDefaulted() const
{
  return m_itemId.isDefaulted();
}

void OSItem::setIsDefaulted(bool isDefaulted)
{
  m_itemId.setIsDefaulted(isDefaulted);
  if (isDefaulted) {
    m_textLbl->setStyleSheet("QLabel { color: #006837 }");
    this->setRemoveable(false);
  } else {
    m_textLbl->setStyleSheet("QLabel { color: black }");
    this->setRemoveable(true);
  }
}

QString OSItem::text() const
{
  return m_textLbl->text();
}

void OSItem::setText(const QString & text)
{
  m_textLbl->setText(text);

  setToolTip(text);
}

bool OSItem::draggable() const
{
  return m_draggable;
}

void OSItem::setDraggable(bool draggable)
{
  m_draggable = draggable;
}

bool OSItem::inspectable() const
{
  return m_inspectable;
}

void OSItem::setInspectable(bool inspectable)
{
  m_inspectable = inspectable;
}

bool OSItem::useLargeIcon()
{
  return m_useLargeIcon;
}

void OSItem::setUseLargeIcon(bool useLargeIcon)
{
  m_useLargeIcon = useLargeIcon;

  m_imageLeftLbl->setVisible(! useLargeIcon);
}

QPixmap OSItem::leftPixmap() const
{
  const QPixmap * pixmap = m_imageLeftLbl->pixmap();
  if(pixmap){
    return *m_imageLeftLbl->pixmap();
  }
  else{
    return QPixmap();
  }
}

void OSItem::setLeftPixmap(const QPixmap & pixmap)
{
  setLabelPixmap(m_imageLeftLbl, pixmap);
}

QPixmap OSItem::rightPixmap() const
{
  if(!m_imageRightLbl) return QPixmap();

  const QPixmap * pixmap = m_imageRightLbl->pixmap();
  if(pixmap){
    return *m_imageRightLbl->pixmap();
  }
  else{
    return QPixmap();
  }
}

void OSItem::setRightPixmap(const QPixmap & pixmap)
{
  setLabelPixmap(m_imageRightLbl, pixmap);
}

void OSItem::setLabelPixmap(QLabel * label, const QPixmap & pixmap)
{
  if(!label) return;

  int w = pixmap.size().width();
  int h = pixmap.size().height();
  if(w==-1 || h==-1){
    return;
  }

  label->setPixmap(pixmap);
  w = label->pixmap()->size().width();
  h = label->pixmap()->size().height();
  OS_ASSERT(w!=-1 && h!=-1); 
  label->setFixedSize(w,h);
}

bool OSItem::isBold()
{
  return m_textLbl->font().bold();
}

void OSItem::setBold(bool isBold)
{
  QFont font = m_textLbl->font();
  font.setBold(isBold);
  m_textLbl->setFont(font);
}

QColor OSItem::textColor()
{
  return m_textLbl->palette().color(m_textLbl->backgroundRole());
}

void OSItem::setTextColor(QColor color)
{
  QPalette palette = m_textLbl->palette();
  palette.setColor(m_textLbl->backgroundRole(), color);
  palette.setColor(m_textLbl->foregroundRole(), color);
  m_textLbl->setPalette(palette);
}

void OSItem::setFixedWidth(int width)
{
  if(width<0) return;
  QWidget::setFixedWidth(width);
}

void OSItem::setFixedHeight(int height)
{
  if(height<0) return;
  QWidget::setFixedHeight(height);
}

void OSItem::setFixedSize(const QSize & size)
{
  m_size = size;
  QWidget::setFixedSize(m_size);
}

void OSItem::setAspectRatio(AspectRatio aspectRatio)
{
  if(aspectRatio == AspectRatio::Rectangle){
    m_size = QSize(ITEM_WIDTH,ITEM_HEIGHT);
  }else if(aspectRatio == AspectRatio::Square){
    m_size = QSize(ITEM_SIDE,ITEM_SIDE);
  }
  QWidget::setFixedSize(m_size);
}

bool OSItem::selected() const
{
  return m_selected;
}

void OSItem::setSelected(bool selected)
{
  m_selected = selected;

  update();
}

bool OSItem::removeable() const
{
  return m_removeButton->isVisible();
}

void OSItem::setRemoveable(bool removeable)
{
  //return m_removeButton->hide();
  m_removeButton->setVisible(removeable);
}

OSItemType OSItem::osItemType() const
{
  return m_osItemType;
}

void OSItem::setOSItemType(OSItemType osItemType)
{
  m_osItemType = osItemType;

  update();
}

void OSItem::paintEvent(QPaintEvent * event)
{
  QPainter p(this);

  if( m_osItemType == OSItemType::ListItem )
  {
    if( m_selected )
    {
      QLinearGradient linearGrad(QPointF(0,0), QPointF(0,rect().height()));
      linearGrad.setColorAt(0, QColor(113,153,200,255));
      linearGrad.setColorAt(0.10, QColor(113,153,200,150));
      linearGrad.setColorAt(0.15, QColor(210,222,236,150));
      linearGrad.setColorAt(1.00, QColor(210,222,236,150));

      QBrush brush(linearGrad);
      p.setBrush(brush);
      p.setPen(Qt::NoPen);

      p.drawRect(0,0,rect().width(),rect().height() - 1);
    }

    p.setPen(QPen(Qt::black));
    p.drawLine(0,rect().height() - 1,rect().width(),rect().height() - 1);
  }
  else if( m_osItemType == OSItemType::LibraryItem )
  {
    QPen pen(QColor(128,128,128));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    p.setPen(pen);
    p.setBrush(QColor(206,206,206));
    p.drawRoundedRect(5,4,rect().width() - 10,rect().height() - 10,10,10);
  }
  else if( m_osItemType == OSItemType::DropzoneSquare || m_osItemType == OSItemType::DropzoneRectangle )
  {
    QPen pen(QColor(128,128,128));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    p.setPen(pen);
    p.setBrush(QColor(206,206,206));
    p.drawRoundedRect(0,0,rect().width() - 1,rect().height() - 1,10,10);
  }

  if( m_useLargeIcon )
  {
    QRect _rect = rect();
    QRect newRect(_rect.x() + 7,_rect.y() + 7,_rect.width() - 14,_rect.height() - 14);

    p.drawPixmap(newRect,m_largePixmap);  
  }
}

void OSItem::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton){
    if( m_mouseDown ){
      event->accept();
      emit itemClicked(this);
      m_mouseDown = false;
    }
  }
}

void OSItem::mousePressEvent(QMouseEvent * event)
{
  event->accept();

  if (event->button() == Qt::LeftButton){
    m_mouseDown = true;
    m_dragStartPosition = event->pos();
  }
}

void OSItem::mouseMoveEvent(QMouseEvent *event)
{
  if (!m_draggable){
    return;
  }

  if (!m_mouseDown){
    return;
  }

  if (!(event->buttons() && Qt::LeftButton)){
    return;
  }

  if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()){
    return;
  }

  // start a drag
  m_mouseDown = false;

  QString mimeDataText = m_itemId.mimeDataText();

  auto mimeData = new QMimeData;
  mimeData->setText(mimeDataText);

  QWidget* parent = this->parentWidget();
  OS_ASSERT(parent);

  // parent the QDrag on this parent instead of this, in case this item is deleted during drag
  auto drag = new QDrag(parent);
  drag->setMimeData(mimeData);
  
  QPixmap _pixmap(size());
  _pixmap.fill(Qt::transparent);

  render(&_pixmap,QPoint(),QRegion(),RenderFlags(DrawChildren));

  drag->setPixmap(_pixmap);
  drag->setHotSpot(event->pos());

  drag->exec(Qt::CopyAction);
}

void OSItem::leaveEvent(QEvent * event)
{
  m_mouseDown = false;
  event->accept();
}

void OSItem::dragEnterEvent(QDragEnterEvent *event)
{
  if(event->proposedAction() == Qt::CopyAction){
    event->accept();
  }
}

void OSItem::dropEvent(QDropEvent *event)
{
  event->accept();
  if(event->proposedAction() == Qt::CopyAction){
    const QMimeData* mimeData =event->mimeData();
    OS_ASSERT(mimeData);
    OSItemId replacementItemId(mimeData);
    emit itemReplacementDropped(this, replacementItemId);
  }
}

/****************** SLOTS ******************/

void OSItem::onRemoveClicked()
{
  // Note: an OSDropZone2 owns this OSItem;
  // there should be a parent...
  //OS_ASSERT(this->parent());

  // ... and it should be a OSDropZone2...
  //auto dropZone = qobject_cast<OSDropZone2 *>(this->parent());
  //OS_ASSERT(dropZone);

  // ... and it needs to listen to, and act on,
  // this signal to cause a model object reset
  emit itemRemoveClicked(this);
}

} // openstudio

