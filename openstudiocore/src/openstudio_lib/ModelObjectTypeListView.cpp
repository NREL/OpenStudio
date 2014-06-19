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

#include "ModelObjectTypeListView.hpp"
#include "ModelObjectTypeItem.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "OSCollapsibleItemHeader.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <iostream>

namespace openstudio {

ModelObjectTypeListView::ModelObjectTypeListView(const model::Model& model, 
                                                 bool addScrollArea, 
                                                 OSItemType headerType, 
                                                 bool showLocalBCL,
                                                 QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), m_model(model), m_headerType(headerType), m_showLocalBCL(showLocalBCL)
{ 
}

ModelObjectTypeListView::ModelObjectTypeListView(const std::vector<std::pair<IddObjectType, std::string> >& modelObjectTypesAndNames,
                                                 const model::Model& model, bool addScrollArea, 
                                                 OSItemType headerType, bool showLocalBCL, QWidget * parent )
  : OSCollapsibleItemList(addScrollArea, parent), 
    m_modelObjectTypesAndNames(modelObjectTypesAndNames), 
    m_model(model), m_headerType(headerType), m_showLocalBCL(showLocalBCL)
{ 
  for(auto modelObjectTypeAndName = m_modelObjectTypesAndNames.rbegin();
      modelObjectTypeAndName != m_modelObjectTypesAndNames.rend();
      ++modelObjectTypeAndName)
  {
    addModelObjectType(modelObjectTypeAndName->first, modelObjectTypeAndName->second);
  }
}

void ModelObjectTypeListView::addModelObjectType(const IddObjectType& iddObjectType, const std::string& name)
{
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false), m_headerType);
  ModelObjectListView* modelObjectListView = new ModelObjectListView(iddObjectType, m_model, false,m_showLocalBCL);
  ModelObjectTypeItem* modelObjectTypeItem = new ModelObjectTypeItem(collapsibleItemHeader, modelObjectListView);

  addCollapsibleItem(modelObjectTypeItem);
}

IddObjectType ModelObjectTypeListView::currentIddObjectType() const
{
  OSCollapsibleItem* selectedCollapsibleItem = this->selectedCollapsibleItem();
  ModelObjectTypeItem* modelObjectTypeItem = qobject_cast<ModelObjectTypeItem*>(selectedCollapsibleItem);
  OS_ASSERT(modelObjectTypeItem);
  return modelObjectTypeItem->iddObjectType();
}

boost::optional<openstudio::model::ModelObject> ModelObjectTypeListView::selectedModelObject() const
{
  OSItem* selectedItem = this->selectedItem();
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(selectedItem);
  if (modelObjectItem){
    return modelObjectItem->modelObject();
  }
  return boost::none;
}

} // openstudio

