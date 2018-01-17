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

#include "TabDropDock.hpp"
#include "TabBarDrag.hpp"
#include "BrowserView.hpp"
#include "PlotView.hpp"
#include <QApplication>
#include <QDrag>


namespace resultsviewer{

TabDropDock::TabDropDock( QWidget* parent): QTabWidget(parent)
{
//  setAcceptDrops(true);
//  setAttribute(Qt::WA_Hover,true);
//  setMouseTracking(true);
  auto tabBar = new TabBarDrag(this);
  connect(tabBar, &TabBarDrag::signalPerformDrag, this, &TabDropDock::slotPerformDrag);
  setTabBar(tabBar);
}


void TabDropDock::dropEvent(QDropEvent *evt)
{
/*  resultsviewer::BrowserView *bv = qobject_cast<resultsviewer::BrowserView *>(evt->source());
  resultsviewer::PlotView *pv = qobject_cast<resultsviewer::PlotView *>(evt->source());
  if (pv)
  {
    if (pv->parent() == 0)
    {
      addTab(pv,pv->windowTitle());
      setCurrentIndex(count()-1);
    }
  }
  if (bv)
  {
    if (bv->parent() == 0)
    {
      addTab(bv,bv->windowTitle());
      setCurrentIndex(count()-1);
    }
  }
*/
}

void TabDropDock::dragEnterEvent(QDragEnterEvent *e)
{
  e->accept();
}


/*
void TabDropDock::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton) m_startDragPos = e->pos();
}

void TabDropDock::mouseMoveEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
  {
    int distance = (e->pos() - m_startDragPos).manhattanLength();
    if (distance >= QApplication::startDragDistance()) performDrag();
  }
}
*/
void TabDropDock::slotPerformDrag()
{
//  emit(signalFloatOrDockMe(this));
  auto drag = new QDrag(this);
  QWidget *widget = currentWidget();
  if (widget)
  {
    auto mimeData = new WidgetMimeData(widget);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::MoveAction) != Qt::MoveAction)
    {
      if (widget->parent() != nullptr)
      {
        widget->setParent(nullptr);
        widget->setGeometry(QCursor::pos().x(),QCursor::pos().y(),width(),height());
        widget->show();
        widget->raise();
        widget->activateWindow();
//        removeTab(currentIndex());
      }
    }
  }
//  this->repaint();
}



};
