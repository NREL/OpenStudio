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

#include "TabBarDrag.hpp"
#include <QApplication>


namespace resultsviewer{

TabBarDrag::TabBarDrag( QWidget* parent): QTabBar(parent)
{
//  setAcceptDrops(true);
}

/*
void TabBarDrag::dropEvent(QDropEvent *evt)
{
  QWidget *source = evt->source();
  if (source)
  {
    if (source->parent() == 0)
    {
      addTab(source,source->windowTitle());
      setCurrentIndex(count()-1);
    }
    else
    {
      browser->setParent(0);
      browser->setGeometry(10,30,1024,768);
      browser->show();
      browser->raise();
      browser->activateWindow();
    }
  }
}

void TabBarDrag::dragEnterEvent(QDragEnterEvent *e)
{
  e->accept();
}
*/

void TabBarDrag::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton) m_startDragPos = e->pos();
  QTabBar::mousePressEvent(e);
}

void TabBarDrag::mouseMoveEvent(QMouseEvent *e)
{
  int x = e->pos().x();
  int y = e->pos().y();
  if (( (x < pos().x()) || ( x > (pos().x() + this->frameGeometry().width()) ) )
    && ( ( y < pos().y() ) || ( y > (pos().y() + frameGeometry().height()) ) ))
  {
    if (e->buttons() & Qt::LeftButton)
    {
      int distance = (e->pos() - m_startDragPos).manhattanLength();
      if (distance >= QApplication::startDragDistance()) emit(signalPerformDrag());
    }
//    this->moveTab(0,0);
  }
  else
    QTabBar::mouseMoveEvent(e);
}

void TabBarDrag::performDrag()
{
  emit(signalPerformDrag());
  this->setCurrentIndex(0);
/*  QDrag *drag = new QDrag(this);
  QWidget *widget = currentWidget();
  WidgetMimeData *mimeData = new WidgetMimeData(widget);
  drag->setMimeData(mimeData);
  if (drag->exec(Qt::MoveAction) != Qt::MoveAction)
  {
    if (widget->parent() != 0)
    {
      widget->setParent(0);
      widget->setGeometry(QCursor::pos().x(),QCursor::pos().y(),width(),height());
      widget->show();
      widget->raise();
      widget->activateWindow();
    }
  }
*/
}



};
