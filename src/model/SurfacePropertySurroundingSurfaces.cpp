/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SurfacePropertySurroundingSurfaces.hpp"
#include "SurfacePropertySurroundingSurfaces_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ModelExtensibleGroup.hpp"
#include "SurfacePropertyLocalEnvironment.hpp"
#include "SurfacePropertyLocalEnvironment_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/units/Unit.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_SurroundingSurfaces_FieldEnums.hxx>

namespace openstudio {
namespace model {

  /***************************************************************************************************************************************************
   *                                       S U R R O U N D I N G    S U R F A C E    G R O U P    W R A P P E R                                     *
   **************************************************************************************************************************************************/

  SurroundingSurfaceGroup::SurroundingSurfaceGroup(std::string surroundingSurfaceName, double viewFactor, const Schedule& temperatureSchedule)
    : m_surroundingSurfaceName(std::move(surroundingSurfaceName)), m_viewFactor(viewFactor), m_tempSch(temperatureSchedule) {

    // Matches the maximum value in IDD
    if (m_viewFactor > 1) {
      LOG_AND_THROW("Unable to create SurroundingSurfaceGroup, View Factor of " << m_viewFactor << " cannot be more than 1.");
    } else if (m_viewFactor < 0) {
      LOG_AND_THROW("Unable to create SurroundingSurfaceGroup, View Factor of " << m_viewFactor << " cannot be less than 0.");
    }

    if (m_surroundingSurfaceName.empty()) {
      LOG_AND_THROW("Unable to create SurroundingSurfaceGroup, SurroundingSurfaceName cannot be empty.");
    }
  }

  std::string SurroundingSurfaceGroup::surroundingSurfaceName() const {
    return m_surroundingSurfaceName;
  }

  double SurroundingSurfaceGroup::viewFactor() const {
    return m_viewFactor;
  }

  Schedule SurroundingSurfaceGroup::temperatureSchedule() const {
    return m_tempSch;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::SurroundingSurfaceGroup& surroundingSurfaceGroup) {
    out << "(Surface = '" << surroundingSurfaceGroup.surroundingSurfaceName() << "', " << "View Factor = " << surroundingSurfaceGroup.viewFactor()
        << ", " << "Schedule = '" << surroundingSurfaceGroup.temperatureSchedule().nameString() << "')";
    return out;
  }

  namespace detail {

    SurfacePropertySurroundingSurfaces_Impl::SurfacePropertySurroundingSurfaces_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertySurroundingSurfaces::iddObjectType());
    }

    SurfacePropertySurroundingSurfaces_Impl::SurfacePropertySurroundingSurfaces_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertySurroundingSurfaces::iddObjectType());
    }

    SurfacePropertySurroundingSurfaces_Impl::SurfacePropertySurroundingSurfaces_Impl(const SurfacePropertySurroundingSurfaces_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertySurroundingSurfaces_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType SurfacePropertySurroundingSurfaces_Impl::iddObjectType() const {
      return SurfacePropertySurroundingSurfaces::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SurfacePropertySurroundingSurfaces_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;

      {
        UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
        UnsignedVector::const_iterator b(fieldIndices.begin());
        UnsignedVector::const_iterator e(fieldIndices.end());
        if (std::find(b, e, OS_SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName) != e) {
          result.emplace_back("SurfacePropertySurroundingSurfaces", "Sky Temperature");
        }
        if (std::find(b, e, OS_SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName) != e) {
          result.emplace_back("SurfacePropertySurroundingSurfaces", "Ground Temperature");
        }
      }

      {
        // This is unusual, but there is also a schedule in the Extensible Fields
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
        if (std::find(b, e, OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName) != e) {
          result.emplace_back("SurfacePropertySurroundingSurfaces", "Surrounding Surface Temperature Schedule");
        }
      }

      return result;
    }

    boost::optional<double> SurfacePropertySurroundingSurfaces_Impl::skyViewFactor() const {
      return getDouble(OS_SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, true);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::isSkyViewFactorAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    boost::optional<Schedule> SurfacePropertySurroundingSurfaces_Impl::skyTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName);
    }

    boost::optional<double> SurfacePropertySurroundingSurfaces_Impl::groundViewFactor() const {
      return getDouble(OS_SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, true);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::isGroundViewFactorAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    boost::optional<Schedule> SurfacePropertySurroundingSurfaces_Impl::groundTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::setSkyViewFactor(double skyViewFactor) {
      bool result = setDouble(OS_SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, skyViewFactor);
      return result;
    }

    void SurfacePropertySurroundingSurfaces_Impl::autocalculateSkyViewFactor() {
      bool result = setString(OS_SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, "autocalculate");
      OS_ASSERT(result);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::setSkyTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName, "SurfacePropertySurroundingSurfaces",
                                "Sky Temperature", schedule);
      return result;
    }

    void SurfacePropertySurroundingSurfaces_Impl::resetSkyTemperatureSchedule() {
      bool result = setString(OS_SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::setGroundViewFactor(double groundViewFactor) {
      bool result = setDouble(OS_SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, groundViewFactor);
      return result;
    }

    void SurfacePropertySurroundingSurfaces_Impl::autocalculateGroundViewFactor() {
      bool result = setString(OS_SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, "autocalculate");
      OS_ASSERT(result);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::setGroundTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName, "SurfacePropertySurroundingSurfaces",
                                "Ground Temperature", schedule);
      return result;
    }

    void SurfacePropertySurroundingSurfaces_Impl::resetGroundTemperatureSchedule() {
      bool result = setString(OS_SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    // Extensible groups

    unsigned int SurfacePropertySurroundingSurfaces_Impl::numberofSurroundingSurfaceGroups() const {
      return numExtensibleGroups();
    }

    boost::optional<unsigned> SurfacePropertySurroundingSurfaces_Impl::surroundingSurfaceGroupIndex(const std::string& surroundingSurfaceName) const {
      boost::optional<unsigned> result;

      // Find with custom predicate, checking handle equality between the toSurface and the fromSurface pairs
      // We do it with extensibleGroups() (rather than surroundingSurfaceGroups()) and getString to avoid overhead
      // of manipulating actual model objects (getTarget, then create a SurroundingSurfaceGroups wrapper, get handle convert to string...)
      // and speed up the routine
      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto it = std::find_if(egs.begin(), egs.end(), [&surroundingSurfaceName](const WorkspaceExtensibleGroup& eg) {
        return openstudio::istringEqual(eg.getField(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName).get(),
                                        surroundingSurfaceName);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    boost::optional<unsigned>
      SurfacePropertySurroundingSurfaces_Impl::surroundingSurfaceGroupIndex(const SurroundingSurfaceGroup& surroundingSurfaceGroup) const {
      return surroundingSurfaceGroupIndex(surroundingSurfaceGroup.surroundingSurfaceName());
    }

    boost::optional<SurroundingSurfaceGroup> SurfacePropertySurroundingSurfaces_Impl::getSurroundingSurfaceGroup(unsigned groupIndex) const {

      boost::optional<SurroundingSurfaceGroup> result;

      if (groupIndex >= numberofSurroundingSurfaceGroups()) {
        LOG(Error, "Asked to get SurroundingSurfaceGroup with index " << groupIndex << ", but " << briefDescription() << " has just "
                                                                      << numberofSurroundingSurfaceGroups() << " SurroundingSurfaceGroups.");
        return result;
      }

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

      boost::optional<std::string> surfaceName_ = group.getString(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName);
      boost::optional<double> viewFactor_ = group.getDouble(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor);
      boost::optional<Schedule> tempSch_ =
        group.getModelObjectTarget<Schedule>(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName);

      if (!surfaceName_) {
        LOG(Error, "Could not retrieve SurroundingSurfaceName for extensible group " << group.groupIndex() << ".");
      }
      if (!viewFactor_) {
        LOG(Error, "Could not retrieve SurroundingSurfaceViewFactor for extensible group " << group.groupIndex() << ".");
      }
      if (!tempSch_) {
        LOG(Error, "Could not retrieve SurroundingSurfaceTemperatureScheduleName for extensible group " << group.groupIndex() << ".");
      }

      result = SurroundingSurfaceGroup(surfaceName_.get(), viewFactor_.get(), tempSch_.get());

      return result;
    }

    bool SurfacePropertySurroundingSurfaces_Impl::addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup) {

      // Bounds checking happens in the SurroundingSurfaceGroup ctor

      // Check if viewFactor already exists
      boost::optional<unsigned> _existingIndex = surroundingSurfaceGroupIndex(surroundingSurfaceGroup);
      if (_existingIndex) {
        boost::optional<SurroundingSurfaceGroup> _group = getSurroundingSurfaceGroup(_existingIndex.get());
        OS_ASSERT(_group);
        LOG(Warn, "For " << briefDescription() << ", SurroundingSurfaceGroup already exists, will be modified in place from " << _group.get()
                         << " to " << surroundingSurfaceGroup << ".");
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

      bool surfaceNameOk = eg.setString(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName,
                                        surroundingSurfaceGroup.surroundingSurfaceName());

      bool viewFactorOk =
        eg.setDouble(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor, surroundingSurfaceGroup.viewFactor());

      auto sch = surroundingSurfaceGroup.temperatureSchedule();
      bool schOk =
        eg.setPointer(OS_SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, sch.handle(), false);
      if (!schOk) {
        LOG(Error, "Unable to add View Factor which has an incompatible toSurface object to " << briefDescription());
        OS_ASSERT(false);
      }

      if (surfaceNameOk && viewFactorOk && schOk) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    // Overloads, it creates a SurroundingSurfaceGroup wrapper, then call `addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup)`
    bool SurfacePropertySurroundingSurfaces_Impl::addSurroundingSurfaceGroup(const std::string& surroundingSurfaceName, double viewFactor,
                                                                             const Schedule& temperatureSchedule) {
      SurroundingSurfaceGroup surroundingSurfaceGroup(surroundingSurfaceName, viewFactor, temperatureSchedule);
      return addSurroundingSurfaceGroup(surroundingSurfaceGroup);
    }

    bool SurfacePropertySurroundingSurfaces_Impl::addSurroundingSurfaceGroups(const std::vector<SurroundingSurfaceGroup>& surroundingSurfaceGroups) {
      bool result = true;

      for (const auto& surroundingSurfaceGroup : surroundingSurfaceGroups) {
        bool thisResult = addSurroundingSurfaceGroup(surroundingSurfaceGroup);
        if (!thisResult) {
          LOG(Error,
              "Could not add surroundingSurfaceGroup " << surroundingSurfaceGroup << " to " << briefDescription() << ". Continuing with others.");
          // OS_ASSERT(false);
          // result = false;
        }
      }

      return result;
    }

    bool SurfacePropertySurroundingSurfaces_Impl::removeSurroundingSurfaceGroup(unsigned groupIndex) {
      bool result = false;

      unsigned int num = numberofSurroundingSurfaceGroups();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      }
      return result;
    }

    void SurfacePropertySurroundingSurfaces_Impl::removeAllSurroundingSurfaceGroups() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    std::vector<SurroundingSurfaceGroup> SurfacePropertySurroundingSurfaces_Impl::surroundingSurfaceGroups() const {
      std::vector<SurroundingSurfaceGroup> result;

      for (unsigned i = 0; i < numberofSurroundingSurfaceGroups(); ++i) {

        boost::optional<SurroundingSurfaceGroup> group_ = getSurroundingSurfaceGroup(i);

        // getViewFactor is responsible for handling error and issuing Error log messages.
        // Here we add it to the result array if it worked, and if it didn't, we keep going
        // We just issue a message about index so user can delete it easily
        if (group_) {
          result.push_back(group_.get());
        } else {
          LOG(Error, briefDescription() << " has an invalid SurroundingSurfaceGroup group at index " << i);
        }
      }

      return result;
    }

    // Helper
    boost::optional<SurfacePropertyLocalEnvironment> SurfacePropertySurroundingSurfaces_Impl::surfacePropertyLocalEnvironment() const {
      std::vector<SurfacePropertyLocalEnvironment> localEnvs =
        getObject<ModelObject>().getModelObjectSources<SurfacePropertyLocalEnvironment>(SurfacePropertyLocalEnvironment::iddObjectType());
      if (localEnvs.empty()) {
        return boost::none;
      } else if (localEnvs.size() == 1) {
        return localEnvs.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyLocalEnvironments point to this SurfacePropertySurroundingSurfaces");
        return boost::none;
      }
    }

  }  // namespace detail

  SurfacePropertySurroundingSurfaces::SurfacePropertySurroundingSurfaces(const Model& model)
    : ModelObject(SurfacePropertySurroundingSurfaces::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>());

    autocalculateSkyViewFactor();
    autocalculateGroundViewFactor();
    // TODO: Appropriately handle the following required object-list fields.
  }

  IddObjectType SurfacePropertySurroundingSurfaces::iddObjectType() {
    return {IddObjectType::OS_SurfaceProperty_SurroundingSurfaces};
  }

  boost::optional<double> SurfacePropertySurroundingSurfaces::skyViewFactor() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->skyViewFactor();
  }

  bool SurfacePropertySurroundingSurfaces::isSkyViewFactorAutocalculated() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->isSkyViewFactorAutocalculated();
  }

  boost::optional<Schedule> SurfacePropertySurroundingSurfaces::skyTemperatureSchedule() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->skyTemperatureSchedule();
  }

  boost::optional<double> SurfacePropertySurroundingSurfaces::groundViewFactor() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->groundViewFactor();
  }

  bool SurfacePropertySurroundingSurfaces::isGroundViewFactorAutocalculated() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->isGroundViewFactorAutocalculated();
  }

  boost::optional<Schedule> SurfacePropertySurroundingSurfaces::groundTemperatureSchedule() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->groundTemperatureSchedule();
  }

  bool SurfacePropertySurroundingSurfaces::setSkyViewFactor(double skyViewFactor) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->setSkyViewFactor(skyViewFactor);
  }

  void SurfacePropertySurroundingSurfaces::autocalculateSkyViewFactor() {
    getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->autocalculateSkyViewFactor();
  }

  bool SurfacePropertySurroundingSurfaces::setSkyTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->setSkyTemperatureSchedule(schedule);
  }

  void SurfacePropertySurroundingSurfaces::resetSkyTemperatureSchedule() {
    getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->resetSkyTemperatureSchedule();
  }

  bool SurfacePropertySurroundingSurfaces::setGroundViewFactor(double groundViewFactor) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->setGroundViewFactor(groundViewFactor);
  }

  void SurfacePropertySurroundingSurfaces::autocalculateGroundViewFactor() {
    getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->autocalculateGroundViewFactor();
  }

  bool SurfacePropertySurroundingSurfaces::setGroundTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->setGroundTemperatureSchedule(schedule);
  }

  void SurfacePropertySurroundingSurfaces::resetGroundTemperatureSchedule() {
    getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->resetGroundTemperatureSchedule();
  }

  boost::optional<SurfacePropertyLocalEnvironment> SurfacePropertySurroundingSurfaces::surfacePropertyLocalEnvironment() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->surfacePropertyLocalEnvironment();
  }

  // Extensible fields

  std::vector<SurroundingSurfaceGroup> SurfacePropertySurroundingSurfaces::surroundingSurfaceGroups() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->surroundingSurfaceGroups();
  }

  unsigned int SurfacePropertySurroundingSurfaces::numberofSurroundingSurfaceGroups() const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->numberofSurroundingSurfaceGroups();
  }

  boost::optional<unsigned>
    SurfacePropertySurroundingSurfaces::surroundingSurfaceGroupIndex(const SurroundingSurfaceGroup& surroundingSurfaceGroup) const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->surroundingSurfaceGroupIndex(surroundingSurfaceGroup);
  }
  boost::optional<unsigned> SurfacePropertySurroundingSurfaces::surroundingSurfaceGroupIndex(const std::string& surroundingSurfaceName) const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->surroundingSurfaceGroupIndex(surroundingSurfaceName);
  }

  boost::optional<SurroundingSurfaceGroup> SurfacePropertySurroundingSurfaces::getSurroundingSurfaceGroup(unsigned groupIndex) const {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->getSurroundingSurfaceGroup(groupIndex);
  }

  bool SurfacePropertySurroundingSurfaces::addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->addSurroundingSurfaceGroup(surroundingSurfaceGroup);
  }

  // Overloads, it creates a SurroundingSurfaceGroup wrapper, then call `addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup)`
  bool SurfacePropertySurroundingSurfaces::addSurroundingSurfaceGroup(const std::string& surroundingSurfaceName, double viewFactor,
                                                                      const Schedule& temperatureSchedule) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->addSurroundingSurfaceGroup(surroundingSurfaceName, viewFactor,
                                                                                                  temperatureSchedule);
  }

  bool SurfacePropertySurroundingSurfaces::addSurroundingSurfaceGroups(const std::vector<SurroundingSurfaceGroup>& surroundingSurfaceGroups) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->addSurroundingSurfaceGroups(surroundingSurfaceGroups);
  }

  bool SurfacePropertySurroundingSurfaces::removeSurroundingSurfaceGroup(unsigned groupIndex) {
    return getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->removeSurroundingSurfaceGroup(groupIndex);
  }

  void SurfacePropertySurroundingSurfaces::removeAllSurroundingSurfaceGroups() {
    getImpl<detail::SurfacePropertySurroundingSurfaces_Impl>()->removeAllSurroundingSurfaceGroups();
  }

  /// @cond
  SurfacePropertySurroundingSurfaces::SurfacePropertySurroundingSurfaces(std::shared_ptr<detail::SurfacePropertySurroundingSurfaces_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
