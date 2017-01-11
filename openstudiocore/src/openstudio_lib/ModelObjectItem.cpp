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

#include "ModelObjectItem.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/MeasureBadge.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"

#include <QLabel>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

OSItemId modelObjectToItemId(const openstudio::model::ModelObject& modelObject, bool isDefaulted)
{
  std::stringstream ss;
  ss << modelObject;
  return OSItemId(toQString(modelObject.handle()), modelToSourceId(modelObject.model()), isDefaulted, toQString(ss.str()));
}

QString modelToSourceId(const openstudio::model::Model& model)
{
  std::stringstream ss;
  ss << model.getImpl<model::detail::Model_Impl>().get();
  std::string result = ss.str();
  return toQString(result);
}

ModelObjectItem::ModelObjectItem(const openstudio::model::ModelObject& modelObject,
                                 bool isDefaulted,
                                 OSItemType type,
                                 QWidget * parent )
  : OSItem(modelObjectToItemId(modelObject, isDefaulted), type, parent),
    m_handle(modelObject.handle()),
    m_modelObject(modelObject)
{
  this->setText(QString::fromStdString(m_modelObject.name().get()));

  m_modelObject.getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<ModelObjectItem, &ModelObjectItem::onObjectChanged>(this);

  if (!modelObject.getModelObjectSources<model::ComponentData>().empty()){
    m_measureBadge->setMeasureBadgeType(MeasureBadgeType::BCLMeasure);
  }
}

openstudio::Handle ModelObjectItem::handle() const
{
  return m_handle;
}

openstudio::model::ModelObject ModelObjectItem::modelObject() const
{
  return m_modelObject;
}

bool ModelObjectItem::equal(const openstudio::OSItem* otherItem) const
{
  if (m_modelObject.handle().isNull()){
    return false;
  }

  const ModelObjectItem* modelObjectItem = qobject_cast<const ModelObjectItem*>(otherItem);
  if (modelObjectItem){
    model::ModelObject otherModelObject = modelObjectItem->modelObject();
    if (otherModelObject.handle().isNull()){
      return false;
    }
    return (m_modelObject.handle() == otherModelObject.handle());
  }
  return false;
}

void ModelObjectItem::onObjectChanged()
{
  this->setText(QString::fromStdString(m_modelObject.name().get()));
}

} // openstudio

