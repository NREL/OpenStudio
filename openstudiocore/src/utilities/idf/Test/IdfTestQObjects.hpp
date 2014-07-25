/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP
#define UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP

#include <QObject>

#include "../Workspace.hpp"
#include "../Workspace_Impl.hpp"
#include "../WorkspaceObject.hpp"

#include "../../core/Assert.hpp"

#include <boost/optional.hpp>

using namespace openstudio;

class WorkspaceReciever : public QObject {
  Q_OBJECT;
 public:

  WorkspaceReciever(const Workspace& workspace)
  {
    std::shared_ptr<openstudio::detail::Workspace_Impl> impl = workspace.getImpl<openstudio::detail::Workspace_Impl>();
    
    bool test;
    test = connect(impl.get(), SIGNAL(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this, SLOT(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   Qt::QueuedConnection);
    OS_ASSERT(test);

    test = connect(impl.get(), SIGNAL(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this, SLOT(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   Qt::QueuedConnection);
    OS_ASSERT(test);
  }

  void clear()
  {
    m_objectImpl.reset();
    m_iddObjectType.reset();
    m_handle.reset();
  }

  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> m_objectImpl;

  boost::optional<IddObjectType> m_iddObjectType;

  boost::optional<Handle> m_handle;

 public slots:

  void removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) 
  {
    m_objectImpl = object;
    m_iddObjectType = iddObjectType;
    m_handle = handle;
  }

  void addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) 
  {
    m_objectImpl = object;
    m_iddObjectType = iddObjectType;
    m_handle = handle;
  }

};

#endif // UTILITIES_IDF_TEST_IDFTESTQOBJECTS_HPP
