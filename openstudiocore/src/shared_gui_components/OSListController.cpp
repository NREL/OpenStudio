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

#include "OSListController.hpp"
#include "../utilities/core/Assert.hpp"
#include <algorithm>
#include <QWidget>

namespace openstudio {

OSListController::OSListController()
  : QObject()
{
  m_selectionController = QSharedPointer<OSItemSelectionController>(new OSItemSelectionController());

  m_selectionController->registerListController(this);
}

OSListController::~OSListController()
{
  OS_ASSERT(m_selectionController);

  m_selectionController->unregisterListController(this);
}

QSharedPointer<OSItemSelectionController> OSListController::selectionController() const
{
  return m_selectionController;
}

void OSListController::setSelectionController(QSharedPointer<OSItemSelectionController> controller)
{
  if( (controller) && (m_selectionController != controller) )
  {
    if(m_selectionController)
    {
      m_selectionController->unregisterListController(this);
    }

    m_selectionController = controller;

    m_selectionController->registerListController(this);

    m_selectionController->unselectAllItems();

    emit modelReset();
  }
  else if( controller.isNull() && m_selectionController )
  {
    m_selectionController->unregisterListController(this);

    m_selectionController.clear();
  }
}

void OSListController::registerItem(QPointer<OSListItem> item)
{
  m_registeredItems.push_back(item);
}

void OSListController::unregisterItem(QPointer<OSListItem> item)
{
  auto it = std::find(m_registeredItems.begin(),m_registeredItems.end(),item);

  // This should be true because we manage m_registeredItems and the use of the acccessor methods.
  OS_ASSERT(it != m_registeredItems.end());

  m_registeredItems.erase(it);
}

OSListItem::OSListItem(OSListController * listController)
  : m_listController(listController)
{
  if(m_listController)
  {
    m_listController->registerItem(this);
  }
}

OSListItem::~OSListItem()
{
  if(m_listController)
  {
    // This will do nothing if the item is not selected
    m_listController->selectionController()->removeSelectedItem(this);

    // Unregister item from the list controller
    m_listController->unregisterItem(this);
  }
}

void OSListItem::setController(OSListController * controller)
{
  if( controller && (m_listController != controller))
  {
    if(m_listController)
    {
      m_listController->unregisterItem(this);
    }

    m_listController = controller;

    m_listController->registerItem(this);
  }
  else if( ! controller &&  m_listController )
  {
    m_listController->unregisterItem(this);
  }
}

bool OSListItem::isSelected() const
{
  if(m_listController)
  {
    QSharedPointer<OSItemSelectionController> selectionController = m_listController->selectionController();

    std::vector<QPointer<OSListItem> > selectedItems = selectionController->m_selectedItems;

    std::vector<QPointer<OSListItem> >::const_iterator it = std::find(selectedItems.begin(),selectedItems.end(),this);

    if( it != selectedItems.end() )
    {
      return true;
    }
  }

  return false;
}

void OSListItem::setSelected(bool isSelected)
{
  if(m_listController)
  {
    QSharedPointer<OSItemSelectionController> selectionController = m_listController->selectionController();

    auto it = std::find(selectionController->m_selectedItems.begin(),selectionController->m_selectedItems.end(),this);

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

void OSListItem::toggleSelected()
{
  setSelected( ! isSelected() );
}

OSItemSelectionController::OSItemSelectionController()
 : QObject(),
   m_allowMultipleSelections(false)
{
}

void OSItemSelectionController::setAllowMultipleSelections(bool multipleSelections)
{
  m_allowMultipleSelections = multipleSelections;

  if( ! m_allowMultipleSelections )
  {
    unselectAllItems();
  }
}

void OSItemSelectionController::unselectAllItems()
{
  std::vector<QPointer<OSListItem> > selectedItems = m_selectedItems;

  for( std::vector<QPointer<OSListItem> >::const_iterator it = selectedItems.begin();
       it != selectedItems.end();
       ++it )
  {
    (*it)->setSelected(false);
  }
}

void OSItemSelectionController::selectAllItems()
{
  if( m_allowMultipleSelections )
  {
    for( const auto & listController : m_listControllers )
    {
      for( auto & registeredItem : listController->m_registeredItems)
      {
        registeredItem->setSelected(true);
      }
    }
  }
}

void OSItemSelectionController::emitSelectionChanged()
{
  emit selectionChanged(m_selectedItems);
}

std::vector<QPointer<OSListItem> > OSItemSelectionController::selectedItems() const
{
  return m_selectedItems;
}

bool OSItemSelectionController::allowMultipleSelections() const
{
  return m_allowMultipleSelections;
}

void OSItemSelectionController::registerListController(OSListController * listController)
{
  m_listControllers.push_back(QPointer<OSListController>(listController));
}

void OSItemSelectionController::unregisterListController(OSListController * listController)
{
  auto it = std::find(m_listControllers.begin(),m_listControllers.end(),listController);

  // This should be true because we manage m_listControllers and the use of the acccessor methods.
  OS_ASSERT(it != m_listControllers.end());

  m_listControllers.erase(it);
}

void OSItemSelectionController::addSelectedItem(OSListItem * item)
{
  OS_ASSERT(item);

  m_selectedItems.push_back(item);

  emitSelectionChanged();
}

void OSItemSelectionController::removeSelectedItem(OSListItem * item)
{
  OS_ASSERT(item);

  auto it = 
    std::find(m_selectedItems.begin(),m_selectedItems.end(),item);

  if( it != m_selectedItems.end() )
  {
    m_selectedItems.erase(it);

    emitSelectionChanged();
  }
}

QWidget * OSItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 
  return new QWidget();
}

QGraphicsObject * OSGraphicsItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 
  return nullptr;
}

} // openstudio

