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

#include "OSGridController.hpp"

#include <QWidget>

#include <utilities/core/Assert.hpp>
#include <algorithm>

namespace openstudio {

OSGridController::OSGridController()
  : QObject()
{
  m_selectionController = QSharedPointer<OSGridItemSelectionController>(new OSGridItemSelectionController());

  m_selectionController->registerGridController(this);
}

OSGridController::~OSGridController()
{
  OS_ASSERT(m_selectionController);

  m_selectionController->unregisterGridController(this);
}

QSharedPointer<OSGridItemSelectionController> OSGridController::selectionController() const
{
  return m_selectionController;
}

void OSGridController::setSelectionController(QSharedPointer<OSGridItemSelectionController> controller)
{
  if( (controller) && (m_selectionController != controller) )
  {
    if(m_selectionController)
    {
      m_selectionController->unregisterGridController(this);
    }

    m_selectionController = controller;

    m_selectionController->registerGridController(this);

    m_selectionController->unselectAllItems();

    emit modelReset();
  }
  else if( controller.isNull() && m_selectionController )
  {
    m_selectionController->unregisterGridController(this);

    m_selectionController.clear();
  }
}

void OSGridController::registerItem(QPointer<OSGridItem> item)
{
  m_registeredItems.push_back(item);
}

void OSGridController::unregisterItem(QPointer<OSGridItem> item)
{
  std::vector<QPointer<OSGridItem> >::iterator it = std::find(m_registeredItems.begin(),m_registeredItems.end(),item);

  // This should be true because we manage m_registeredItems and the use of the acccessor methods.
  OS_ASSERT(it != m_registeredItems.end());

  m_registeredItems.erase(it);
}

OSGridItem::OSGridItem(OSGridController * gridController)
  : m_gridController(gridController)
{
  if(m_gridController)
  {
    m_gridController->registerItem(this);
  }
}

OSGridItem::~OSGridItem()
{
  if(m_gridController)
  {
    // This will do nothing if the item is not selected
    m_gridController->selectionController()->removeSelectedItem(this);

    // Unregister item from the grid controller
    m_gridController->unregisterItem(this);
  }
}

void OSGridItem::setController(OSGridController * controller)
{
  if( controller && (m_gridController != controller))
  {
    if(m_gridController)
    {
      m_gridController->unregisterItem(this);
    }

    m_gridController = controller;

    m_gridController->registerItem(this);
  }
  else if( ! controller &&  m_gridController )
  {
    m_gridController->unregisterItem(this);
  }
}

bool OSGridItem::isSelected() const
{
  if(m_gridController)
  {
    QSharedPointer<OSGridItemSelectionController> selectionController = m_gridController->selectionController();

    std::vector<QPointer<OSGridItem> > selectedItems = selectionController->m_selectedItems;

    std::vector<QPointer<OSGridItem> >::const_iterator it = std::find(selectedItems.begin(),selectedItems.end(),this);

    if( it != selectedItems.end() )
    {
      return true;
    }
  }

  return false;
}

void OSGridItem::setSelected(bool isSelected)
{
  if(m_gridController)
  {
    QSharedPointer<OSGridItemSelectionController> selectionController = m_gridController->selectionController();

    std::vector<QPointer<OSGridItem> >::iterator it = 
      std::find(selectionController->m_selectedItems.begin(),selectionController->m_selectedItems.end(),this);

    bool wasSelected = ( it != selectionController->m_selectedItems.end() );

    if( isSelected )
    {
      if( wasSelected )
      { 
        return; 
      }
      else
      {
        if( ! selectionController->m_allowMultipleSelections )
        {
          selectionController->unselectAllItems();
        }

        selectionController->addSelectedItem(this);

        emit selectedChanged(true);
      }
    }
    else
    {
      if( wasSelected )
      {
        selectionController->removeSelectedItem(this);

        emit selectedChanged(false);
      }
      else
      {
        return;
      }
    }
  }
}

void OSGridItem::toggleSelected()
{
  setSelected( ! isSelected() );
}

OSGridItemSelectionController::OSGridItemSelectionController()
 : QObject(),
   m_allowMultipleSelections(false)
{
}

void OSGridItemSelectionController::setAllowMultipleSelections(bool multipleSelections)
{
  m_allowMultipleSelections = multipleSelections;

  if( ! m_allowMultipleSelections )
  {
    unselectAllItems();
  }
}

void OSGridItemSelectionController::unselectAllItems()
{
  std::vector<QPointer<OSGridItem> > selectedItems = m_selectedItems;

  for( std::vector<QPointer<OSGridItem> >::const_iterator it = selectedItems.begin();
       it != selectedItems.end();
       it++ )
  {
    (*it)->setSelected(false);
  }
}

void OSGridItemSelectionController::selectAllItems()
{
  if( m_allowMultipleSelections )
  {
    for( std::vector<QPointer<OSGridController> >::iterator gridIt = m_gridControllers.begin(); 
         gridIt != m_gridControllers.end();
         gridIt++ )
    {
      QPointer<OSGridController> t_gridController = (*gridIt);

      for( std::vector<QPointer<OSGridItem> >::iterator itemit = t_gridController->m_registeredItems.begin();
           itemit != t_gridController->m_registeredItems.end();
           itemit++ )
      {
        (*itemit)->setSelected(true);
      }
    }
  }
}

void OSGridItemSelectionController::emitSelectionChanged()
{
  emit selectionChanged(m_selectedItems);
}

std::vector<QPointer<OSGridItem> > OSGridItemSelectionController::selectedItems() const
{
  return m_selectedItems;
}

bool OSGridItemSelectionController::allowMultipleSelections() const
{
  return m_allowMultipleSelections;
}

void OSGridItemSelectionController::registerGridController(OSGridController * gridController)
{
  m_gridControllers.push_back(QPointer<OSGridController>(gridController));
}

void OSGridItemSelectionController::unregisterGridController(OSGridController * gridController)
{
  std::vector<QPointer<OSGridController> >::iterator it = std::find(m_gridControllers.begin(),m_gridControllers.end(),gridController);

  // This should be true because we manage m_gridControllers and the use of the acccessor methods.
  OS_ASSERT(it != m_gridControllers.end());

  m_gridControllers.erase(it);
}

void OSGridItemSelectionController::addSelectedItem(OSGridItem * item)
{
  OS_ASSERT(item);

  m_selectedItems.push_back(item);

  emitSelectionChanged();
}

void OSGridItemSelectionController::removeSelectedItem(OSGridItem * item)
{
  OS_ASSERT(item);

  std::vector<QPointer<OSGridItem> >::iterator it = 
    std::find(m_selectedItems.begin(),m_selectedItems.end(),item);

  if( it != m_selectedItems.end() )
  {
    m_selectedItems.erase(it);

    emitSelectionChanged();
  }
}

QWidget * OSGridItemDelegate::view(QSharedPointer<OSGridItem> dataSource) 
{ 
  return new QWidget();
}

} // openstudio

