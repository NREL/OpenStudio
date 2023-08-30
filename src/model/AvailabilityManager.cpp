/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Loop.hpp"
#include "Loop_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AvailabilityManager_Impl::AvailabilityManager_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    AvailabilityManager_Impl::AvailabilityManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    AvailabilityManager_Impl::AvailabilityManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    AvailabilityManager_Impl::AvailabilityManager_Impl(const AvailabilityManager_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

    boost::optional<Loop> AvailabilityManager_Impl::loop() const {
      auto t_handle = handle();
      for (const auto& avmList : model().getConcreteModelObjects<AvailabilityManagerAssignmentList>()) {
        for (const auto& avm : avmList.availabilityManagers()) {
          if (avm.handle() == t_handle) {
            return avmList.loop();
          }
        }
      }
      return boost::none;
    }

  }  // namespace detail

  AvailabilityManager::AvailabilityManager(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::AvailabilityManager_Impl>());
  }

  AvailabilityManager::AvailabilityManager(std::shared_ptr<detail::AvailabilityManager_Impl> p) : ModelObject(std::move(p)) {}

  boost::optional<Loop> AvailabilityManager::loop() const {
    return getImpl<detail::AvailabilityManager_Impl>()->loop();
  }

}  // namespace model

}  // namespace openstudio
