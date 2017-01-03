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

  // This should be true because we manage m_registeredItems and the use of the accessor methods.
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

  // This should be true because we manage m_listControllers and the use of the accessor methods.
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

