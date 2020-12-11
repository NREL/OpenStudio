/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
        LOG_AND_THROW("ExteriorLoadInstance does not have a definition." << std::endl << this->idfObject());
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
