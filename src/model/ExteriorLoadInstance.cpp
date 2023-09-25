/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorLoadInstance.hpp"
#include "ExteriorLoadInstance_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ExteriorLoadDefinition.hpp"
#include "ExteriorLoadDefinition_Impl.hpp"
#include "Space.hpp"
#include "SpaceType.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorLoadInstance_Impl::ExteriorLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    ExteriorLoadInstance_Impl::ExteriorLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ExteriorLoadInstance_Impl::ExteriorLoadInstance_Impl(const ExteriorLoadInstance_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ExteriorLoadDefinition ExteriorLoadInstance_Impl::definition() const {
      OptionalExteriorLoadDefinition result = getObject<ModelObject>().getModelObjectTarget<ExteriorLoadDefinition>(this->definitionIndex());
      if (!result) {
        LOG_AND_THROW("ExteriorLoadInstance does not have a definition." << '\n' << this->idfObject());
      }
      return result.get();
    }

    void ExteriorLoadInstance_Impl::makeUnique() {
      ExteriorLoadDefinition definition = this->definition();
      ExteriorLoadInstanceVector instances = definition.instances();
      OS_ASSERT(!instances.empty());
      if (instances.size() == 1) {
        OS_ASSERT(this->handle() == instances[0].handle());
      } else {
        ModelObject clone = definition.clone(this->model());
        OS_ASSERT(this->setPointer(this->definitionIndex(), clone.handle()));
      }
    }

    int ExteriorLoadInstance_Impl::quantity() const {
      return (int)this->multiplier();
    }

  }  // namespace detail

  ExteriorLoadInstance::ExteriorLoadInstance(IddObjectType type, const ExteriorLoadDefinition& definition) : ModelObject(type, definition.model()) {
    OS_ASSERT(getImpl<detail::ExteriorLoadInstance_Impl>());
    OS_ASSERT(this->setPointer(getImpl<detail::ExteriorLoadInstance_Impl>()->definitionIndex(), definition.handle()));
  }

  ExteriorLoadInstance::ExteriorLoadInstance(std::shared_ptr<detail::ExteriorLoadInstance_Impl> impl) : ModelObject(std::move(impl)) {}

  ExteriorLoadDefinition ExteriorLoadInstance::definition() const {
    return getImpl<detail::ExteriorLoadInstance_Impl>()->definition();
  }

  bool ExteriorLoadInstance::setDefinition(const ExteriorLoadDefinition& definition) {
    return getImpl<detail::ExteriorLoadInstance_Impl>()->setDefinition(definition);
  }

  void ExteriorLoadInstance::makeUnique() {
    getImpl<detail::ExteriorLoadInstance_Impl>()->makeUnique();
  }

  double ExteriorLoadInstance::multiplier() const {
    return getImpl<detail::ExteriorLoadInstance_Impl>()->multiplier();
  }

  bool ExteriorLoadInstance::isMultiplierDefaulted() const {
    return getImpl<detail::ExteriorLoadInstance_Impl>()->isMultiplierDefaulted();
  }

  int ExteriorLoadInstance::quantity() const {
    return getImpl<detail::ExteriorLoadInstance_Impl>()->quantity();
  }

}  // namespace model
}  // namespace openstudio
