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

#include "OSVectorController.hpp"
#include "OSItem.hpp"

namespace openstudio {

OSVectorController::OSVectorController()
  : QObject()
{
}

void OSVectorController::reportItems()
{
  emit itemIds(this->makeVector());
}

void OSVectorController::removeItem(OSItem* item)
{
  this->onRemoveItem(item);
}

void OSVectorController::replaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  this->onReplaceItem(currentItem, replacementItemId);
}

void OSVectorController::drop(const OSItemId& itemId)
{
  this->onDrop(itemId);
}

void OSVectorController::makeNewItem()
{
  this->onMakeNewItem();
}

void OSVectorController::onRemoveItem(OSItem* item)
{
}

void OSVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
}

void OSVectorController::onDrop(const OSItemId& itemId)
{
}

void OSVectorController::onMakeNewItem()
{
}

} // openstudio

