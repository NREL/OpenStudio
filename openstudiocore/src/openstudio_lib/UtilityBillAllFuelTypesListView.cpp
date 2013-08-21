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

#include <openstudio_lib/UtilityBillAllFuelTypesListView.hpp>

#include <openstudio_lib/ModelObjectTypeItem.hpp>
#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/ModelObjectListView.hpp>
#include <openstudio_lib/OSCollapsibleItemHeader.hpp>
#include <openstudio_lib/OSItem.hpp>
#include <openstudio_lib/UtilityBillFuelTypeItem.hpp>
#include <openstudio_lib/UtilityBillFuelTypeListView.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/UtilityBill.hpp>
#include <model/UtilityBill_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <iostream>

namespace openstudio {

UtilityBillAllFuelTypesListView::UtilityBillAllFuelTypesListView(const model::Model& model, 
                                                 bool addScrollArea, 
                                                 OSItem::Type headerType,
                                                 QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), m_model(model), m_headerType(headerType)
{ 
}

UtilityBillAllFuelTypesListView::UtilityBillAllFuelTypesListView(const std::vector<std::pair<FuelType, std::string> >& utilityBillFuelTypesAndNames,
                                                 const model::Model& model,
                                                 bool addScrollArea, 
                                                 OSItem::Type headerType,
                                                 QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), 
    m_utilityBillFuelTypesAndNames(utilityBillFuelTypesAndNames), 
    m_model(model),
    m_headerType(headerType)
{ 
  typedef std::vector<std::pair<FuelType, std::string> >::value_type PairType;
  BOOST_REVERSE_FOREACH(PairType utilityBillFuelTypesAndName, m_utilityBillFuelTypesAndNames){
    addUtilityBillFuelType(utilityBillFuelTypesAndName.first, utilityBillFuelTypesAndName.second);
  }
}

void UtilityBillAllFuelTypesListView::addModelObjectType(const IddObjectType& iddObjectType, const std::string& name)
{
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false), m_headerType);
  ModelObjectListView* modelObjectListView = new ModelObjectListView(iddObjectType, m_model, false,false);
  ModelObjectTypeItem* modelObjectTypeItem = new ModelObjectTypeItem(collapsibleItemHeader, modelObjectListView);

  addCollapsibleItem(modelObjectTypeItem);
}

void UtilityBillAllFuelTypesListView::addUtilityBillFuelType(const FuelType & fuelType, const std::string& name)
{
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false), m_headerType);
  UtilityBillFuelTypeListView* utilityBillFuelTypeListView = new UtilityBillFuelTypeListView(m_model, fuelType, false);
  UtilityBillFuelTypeItem* utilityBillFuelTypeItem = new UtilityBillFuelTypeItem(collapsibleItemHeader, utilityBillFuelTypeListView);

  addCollapsibleItem(utilityBillFuelTypeItem);
}

FuelType UtilityBillAllFuelTypesListView::currentFuelType() const
{
  OSCollapsibleItem* selectedCollapsibleItem = this->selectedCollapsibleItem();
  UtilityBillFuelTypeItem* utilityBillFuelTypeItem = qobject_cast<UtilityBillFuelTypeItem*>(selectedCollapsibleItem);
  OS_ASSERT(utilityBillFuelTypeItem);
  return utilityBillFuelTypeItem->fuelType();
}

boost::optional<openstudio::model::ModelObject> UtilityBillAllFuelTypesListView::selectedModelObject() const
{
  OSItem* selectedItem = this->selectedItem();
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(selectedItem);
  if (modelObjectItem){
    return modelObjectItem->modelObject();
  }
  return boost::none;
}

boost::optional<openstudio::FuelType> UtilityBillAllFuelTypesListView::selectedFuelType() const
{
  boost::optional<openstudio::model::ModelObject> modelObject = selectedModelObject();
  if(modelObject){
    if(boost::optional<model::UtilityBill> utilityBill = modelObject.get().optionalCast<model::UtilityBill>()){
      return utilityBill.get().fuelType();
    }
    // Opps, its not a UtilityBill!
    OS_ASSERT(false);
  }
  return boost::none;
}

} // openstudio

