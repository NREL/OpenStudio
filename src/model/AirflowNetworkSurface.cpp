/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkSurface.hpp"
#include "AirflowNetworkSurface_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "AirflowNetworkComponent.hpp"
#include "AirflowNetworkComponent_Impl.hpp"
#include "AirflowNetworkExternalNode.hpp"
#include "AirflowNetworkExternalNode_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirflowNetworkOccupantVentilationControl.hpp"
#include "AirflowNetworkOccupantVentilationControl_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkSurface_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkLinkage_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSurface::iddObjectType());
    }

    AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkLinkage_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkSurface::iddObjectType());
    }

    AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const AirflowNetworkSurface_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkLinkage_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkSurface_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkSurface_Impl::iddObjectType() const {
      return AirflowNetworkSurface::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirflowNetworkSurface_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName) != e) {
        result.emplace_back("AirflowNetworkSurface", "Ventilation Control Zone Temperature Setpoint");
      }
      if (std::find(b, e, OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName) != e) {
        result.emplace_back("AirflowNetworkSurface", "Venting Availability");
      }
      return result;
    }

    PlanarSurface AirflowNetworkSurface_Impl::surface() const {
      boost::optional<PlanarSurface> value = optionalSurface();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
      }
      return value.get();
    }

    boost::optional<AirflowNetworkComponent> AirflowNetworkSurface_Impl::leakageComponent() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkComponent>(OS_AirflowNetworkSurfaceFields::LeakageComponentName);
    }

    boost::optional<AirflowNetworkExternalNode> AirflowNetworkSurface_Impl::externalNode() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkExternalNode>(OS_AirflowNetworkSurfaceFields::ExternalNodeName);
    }

    double AirflowNetworkSurface_Impl::windowDoorOpeningFactorOrCrackFactor() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isWindowDoorOpeningFactorOrCrackFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor);
    }

    std::string AirflowNetworkSurface_Impl::ventilationControlMode() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkSurfaceFields::VentilationControlMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isVentilationControlModeDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::VentilationControlMode);
    }

    boost::optional<Schedule> AirflowNetworkSurface_Impl::ventilationControlZoneTemperatureSetpointSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName);
    }

    double AirflowNetworkSurface_Impl::minimumVentingOpenFactor() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor);
    }

    double AirflowNetworkSurface_Impl::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
    }

    double AirflowNetworkSurface_Impl::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
    }

    double AirflowNetworkSurface_Impl::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
    }

    double AirflowNetworkSurface_Impl::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSurface_Impl::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
    }

    boost::optional<Schedule> AirflowNetworkSurface_Impl::ventingAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName);
    }

    boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkSurface_Impl::occupantVentilationControl() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkOccupantVentilationControl>(
        OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName);
    }

    bool AirflowNetworkSurface_Impl::setSurface(const PlanarSurface& surfAndSubSurf) {
      bool result = setPointer(OS_AirflowNetworkSurfaceFields::SurfaceName, surfAndSubSurf.handle());
      return result;
    }

    void AirflowNetworkSurface_Impl::resetSurface() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::SurfaceName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setLeakageComponent(const AirflowNetworkComponent& surfaceAirflowLeakage) {
      return setPointer(OS_AirflowNetworkSurfaceFields::LeakageComponentName, surfaceAirflowLeakage.handle());
    }

    bool AirflowNetworkSurface_Impl::setExternalNode(const AirflowNetworkExternalNode& externalNode) {
      return setPointer(OS_AirflowNetworkSurfaceFields::ExternalNodeName, externalNode.handle());
    }

    void AirflowNetworkSurface_Impl::resetExternalNode() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::ExternalNodeName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor, windowDoorOpeningFactorOrCrackFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetWindowDoorOpeningFactorOrCrackFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setVentilationControlMode(const std::string& ventilationControlMode) {
      bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlMode, ventilationControlMode);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetVentilationControlMode() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlMode, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName, "AirflowNetworkSurface",
                                "Ventilation Control Zone Temperature Setpoint", schedule);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetVentilationControlZoneTemperatureSetpointSchedule() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor, minimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor,
                              indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor,
                              indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor,
                              indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor,
                              indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setVentingAvailabilitySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName, "AirflowNetworkSurface", "Venting Availability", schedule);
      return result;
    }

    void AirflowNetworkSurface_Impl::resetVentingAvailabilitySchedule() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSurface_Impl::setOccupantVentilationControl(
      const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl) {
      return setPointer(OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName, airflowNetworkOccupantVentilationControl.handle());
    }

    void AirflowNetworkSurface_Impl::resetOccupantVentilationControl() {
      bool result = setString(OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName, "");
      OS_ASSERT(result);
    }

    boost::optional<PlanarSurface> AirflowNetworkSurface_Impl::optionalSurface() const {
      return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_AirflowNetworkSurfaceFields::SurfaceName);
    }

    bool AirflowNetworkSurface_Impl::setParent(ParentObject& surfAndSubSurf) {
      bool result = setPointer(OS_AirflowNetworkSurfaceFields::SurfaceName, surfAndSubSurf.handle());
      return result;
    }

  }  // namespace detail

  AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const Handle& compHandle, const Handle& surfHandle)
    : AirflowNetworkLinkage(AirflowNetworkSurface::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());
    bool ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setPointer(OS_AirflowNetworkSurfaceFields::LeakageComponentName, compHandle);
    OS_ASSERT(ok);
    ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setPointer(OS_AirflowNetworkSurfaceFields::SurfaceName, surfHandle);
    OS_ASSERT(ok);
  }

  /*
AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const Surface &surface)
  : AirflowNetworkLinkage(AirflowNetworkSurface::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());
  bool ok;
  ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surface);
  OS_ASSERT(ok);
}

AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const SubSurface &surface)
  : AirflowNetworkLinkage(AirflowNetworkSurface::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());
  bool ok;
  ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surface);
  OS_ASSERT(ok);
}

AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const Surface &surface,
  const AirflowNetworkComponent& component)
  : AirflowNetworkLinkage(AirflowNetworkSurface::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());
  bool ok;
  ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surface);
  OS_ASSERT(ok);
  ok = setLeakageComponent(component);
  OS_ASSERT(ok);
}

AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const SubSurface &surface,
  const AirflowNetworkComponent& component)
  : AirflowNetworkLinkage(AirflowNetworkSurface::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());
  bool ok;
  ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surface);
  OS_ASSERT(ok);
  ok = setLeakageComponent(component);
  OS_ASSERT(ok);
}
*/

  IddObjectType AirflowNetworkSurface::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkSurface};
  }

  std::vector<std::string> AirflowNetworkSurface::ventilationControlModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirflowNetworkSurfaceFields::VentilationControlMode);
  }

  PlanarSurface AirflowNetworkSurface::surface() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->surface();
  }

  boost::optional<AirflowNetworkComponent> AirflowNetworkSurface::leakageComponent() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->leakageComponent();
  }

  boost::optional<AirflowNetworkExternalNode> AirflowNetworkSurface::externalNode() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->externalNode();
  }

  double AirflowNetworkSurface::windowDoorOpeningFactorOrCrackFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->windowDoorOpeningFactorOrCrackFactor();
  }

  bool AirflowNetworkSurface::isWindowDoorOpeningFactorOrCrackFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isWindowDoorOpeningFactorOrCrackFactorDefaulted();
  }

  std::string AirflowNetworkSurface::ventilationControlMode() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->ventilationControlMode();
  }

  bool AirflowNetworkSurface::isVentilationControlModeDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isVentilationControlModeDefaulted();
  }

  boost::optional<Schedule> AirflowNetworkSurface::ventilationControlZoneTemperatureSetpointSchedule() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->ventilationControlZoneTemperatureSetpointSchedule();
  }

  double AirflowNetworkSurface::minimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->minimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::isMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isMinimumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkSurface::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkSurface::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkSurface::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkSurface::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
  }

  boost::optional<Schedule> AirflowNetworkSurface::ventingAvailabilitySchedule() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->ventingAvailabilitySchedule();
  }

  boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkSurface::occupantVentilationControl() const {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->occupantVentilationControl();
  }

  bool AirflowNetworkSurface::setSurface(const Surface& surfAndSubSurf) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surfAndSubSurf);
  }

  bool AirflowNetworkSurface::setSurface(const SubSurface& surfAndSubSurf) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surfAndSubSurf);
  }

  void AirflowNetworkSurface::resetSurface() {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->resetSurface();
  }

  bool AirflowNetworkSurface::setLeakageComponent(const AirflowNetworkComponent& surfaceAirflowLeakage) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setLeakageComponent(surfaceAirflowLeakage);
  }

  bool AirflowNetworkSurface::setExternalNode(const AirflowNetworkExternalNode& externalNode) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setExternalNode(externalNode);
  }

  void AirflowNetworkSurface::resetExternalNode() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetExternalNode();
  }

  bool AirflowNetworkSurface::setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setWindowDoorOpeningFactorOrCrackFactor(windowDoorOpeningFactorOrCrackFactor);
  }

  void AirflowNetworkSurface::resetWindowDoorOpeningFactorOrCrackFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetWindowDoorOpeningFactorOrCrackFactor();
  }

  bool AirflowNetworkSurface::setVentilationControlMode(const std::string& ventilationControlMode) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentilationControlMode(ventilationControlMode);
  }

  void AirflowNetworkSurface::resetVentilationControlMode() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentilationControlMode();
  }

  bool AirflowNetworkSurface::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentilationControlZoneTemperatureSetpointSchedule(schedule);
  }

  void AirflowNetworkSurface::resetVentilationControlZoneTemperatureSetpointSchedule() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentilationControlZoneTemperatureSetpointSchedule();
  }

  bool AirflowNetworkSurface::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setMinimumVentingOpenFactor(minimumVentingOpenFactor);
  }

  void AirflowNetworkSurface::resetMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetMinimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
    double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
  }

  void AirflowNetworkSurface::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
    double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
  }

  void AirflowNetworkSurface::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
    double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
  }

  void AirflowNetworkSurface::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
    double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
  }

  void AirflowNetworkSurface::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkSurface::setVentingAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentingAvailabilitySchedule(schedule);
  }

  void AirflowNetworkSurface::resetVentingAvailabilitySchedule() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentingAvailabilitySchedule();
  }

  bool
    AirflowNetworkSurface::setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl) {
    return getImpl<detail::AirflowNetworkSurface_Impl>()->setOccupantVentilationControl(airflowNetworkOccupantVentilationControl);
  }

  void AirflowNetworkSurface::resetOccupantVentilationControl() {
    getImpl<detail::AirflowNetworkSurface_Impl>()->resetOccupantVentilationControl();
  }

  /// @cond
  AirflowNetworkSurface::AirflowNetworkSurface(std::shared_ptr<detail::AirflowNetworkSurface_Impl> impl) : AirflowNetworkLinkage(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
