/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "OSGridView.hpp"

#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include "../shared_gui_components/OSCollapsibleView.hpp"

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

OSGridView::OSGridView(std::vector<model::ModelObject> modelObjects, QWidget * parent)
  : QWidget(parent)
{
  m_gridLayout = new QGridLayout();
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop);
  setLayout(m_gridLayout);

  setContentsMargins(5,5,5,5);
}

void OSGridView::setGridController(QSharedPointer<OSGridController> gridController)
{
  if( m_gridController )
  {
    m_gridController->disconnect(this);
  }

  m_gridController = gridController;

  connect(m_gridController.data(),SIGNAL(itemInserted(int,int)),this,SLOT(insertItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(itemRemoved(int,int)),this,SLOT(removeItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(itemChanged(int,int)),this,SLOT(refreshItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(modelReset()),this,SLOT(refreshAllViews()));

  refreshAll();
}

QSharedPointer<OSGridController> OSGridView::gridController() const
{
  return m_gridController;
}
void OSGridView::refreshAll()
{
  QLayoutItem *child;
  while((child = m_gridLayout->takeAt(0)) != 0)
  {
      QWidget * widget = child->widget();

      OS_ASSERT(widget);

      delete widget;

      delete child;
  }

  if( m_gridController )
  {
    for( int i = 0, n = m_gridController->rowCount(); i < n; i++ )
    {
      for( int j = 0, n = m_gridController->columnCount(); i < n; i++ )
      {
        addWidget(i,j);
      }
    }
  }
}

void OSGridView::addWidget(int row, int column)
{
  OS_ASSERT(m_gridController);

  //QSharedPointer<QWidget> widget = m_gridController->itemAt(row,column);

  //OS_ASSERT(itemData);

  //QWidget * itemView = m_delegate->view(itemData);

  //itemView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

  //m_gridLayout->insertWidget(i,itemView);

  //m_widgetItemPairs.insert( std::make_pair<QObject *,QSharedPointer<OSGridItem> >(itemView,itemData) );

  //bool bingo = connect(itemView,SIGNAL(destroyed(QObject *)),this,SLOT(removePair(QObject *)));

  //OS_ASSERT(bingo);
}

void OSGridView::removeWidget(int row, int column)
{
  int index = 0; // TODO

  m_gridLayout->takeAt(index);

  QWidget * widget = 0;

  OS_ASSERT(widget);

  delete widget;
}

void OSGridView::refresh(int row, int column)
{
}

} // openstudio

