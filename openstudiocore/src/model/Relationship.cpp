/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

// #include "Relationship.hpp"
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
