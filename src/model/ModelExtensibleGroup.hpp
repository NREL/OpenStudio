/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  class Schedule;

  /** ModelExtensibleGroup is a WorkspaceExtensibleGroup with additional methods added to support
 *  extensible \link ModelObject ModelObjects\endlink. The ExtensibleGroup inheritance hierarchy
 *  roughly follows the IdfObject <- WorkspaceObject <- ModelObject <- etc. hierarchy, except that
 *  all concrete \link ModelObject ModelObjects \endlink with ExtensibleGroup derived classes
 *  derive directly from ModelExtensibleGroup--there are no extensions of ModelExtensibleGroup for
 *  intermediate abstract classes. */
  class MODEL_API ModelExtensibleGroup : public WorkspaceExtensibleGroup
  {
   public:
    virtual ~ModelExtensibleGroup() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ModelExtensibleGroup(const ModelExtensibleGroup& other) = default;
    ModelExtensibleGroup(ModelExtensibleGroup&& other) = default;
    ModelExtensibleGroup& operator=(const ModelExtensibleGroup&) = default;
    ModelExtensibleGroup& operator=(ModelExtensibleGroup&&) = default;

    /** @name Template Methods */
    //@{

    /** Get the ModelObject of type T pointed to from fieldIndex of this extensible group, if
   *  possible. */
    template <typename T>
    boost::optional<T> getModelObjectTarget(unsigned fieldIndex) const {
      if (!isValid(fieldIndex)) {
        return boost::none;
      }
      return ModelObject(getImpl<detail::ModelObject_Impl>()).getModelObjectTarget<T>(mf_toIndex(fieldIndex));
    }

    /** Get all the \link ModelObject ModelObjects \endlink of type T pointed to from this
   *  extensible group. Return value will always be .empty() if this extensible group object is
   *  .empty(). */
    template <typename T>
    std::vector<T> getModelObjectTargets() const {
      std::vector<T> result;
      if (empty()) {
        return result;
      }
      for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
        boost::optional<T> oTarget = getModelObjectTarget<T>(fieldIndex);
        if (oTarget) {
          result.push_back(*oTarget);
        }
      }
      return result;
    }

    /** Sets index to point to schedule if schedule's ScheduleTypeLimits are compatible with the
   *  ScheduleType in the ScheduleTypeRegistry for (className,scheduleDisplayName), or if
   *  schedule's ScheduleTypeLimits have not yet been set (in which case the ScheduleTypeRegistry
   *  is used to retrieve or create an appropriate one).
   *  Because ModelExtensibleGroup has no knowledge of its model,
   *  you should ensure that schedule belongs to the right model in the first place */
    bool setSchedule(unsigned index, const std::string& className, const std::string& scheduleDisplayName, Schedule& schedule);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ModelObject_Impl;

    friend class IdfExtensibleGroup;

    ModelExtensibleGroup(std::shared_ptr<detail::ModelObject_Impl> impl, unsigned index);

    /// @endcond
   private:
    /** Private default constructor. */
    ModelExtensibleGroup();

    REGISTER_LOGGER("openstudio.model.ModelExtensibleGroup");
  };

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODELEXTENSIBLEGROUP_HPPP
