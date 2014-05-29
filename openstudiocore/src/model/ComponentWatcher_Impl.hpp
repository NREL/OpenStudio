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

#ifndef MODEL_COMPONENTWATCHER_IMPL_HPP
#define MODEL_COMPONENTWATCHER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ComponentData.hpp"

#include "../utilities/core/Logger.hpp"

#include <QObject>

namespace openstudio {
namespace model {

class ComponentWatcher;

namespace detail {

  class MODEL_API ComponentWatcher_Impl : public QObject, public std::enable_shared_from_this<ComponentWatcher_Impl> {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ComponentWatcher_Impl(ComponentData& componentData);

    virtual ~ComponentWatcher_Impl() {}

    //@}
    /** @name Getters */
    //@{

    ComponentWatcher componentWatcher() const;

    ComponentData componentData() const;

    //@}
   signals: 

    void obsolete(const ComponentWatcher& watcher);

   public slots:

    void dataChange();

    void nameChange();

    void componentDataChange();

    void relationshipChange(int index,Handle newHandle,Handle oldHandle);
    
    void objectRemove(const Handle& handleOfRemovedObject);

    void objectAdd(const WorkspaceObject& addedObject);

   private:
    ComponentData m_componentData;
    std::vector<ModelObject> m_componentObjects;

    void mf_changeComponentVersion();

    void mf_refreshComponentContents(bool logWarnings);

    void mf_removeComponent();

    REGISTER_LOGGER("openstudio.model.ComponentWatcher");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COMPONENTWATCHER_IMPL_HPP
