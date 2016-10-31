/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemActuator::iddObjectType());
  }

  EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemActuator::iddObjectType());
  }

  EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const EnergyManagementSystemActuator_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemActuator_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemActuator_Impl::iddObjectType() const {
    return EnergyManagementSystemActuator::iddObjectType();
  }

  ModelObject EnergyManagementSystemActuator_Impl::actuatedComponent() const {
    
    return this->getTarget(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName)->cast<ModelObject>();
  }

  std::string EnergyManagementSystemActuator_Impl::actuatedComponentControlType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType,true);
    if (value) {
      return value.get();
    } else {
      return "";
    }
  }

  std::string EnergyManagementSystemActuator_Impl::actuatedComponentType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType, true);
    if (value) {
      return value.get();
    } else {
      return "";
    }
  }

  bool EnergyManagementSystemActuator_Impl::setActuatedComponent(const ModelObject& modelObject) {
    return setPointer(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName, modelObject.handle());
  }

  bool EnergyManagementSystemActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    bool result = setString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
    return result;
  }

  bool EnergyManagementSystemActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
    bool result = setString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType, actuatedComponentType);
    return result;
  }

} // detail

EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& modelObject, const std::string actuatedComponentType, const std::string actuatedComponentControlType)
  : ModelObject(EnergyManagementSystemActuator::iddObjectType(), modelObject.model()) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
  bool ok = setActuatedComponent(modelObject);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponent to "
      << modelObject.briefDescription() << ".");
  }
  ok = setActuatedComponentType(actuatedComponentType);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to "
      << actuatedComponentType << ".");
  }
  ok = setActuatedComponentControlType(actuatedComponentControlType);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to "
      << actuatedComponentControlType << ".");
  } 
}

EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& modelObject)
  : ModelObject(EnergyManagementSystemActuator::iddObjectType(), modelObject.model())
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
  setActuatedComponent(modelObject);
  //TODO set below to some appropriate default that depends on the modelObject::IddType
  //setActuatedComponentControlType();
  //setActuatedComponentType();
}

EnergyManagementSystemActuator::EnergyManagementSystemActuator(const Model& model)
  : ModelObject(EnergyManagementSystemActuator::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
  //This constructor is used in the ReverseTranslator when the ActuatedComponent doesnt Reverse Translate.
  //In that case, a generic Actuator is created with ActuatedComponent blank (but the old value is in a comment)
  //TODO setActuatedComponent to a default modelObject.
  //TODO set below to some appropriate default that depends on the modelObject::IddType
  //setActuatedComponentControlType();
  //setActuatedComponentType();
}

IddObjectType EnergyManagementSystemActuator::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Actuator);
}

ModelObject EnergyManagementSystemActuator::actuatedComponent() const {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponent();
}

std::string EnergyManagementSystemActuator::actuatedComponentControlType() const {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponentControlType();
}

std::string EnergyManagementSystemActuator::actuatedComponentType() const {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponentType();
}

bool EnergyManagementSystemActuator::setActuatedComponent(const ModelObject& modelObject) {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponent(modelObject);
}

bool EnergyManagementSystemActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponentControlType(actuatedComponentControlType);
}

bool EnergyManagementSystemActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
}

/// @cond
EnergyManagementSystemActuator::EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

