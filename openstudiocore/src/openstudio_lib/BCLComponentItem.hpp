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

#ifndef OPENSTUDIO_BCLCOMPONENTITEM_HPP
#define OPENSTUDIO_BCLCOMPONENTITEM_HPP

#include "OSItem.hpp"
#include "../utilities/bcl/BCLComponent.hpp"

namespace openstudio {

OSItemId bclComponentToItemId(const BCLComponent & component);

class BCLComponentItem : public OSItem
{
  Q_OBJECT

public:

  BCLComponentItem( const BCLComponent & component,
                    OSItemType type,
                    QWidget * parent = 0 );
  virtual ~BCLComponentItem() {}

  virtual bool equal(const openstudio::OSItem *) const;

private:

  BCLComponent m_bclComponent;

  //openstudio::Handle m_handle;
};

} // openstudio

#endif // OPENSTUDIO_BCLCOMPONENTITEM_HPP

