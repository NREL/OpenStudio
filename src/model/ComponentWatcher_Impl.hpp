/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMPONENTWATCHER_IMPL_HPP
#define MODEL_COMPONENTWATCHER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ComponentData.hpp"

#include "../utilities/core/Logger.hpp"

#include "../nano/nano_signal_slot.hpp"  // Signal-Slot replacement

namespace openstudio {
namespace model {

  class ComponentWatcher;

  namespace detail {

    class MODEL_API ComponentWatcher_Impl
      : public std::enable_shared_from_this<ComponentWatcher_Impl>
      , public Nano::Observer
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ComponentWatcher_Impl(ComponentData& componentData);

      virtual ~ComponentWatcher_Impl() = default;

      //@}
      /** @name Getters */
      //@{

      ComponentWatcher componentWatcher() const;

      ComponentData componentData() const;

      //@}
      /** @name Nano Signals */
      //@{

      Nano::Signal<void(const ComponentWatcher&)> obsolete;

      void dataChange();

      void nameChange();

      void componentDataChange();

      void relationshipChange(int index, Handle newHandle, Handle oldHandle);

      void objectRemove(const Handle& handleOfRemovedObject);

      void objectAdd(const WorkspaceObject& addedObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

     private:
      ComponentData m_componentData;
      std::vector<ModelObject> m_componentObjects;

      void mf_changeComponentVersion();

      void mf_refreshComponentContents(bool logWarnings);

      void mf_removeComponent();

      REGISTER_LOGGER("openstudio.model.ComponentWatcher");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTWATCHER_IMPL_HPP
