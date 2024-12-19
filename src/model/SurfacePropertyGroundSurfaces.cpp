/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SurfacePropertyGroundSurfaces.hpp"
#include "SurfacePropertyGroundSurfaces_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ModelExtensibleGroup.hpp"
#include "SurfacePropertyLocalEnvironment.hpp"
#include "SurfacePropertyLocalEnvironment_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_GroundSurfaces_FieldEnums.hxx>
#include <utility>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  /***************************************************************************************************************************************************
   *                                             G R O U N D    S U R F A C E    G R O U P    W R A P P E R                                          *
   **************************************************************************************************************************************************/

  GroundSurfaceGroup::GroundSurfaceGroup(std::string groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule,
                                         boost::optional<Schedule> reflectanceSchedule)
    : m_groundSurfaceName(std::move(groundSurfaceName)),
      m_viewFactor(viewFactor),
      m_temperatureSch(std::move(temperatureSchedule)),
      m_reflectanceSch(std::move(reflectanceSchedule)) {

    // Matches the maximum value in IDD
    if (m_viewFactor > 1) {
      LOG_AND_THROW("Unable to create GroundSurfaceGroup, View Factor of " << m_viewFactor << " cannot be more than 1.");
    } else if (m_viewFactor < 0) {
      LOG_AND_THROW("Unable to create GroundSurfaceGroup, View Factor of " << m_viewFactor << " cannot be less than 0.");
    }

    if (m_groundSurfaceName.empty()) {
      LOG_AND_THROW("Unable to create GroundSurfaceGroup, GroundSurfaceName cannot be empty.");
    }
  }

  std::string GroundSurfaceGroup::groundSurfaceName() const {
    return m_groundSurfaceName;
  }

  double GroundSurfaceGroup::viewFactor() const {
    return m_viewFactor;
  }

  boost::optional<Schedule> GroundSurfaceGroup::temperatureSchedule() const {
    return m_temperatureSch;
  }

  boost::optional<Schedule> GroundSurfaceGroup::reflectanceSchedule() const {
    return m_reflectanceSch;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::GroundSurfaceGroup& groundSurfaceGroup) {
    out << "(Surface = '" << groundSurfaceGroup.groundSurfaceName() << "', " << "View Factor = " << groundSurfaceGroup.viewFactor() << ", ";
    if (groundSurfaceGroup.temperatureSchedule()) {
      out << "Temperature Schedule = '" << groundSurfaceGroup.temperatureSchedule()->nameString() << "', ";
    } else {
      out << "No Temperature Schedule Assigned, ";
    }
    if (groundSurfaceGroup.reflectanceSchedule()) {
      out << "Reflectance Schedule = '" << groundSurfaceGroup.reflectanceSchedule()->nameString() << "')";
    } else {
      out << "No Reflectance Schedule Assigned)";
    }
    return out;
  }

  namespace detail {

    SurfacePropertyGroundSurfaces_Impl::SurfacePropertyGroundSurfaces_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyGroundSurfaces::iddObjectType());
    }

    SurfacePropertyGroundSurfaces_Impl::SurfacePropertyGroundSurfaces_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertyGroundSurfaces::iddObjectType());
    }

    SurfacePropertyGroundSurfaces_Impl::SurfacePropertyGroundSurfaces_Impl(const SurfacePropertyGroundSurfaces_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertyGroundSurfaces_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Surfaces Property Ground Surfaces Average Temperature",
                                                   "Surfaces Property Ground Surfaces Average Reflectance"};
      return result;
    }

    IddObjectType SurfacePropertyGroundSurfaces_Impl::iddObjectType() const {
      return SurfacePropertyGroundSurfaces::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SurfacePropertyGroundSurfaces_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;

      // This is unusual, but the schedule is in the Extensible Fields (and only there)
      // UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector fieldIndices;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        UnsignedVector thisFieldIndices = group.getSourceFieldIndices(schedule.handle());
        fieldIndices.insert(fieldIndices.end(), thisFieldIndices.begin(), thisFieldIndices.end());
      }

      // Make unique, have to sort before calling unique, unique only works on consecutive elements
      std::sort(fieldIndices.begin(), fieldIndices.end());
      fieldIndices.erase(std::unique(fieldIndices.begin(), fieldIndices.end()), fieldIndices.end());

      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName) != e) {
        result.emplace_back("SurfacePropertyGroundSurfaces", "Ground Surface Temperature");
      } else if (std::find(b, e, OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName) != e) {
        result.emplace_back("SurfacePropertyGroundSurfaces", "Ground Surface Reflectance");
      }

      return result;
    }

    // Extensible groups

    unsigned int SurfacePropertyGroundSurfaces_Impl::numberofGroundSurfaceGroups() const {
      return numExtensibleGroups();
    }

    boost::optional<unsigned> SurfacePropertyGroundSurfaces_Impl::groundSurfaceGroupIndex(const std::string& groundSurfaceName) const {
      boost::optional<unsigned> result;

      // Find with custom predicate, checking handle equality between the toSurface and the fromSurface pairs
      // We do it with extensibleGroups() (rather than groundSurfaceGroups()) and getString to avoid overhead
      // of manipulating actual model objects (getTarget, then create a GroundSurfaceGroups wrapper, get handle convert to string...)
      // and speed up the routine
      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto it = std::find_if(egs.begin(), egs.end(), [&groundSurfaceName](const WorkspaceExtensibleGroup& eg) {
        return openstudio::istringEqual(eg.getField(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName).get(), groundSurfaceName);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    boost::optional<unsigned> SurfacePropertyGroundSurfaces_Impl::groundSurfaceGroupIndex(const GroundSurfaceGroup& groundSurfaceGroup) const {
      return groundSurfaceGroupIndex(groundSurfaceGroup.groundSurfaceName());
    }

    boost::optional<GroundSurfaceGroup> SurfacePropertyGroundSurfaces_Impl::getGroundSurfaceGroup(unsigned groupIndex) const {

      boost::optional<GroundSurfaceGroup> result;

      if (groupIndex >= numberofGroundSurfaceGroups()) {
        LOG(Error, "Asked to get GroundSurfaceGroup with index " << groupIndex << ", but " << briefDescription() << " has just "
                                                                 << numberofGroundSurfaceGroups() << " GroundSurfaceGroups.");
        return result;
      }

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

      boost::optional<std::string> surfaceName_ = group.getString(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName);
      boost::optional<double> viewFactor_ = group.getDouble(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor);
      boost::optional<Schedule> tempSch_ =
        group.getModelObjectTarget<Schedule>(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName);

      boost::optional<Schedule> reflectanceSch_ =
        group.getModelObjectTarget<Schedule>(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName);

      if (!surfaceName_) {
        LOG(Error, "Could not retrieve GroundSurfaceName for extensible group " << group.groupIndex() << ".");
      }
      if (!viewFactor_) {
        LOG(Error, "Could not retrieve GroundSurfaceViewFactor for extensible group " << group.groupIndex() << ".");
      }

      // Schedules are optional

      result = GroundSurfaceGroup(surfaceName_.get(), viewFactor_.get(), tempSch_, reflectanceSch_);

      return result;
    }

    bool SurfacePropertyGroundSurfaces_Impl::addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup) {

      // Bounds checking happens in the GroundSurfaceGroup ctor

      // Check if viewFactor already exists
      boost::optional<unsigned> _existingIndex = groundSurfaceGroupIndex(groundSurfaceGroup);
      if (_existingIndex) {
        boost::optional<GroundSurfaceGroup> _group = getGroundSurfaceGroup(_existingIndex.get());
        OS_ASSERT(_group);
        LOG(Warn, "For " << briefDescription() << ", GroundSurfaceGroup already exists, will be modified in place from " << _group.get() << " to "
                         << groundSurfaceGroup << ".");
      }

      // If existing, get it, otherwise Push an extensible group. ModelExtensibleGroup cannot be default-constructed, so use a ternary operator
      // Given that we will potentially push many extensible groups
      // we will suffer from going through the internal checks in WorkspaceObject, especially in setPointer,
      // and performance will be roughly O(n^2) when adding groups.
      // So to improve the performance we pushExtensible group without validity check, and call setPointer without validity check (`isValid`)
      std::vector<std::string> temp;
      ModelExtensibleGroup eg = (_existingIndex ? getExtensibleGroup(_existingIndex.get()).cast<ModelExtensibleGroup>()
                                                : pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>());

      bool result = false;

      bool surfaceNameOk = eg.setString(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName, groundSurfaceGroup.groundSurfaceName());

      bool viewFactorOk = eg.setDouble(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor, groundSurfaceGroup.viewFactor());

      bool schTempOk = true;
      if (auto sch_ = groundSurfaceGroup.temperatureSchedule()) {
        schTempOk = eg.setPointer(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName, sch_->handle(), false);
        if (!schTempOk) {
          LOG(Error, "Unable to add Ground Surface Group which has an incompatible Temperature Schedule to " << briefDescription());
          OS_ASSERT(false);
        }
      } else {
        eg.setString(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName, "");
      }

      bool schRefOk = true;
      if (auto sch_ = groundSurfaceGroup.reflectanceSchedule()) {
        schRefOk = eg.setPointer(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName, sch_->handle(), false);
        if (!schRefOk) {
          LOG(Error, "Unable to add Ground Surface Group which has an incompatible Reflectance Schedule to " << briefDescription());

          OS_ASSERT(false);
        }
      } else {
        eg.setString(OS_SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName, "");
      }

      if (surfaceNameOk && viewFactorOk && schTempOk && schRefOk) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    // Overloads, it creates a GroundSurfaceGroup wrapper, then call `addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup)`
    bool SurfacePropertyGroundSurfaces_Impl::addGroundSurfaceGroup(const std::string& groundSurfaceName, double viewFactor,
                                                                   boost::optional<Schedule> temperatureSchedule,
                                                                   boost::optional<Schedule> reflectanceSchedule) {
      GroundSurfaceGroup groundSurfaceGroup(groundSurfaceName, viewFactor, std::move(temperatureSchedule), std::move(reflectanceSchedule));
      return addGroundSurfaceGroup(groundSurfaceGroup);
    }

    bool SurfacePropertyGroundSurfaces_Impl::addGroundSurfaceGroups(const std::vector<GroundSurfaceGroup>& groundSurfaceGroups) {
      bool result = true;

      for (const auto& groundSurfaceGroup : groundSurfaceGroups) {
        bool thisResult = addGroundSurfaceGroup(groundSurfaceGroup);
        if (!thisResult) {
          LOG(Error, "Could not add groundSurfaceGroup " << groundSurfaceGroup << " to " << briefDescription() << ". Continuing with others.");
          // OS_ASSERT(false);
          // result = false;
        }
      }

      return result;
    }

    bool SurfacePropertyGroundSurfaces_Impl::removeGroundSurfaceGroup(unsigned groupIndex) {
      bool result = false;

      unsigned int num = numberofGroundSurfaceGroups();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      }
      return result;
    }

    void SurfacePropertyGroundSurfaces_Impl::removeAllGroundSurfaceGroups() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    std::vector<GroundSurfaceGroup> SurfacePropertyGroundSurfaces_Impl::groundSurfaceGroups() const {
      std::vector<GroundSurfaceGroup> result;

      for (unsigned i = 0; i < numberofGroundSurfaceGroups(); ++i) {

        boost::optional<GroundSurfaceGroup> group_ = getGroundSurfaceGroup(i);

        // getViewFactor is responsible for handling error and issuing Error log messages.
        // Here we add it to the result array if it worked, and if it didn't, we keep going
        // We just issue a message about index so user can delete it easily
        if (group_) {
          result.push_back(group_.get());
        } else {
          LOG(Error, briefDescription() << " has an invalid GroundSurfaceGroup group at index " << i);
        }
      }

      return result;
    }

    // Helper
    boost::optional<SurfacePropertyLocalEnvironment> SurfacePropertyGroundSurfaces_Impl::surfacePropertyLocalEnvironment() const {
      std::vector<SurfacePropertyLocalEnvironment> localEnvs =
        getObject<ModelObject>().getModelObjectSources<SurfacePropertyLocalEnvironment>(SurfacePropertyLocalEnvironment::iddObjectType());
      if (localEnvs.empty()) {
        return boost::none;
      } else if (localEnvs.size() == 1) {
        return localEnvs.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyLocalEnvironments point to this SurfacePropertyGroundSurfaces");
        return boost::none;
      }
    }

  }  // namespace detail

  SurfacePropertyGroundSurfaces::SurfacePropertyGroundSurfaces(const Model& model)
    : ModelObject(SurfacePropertyGroundSurfaces::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SurfacePropertyGroundSurfaces_Impl>());
  }

  IddObjectType SurfacePropertyGroundSurfaces::iddObjectType() {
    return {IddObjectType::OS_SurfaceProperty_GroundSurfaces};
  }

  // Extensible fields

  std::vector<GroundSurfaceGroup> SurfacePropertyGroundSurfaces::groundSurfaceGroups() const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->groundSurfaceGroups();
  }

  unsigned int SurfacePropertyGroundSurfaces::numberofGroundSurfaceGroups() const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->numberofGroundSurfaceGroups();
  }

  boost::optional<unsigned> SurfacePropertyGroundSurfaces::groundSurfaceGroupIndex(const GroundSurfaceGroup& groundSurfaceGroup) const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->groundSurfaceGroupIndex(groundSurfaceGroup);
  }
  boost::optional<unsigned> SurfacePropertyGroundSurfaces::groundSurfaceGroupIndex(const std::string& groundSurfaceName) const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->groundSurfaceGroupIndex(groundSurfaceName);
  }

  boost::optional<GroundSurfaceGroup> SurfacePropertyGroundSurfaces::getGroundSurfaceGroup(unsigned groupIndex) const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->getGroundSurfaceGroup(groupIndex);
  }

  bool SurfacePropertyGroundSurfaces::addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup) {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->addGroundSurfaceGroup(groundSurfaceGroup);
  }

  // Overloads, it creates a GroundSurfaceGroup wrapper, then call `addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup)`
  bool SurfacePropertyGroundSurfaces::addGroundSurfaceGroup(const std::string& groundSurfaceName, double viewFactor,
                                                            boost::optional<Schedule> temperatureSchedule,
                                                            boost::optional<Schedule> reflectanceSchedule) {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->addGroundSurfaceGroup(groundSurfaceName, viewFactor, std::move(temperatureSchedule),
                                                                                        std::move(reflectanceSchedule));
  }

  bool SurfacePropertyGroundSurfaces::addGroundSurfaceGroups(const std::vector<GroundSurfaceGroup>& groundSurfaceGroups) {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->addGroundSurfaceGroups(groundSurfaceGroups);
  }

  bool SurfacePropertyGroundSurfaces::removeGroundSurfaceGroup(unsigned groupIndex) {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->removeGroundSurfaceGroup(groupIndex);
  }

  void SurfacePropertyGroundSurfaces::removeAllGroundSurfaceGroups() {
    getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->removeAllGroundSurfaceGroups();
  }

  boost::optional<SurfacePropertyLocalEnvironment> SurfacePropertyGroundSurfaces::surfacePropertyLocalEnvironment() const {
    return getImpl<detail::SurfacePropertyGroundSurfaces_Impl>()->surfacePropertyLocalEnvironment();
  }

  /// @cond
  SurfacePropertyGroundSurfaces::SurfacePropertyGroundSurfaces(std::shared_ptr<detail::SurfacePropertyGroundSurfaces_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
