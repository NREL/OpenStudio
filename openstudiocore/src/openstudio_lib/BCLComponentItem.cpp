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

#include "BCLComponentItem.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/MeasureBadge.hpp"

#include "../utilities/bcl/BCLComponent.hpp"
#include "../utilities/bcl/LocalBCL.hpp"

#include <QLabel>

namespace openstudio {

OSItemId bclComponentToItemId(const openstudio::BCLComponent & component)
{
  OSItemId itemId(QString::fromStdString(component.uid()),OSItemId::BCL_SOURCE_ID,false);

  return itemId;
}

BCLComponentItem::BCLComponentItem( const BCLComponent & component, 
                                    OSItemType type,
                                    QWidget * parent )
  : OSItem(bclComponentToItemId(component),type,parent),
    m_bclComponent(component)
{
  setText(QString::fromStdString(m_bclComponent.name()));

  m_measureBadge->setMeasureBadgeType(MeasureBadgeType::BCLMeasure);
}

bool BCLComponentItem::equal(const openstudio::OSItem * item) const
{
  if( item->itemId().sourceId() == OSItemId::BCL_SOURCE_ID )
  {
    if( item->itemId().itemId() == itemId().itemId() )
    {
      return true;
    }
  }

  return false;
}

} // openstudio

