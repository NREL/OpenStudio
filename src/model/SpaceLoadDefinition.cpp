/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SpaceLoadDefinition.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SpaceLoadInstance.hpp"
#include "SpaceLoadInstance_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {}

    SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const SpaceLoadDefinition_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    std::vector<openstudio::IdfObject> SpaceLoadDefinition_Impl::remove() {
      std::vector<openstudio::IdfObject> temp1;
      std::vector<SpaceLoadInstance> instances = this->instances();
      for (SpaceLoadInstance& instance : instances) {
        std::vector<openstudio::IdfObject> temp2 = instance.remove();
        temp1.insert(temp1.end(), temp2.begin(), temp2.end());
      }

      std::vector<openstudio::IdfObject> result = ResourceObject_Impl::remove();
      result.insert(result.end(), temp1.begin(), temp1.end());

      return result;
    }

    std::vector<SpaceLoadInstance> SpaceLoadDefinition_Impl::instances() const {
      return getObject<ModelObject>().getModelObjectSources<SpaceLoadInstance>();
    }

    double SpaceLoadDefinition_Impl::floorArea() const {
      double result = 0;
      for (const SpaceLoadInstance& instance : this->instances()) {
        result += instance.floorArea();
      }
      return result;
    }

    int SpaceLoadDefinition_Impl::quantity() const {
      int result = 0;
      for (const SpaceLoadInstance& instance : this->instances()) {
        result += instance.quantity();
      }
      return result;
    }

  }  // namespace detail

  SpaceLoadDefinition::SpaceLoadDefinition(IddObjectType type, const Model& model) : ResourceObject(type, model) {
    OS_ASSERT(getImpl<detail::SpaceLoadDefinition_Impl>());
  }

  SpaceLoadDefinition::SpaceLoadDefinition(std::shared_ptr<detail::SpaceLoadDefinition_Impl> impl) : ResourceObject(std::move(impl)) {}

  std::vector<SpaceLoadInstance> SpaceLoadDefinition::instances() const {
    return getImpl<detail::SpaceLoadDefinition_Impl>()->instances();
  }

  double SpaceLoadDefinition::floorArea() const {
    return getImpl<detail::SpaceLoadDefinition_Impl>()->floorArea();
  }

  int SpaceLoadDefinition::quantity() const {
    return getImpl<detail::SpaceLoadDefinition_Impl>()->quantity();
  }

}  // namespace model
}  // namespace openstudio
