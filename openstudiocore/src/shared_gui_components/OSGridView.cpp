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
#include <utilities/idd/IddObject.hpp>

namespace openstudio {

OSGridView::OSGridView(const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_model(model),
  m_gridController(0)
{
  m_gridLayout = new QGridLayout();
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop);
  setLayout(m_gridLayout);

  setContentsMargins(5,5,5,5);

  setGridController(new OSGridController(openstudio::IddObjectType::Refrigeration_Case, model));

  refreshAll();
}

OSGridView::OSGridView(std::vector<model::ModelObject> modelObjects, QWidget * parent)
  : QWidget(parent)
{
  m_gridLayout = new QGridLayout();
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop);
  setLayout(m_gridLayout);

  setContentsMargins(5,5,5,5);
}

void OSGridView::setGridController(OSGridController * gridController)
{
  if( m_gridController )
  {
    m_gridController->disconnect(this);
  }

  m_gridController = gridController;

  bool isConnected = false;

  isConnected = connect(m_gridController,SIGNAL(itemInserted(int,int)),this,SLOT(addWidget(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(itemRemoved(int,int)),this,SLOT(removeWidget(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(itemChanged(int,int)),this,SLOT(refresh(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(modelReset()),this,SLOT(refreshAll()));
  OS_ASSERT(isConnected);
    
  refreshAll();
}

OSGridController * OSGridView::gridController() const
{
  return m_gridController;
}

void OSGridView::refresh(int row, int column)
{
  removeWidget(row,column);

  addWidget(row,column);
}

void OSGridView::removeWidget(int row, int column)
{
  QLayoutItem * item = m_gridLayout->itemAtPosition(row,column);

  OS_ASSERT(item);

  QWidget * widget = item->widget();

  OS_ASSERT(widget);

  delete widget;

  delete item;
}

void OSGridView::refreshAll()
{
  QLayoutItem * child;
  while((child = m_gridLayout->takeAt(0)) != 0)
  {
      QWidget * widget = child->widget();

      OS_ASSERT(widget);

      delete widget;

      delete child;
  }

  if( m_gridController )
  {
    for( int i = 0; i < m_gridController->rowCount(); i++ )
    {
      for( int j = 0; j < m_gridController->columnCount(); j++ )
      {
        addWidget(i,j);
      }
    }
  }
}

void OSGridView::addWidget(int row, int column)
{
  OS_ASSERT(m_gridController);

  QWidget * widget = m_gridController->widgetAt(row,column);

  OS_ASSERT(m_gridLayout);
  
  m_gridLayout->addWidget(widget,row,column);
}

} // openstudio

