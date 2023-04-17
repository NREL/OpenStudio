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

#ifndef MODEL_COMPONENTWATCHER_HPP
#define MODEL_COMPONENTWATCHER_HPP

#include "ModelAPI.hpp"

#include <memory>
#include <vector>

namespace openstudio {
namespace model {

  class ComponentData;

  namespace detail {
    class Model_Impl;
    class ComponentWatcher_Impl;
  }  // namespace detail

  /** ComponentWatcher uses signals and slots to monitor the data associated with a Component.
 *  \link ComponentWatcher ComponentWatchers \endlink are constructed by \link Model Models
 *  \endlink and \link Component Components\endlink. They are not for general (public) use.
 *  If data changes, ComponentWatcher calls componentData.createVersionUUID. The Component is
 *  destroyed (ComponentData is removed from the model) if the user removes ComponentData or
 *  the primary object, or if the relationship between objects in the component changes. */
  class MODEL_API ComponentWatcher
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // ETH@20110615 This constructor should probably be protected, with Model (etc.) as a friend.
    ComponentWatcher(ComponentData& componentData);

    virtual ~ComponentWatcher() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ComponentWatcher(const ComponentWatcher& other) = default;
    ComponentWatcher(ComponentWatcher&& other) = default;
    ComponentWatcher& operator=(const ComponentWatcher&) = default;
    ComponentWatcher& operator=(ComponentWatcher&&) = default;

    //@}
    /** @name Getters */
    //@{

    ComponentData componentData() const;

    //@}
    /** @name Queries */
    //@{

    bool operator==(const ComponentWatcher& other);

    bool operator!=(const ComponentWatcher& other);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ComponentWatcher_Impl;

    friend class detail::Model_Impl;
    friend class detail::ComponentWatcher_Impl;

    std::shared_ptr<detail::ComponentWatcher_Impl> getImpl() const;

    ComponentWatcher(std::shared_ptr<detail::ComponentWatcher_Impl> impl);

    /// @endcond
   private:
    std::shared_ptr<detail::ComponentWatcher_Impl> m_impl;
  };

  /** \relates ComponentWatcher */
  using ComponentWatcherVector = std::vector<ComponentWatcher>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTWATCHER_HPP
