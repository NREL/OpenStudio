/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkNode.hpp"
#include "AirflowNetworkNode_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AirflowNetworkNode_Impl::AirflowNetworkNode_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    AirflowNetworkNode_Impl::AirflowNetworkNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    AirflowNetworkNode_Impl::AirflowNetworkNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    AirflowNetworkNode_Impl::AirflowNetworkNode_Impl(const AirflowNetworkNode_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

  }  // namespace detail

  AirflowNetworkNode::AirflowNetworkNode(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkNode_Impl>());
  }

  AirflowNetworkNode::AirflowNetworkNode(std::shared_ptr<detail::AirflowNetworkNode_Impl> p) : ModelObject(p) {}

}  // namespace model

}  // namespace openstudio
