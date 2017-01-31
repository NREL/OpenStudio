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

#include "UtilityBillAllFuelTypesListView.hpp"

#include "ModelObjectTypeItem.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "OSCollapsibleItemHeader.hpp"
#include "OSItem.hpp"
#include "UtilityBillFuelTypeItem.hpp"
#include "UtilityBillFuelTypeListView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <iostream>

namespace openstudio {

UtilityBillAllFuelTypesListView::UtilityBillAllFuelTypesListView(const model::Model& model, 
                                                 bool addScrollArea, 
                                                 OSItemType headerType,
                                                 QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), m_model(model), m_headerType(headerType)
{ 
}

UtilityBillAllFuelTypesListView::UtilityBillAllFuelTypesListView(const std::vector<std::pair<FuelType, std::string> >& utilityBillFuelTypesAndNames,
                                                 const model::Model& model,
                                                 bool addScrollArea, 
                                                 OSItemType headerType,
                                                 QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), 
    m_utilityBillFuelTypesAndNames(utilityBillFuelTypesAndNames), 
    m_model(model),
    m_headerType(headerType)
{ 
  for(auto utilityBillFuelTypesAndName = m_utilityBillFuelTypesAndNames.rbegin();
      utilityBillFuelTypesAndName != m_utilityBillFuelTypesAndNames.rend();
      ++utilityBillFuelTypesAndName)
  {
    addUtilityBillFuelType(utilityBillFuelTypesAndName->first, utilityBillFuelTypesAndName->second);
  }
}

void UtilityBillAllFuelTypesListView::addModelObjectType(const IddObjectType& iddObjectType, const std::string& name)
{
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false), m_headerType);
  auto modelObjectListView = new ModelObjectListView(iddObjectType, m_model, false,false);
  auto modelObjectTypeItem = new ModelObjectTypeItem(collapsibleItemHeader, modelObjectListView);

  addCollapsibleItem(modelObjectTypeItem);
}

void UtilityBillAllFuelTypesListView::addUtilityBillFuelType(const FuelType & fuelType, const std::string& name)
{
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false), m_headerType);
  auto utilityBillFuelTypeListView = new UtilityBillFuelTypeListView(m_model, fuelType, false);
  auto utilityBillFuelTypeItem = new UtilityBillFuelTypeItem(collapsibleItemHeader, utilityBillFuelTypeListView);

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

