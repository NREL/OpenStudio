/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
