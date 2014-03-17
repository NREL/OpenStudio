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

#include "OSItem.hpp"
#include "RefrigerationGraphicsItems.hpp"
#include "../utilities/core/Assert.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include <QPainter>
#include <utility>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QLabel>

namespace openstudio {

const int RefrigerationSystemView::verticalSpacing = 20;
const int RefrigerationSystemView::margin = 10;
const double RefrigerationSystemView::componentHeight = 75;

RefrigerationView::RefrigerationView()
  : QWidget()
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(0);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);

  header = new QWidget();
  header->setObjectName("RefrigerationHeader");
  header->setStyleSheet("QWidget#RefrigerationHeader { background: #C3C3C3; }");
  header->setFixedHeight(35);
  mainVLayout->addWidget(header);

  QHBoxLayout * headerLayout = new QHBoxLayout();
  headerLayout->setContentsMargins(5,5,5,5);
  headerLayout->setSpacing(0);
  header->setLayout(headerLayout);

  nameLabel = new QLabel();
  headerLayout->addWidget(nameLabel);

  zoomOutButton = new ZoomOutButton();
  headerLayout->addStretch();
  headerLayout->addWidget(zoomOutButton);

  graphicsView = new QGraphicsView();
  graphicsView->setObjectName("GrayWidget");
  mainVLayout->addWidget(graphicsView);
}

RefrigerationSystemMiniView::RefrigerationSystemMiniView()
{
  refrigerationSystemView = new RefrigerationSystemView();
  refrigerationSystemView->setParentItem(this);
  refrigerationSystemView->setEnabled(false);

  adjustLayout();

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(cellWidth() - removeButtonItem->boundingRect().width() - 10,headerHeight() / 2.0 - removeButtonItem->boundingRect().height() / 2.0);

  zoomInButtonItem = new ZoomInButtonItem();
  zoomInButtonItem->setParentItem(this);
  zoomInButtonItem->setPos(removeButtonItem->pos().x() - 10 - zoomInButtonItem->boundingRect().width(),headerHeight() / 2.0 - zoomInButtonItem->boundingRect().height() / 2.0);

  setAcceptDrops(false);
}

void RefrigerationSystemMiniView::adjustLayout()
{
  double dx = (contentRect().width() - 20) / refrigerationSystemView->boundingRect().width();
  double dy = (contentRect().height() - 20)  / refrigerationSystemView->boundingRect().height();

  if( dx < dy )
  {
    dy = dx;
  }
  else
  {
    dx = dy;
  }

  refrigerationSystemView->setTransform(QTransform(dx,
                                        0,0,
                                        dy,
                                        0,0));

  refrigerationSystemView->setPos(contentRect().x() + contentRect().width() / 2.0 - refrigerationSystemView->boundingRect().width() * dx / 2.0,
                                  contentRect().y() + contentRect().height() / 2.0 - refrigerationSystemView->boundingRect().height() * dy / 2.0);
}

QRectF RefrigerationSystemMiniView::boundingRect() const
{
  return QRectF(QPoint(0,0),cellSize());
}

int RefrigerationSystemMiniView::cellWidth()
{
  return 350;
}

int RefrigerationSystemMiniView::headerHeight()
{
  return 50;
}

QSize RefrigerationSystemMiniView::cellSize()
{
  return QSize(cellWidth(),cellWidth() + headerHeight());
}

QRectF RefrigerationSystemMiniView::contentRect() const
{
  return QRectF(0,headerHeight(),cellWidth(),cellWidth());
}

QRectF RefrigerationSystemMiniView::headerRect() const
{
  return QRectF(0,0,cellWidth(),headerHeight());
}

void RefrigerationSystemMiniView::setName(const QString & name)
{
  m_name = name;

  update();
}

void RefrigerationSystemMiniView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(boundingRect());

  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF _headerRect = headerRect();

  painter->drawRect(_headerRect);

  painter->drawText(QRectF(_headerRect.x() + 5, _headerRect.y() + 5, _headerRect.width() - 10, _headerRect.height() - 10),Qt::AlignVCenter | Qt::AlignLeft,m_name);
}

RefrigerationSystemView::RefrigerationSystemView()
  : QGraphicsObject()
{
  // Condenser Item

  refrigerationCondenserView = new RefrigerationCondenserView();
  refrigerationCondenserView->setParentItem(this);

  // Sub Cooler Item

  refrigerationSubCoolerView = new RefrigerationSubCoolerView();
  refrigerationSubCoolerView->setParentItem(this);

  // Heat Reclaim Item

  refrigerationHeatReclaimView = new RefrigerationHeatReclaimView();
  refrigerationHeatReclaimView->setParentItem(this);
  refrigerationHeatReclaimView->hide();

  // Compressor Item

  refrigerationCompressorView = new RefrigerationCompressorView();
  refrigerationCompressorView->setParentItem(this);

  // Liquid Suction HX Item

  refrigerationSHXView = new RefrigerationSHXView();
  refrigerationSHXView->setParentItem(this);

  // Cases Item

  refrigerationCasesView = new RefrigerationCasesView();
  refrigerationCasesView->setParentItem(this);

  bool bingo = connect(refrigerationCasesView->expandButton,SIGNAL(mouseClicked(bool)),this,SLOT(setCasesExpanded(bool)));
  OS_ASSERT(bingo);

  // Secondary Item

  refrigerationSecondaryView = new RefrigerationSecondaryView();
  refrigerationSecondaryView->setParentItem(this);

  adjustLayout();
}

void RefrigerationSystemView::setId(const OSItemId & id)
{
  m_id = id;
}

void RefrigerationSystemView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationSystemView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

void RefrigerationSystemView::setCasesExpanded(bool expanded)
{
  refrigerationCasesView->setExpanded(expanded);

  adjustLayout();
}

void RefrigerationSystemView::adjustLayout()
{
  prepareGeometryChange();

  // Condenser

  refrigerationCondenserView->setPos(centerXPos() - refrigerationCondenserView->boundingRect().width() / 2, 
                                     verticalSpacing);

  // Mechanical Sub Cooler

  refrigerationSubCoolerView->setPos(leftXPos() - refrigerationSubCoolerView->boundingRect().width() / 2, 
                                     refrigerationCondenserView->y() + refrigerationCondenserView->boundingRect().height() + verticalSpacing);

  // Heat Reclaim

  refrigerationHeatReclaimView->setPos(rightXPos() - refrigerationHeatReclaimView->boundingRect().width() / 2, 
                                       refrigerationCondenserView->y() + refrigerationCondenserView->boundingRect().height() + verticalSpacing);
  
  // Compressor

  refrigerationCompressorView->setPos(rightXPos() - refrigerationCompressorView->boundingRect().width() / 2, 
                                      refrigerationHeatReclaimView->y() + refrigerationHeatReclaimView->boundingRect().height() + verticalSpacing);

  // SHX

  refrigerationSHXView->setPos(leftXPos() - refrigerationSHXView->boundingRect().width() / 2, 
                               refrigerationSubCoolerView->y() + refrigerationSubCoolerView->boundingRect().height() + verticalSpacing);

  // Cases

  refrigerationCasesView->setPos(centerXPos() - refrigerationCasesView->boundingRect().width() / 2, 
                                 refrigerationSHXView->y() + refrigerationSHXView->boundingRect().height() + verticalSpacing);

  // Cascade or Secondary

  refrigerationSecondaryView->setPos(centerXPos() - refrigerationSecondaryView->boundingRect().width() / 2, 
                                 refrigerationCasesView->y() + refrigerationCasesView->boundingRect().height() + verticalSpacing);

  if( QGraphicsScene * _scene = scene() )
  {
    _scene->setSceneRect(boundingRect());
  }
}

void RefrigerationSystemView::paint( QPainter *painter, 
                                     const QStyleOptionGraphicsItem *option, 
                                     QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QColor(206,206,206));
  painter->setPen(Qt::NoPen);

  QRectF _boudingRect = boundingRect();

  painter->drawRoundedRect(_boudingRect.x(),_boudingRect.y() + 5,_boudingRect.width(),_boudingRect.height() - 10,5,5);

  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF compressorRect = refrigerationCompressorView->boundingRect();
  QPointF compressorPoint = refrigerationCompressorView->pos();

  QRectF condRect = refrigerationCondenserView->boundingRect();
  QPointF condPoint = refrigerationCondenserView->pos();

  QRectF subCoolerRect = refrigerationSubCoolerView->boundingRect();
  QPointF subCoolerPoint = refrigerationSubCoolerView->pos();

  QRectF shxRect = refrigerationSHXView->boundingRect();
  QPointF shxPoint = refrigerationSHXView->pos();

  QRectF caseRect = refrigerationCasesView->boundingRect();
  QPointF casePoint = refrigerationCasesView->pos();

  painter->drawLine(compressorPoint.x() + compressorRect.width() / 2.0, compressorPoint.y(),
                    compressorPoint.x() + compressorRect.width() / 2.0, condPoint.y() + condRect.height() / 2.0);

  painter->drawLine(compressorPoint.x() + compressorRect.width() / 2.0, condPoint.y() + condRect.height() / 2.0,
                    condPoint.x() + condRect.width(), condPoint.y() + condRect.height() / 2.0);

  painter->drawLine(condPoint.x(),condPoint.y() + condRect.height() / 2.0,
                    subCoolerPoint.x() + subCoolerRect.width() / 2.0,condPoint.y() + condRect.height() / 2.0);

  painter->drawLine(subCoolerPoint.x() + subCoolerRect.width() / 2.0,condPoint.y() + condRect.height() / 2.0,
                    subCoolerPoint.x() + subCoolerRect.width() / 2.0,subCoolerPoint.y());

  painter->drawLine(subCoolerPoint.x() + subCoolerRect.width() / 2.0,subCoolerPoint.y() + subCoolerRect.height(),
                    shxPoint.x() + shxRect.width() / 2.0,shxPoint.y());

  painter->drawLine(shxPoint.x() + shxRect.width() / 2.0,shxPoint.y() + shxRect.height(),
                    shxPoint.x() + shxRect.width() / 2.0,casePoint.y());

  if( compressorPoint.x() + compressorRect.width() / 2.0 > casePoint.x() + caseRect.width() )
  {
    painter->drawLine(casePoint.x() + caseRect.width(),casePoint.y() + caseRect.height() / 2.0,
                      compressorPoint.x() + compressorRect.width() / 2.0,casePoint.y() + caseRect.height() / 2.0);

    painter->drawLine(compressorPoint.x() + compressorRect.width() / 2.0,casePoint.y() + caseRect.height() / 2.0,
                      compressorPoint.x() + compressorRect.width() / 2.0,compressorPoint.y() + compressorRect.height());
  }
  else
  {
    painter->drawLine(compressorPoint.x() + compressorRect.width() / 2.0,compressorPoint.y() + compressorRect.height(),
                      compressorPoint.x() + compressorRect.width() / 2.0,casePoint.y());
  }
}

QRectF RefrigerationSystemView::boundingRect() const
{
  refrigerationCompressorView->adjustLayout();

  double rightWidth = refrigerationHeatReclaimView->boundingRect().width() > refrigerationCompressorView->boundingRect().width() ?
                      refrigerationHeatReclaimView->boundingRect().width() : refrigerationCompressorView->boundingRect().width();

  double x = margin +
             refrigerationSubCoolerView->boundingRect().width() + 
             margin + 
             refrigerationCondenserView->boundingRect().width() +
             margin +
             rightWidth +
             margin;

  return QRectF(0,0,x,refrigerationSecondaryView->y() + refrigerationSecondaryView->boundingRect().height() + verticalSpacing);
  //return QRectF(0,0,x,refrigerationCasesView->y() + refrigerationCasesView->boundingRect().height() + verticalSpacing);
}

int RefrigerationSystemView::leftXPos() const
{
  return margin + (refrigerationSubCoolerView->boundingRect().width() / 2.0);
}

int RefrigerationSystemView::centerXPos() const
{
  return margin + 
         refrigerationSubCoolerView->boundingRect().width() +
         margin +
         (refrigerationCondenserView->boundingRect().width() / 2.0);
}

int RefrigerationSystemView::rightXPos() const
{
  refrigerationCompressorView->adjustLayout();

  double rightWidth = refrigerationHeatReclaimView->boundingRect().width() > refrigerationCompressorView->boundingRect().width() ?
                      refrigerationHeatReclaimView->boundingRect().width() : refrigerationCompressorView->boundingRect().width();

  return margin + 
         refrigerationSubCoolerView->boundingRect().width() +
         margin +
         refrigerationCondenserView->boundingRect().width() +
         margin +
         (rightWidth / 2.0);
}

RefrigerationCasesView::RefrigerationCasesView()
  : QGraphicsObject(),
    m_numberOfDisplayCases(0),
    m_numberOfWalkinCases(0),
    m_displayCasesPixmap(QPixmap(":/images/display_case.png").scaled(displayCasesRect().height(),displayCasesRect().height())),
    m_walkinPixmap(QPixmap(":/images/walkin_case.png").scaled(displayCasesRect().height(),displayCasesRect().height()))
{
  refrigerationCasesDropZoneView = new RefrigerationCasesDropZoneView();

  refrigerationCasesDropZoneView->setParentItem(this);

  refrigerationCasesDropZoneView->setPos(RefrigerationSystemView::margin / 2.0,RefrigerationSystemView::margin / 2.0);

  expandButton = new CaseViewExpandButton();

  expandButton->setParentItem(this);

  setExpanded(false);

  adjustLayout();
}

void RefrigerationCasesView::setNumberOfDisplayCases(int number)
{
  prepareGeometryChange();

  m_numberOfDisplayCases = number;

  update();
}

void RefrigerationCasesView::setNumberOfWalkinCases(int number)
{
  prepareGeometryChange();

  m_numberOfWalkinCases = number;

  update();
}

void RefrigerationCasesView::setExpanded(bool exapanded)
{
  prepareGeometryChange();

  m_expanded = exapanded;

  for( std::vector<QGraphicsObject *>::iterator it = m_caseDetailViews.begin();
       it != m_caseDetailViews.end();
       ++it )
  {
    if( m_expanded )
    {
      (*it)->show();
    }
    else
    {
      (*it)->hide();
    }
  }

  adjustLayout();
}

void RefrigerationCasesView::paint( QPainter *painter, 
                                    const QStyleOptionGraphicsItem *option, 
                                    QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF _displayCasesRect = displayCasesRect();

  painter->drawRect(boundingRect());

  painter->drawRect(_displayCasesRect); 

  painter->drawPixmap(_displayCasesRect.x(),
                      _displayCasesRect.y(),
                      m_displayCasesPixmap);

  painter->drawText(QRectF(_displayCasesRect.x() + m_displayCasesPixmap.width(),_displayCasesRect.y(),_displayCasesRect.width() - m_displayCasesPixmap.width(),_displayCasesRect.height()),
                          Qt::AlignCenter | Qt::TextWordWrap,QString::number(m_numberOfDisplayCases) + "\n Display Cases");

  QRectF _walkinRect = walkinCasesRect();

  painter->drawRect(_walkinRect); 

  painter->drawPixmap(_walkinRect.x(),
                      _walkinRect.y(),
                      m_walkinPixmap);

  painter->drawText(QRectF(_walkinRect.x() + m_walkinPixmap.width(),_walkinRect.y(),_walkinRect.width() - m_walkinPixmap.width(),_walkinRect.height()),
                          Qt::AlignCenter | Qt::TextWordWrap,QString::number(m_numberOfWalkinCases) + "\n Walkin Cases");
}

QRectF RefrigerationCasesView::displayCasesRect()
{
  double length = RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin;

  return QRectF(RefrigerationSystemView::margin / 2.0 + RefrigerationCasesDropZoneView::size().width() + RefrigerationSystemView::margin / 2.0,
                RefrigerationSystemView::margin / 2.0,
                130,length);
}

QRectF RefrigerationCasesView::walkinCasesRect()
{
  double length = RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin;

  return QRectF(displayCasesRect().x() + displayCasesRect().width() + RefrigerationSystemView::margin / 2.0,
                RefrigerationSystemView::margin / 2.0,
                130,length);
}

QRectF RefrigerationCasesView::summaryRect()
{
  double length = RefrigerationSystemView::componentHeight;

  return QRectF(0,0,
                RefrigerationSystemView::margin / 2.0 + RefrigerationCasesDropZoneView::size().width() +
                RefrigerationSystemView::margin / 2.0 + displayCasesRect().width() + 
                RefrigerationSystemView::margin / 2.0 + walkinCasesRect().width() + 
                RefrigerationSystemView::margin / 2.0,
                length);
}

QSizeF RefrigerationCasesView::size() const
{
  double width = 0.0;
  double height = 0.0;

  QRectF _summaryRect = summaryRect();

  if( m_expanded )
  {
    width = _summaryRect.width();

    int numberOfCases = m_caseDetailViews.size();

    if( numberOfCases > 0 )
    {
      height = casePos(numberOfCases - 1).y() + 
               RefrigerationCaseDetailView::size().height() + 
               RefrigerationSystemView::margin / 2.0;
    }
    else
    {
      height = _summaryRect.height();
    }
  }
  else
  {
    width = _summaryRect.width();

    height = _summaryRect.height();
  }

  // Add room for a footer with expand button.
  height = height + CaseViewExpandButton::size().height() + RefrigerationSystemView::margin / 2.0;

  return QSizeF(width,height);
}


QRectF RefrigerationCasesView::boundingRect() const
{
  QSizeF _size = size();

  return QRectF(0,0,_size.width(),_size.height());
}

void RefrigerationCasesView::insertCaseDetailView(int index, QGraphicsObject * object)
{
  prepareGeometryChange();

  m_caseDetailViews.insert(m_caseDetailViews.begin() + index,object);

  object->setParentItem(this);

  if( m_expanded )
  {
    object->show();
  }
  else
  {
    object->hide();
  }

  adjustLayout();
}

void RefrigerationCasesView::removeAllCaseDetailViews()
{
  prepareGeometryChange();

  for( std::vector<QGraphicsObject *>::iterator it = m_caseDetailViews.begin();
       it != m_caseDetailViews.end(); )
  {
    delete * it;

    it = m_caseDetailViews.erase(it);
  }

  adjustLayout();
}

void RefrigerationCasesView::adjustLayout()
{
  int i = 0;

  for( std::vector<QGraphicsObject *>::iterator it = m_caseDetailViews.begin();
       it != m_caseDetailViews.end();
       ++it )
  {
    (*it)->setPos(casePos(i));

    i++;
  }

  QSizeF _size = size();

  double x = _size.width() / 2.0 - CaseViewExpandButton::size().width() / 2.0;
  double y = _size.height() - CaseViewExpandButton::size().height() - RefrigerationSystemView::margin / 2.0;

  expandButton->setPos(x,y);
}

QPointF RefrigerationCasesView::casePos(int index) const
{
  QRectF _summaryRect = summaryRect();

  double x = RefrigerationSystemView::margin / 2.0;
  double y = _summaryRect.height() + index * ( RefrigerationCaseDetailView::size().height() + RefrigerationSystemView::margin);

  return QPointF(x,y);
}

RefrigerationCaseDetailView::RefrigerationCaseDetailView()
  : m_displayCasesPixmap(QPixmap(":/images/display_case.png").scaled(iconRect().width(),iconRect().height())),
    m_walkinPixmap(QPixmap(":/images/walkin_case.png").scaled(iconRect().width(),iconRect().height()))
{
  removeButtonItem = new RemoveButtonItem();

  removeButtonItem->setParentItem(this);

  removeButtonItem->setPos(size().width() - removeButtonItem->boundingRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::margin);

  bool bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);

  setId(OSItemId());
}

void RefrigerationCaseDetailView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationCaseDetailView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

void RefrigerationCaseDetailView::onRemoveButtonClicked()
{
  emit removeClicked(m_id);
}

QSizeF RefrigerationCaseDetailView::size()
{
  return QSizeF(iconRect().width() + nameRect().width(),RefrigerationSystemView::componentHeight);
}

QRectF RefrigerationCaseDetailView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void RefrigerationCaseDetailView::setName(const QString & name)
{
  m_name = name;
}

void RefrigerationCaseDetailView::setId(const OSItemId & id)
{
  m_id = id;
}

QRectF RefrigerationCaseDetailView::nameRect()
{
  return QRectF(iconRect().width(),0,RefrigerationCasesView::summaryRect().width() - iconRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::componentHeight);
}

QRectF RefrigerationCaseDetailView::iconRect()
{
  return QRectF(0,0,RefrigerationSystemView::componentHeight,RefrigerationSystemView::componentHeight);
}

void RefrigerationCaseDetailView::paint( QPainter *painter, 
            const QStyleOptionGraphicsItem *option, 
            QWidget * widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF _iconRect = iconRect();

  painter->drawRect(_iconRect);

  painter->drawPixmap(_iconRect.toRect(),m_displayCasesPixmap);

  painter->drawRect(boundingRect());

  painter->drawText(nameRect(),Qt::AlignCenter,m_name);
}

RefrigerationCondenserView::RefrigerationCondenserView()
{
  setIcon(QPixmap(":/images/air_cooled.png"));

  removeButtonItem = new RemoveButtonItem();

  removeButtonItem->setParentItem(this);

  removeButtonItem->setPos(size().width() - removeButtonItem->boundingRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::margin);

  bool bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);

  setCondenserId(OSItemId());
}

void RefrigerationCondenserView::setIcon(const QPixmap & pixmap)
{
  m_pixmap = pixmap.scaled(RefrigerationSystemView::componentHeight,RefrigerationSystemView::componentHeight);
  update();
}

void RefrigerationCondenserView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationCondenserView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

void RefrigerationCondenserView::setCondenserName(const QString & name)
{
  m_name = name;

  update();
}

void RefrigerationCondenserView::setCondenserId(const OSItemId & id)
{
  m_id = id;

  removeButtonItem->setVisible(! m_id.itemId().isEmpty());

  update();
}

void RefrigerationCondenserView::onRemoveButtonClicked()
{
  emit removeClicked(m_id);

  update();
}

void RefrigerationCondenserView::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  QRectF _boudingRect = boundingRect();

  if( m_id.itemId().isEmpty() )
  {
    painter->drawText(_boudingRect,Qt::AlignCenter | Qt::TextWordWrap,"Drop Condenser");

    painter->drawRoundedRect(_boudingRect,5,5);
  }
  else
  {
    painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

    painter->drawRoundedRect(_boudingRect,5,5);

    painter->drawPixmap(_boudingRect.width() / 2.0 - m_pixmap.rect().width() / 2.0,
                        _boudingRect.height() / 2.0 - m_pixmap.rect().height() / 2.0,
                        m_pixmap);
  }
}

QRectF RefrigerationCondenserView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

QSizeF RefrigerationCondenserView::size()
{
  return QSizeF(150,RefrigerationSystemView::componentHeight);
}

RefrigerationCompressorDetailView::RefrigerationCompressorDetailView()
  : m_pixmap(QPixmap(":/images/compressor.png").scaled(RefrigerationSystemView::componentHeight,RefrigerationSystemView::componentHeight))
{
  removeButtonItem = new RemoveButtonItem();

  removeButtonItem->setParentItem(this);

  removeButtonItem->setPos(size().width() - removeButtonItem->boundingRect().width(),0);

  bool bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);

  setId(OSItemId());
}

void RefrigerationCompressorDetailView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationCompressorDetailView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

void RefrigerationCompressorDetailView::onRemoveButtonClicked()
{
  emit removeClicked(m_id);
}

QSizeF RefrigerationCompressorDetailView::size()
{
  return QSizeF(75,RefrigerationCompressorView::height() - RefrigerationSystemView::margin);
}

void RefrigerationCompressorDetailView::setLabel(const QString & label)
{
  m_label = label;
}

void RefrigerationCompressorDetailView::setId(const OSItemId & id)
{
  m_id = id;
}

QRectF RefrigerationCompressorDetailView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void RefrigerationCompressorDetailView::paint( QPainter *painter, 
                                               const QStyleOptionGraphicsItem *option, 
                                               QWidget * widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF _boudingRect = boundingRect();

  painter->drawPixmap(_boudingRect.width() / 2.0 - m_pixmap.rect().width() / 2.0,
                      _boudingRect.height() / 2.0 - m_pixmap.rect().height() / 2.0,
                      m_pixmap);

  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,m_label);

}

RefrigerationCompressorDropZoneView::RefrigerationCompressorDropZoneView()
{
}

QRectF RefrigerationCompressorDropZoneView::boundingRect() const
{
  return QRectF(0,0,100,RefrigerationCompressorView::height() - RefrigerationSystemView::margin);
}

void RefrigerationCompressorDropZoneView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  painter->drawRoundedRect(boundingRect(),5,5);

  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,"Drag and Drop\nCompressor");
}

double RefrigerationCompressorView::height()
{
  return RefrigerationSystemView::componentHeight;
}

void RefrigerationCompressorView::insertCompressorDetailView(int index, QGraphicsObject * object)
{
  prepareGeometryChange();

  m_compressorDetailViews.insert(m_compressorDetailViews.begin() + index,object);

  object->setParentItem(this);

  adjustLayout();
}

void RefrigerationCompressorView::removeAllCompressorDetailViews()
{
  prepareGeometryChange();

  for( std::vector<QGraphicsObject *>::iterator it = m_compressorDetailViews.begin();
       it != m_compressorDetailViews.end(); )
  {
    delete * it;

    it = m_compressorDetailViews.erase(it);
  }

  adjustLayout();
}

void RefrigerationCompressorView::adjustLayout()
{
  refrigerationCompressorDropZoneView->setPos(RefrigerationSystemView::margin / 2.0,RefrigerationSystemView::margin / 2.0);

  int x = refrigerationCompressorDropZoneView->pos().x() + refrigerationCompressorDropZoneView->boundingRect().width() + RefrigerationSystemView::margin / 2.0;
  int y = RefrigerationSystemView::margin / 2.0;

  for( std::vector<QGraphicsObject *>::iterator it = m_compressorDetailViews.begin();
       it != m_compressorDetailViews.end();
       ++it )
  {
    (*it)->setPos(x,y);

    x = x + RefrigerationCompressorDetailView::size().width() + RefrigerationSystemView::margin / 2.0;
  }
}

RefrigerationCompressorView::RefrigerationCompressorView()
{
  refrigerationCompressorDropZoneView = new RefrigerationCompressorDropZoneView();
  refrigerationCompressorDropZoneView->setParentItem(this);

  adjustLayout();
}

void RefrigerationCompressorView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRect(boundingRect());
}

QRectF RefrigerationCompressorView::boundingRect() const
{
  return QRectF(0,0,
                RefrigerationSystemView::margin / 2.0 + refrigerationCompressorDropZoneView->boundingRect().width() +
                m_compressorDetailViews.size() * (RefrigerationCompressorDetailView::size().width() + RefrigerationSystemView::margin / 2.0) 
                + RefrigerationSystemView::margin / 2.0
                ,height());
}

RefrigerationCasesDropZoneView::RefrigerationCasesDropZoneView()
{
}

QSizeF RefrigerationCasesDropZoneView::size()
{
  return QSizeF(100,RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin);
}

QRectF RefrigerationCasesDropZoneView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void RefrigerationCasesDropZoneView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  painter->drawRoundedRect(boundingRect(),5,5);

  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,"Drag and Drop\nCases");
}

RefrigerationSubCoolerView::RefrigerationSubCoolerView()
  : m_pixmap(QPixmap(":/images/mechanical-sub-cooler.png").scaled(RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin,
                                                                  RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin))
{
  removeButtonItem = new RemoveButtonItem();

  removeButtonItem->setParentItem(this);

  removeButtonItem->setPos(size().width() - removeButtonItem->boundingRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::margin);

  bool bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);

  setId(OSItemId());
}

void RefrigerationSubCoolerView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  QRectF _boudingRect = boundingRect();

  if( m_id.itemId().isEmpty() )
  {
    painter->drawText(_boudingRect,Qt::AlignCenter | Qt::TextWordWrap," Drop Mechanical Sub Cooler ");

    painter->drawRoundedRect(_boudingRect,5,5);
  }
  else
  {
    painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

    painter->drawRoundedRect(_boudingRect,5,5);

    painter->drawPixmap(_boudingRect.width() / 2.0 - m_pixmap.rect().width() / 2.0,
                        _boudingRect.height() / 2.0 - m_pixmap.rect().height() / 2.0,
                        m_pixmap);
  }
}

void RefrigerationSubCoolerView::setId(const OSItemId & id)
{
  m_id = id;

  removeButtonItem->setVisible(! m_id.itemId().isEmpty());

  update();
}

void RefrigerationSubCoolerView::setName(const QString & name)
{
  m_name = name;

  update();
}

void RefrigerationSubCoolerView::onRemoveButtonClicked()
{
  emit removeClicked(m_id);
}

QSizeF RefrigerationSubCoolerView::size()
{
  return QSizeF(150,RefrigerationSystemView::componentHeight);
}

QRectF RefrigerationSubCoolerView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void RefrigerationSubCoolerView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationSubCoolerView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

void RefrigerationHeatReclaimView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRect(boundingRect());

  painter->drawText(boundingRect(),Qt::AlignCenter,"Heat Reclaim");
}

QRectF RefrigerationHeatReclaimView::boundingRect() const
{
  return QRectF(0,0,150,RefrigerationSystemView::componentHeight);
}

void RefrigerationSHXView::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  QRectF _boudingRect = boundingRect();

  if( m_id.itemId().isEmpty() )
  {
    painter->drawText(_boudingRect,Qt::AlignCenter | Qt::TextWordWrap," Drop Liquid Suction HX ");

    painter->drawRoundedRect(_boudingRect,5,5);
  }
  else
  {
    painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

    painter->drawRoundedRect(_boudingRect,5,5);

    painter->drawPixmap(_boudingRect.width() / 2.0 - m_pixmap.rect().width() / 2.0,
                        _boudingRect.height() / 2.0 - m_pixmap.rect().height() / 2.0,
                        m_pixmap);
  }
}

RefrigerationSHXView::RefrigerationSHXView()
  : m_pixmap(QPixmap(":/images/slhx.png").scaled(RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin,
                                                 RefrigerationSystemView::componentHeight - RefrigerationSystemView::margin))
{
  removeButtonItem = new RemoveButtonItem();

  removeButtonItem->setParentItem(this);

  removeButtonItem->setPos(size().width() - removeButtonItem->boundingRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::margin);

  bool bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);

  setId(OSItemId());
}

void RefrigerationSHXView::setId(const OSItemId & id)
{
  m_id = id;

  removeButtonItem->setVisible(! m_id.itemId().isEmpty());

  update();
}

void RefrigerationSHXView::setName(const QString & name)
{
  m_name = name;

  update();
}

void RefrigerationSHXView::onRemoveButtonClicked()
{
  emit removeClicked(m_id);
}

QSizeF RefrigerationSHXView::size()
{
  return QSizeF(150,RefrigerationSystemView::componentHeight);
}

QRectF RefrigerationSHXView::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void RefrigerationSHXView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void RefrigerationSHXView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      update();

      emit inspectClicked(m_id);
    }
  }
}

QRectF SecondaryDropZoneView::boundingRect() const
{
  return QRectF(0,0,RefrigerationCasesView::summaryRect().width(),RefrigerationSystemView::componentHeight);
}

void SecondaryDropZoneView::paint( QPainter *painter, 
                                   const QStyleOptionGraphicsItem *option, 
                                   QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  painter->drawRoundedRect(boundingRect(),5,5);

  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,"Add Cascade or Secondary System");
}

SecondaryDetailView::SecondaryDetailView()
  : QGraphicsObject()
{
  zoomInButtonItem = new ZoomInButtonItem();
  zoomInButtonItem->setParentItem(this);
  zoomInButtonItem->setPos(width() - RefrigerationSystemView::margin - zoomInButtonItem->boundingRect().width(),RefrigerationSystemView::margin);
  bool bingo = connect(zoomInButtonItem,SIGNAL(mouseClicked()),this,SLOT(onZoomButtonClicked()));
  OS_ASSERT(bingo);

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(zoomInButtonItem->x() - removeButtonItem->boundingRect().width() - RefrigerationSystemView::margin,RefrigerationSystemView::margin); 
  bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  OS_ASSERT(bingo);
}

double SecondaryDetailView::width()
{
  return RefrigerationCasesView::summaryRect().width();
}

double SecondaryDetailView::height()
{
  return RefrigerationSystemView::componentHeight / 2.0;
}

QRectF SecondaryDetailView::nameRect()
{
  return QRectF(RefrigerationSystemView::margin,RefrigerationSystemView::margin,
                removeButtonItem->x() - RefrigerationSystemView::margin * 2.0,height() - RefrigerationSystemView::margin * 2.0); 
}

void SecondaryDetailView::onRemoveButtonClicked()
{
  emit removeClicked(m_handle);
}

void SecondaryDetailView::onZoomButtonClicked()
{
  emit zoomInOnSystemClicked(m_handle);
}

void SecondaryDetailView::setName(const QString & name)
{
  m_name = name;
  update();
}

void SecondaryDetailView::setHandle(const Handle & handle)
{
  m_handle = handle;
  update();
}

QRectF SecondaryDetailView::boundingRect() const
{
  return QRectF(0,0,width(),height());
}

void SecondaryDetailView::paint( QPainter *painter, 
                                   const QStyleOptionGraphicsItem *option, 
                                   QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRect(boundingRect());
  painter->drawText(nameRect(),Qt::AlignCenter,m_name);
}

RefrigerationSecondaryView::RefrigerationSecondaryView()
  : QGraphicsObject()
{
  secondaryDropZoneView = new SecondaryDropZoneView();
  secondaryDropZoneView->setParentItem(this);

  adjustLayout();
}

void RefrigerationSecondaryView::insertSecondaryDetailView(int index, QGraphicsObject * object)
{
  m_secondaryDetailViews.insert(m_secondaryDetailViews.begin() + index,object);
  object->setParentItem(this);

  adjustLayout();
}

void RefrigerationSecondaryView::removeAllSecondaryDetailViews()
{
  for( std::vector<QGraphicsObject *>::iterator it = m_secondaryDetailViews.begin();
       it != m_secondaryDetailViews.end(); )
  {
    delete * it;
    it = m_secondaryDetailViews.erase(it);
  }

  adjustLayout();
}

void RefrigerationSecondaryView::adjustLayout()
{
  prepareGeometryChange();

  int x = 0;
  int y = 0;

  secondaryDropZoneView->setPos(0,0);
  y = y + secondaryDropZoneView->boundingRect().height() + RefrigerationSystemView::margin / 2.0;

  for( std::vector<QGraphicsObject *>::iterator it = m_secondaryDetailViews.begin();
       it != m_secondaryDetailViews.end();
       ++it )
  {
    (*it)->setPos(x,y);

    y = y + (*it)->boundingRect().height() + RefrigerationSystemView::margin / 2.0;
  }

  m_height = y; 
}

void RefrigerationSecondaryView::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));
}

QRectF RefrigerationSecondaryView::boundingRect() const
{
  return QRectF(0,0,RefrigerationCasesView::summaryRect().width(),m_height);
}

QRectF RefrigerationSystemDropZoneView::boundingRect() const
{
  return QRectF(QPoint(0,0),RefrigerationSystemMiniView::cellSize());
}

void RefrigerationSystemDropZoneView::paint( QPainter *painter, 
                                                 const QStyleOptionGraphicsItem *option, 
                                                 QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  painter->drawRect(boundingRect());

  QFont font = painter->font();
  font.setPointSize(30);
  painter->setFont(font);
  painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));
  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,"Drop Refrigeration System");
}

RefrigerationSystemDetailView::RefrigerationSystemDetailView()
  : QGraphicsObject()
{
  refrigerationSystemView = new RefrigerationSystemView();
  refrigerationSystemView->setParentItem(this);
  refrigerationSystemView->setPos(0,50);

  zoomOutButton = new ZoomOutButtonItem();
  zoomOutButton->setParentItem(this);
  zoomOutButton->setPos(800 - zoomOutButton->boundingRect().width() - 10,10);
}

QRectF RefrigerationSystemDetailView::boundingRect() const
{
  int width = refrigerationSystemView->boundingRect().width();
  int height = refrigerationSystemView->boundingRect().height() + 50;

  return QRectF(0,0,width,height);
}

void RefrigerationSystemDetailView::paint( QPainter *painter, 
            const QStyleOptionGraphicsItem *option, 
            QWidget *widget )
{
}

CaseViewExpandButton::CaseViewExpandButton()
   : m_openImage(QPixmap(":/images/contextual_arrow.png")),
     m_closeImage(QPixmap(":/images/contextual_arrow_up.png"))
{
}

QSize CaseViewExpandButton::size()
{
  return QSize(10,10);
}

QRectF CaseViewExpandButton::boundingRect() const
{
  return QRectF(0,0,size().width(),size().height());
}

void CaseViewExpandButton::paint(QPainter *painter, 
                                 const QStyleOptionGraphicsItem *option, 
                                 QWidget * widget)
{
  QRect rect(0,0,size().width(),size().height());

  if( m_checked )
  {
    painter->drawPixmap(rect,m_closeImage);
  }
  else
  {
    painter->drawPixmap(rect,m_openImage);
  }
}

} // openstudio

