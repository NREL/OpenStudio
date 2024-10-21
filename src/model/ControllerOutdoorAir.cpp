/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "ControllerMechanicalVentilation.hpp"
#include "ControllerMechanicalVentilation_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "AirflowNetworkOutdoorAirflow.hpp"
#include "AirflowNetworkOutdoorAirflow_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;
using openstudio::istringEqual;

namespace openstudio {
namespace model {

  namespace detail {

    ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ControllerOutdoorAir::iddObjectType());
    }

    ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ControllerOutdoorAir::iddObjectType());
    }

    ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(const ControllerOutdoorAir_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    // virtual destructor

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ControllerOutdoorAir_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// TODO: Note that the key value for these outputs is the AirLoopHVAC name,
                                                   // not the name of the Controller:OutdoorAir.
                                                   "Air System Outdoor Air Economizer Status",
                                                   "Air System Outdoor Air Heat Recovery Bypass Status",
                                                   "Air System Outdoor Air Heat Recovery Bypass Heating Coil Activity Status",
                                                   "Air System Outdoor Air Heat Recovery Bypass Minimum Outdoor Air Mixed Air Temperature",
                                                   "Air System Outdoor Air High Humidity Control Status",
                                                   "Air System Outdoor Air Flow Fraction",
                                                   "Air System Outdoor Air Minimum Flow Fraction",
                                                   "Air System Outdoor Air Mass Flow Rate",
                                                   "Air System Mixed Air Mass Flow Rate",
                                                   "Air System Outdoor Air Maximum Flow Fraction",
                                                   "Air System Outdoor Air Mechanical Ventilation Requested Mass Flow Rate"};
      return result;
    }

    IddObjectType ControllerOutdoorAir_Impl::iddObjectType() const {
      return ControllerOutdoorAir::iddObjectType();
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ControllerOutdoorAir_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ControllerOutdoorAir_Impl::children() const {
      std::vector<ModelObject> result;
      std::vector<AirflowNetworkOutdoorAirflow> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkOutdoorAirflow>(AirflowNetworkOutdoorAirflow::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    boost::optional<Curve> ControllerOutdoorAir_Impl::electronicEnthalpyLimitCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName);
    }

    bool ControllerOutdoorAir_Impl::setElectronicEnthalpyLimitCurve(const Curve& curve) {
      bool result = setPointer(OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName, curve.handle());
      return result;
    }

    void ControllerOutdoorAir_Impl::resetElectronicEnthalpyLimitCurve() {
      bool result = setString(OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName, "");
      OS_ASSERT(result);
    }

    OptionalAirLoopHVACOutdoorAirSystem ControllerOutdoorAir_Impl::airLoopHVACOutdoorAirSystem() const {
      AirLoopHVACOutdoorAirSystemVector oaSystems;
      oaSystems = this->model().getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>();
      AirLoopHVACOutdoorAirSystemVector::iterator it;
      for (it = oaSystems.begin(); it != oaSystems.end(); ++it) {
        if (it->getControllerOutdoorAir().handle() == this->handle()) {
          return OptionalAirLoopHVACOutdoorAirSystem(*it);
        }
      }
      return {};
    }

    boost::optional<double> ControllerOutdoorAir_Impl::minimumOutdoorAirFlowRate() const {
      return getDouble(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, true);
    }

    bool ControllerOutdoorAir_Impl::isMinimumOutdoorAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ControllerOutdoorAir_Impl::maximumOutdoorAirFlowRate() const {
      return getDouble(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, true);
    }

    bool ControllerOutdoorAir_Impl::isMaximumOutdoorAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool ControllerOutdoorAir_Impl::setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate) {
      bool result(false);
      if (minimumOutdoorAirFlowRate) {
        result = setDouble(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, minimumOutdoorAirFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void ControllerOutdoorAir_Impl::autosizeMinimumOutdoorAirFlowRate() {
      bool result = setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ControllerOutdoorAir_Impl::setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate) {
      bool result(false);
      if (maximumOutdoorAirFlowRate) {
        result = setDouble(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, maximumOutdoorAirFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void ControllerOutdoorAir_Impl::autosizeMaximumOutdoorAirFlowRate() {
      bool result = setString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    std::string ControllerOutdoorAir_Impl::getEconomizerControlType() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlType).get();
    }

    bool ControllerOutdoorAir_Impl::setEconomizerControlType(const std::string& value) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlType, value);
    }

    std::string ControllerOutdoorAir_Impl::getEconomizerControlActionType() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlActionType).get();
    }

    bool ControllerOutdoorAir_Impl::setEconomizerControlActionType(const std::string& value) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlActionType, value);
    }

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitDryBulbTemperature() const {
      return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature);
    }

    //set needs to return a boolean (true if successful, false if not successful)
    bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitDryBulbTemperature(boost::optional<double> value) {
      bool result = false;
      if (value) {
        result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, value.get());
      } else {
        result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, "");
      }
      return result;
    }

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitEnthalpy() const {
      return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy);
    }

    bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitEnthalpy(boost::optional<double> value) {
      bool result = false;
      if (value) {
        result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, value.get());
      } else {
        result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, "");
      }
      return result;
    }

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitDewpointTemperature() const {
      return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature);
    }

    bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitDewpointTemperature(boost::optional<double> value) {
      bool result = false;
      if (value) {
        result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, value.get());
      } else {
        result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, "");
      }
      return result;
    }

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMinimumLimitDryBulbTemperature() const {
      return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature);
    }

    bool ControllerOutdoorAir_Impl::setEconomizerMinimumLimitDryBulbTemperature(boost::optional<double> value) {
      bool result = false;
      if (value) {
        result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature, value.get());
      } else {
        result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature, "");
      }
      return result;
    }

    std::string ControllerOutdoorAir_Impl::getLockoutType() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::LockoutType).get();
    }

    bool ControllerOutdoorAir_Impl::setLockoutType(const std::string& value) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::LockoutType, value);
    }

    std::string ControllerOutdoorAir_Impl::getMinimumLimitType() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::MinimumLimitType).get();
    }

    bool ControllerOutdoorAir_Impl::setMinimumLimitType(const std::string& value) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::MinimumLimitType, value);
    }

    boost::optional<bool> ControllerOutdoorAir_Impl::getHighHumidityControl() const {
      return getBooleanFieldValue(OS_Controller_OutdoorAirFields::HighHumidityControl);
    }

    boost::optional<ThermalZone> ControllerOutdoorAir_Impl::humidistatControlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Controller_OutdoorAirFields::HumidistatControlZoneName);
    }

    bool ControllerOutdoorAir_Impl::setHumidistatControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_Controller_OutdoorAirFields::HumidistatControlZoneName, thermalZone.handle());
      result = result && setString(OS_Controller_OutdoorAirFields::HighHumidityControl, "Yes");
      return result;
    }

    void ControllerOutdoorAir_Impl::resetHumidistatControlZone() {
      bool result = setString(OS_Controller_OutdoorAirFields::HumidistatControlZoneName, "");
      result = result && setString(OS_Controller_OutdoorAirFields::HighHumidityControl, "No");
      OS_ASSERT(result);
    }

    double ControllerOutdoorAir_Impl::getHighHumidityOutdoorAirFlowRatio() const {
      boost::optional<double> value = getDouble(openstudio::OS_Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ControllerOutdoorAir_Impl::setHighHumidityOutdoorAirFlowRatio(double v) {
      bool result = setDouble(openstudio::OS_Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio, v);
      return result;
    }

    bool ControllerOutdoorAir_Impl::getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const {
      return getBooleanFieldValue(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio);
    }

    bool ControllerOutdoorAir_Impl::setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v) {
      return setBooleanFieldValue(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, v);
    }

    OptionalString ControllerOutdoorAir_Impl::getHeatRecoveryBypassControlType() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::HeatRecoveryBypassControlType);
    }

    bool ControllerOutdoorAir_Impl::setHeatRecoveryBypassControlType(const std::string& v) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::HeatRecoveryBypassControlType, v);
    }

    std::string ControllerOutdoorAir_Impl::economizerOperationStaging() const {
      return getString(openstudio::OS_Controller_OutdoorAirFields::EconomizerOperationStaging).get();
    }

    bool ControllerOutdoorAir_Impl::setEconomizerOperationStaging(const std::string& v) {
      return setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerOperationStaging, v);
    }

    ControllerMechanicalVentilation ControllerOutdoorAir_Impl::controllerMechanicalVentilation() const {
      boost::optional<ControllerMechanicalVentilation> value = optionalControllerMechanicalVentilation();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Controller Mechanical Ventilation attached.");
      }
      return value.get();
    }

    bool ControllerOutdoorAir_Impl::setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation) {
      bool result = setPointer(OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation, controllerMechanicalVentilation.handle());
      return result;
    }

    boost::optional<ControllerMechanicalVentilation> ControllerOutdoorAir_Impl::optionalControllerMechanicalVentilation() const {
      return getObject<ModelObject>().getModelObjectTarget<ControllerMechanicalVentilation>(
        OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation);
    }

    boost::optional<ModelObject> ControllerOutdoorAir_Impl::controllerMechanicalVentilationAsModelObject() const {
      OptionalModelObject result = controllerMechanicalVentilation();
      return result;
    }

    bool ControllerOutdoorAir_Impl::setControllerMechanicalVentilationAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalControllerMechanicalVentilation intermediate = modelObject->optionalCast<ControllerMechanicalVentilation>();
        if (intermediate) {
          ControllerMechanicalVentilation controllerMechanicalVentilation(*intermediate);
          return setControllerMechanicalVentilation(controllerMechanicalVentilation);
        }
      }
      return false;
    }

    std::vector<openstudio::IdfObject> ControllerOutdoorAir_Impl::remove() {
      controllerMechanicalVentilation().remove();

      return ParentObject_Impl::remove();
    }

    ModelObject ControllerOutdoorAir_Impl::clone(Model model) const {
      auto oaControllerClone = ParentObject_Impl::clone(model).cast<ControllerOutdoorAir>();

      auto mechVentControllerClone = controllerMechanicalVentilation().clone(model).cast<ControllerMechanicalVentilation>();

      oaControllerClone.setControllerMechanicalVentilation(mechVentControllerClone);

      return std::move(oaControllerClone);
    }

    boost::optional<Schedule> ControllerOutdoorAir_Impl::minimumOutdoorAirSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName);
    }

    bool ControllerOutdoorAir_Impl::setMinimumOutdoorAirSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName, "ControllerOutdoorAir", "Minimum Outdoor Air", schedule);
      return result;
    }

    void ControllerOutdoorAir_Impl::resetMinimumOutdoorAirSchedule() {
      bool result = setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ControllerOutdoorAir_Impl::minimumFractionofOutdoorAirSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName);
    }

    bool ControllerOutdoorAir_Impl::setMinimumFractionofOutdoorAirSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName, "ControllerOutdoorAir",
                                "Minimum Fraction of Outdoor Air", schedule);
      return result;
    }

    void ControllerOutdoorAir_Impl::resetMinimumFractionofOutdoorAirSchedule() {
      bool result = setString(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ControllerOutdoorAir_Impl::maximumFractionofOutdoorAirSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName);
    }

    bool ControllerOutdoorAir_Impl::setMaximumFractionofOutdoorAirSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName, "ControllerOutdoorAir",
                                "Maximum Fraction of Outdoor Air", schedule);
      return result;
    }

    void ControllerOutdoorAir_Impl::resetMaximumFractionofOutdoorAirSchedule() {
      bool result = setString(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ControllerOutdoorAir_Impl::timeofDayEconomizerControlSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName);
    }

    bool ControllerOutdoorAir_Impl::setTimeofDayEconomizerControlSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName, "ControllerOutdoorAir",
                                "Time of Day Economizer Control", schedule);
      return result;
    }

    void ControllerOutdoorAir_Impl::resetTimeofDayEconomizerControlSchedule() {
      bool result = setString(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName, "");
      OS_ASSERT(result);
    }

    std::vector<ScheduleTypeKey> ControllerOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ControllerOutdoorAir", "Minimum Outdoor Air"));
      }
      if (std::find(b, e, OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ControllerOutdoorAir", "Minimum Fraction of Outdoor Air"));
      }
      if (std::find(b, e, OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ControllerOutdoorAir", "Maximum Fraction of Outdoor Air"));
      }
      if (std::find(b, e, OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ControllerOutdoorAir", "Time of Day Economizer Control"));
      }
      return result;
    }

    AirflowNetworkOutdoorAirflow ControllerOutdoorAir_Impl::getAirflowNetworkOutdoorAirflow(const AirflowNetworkCrack& crack) {
      boost::optional<AirflowNetworkOutdoorAirflow> opt = airflowNetworkOutdoorAirflow();
      if (opt) {
        boost::optional<AirflowNetworkCrack> oldCrack = opt->crack();
        if (oldCrack) {
          if (oldCrack->handle() == crack.handle()) {
            return opt.get();
          }
        }
        opt->remove();
      }
      return AirflowNetworkOutdoorAirflow(model(), crack, handle());
    }

    boost::optional<AirflowNetworkOutdoorAirflow> ControllerOutdoorAir_Impl::airflowNetworkOutdoorAirflow() const {
      std::vector<AirflowNetworkOutdoorAirflow> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkOutdoorAirflow>(AirflowNetworkOutdoorAirflow::iddObjectType());
      auto count = myAFNItems.size();
      if (count == 1) {
        return myAFNItems[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquipmentLinkage attached, returning first.");
        return myAFNItems[0];
      }
      return boost::none;
    }

    boost::optional<double> ControllerOutdoorAir_Impl::autosizedMinimumOutdoorAirFlowRate() const {
      return getAutosizedValue("Minimum Outdoor Air Flow Rate", "m3/s");
    }

    boost::optional<double> ControllerOutdoorAir_Impl::autosizedMaximumOutdoorAirFlowRate() const {
      return getAutosizedValue("Maximum Outdoor Air Flow Rate", "m3/s");
    }

    void ControllerOutdoorAir_Impl::autosize() {
      autosizeMinimumOutdoorAirFlowRate();
      autosizeMaximumOutdoorAirFlowRate();
    }

    void ControllerOutdoorAir_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMinimumOutdoorAirFlowRate();
      if (val) {
        setMinimumOutdoorAirFlowRate(val.get());
      }

      val = autosizedMaximumOutdoorAirFlowRate();
      if (val) {
        setMaximumOutdoorAirFlowRate(val.get());
      }
    }

    std::vector<EMSActuatorNames> ControllerOutdoorAir_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Outdoor Air Controller", "Air Mass Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> ControllerOutdoorAir_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Outdoor Air Controller Maximum Mass Flow Rate", "Outdoor Air Controller Minimum Mass Flow Rate"};
      return types;
    }
  }  // namespace detail

  // create a new ControllerOutdoorAir object in the model's workspace
  ControllerOutdoorAir::ControllerOutdoorAir(const Model& model) : ParentObject(ControllerOutdoorAir::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ControllerOutdoorAir_Impl>());

    setString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, "Autosize");
    setMinimumOutdoorAirFlowRate(0.0);
    setEconomizerControlType("NoEconomizer");
    setEconomizerControlActionType("ModulateFlow");
    setEconomizerMaximumLimitDryBulbTemperature(28.0);
    setEconomizerMaximumLimitEnthalpy(64000.0);
    setString(OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, "");
    setString(OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName, "");
    setEconomizerMinimumLimitDryBulbTemperature(-100.0);
    setLockoutType("NoLockout");
    setMinimumLimitType("FixedMinimum");
    setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName, "");
    setString(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName, "");
    setString(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName, "");
    setString(OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation, "");
    setString(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName, "");
    setString(OS_Controller_OutdoorAirFields::HighHumidityControl, "No");
    setHighHumidityOutdoorAirFlowRatio(1.0);
    setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(true);
    setHeatRecoveryBypassControlType("BypassWhenWithinEconomizerLimits");
    setEconomizerOperationStaging("InterlockedWithMechanicalCooling");

    ControllerMechanicalVentilation controllerMechanicalVentilation(model);
    setControllerMechanicalVentilation(controllerMechanicalVentilation);
  }

  ControllerOutdoorAir::ControllerOutdoorAir(std::shared_ptr<detail::ControllerOutdoorAir_Impl> impl) : ParentObject(std::move(impl)) {}

  boost::optional<Curve> ControllerOutdoorAir::electronicEnthalpyLimitCurve() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->electronicEnthalpyLimitCurve();
  }

  bool ControllerOutdoorAir::setElectronicEnthalpyLimitCurve(const Curve& curve) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setElectronicEnthalpyLimitCurve(curve);
  }

  void ControllerOutdoorAir::resetElectronicEnthalpyLimitCurve() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetElectronicEnthalpyLimitCurve();
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> ControllerOutdoorAir::airLoopHVACOutdoorAirSystem() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->airLoopHVACOutdoorAirSystem();
  }

  std::string ControllerOutdoorAir::getEconomizerControlType() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerControlType();
  }

  bool ControllerOutdoorAir::setEconomizerControlType(const std::string& value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerControlType(value);
  }

  std::string ControllerOutdoorAir::getEconomizerControlActionType() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerControlActionType();
  }

  bool ControllerOutdoorAir::setEconomizerControlActionType(const std::string& value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerControlActionType(value);
  }

  boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitDryBulbTemperature() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitDryBulbTemperature();
  }

  bool ControllerOutdoorAir::setEconomizerMaximumLimitDryBulbTemperature(double value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDryBulbTemperature(value);
  }

  void ControllerOutdoorAir::resetEconomizerMaximumLimitDryBulbTemperature() {
    bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDryBulbTemperature(boost::none);
    OS_ASSERT(test);
  }

  boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitEnthalpy() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitEnthalpy();
  }

  bool ControllerOutdoorAir::setEconomizerMaximumLimitEnthalpy(double value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitEnthalpy(value);
  }

  void ControllerOutdoorAir::resetEconomizerMaximumLimitEnthalpy() {
    bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitEnthalpy(boost::none);
    OS_ASSERT(test);
  }

  boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitDewpointTemperature() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitDewpointTemperature();
  }

  bool ControllerOutdoorAir::setEconomizerMaximumLimitDewpointTemperature(double value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDewpointTemperature(value);
  }

  void ControllerOutdoorAir::resetEconomizerMaximumLimitDewpointTemperature() {
    bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDewpointTemperature(boost::none);
    OS_ASSERT(test);
  }

  boost::optional<double> ControllerOutdoorAir::getEconomizerMinimumLimitDryBulbTemperature() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMinimumLimitDryBulbTemperature();
  }

  bool ControllerOutdoorAir::setEconomizerMinimumLimitDryBulbTemperature(double value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMinimumLimitDryBulbTemperature(value);
  }

  void ControllerOutdoorAir::resetEconomizerMinimumLimitDryBulbTemperature() {
    bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMinimumLimitDryBulbTemperature(boost::none);
    OS_ASSERT(test);
  }

  std::string ControllerOutdoorAir::getLockoutType() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getLockoutType();
  }

  bool ControllerOutdoorAir::setLockoutType(const std::string& value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setLockoutType(value);
  }

  std::string ControllerOutdoorAir::getMinimumLimitType() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getMinimumLimitType();
  }

  bool ControllerOutdoorAir::setMinimumLimitType(const std::string& value) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumLimitType(value);
  }

  boost::optional<bool> ControllerOutdoorAir::getHighHumidityControl() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getHighHumidityControl();
  }

  bool ControllerOutdoorAir::setHighHumidityControl(bool val) {
    DEPRECATED_AT_MSG(3, 8, 0, "Use setHumidistatControlZone instead.");
    return false;
  }

  boost::optional<ThermalZone> ControllerOutdoorAir::humidistatControlZone() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->humidistatControlZone();
  }

  bool ControllerOutdoorAir::setHumidistatControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setHumidistatControlZone(thermalZone);
  }

  void ControllerOutdoorAir::resetHumidistatControlZone() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetHumidistatControlZone();
  }

  double ControllerOutdoorAir::getHighHumidityOutdoorAirFlowRatio() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getHighHumidityOutdoorAirFlowRatio();
  }

  bool ControllerOutdoorAir::setHighHumidityOutdoorAirFlowRatio(double v) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setHighHumidityOutdoorAirFlowRatio(v);
  }

  bool ControllerOutdoorAir::getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio();
  }

  bool ControllerOutdoorAir::setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(v);
  }

  boost::optional<std::string> ControllerOutdoorAir::getHeatRecoveryBypassControlType() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getHeatRecoveryBypassControlType();
  }

  bool ControllerOutdoorAir::setHeatRecoveryBypassControlType(const std::string& v) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setHeatRecoveryBypassControlType(v);
  }

  std::string ControllerOutdoorAir::economizerOperationStaging() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->economizerOperationStaging();
  }

  bool ControllerOutdoorAir::setEconomizerOperationStaging(const std::string& v) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerOperationStaging(v);
  }

  IddObjectType ControllerOutdoorAir::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Controller_OutdoorAir);
    return result;
  }

  boost::optional<double> ControllerOutdoorAir::minimumOutdoorAirFlowRate() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumOutdoorAirFlowRate();
  }

  bool ControllerOutdoorAir::isMinimumOutdoorAirFlowRateAutosized() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->isMinimumOutdoorAirFlowRateAutosized();
  }

  boost::optional<double> ControllerOutdoorAir::maximumOutdoorAirFlowRate() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->maximumOutdoorAirFlowRate();
  }

  bool ControllerOutdoorAir::isMaximumOutdoorAirFlowRateAutosized() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->isMaximumOutdoorAirFlowRateAutosized();
  }

  bool ControllerOutdoorAir::setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumOutdoorAirFlowRate(minimumOutdoorAirFlowRate);
  }

  void ControllerOutdoorAir::autosizeMinimumOutdoorAirFlowRate() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->autosizeMinimumOutdoorAirFlowRate();
  }

  bool ControllerOutdoorAir::setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMaximumOutdoorAirFlowRate(maximumOutdoorAirFlowRate);
  }

  void ControllerOutdoorAir::autosizeMaximumOutdoorAirFlowRate() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->autosizeMaximumOutdoorAirFlowRate();
  }

  ControllerMechanicalVentilation ControllerOutdoorAir::controllerMechanicalVentilation() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->controllerMechanicalVentilation();
  }

  bool ControllerOutdoorAir::setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setControllerMechanicalVentilation(controllerMechanicalVentilation);
  }

  std::vector<openstudio::IdfObject> ControllerOutdoorAir::remove() {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->remove();
  }

  boost::optional<Schedule> ControllerOutdoorAir::minimumOutdoorAirSchedule() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumOutdoorAirSchedule();
  }

  bool ControllerOutdoorAir::setMinimumOutdoorAirSchedule(Schedule& schedule) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumOutdoorAirSchedule(schedule);
  }

  void ControllerOutdoorAir::resetMinimumOutdoorAirSchedule() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetMinimumOutdoorAirSchedule();
  }

  boost::optional<Schedule> ControllerOutdoorAir::minimumFractionofOutdoorAirSchedule() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumFractionofOutdoorAirSchedule();
  }

  bool ControllerOutdoorAir::setMinimumFractionofOutdoorAirSchedule(Schedule& schedule) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumFractionofOutdoorAirSchedule(schedule);
  }

  void ControllerOutdoorAir::resetMinimumFractionofOutdoorAirSchedule() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetMinimumFractionofOutdoorAirSchedule();
  }

  boost::optional<Schedule> ControllerOutdoorAir::maximumFractionofOutdoorAirSchedule() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->maximumFractionofOutdoorAirSchedule();
  }

  bool ControllerOutdoorAir::setMaximumFractionofOutdoorAirSchedule(Schedule& schedule) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setMaximumFractionofOutdoorAirSchedule(schedule);
  }

  void ControllerOutdoorAir::resetMaximumFractionofOutdoorAirSchedule() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetMaximumFractionofOutdoorAirSchedule();
  }

  boost::optional<Schedule> ControllerOutdoorAir::timeofDayEconomizerControlSchedule() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->timeofDayEconomizerControlSchedule();
  }

  bool ControllerOutdoorAir::setTimeofDayEconomizerControlSchedule(Schedule& schedule) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->setTimeofDayEconomizerControlSchedule(schedule);
  }

  void ControllerOutdoorAir::resetTimeofDayEconomizerControlSchedule() {
    getImpl<detail::ControllerOutdoorAir_Impl>()->resetTimeofDayEconomizerControlSchedule();
  }

  AirflowNetworkOutdoorAirflow ControllerOutdoorAir::getAirflowNetworkOutdoorAirflow(const AirflowNetworkCrack& crack) {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->getAirflowNetworkOutdoorAirflow(crack);
  }

  boost::optional<AirflowNetworkOutdoorAirflow> ControllerOutdoorAir::airflowNetworkOutdoorAirflow() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->airflowNetworkOutdoorAirflow();
  }

  boost::optional<double> ControllerOutdoorAir::autosizedMinimumOutdoorAirFlowRate() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->autosizedMinimumOutdoorAirFlowRate();
  }

  boost::optional<double> ControllerOutdoorAir::autosizedMaximumOutdoorAirFlowRate() const {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->autosizedMaximumOutdoorAirFlowRate();
  }

  void ControllerOutdoorAir::autosize() {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->autosize();
  }

  void ControllerOutdoorAir::applySizingValues() {
    return getImpl<detail::ControllerOutdoorAir_Impl>()->applySizingValues();
  }

}  // namespace model
}  // namespace openstudio
