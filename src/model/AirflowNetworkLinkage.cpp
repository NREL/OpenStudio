/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkLinkage.hpp"
#include "AirflowNetworkLinkage_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    AirflowNetworkLinkage_Impl::AirflowNetworkLinkage_Impl(const AirflowNetworkLinkage_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

  }  // namespace detail

  AirflowNetworkLinkage::AirflowNetworkLinkage(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkLinkage_Impl>());
  }

  AirflowNetworkLinkage::AirflowNetworkLinkage(std::shared_ptr<detail::AirflowNetworkLinkage_Impl> impl) : ModelObject(impl) {}

}  // namespace model

}  // namespace openstudio
