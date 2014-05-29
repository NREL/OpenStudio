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

#include "ModelObjectTypeItem.hpp"
#include "OSCollapsibleItemHeader.hpp"
#include "ModelObjectListView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <iostream>

namespace openstudio {

ModelObjectTypeItem::ModelObjectTypeItem(OSCollapsibleItemHeader* collapsibleItemHeader,
                                         ModelObjectListView* modelObjectListView, 
                                         QWidget * parent )
  : OSCollapsibleItem(collapsibleItemHeader, modelObjectListView, parent)
{
}

IddObjectType ModelObjectTypeItem::iddObjectType() const
{
  OSItemList* itemList = this->itemList();
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(itemList);
  OS_ASSERT(modelObjectListView);
  return modelObjectListView->iddObjectType();
}

} // openstudio

