/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
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

AvailabilityManager_Impl::AvailabilityManager_Impl(IddObjectType type, Model_Impl* model)
  : ModelObject_Impl(type,model)
{
}

AvailabilityManager_Impl::AvailabilityManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{
}

AvailabilityManager_Impl::AvailabilityManager_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
}

AvailabilityManager_Impl::AvailabilityManager_Impl(const AvailabilityManager_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandles)
  : ModelObject_Impl(other,model,keepHandles)
{
}

boost::optional<Loop> AvailabilityManager_Impl::loop() const {
  auto t_handle = handle();
  for( const auto & loop : model().getConcreteModelObjects<AirLoopHVAC>() ) {
    if( auto availabilityManager = loop.availabilityManager() ) {
      if( availabilityManager->handle() == t_handle ) {
        return loop;
      }
    }
  }

  return boost::none;
}

} // detail

AvailabilityManager::AvailabilityManager(IddObjectType type,const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManager_Impl>());
}

AvailabilityManager::AvailabilityManager(std::shared_ptr<detail::AvailabilityManager_Impl> p)
  : ModelObject(std::move(p))
{}

boost::optional<Loop> AvailabilityManager::loop() const {
  return getImpl<detail::AvailabilityManager_Impl>()->loop();
}


} // model

} // openstudio

