/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PhotovoltaicPerformance.hpp"
#include "PhotovoltaicPerformance_Impl.hpp"
#include "GeneratorPhotovoltaic.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(const PhotovoltaicPerformance_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

    std::vector<openstudio::IdfObject> PhotovoltaicPerformance_Impl::remove() {
      std::vector<openstudio::IdfObject> result;

      if (getSources(GeneratorPhotovoltaic::iddObjectType()).empty()) {
        result = ModelObject_Impl::remove();
      }

      return result;
    }

  }  // namespace detail

  PhotovoltaicPerformance::PhotovoltaicPerformance(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::PhotovoltaicPerformance_Impl>());
  }

  PhotovoltaicPerformance::PhotovoltaicPerformance(std::shared_ptr<detail::PhotovoltaicPerformance_Impl> p) : ModelObject(std::move(p)) {}

}  // namespace model

}  // namespace openstudio
