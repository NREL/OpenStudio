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

#ifndef MODEL_MODELEXTENSIBLEGROUP_HPP
#define MODEL_MODELEXTENSIBLEGROUP_HPP

#include "ModelAPI.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio {
namespace model {

/** ModelExtensibleGroup is a WorkspaceExtensibleGroup with additional methods added to support 
 *  extensible \link ModelObject ModelObjects\endlink. The ExtensibleGroup inheritance hierarchy
 *  roughly follows the IdfObject <- WorkspaceObject <- ModelObject <- etc. hierarchy, except that
 *  all concrete \link ModelObject ModelObjects \endlink with ExtensibleGroup derived classes 
 *  derive directly from ModelExtensibleGroup--there are no extensions of ModelExtensibleGroup for
 *  intermediate abstract classes. */
class MODEL_API ModelExtensibleGroup : public WorkspaceExtensibleGroup {
 public:
  virtual ~ModelExtensibleGroup() {}

  /** @name Template Methods */
  //@{

  /** Get the ModelObject of type T pointed to from fieldIndex of this extensible group, if 
   *  possible. */
  template <typename T>
  boost::optional<T> getModelObjectTarget(unsigned fieldIndex) const {
    if (!isValid(fieldIndex)) { return boost::none; }
    return ModelObject(getImpl<detail::ModelObject_Impl>()).getModelObjectTarget<T>(mf_toIndex(fieldIndex));
  }

  /** Get all the \link ModelObject ModelObjects \endlink of type T pointed to from this 
   *  extensible group. Return value will always be .empty() if this extensible group object is
   *  .empty(). */
  template <typename T>
  std::vector<T> getModelObjectTargets() const {
    std::vector<T> result;
    if (empty()) { return result; }
    for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
      boost::optional<T> oTarget = getModelObjectTarget<T>(fieldIndex);
      if (oTarget) { result.push_back(*oTarget); }
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::ModelObject_Impl ImplType;

  friend class IdfExtensibleGroup;

  ModelExtensibleGroup(std::shared_ptr<detail::ModelObject_Impl> impl,unsigned index);

  /// @endcond
 private:

  /** Private default constructor. */
  ModelExtensibleGroup();

  REGISTER_LOGGER("openstudio.model.ModelExtensibleGroup");
};

} // model
} // openstudio

#endif // MODEL_MODELEXTENSIBLEGROUP_HPPP
