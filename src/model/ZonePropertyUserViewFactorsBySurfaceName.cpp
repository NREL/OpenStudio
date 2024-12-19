/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <vector>
#include <string>
#include "ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include <algorithm>

namespace openstudio {
namespace model {

  /*****************************************************************************************************************************************************
*                                                      V I E W    F A C T O R    W R A P P E R                                                      *
*****************************************************************************************************************************************************/
  ViewFactor::ViewFactor(const ModelObject& fromSurface, const ModelObject& toSurface, double viewFactor)
    : m_from_surface(fromSurface), m_to_surface(toSurface), m_view_factor(viewFactor) {
    // Matches the maximum value in IDD
    if (m_view_factor > 1) {
      LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
    }

    // Note JM 2019-09-13: E+ will issue a Warning if you don't have numSurfaces^2 viewFactors in HeatBalanceIntRadExchange::GetInputViewFactorsbyName
    // and set them to zero (which is probably what you want). In order to avoid this warning, I'll allow toSurface == fromSurface
    if (fromSurface.handle() == toSurface.handle()) {
      // LOG_AND_THROW("Cannot create a viewFactor when fromSurface and toSurface are the same: " << fromSurface.briefDescription());

      // Issue a warning if viewFactor isn't zero though, it's unusual for a surface to view itself
      if (viewFactor != 0.0) {
        LOG(Warn, "You are creating a viewFactor with a value of "
                    << viewFactor << " while fromSurface and toSurface are the same: " << fromSurface.briefDescription());
      }
    }

    // Check the IDD types to ensure they are ok
    IddObjectType fromIddType = fromSurface.iddObjectType();
    if ((fromIddType != IddObjectType::OS_Surface) && (fromIddType != IddObjectType::OS_SubSurface)
        && (fromIddType != IddObjectType::OS_InternalMass)) {
      LOG_AND_THROW("fromSurface can be only of type Surface, SubSurface or InternalMass, not " << fromIddType.value());
    }

    IddObjectType toIddType = toSurface.iddObjectType();
    if ((toIddType != IddObjectType::OS_Surface) && (toIddType != IddObjectType::OS_SubSurface) && (toIddType != IddObjectType::OS_InternalMass)) {
      LOG_AND_THROW("toSurface can be only of type Surface, SubSurface or InternalMass, not " << toIddType.value());
    }
  }

  ModelObject ViewFactor::fromSurface() const {
    return m_from_surface;
  }

  ModelObject ViewFactor::toSurface() const {
    return m_to_surface;
  }

  double ViewFactor::viewFactor() const {
    return m_view_factor;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactor& viewFactor) {
    out << "(from " << viewFactor.fromSurface().iddObject().name() << "='" << viewFactor.fromSurface().nameString() << "', " << "to "
        << viewFactor.toSurface().iddObject().name() << "='" << viewFactor.toSurface().nameString() << "', "
        << "view factor=" << viewFactor.viewFactor() << ")";
    return out;
  }

  /*****************************************************************************************************************************************************
*                                  Z O N E P R O P E R T Y U S E R V I E W F A C T O R S B Y S U R F A C E N A M E                                  *
*****************************************************************************************************************************************************/

  namespace detail {

    ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
    }

    ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
    }

    ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(
      const ZonePropertyUserViewFactorsBySurfaceName_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZonePropertyUserViewFactorsBySurfaceName_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ZonePropertyUserViewFactorsBySurfaceName_Impl::iddObjectType() const {
      return ZonePropertyUserViewFactorsBySurfaceName::iddObjectType();
    }

    ModelObject ZonePropertyUserViewFactorsBySurfaceName_Impl::clone(Model /*model*/) const {
      LOG_AND_THROW("Cloning isn't allowed for ZonePropertyUserViewFactorsBySurfaceName in order to guarantee that every "
                    "ZonePropertyUserViewFactorsBySurfaceName has a thermal zone, and"
                    "that a thermal zone must have only one ZonePropertyUserViewFactorsBySurfaceName.");
    }

    ThermalZone ZonePropertyUserViewFactorsBySurfaceName_Impl::thermalZone() const {
      boost::optional<ThermalZone> thermalZone =
        getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName);
      OS_ASSERT(thermalZone);
      return thermalZone.get();
    }

    unsigned int ZonePropertyUserViewFactorsBySurfaceName_Impl::numberofViewFactors() const {
      return numExtensibleGroups();
    }

    boost::optional<unsigned> ZonePropertyUserViewFactorsBySurfaceName_Impl::viewFactorIndex(const ViewFactor& t_viewFactor) const {
      boost::optional<unsigned> result;

      // Find with custom predicate, checking handle equality between the toSurface and the fromSurface pairs
      // We do it with extensibleGroups() (rather than viewFactors()) and getString to avoid overhead
      // of manipulating actual model objects (getTarget, then create a ViewFactor wrapper, get handle convert to string...) and speed up the routine
      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto from = toString(t_viewFactor.fromSurface().handle());
      auto to = toString(t_viewFactor.toSurface().handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return ((eg.getField(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName).get() == from)
                && (eg.getField(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName).get() == to));
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    boost::optional<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName_Impl::getViewFactor(unsigned groupIndex) const {

      boost::optional<ViewFactor> result;

      if (groupIndex >= numberofViewFactors()) {
        LOG(Error, "Asked to get ViewFactor with index " << groupIndex << ", but " << briefDescription() << " has just " << numberofViewFactors()
                                                         << " ViewFactors.");
        return result;
      }

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

      boost::optional<ModelObject> _toSurface =
        group.getModelObjectTarget<ModelObject>(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName);
      boost::optional<ModelObject> _fromSurface =
        group.getModelObjectTarget<ModelObject>(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName);
      boost::optional<double> value = group.getDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor);

      if (!_toSurface) {
        LOG(Error, "Could not retrieve FromSurfaceName for extensible group " << group.groupIndex() << ".");
      }
      if (!_fromSurface) {
        LOG(Error, "Could not retrieve ToSurfaceName for extensible group " << group.groupIndex() << ".");
      }
      if (!value) {
        LOG(Error, "Could not retrieve ViewFactor for extensible group " << group.groupIndex() << ".");
      }

      result = ViewFactor(_toSurface.get(), _fromSurface.get(), value.get());

      return result;
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const ViewFactor& viewFactor) {
      bool result = false;

      // This the only place we need to cast, in order to check if the actual to/from stuff is part of our zone
      // This lambda checks that the InternalMass/Surface/SubSurface is part of the ThermalZone referenced by this object
      auto checkIfInThermalZone = [this](const ModelObject& modelObject) {
        bool result = false;
        boost::optional<Space> _space;

        if (boost::optional<Surface> _fromSurfaceAsSurface = modelObject.optionalCast<Surface>()) {
          _space = _fromSurfaceAsSurface->space();
        } else if (boost::optional<SubSurface> _fromSurfaceAsSubSurface = modelObject.optionalCast<SubSurface>()) {
          _space = _fromSurfaceAsSubSurface->space();
        } else if (boost::optional<InternalMass> _fromSurfaceAsInternalMass = modelObject.optionalCast<InternalMass>()) {
          _space = _fromSurfaceAsInternalMass->space();
        } else {
          // This can't happen, ViewFactor shouldn't accept anything else
          OS_ASSERT(false);
        }

        if (_space) {
          if (boost::optional<ThermalZone> _zone = _space->thermalZone()) {
            if (_zone->handle() == this->thermalZone().handle()) {
              result = true;
            }
          }
        }
        return result;
      };

      ModelObject fromSurface = viewFactor.fromSurface();
      if (!checkIfInThermalZone(fromSurface)) {
        LOG(Error, "Cannot add ViewFactor to " << briefDescription() << " because fromSurface=" << fromSurface.briefDescription()
                                               << "'is not part of the ThermalZone.");
        return false;
      }

      ModelObject toSurface = viewFactor.toSurface();
      if (!checkIfInThermalZone(toSurface)) {
        LOG(Error, "Cannot add ViewFactor to " << briefDescription() << " because toSurface=" << toSurface.briefDescription()
                                               << "'is not part of the ThermalZone.");
        return false;
      }

      // Check if viewFactor already exists
      boost::optional<unsigned> _existingIndex = viewFactorIndex(viewFactor);
      if (_existingIndex) {
        boost::optional<ViewFactor> _viewFactor = getViewFactor(_existingIndex.get());
        OS_ASSERT(_viewFactor);
        LOG(Warn, "For " << briefDescription() << ", ViewFactor already exists, will be modified in place from " << _viewFactor.get() << " to "
                         << viewFactor << ".");
      }

      // If existing, get it, otherwise Push an extensible group. ModelExtensibleGroup cannot be default-constructed, so use a ternary operator
      // Given that we will potentially push many extensible groups
      // (number of viewFactors to add in a Zone = x^2 where x is number of surfaces + subsurfaces + internalMass)
      // we will suffer from going through the internal checks in WorkspaceObject, especially in setPointer,
      // and performance will be roughly O(n^2) when adding viewFactors.
      // So to improve the performance we pushExtensible group without validity check, and call setPointer without validity check (`isValid`)
      std::vector<std::string> temp;
      ModelExtensibleGroup eg = (_existingIndex ? getExtensibleGroup(_existingIndex.get()).cast<ModelExtensibleGroup>()
                                                : pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>());

      bool from = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName, fromSurface.handle(), false);
      if (!from) {
        LOG(Error, "Unable to add View Factor which has an incompatible fromSurface object to " << briefDescription());
        OS_ASSERT(false);
      }

      bool to = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName, toSurface.handle(), false);
      if (!to) {
        LOG(Error, "Unable to add View Factor which has an incompatible toSurface object to " << briefDescription());
        OS_ASSERT(false);
      }

      bool value = eg.setDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor, viewFactor.viewFactor());
      if (from && to && value) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const Surface& toSurface, double value) {
      ViewFactor viewFactor(fromSurface, toSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double value) {
      ViewFactor viewFactor(fromSurface, toSubSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double value) {
      ViewFactor viewFactor(fromSurface, toInternalMass, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface,
                                                                      double value) {
      ViewFactor viewFactor(fromSubSurface, toSubSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double value) {
      ViewFactor viewFactor(fromSubSurface, toSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass,
                                                                      double value) {
      ViewFactor viewFactor(fromSubSurface, toInternalMass, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toSubSurface,
                                                                      double value) {
      ViewFactor viewFactor(fromInternalMass, toSubSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double value) {
      ViewFactor viewFactor(fromInternalMass, toSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface,
                                                                      double value) {
      ViewFactor viewFactor(fromInternalMass, toSubSurface, value);
      return addViewFactor(viewFactor);
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::removeViewFactor(unsigned groupIndex) {
      bool result = false;

      unsigned int num = numberofViewFactors();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      }
      return result;
    }

    void ZonePropertyUserViewFactorsBySurfaceName_Impl::removeAllViewFactors() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    std::vector<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName_Impl::viewFactors() const {
      std::vector<ViewFactor> result;

      for (unsigned i = 0; i < numberofViewFactors(); ++i) {

        boost::optional<ViewFactor> _viewFactor = getViewFactor(i);

        // getViewFactor is responsible for handling error and issuing Error log messages.
        // Here we add it to the result array if it worked, and if it didn't, we keep going
        // We just issue a message about index so user can delete it easily
        if (_viewFactor) {
          result.push_back(_viewFactor.get());
        } else {
          LOG(Error, briefDescription() << " has an invalid ViewFactor group at index " << i);
        }
      }

      return result;
    }

    bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactors(const std::vector<ViewFactor>& viewFactors) {
      bool result = true;

      for (const ViewFactor& viewFactor : viewFactors) {
        bool thisResult = addViewFactor(viewFactor);
        if (!thisResult) {
          LOG(Error, "Could not add viewFactor " << viewFactor << " to " << briefDescription() << ". Continuing with others.");
          // OS_ASSERT(false);
          // result = false;
        }
      }

      return result;
    }

  }  // namespace detail

  ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(const ThermalZone& thermalZone)
    : ModelObject(ZonePropertyUserViewFactorsBySurfaceName::iddObjectType(), thermalZone.model()) {
    std::vector<ZonePropertyUserViewFactorsBySurfaceName> zoneProps =
      thermalZone.getModelObjectSources<ZonePropertyUserViewFactorsBySurfaceName>(ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
    if (!zoneProps.empty()) {
      remove();
      LOG_AND_THROW(thermalZone.briefDescription() << " already has a ZonePropertyUserViewFactorsBySurfaceName, cannot create a new one. Use "
                                                      "ThermalZone::getZonePropertyUserViewFactorsBySurfaceName() instead.");
    }
    OS_ASSERT(getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>());

    bool ok = setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName, thermalZone.handle());
    OS_ASSERT(ok);
  }

  IddObjectType ZonePropertyUserViewFactorsBySurfaceName::iddObjectType() {
    return {IddObjectType::OS_ZoneProperty_UserViewFactors_BySurfaceName};
  }

  ThermalZone ZonePropertyUserViewFactorsBySurfaceName::thermalZone() const {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->thermalZone();
  }

  unsigned int ZonePropertyUserViewFactorsBySurfaceName::numberofViewFactors() const {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->numberofViewFactors();
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const ViewFactor& viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toSurface, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toSubSurface, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toInternalMass, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toSubSurface, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toSurface, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass,
                                                               double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toInternalMass, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInternalMass,
                                                               double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toInternalMass, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toSurface, viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface,
                                                               double viewFactor) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toSubSurface, viewFactor);
  }

  boost::optional<unsigned> ZonePropertyUserViewFactorsBySurfaceName::viewFactorIndex(const ViewFactor& viewFactor) const {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->viewFactorIndex(viewFactor);
  }

  boost::optional<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName::getViewFactor(unsigned groupIndex) const {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->getViewFactor(groupIndex);
  }

  void ZonePropertyUserViewFactorsBySurfaceName::removeViewFactor(int groupIndex) {
    getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeViewFactor(groupIndex);
  }

  void ZonePropertyUserViewFactorsBySurfaceName::removeAllViewFactors() {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeAllViewFactors();
  }

  std::vector<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName::viewFactors() const {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->viewFactors();
  }

  bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactors(const std::vector<ViewFactor>& viewFactors) {
    return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactors(viewFactors);
  }

  /// @cond
  ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(
    std::shared_ptr<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
