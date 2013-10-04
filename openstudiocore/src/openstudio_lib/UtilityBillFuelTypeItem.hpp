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

#ifndef OPENSTUDIO_UTILITYBILLFUELTYPEITEM_H
#define OPENSTUDIO_UTILITYBILLFUELTYPEITEM_H

#include <openstudio_lib/OSCollapsibleItem.hpp>

#include <model/Model.hpp>

namespace openstudio {

class FuelType;
class UtilityBillFuelTypeListView;
class OSCollapsibleItemHeader;

class UtilityBillFuelTypeItem : public OSCollapsibleItem
{
  Q_OBJECT

  public:

    UtilityBillFuelTypeItem(OSCollapsibleItemHeader* collapsibleItemHeader,
                        UtilityBillFuelTypeListView* utilityBillFuelTypeListView, 
                        QWidget * parent = 0 );

    virtual ~UtilityBillFuelTypeItem() {}

    IddObjectType iddObjectType() const;

    FuelType fuelType() const;

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLFUELTYPEITEM_H

