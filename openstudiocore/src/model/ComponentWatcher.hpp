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
}

/** ComponentWatcher uses Qt signals and slots to monitor the data associated with a Component. 
 *  \link ComponentWatcher ComponentWatchers \endlink are constructed by \link Model Models 
 *  \endlink and \link Component Components\endlink. They are not for general (public) use. 
 *  If data changes, ComponentWatcher calls componentData.createVersionUUID. The Component is 
 *  destroyed (ComponentData is removed from the model) if the user removes ComponentData or
 *  the primary object, or if the relationship between objects in the component changes. */
class MODEL_API ComponentWatcher {
 public:
  /** @name Constructors and Destructors */
  //@{

  // ETH@20110615 This constructor should probably be protected, with Model (etc.) as a friend.
  ComponentWatcher(ComponentData& componentData);

  virtual ~ComponentWatcher() {}

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
  typedef detail::ComponentWatcher_Impl ImplType;
  
  friend class detail::Model_Impl;
  friend class detail::ComponentWatcher_Impl;

  std::shared_ptr<detail::ComponentWatcher_Impl> getImpl() const;

  ComponentWatcher(std::shared_ptr<detail::ComponentWatcher_Impl> impl);

  /// @endcond
 private:
  std::shared_ptr<detail::ComponentWatcher_Impl> m_impl;
};

/** \relates ComponentWatcher */
typedef std::vector<ComponentWatcher> ComponentWatcherVector;

} // model
} // openstudio

#endif // MODEL_COMPONENTWATCHER_HPP
