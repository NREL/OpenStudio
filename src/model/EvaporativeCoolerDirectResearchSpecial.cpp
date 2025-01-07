/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "EvaporativeCoolerDirectResearchSpecial.hpp"
#include "EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"

// containing ZoneHVAC Component
#include "ZoneHVACEvaporativeCoolerUnit.hpp"
#include "ZoneHVACEvaporativeCoolerUnit_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EvaporativeCoolerDirectResearchSpecial::iddObjectType());
    }

    EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EvaporativeCoolerDirectResearchSpecial::iddObjectType());
    }

    EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(const EvaporativeCoolerDirectResearchSpecial_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EvaporativeCoolerDirectResearchSpecial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Evaporative Cooler Electricity Rate", "Evaporative Cooler Stage Effectiveness", "Evaporative Cooler Electricity Energy",
        "Evaporative Cooler Water Volume", "Evaporative Cooler Mains Water Volume"

        // Doc doesn't specify, but 95% sure this is only
        // If Supply Water Storage Tank Name is specified:
        // TODO: if storage tank gets implemented
        //"Evaporative Cooler Storage Tank Water Volume",
        //"Evaporative Cooler Starved Water Volume",
        //"Evaporative Cooler Starved Mains Water Volume"
      };
      return result;
    }

    IddObjectType EvaporativeCoolerDirectResearchSpecial_Impl::iddObjectType() const {
      return EvaporativeCoolerDirectResearchSpecial::iddObjectType();
    }

    std::vector<ScheduleTypeKey> EvaporativeCoolerDirectResearchSpecial_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("EvaporativeCoolerDirectResearchSpecial", "Availability"));
      }
      return result;
    }

    unsigned EvaporativeCoolerDirectResearchSpecial_Impl::inletPort() const {
      return OS_EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName;
    }

    unsigned EvaporativeCoolerDirectResearchSpecial_Impl::outletPort() const {
      return OS_EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName;
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (!airLoop->demandComponent(node.handle())) {
          if (StraightComponent_Impl::addToNode(node)) {
            if (boost::optional<Node> node = this->outletModelObject()->optionalCast<Node>()) {
              this->setSensorNode(node.get());
            }
            return true;
          }
        }
      }

      if (auto oa = node.airLoopHVACOutdoorAirSystem()) {
        if (StraightComponent_Impl::addToNode(node)) {
          if (auto _node = outletModelObject()->optionalCast<Node>()) {
            setSensorNode(_node.get());
          }
          return true;
        }
      }

      return false;
    }

    std::vector<openstudio::IdfObject> EvaporativeCoolerDirectResearchSpecial_Impl::remove() {
      return StraightComponent_Impl::remove();
    }

    Schedule EvaporativeCoolerDirectResearchSpecial_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<EvaporativeCoolerDirectResearchSpecial_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ZoneHVACComponent> EvaporativeCoolerDirectResearchSpecial_Impl::containingZoneHVACComponent() const {

      std::vector<ZoneHVACComponent> zoneHVACComponent = this->model().getModelObjects<ZoneHVACComponent>();
      for (const auto& elem : zoneHVACComponent) {
        switch (elem.iddObject().type().value()) {
          case openstudio::IddObjectType::OS_ZoneHVAC_EvaporativeCoolerUnit: {
            auto component = elem.cast<ZoneHVACEvaporativeCoolerUnit>();
            if (component.firstEvaporativeCooler().handle() == this->handle()) {
              return elem;
            }
            // I guess it's fine since this is optional anyways
            // } else if (auto comp_ = component.secondEvaporativeCooler(); comp_ && comp_->handle() == this->handle()) { return elem; }
            break;
          }
          default: {
            break;
          }
        }
      }
      return boost::none;
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName, "EvaporativeCoolerDirectResearchSpecial",
                                "Availability", schedule);
      return result;
    }

    double EvaporativeCoolerDirectResearchSpecial_Impl::coolerDesignEffectiveness() const {
      return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::CoolerDesignEffectiveness).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setCoolerDesignEffectiveness(double value) {
      return this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::CoolerDesignEffectiveness, value);
    }

    boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::recirculatingWaterPumpPowerConsumption() const {
      return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption);
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setRecirculatingWaterPumpPowerConsumption(double value) {
      return this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption, value);
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::autosizeRecirculatingWaterPumpPowerConsumption() {
      this->setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption, "Autosize");
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::isRecirculatingWaterPumpPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::primaryAirDesignFlowRate() const {
      return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate);
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setPrimaryAirDesignFlowRate(double value) {
      return this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, value);
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::autosizePrimaryAirDesignFlowRate() {
      this->setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, "Autosize");
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::isPrimaryAirDesignFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Node> EvaporativeCoolerDirectResearchSpecial_Impl::sensorNode() const {
      boost::optional<WorkspaceObject> wo = this->getTarget(OS_EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName);

      if (wo) {
        return wo->optionalCast<Node>();
      } else {
        return {};
      }
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setSensorNode(const Node& node) {
      return this->setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName, node.handle());
    }

    double EvaporativeCoolerDirectResearchSpecial_Impl::driftLossFraction() const {
      return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setDriftLossFraction(double value) {
      return this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction, value);
    }

    double EvaporativeCoolerDirectResearchSpecial_Impl::blowdownConcentrationRatio() const {
      return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setBlowdownConcentrationRatio(double value) {
      return this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio, value);
    }

    boost::optional<ModelObject> EvaporativeCoolerDirectResearchSpecial_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial_Impl::effectivenessFlowRatioModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName);
    }

    double EvaporativeCoolerDirectResearchSpecial_Impl::waterPumpPowerSizingFactor() const {
      boost::optional<double> value = getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial_Impl::waterPumpPowerModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName);
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setEffectivenessFlowRatioModifierCurve(const Curve& curve) {
      return setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName, curve.handle());
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::resetEffectivenessFlowRatioModifierCurve() {
      bool result = setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor) {
      bool result = setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor, waterPumpPowerSizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setWaterPumpPowerModifierCurve(const Curve& curve) {
      return setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName, curve.handle());
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::resetWaterPumpPowerModifierCurve() {
      bool result = setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName, "");
      OS_ASSERT(result);
    }

    // Evaporative Operation Minimum Drybulb Temperature
    double EvaporativeCoolerDirectResearchSpecial_Impl::evaporativeOperationMinimumDrybulbTemperature() const {
      return getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setEvaporativeOperationMinimumDrybulbTemperature(
      double evaporativeOperationMinimumDrybulbTemperature) {
      bool result = setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature,
                              evaporativeOperationMinimumDrybulbTemperature);
      return result;
    }

    // Evaporative Operation Maximum Limit Wetbulb Temperature
    double EvaporativeCoolerDirectResearchSpecial_Impl::evaporativeOperationMaximumLimitWetbulbTemperature() const {
      return getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setEvaporativeOperationMaximumLimitWetbulbTemperature(
      double evaporativeOperationMaximumLimitWetbulbTemperature) {
      bool result = setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature,
                              evaporativeOperationMaximumLimitWetbulbTemperature);
      return result;
    }

    // Evaporative Operation Maximum Limit Drybulb Temperature
    double EvaporativeCoolerDirectResearchSpecial_Impl::evaporativeOperationMaximumLimitDrybulbTemperature() const {
      return getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature).get();
    }

    bool EvaporativeCoolerDirectResearchSpecial_Impl::setEvaporativeOperationMaximumLimitDrybulbTemperature(
      double evaporativeOperationMaximumLimitDrybulbTemperature) {
      bool result = setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature,
                              evaporativeOperationMaximumLimitDrybulbTemperature);
      return result;
    }

    boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::autosizedRecirculatingWaterPumpPowerConsumption() const {
      return getAutosizedValue("Recirculating Pump Power", "W");
    }

    boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::autosizedPrimaryAirDesignFlowRate() const {
      return getAutosizedValue("Primary Air Design Flow Rate", "m3/s");
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::autosize() {
      autosizeRecirculatingWaterPumpPowerConsumption();
      autosizePrimaryAirDesignFlowRate();
    }

    void EvaporativeCoolerDirectResearchSpecial_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRecirculatingWaterPumpPowerConsumption();
      if (val) {
        setRecirculatingWaterPumpPowerConsumption(val.get());
      }

      val = autosizedPrimaryAirDesignFlowRate();
      if (val) {
        setPrimaryAirDesignFlowRate(val.get());
      }
    }

    ComponentType EvaporativeCoolerDirectResearchSpecial_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> EvaporativeCoolerDirectResearchSpecial_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> EvaporativeCoolerDirectResearchSpecial_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> EvaporativeCoolerDirectResearchSpecial_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  // create a new EvaporativeCoolerDirectResearchSpecial object in the model's workspace
  EvaporativeCoolerDirectResearchSpecial::EvaporativeCoolerDirectResearchSpecial(const Model& model, Schedule& schedule)
    : StraightComponent(EvaporativeCoolerDirectResearchSpecial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>());

    this->setAvailabilitySchedule(schedule);

    setCoolerDesignEffectiveness(1.0);

    setRecirculatingWaterPumpPowerConsumption(0.0);

    setDriftLossFraction(0.0);

    setBlowdownConcentrationRatio(0.0);

    // TODO: JM 2019-02-25: This input seems wrong given the current E+ IDD notes / IO documentation.
    // This isn't a regular sizing factor, but something in W/(m3/s), with a default of 90 and a range of 55 to 150.0
    // @kbenne for review please
    setWaterPumpPowerSizingFactor(0.1);

    // E+ suggested values
    setEvaporativeOperationMinimumDrybulbTemperature(16);
    setEvaporativeOperationMaximumLimitWetbulbTemperature(24);
    setEvaporativeOperationMaximumLimitDrybulbTemperature(28);
  }

  Schedule EvaporativeCoolerDirectResearchSpecial::availabilitySchedule() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->availabilitySchedule();
  }

  Schedule EvaporativeCoolerDirectResearchSpecial::availableSchedule() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->availabilitySchedule();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool EvaporativeCoolerDirectResearchSpecial::setAvailableSchedule(Schedule& schedule) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setAvailabilitySchedule(schedule);
  }

  double EvaporativeCoolerDirectResearchSpecial::coolerDesignEffectiveness() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->coolerDesignEffectiveness();
  }

  double EvaporativeCoolerDirectResearchSpecial::coolerEffectiveness() const {
    LOG(Info, "This method is deprecated, please replace it with coolerDesignEffectiveness.");
    return coolerDesignEffectiveness();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setCoolerDesignEffectiveness(double value) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setCoolerDesignEffectiveness(value);
  }

  bool EvaporativeCoolerDirectResearchSpecial::setCoolerEffectiveness(double value) {
    LOG(Info, "This method is deprecated, please replace it with setCoolerDesignEffectiveness.");
    return setCoolerDesignEffectiveness(value);
  }

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial::recirculatingWaterPumpPowerConsumption() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->recirculatingWaterPumpPowerConsumption();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setRecirculatingWaterPumpPowerConsumption(double value) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setRecirculatingWaterPumpPowerConsumption(value);
  }

  void EvaporativeCoolerDirectResearchSpecial::autosizeRecirculatingWaterPumpPowerConsumption() {
    getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizeRecirculatingWaterPumpPowerConsumption();
  }

  bool EvaporativeCoolerDirectResearchSpecial::isRecirculatingWaterPumpPowerConsumptionAutosized() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->isRecirculatingWaterPumpPowerConsumptionAutosized();
  }

  boost::optional<Node> EvaporativeCoolerDirectResearchSpecial::sensorNode() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->sensorNode();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setSensorNode(const Node& node) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setSensorNode(node);
  }

  double EvaporativeCoolerDirectResearchSpecial::driftLossFraction() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->driftLossFraction();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setDriftLossFraction(double value) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setDriftLossFraction(value);
  }

  double EvaporativeCoolerDirectResearchSpecial::blowdownConcentrationRatio() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->blowdownConcentrationRatio();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setBlowdownConcentrationRatio(double value) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setBlowdownConcentrationRatio(value);
  }

  boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial::effectivenessFlowRatioModifierCurve() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->effectivenessFlowRatioModifierCurve();
  }

  double EvaporativeCoolerDirectResearchSpecial::waterPumpPowerSizingFactor() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->waterPumpPowerSizingFactor();
  }

  boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial::waterPumpPowerModifierCurve() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->waterPumpPowerModifierCurve();
  }
  bool EvaporativeCoolerDirectResearchSpecial::setEffectivenessFlowRatioModifierCurve(const Curve& curve) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setEffectivenessFlowRatioModifierCurve(curve);
  }

  void EvaporativeCoolerDirectResearchSpecial::resetEffectivenessFlowRatioModifierCurve() {
    getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->resetEffectivenessFlowRatioModifierCurve();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setWaterPumpPowerSizingFactor(waterPumpPowerSizingFactor);
  }

  bool EvaporativeCoolerDirectResearchSpecial::setWaterPumpPowerModifierCurve(const Curve& curve) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setWaterPumpPowerModifierCurve(curve);
  }

  void EvaporativeCoolerDirectResearchSpecial::resetWaterPumpPowerModifierCurve() {
    getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->resetWaterPumpPowerModifierCurve();
  }

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial::primaryAirDesignFlowRate() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->primaryAirDesignFlowRate();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setPrimaryAirDesignFlowRate(double value) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setPrimaryAirDesignFlowRate(value);
  }

  void EvaporativeCoolerDirectResearchSpecial::autosizePrimaryAirDesignFlowRate() {
    getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizePrimaryAirDesignFlowRate();
  }

  bool EvaporativeCoolerDirectResearchSpecial::isPrimaryAirDesignFlowRateAutosized() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->isPrimaryAirDesignFlowRateAutosized();
  }

  // Evaporative Operation Minimum Drybulb Temperature
  double EvaporativeCoolerDirectResearchSpecial::evaporativeOperationMinimumDrybulbTemperature() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->evaporativeOperationMinimumDrybulbTemperature();
  }

  bool
    EvaporativeCoolerDirectResearchSpecial::setEvaporativeOperationMinimumDrybulbTemperature(double evaporativeOperationMinimumDrybulbTemperature) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setEvaporativeOperationMinimumDrybulbTemperature(
      evaporativeOperationMinimumDrybulbTemperature);
  }

  // Evaporative Operation Maximum Limit Wetbulb Temperature
  double EvaporativeCoolerDirectResearchSpecial::evaporativeOperationMaximumLimitWetbulbTemperature() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->evaporativeOperationMaximumLimitWetbulbTemperature();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setEvaporativeOperationMaximumLimitWetbulbTemperature(
    double evaporativeOperationMaximumLimitWetbulbTemperature) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setEvaporativeOperationMaximumLimitWetbulbTemperature(
      evaporativeOperationMaximumLimitWetbulbTemperature);
  }

  // Evaporative Operation Maximum Limit Drybulb Temperature
  double EvaporativeCoolerDirectResearchSpecial::evaporativeOperationMaximumLimitDrybulbTemperature() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->evaporativeOperationMaximumLimitDrybulbTemperature();
  }

  bool EvaporativeCoolerDirectResearchSpecial::setEvaporativeOperationMaximumLimitDrybulbTemperature(
    double evaporativeOperationMaximumLimitDrybulbTemperature) {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setEvaporativeOperationMaximumLimitDrybulbTemperature(
      evaporativeOperationMaximumLimitDrybulbTemperature);
  }

  EvaporativeCoolerDirectResearchSpecial::EvaporativeCoolerDirectResearchSpecial(
    std::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> p)
    : StraightComponent(std::move(p)) {}

  IddObjectType EvaporativeCoolerDirectResearchSpecial::iddObjectType() {
    IddObjectType result(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
    return result;
  }

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial::autosizedRecirculatingWaterPumpPowerConsumption() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizedRecirculatingWaterPumpPowerConsumption();
  }

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial::autosizedPrimaryAirDesignFlowRate() const {
    return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizedPrimaryAirDesignFlowRate();
  }

}  // namespace model
}  // namespace openstudio
