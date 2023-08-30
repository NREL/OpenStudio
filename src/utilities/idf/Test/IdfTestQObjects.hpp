/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
