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

#include "Relationship.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model.hpp"
#include "Component.hpp"
#include "Component_Impl.hpp"
#include "ComponentData.hpp"
#include "ComponentData_Impl.hpp"

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Assert.hpp"

#include <QMetaType>

namespace openstudio {
namespace model {

ModelObject Relationship::modelObject() const {
  return m_object;
}

std::string Relationship::name() const {
  return m_name;
}

bool Relationship::isSettable() const {
  return m_qMetaProperty.isWritable();
}

bool Relationship::isSingular() const {
  std::string typeName = m_qMetaProperty.typeName();
  return (typeName == "boost::optional<openstudio::model::ModelObject>");
}

boost::optional<ModelObject> Relationship::relatedModelObject() const
{
  assertSingular(true);

  QVariant value = m_object.getImpl<detail::ModelObject_Impl>()->property(m_name.c_str());
  OS_ASSERT(value.canConvert<boost::optional<ModelObject> >());
  boost::optional<ModelObject> result = value.value<boost::optional<ModelObject> >();
  return result;
}

std::vector<ModelObject> Relationship::relatedModelObjects() const
{
  assertSingular(false);

  QVariant value = m_object.getImpl<detail::ModelObject_Impl>()->property(m_name.c_str());
  OS_ASSERT(value.canConvert<std::vector<ModelObject> >());
  std::vector<ModelObject> result = value.value<std::vector<ModelObject> >();
  return result;
}

bool Relationship::setRelatedModelObject(boost::optional<ModelObject> relatedModelObject)
{
  assertSingular(true);

  bool result = false;  
  if (isSettable()) { 

    // handle ComponentData set
    if (relatedModelObject && relatedModelObject->optionalCast<ComponentData>()) {
      if (relatedModelObject->model() != m_object.model()) {
        if (relatedModelObject->model().optionalCast<Component>()) {
          return setRelatedModelObject(relatedModelObject->model().cast<Component>());
        }
      }
    }

    QVariant value = QVariant::fromValue(relatedModelObject);
    m_object.getImpl<detail::ModelObject_Impl>()->setProperty(m_name.c_str(), value);
    
    // check set
    boost::optional<ModelObject> newValue = this->relatedModelObject();
    if (relatedModelObject){
      if (newValue){
        result = (relatedModelObject->handle() == newValue->handle());
      }
    }else{
      result = !newValue;
    }
  }

 return result;
}

bool Relationship::setRelatedModelObject(const Component& component) {
  OptionalComponentData ocd = m_object.model().insertComponent(component);
  if (!ocd) {
    return false;
  }
  // TODO: Bad set may junk up model with unnecessary objects.
  return setRelatedModelObject(ocd->primaryComponentObject());
}

bool Relationship::resetRelatedModelObject()
{
  return setRelatedModelObject(boost::none);
}

bool Relationship::setRelatedModelObjects(std::vector<ModelObject> relatedModelObjects)
{
  assertSingular(false);

  bool result = false;  
  if (isSettable()) { 
    QVariant value = QVariant::fromValue(relatedModelObjects);
    m_object.getImpl<detail::ModelObject_Impl>()->setProperty(m_name.c_str(), value);
    
    // check set
    std::vector<ModelObject> newValue = this->relatedModelObjects();
    if (newValue.size() == relatedModelObjects.size()){
      result = true;
      for (unsigned i = 0; i < newValue.size(); ++i){
        if (newValue[i].handle() != relatedModelObjects[i].handle()){
          result = false;
          break;
        }
      }
    }
  }

 return result;
}

bool Relationship::clearRelatedModelObjects()
{
  return setRelatedModelObjects(std::vector<ModelObject>());
}

// only called by ModelObject_Impl--no need to validate arguments
Relationship::Relationship(const ModelObject& object,const std::string& name) 
  : m_object(object), m_name(name)
{
  const QMetaObject* metaObject = m_object.getImpl<detail::ModelObject_Impl>()->metaObject();
  m_qMetaProperty = metaObject->property(metaObject->indexOfProperty(m_name.c_str()));
}

void Relationship::assertSingular(bool singular) const {
  if (singular) {
    if (!isSingular()) {
      LOG_AND_THROW("Relationship named '" << m_name << "', and associated with ModelObject" << 
        m_object.briefDescription() << ", is singular but is being used in a plural contex.");
    }
  } else {
    if (isSingular()) {
      LOG_AND_THROW("Relationship named '" << m_name << "', and associated with ModelObject" << 
        m_object.briefDescription() << ", is plural but is being used in a singular contex.");
    }    
  }
}


} // model
} // openstudio
