/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalDualDuctVAVOutdoorAir.hpp"
#include "AirTerminalDualDuctVAVOutdoorAir_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirTerminal_DualDuct_VAV_OutdoorAir_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalDualDuctVAVOutdoorAir::iddObjectType());
    }

    AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalDualDuctVAVOutdoorAir::iddObjectType());
    }

    AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const AirTerminalDualDuctVAVOutdoorAir_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirTerminalDualDuctVAVOutdoorAir_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// These apply to all AirTerminals
                                                   "Zone Air Terminal Sensible Heating Energy",
                                                   "Zone Air Terminal Sensible Heating Rate",
                                                   "Zone Air Terminal Sensible Cooling Energy",
                                                   "Zone Air Terminal Sensible Cooling Rate",

                                                   "Zone Air Terminal Outdoor Air Duct Damper Position",
                                                   "Zone Air Terminal Recirculated Air Duct Damper Position",
                                                   "Zone Air Terminal Outdoor Air Fraction"};
      return result;
    }

    IddObjectType AirTerminalDualDuctVAVOutdoorAir_Impl::iddObjectType() const {
      return AirTerminalDualDuctVAVOutdoorAir::iddObjectType();
    }

    // Availability Schedule
    std::vector<ScheduleTypeKey> AirTerminalDualDuctVAVOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalDualDuctVAVOutdoorAir", "Availability Schedule"));
      }
      return result;
    }

    Schedule AirTerminalDualDuctVAVOutdoorAir_Impl::availabilitySchedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule);
      if (!result) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return result.get();
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule, "AirTerminalDualDuctVAVOutdoorAir",
                                "Availability Schedule", schedule);
      return result;
    }

    unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::outletPort() const {
      return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode;
    }

    unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::inletPort(unsigned branchIndex) const {
      if (branchIndex == 0) {
        return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode;
      } else if (branchIndex == 1) {
        return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode;
      } else {
        LOG(Warn, "Calling inletPort with branchIndex greater than 1 for " << briefDescription() << " is not valid.");
        LOG(Warn, briefDescription() << " has only two branches.");
        return std::numeric_limits<unsigned>::max();
      }
    }

    unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::nextInletPort() const {
      LOG(Warn, "nextInletPort is not supported for " << briefDescription() << " .");
      LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
      return std::numeric_limits<unsigned>::max();
    }

    unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::newInletPortAfterBranch(unsigned /*branchIndex*/) {
      LOG(Warn, "newInletPortAfterBranch is not supported for " << briefDescription() << " .");
      LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
      return std::numeric_limits<unsigned>::max();
    }

    void AirTerminalDualDuctVAVOutdoorAir_Impl::removePortForBranch(unsigned /*branchIndex*/) {
      LOG(Warn, "removePortForBranch is not supported for " << briefDescription() << " .");
      LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
    }

    /*
  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetAirOutletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setOutdoorAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetOutdoorAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setRecirculatedAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetRecirculatedAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode, "");
    OS_ASSERT(result);
  }
    */

    // Maximum Zone Total Airflow Rate
    boost::optional<double> AirTerminalDualDuctVAVOutdoorAir_Impl::maximumTerminalAirFlowRate() const {
      return getDouble(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, true);
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::isMaximumTerminalAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate) {
      bool result = setDouble(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, maximumTerminalAirFlowRate);
      return result;
    }

    void AirTerminalDualDuctVAVOutdoorAir_Impl::autosizeMaximumTerminalAirFlowRate() {
      bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> AirTerminalDualDuctVAVOutdoorAir_Impl::autosizedMaximumTerminalAirFlowRate() const {
      return getAutosizedValue("Maximum Air Flow Rate", "m3/s");
    }

    // Design Specification Outdoor Air Object Name. Here we implement a bool, which will pull the Space/SpaceType DSOA
    bool AirTerminalDualDuctVAVOutdoorAir_Impl::controlForOutdoorAir() const {
      return getBooleanFieldValue(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::ControlForOutdoorAir);
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::setControlForOutdoorAir(bool controlForOutdoorAir) {
      // TODO: I have changed setBooleanFieldValue to return bool in unmerged PR #2875
      setBooleanFieldValue(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::ControlForOutdoorAir, controlForOutdoorAir);
      // TODO: Temporary hack
      return true;
    }

    // Per Person Ventilation Rate Mode
    std::string AirTerminalDualDuctVAVOutdoorAir_Impl::perPersonVentilationRateMode() const {
      boost::optional<std::string> value = getString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode) {
      bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode, perPersonVentilationRateMode);
      return result;
    }

    boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir_Impl::outdoorAirInletNode() const {
      boost::optional<Node> node;
      if (auto mo = inletModelObject(0)) {
        node = mo->optionalCast<Node>();
        OS_ASSERT(node);
      }
      return node;
    }

    boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir_Impl::recirculatedAirInletNode() const {
      boost::optional<Node> node;
      if (auto mo = inletModelObject(1)) {
        node = mo->optionalCast<Node>();
        OS_ASSERT(node);
      }
      return node;
    }

    std::vector<IdfObject> AirTerminalDualDuctVAVOutdoorAir_Impl::remove() {
      auto modelObject = getObject<AirTerminalDualDuctVAVOutdoorAir>();
      AirLoopHVAC_Impl::removeDualDuctTerminalFromAirLoopHVAC(modelObject, inletPort(0), inletPort(1), outletPort());
      return Mixer_Impl::remove();
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::addToNode(Node& node) {
      auto mo = getObject<AirTerminalDualDuctVAVOutdoorAir>();
      return AirLoopHVAC_Impl::addDualDuctTerminalToNode(mo, inletPort(0), inletPort(1), outletPort(), node);
    }

    ModelObject AirTerminalDualDuctVAVOutdoorAir_Impl::clone(Model model) const {
      auto t_clone = Mixer_Impl::clone(model).cast<AirTerminalDualDuctVAVOutdoorAir>();

      t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode, "");
      t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode, "");
      t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode, "");

      return std::move(t_clone);
    }

    bool AirTerminalDualDuctVAVOutdoorAir_Impl::isRemovable() const {
      return HVACComponent_Impl::isRemovable();
    }

    void AirTerminalDualDuctVAVOutdoorAir_Impl::autosize() {
      autosizeMaximumTerminalAirFlowRate();
    }

    void AirTerminalDualDuctVAVOutdoorAir_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumTerminalAirFlowRate();
      if (val) {
        setMaximumTerminalAirFlowRate(val.get());
      }
    }

    ComponentType AirTerminalDualDuctVAVOutdoorAir_Impl::componentType() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->componentType();
      }

      return ComponentType::None;
    }

    std::vector<FuelType> AirTerminalDualDuctVAVOutdoorAir_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalDualDuctVAVOutdoorAir_Impl::heatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> AirTerminalDualDuctVAVOutdoorAir_Impl::appGHeatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  AirTerminalDualDuctVAVOutdoorAir::AirTerminalDualDuctVAVOutdoorAir(const Model& model)
    : Mixer(AirTerminalDualDuctVAVOutdoorAir::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>());

    Schedule sch = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(sch);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << sch.briefDescription() << ".");
    }

    autosizeMaximumTerminalAirFlowRate();
    setPerPersonVentilationRateMode("CurrentOccupancy");
    // This is a very OA-centric object, so enabled by default
    setControlForOutdoorAir(true);
  }

  IddObjectType AirTerminalDualDuctVAVOutdoorAir::iddObjectType() {
    return {IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir};
  }

  std::vector<std::string> AirTerminalDualDuctVAVOutdoorAir::perPersonVentilationRateModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode);
  }

  Schedule AirTerminalDualDuctVAVOutdoorAir::availabilitySchedule() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->availabilitySchedule();
  }

  bool AirTerminalDualDuctVAVOutdoorAir::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setAvailabilitySchedule(schedule);
  }

  boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir::outdoorAirInletNode() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->outdoorAirInletNode();
  }

  boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir::recirculatedAirInletNode() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->recirculatedAirInletNode();
  }

  // Maximum Terminal Air Flow Rate
  boost::optional<double> AirTerminalDualDuctVAVOutdoorAir::maximumTerminalAirFlowRate() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->maximumTerminalAirFlowRate();
  }

  bool AirTerminalDualDuctVAVOutdoorAir::setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate) {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setMaximumTerminalAirFlowRate(maximumTerminalAirFlowRate);
  }

  void AirTerminalDualDuctVAVOutdoorAir::autosizeMaximumTerminalAirFlowRate() {
    getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->autosizeMaximumTerminalAirFlowRate();
  }

  bool AirTerminalDualDuctVAVOutdoorAir::isMaximumTerminalAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->isMaximumTerminalAirFlowRateAutosized();
  }

  // Per Person Ventilation Rate Mode
  std::string AirTerminalDualDuctVAVOutdoorAir::perPersonVentilationRateMode() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->perPersonVentilationRateMode();
  }

  bool AirTerminalDualDuctVAVOutdoorAir::setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode) {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setPerPersonVentilationRateMode(perPersonVentilationRateMode);
  }

  // Instead of Design Specification Outdoor Air Object Name
  bool AirTerminalDualDuctVAVOutdoorAir::controlForOutdoorAir() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->controlForOutdoorAir();
  }

  bool AirTerminalDualDuctVAVOutdoorAir::setControlForOutdoorAir(bool controlForOutdoorAir) {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setControlForOutdoorAir(controlForOutdoorAir);
  }

  boost::optional<double> AirTerminalDualDuctVAVOutdoorAir::autosizedMaximumTerminalAirFlowRate() const {
    return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->autosizedMaximumTerminalAirFlowRate();
  }

  /*
 *void AirTerminalDualDuctVAVOutdoorAir::autosize() {
 *  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->autosize();
 *}
 *
 *void AirTerminalDualDuctVAVOutdoorAir::applySizingValues() {
 *  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->applySizingValues();
 *}
 */

  /// @cond
  AirTerminalDualDuctVAVOutdoorAir::AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl)
    : Mixer(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
