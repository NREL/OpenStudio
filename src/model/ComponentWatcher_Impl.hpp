/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
      , public Nano::Observer<>
    {

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
