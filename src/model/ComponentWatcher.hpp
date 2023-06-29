/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
