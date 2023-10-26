/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorLoadDefinition.hpp"
#include "ExteriorLoadDefinition_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ExteriorLoadInstance.hpp"
#include "ExteriorLoadInstance_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorLoadDefinition_Impl::ExteriorLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {}

    ExteriorLoadDefinition_Impl::ExteriorLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    ExteriorLoadDefinition_Impl::ExteriorLoadDefinition_Impl(const ExteriorLoadDefinition_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    std::vector<openstudio::IdfObject> ExteriorLoadDefinition_Impl::remove() {
      std::vector<openstudio::IdfObject> temp1;
      std::vector<ExteriorLoadInstance> instances = this->instances();
      for (ExteriorLoadInstance& instance : instances) {
        std::vector<openstudio::IdfObject> temp2 = instance.remove();
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
      }

      std::vector<openstudio::IdfObject> result = ResourceObject_Impl::remove();
      result.insert(result.end(), temp1.begin(), temp1.end());

      return result;
    }

    std::vector<ExteriorLoadInstance> ExteriorLoadDefinition_Impl::instances() const {
      return getObject<ModelObject>().getModelObjectSources<ExteriorLoadInstance>();
    }

    int ExteriorLoadDefinition_Impl::quantity() const {
      int result = 0;
      for (const ExteriorLoadInstance& instance : this->instances()) {
        result += instance.quantity();
      }
      return result;
    }

  }  // namespace detail

  ExteriorLoadDefinition::ExteriorLoadDefinition(IddObjectType type, const Model& model) : ResourceObject(type, model) {
    OS_ASSERT(getImpl<detail::ExteriorLoadDefinition_Impl>());
  }

  ExteriorLoadDefinition::ExteriorLoadDefinition(std::shared_ptr<detail::ExteriorLoadDefinition_Impl> impl) : ResourceObject(std::move(impl)) {}

  std::vector<ExteriorLoadInstance> ExteriorLoadDefinition::instances() const {
    return getImpl<detail::ExteriorLoadDefinition_Impl>()->instances();
  }

  int ExteriorLoadDefinition::quantity() const {
    return getImpl<detail::ExteriorLoadDefinition_Impl>()->quantity();
  }

}  // namespace model
}  // namespace openstudio
