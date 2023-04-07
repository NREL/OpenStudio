/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP
#define UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP

#include "../Workspace.hpp"
#include "../Workspace_Impl.hpp"
#include "../WorkspaceObject.hpp"

#include "../../core/Assert.hpp"

#include <boost/optional.hpp>

using namespace openstudio;

class WorkspaceReciever
{
 public:
  WorkspaceReciever(const Workspace& workspace) {

    // Due to the switch to Nano Signals and Slots, testing for connects are no longer
    // necessary because the NanoS&S are guarenteed to connect, else error on compile

    std::shared_ptr<openstudio::detail::Workspace_Impl> impl = workspace.getImpl<openstudio::detail::Workspace_Impl>();

    impl->Workspace_Impl::removeWorkspaceObject.connect<WorkspaceReciever, &WorkspaceReciever::removeWorkspaceObject>(this);

    //test = connect(impl.get(), SIGNAL(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //               this, SLOT(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //               Qt::QueuedConnection);
    //OS_ASSERT(test);

    impl->Workspace_Impl::addWorkspaceObject.connect<WorkspaceReciever, &WorkspaceReciever::addWorkspaceObject>(this);

    //test = connect(impl.get(), SIGNAL(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //               this, SLOT(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
    //               Qt::QueuedConnection);
    //OS_ASSERT(test);
  }

  void clear() {
    m_objectImpl.reset();
    m_iddObjectType.reset();
    m_handle.reset();
  }

  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> m_objectImpl;

  boost::optional<IddObjectType> m_iddObjectType;

  boost::optional<Handle> m_handle;

 public:
  void removeWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) {
    m_objectImpl = object.getImpl<detail::WorkspaceObject_Impl>();
    m_iddObjectType = iddObjectType;
    m_handle = handle;
  }

  void addWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) {
    m_objectImpl = object.getImpl<detail::WorkspaceObject_Impl>();
    m_iddObjectType = iddObjectType;
    m_handle = handle;
  }
};

#endif  // UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP
