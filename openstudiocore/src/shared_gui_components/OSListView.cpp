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

#include "OSListView.hpp"
#include "OSListController.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsObject>

namespace openstudio {

OSListView::OSListView(bool scrollable, QWidget * parent)
  : QWidget(parent),
  m_widgetItemPairs(std::map<QObject *,QSharedPointer<OSListItem> >()),
  m_scrollable(scrollable),
  m_scrollArea(nullptr)
{
  m_delegate = QSharedPointer<OSItemDelegate>(new OSItemDelegate());
  m_mainVLayout = new QVBoxLayout();
  m_mainVLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_mainVLayout->setAlignment(Qt::AlignTop);

  if( scrollable )
  {
    auto scrollWidget = new QWidget();
    scrollWidget->setObjectName("ScrollWidget");
    scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
    scrollWidget->setLayout(m_mainVLayout);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setBackgroundRole(QPalette::NoRole);

    auto scrollLayout = new QVBoxLayout();
    scrollLayout->setContentsMargins(0,0,0,0);
    scrollLayout->addWidget(m_scrollArea);

    setLayout(scrollLayout);
  }
  else
  {
    setLayout(m_mainVLayout);
  }

  setContentsMargins(5,5,5,5);
  setSpacing(5);
}

void OSListView::setHorizontalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSListView::setVerticalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSListView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OSListView::setDelegate(QSharedPointer<OSItemDelegate> delegate)
{
  if( delegate )
  { 
    m_delegate = delegate;

    refreshAllViews();
  }
}

void OSListView::setListController(QSharedPointer<OSListController> listController)
{
  if( m_listController )
  {
    m_listController->disconnect(this);
  }

  m_listController = listController;

  connect(m_listController.data(), &OSListController::itemInserted, this, &OSListView::insertItemView);
  connect(m_listController.data(), &OSListController::itemRemoved, this, &OSListView::removeItemView);
  connect(m_listController.data(), &OSListController::itemChanged, this, &OSListView::refreshItemView);
  connect(m_listController.data(), &OSListController::modelReset, this, &OSListView::refreshAllViews);
  refreshAllViews();
}

QSharedPointer<OSListController> OSListView::listController() const
{
  return m_listController;
}

void OSListView::setSpacing(int spacing)
{
  m_mainVLayout->setSpacing(spacing);
}

void OSListView::setContentsMargins(int left,int top,int right,int bottom)
{
  m_mainVLayout->setContentsMargins(left,top,right,bottom);
}

void OSListView::refreshAllViews()
{
  QLayoutItem *child;
  while((child = m_mainVLayout->takeAt(0)) != nullptr)
  {
    QWidget * widget = child->widget();

    OS_ASSERT(widget);

    delete widget;

    delete child;
  }

  if( m_listController )
  {
    for( int i = 0, n = m_listController->count(); i < n; i++ )
    {
      insertItemView(i);
    }
  }
}

void OSListView::insertItemView(int i)
{
  OS_ASSERT(m_listController);

  QSharedPointer<OSListItem> itemData = m_listController->itemAt(i);

  OS_ASSERT(itemData);

  QWidget * itemView = m_delegate->view(itemData);

  itemView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

  m_mainVLayout->insertWidget(i,itemView);

  m_widgetItemPairs.insert( std::make_pair(itemView,itemData) );

  // For some reason, this needs to use the old-style connect on mac (exiting OS app crash)
  bool isConnected = connect(itemView, SIGNAL(destroyed(QObject *)), this, SLOT(removePair(QObject *)));
  OS_ASSERT(isConnected);
}

void OSListView::removeItemView(int i)
{
  QLayoutItem * item = m_mainVLayout->takeAt(i);

  OS_ASSERT(item);

  QWidget * widget = item->widget();

  OS_ASSERT(widget);

  delete widget;

  delete item;
}

void OSListView::removePair(QObject * object) {
  m_widgetItemPairs.erase(m_widgetItemPairs.find(object));
}

void OSListView::refreshItemView(int i)
{
  if (i < int(m_widgetItemPairs.size())) {
    removeItemView(i);
  }
  else {
    LOG(Trace,"Not calling removeItemView(" << i << "), because the list is not that long.");
  }

  insertItemView(i);
}

} // openstudio

