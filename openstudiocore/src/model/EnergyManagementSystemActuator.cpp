/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

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
    return result;
  }

  IddObjectType EnergyManagementSystemActuator_Impl::iddObjectType() const {
    return EnergyManagementSystemActuator::iddObjectType();
  }

  boost::optional<ModelObject> EnergyManagementSystemActuator_Impl::actuatedComponent() const {
    boost::optional<ModelObject> result;
    boost::optional<WorkspaceObject> wo = this->getTarget(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName);
    if (wo) {
      result = wo->cast<ModelObject>();
    }
    return result;
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

  std::vector< std::pair<std::string, std::string> > EnergyManagementSystemActuator_Impl::haystackTags() {
    std::vector< std::pair<std::string, std::string> > result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> tag = group.cast<WorkspaceExtensibleGroup>().getString(OS_EnergyManagementSystem_ActuatorExtensibleFields::HaystackTag);
      boost::optional<std::string> value = group.cast<WorkspaceExtensibleGroup>().getString(OS_EnergyManagementSystem_ActuatorExtensibleFields::HaystackValue);

      if (tag && value) {
        result.push_back(std::make_pair(tag.get(), value.get()));
      }
    }

    return result;
  }

  bool EnergyManagementSystemActuator_Impl::addHaystackTag(std::string tag, std::string value) {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool temp = false;
    bool ok = false;

    temp = eg.setString(OS_EnergyManagementSystem_ActuatorExtensibleFields::HaystackTag, tag);
    ok = eg.setString(OS_EnergyManagementSystem_ActuatorExtensibleFields::HaystackValue, value);
    /*
    if (temp && ok) {
      setNumberofUserDefinedConstituents(num + 1);
    }
    else {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
    }
    */
    return temp;
  }

  void EnergyManagementSystemActuator_Impl::removeAllHaystackTags() {
    getObject<ModelObject>().clearExtensibleGroups();
    //resetNumberofHaystackTags();
  }

  void EnergyManagementSystemActuator_Impl::removeHaystackTag(unsigned groupIndex) {
    unsigned numberofDataPairs = numExtensibleGroups();
    if (groupIndex < numberofDataPairs) {
     // unsigned int num;
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      /*
      if (numberofUserDefinedConstituents()) {
      num = numberofUserDefinedConstituents().get();
      setNumberofUserDefinedConstituents(num - 1);
      }
      else {
      setNumberofUserDefinedConstituents(numExtensibleGroups());
      }
      */
    }
  }

  boost::optional<unsigned int> EnergyManagementSystemActuator_Impl::numberofHaystackTags() const {
    boost::optional<unsigned int> value;
    std::vector< std::pair<std::string, std::string> > tags;
   // tags = haystackTags();
    value = tags.size();
    return value;
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

boost::optional<ModelObject> EnergyManagementSystemActuator::actuatedComponent() const {
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

void EnergyManagementSystemActuator::removeAllHaystackTags() {
   getImpl<detail::EnergyManagementSystemActuator_Impl>()->removeAllHaystackTags();
}

void EnergyManagementSystemActuator::removeHaystackTag(const int groupIndex) {
  getImpl<detail::EnergyManagementSystemActuator_Impl>()->removeHaystackTag(groupIndex);
}

bool EnergyManagementSystemActuator::addHaystackTag(std::string tag, std::string value) {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->addHaystackTag(tag, value);
}

std::vector< std::pair<std::string, std::string> > EnergyManagementSystemActuator::haystackTags() {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->haystackTags();
}

boost::optional<unsigned int> EnergyManagementSystemActuator::numberofHaystackTags() const {
  return getImpl<detail::EnergyManagementSystemActuator_Impl>()->numberofHaystackTags();
}

/// @cond
EnergyManagementSystemActuator::EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

