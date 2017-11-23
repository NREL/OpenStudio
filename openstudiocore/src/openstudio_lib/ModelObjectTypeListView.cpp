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
  auto modelObjectListView = new ModelObjectListView(iddObjectType, m_model, false, m_showLocalBCL);
  auto modelObjectTypeItem = new ModelObjectTypeItem(collapsibleItemHeader, modelObjectListView);

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

