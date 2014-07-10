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
  return OSItemId(modelObject.handle().toString(), modelToSourceId(modelObject.model()), isDefaulted, toQString(ss.str()));
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

  bool isConnected = connect( m_modelObject.getImpl<openstudio::model::detail::ModelObject_Impl>().get(),
                              SIGNAL(onChange()),
                              this,
                              SLOT(onObjectChanged()) );
  OS_ASSERT(isConnected);

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

