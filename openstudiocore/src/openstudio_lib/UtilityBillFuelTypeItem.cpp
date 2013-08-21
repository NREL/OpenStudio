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

#include <openstudio_lib/UtilityBillFuelTypeItem.hpp>
#include <openstudio_lib/OSCollapsibleItemHeader.hpp>
#include <openstudio_lib/UtilityBillFuelTypeListView.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/data/DataEnums.hpp>

#include <iostream>

namespace openstudio {

UtilityBillFuelTypeItem::UtilityBillFuelTypeItem(OSCollapsibleItemHeader* collapsibleItemHeader,
                                         UtilityBillFuelTypeListView* utilityBillFuelTypeListView, 
                                         QWidget * parent )
  : OSCollapsibleItem(collapsibleItemHeader, utilityBillFuelTypeListView, parent)
{
}

IddObjectType UtilityBillFuelTypeItem::iddObjectType() const
{
  OSItemList* itemList = this->itemList();
  UtilityBillFuelTypeListView* utilityBillFuelTypeListView = qobject_cast<UtilityBillFuelTypeListView*>(itemList);
  OS_ASSERT(utilityBillFuelTypeListView);
  OS_ASSERT(utilityBillFuelTypeListView->iddObjectType() == openstudio::IddObjectType::OS_UtilityBill);
  return utilityBillFuelTypeListView->iddObjectType();
}

FuelType UtilityBillFuelTypeItem::fuelType() const
{
  OSItemList* itemList = this->itemList();
  UtilityBillFuelTypeListView* utilityBillFuelTypeListView = qobject_cast<UtilityBillFuelTypeListView*>(itemList);
  OS_ASSERT(utilityBillFuelTypeListView);
  FuelType fuelType = utilityBillFuelTypeListView->fuelType();
  return fuelType;
}

} // openstudio

